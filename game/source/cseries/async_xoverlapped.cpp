#include "cseries/async_xoverlapped.hpp"

#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "xbox/xbox.hpp"

#include <windows.h>

REFERENCE_DECLARE(0x0240AED0, s_overlapped_globals, g_overlapped_globals);

HOOK_DECLARE(0x005A9050, overlapped_update);
HOOK_DECLARE(0x005A9230, task_is_complete);

//HOOK_DECLARE(0x005A8F40, overlapped_task_start_internal);
HOOK_DECLARE_CALL(0x0074CA3C, overlapped_task_start_internal); // c_content_enumeration_overlapped_task::success
HOOK_DECLARE_CALL(0x00A82786, overlapped_task_start_internal); // user_interface_session_string_verify_update

//HOOK_DECLARE_CALL(0x00AA4306, overlapped_task_start_internal); // c_player_profile_interface::update_service_tag

//HOOK_DECLARE_CALL(0x00AD3699, overlapped_task_start_internal); // c_player_marketplace::show_purchase_ui
//HOOK_DECLARE_CALL(0x00AD36F6, overlapped_task_start_internal); // c_player_marketplace::start_asset_consume
//HOOK_DECLARE_CALL(0x00AD3810, overlapped_task_start_internal); // c_player_marketplace::start_assets_enumerate
HOOK_DECLARE_CALL(0x00AE668B, overlapped_task_start_internal); // c_saved_film_take_screenshot::update
HOOK_DECLARE_CALL(0x00AE6868, overlapped_task_start_internal); // c_saved_film_take_screenshot::update
HOOK_DECLARE_CALL(0x00AEA33B, overlapped_task_start_internal); // ?
HOOK_DECLARE_CALL(0x00AF0D67, overlapped_task_start_internal); // c_gui_start_menu_hq_service_record_file_share_choose_item::__vftable50
HOOK_DECLARE_CALL(0x00AF0FBE, overlapped_task_start_internal); // c_gui_start_menu_hq_service_record_file_share_choose_item::__vftable49
HOOK_DECLARE_CALL(0x00AF26D6, overlapped_task_start_internal); // c_gui_start_menu_hq_service_record_file_share_item_selected::__vftable50
HOOK_DECLARE_CALL(0x00AF5088, overlapped_task_start_internal); // c_gui_start_menu_hq_screenshots_options::__vftable24
HOOK_DECLARE_CALL(0x00AF52D2, overlapped_task_start_internal); // c_gui_start_menu_hq_screenshots_options::__vftable24
HOOK_DECLARE_CALL(0x00AF8495, overlapped_task_start_internal); // c_start_menu_settings_appearance_emblem::handle_list_item_chosen
HOOK_DECLARE_CALL(0x00B0373C, overlapped_task_start_internal); // c_gui_screen_pregame_selection::begin_selected_item_text_edit
HOOK_DECLARE_CALL(0x00B0B8D6, overlapped_task_start_internal); // ?
HOOK_DECLARE_CALL(0x00B0BAA0, overlapped_task_start_internal); // ?
HOOK_DECLARE_CALL(0x00B0DA48, overlapped_task_start_internal); // c_gui_screen_game_options::update_save_as_new_operation

// #TODO: add me to `s_overlapped_globals`
c_static_array<XOVERLAPPED, 64> task_slot_overlapped;

PXOVERLAPPED find_overlapped(s_task_slot* task_slot)
{
	PXOVERLAPPED overlapped = NULL;

	for (long i = 0; i < 64 || overlapped == NULL; i++)
	{
		if (&g_overlapped_globals.task_slots[i] == task_slot)
			overlapped = &task_slot_overlapped[i];
	}

	return overlapped;
}

c_async_xoverlapped_scope_lock::c_async_xoverlapped_scope_lock()
{
	if (game_is_multithreaded())
		internal_critical_section_enter(_critical_section_xoverlapped_memory);
}

c_async_xoverlapped_scope_lock::~c_async_xoverlapped_scope_lock()
{
	if (game_is_multithreaded())
		internal_critical_section_leave(_critical_section_xoverlapped_memory);
}

bool c_overlapped_task::busy() const
{
	//return DECLFUNC(0x00476C30, bool, __thiscall, c_overlapped_task const*)(this);

	return m_task_state == _overlapped_task_state_starting
		|| m_task_state == _overlapped_task_state_pending
		|| m_task_state == _overlapped_task_state_completing;
}

