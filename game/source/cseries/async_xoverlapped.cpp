#include "cseries/async_xoverlapped.hpp"

#include "cseries/async_xoverlapped_memory.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "interface/interface_constants.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "networking/online/online_error.hpp"
#include "networking/online/online_guide_pc.hpp"
#include "text/draw_string.hpp"
#include "xbox/xbox.hpp"

#include <windows.h>

//REFERENCE_DECLARE(0x0240AED0, s_overlapped_globals, g_overlapped_globals);

HOOK_DECLARE(0x005A8D50, find_task_slot);
HOOK_DECLARE(0x005A8DD0, overlapped_dispose);
HOOK_DECLARE(0x005A8DE0, overlapped_initialize);
HOOK_DECLARE(0x005A8E20, overlapped_render);
HOOK_DECLARE(0x005A8E30, overlapped_task_block_until_finished);
HOOK_DECLARE(0x005A8ED0, overlapped_task_is_running);
HOOK_DECLARE(0x005A8F40, overlapped_task_start_internal);
HOOK_DECLARE(0x005A8F50, overlapped_task_terminate);
HOOK_DECLARE(0x005A8FA0, overlapped_task_toggle_debug_rendering);
HOOK_DECLARE(0x005A8FB0, overlapped_task_wait_for_all_tasks_to_finish);
HOOK_DECLARE(0x005A9050, overlapped_update);
HOOK_DECLARE(0x005A91E0, task_block_until_finished);
HOOK_DECLARE(0x005A9230, task_is_complete);
HOOK_DECLARE(0x005A9280, task_now_finished);

//HOOK_DECLARE_CALL(0x0074CA3C, overlapped_task_start_internal); // c_content_enumeration_overlapped_task, c_content_enumeration_overlapped_task::success
//HOOK_DECLARE_CALL(0x00A82786, overlapped_task_start_internal); // c_string_verify_task, user_interface_session_string_verify_update
////HOOK_DECLARE_CALL(0x00AA4306, overlapped_task_start_internal); // c_string_verify_task, c_player_profile_interface::update_service_tag
////HOOK_DECLARE_CALL(0x00AD3699, overlapped_task_start_internal); // c_online_marketplace_purchase_task, c_player_marketplace::show_purchase_ui
////HOOK_DECLARE_CALL(0x00AD36F6, overlapped_task_start_internal); // c_online_marketplace_consume_asset_task, c_player_marketplace::start_asset_consume
////HOOK_DECLARE_CALL(0x00AD3810, overlapped_task_start_internal); // c_online_marketplace_enumerate_assets_task, c_player_marketplace::start_assets_enumerate
//HOOK_DECLARE_CALL(0x00AE668B, overlapped_task_start_internal); // c_virtual_keyboard_task, c_saved_film_take_screenshot::update
//HOOK_DECLARE_CALL(0x00AE6868, overlapped_task_start_internal); // c_virtual_keyboard_task, c_saved_film_take_screenshot::update
//HOOK_DECLARE_CALL(0x00AEA33B, overlapped_task_start_internal); // c_virtual_keyboard_task, c_start_menu_game_editor::begin_save_as_current_edited_variant
//HOOK_DECLARE_CALL(0x00AF0D67, overlapped_task_start_internal); // c_virtual_keyboard_task, c_gui_start_menu_hq_service_record_file_share_choose_item::handle_dialog_result
//HOOK_DECLARE_CALL(0x00AF0FBE, overlapped_task_start_internal); // c_string_verify_task, c_gui_start_menu_hq_service_record_file_share_choose_item::handle_list_item_chosen
//HOOK_DECLARE_CALL(0x00AF26D6, overlapped_task_start_internal); // c_virtual_keyboard_task, c_gui_start_menu_hq_service_record_file_share_item_selected::handle_dialog_result
//HOOK_DECLARE_CALL(0x00AF5088, overlapped_task_start_internal); // c_virtual_keyboard_task, c_gui_start_menu_hq_screenshots_options::update
//HOOK_DECLARE_CALL(0x00AF52D2, overlapped_task_start_internal); // c_virtual_keyboard_task, c_gui_start_menu_hq_screenshots_options::update
//HOOK_DECLARE_CALL(0x00AF8495, overlapped_task_start_internal); // c_virtual_keyboard_task, c_start_menu_settings_appearance_emblem::handle_list_item_chosen
//HOOK_DECLARE_CALL(0x00B0373C, overlapped_task_start_internal); // c_virtual_keyboard_task, c_gui_screen_pregame_selection::begin_selected_item_text_edit
//HOOK_DECLARE_CALL(0x00B0B8D6, overlapped_task_start_internal); // c_virtual_keyboard_task, c_gui_screen_game_options::?
//HOOK_DECLARE_CALL(0x00B0BAA0, overlapped_task_start_internal); // c_virtual_keyboard_task, c_gui_screen_game_options::begin_save_as_current_edited_variant
//HOOK_DECLARE_CALL(0x00B0DA48, overlapped_task_start_internal); // c_string_verify_task, c_gui_screen_game_options::update_save_as_new_operation

