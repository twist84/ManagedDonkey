#pragma once

#include "tag_files/tag_groups.hpp"

enum e_geometry_block_type
{
	_geometry_block_type_all = 0,
	_geometry_block_type_bsp_geometry,
	_geometry_block_type_screenshot
};

enum e_geometry_block_info_bit
{
	_geometry_block_cached_bit = 0,
	_geometry_block_uses_dynamic_cache_bit,
	_geometry_block_predicted_bit,

	k_number_of_geometry_block_info_bits
};

struct geometry_block_info
{
	long geometry_block_offset;
	long geometry_block_size;
	long section_data_size;
	long resource_data_size;
	s_tag_block resources;
	long owner_tag_index;
	short owner_tag_section_offset;
	bool runtime_section_data_valid;
	c_flags<enum e_geometry_block_info_bit, uint8, k_number_of_geometry_block_info_bits> flags;
	long runtime_cache_block_index;
};
static_assert(sizeof(geometry_block_info) == 0x28);

extern bool __cdecl _geometry_cache_block_query(geometry_block_info* block_info, long flags);
extern void __cdecl geometry_cache_block_delete(geometry_block_info* block_info);
extern void __cdecl geometry_cache_block_for_one_frame(e_geometry_block_type block_type);
extern void __cdecl geometry_cache_block_new(geometry_block_info* block_info);
extern void __cdecl geometry_cache_clear_structure_block_pending_count();
extern void __cdecl geometry_cache_close();
extern void __cdecl geometry_cache_debug_render();
extern void __cdecl geometry_cache_delete();
extern void __cdecl geometry_cache_flush();
extern void __cdecl geometry_cache_flush_tag(long owner_tag_index);
extern void __cdecl geometry_cache_get_base_address_and_size(void** base_address, long* size);
extern long __cdecl geometry_cache_get_structure_block_pending_count();
extern void __cdecl geometry_cache_idle();
extern bool __cdecl geometry_cache_in_use();
extern bool __cdecl geometry_cache_is_blocking();
extern void __cdecl geometry_cache_memory_dispose();
extern void __cdecl geometry_cache_memory_initialize(e_map_memory_configuration memory_configuration);
extern void __cdecl geometry_cache_new();
extern void __cdecl geometry_cache_open();
extern bool __cdecl geometry_cache_predict(geometry_block_info* block_info);
extern void __cdecl geometry_cache_set_blocking(bool blocking);
extern void __cdecl geometry_cache_update_for_render();

