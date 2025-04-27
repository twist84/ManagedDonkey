#include "cache/pc_geometry_cache.hpp"

#include "cseries/cseries.hpp"

bool __cdecl _geometry_cache_block_query(geometry_block_info* block_info, int32 flags)
{
	return INVOKE(0x00618C70, _geometry_cache_block_query, block_info, flags);
}

void __cdecl geometry_cache_block_delete(geometry_block_info* block_info)
{
	INVOKE(0x00618C80, geometry_cache_block_delete, block_info);
}

void __cdecl geometry_cache_block_for_one_frame(e_geometry_block_type block_type)
{
	INVOKE(0x00618C90, geometry_cache_block_for_one_frame, block_type);
}

void __cdecl geometry_cache_block_new(geometry_block_info* block_info)
{
	INVOKE(0x00618CA0, geometry_cache_block_new, block_info);
}

void __cdecl geometry_cache_clear_structure_block_pending_count()
{
	INVOKE(0x00618CB0, geometry_cache_clear_structure_block_pending_count);
}

void __cdecl geometry_cache_close()
{
	INVOKE(0x00618CC0, geometry_cache_close);
}

void __cdecl geometry_cache_debug_render()
{
	INVOKE(0x00618CD0, geometry_cache_close);
}

void __cdecl geometry_cache_delete()
{
	INVOKE(0x00618CE0, geometry_cache_delete);
}

void __cdecl geometry_cache_flush()
{
	INVOKE(0x00618CF0, geometry_cache_flush);
}

void __cdecl geometry_cache_flush_tag(int32 owner_tag_index)
{
	INVOKE(0x00618D00, geometry_cache_flush_tag, owner_tag_index);
}

void __cdecl geometry_cache_get_base_address_and_size(void** base_address, int32* size)
{
	INVOKE(0x00618D10, geometry_cache_get_base_address_and_size, base_address, size);
}

int32 __cdecl geometry_cache_get_structure_block_pending_count()
{
	return INVOKE(0x00618D20, geometry_cache_get_structure_block_pending_count);
}

void __cdecl geometry_cache_idle()
{
	INVOKE(0x00618D30, geometry_cache_idle);
}

bool __cdecl geometry_cache_in_use()
{
	return INVOKE(0x00618D40, geometry_cache_in_use);
}

bool __cdecl geometry_cache_is_blocking()
{
	return INVOKE(0x00618D50, geometry_cache_is_blocking);
}

void __cdecl geometry_cache_memory_dispose()
{
	INVOKE(0x00618D60, geometry_cache_memory_dispose);
}

void __cdecl geometry_cache_memory_initialize(e_map_memory_configuration memory_configuration)
{
	INVOKE(0x00618D70, geometry_cache_memory_initialize, memory_configuration);
}

void __cdecl geometry_cache_new()
{
	INVOKE(0x00618D80, geometry_cache_new);
}

void __cdecl geometry_cache_open()
{
	INVOKE(0x00618D90, geometry_cache_open);
}

bool __cdecl geometry_cache_predict(geometry_block_info* block_info)
{
	return INVOKE(0x00618DA0, geometry_cache_predict, block_info);
}

void __cdecl geometry_cache_set_blocking(bool blocking)
{
	INVOKE(0x00618DB0, geometry_cache_set_blocking, blocking);
}

void __cdecl geometry_cache_update_for_render()
{
	INVOKE(0x00618DC0, geometry_cache_update_for_render);
}

