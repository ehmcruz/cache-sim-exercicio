#ifndef __CACHE_SIM_MEMORY_HIERARCHY_HEADER_H__
#define __CACHE_SIM_MEMORY_HIERARCHY_HEADER_H__

#include <string>
#include <string_view>

#include <cstdint>

#include <my-lib/matrix.h>

namespace Sim
{

// ----------------------------------------------------

enum class AccessType {
	Read,
	Write
};

using Addr = uint32_t;
using Time = uint64_t;

// ----------------------------------------------------

inline constexpr uint32_t addr_bits = sizeof(Addr) * 8;

// ----------------------------------------------------

Time get_current_time ();

// ----------------------------------------------------

class HierarchyElement
{
public:
	enum class Type {
		MainMemory,
		Cache
	};
protected:
	const Type type;
	const std::string name;
	HierarchyElement *next_level = nullptr;

public:
	HierarchyElement (const Type type_, const std::string_view name_)
		: type{type_}, name(name_)
	{

	}

	virtual uint32_t access (const AccessType type, const Addr addr) = 0;
	virtual void print_stats () const = 0;

	Type get_type () const
	{
		return this->type;
	}

	void set_next_level (HierarchyElement *next_level)
	{
		this->next_level = next_level;
	}

	HierarchyElement* get_next_level () const
	{
		return this->next_level;
	}
};

// ----------------------------------------------------

enum class WritePolicy : uint32_t {
	WriteThrough,
	WriteBack
};

const char* WritePolicy_str (const WritePolicy value);

enum class SlotState {
	Free,
	Used,
	Dirty
};

class Cache : public HierarchyElement
{
private:
	const uint32_t n_sets;
	const uint32_t assoc;
	const uint32_t line_size;
	const uint32_t latency;
	const WritePolicy write_policy;

	const uint32_t set_bits;
	const uint32_t line_bits;
	const uint32_t tag_bits;

	struct Slot {
		Addr tag;
		Time last_access_time;
		SlotState state = SlotState::Free;
	};

	Mylib::Matrix<Slot, true> matrix;

	uint64_t stats_access_read = 0;
	uint64_t stats_access_write = 0;
	uint64_t stats_misses = 0;

public:
	Cache (const std::string_view name_, const uint32_t n_sets_, const uint32_t assoc_, const uint32_t line_size_, const uint32_t latency_, const WritePolicy write_policy_);
	~Cache ();

	uint32_t access (const AccessType type, const Addr addr) final override;
	void print_stats () const final override;

	uint32_t size () const
	{
		return this->n_sets * this->assoc * this->line_size;
	}

private:
	Addr generate_addr (const Addr tag, const Addr set) const
	{
		return (tag << (this->set_bits + this->line_bits)) | (set << this->line_bits);
	}
};

// ----------------------------------------------------

class MainMemory : public HierarchyElement
{
private:
	uint32_t latency;

	uint64_t stats_access_read = 0;
	uint64_t stats_access_write = 0;
public:
	MainMemory (const std::string_view name_, const uint32_t latency_);

	uint32_t access (const AccessType type, const Addr addr) final override;
	void print_stats () const final override;
};

// ----------------------------------------------------

}

#endif