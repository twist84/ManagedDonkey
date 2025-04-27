#include "rasterizer/rasterizer_memory.hpp"

REFERENCE_DECLARE(0x05115B88, s_rasterizer_memory_globals, rasterizer_memory_globals);

//.text:00A6D890 ; void __cdecl rasterizer_memory_GPU_frontend_allocate_from_cost(s_rasterizer_memory_cost const* cost)
//.text:00A6D8C0 ; real32 __cdecl rasterizer_memory_GPU_frontend_remaining()
//.text:00A6D8D0 ; void __cdecl rasterizer_memory_add_cost(s_rasterizer_memory_cost const* cost, s_rasterizer_memory_cost* result)
//.text:00A6D900 ; void __cdecl rasterizer_memory_add_cost_difference(s_rasterizer_memory_cost const* higher_cost, s_rasterizer_memory_cost const* lower_cost, s_rasterizer_memory_cost* result)
//.text:00A6D940 ; int32 __cdecl rasterizer_memory_allocate(int32, int32 element_count, int32 element_size)
//.text:00A6D9E0 ; void* __cdecl rasterizer_memory_allocate_from_CPU_puddle(int32 element_count, int32 element_size)
//.text:00A6DA10 ; int32 __cdecl rasterizer_memory_allocate_vertex_buffer(int32, int32, int32, int32, int32)
//.text:00A6DA20 ; int32 __cdecl rasterizer_memory_bytes_remaining(int32)
//.text:00A6DA50 ; bool __cdecl rasterizer_memory_compare_costs(s_rasterizer_memory_cost const* cost, s_rasterizer_memory_cost const* target_cost, bool a3)
//.text:00A6DA90 ; void __cdecl rasterizer_memory_flush_cache()
//.text:00A6DAA0 ; void* __cdecl rasterizer_memory_get(int32)
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

//.text:00A6DB90 ; void __cdecl rasterizer_memory_set_cost(s_rasterizer_memory_cost* cost, real32 cost_scale, int32 CPU_memory_in_bytes, int32 GPU_memory_in_bytes, real32 GPU_frontend_cost)
//.text:00A6DC50 ; void __cdecl rasterizer_memory_zero_cost(s_rasterizer_memory_cost* result)

