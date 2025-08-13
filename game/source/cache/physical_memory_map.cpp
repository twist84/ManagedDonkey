#include "cache/physical_memory_map.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE(0x0238EC50, s_physical_memory_globals, physical_memory_globals);
REFERENCE_DECLARE(0x0238ED04, void*, resources_buffer);
REFERENCE_DECLARE(0x0238ED08, void*, k_physical_memory_base_virtual_address);
REFERENCE_DECLARE(0x0238ED0C, void*, k_virtual_to_physical_base_offset);

HOOK_DECLARE(0x0051D180, _physical_memory_malloc);
HOOK_DECLARE(0x0051D5C0, physical_memory_free);
HOOK_DECLARE(0x0051DB10, physical_memory_resize_region_lock);

uns32 g_physical_memory_data_size_increase_mb = 0;
uns32 g_physical_memory_cache_size_increase_mb = 512 + (64 * 2);

uns32 g_physical_memory_data_size_increase_kb = 1024 * 1024 * g_physical_memory_data_size_increase_mb;
uns32 g_physical_memory_data_size_new = physical_memory_round_up_allocation_size(k_physical_memory_data_size + g_physical_memory_data_size_increase_kb);

uns32 g_physical_memory_cache_size_increase_kb = 1024 * 1024 * g_physical_memory_cache_size_increase_mb;
uns32 g_physical_memory_cache_size_new = physical_memory_round_up_allocation_size(k_physical_memory_cache_size + g_physical_memory_cache_size_increase_kb);

void recalculate_data_size_increase(int32 data_size_increase_mb)
{
	g_physical_memory_data_size_increase_mb = data_size_increase_mb;
	g_physical_memory_data_size_increase_kb = 1024 * 1024 * g_physical_memory_data_size_increase_mb;
	g_physical_memory_data_size_new = physical_memory_round_up_allocation_size(k_physical_memory_data_size + g_physical_memory_data_size_increase_kb);
}

void recalculate_cache_size_increase(int32 cache_size_increase_mb)
{
	g_physical_memory_cache_size_increase_mb = cache_size_increase_mb;
	g_physical_memory_cache_size_increase_kb = 1024 * 1024 * g_physical_memory_cache_size_increase_mb;
	g_physical_memory_cache_size_new = physical_memory_round_up_allocation_size(k_physical_memory_cache_size + g_physical_memory_cache_size_increase_kb);
}

const char* const k_physical_memory_stage_names[k_memory_stage_count]
{
	"initial",
	"game_initialize",
	"resize_initialize",
	"resize_available",
	"map_type_configuration",
	"level_initialize",
	"resize_locked",
	"in_level"
};
static_assert(NUMBEROF(k_physical_memory_stage_names) == k_memory_stage_count);

void* __cdecl _physical_memory_malloc(memory_stage stage, const char* name, int32 size, uns32 flags)
{
	return physical_memory_system_malloc(size, NULL);
}

uns32 __cdecl align_up(uns32 value, int32 alignment_bits)
{
	return INVOKE(0x0051D280, align_up, value, alignment_bits);

	//return (value + (1 << alignment_bits) - 1) & (0xFFFFFFFF << alignment_bits);
}

//.text:0051D2A0 ; c_physical_memory_allocation::allocate
//.text:0051D2F0 ; c_physical_memory_index::allocate_specific_page
//.text:0051D330 ; c_physical_memory_index::free_region
//.text:0051D360 ; c_physical_memory_index::free_region_pages
//.text:0051D380 ; c_physical_memory_index::free_specific_page
//.text:0051D3B0 ; c_physical_memory_index::get_page_count
//.text:0051D3C0 ; c_physical_memory_index::get_region_size
//.text:0051D3D0 ; c_physical_memory_index::grow_region
//.text:0051D420 ; c_physical_memory_index::grow_region_pages

void __cdecl physical_memory_adjust_resize_region(c_basic_buffer<void> resize_region_a, c_basic_buffer<void> resize_region_b)
{
	INVOKE(0x0051D500, physical_memory_adjust_resize_region, resize_region_a, resize_region_b);

	//if ((physical_memory_globals.current_stage - 3) <= 2)
	//{
	//	if (physical_memory_globals.resize_region_listener)
	//		physical_memory_globals.resize_region_listener->resize_no_fail(resize_region_a, resize_region_b);
	//}
}

bool __cdecl physical_memory_can_allocate_fixed_allocations()
{
	return INVOKE(0x0051D560, physical_memory_can_allocate_fixed_allocations);

	//return physical_memory_globals.current_stage < _memory_stage_in_level;
}

void __cdecl physical_memory_create_resizeable_contiguous_region(c_physical_memory_contiguous_region_listener* resize_region_listener)
{
	return INVOKE(0x0051D570, physical_memory_create_resizeable_contiguous_region, resize_region_listener);

	//physical_memory_globals.resize_region_listener = resize_region_listener;
}

