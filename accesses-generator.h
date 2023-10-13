#ifndef __CACHE_SIM_ACCESSES_GENERATOR_HEADER_H__
#define __CACHE_SIM_ACCESSES_GENERATOR_HEADER_H__

#include <vector>

#include <cstdint>

#include "memory-hierarchy.h"

namespace Sim
{

// ----------------------------------------------------

struct MemoryAccess {
	Addr addr;
	AccessType type;
};

std::vector<MemoryAccess> generate_accesses_linear (const uint32_t n, const uint32_t buffer_size, const uint32_t stride);

// ----------------------------------------------------

}

#endif