int32 const k_maximum_task_slots = 64;

struct s_task_slot
{
	// added back
	XOVERLAPPED overlapped;

	c_overlapped_task* task;
	uns32 calling_result;
	bool cancelled;
};
//static_assert(sizeof(s_task_slot) == 0xC);
static_assert(sizeof(s_task_slot) == 0x28);

struct s_overlapped_globals
{
	bool toggle_debug_rendering;

	c_static_array<s_task_slot, k_maximum_task_slots> task_slots;

	bool inject_error;
	c_static_string<64> inject_error_context;

	bool pause;
	c_static_string<64> pause_context;

	int32 description_count;
	c_static_array<c_static_string<64>, k_maximum_task_slots> descriptions;
};
//static_assert(sizeof(s_overlapped_globals) == 0x30C);
static_assert(sizeof(s_overlapped_globals) == 0x1A8C);

static s_overlapped_globals g_overlapped_globals{};

c_async_xoverlapped_scope_lock::c_async_xoverlapped_scope_lock()
{
	if (game_is_multithreaded())
		internal_critical_section_enter(k_crit_section_async_xoverlapped_memory);
}

c_async_xoverlapped_scope_lock::~c_async_xoverlapped_scope_lock()
{
	if (game_is_multithreaded())
		internal_critical_section_leave(k_crit_section_async_xoverlapped_memory);
}

bool c_overlapped_task::busy() const
{
	//return INVOKE_CLASS_MEMBER(0x00476C30, c_overlapped_task, busy);

	return m_task_state == _overlapped_task_state_starting
		|| m_task_state == _overlapped_task_state_pending
		|| m_task_state == _overlapped_task_state_completing;
}

c_overlapped_task::c_overlapped_task(const char* file, int32 line) :
	m_task_flags(),
	m_task_state(_overlapped_task_state_none),
	m_file(file),
	m_line(line)
{
	//DECLFUNC(0x005A8C80, void, __thiscall, c_overlapped_task*, const char*, int32)(this, file, line);
}

//.text:005A8CB0 ; public: virtual c_overlapped_task::~c_overlapped_task()
//.text:005A8CE0 ; public: virtual void* c_overlapped_task::`vector deleting destructor'(unsigned int)
c_overlapped_task::~c_overlapped_task()
{
	if (c_overlapped_task::busy())
	{
		overlapped_task_block_until_finished(this);
	}
}

void c_overlapped_task::complete()
{
	INVOKE_CLASS_MEMBER(0x005A8D30, c_overlapped_task, complete);
}

void c_overlapped_task::failure(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error)
{
	INVOKE_CLASS_MEMBER(0x005A8D40, c_overlapped_task, failure, calling_result, overlapped_error, overlapped_extended_error);
}

