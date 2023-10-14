#include <iostream>
#include <array>

#include <my-lib/math.h>
#include <my-lib/std.h>

#include "memory-hierarchy.h"

namespace Sim
{

// ----------------------------------------------------

const char* WritePolicy_str (const WritePolicy value)
{
	static constexpr auto strs = std::to_array<const char*>({
		"WriteThrough",
		"WriteBack"
		});

	mylib_assert_exception_msg(std::to_underlying(value) < strs.size(), "invalid WritePolicy value ", std::to_underlying(value))

	return strs[ std::to_underlying(value) ];
}

Cache::Cache (const std::string_view name_, const uint32_t n_sets_, const uint32_t assoc_, const uint32_t line_size_, const uint32_t latency_, const WritePolicy write_policy_)
	: HierarchyElement(Type::Cache, name_),
	  n_sets{n_sets_},
	  assoc{assoc_},
	  line_size{line_size_},
	  latency{latency_},
	  write_policy{write_policy_},
	  set_bits{Mylib::Math::base2_log_of_integer(n_sets_)},
	  line_bits{Mylib::Math::base2_log_of_integer(line_size_)},
	  tag_bits{addr_bits - this->set_bits - this->line_bits}
{
	this->matrix = Mylib::Matrix<Slot, true>(this->n_sets, this->assoc);

	std::cout << "created cache " << this->name
		<< " size " << this->size()
		<< " sets " << this->n_sets << " [set_bits " << this->set_bits << "]"
		<< " assoc " << this->assoc
		<< " line_size " << this->line_size << " [line_bits " << this->line_bits << "]"
		<< " [tag_bits " << this->tag_bits << "]"
		<< " latency " << this->latency
		<< " write_policy " << WritePolicy_str(this->write_policy)
		<< std::endl;
}

void Cache::print_stats () const
{
	std::cout << "Statitics of " << this->name << std::endl;
	
	std::cout << this->name << ".size " << this->size() << std::endl;

	std::cout << this->name << ".read_accesses " << this->stats_access_read << std::endl;
	std::cout << this->name << ".write_accesses " << this->stats_access_write << std::endl;

	const uint64_t total_accesses = this->stats_access_read + this->stats_access_write;
	std::cout << this->name << ".total_accesses " << total_accesses << std::endl;

	std::cout << this->name << ".misses " << this->stats_misses << std::endl;

	const double miss_ratio = (static_cast<double>(this->stats_misses) / static_cast<double>(total_accesses));
	std::cout << this->name << ".miss_ratio " << miss_ratio << std::endl;
}

// ----------------------------------------------------

MainMemory::MainMemory (const std::string_view name_, const uint32_t latency_)
	: HierarchyElement(Type::MainMemory, name_),
	  latency{latency_}
{
	std::cout << "created MainMemory named " << this->name << " latency " << this->latency << std::endl;
}

uint32_t MainMemory::access (const AccessType type, const Addr addr)
{
	if (type == AccessType::Read)
		this->stats_access_read++;
	else
		this->stats_access_write++;

	return this->latency;
}

void MainMemory::print_stats () const
{
	std::cout << "Statitics of " << this->name << std::endl;
	std::cout << this->name << ".read_accesses " << this->stats_access_read << std::endl;
	std::cout << this->name << ".write_accesses " << this->stats_access_write << std::endl;

	const uint64_t total_accesses = this->stats_access_read + this->stats_access_write;
	std::cout << this->name << ".total_accesses " << total_accesses << std::endl;
}

// ----------------------------------------------------

}