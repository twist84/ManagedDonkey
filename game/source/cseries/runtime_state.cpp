#include "cseries/runtime_state.hpp"

#include "cache/physical_memory_map.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/crc.hpp"
#include "memory/data.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x0189D394, c_allocation_base*, g_runtime_state_allocation);
REFERENCE_DECLARE(0x0189D3C0, c_runtime_state_allocation, g_actual_runtime_state_allocation);
REFERENCE_DECLARE(0x022C0108, s_runtime_state_globals, g_runtime_state_globals);

HOOK_DECLARE(0x00509D30, runtime_state_data_new);
//HOOK_DECLARE(0x00509DC0, runtime_state_dispose);
//HOOK_DECLARE(0x00509DD0, runtime_state_dispose_from_old_map);
HOOK_DECLARE(0x00509DE0, runtime_state_get_buffer_address);
//HOOK_DECLARE(0x00509E00, runtime_state_initialize);
//HOOK_DECLARE(0x00509E10, runtime_state_initialize_for_new_map);
HOOK_DECLARE(0x00509E20, runtime_state_malloc_aligned);
HOOK_DECLARE(0x00509EA0, runtime_state_malloc);
HOOK_DECLARE(0x00509F00, runtime_state_shell_dispose);
HOOK_DECLARE(0x00509F30, runtime_state_shell_initialize);
HOOK_DECLARE(0x0065C940, runtime_state_allocate_buffer);
HOOK_DECLARE(0x0065C960, runtime_state_free_buffer);

//.text:00509C40 ; c_runtime_state_allocation::c_runtime_state_allocation
//.text:00509C50
//.text:00509C80
//.text:00509CB0 ; c_runtime_state_allocation::allocate
//.text:00509D10 ; c_runtime_state_allocation::deallocate
//.text:00509D20

s_data_array* __cdecl runtime_state_data_new(const char* name, int32 maximum_count, int32 size)
{
	//return INVOKE(0x00509D30, runtime_state_data_new, name, maximum_count, size);

	s_data_array* data = (s_data_array*)runtime_state_malloc(name, "data array", data_allocation_size(maximum_count, size, 0));
	data_initialize(data, name, maximum_count, size, 0, g_no_allocation);
	return data;
}

void __cdecl runtime_state_dispose()
{
	//INVOKE(0x00509DC0, runtime_state_dispose);
}

void __cdecl runtime_state_dispose_from_old_map()
{
	//INVOKE(0x00509DD0, runtime_state_dispose_from_old_map);
}

const void* __cdecl runtime_state_get_buffer_address(int32* buffer_size)
{
	//return INVOKE(0x00509DE0, runtime_state_get_buffer_address, buffer_size);

	ASSERT(buffer_size);

	*buffer_size = g_runtime_state_globals.allocated_size;
	return g_runtime_state_globals.base_address;
}

void __cdecl runtime_state_initialize()
{
	//INVOKE(0x00509E00, runtime_state_initialize);

	ASSERT(g_runtime_state_globals.base_address);
}

void __cdecl runtime_state_initialize_for_new_map()
{
	//INVOKE(0x00509E10, runtime_state_initialize_for_new_map);

	g_runtime_state_globals.locked = true;
}

void* __cdecl runtime_state_malloc_aligned(const char* name, const char* type, int32 size, int32 alignment_bits)
{
	//return INVOKE(0x00509E20, runtime_state_malloc_aligned, name, type, size, alignment_bits);

	ASSERT(!g_runtime_state_globals.locked);

	void* allocation_end = offset_pointer(g_runtime_state_globals.base_address, g_runtime_state_globals.allocated_size);
	void* allocation_end_aligned = align_pointer(allocation_end, alignment_bits);
	int32 true_size = size + pointer_difference(allocation_end, allocation_end_aligned);

	ASSERT(g_runtime_state_globals.allocated_size + true_size <= k_runtime_state_size);
	g_runtime_state_globals.allocated_size += true_size;

	event(_event_message, "system:runtime_state: runtime_state_malloc %s: %d (total: %d)", name, true_size, g_runtime_state_globals.allocated_size);

	g_runtime_state_globals.size_checksum = crc_checksum_buffer(g_runtime_state_globals.size_checksum, (byte*)&true_size, sizeof(true_size));

	return allocation_end_aligned;
}

void* __cdecl runtime_state_malloc(const char* name, const char* type, int32 size)
{
	//return INVOKE(0x00509EA0, runtime_state_malloc, name, type, size);

	return runtime_state_malloc_aligned(name, type, size, 2);
}

void __cdecl runtime_state_shell_dispose()
{
	//INVOKE(0x00509F00, runtime_state_shell_dispose);

	if (g_runtime_state_globals.base_address)
	{
		//debug_track_nonheap_free(g_runtime_state_globals.base_address, __FILE__, __LINE__);
		runtime_state_free_buffer(g_runtime_state_globals.base_address);
		csmemset(&g_runtime_state_globals, 0, sizeof(g_runtime_state_globals));
	}
}

void __cdecl runtime_state_shell_initialize()
{
	//INVOKE(0x00509F30, runtime_state_shell_initialize);

	ASSERT(!g_runtime_state_globals.base_address);
	g_runtime_state_globals.base_address = runtime_state_allocate_buffer(k_runtime_state_size);
	ASSERT(g_runtime_state_globals.base_address);

	//debug_track_nonheap_alloc(g_runtime_state_globals.base_address, k_runtime_state_size, 0, __FILE__, __LINE__, NONE, "available runtime state", 0);

	g_runtime_state_globals.allocated_size = 0;
}

void* __cdecl runtime_state_allocate_buffer(int32 size)
{
	//return INVOKE(0x0065C940, runtime_state_allocate_buffer, size);

	//_physical_memory_malloc(_memory_stage_game_initialize, "runtime state (not game state)", 3, __FILE__, __LINE__);
	return _physical_memory_malloc(_memory_stage_game_initialize, "runtime state (not game state)", size, 3);
}

void __cdecl runtime_state_free_buffer(void* buffer)
{
	//INVOKE(0x0065C960, runtime_state_free_buffer, buffer);

	physical_memory_free(buffer);
}