s_task_slot* __cdecl find_task_slot(const c_overlapped_task* task)
{
	//return INVOKE(0x005A8D50, find_task_slot, task);

	ASSERT(task);

	s_task_slot* task_slot = NULL;
	for (int32 task_slot_index = 0; task_slot_index < k_maximum_task_slots; task_slot_index++)
	{
		if (g_overlapped_globals.task_slots[task_slot_index].task == task)
			task_slot = &g_overlapped_globals.task_slots[task_slot_index];
	}

	return task_slot;
}

bool c_overlapped_task::is_result_successful(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error)
{
	return INVOKE_CLASS_MEMBER(0x005A8DA0, c_overlapped_task, is_result_successful, calling_result, overlapped_error, overlapped_extended_error);
}

void __cdecl overlapped_dispose()
{
	//INVOKE(0x005A8DD0, overlapped_dispose);

	overlapped_task_wait_for_all_tasks_to_finish();
	c_virtual_keyboard_task::dispose_instance();
	overlapped_memory_dispose();
}

void __cdecl overlapped_initialize()
{
	//INVOKE(0x005A8DE0, overlapped_initialize);

	csmemset(&g_overlapped_globals, 0, sizeof(g_overlapped_globals));
	overlapped_memory_initialize();
	c_virtual_keyboard_task::get_instance(__FILE__, __LINE__, k_any_controller, NULL, NULL, NULL, 0, 0, true);
	g_overlapped_globals.toggle_debug_rendering = false;
	g_overlapped_globals.inject_error = false;
	g_overlapped_globals.pause = false;
	g_overlapped_globals.description_count = false;
}

void __cdecl overlapped_render()
{
	//INVOKE(0x005A8E20, overlapped_render);

	if (g_overlapped_globals.toggle_debug_rendering)
	{
		c_simple_font_draw_string draw_string;

		rectangle2d bounds{};
		interface_get_current_display_settings(NULL, NULL, NULL, &bounds);
		bounds.x0 += (bounds.x1 - bounds.x0) >> 1;
		draw_string.set_bounds(&bounds);
		draw_string.set_color(global_real_argb_green);

		draw_string.draw(NULL, "---------- xoverlapped tasks ----------\r\n");

		for (int32 task_slot_index = 0; task_slot_index < k_maximum_task_slots; task_slot_index++)
		{
			const char* status = NULL;
			char string[256]{};

			{
				c_async_xoverlapped_scope_lock scope_lock;

				s_task_slot* task_slot = &g_overlapped_globals.task_slots[task_slot_index];

				if (task_slot->task)
				{
					switch (task_slot->task->get_task_state())
					{
					case _overlapped_task_state_none:
						status = "none";
						break;
					case _overlapped_task_state_starting:
						status = "starting";
						break;
					case _overlapped_task_state_pending:
						status = "pending";
						break;
					case _overlapped_task_state_completing:
						status = "completing";
						break;
					case _overlapped_task_state_succeeded:
						status = "success";
						break;
					case _overlapped_task_state_failed:
						status = "failure";
						break;
					default:
						status = "<unknown>";
						break;
					}

					csnzprintf(string, sizeof(string), "#%ld task info: context= '%s', status= '%s', file= '%s', line= '%ld'\r\n",
						task_slot_index,
						task_slot->task->get_context_string(),
						status,
						task_slot->task->get_file(),
						task_slot->task->get_line());
				}
			}

			if (string[0])
				draw_string.draw_more(NULL, string);
		}

		draw_string.draw_more(NULL, "---------------------------------------");
	}
}

void __cdecl overlapped_task_block_until_finished(const c_overlapped_task* task)
{
	//INVOKE(0x005A8E30, overlapped_task_block_until_finished, task);

	c_async_xoverlapped_scope_lock scope_lock;
	if (s_task_slot* task_slot = find_task_slot(task))
	{
		task_block_until_finished(task_slot);
	}
}

bool __cdecl overlapped_task_is_running(const c_overlapped_task* task)
{
	//return INVOKE(0x005A8ED0, overlapped_task_is_running, task);

	c_async_xoverlapped_scope_lock scope_lock;
	if (s_task_slot* task_slot = find_task_slot(task))
	{
		return task_slot->task->get_task_state() == _overlapped_task_state_pending || task_slot->task->get_task_state() == _overlapped_task_state_completing;
	}

	return false;
}

