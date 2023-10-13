#include <iostream>
#include <string_view>

#include <cstdlib>

#include "pugixml.hpp"

#include "memory-hierarchy.h"
#include "accesses-generator.h"

namespace Sim
{

// ----------------------------------------------------

HierarchyElement *memory_hierarchy_top = nullptr;
std::vector<MemoryAccess> accesses;
Time current_time = 0;

// ----------------------------------------------------

// mem_access must return the number of cycles required for the access

uint32_t mem_access (const AccessType type, const uint32_t addr)
{
	return memory_hierarchy_top->access(type, addr);
}

void create_hierarchy (const std::string_view fname)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fname.data());

	MainMemory *main_memory = nullptr;

	if (result)
		std::cout << "XML [" << fname << "] parsed without errors]" << std::endl;
	else {
		std::cout << "XML [" << fname << "] parsed with errors" << std::endl;
		std::cout << "Load result: " << result.description() << std::endl;
		exit(1);
	}

	pugi::xml_node base_node = doc.child("MemoryHierarchy");
	bool is_first = true;

	for (pugi::xml_node element = base_node.child("element"); element; element = element.next_sibling("element")) {
		const char *type = element.attribute("type").value();
		std::cout << "Element " << type << std::endl;

		if (std::string_view(type) == "main_memory") {
			if (!is_first) {
				std::cout << "Error! The main memory must be the first one." << std::endl;
				exit(1);
			}
			if (main_memory) {
				std::cout << "Error! Only one main memory is allowed." << std::endl;
				exit(1);
			}

			const char *name = element.child_value("name");
			const char *str_latency = element.child_value("latency");

			main_memory = new MainMemory(name, atoi(str_latency));

			memory_hierarchy_top = main_memory;
		}
		else if (std::string_view(type) == "cache") {
			if (is_first) {
				std::cout << "Error! The main memory must be the first one." << std::endl;
				exit(1);
			}

			const char *name = element.child_value("name");
			const char *str_sets = element.child_value("sets");
			const char *str_assoc = element.child_value("assoc");
			const char *str_line_size = element.child_value("linesize");
			const char *str_writepolicy = element.child_value("writepolicy");
			const char *str_latency = element.child_value("latency");

			WritePolicy wp;
			
			if (std::string_view(str_writepolicy) == "write_through")
				wp = WritePolicy::WriteThrough;
			else if (std::string_view(str_writepolicy) == "write_back")
				wp = WritePolicy::WriteBack;
			else {
				std::cout << "Error! writepolicy must be write_through or write_back" << std::endl;
				exit(1);
			}

			Cache *cache = new Cache(name,
			                         atoi(str_sets),
									 atoi(str_assoc),
									 atoi(str_line_size),
									 atoi(str_latency),
									 wp);

			cache->set_next_level(memory_hierarchy_top);
			memory_hierarchy_top = cache;
		}

		is_first = false;
	}
}

void generate_accesses ()
{
	accesses = generate_accesses_linear(1000000, 64*1024, 4);
}

// ----------------------------------------------------

Time get_current_time ()
{
	return current_time;
}

void run ()
{
	for (const auto& access : accesses) {
		const uint32_t latency = mem_access(access.type, access.addr);
		current_time += latency;
	}

	std::cout << "total time required (cycles): " << current_time << std::endl;
}

void print_stats ()
{
	for (HierarchyElement *el = memory_hierarchy_top; el; el = el->get_next_level()) {
		std::cout << "-------------------------------" << std::endl;
		el->print_stats();
	}
}

// ----------------------------------------------------

} // end namespace Sim

int main (const int argc, const char **argv)
{
	if (argc != 2) {
		std::cout << "Command: " << argv[0] << " [xml_file]" << std::endl;
		exit(1);
	}

	Sim::create_hierarchy(argv[1]);
	Sim::generate_accesses();
	Sim::run();
	Sim::print_stats();

	return 0;
}