c_overlapped_task* c_overlapped_task::constructor(char const* file, long line)
{
	//DECLFUNC(0x005A8C80, c_overlapped_task*, __thiscall, c_overlapped_task const*, char const*, long)(this, file, line);

	REFERENCE_DECLARE(this, dword, vftable);
	vftable = 0x0165B220;

	m_task_flags.clear();
	set_file(file);
	set_line(line);

	set_task_state_internal(_overlapped_task_state_none);

	return this;
}

//.text:005A8CB0 ; public: virtual __cdecl c_overlapped_task::~c_overlapped_task()
//.text:005A8CE0 ; public: virtual void* __cdecl c_overlapped_task::`vector deleting destructor'(unsigned int)
//.text:005A8D30 ; public: virtual void __cdecl c_overlapped_task::complete()
//.text:005A8D40 ; public: virtual void __cdecl c_overlapped_task::failure(dword calling_result, dword overlapped_error, dword overlapped_extended_error)
//.text:005A8D50 ; s_task_slot* __cdecl find_task_slot(c_overlapped_task const* task)
//.text:005A8DA0 ; public: virtual bool __cdecl c_overlapped_task::is_result_successful(dword calling_result, dword overlapped_error, dword overlapped_extended_error)

void __cdecl overlapped_dispose()
{
	INVOKE(0x005A8DD0, overlapped_dispose);
}

void __cdecl overlapped_initialize()
{
	INVOKE(0x005A8DE0, overlapped_initialize);
}

void __cdecl overlapped_render()
{
	INVOKE(0x005A8E20, overlapped_render);
}

//.text:005A8E30 ; void __cdecl overlapped_task_block_until_finished(c_overlapped_task const* task)
//.text:005A8ED0 ; bool __cdecl overlapped_task_is_running(c_overlapped_task const* task)

// #TODO: completely rewrite this implementaion
bool __cdecl overlapped_task_start_internal(c_overlapped_task* task, char const* file, long line)
{
	//return INVOKE(0x005A8F40, overlapped_task_start_internal, task, file, line);

	ASSERT(task);

	bool result = false;
	task->set_file(file);
	task->set_line(line);

	//dword result = task->start(get_donkey_module());
	//
	//if (result == E_FAIL || result == 0xFFFFFFFF)
	//	return false;
	//
	//task->m_task_state = _overlapped_task_state_succeeded;
	//
	//return true;

	c_async_xoverlapped_scope_lock scope_lock;

	s_task_slot* first_free_task_slot = NULL;
	for (long task_slot_index = 0; task_slot_index < 64; task_slot_index++)
	{
		if (!g_overlapped_globals.task_slots[task_slot_index].task)
		{
			first_free_task_slot = &g_overlapped_globals.task_slots[task_slot_index];
			break;
		}
	}

	if (first_free_task_slot)
	{
		first_free_task_slot->task = task;
		first_free_task_slot->task->set_task_state_internal(_overlapped_task_state_starting);

		//bool context_matches_description = false;
		//if (strlen(task->get_context_string()) < 64)
		//{
		//	for (long i = 0; i < g_overlapped_globals.description_count; i++)
		//	{
		//		if (g_overlapped_globals.descriptions[i].is_equal(task->get_context_string()))
		//		{
		//			context_matches_description = true;
		//			break;
		//		}
		//	}
		//
		//	if (!context_matches_description)
		//	{
		//		if (g_overlapped_globals.description_count < g_overlapped_globals.descriptions.get_count())
		//		{
		//			generate_event(_event_level_warning, "xoverlapped: task context string '%s' not added to error-code-injection table, maximum tasks already being tracked!",
		//				task->get_context_string());
		//		}
		//		else
		//		{
		//			g_overlapped_globals.descriptions[g_overlapped_globals.description_count++].set(task->get_context_string());
		//		}
		//	}
		//	else
		//	{
		//		generate_event(_event_level_warning, "xoverlapped: task context string '%s' is too long to fit into error-code-injection table (maximum %ld characters)",
		//			task->get_context_string(),
		//			64);
		//	}
		//}

		result = true;
	}
	else
	{
		generate_event(_event_level_status, "xoverlapped: overlapped_task_start() failed: maximum tasks already active!");
	}

	return result;
}

//.text:005A8F50 ; void __cdecl overlapped_task_terminate(c_overlapped_task* task)
//.text:005A8FA0 ; void __cdecl overlapped_task_toggle_debug_rendering(bool toggle_debug_rendering)
//.text:005A8FB0 ; void __cdecl overlapped_task_wait_for_all_tasks_to_finish()