bool __cdecl overlapped_task_start_internal(c_overlapped_task* task, const char* file, int32 line)
{
	//return INVOKE(0x005A8F40, overlapped_task_start_internal, task, file, line);

	ASSERT(task);

	bool result = false;
	task->set_file(file);
	task->set_line(line);

	c_async_xoverlapped_scope_lock scope_lock;

	s_task_slot* first_free_task_slot = NULL;
	for (int32 task_slot_index = 0; task_slot_index < k_maximum_task_slots; task_slot_index++)
	{
		if (!g_overlapped_globals.task_slots[task_slot_index].task)
		{
			first_free_task_slot = &g_overlapped_globals.task_slots[task_slot_index];
			break;
		}
	}

	if (!first_free_task_slot)
	{
		event(_event_status, "xoverlapped: overlapped_task_start() failed: maximum tasks already active!");
		return false;
	}

	first_free_task_slot->task = task;
	first_free_task_slot->task->set_task_state_internal(_overlapped_task_state_starting);

	bool context_matches_description = false;
	if (strlen(task->get_context_string()) >= k_maximum_task_slots)
	{
		event(_event_warning, "xoverlapped: task context string '%s' is too long to fit into error-code-injection table (maximum %ld characters)",
			task->get_context_string(),
			64);
	}
	else
	{
		for (int32 description_index = 0; description_index < g_overlapped_globals.description_count; description_index++)
		{
			if (g_overlapped_globals.descriptions[description_index].is_equal(task->get_context_string()))
			{
				context_matches_description = true;
				break;
			}
		}

		if (!context_matches_description)
		{
			if (g_overlapped_globals.description_count >= g_overlapped_globals.descriptions.get_count())
			{
				event(_event_warning, "xoverlapped: task context string '%s' not added to error-code-injection table, maximum tasks already being tracked!",
					task->get_context_string());
			}
			else
			{
				g_overlapped_globals.descriptions[g_overlapped_globals.description_count++].set(task->get_context_string());
			}
		}
	}

	return true;
}

void __cdecl overlapped_task_terminate(c_overlapped_task* task)
{
	//INVOKE(0x005A8F50, overlapped_task_terminate, task);

	c_async_xoverlapped_scope_lock scope_lock;
	if (s_task_slot* task_slot = find_task_slot(task))
	{
		task_slot->cancelled = true;
	}
}

void __cdecl overlapped_task_toggle_debug_rendering(bool toggle_debug_rendering)
{
	//INVOKE(0x005A8FA0, overlapped_task_toggle_debug_rendering, toggle_debug_rendering);

	g_overlapped_globals.toggle_debug_rendering = toggle_debug_rendering;
}

void __cdecl overlapped_task_wait_for_all_tasks_to_finish()
{
	//INVOKE(0x005A8FB0, overlapped_task_wait_for_all_tasks_to_finish);

	for (int32 task_slot_index = 0; task_slot_index < k_maximum_task_slots; task_slot_index++)
	{
		c_async_xoverlapped_scope_lock scope_lock;
		if (!g_overlapped_globals.task_slots[task_slot_index].task)
		{
			continue;
		}

		if (g_overlapped_globals.task_slots[task_slot_index].task->get_task_state() != _overlapped_task_state_pending)
		{
			continue;
		}

		task_block_until_finished(&g_overlapped_globals.task_slots[task_slot_index]);
	}
}

