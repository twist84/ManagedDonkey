#pragma once

#include "cseries/cseries.hpp"

enum
{
	k_rasterizer_memory_block_size_in_bytes = 64,

	k_rasterizer_memory_cpu_memory_pool_size_max = 0x100000,
	k_rasterizer_memory_cpu_puddle_size_max = 0x1
};

struct s_rasterizer_memory_cost
{
	word CPU_memory_blocks; // CPU_memory_in_bytes / k_rasterizer_memory_block_size_in_bytes
	word GPU_memory_blocks; // GPU_memory_in_bytes / k_rasterizer_memory_block_size_in_bytes
	real GPU_frontend_cost;
};
static_assert(sizeof(s_rasterizer_memory_cost) == 0x8);

struct s_rasterizer_memory_globals
{
	bool initialized;

	byte __data1[0x7];

	long CPU_memory_pool_size;
	byte CPU_memory_pool[k_rasterizer_memory_cpu_memory_pool_size_max];
	long __unknown10000C; // CPU_memory_pool_size_used?

	long CPU_puddle_size;
	byte CPU_puddle[k_rasterizer_memory_cpu_puddle_size_max];

	long __unknown100018;
	long GPU_memory_pool_size;
	long GPU_memory_pool_size_max;

	byte __data100024[0x18];

	bool __unknown10003C;
	real GPU_frontend_remaining;
};
static_assert(sizeof(s_rasterizer_memory_globals) == 0x100044);

extern s_rasterizer_memory_globals& rasterizer_memory_globals;

extern void __cdecl rasterizer_memory_initialize();

