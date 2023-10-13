#include <iostream>

#include "memory-hierarchy.h"

namespace Sim
{

// ----------------------------------------------------

Cache::Cache (const std::string_view name_, const uint32_t n_sets_, const uint32_t assoc_, const uint32_t line_size_, const uint32_t latency_, const WritePolicy write_policy_)
	: name{name_},
	  n_sets{n_sets_},
	  assoc{assoc_},
	  line_size{line_size_},
	  latency{latency_},
	  write_policy{write_policy_},
	  set_bits{Mylib::base2_log_of_integer(n_sets_)},
	  line_bits{Mylib::base2_log_of_integer(line_size_)},
	  tag_bits{addr_bits - this->set_bits - this->line_bits}
{
	this->matrix = Mylib::Matrix(this->n_sets, this->assoc);
	std::cout << "created cache " << this->name << " size " << this->size() << std::endl;
}

MainMemory::MainMemory (const uint32_t latency_)
	: latency{latency_}
{

}

uint32_t MainMemory::access (const AccessType type, const Addr addr)
{
	if (type == AccessType::Read)
		this->stats_access_read++;
	else
		this->stats_access_write++;
	
	return this->latency;
}

// ----------------------------------------------------

}