void __cdecl overlapped_update()
{
	//INVOKE(0x005A9050, overlapped_update);

	for (int32 task_slot_index = 0; task_slot_index < k_maximum_task_slots; task_slot_index++)
	{
		{
			c_async_xoverlapped_scope_lock scope_lock;
			s_task_slot* task_slot = &g_overlapped_globals.task_slots[task_slot_index];

			if (task_slot->task && task_slot->task->get_task_state() == _overlapped_task_state_pending)
			{
				uns32 return_result = ERROR_SUCCESS;
				uns32 calling_result = ERROR_SUCCESS;
				uns32 overlapped_error = ERROR_SUCCESS;
				uns32 overlapped_extended_error = ERROR_SUCCESS;
				if (task_is_complete(task_slot, &return_result, &calling_result, &overlapped_error, &overlapped_extended_error))
				{
					task_now_finished(task_slot, return_result, calling_result, overlapped_error, overlapped_extended_error);
				}
				else
				{
					task_slot->task->update(return_result);
				}
			}
		}

		{
			c_async_xoverlapped_scope_lock scope_lock;
			s_task_slot* task_slot = &g_overlapped_globals.task_slots[task_slot_index];

			if (task_slot->task && task_slot->task->get_task_state() == _overlapped_task_state_starting)
			{
				int32 pending_task_count = 0;
			
				for (s_task_slot& pending_task_slot : g_overlapped_globals.task_slots)
				{
					if (pending_task_slot.task && pending_task_slot.task->get_task_state() == _overlapped_task_state_pending)
					{
						pending_task_count++;
					}
				}
			
				if (pending_task_count < 3)
				{
					task_slot->calling_result = task_slot->task->start(&task_slot->overlapped);
					task_slot->task->set_task_state_internal(_overlapped_task_state_pending);
					task_slot->cancelled = false;
				}
			}
		}
	}
}

void c_overlapped_task::reset()
{
	INVOKE_CLASS_MEMBER(0x005A9130, c_overlapped_task, reset);
}

e_overlapped_task_state c_overlapped_task::get_task_state() const
{
	return m_task_state;
}

void c_overlapped_task::set_task_state_internal(e_overlapped_task_state task_state)
{
	//INVOKE_CLASS_MEMBER(0x005A91B0, c_overlapped_task, set_task_state_internal, task_state);

	m_task_state = task_state;
}

void c_overlapped_task::success(uns32 return_result)
{
	INVOKE_CLASS_MEMBER(0x005A91D0, c_overlapped_task, success, return_result);
}

void __cdecl task_block_until_finished(s_task_slot* task_slot)
{
	//INVOKE(0x005A91E0, task_block_until_finished, task_slot);

	uns32 return_result = ERROR_SUCCESS;
	uns32 calling_result = ERROR_SUCCESS;
	uns32 overlapped_error = ERROR_SUCCESS;
	uns32 overlapped_extended_error = ERROR_SUCCESS;

	while (task_slot->task && !task_is_complete(task_slot, &return_result, &calling_result, &overlapped_error, &overlapped_extended_error))
	{
		overlapped_update();
		switch_to_thread();
	}

	if (task_slot->task)
		task_now_finished(task_slot, return_result, calling_result, overlapped_error, overlapped_extended_error);
}

bool __cdecl task_is_complete(s_task_slot* task_slot, uns32* return_result, uns32* calling_result, uns32* overlapped_error, uns32* overlapped_extended_error)
{
	//return INVOKE(0x005A9230, task_is_complete, task_slot, return_result, calling_result, overlapped_error, overlapped_extended_error);

	ASSERT(task_slot != NULL);
	ASSERT(task_slot->task != NULL);
	ASSERT(return_result != NULL);
	ASSERT(calling_result != NULL);
	ASSERT(overlapped_error != NULL);
	ASSERT(overlapped_extended_error != NULL);

	*return_result = ERROR_SUCCESS;
	*calling_result = task_slot->calling_result;
	*overlapped_error = ERROR_SUCCESS;
	*overlapped_extended_error = ERROR_SUCCESS;

	bool result = true;
	if (task_slot->task->get_task_state() == _overlapped_task_state_starting)
	{
		result = false;
	}
	else if (task_slot->calling_result == ERROR_IO_PENDING)
	{
		result = false;
	
		if (task_slot->overlapped.InternalLow != ERROR_IO_PENDING)
		{
			result = true;
			*overlapped_error = XGetOverlappedResult(&task_slot->overlapped, return_result, FALSE);
			if (*overlapped_error)
			{
				if (*overlapped_error != ERROR_IO_INCOMPLETE && *overlapped_error != ERROR_IO_PENDING)
				{
					*overlapped_extended_error = XGetOverlappedExtendedError(&task_slot->overlapped);
				}
			}
		}
	}
	
	if (g_overlapped_globals.pause)
	{
		if (g_overlapped_globals.pause_context.is_equal(task_slot->task->get_context_string()))
			result = false;
	}
	
	return result;
}

