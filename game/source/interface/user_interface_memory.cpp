#include "interface/user_interface_memory.hpp"

#include "cache/physical_memory_map.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "main/console.hpp"
#include "main/main_game.hpp"
#include "memory/memory_pool.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"

REFERENCE_DECLARE(0x0526979C, s_memory_pool*, g_ui_memory_pool);
REFERENCE_DECLARE(0x052697A0, int32, g_ui_memory_pool_size);
REFERENCE_DECLARE(0x052697A5, bool, g_ui_memory_pool_dumped);
static REFERENCE_DECLARE(0x019428EC, c_physical_memory_allocation, g_physical_memory_allocation); // only use in this translation unit

HOOK_DECLARE(0x00AB4E50, user_interface_free);
HOOK_DECLARE(0x00AB4ED0, user_interface_malloc);
HOOK_DECLARE(0x00AB4EF0, user_interface_malloc_tracked);
HOOK_DECLARE(0x00AB5060, user_interface_memory_initialize);
HOOK_DECLARE(0x00AB5140, user_interface_realloc);
HOOK_DECLARE(0x00AB5160, user_interface_realloc_tracked);

int32 g_ui_memory_pool_blocks = 0;
int32 g_ui_memory_pool_bytes_used = 0;

int32 g_memory_report_threshold = 0x40000;

bool g_ui_memory_status_line_initialized = false;
bool g_ui_memory_status_line_enabled = false;
s_status_line g_ui_memory_status_line;

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

void* operator new(unsigned int size, e_ui_allocation_marker unused)
{
	return user_interface_malloc(size);
}

void operator delete(void* object, e_ui_allocation_marker unused)
{
	VASSERT(false, "this operator should never be used - use ui_track_delete(), or user_interface_free() for primitive types");
}

void __cdecl user_interface_free(const void* ptr)
{
	//INVOKE(0x00AB4E50, user_interface_free, ptr);

	c_ui_memory_scope_lock scope_lock;
	
	ASSERT(ptr != NULL);
	ASSERT(g_ui_memory_pool != NULL);
	//ASSERT(user_interface_is_tracked_memory(ptr));
	//ASSERT(!user_interface_messaging_is_memory_in_message_queue(ptr));

	g_ui_memory_pool_blocks--;
	g_ui_memory_pool_bytes_used -= memory_pool_get_block_size(g_ui_memory_pool, &ptr);
	memory_pool_block_free(g_ui_memory_pool, &ptr);

	user_interface_memory_update_status_line();
}

void* __cdecl user_interface_malloc(unsigned int size)
{
	//return INVOKE(0x00AB4ED0, user_interface_malloc, size);

	return user_interface_malloc_tracked(size, NULL, 0);
}

void* __cdecl user_interface_malloc_tracked(unsigned int size, const char* file, int32 line)
{
	//return INVOKE(0x00AB4EF0, user_interface_malloc_tracked, size, file, line);

	c_ui_memory_scope_lock scope_lock;

	ASSERT(g_ui_memory_pool != NULL);

	void* ptr = NULL;
	if (memory_pool_block_allocate(g_ui_memory_pool, &ptr, size, file, line))
	{
		g_ui_memory_pool_blocks++;
		g_ui_memory_pool_bytes_used += memory_pool_get_block_size(g_ui_memory_pool, (const void**)&ptr);
		if (g_ui_memory_pool_bytes_used >= g_memory_report_threshold)
		{
			event(_event_message, "ui:memory_peak: reached %d bytes allocated",
				g_ui_memory_pool_bytes_used);

			g_memory_report_threshold = g_ui_memory_pool_bytes_used + 0x10000;
		}

	}
	else if (memory_pool_block_allocate(g_ui_memory_pool, &ptr, size, file, line))
	{
		g_ui_memory_pool_blocks++;
		g_ui_memory_pool_bytes_used += memory_pool_get_block_size(g_ui_memory_pool, (const void**)&ptr);
	}
	else
	{
		user_interface_out_of_memory(size, file, line);
		ptr = NULL;
	}

	user_interface_memory_update_status_line();

	return ptr;
}

c_allocation_base* __cdecl user_interface_memory_allocation()
{
	return INVOKE(0x00AB4FE0, user_interface_memory_allocation);

	//return &g_user_interface_allocation;
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

void __cdecl user_interface_memory_dump(const char* prefix)
{
	INVOKE(0x00AB5010, user_interface_memory_dump, prefix);
}

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

	ASSERT(g_ui_memory_pool);
	memory_pool_toggle_reference_tracking(g_ui_memory_pool, false);
	memory_pool_toggle_allocation_from_anywhere_in_pool(g_ui_memory_pool, true);

	if (!g_ui_memory_status_line_initialized)
	{
		status_lines_initialize(&g_ui_memory_status_line, &g_ui_memory_status_line_enabled, 1);
		g_ui_memory_status_line_initialized = true;
	}
}

//.text:00AB50D0 ; bool __cdecl user_interface_memory_initialized()
//.text:00AB50E0 ; void __cdecl user_interface_memory_pool_verify()
//.text:00AB50F0 ; void __cdecl user_interface_memory_print_usage()

void __cdecl user_interface_memory_update_status_line()
{
	//INVOKE(0x00AB5100, user_interface_memory_update_status_line);

	g_ui_memory_status_line.text.print("ui memory: #%ld blocks : %ld bytes used",
		g_ui_memory_pool_blocks,
		g_ui_memory_pool_bytes_used);
}

void __cdecl user_interface_out_of_memory(unsigned int size, const char* file, int32 line)
{
	//INVOKE(0x00AB5110, user_interface_out_of_memory, size, file, line);

	if (!g_ui_memory_pool_dumped)
	{
		user_interface_memory_dump("ui");
		g_ui_memory_pool_dumped = true;
	}
	
	event(_event_critical, "ui: ui failed to allocate %d bytes at %s#%d", size, file ? file : "NULL", line);
}

void* __cdecl user_interface_realloc(void* ptr, unsigned int size)
{
	//return INVOKE(0x00AB5140, user_interface_realloc, ptr, size);

	return user_interface_realloc_tracked(ptr, size, NULL, 0);
}

void* __cdecl user_interface_realloc_tracked(void* ptr, unsigned int size, const char* file, int32 line)
{
	//return INVOKE(0x00AB5160, user_interface_realloc_tracked, ptr, size, file, line);

	c_ui_memory_scope_lock scope_lock;

	ASSERT(g_ui_memory_pool != NULL);

	if (ptr)
	{
		g_ui_memory_pool_blocks--;
		g_ui_memory_pool_bytes_used -= memory_pool_get_block_size(g_ui_memory_pool, (const void**)&ptr);
		if (memory_pool_block_reallocate(g_ui_memory_pool, &ptr, size, file, line))
		{
			g_ui_memory_pool_blocks++;
			g_ui_memory_pool_bytes_used += memory_pool_get_block_size(g_ui_memory_pool, (const void**)&ptr);
		}
		if (memory_pool_block_reallocate(g_ui_memory_pool, &ptr, size, file, line))
		{
			g_ui_memory_pool_blocks++;
			g_ui_memory_pool_bytes_used += memory_pool_get_block_size(g_ui_memory_pool, (const void**)&ptr);
		}
		else
		{
			user_interface_out_of_memory(size, file, line);
			ptr = NULL;
		}
	}
	else
	{
		ptr = user_interface_malloc_tracked(size, file, line);
	}

	user_interface_memory_update_status_line();

	return ptr;
}

