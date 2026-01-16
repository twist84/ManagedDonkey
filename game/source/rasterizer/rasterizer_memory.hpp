#pragma once

#include "cseries/cseries.hpp"

enum
{
	k_rasterizer_CPU_memory_pool_size_in_bytes = 0x100000,
	k_rasterizer_CPU_memory_puddle_size_in_bytes = 0x1,
	k_rasterizer_GPU_memory_pool_count = 2,
	k_rasterizer_GPU_memory_pool_alignment = 64,
};

struct s_rasterizer_memory_cost
{
	uns16 CPU_memory_blocks; // CPU_memory_in_bytes / k_rasterizer_GPU_memory_pool_alignment
	uns16 GPU_memory_blocks; // GPU_memory_in_bytes / k_rasterizer_GPU_memory_pool_alignment
	real32 GPU_frontend_cost;
};
COMPILE_ASSERT(sizeof(s_rasterizer_memory_cost) == 0x8);

struct s_rasterizer_GPU_memory_pool
{
	void* data;
	uns32 fence_handle;
	bool fence_enabled;
};

struct s_rasterizer_memory_globals
{
	bool initialized;
	void* physical_memory;
	int32 CPU_memory_pool_size;
	uns8 CPU_memory_pool[k_rasterizer_CPU_memory_pool_size_in_bytes];
	int32 CPU_memory_pool_max_size_reached;
	int32 CPU_memory_puddle_size;
	uns8 CPU_memory_puddle[k_rasterizer_CPU_memory_puddle_size_in_bytes];
	int32 GPU_memory_pool_index;
	int32 GPU_memory_pool_size;
	int32 GPU_memory_pool_size_max;
	__declspec(align(8)) s_rasterizer_GPU_memory_pool GPU_memory_pools[k_rasterizer_GPU_memory_pool_count];
	bool GPU_memory_pool_is_dirty;
	real32 GPU_frontend_remaining;
};
COMPILE_ASSERT(sizeof(s_rasterizer_memory_globals) == 0x100048);

extern s_rasterizer_memory_globals& rasterizer_memory_globals;

extern void __cdecl rasterizer_memory_initialize();
extern void __cdecl rasterizer_memory_reset();