void __cdecl task_now_finished(s_task_slot* task_slot, uns32 return_result, uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error)
{
	//INVOKE(0x005A9280, task_now_finished, task_slot, return_result, calling_result, overlapped_error, overlapped_extended_error);
	
	ASSERT(task_slot->task != NULL);
	ASSERT(task_slot->task->get_task_state() == _overlapped_task_state_starting || task_slot->task->get_task_state() == _overlapped_task_state_pending);

	bool succeeded = false;
	if (task_slot->task->get_task_state() == _overlapped_task_state_starting)
	{
		succeeded = false;
	}
	else
	{
		succeeded = !task_slot->cancelled && task_slot->task->is_result_successful(calling_result, overlapped_error, overlapped_extended_error);

		if (g_overlapped_globals.inject_error)
		{
			if (g_overlapped_globals.inject_error_context.is_equal(task_slot->task->get_context_string()))
			{
				event(_event_warning, "xoverlapped: injecting error for task %s",
					g_overlapped_globals.inject_error_context.get_string());
		
				succeeded = false;
			}
		}
	}

	task_slot->task->set_task_state_internal(_overlapped_task_state_completing);

	e_overlapped_task_state task_state = _overlapped_task_state_none;
	if (succeeded)
	{
		task_state = _overlapped_task_state_succeeded;
		task_slot->task->success(return_result);

		event(_event_status, "xoverlapped: task '%s' succeeded. return result= %s",
			task_slot->task->get_context_string(),
			online_error_get_string(return_result).get_string());
	}
	else
	{
		task_state = _overlapped_task_state_failed;

		if (task_slot->cancelled)
		{
			overlapped_error = ERROR_CANCELLED;
		}

		task_slot->task->failure(calling_result, overlapped_error, overlapped_extended_error);
		if (task_slot->cancelled ||
			overlapped_error == ERROR_CANCELLED ||
			overlapped_error == ERROR_FUNCTION_FAILED && overlapped_extended_error == ERROR_CANCELLED)
		{
			event(_event_message, "xoverlapped: task '%s' was cancelled.",
				task_slot->task->get_context_string());
		}
		else
		{
			if (csstrcmp("XStringVerify", task_slot->task->get_context_string()) != 0)
			{
				event(_event_warning, "xoverlapped: task '%s' failed. calling result= %s, error= %s, extended error= %s, file= '%s', line= '%ld'",
					task_slot->task->get_context_string(),
					online_error_get_string(calling_result).get_string(),
					online_error_get_string(overlapped_error).get_string(),
					online_error_get_string(overlapped_extended_error).get_string(),
					task_slot->task->get_file(),
					task_slot->task->get_line());
			}
		}
	}

	task_slot->task->complete();

	if (task_slot->task->task_was_recycled_during_completion())
	{
		task_slot->task->task_recycled_during_completion(false);
	}
	else if (task_slot->task->busy())
	{
		task_slot->task->set_task_state_internal(task_state);
	}

	csmemset(task_slot, 0, sizeof(s_task_slot));
}

void c_overlapped_task::task_recycled_during_completion(bool recycled_during_completion)
{
	//return INVOKE_CLASS_MEMBER(0x005A9360, c_overlapped_task, task_recycled_during_completion, recycled_during_completion);

	m_task_flags.set(_restarted_during_completion_bit, recycled_during_completion);
}

bool c_overlapped_task::task_was_recycled_during_completion() const
{
	//return INVOKE_CLASS_MEMBER(0x005A9380, c_overlapped_task, task_was_recycled_during_completion);

	return m_task_flags.test(_restarted_during_completion_bit);
}

