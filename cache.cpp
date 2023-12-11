#include "memory-hierarchy.h"

#include <my-lib/bit.h>

namespace Sim
{

// ----------------------------------------------------

// Função deve retornar a latência em ciclos do acesso.
// A latência retornada deverá ser somada à do nível seguinte da hierarquia em caso de cache miss.

uint32_t Cache::access (const AccessType type, const Addr addr)
{
		if (type == AccessType::Read)
		this->stats_access_read++;
	else
		this->stats_access_write++;

	uint32_t lat = this->latency;

	const Addr set = Mylib::extract_bits(addr, this->line_bits, this->set_bits);
	const Addr tag = Mylib::extract_bits(addr, this->line_bits + this->set_bits, this->tag_bits);

	// first, check if we have a cache hit

	for (uint32_t i = 0; i < this->assoc; i++) {
		Line& line = this->matrix(set, i);

		if (line.tag == tag && line.state != LineState::Free) {
			line.last_access_time = get_current_time();

			if (type == AccessType::Write) {
				if (this->write_policy == WritePolicy::WriteThrough)
					lat += this->next_level->access(AccessType::Write, addr);
				else
					line.state = LineState::Dirty;
			}
			
			return lat;
		}
	}

	// if we arrive here, we have a cache miss

	this->stats_misses++;

	// first, we fetch the data from lower level

	lat += this->next_level->access(type, addr);

	// now, we need to find an element to insert in the cache
	Line *repl_line = nullptr;

	// first, check if there are any free lines

	for (uint32_t i = 0; i < this->assoc; i++) {
		Line& line = this->matrix(set, i);

		if (line.state == LineState::Free)
			repl_line = &line;
	}

	if (repl_line == nullptr) { // no free lines
		// we need to find a line to replace

		repl_line = &this->matrix(set, 0);
		for (uint32_t i = 1; i < this->assoc; i++) {
			Line& line = this->matrix(set, i);

			if (line.last_access_time < repl_line->last_access_time)
				repl_line = &line;
		}

		// check if it is dirty

		if (repl_line->state == LineState::Dirty)
			lat += this->next_level->access(AccessType::Write, this->generate_addr(repl_line->tag, set));

		repl_line->state = LineState::Free;
	}

	// repl_line now has the line to be replaced

	repl_line->tag = tag;
	repl_line->last_access_time = get_current_time();

	if (type == AccessType::Write) {
		if (this->write_policy == WritePolicy::WriteThrough) {
			repl_line->state = LineState::Used;
			lat += this->next_level->access(AccessType::Write, addr);
		}
		else
			repl_line->state = LineState::Dirty;
	}
	else
		repl_line->state = LineState::Used;

	return lat;
}

// ----------------------------------------------------

}