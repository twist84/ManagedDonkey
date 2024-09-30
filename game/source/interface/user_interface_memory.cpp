#include "interface/user_interface_memory.hpp"

#include "game/game.hpp"
#include "memory/memory_pool.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"

REFERENCE_DECLARE(0x0526979C, s_memory_pool*, g_ui_memory_pool);

HOOK_DECLARE(0x00AB4E50, user_interface_free);

c_ui_memory_scope_lock::c_ui_memory_scope_lock()
{
	if (game_is_multithreaded())
		internal_critical_section_enter(_critical_section_ui_memory);
}

c_ui_memory_scope_lock::~c_ui_memory_scope_lock()
{
	if (game_is_multithreaded())
		internal_critical_section_leave(_critical_section_ui_memory);
}

void __cdecl user_interface_free(void const* ptr)
{
	//INVOKE(0x00AB4E50, user_interface_free, ptr);

	ASSERT(ptr != NULL);
	ASSERT(g_ui_memory_pool != NULL);
	//ASSERT(user_interface_is_tracked_memory(ptr));
	//ASSERT(!user_interface_messaging_is_memory_in_message_queue(ptr));

	c_ui_memory_scope_lock scope_lock;

	memory_pool_get_block_size(g_ui_memory_pool, &ptr);
	memory_pool_block_free(g_ui_memory_pool, &ptr);
}

void* __cdecl user_interface_malloc_tracked(dword size, char const* file, long line)
{
	return INVOKE(0x00AB4EF0, user_interface_malloc_tracked, size, file, line);
}

c_allocation_base* __cdecl user_interface_memory_allocation()
{
	return INVOKE(0x00AB4FE0, user_interface_memory_allocation);
}

