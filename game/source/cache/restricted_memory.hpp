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

	k_total_restricted_memory_regions,
	k_total_game_state_subsections = k_total_restricted_memory_regions,

	k_game_state_shared_region_mirror0 = k_game_state_shared_region + 1,
	k_game_state_shared_region_mirror1 = k_game_state_shared_region + 2,

	k_game_state_header_region_size = 0x30000,
	k_game_state_update_region_size = 0x200000,
	k_game_state_render_region_size = 0x2D0000,
	k_game_state_shared_region_size = 0x480000,

	k_game_state_persist_size = k_game_state_header_region_size + k_game_state_update_region_size + k_game_state_render_region_size + k_game_state_shared_region_size,
	k_game_state_cpu_size = k_game_state_persist_size + (k_game_state_shared_region_size * 2),
};
static_assert(0x0980000 == k_game_state_persist_size);
static_assert(0x1280000 == k_game_state_cpu_size);

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
		void(__cdecl* post_copy_fixup)(void*);
		void(__cdecl* pre_overwrite_fixup)(void*);
	};
	static_assert(sizeof(s_registered_member) == 0x18);

	c_interlocked_long m_mirror_read_in_progress;
	c_interlocked_long m_mirror_write_in_progress;
	c_static_array<s_mirror_slot, k_max_section_mirrors> m_mirrors;
	c_interlocked_long m_new_write_mirror;
	c_interlocked_long m_available_mirror;
	c_synchronized_long m_alias_allowed;
	c_synchronized_long m_alias_count;
	c_restricted_section* m_primary_section;
	dword_flags m_internal_flags; // 0: _initialized
	long __unknown60;
	dword m_subsection_size;
	dword m_reserved_size;
	long m_mirror_count;
	dword m_mirror_section;
	long m_registered_member_count;
	c_static_array<s_registered_member, k_max_registered_members> m_registered_member;
	c_restricted_memory_callbacks* m_callbacks;
	void* m_subsectionC7C;
	void* m_subsectionC80;
	long m_region_index;
};
static_assert(sizeof(c_restricted_memory) == 0xC88);

extern char const*(&g_restricted_region_names)[k_total_restricted_memory_regions];
extern c_restricted_section(&g_restricted_section)[k_total_restricted_memory_regions];
extern c_restricted_memory(&g_restricted_regions)[k_total_restricted_memory_regions];