void __cdecl physical_memory_destroy_resizeable_contiguous_region(const c_physical_memory_contiguous_region_listener* resize_region_listener)
{
	return INVOKE(0x0051D580, physical_memory_destroy_resizeable_contiguous_region, resize_region_listener);

	//physical_memory_globals.resize_region_listener = NULL;
}

void __cdecl physical_memory_dispose()
{
	INVOKE(0x0051D590, physical_memory_dispose);

	//csmemset(&physical_memory_globals, 0, sizeof(physical_memory_globals));
	//VirtualFree(k_physical_memory_base_virtual_address, 0, 0x8000);
}

void __cdecl physical_memory_free(void* memory) // nullsub
{
	physical_memory_system_free(memory);
}

uns32 __cdecl physical_memory_get_broken_memory_offset()
{
	return INVOKE(0x0051D5E0, physical_memory_get_broken_memory_offset);

	//return 4096;
}

s_physical_memory_stage* __cdecl physical_memory_get_current_stage()
{
	return INVOKE(0x0051D5F0, physical_memory_get_current_stage);

	//return &physical_memory_globals.memory_stages[physical_memory_globals.current_stage];
}

c_basic_buffer<void>* __cdecl physical_memory_get_free_memory_region(c_basic_buffer<void>* buffer)
{
	return INVOKE(0x0051D600, physical_memory_get_free_memory_region, buffer);
}

int32 __cdecl physical_memory_get_remaining()
{
	return INVOKE(0x0051D670, physical_memory_get_remaining);

	//s_physical_memory_stage* current_stage = physical_memory_get_current_stage();
	//return current_stage->high_address - current_stage->low_address;
}

void __cdecl physical_memory_initialize()
{
	//INVOKE(0x0051D690, physical_memory_initialize);
	
	recalculate_data_size_increase(g_physical_memory_data_size_increase_mb);
	recalculate_cache_size_increase(g_physical_memory_cache_size_increase_mb);

	k_physical_memory_base_virtual_address = physical_memory_system_malloc(g_physical_memory_data_size_new + g_physical_memory_cache_size_new, NULL);
	k_virtual_to_physical_base_offset = k_physical_memory_base_virtual_address;
	resources_buffer = (byte*)k_physical_memory_base_virtual_address + g_physical_memory_data_size_new;

	csmemset(&physical_memory_globals, 0, sizeof(physical_memory_globals));

	physical_memory_globals.current_stage = _memory_stage_initial;
	physical_memory_globals.minimum_address = (uns32)k_virtual_to_physical_base_offset;
	physical_memory_globals.maximum_address = (uns32)resources_buffer;
	physical_memory_globals.no_mans_land = (int8*)1;

	if (s_physical_memory_stage* current_stage = physical_memory_get_current_stage())
	{
		current_stage->low_address = physical_memory_globals.minimum_address;
		current_stage->high_address = physical_memory_globals.maximum_address;
		current_stage->next_available_zero_allocation = physical_memory_globals.no_mans_land;
		current_stage->allocation_count = 0;
		current_stage->fixed_address_allocation_count = 0;
	}
}

void __cdecl physical_memory_mark_free_memory(c_basic_buffer<void> old_free_region, c_basic_buffer<void> new_free_region)
{
	INVOKE(0x0051D770, physical_memory_adjust_resize_region, old_free_region, new_free_region);

	//physical_memory_adjust_resize_region(old_free_region, new_free_region);
}

void __cdecl physical_memory_query_bounds(uns32 physical_memory_base_address, uns32 physical_memory_query_address, uns32* out_physical_memory_start, uns32* out_physical_memory_end)
{
	INVOKE(0x0051D7A0, physical_memory_query_bounds, physical_memory_base_address, physical_memory_query_address, out_physical_memory_start, out_physical_memory_end);
}

//.text:0051D8A0
//.text:0051D8E0
//.text:0051D920
//.text:0051D940

void __cdecl physical_memory_resize_region_dispose()
{
	INVOKE(0x0051D960, physical_memory_resize_region_dispose);

	//physical_memory_stage_pop(_memory_stage_resize_available);
	//if (physical_memory_globals.resize_region_listener)
	//{
	//	c_basic_buffer<void> free_memory_region{};
	//	physical_memory_get_free_memory_region(&free_memory_region);
	//	physical_memory_globals.resize_region_listener->dispose_resize_buffer(free_memory_region);
	//}
	//physical_memory_stage_pop(_memory_stage_resize_initialize);
}

void __cdecl physical_memory_resize_region_initialize()
{
	INVOKE(0x0051DA50, physical_memory_resize_region_initialize);

	//physical_memory_stage_push(_memory_stage_resize_initialize);
	//if (physical_memory_globals.resize_region_listener)
	//{
	//	c_basic_buffer<void> free_memory_region{};
	//	physical_memory_get_free_memory_region(&free_memory_region);
	//	physical_memory_globals.resize_region_listener->initialize_resize_buffer(free_memory_region);
	//}
	//physical_memory_stage_push(_memory_stage_resize_available);
}