void __cdecl overlapped_update()
{
	//INVOKE(0x005A9050, overlapped_update);

	for (s_task_slot& task_slot : g_overlapped_globals.task_slots)
	{
		{
			c_async_xoverlapped_scope_lock scope_lock;

			if (task_slot.task && task_slot.task->get_task_state() == _overlapped_task_state_pending)
			{
				dword return_result = ERROR_SUCCESS;
				dword calling_result = ERROR_SUCCESS;
				dword overlapped_error = ERROR_SUCCESS;
				dword overlapped_extended_error = ERROR_SUCCESS;
				if (task_is_complete(&task_slot, &return_result, &calling_result, &overlapped_error, &overlapped_extended_error))
					task_now_finished(&task_slot, return_result, calling_result, overlapped_error, overlapped_extended_error);
				else
					task_slot.task->update(return_result);
			}
		}

		// missing x360 overlapped logic
		{
			c_async_xoverlapped_scope_lock scope_lock;
		
			// missing x360 logic
			if (task_slot.task && task_slot.task->get_task_state() == _overlapped_task_state_starting)
			{
				long pending_task_count = 0;
			
				for (s_task_slot& pending_task_slot : g_overlapped_globals.task_slots)
				{
					if (pending_task_slot.task && pending_task_slot.task->get_task_state() == _overlapped_task_state_pending)
						pending_task_count++;
				}
			
				if (pending_task_count < 3)
				{
					PXOVERLAPPED overlapped = find_overlapped(&task_slot);
		
					//task_slot.calling_result = task_slot.task->start(&task_slot.overlapped);
					task_slot.calling_result = task_slot.task->start(overlapped);
					task_slot.task->set_task_state_internal(_overlapped_task_state_pending);
					task_slot.terminated = false;
				}
			}
		}
	}
}

//.text:005A9130 ; public: virtual void __cdecl c_overlapped_task::reset()

e_overlapped_task_state c_overlapped_task::get_task_state() const
{
	return m_task_state;
}

void c_overlapped_task::set_task_state_internal(e_overlapped_task_state task_state)
{
	//DECLFUNC(0x005A91B0, void, __thiscall, c_overlapped_task const*, e_overlapped_task_state)(this, task_state);

	m_task_state = task_state;
}

//.text:005A91D0 ; public: virtual void __cdecl c_overlapped_task::success(dword return_result)
//.text:005A91E0 ; void __cdecl task_block_until_finished(s_task_slot* task_slot)

bool __cdecl task_is_complete(s_task_slot* task_slot, dword* return_result, dword* calling_result, dword* overlapped_error, dword* overlapped_extended_error)
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

	//// windows logic, comment me out
	//return task_slot->task->get_task_state() != _overlapped_task_state_starting && task_slot->calling_result != ERROR_IO_PENDING;

	// missing x360 overlapped logic
	
	bool result = true;
	if (task_slot->task->get_task_state() == _overlapped_task_state_starting)
	{
		result = false;
	}
	else if (task_slot->calling_result == ERROR_IO_PENDING)
	{
		result = false;
	
		PXOVERLAPPED overlapped = find_overlapped(task_slot);
	
		//if (task_slot->overlapped.InternalLow != ERROR_IO_PENDING)
		if (overlapped->InternalLow != ERROR_IO_PENDING)
		{
			result = true;
			//*overlapped_error = XGetOverlappedResult(&task_slot->overlapped, return_result, FALSE);
			*overlapped_error = XGetOverlappedResult(overlapped, return_result, FALSE);
			if (*overlapped_error)
			{
				if (*overlapped_error != ERROR_IO_INCOMPLETE && *overlapped_error != ERROR_IO_PENDING)
					*overlapped_extended_error = XGetOverlappedExtendedError(overlapped);
			}
		}
	}
	
	//if (g_overlapped_globals.paused)
	//{
	//	if (g_overlapped_globals.pause_context.is_equal(task_slot->task->get_context_string()))
	//		result = false;
	//}
	
	return result;
}

void __cdecl task_now_finished(s_task_slot* task_slot, dword return_result, dword calling_result, dword overlapped_error, dword overlapped_extended_error)
{
	INVOKE(0x005A9280, task_now_finished, task_slot, return_result, calling_result, overlapped_error, overlapped_extended_error);

	//ASSERT(task_slot->task != NULL);
	//ASSERT(task_slot->task->get_task_state() == _overlapped_task_state_starting || task_slot->task->get_task_state() == _overlapped_task_state_pending);

	// function seems fine other than needing the missing overlapped memset

	PXOVERLAPPED overlapped = find_overlapped(task_slot);
	csmemset(overlapped, 0, sizeof(XOVERLAPPED));
}

//.text:005A9360 ; public: void __cdecl c_overlapped_task::task_recycled_during_completion(bool)
//.text:005A9380 ; public: bool __cdecl c_overlapped_task::task_was_recycled_during_completion() const

