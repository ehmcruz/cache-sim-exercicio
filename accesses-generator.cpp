#include <random>
#include <iostream>

#include "accesses-generator.h"

namespace Sim
{

// ----------------------------------------------------

std::vector<MemoryAccess> generate_accesses_sequential (const uint32_t n, const uint32_t buffer_size, const uint32_t stride)
{
	std::random_device rd;
	std::mt19937 rg(rd());
	std::uniform_int_distribution<uint32_t> distribution(0, 99);

	std::vector<MemoryAccess> accesses;

	std::cout << "generating " << n << " sequential accesses with buffer size " << buffer_size << " stride " << stride << std::endl;

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

std::vector<MemoryAccess> generate_accesses_random (const uint32_t n, const uint32_t buffer_size)
{
	std::random_device rd;
	std::mt19937 rg(rd());
	std::uniform_int_distribution<uint32_t> distribution_type(0, 99);
	std::uniform_int_distribution<uint32_t> distribution_addr(0, buffer_size);

	std::vector<MemoryAccess> accesses;

	std::cout << "generating " << n << " random accesses with buffer size " << buffer_size << std::endl;

	accesses.reserve(n);

	for (uint32_t i = 0; i < n; i++) {
		const uint32_t addr = distribution_addr(rg);

		AccessType type;

		if (distribution_type(rg) < 80)
			type = AccessType::Read;
		else
			type = AccessType::Write;

		accesses.push_back( MemoryAccess {
			.addr = addr,
			.type = type
			});
	}

	return accesses;
}

// ----------------------------------------------------

}