c_basic_buffer<void>& __cdecl physical_memory_resize_region_lock()
{
	//return INVOKE(0x0051DB10, physical_memory_resize_region_lock);

	static c_basic_buffer<void> resize_region{};

	physical_memory_stage_push(_memory_stage_resize_locked);
	resize_region.set_buffer(resources_buffer, g_physical_memory_cache_size_new);

	return resize_region;
}

void __cdecl physical_memory_resize_region_unlock(c_basic_buffer<void> resize_region)
{
	INVOKE(0x0051DB70, physical_memory_resize_region_unlock, resize_region);

	//physical_memory_globals.current_stage--;
}

uns32 __cdecl physical_memory_round_up_allocation_size(uns32 size)
{
	return INVOKE(0x0051DB80, physical_memory_round_up_allocation_size, size);

	//return align_up(size, k_physical_memory_page_size_bits);
}

//void __cdecl physical_memory_stage_pop_internal(memory_stage stage, memory_stage new_stage)
//{
//	ASSERT(stage == physical_memory_globals.current_stage);
//	ASSERT(stage != _memory_stage_initial);
//	ASSERT(stage > new_stage);
//
//	c_basic_buffer<void> free_memory_region{};
//	c_basic_buffer<void> new_free_memory_region{};
//
//	physical_memory_get_free_memory_region(&free_memory_region);
//	physical_memory_globals.current_stage = new_stage;
//	physical_memory_get_free_memory_region(&new_free_memory_region);
//
//	physical_memory_mark_free_memory(free_memory_region, new_free_memory_region);
//}

void __cdecl physical_memory_stage_pop(memory_stage stage)
{
	INVOKE(0x0051DBA0, physical_memory_stage_pop, stage);

	//ASSERT(stage == physical_memory_globals.current_stage);
	//ASSERT(stage != _memory_stage_initial);
	//
	//physical_memory_stage_pop_internal(stage, memory_stage(physical_memory_globals.current_stage - 1));
}
//HOOK_DECLARE(0x0051DBA0, physical_memory_stage_pop);

void __cdecl physical_memory_stage_push(memory_stage stage)
{
	INVOKE(0x0051DC00, physical_memory_stage_push, stage);

	//memory_stage new_stage = memory_stage(physical_memory_globals.current_stage + 1);
	//ASSERT(stage == new_stage);
	//
	//physical_memory_globals.memory_stages[new_stage] = physical_memory_globals.memory_stages[physical_memory_globals.current_stage];
	//physical_memory_globals.current_stage = new_stage;
	//
	////ASSERT((physical_memory_globals.memory_stages[new_stage].low_address & k_physical_memory_page_mask) == 0);
	////ASSERT((physical_memory_globals.memory_stages[new_stage].high_address & k_physical_memory_page_mask) == 0);
}
//HOOK_DECLARE(0x0051DC00, physical_memory_stage_push);

void __cdecl physical_memory_system_free(void* address)
{
	INVOKE(0x0051DC50, physical_memory_system_free, address);

	//VirtualFree(memory, 0, MEM_RELEASE);
}

void* __cdecl physical_memory_system_malloc(uns32 size, void* address)
{
	return INVOKE(0x0051DC70, physical_memory_system_malloc, size, address);

	//return VirtualAlloc(address, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

bool __cdecl physical_memory_try_to_resize_contiguous_buffer_simple(c_physical_memory_contiguous_region_listener* region_listener, c_basic_buffer<void> in_region, uns32 minimum_new_size, c_basic_buffer<void>* out_new_region)
{
	return INVOKE(0x0051DC90, physical_memory_try_to_resize_contiguous_buffer_simple, region_listener, in_region, minimum_new_size, out_new_region);

	//void* new_base_address = NULL;
	//uns32 new_region_size = 0;
	//bool result = physical_memory_try_to_resize_contiguous_region(region_listener, in_region.begin(), in_region.size(), minimum_new_size, minimum_new_size, 0, &new_base_address, &new_region_size);
	//if (result)
	//	out_new_region->set_buffer(new_base_address, new_region_size);
	//return result;
}

bool __cdecl physical_memory_try_to_resize_contiguous_region(const c_physical_memory_contiguous_region_listener* region_listener, void* in_region_buffer, uns32 in_region_size, uns32 minimum_new_size, uns32 requested_size, uns32 a6, void** out_new_base_address, uns32* out_new_region_size)
{
	return INVOKE(0x0051DCD0, physical_memory_try_to_resize_contiguous_region, region_listener, in_region_buffer, in_region_size, minimum_new_size, requested_size, a6, out_new_base_address, out_new_region_size);
}

//.text:0051DDD0 ; GPU_CONVERT_CPU_TO_CPU_CACHED_READONLY_ADDRESS
//.text:0051DE00 ; c_physical_memory_index::shrink_region
//.text:0051DE30 ; c_physical_memory_index::shrink_region_pages
//.text:0051DE80
//.text:0051DEB0 ; GPU_CONVERT_CPU_TO_GPU_ADDRESS
//.text:0051DED0 ; GPU_CONVERT_GPU_TO_CPU_CACHED_READONLY_ADDRESS