void c_overlapped_task::update(uns32 return_result)
{
	INVOKE_CLASS_MEMBER(0x005A93B0, c_overlapped_task, update, return_result);
}

const char* c_overlapped_task::get_file() const
{
	return m_file;
}

int32 c_overlapped_task::get_line() const
{
	return m_line;
}

void c_overlapped_task::set_file(const char* file)
{
	m_file = file;
}

void c_overlapped_task::set_line(int32 line)
{
	m_line = line;
}

void overlapped_tasks_log_to_debug_txt(e_event_level event_level)
{
	c_async_xoverlapped_scope_lock scope_lock;

	int32 task_index = 0;
	for (int32 task_slot_index = 0; task_slot_index < k_maximum_task_slots; task_slot_index++)
	{
		if (g_overlapped_globals.task_slots[task_slot_index].task)
		{
			s_task_slot* task_slot = &g_overlapped_globals.task_slots[task_slot_index];

			const char* status = NULL;
			switch (task_slot->task->get_task_state())
			{
			case _overlapped_task_state_none:
			{
				status = "none";
			}
			break;
			case _overlapped_task_state_starting:
			{
				status = "starting";
			}
			break;
			case _overlapped_task_state_pending:
			{
				status = "pending";
			}
			break;
			case _overlapped_task_state_completing:
			{
				status = "completing";
			}
			break;
			case _overlapped_task_state_succeeded:
			{
				status = "success";
			}
			break;
			case _overlapped_task_state_failed:
			{
				status = "failure";
			}
			break;
			default:
			{
				status = "<unknown>";
			}
			break;
			}

			event(event_level, "xoverlapped: #%ld task info: context= '%s', status= '%s', file= '%s', line= '%ld'",
				task_index + 1,
				task_slot->task->get_context_string(),
				status,
				task_slot->task->get_file(),
				task_slot->task->get_line());

			task_index++;
		}
	}

}

void overlapped_task_display_task_descriptions()
{
	c_async_xoverlapped_scope_lock scope_lock;

	event(_event_warning, "xoverlapped: dumping task descriptions [count %d]",
		g_overlapped_globals.description_count);

	for (int32 description_index = 0; description_index < g_overlapped_globals.description_count; description_index++)
	{
		event(_event_warning, "xoverlapped: %s",
			g_overlapped_globals.descriptions[description_index].get_string());
	}
}

void overlapped_task_inject_error(const char* context, bool inject_error)
{
	c_async_xoverlapped_scope_lock scope_lock;

	bool context_matches_description = false;
	for (int32 description_index = 0; description_index < g_overlapped_globals.description_count; description_index++)
	{
		if (g_overlapped_globals.descriptions[description_index].is_equal(context))
		{
			context_matches_description = true;
			break;
		}
	}

	if (context_matches_description)
	{
		event(_event_warning, "xoverlapped: setting error injection for %s to %s",
			context,
			inject_error ? "TRUE" : "FALSE");

		g_overlapped_globals.inject_error_context.set(context);
		g_overlapped_globals.inject_error = inject_error;
	}
	else
	{
		event(_event_warning, "xoverlapped: failed to find setting error injection for %s to %s",
			context,
			inject_error ? "TRUE" : "FALSE");
	}
}

void overlapped_task_pause(const char* context, bool pause)
{
	c_async_xoverlapped_scope_lock scope_lock;

	bool context_matches_description = false;
	for (int32 description_index = 0; description_index < g_overlapped_globals.description_count; description_index++)
	{
		if (g_overlapped_globals.descriptions[description_index].is_equal(context))
		{
			context_matches_description = true;
			break;
		}
	}

	if (context_matches_description)
	{
		event(_event_warning, "xoverlapped: setting pause for %s to %s",
			context,
			pause ? "TRUE" : "FALSE");

		g_overlapped_globals.pause_context.set(context);
		g_overlapped_globals.pause = pause;
	}
	else
	{
		event(_event_warning, "xoverlapped: failed to find setting pause for %s to %s",
			context,
			pause ? "TRUE" : "FALSE");
	}
}

