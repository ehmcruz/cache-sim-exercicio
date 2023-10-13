#include <iostream>

#include "memory-hierarchy.h"

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

void create_hierarchy ()
{
	constexpr uint32_t line_size = 64;

	Cache *l1 = new Cache("L1", 64, 4, line_size, 1, WritePolicy::WriteBack);
	MainMemory *main_memory = new MainMemory(200);

	l1->set_next_level(main_memory);

	memory_hierarchy_top = l1;
}

void generate_accesses ()
{
	accesses = generate_accesses_linear(1000000, 64*1024);
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

// ----------------------------------------------------

} // end namespace Sim

int main (const int argc, const char **argv)
{
	Sim::create_hierarchy();
	Sim::generate_accesses();
	Sim::run();

	return 0;
}