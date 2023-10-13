#include "access-generator.h"

std::vector<MemoryAccess> generate_accesses_linear (const uint32_t n, const uint32_t buffer_size)
{
	std::vector<MemoryAccess> accesses;

	accesses.reserve(n);

	for (uint32_t i = 0; i < n; i++)
		accesses.push_back(i %  buffer_size);

	return accesses;
}
