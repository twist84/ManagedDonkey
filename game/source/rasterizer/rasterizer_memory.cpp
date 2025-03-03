#include "rasterizer/rasterizer_memory.hpp"

REFERENCE_DECLARE(0x05115B88, s_rasterizer_memory_globals, rasterizer_memory_globals);

//.text:00A6D890 ; void __cdecl rasterizer_memory_GPU_frontend_allocate_from_cost(s_rasterizer_memory_cost const* cost)
//.text:00A6D8C0 ; real __cdecl rasterizer_memory_GPU_frontend_remaining()
//.text:00A6D8D0 ; void __cdecl rasterizer_memory_add_cost(s_rasterizer_memory_cost const* cost, s_rasterizer_memory_cost* result)
//.text:00A6D900 ; void __cdecl rasterizer_memory_add_cost_difference(s_rasterizer_memory_cost const* higher_cost, s_rasterizer_memory_cost const* lower_cost, s_rasterizer_memory_cost* result)
//.text:00A6D940 ; long __cdecl rasterizer_memory_allocate(long, long element_count, long element_size)
//.text:00A6D9E0 ; void* __cdecl rasterizer_memory_allocate_from_CPU_puddle(long element_count, long element_size)
//.text:00A6DA10 ; long __cdecl rasterizer_memory_allocate_vertex_buffer(long, long, long, long, long)
//.text:00A6DA20 ; long __cdecl rasterizer_memory_bytes_remaining(long)
//.text:00A6DA50 ; bool __cdecl rasterizer_memory_compare_costs(s_rasterizer_memory_cost const* cost, s_rasterizer_memory_cost const* target_cost, bool a3)
//.text:00A6DA90 ; void __cdecl rasterizer_memory_flush_cache()
//.text:00A6DAA0 ; void* __cdecl rasterizer_memory_get(long)
//.text:00A6DAC0 ; rasterizer_memory_get_vertex_buffer?
//.text:00A6DAD0 ; rasterizer_memory_get_vertex_buffer_data?
//.text:00A6DAE0 ; rasterizer_memory_get_CPU_memory_pool_sizes?

void __cdecl rasterizer_memory_initialize()
{
	INVOKE(0x00A6DB10, rasterizer_memory_initialize);
}

void __cdecl rasterizer_memory_reset()
{
	INVOKE(0x00A6DB60, rasterizer_memory_reset);
}

//.text:00A6DB90 ; void __cdecl rasterizer_memory_set_cost(s_rasterizer_memory_cost* cost, real cost_scale, long CPU_memory_in_bytes, long GPU_memory_in_bytes, real GPU_frontend_cost)
//.text:00A6DC50 ; void __cdecl rasterizer_memory_zero_cost(s_rasterizer_memory_cost* result)

