#include "interface/user_interface_memory.hpp"

#include "cache/physical_memory_map.hpp"
#include "game/game.hpp"
#include "main/main_game.hpp"
#include "memory/memory_pool.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"

REFERENCE_DECLARE(0x0526979C, s_memory_pool*, g_ui_memory_pool);
REFERENCE_DECLARE(0x052697A0, int32, g_ui_memory_pool_size);
REFERENCE_DECLARE(0x019428EC, c_physical_memory_allocation, g_physical_memory_allocation); // only use in this translation unit
HOOK_DECLARE(0x00AB4E50, user_interface_free);

c_ui_memory_scope_lock::c_ui_memory_scope_lock()
{
	if (game_is_multithreaded())
	{
		internal_critical_section_enter(k_crit_section_ui_memory_lock);
	}
}

c_ui_memory_scope_lock::~c_ui_memory_scope_lock()
{
	if (game_is_multithreaded())
	{
		internal_critical_section_leave(k_crit_section_ui_memory_lock);
	}
}

void __cdecl user_interface_free(const void* ptr)
{
	//INVOKE(0x00AB4E50, user_interface_free, ptr);

	c_ui_memory_scope_lock scope_lock;

	ASSERT(ptr != NULL);
	ASSERT(g_ui_memory_pool != NULL);
	//ASSERT(user_interface_is_tracked_memory(ptr));
	//ASSERT(!user_interface_messaging_is_memory_in_message_queue(ptr));

	memory_pool_get_block_size(g_ui_memory_pool, &ptr);
	memory_pool_block_free(g_ui_memory_pool, &ptr);
}

void* __cdecl user_interface_malloc_tracked(uns32 size, const char* file, int32 line)
{
	return INVOKE(0x00AB4EF0, user_interface_malloc_tracked, size, file, line);
}

c_allocation_base* __cdecl user_interface_memory_allocation()
{
	return INVOKE(0x00AB4FE0, user_interface_memory_allocation);
}

void __cdecl user_interface_memory_dispose()
{
	//INVOKE(0x00AB4FF0, user_interface_memory_dispose);

	if (g_ui_memory_pool)
	{
		memory_pool_dispose(g_ui_memory_pool);
		g_ui_memory_pool = NULL;
	}
}

//.text:00AB5010 ; void __cdecl user_interface_memory_dump(const char*)
//.text:00AB5030 ; void __cdecl user_interface_memory_get_address_range(void**, void**)

void __cdecl user_interface_memory_initialize(e_map_memory_configuration memory_configuration)
{
	//INVOKE(0x00AB5060, user_interface_memory_initialize, memory_configuration);

	int32 size = 0xD00000;
	if (map_memory_configuration_is_campaign(memory_configuration))
	{
		size = 0x80000;
	}

	g_ui_memory_pool = memory_pool_new("ui memory pool", size, k_memory_pool_no_callback, &g_physical_memory_allocation);
	g_ui_memory_pool_size = memory_pool_allocation_size(size);

	if (g_ui_memory_pool)
	{
		memory_pool_toggle_reference_tracking(g_ui_memory_pool, false);
		memory_pool_toggle_allocation_from_anywhere_in_pool(g_ui_memory_pool, true);
	}
}

