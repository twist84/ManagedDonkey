#include "cache/physical_memory_map.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x0238EC50, s_physical_memory_globals, physical_memory_globals);

char const* const k_physical_memory_stage_names[]
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
static_assert(NUMBEROF(k_physical_memory_stage_names) == _memory_stage_in_level + 1);

void* __cdecl _physical_memory_malloc_fixed(memory_stage stage, char const* name, long size, dword flags)
{
	return INVOKE(0x0051D180, _physical_memory_malloc_fixed, stage, name, size, flags);
}

dword __cdecl align_up(dword value, long alignment_bits)
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

void __cdecl physical_memory_destroy_resizeable_contiguous_region(c_physical_memory_contiguous_region_listener const* resize_region_listener)
{
	return INVOKE(0x0051D580, physical_memory_destroy_resizeable_contiguous_region, resize_region_listener);

	//physical_memory_globals.resize_region_listener = NULL;
}

void __cdecl physical_memory_dispose()
{
	INVOKE(0x0051D590, physical_memory_dispose);

	//csmemset(&physical_memory_globals, 0, sizeof(physical_memory_globals));
	//VirtualFree(physical_memory_base_address, 0, 0x8000);
}

void __cdecl physical_memory_free(void* memory) // nullsub
{
	INVOKE(0x0051D5C0, physical_memory_free, memory);
}
//HOOK_DECLARE(0x0051D5C0, physical_memory_free);

dword __cdecl physical_memory_get_broken_memory_offset()
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

long __cdecl physical_memory_get_remaining()
{
	return INVOKE(0x0051D670, physical_memory_get_remaining);

	//s_physical_memory_stage* current_stage = physical_memory_get_current_stage();
	//return current_stage->high_address - current_stage->low_address;
}

void __cdecl physical_memory_initialize()
{
	INVOKE(0x0051D690, physical_memory_initialize);
}

void __cdecl physical_memory_mark_free_memory(c_basic_buffer<void> resize_region_a, c_basic_buffer<void> resize_region_b)
{
	INVOKE(0x0051D770, physical_memory_adjust_resize_region, resize_region_a, resize_region_b);

	//physical_memory_adjust_resize_region(resize_region_a, resize_region_b);
}

void __cdecl physical_memory_query_bounds(dword a1, dword a2, dword* a3, dword* a4)
{
	INVOKE(0x0051D7A0, physical_memory_query_bounds, a1, a2, a3, a4);
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

c_basic_buffer<void> __cdecl physical_memory_resize_region_lock()
{
	return INVOKE(0x0051DB10, physical_memory_resize_region_lock);
}

void __cdecl physical_memory_resize_region_unlock(c_basic_buffer<void> resize_region)
{
	INVOKE(0x0051DB70, physical_memory_resize_region_unlock, resize_region);

	//physical_memory_globals.current_stage--;
}

dword __cdecl physical_memory_round_up_allocation_size(dword size)
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

//.text:0051DDD0 ; GPU_CONVERT_CPU_TO_CPU_CACHED_READONLY_ADDRESS
//.text:0051DE00 ; c_physical_memory_index::shrink_region
//.text:0051DE30 ; c_physical_memory_index::shrink_region_pages
//.text:0051DE80
//.text:0051DEB0 ; GPU_CONVERT_CPU_TO_GPU_ADDRESS
//.text:0051DED0 ; GPU_CONVERT_GPU_TO_CPU_CACHED_READONLY_ADDRESS

