#include "networking/network_memory.hpp"

REFERENCE_DECLARE(0x01A28D58, s_network_shared_memory_globals, network_shared_memory_globals);
REFERENCE_DECLARE(0x01A28D88, s_network_base_memory_globals, network_base_memory_globals);

REFERENCE_DECLARE(0x01860454, uns16, g_broadcast_port);

c_simulation_distributed_world* __cdecl network_allocate_simulation_distributed_world()
{
    return INVOKE(0x004621A0, network_allocate_simulation_distributed_world);
}

c_network_channel* __cdecl network_channel_iterate(const c_network_channel* channel)
{
    return INVOKE(0x004621B0, network_channel_iterate, channel);
}

void* __cdecl network_heap_allocate_block(int32 block_size)
{
    return INVOKE(0x004621D0, network_heap_allocate_block, block_size);
}

char* __cdecl network_heap_describe(char* buffer, int32 buffer_size)
{
    return INVOKE(0x004621F0, network_heap_describe, buffer, buffer_size);
}

void __cdecl network_heap_free_block(void* block)
{
    INVOKE(0x00462200, network_heap_free_block, block);
}

void __cdecl network_memory_shared_dispose()
{
    INVOKE(0x00462640, network_memory_shared_dispose);
}

void __cdecl network_memory_shared_initialize(e_map_memory_configuration memory_configuration)
{
    INVOKE(0x004626E0, network_memory_shared_initialize, memory_configuration);
}

bool __cdecl network_memory_simulation_initialize(c_simulation_world** world, c_simulation_watcher** watcher, c_simulation_type_collection** type_collection)
{
    //return INVOKE(0x00462830, network_memory_simulation_initialize, world, watcher, type_collection);

    ASSERT(world);
    ASSERT(watcher);
    ASSERT(type_collection);

    *world = &network_base_memory_globals.simulation_world;
    *watcher = &network_base_memory_globals.simulation_watcher;
    *type_collection = &network_base_memory_globals.simulation_types;

    return true;
}

