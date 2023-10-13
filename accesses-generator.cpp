#include <random>

#include "accesses-generator.h"

namespace Sim
{

// ----------------------------------------------------

std::vector<MemoryAccess> generate_accesses_linear (const uint32_t n, const uint32_t buffer_size, const uint32_t stride)
{
	std::random_device rd;
	std::mt19937 rg(rd()); 
	std::uniform_int_distribution<uint32_t> distribution(0,99);

	std::vector<MemoryAccess> accesses;

	accesses.reserve(n);

	uint32_t addr = 0;

	for (uint32_t i = 0; i < n; i++) {
		if (addr >= buffer_size)
			addr = 0;

		AccessType type;

		if (distribution(rg) < 80)
			type = AccessType::Read;
		else
			type = AccessType::Write;

		accesses.push_back( MemoryAccess {
			.addr = addr,
			.type = type
			});

		addr += stride;
	}

	return accesses;
}

// ----------------------------------------------------

}
