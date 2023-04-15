#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/synchronized_value.hpp"

const long k_max_section_mirrors = 4;
const long k_max_registered_members = 128;

enum e_restricted_memory_region
{
	k_game_state_header_region = 0,
	k_game_state_update_region,
	k_game_state_render_region,
	k_game_state_shared_region,
	k_global_render_data_region,

	k_total_restricted_memory_regions
};

enum e_memory_protection;

struct c_restricted_section
{
	dword_flags m_internal_flags;
	long m_protection; // e_memory_protection
	long m_external_lock;
	byte* m_address;
	dword m_size;
	dword m_owner_thread;
};
static_assert(sizeof(c_restricted_section) == 0x18);

struct c_restricted_memory_callbacks;
struct c_restricted_memory
{
	struct s_mirror_slot
	{
		c_interlocked_long valid;
		c_synchronized_long readable_flag;
		c_synchronized_long __unknown8;
		c_restricted_section* restricted_section;
	};
	static_assert(sizeof(s_mirror_slot) == 0x10);

	struct s_registered_member
	{
		// same as `offset`
		dword __unknown0;

		dword offset;
		dword size;
		void(__cdecl* update_callback)(void*);
		void(__cdecl* unknown_callback1)(void*);
		void(__cdecl* unknown_callback2)(void*);
	};
	static_assert(sizeof(s_registered_member) == 0x18);

	c_interlocked_long m_mirror_read_in_progress;
	c_interlocked_long m_mirror_write_in_progress;
	s_mirror_slot m_mirrors[k_max_section_mirrors];
	c_interlocked_long __unknown48;
	c_interlocked_long __unknown4C;
	c_synchronized_long m_alias_allowed;
	c_synchronized_long m_alias_count;
	c_restricted_section* m_primary_section;
	dword_flags m_initialized;
	long __unknown60;
	dword m_subsection_size;
	dword m_reserved_size;
	long m_mirror_count;
	dword m_mirror_section;
	long m_registered_member_count;
	s_registered_member m_registered_member[k_max_registered_members];
	c_restricted_memory_callbacks* m_callbacks;
	void* __unknownC7C;
	void* __unknownC80;
	long m_region_index;
};
static_assert(sizeof(c_restricted_memory) == 0xC88);

struct c_restricted_memory_callbacks
{
public:
	virtual unsigned int filter_size_request(unsigned int size);
	virtual long filter_base_offset(long);
	virtual void handle_allocation(c_restricted_memory const* restricted_memory, char const* name, char const* type, long member_index, void* address, unsigned int size);
	virtual void handle_release(c_restricted_memory const* restricted_memory, long member_index, void* address, unsigned int size);
};

struct c_gamestate_deterministic_allocation_callbacks : c_restricted_memory_callbacks
{
public:
	virtual long filter_base_offset(long, long);
};

struct c_gamestate_nondeterministic_allocation_callbacks : c_restricted_memory_callbacks
{
public:
};

extern char const*(&g_restricted_region_names)[k_total_restricted_memory_regions];
extern c_restricted_memory(&g_restricted_regions)[k_total_restricted_memory_regions];

