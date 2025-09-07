#include "cseries/async.hpp"

#include "cseries/cseries.hpp"
#include "main/loading.hpp"
#include "main/main.hpp"
#include "main/main_render.hpp"
#include "memory/module.hpp"
#include "multithreading/threads.hpp"
#include "networking/network_globals.hpp"
#include "rasterizer/rasterizer.hpp"
#include "simulation/simulation.hpp"
#include "sound/sound_manager.hpp"
#include "multithreading/synchronization.hpp"

REFERENCE_DECLARE(0x022B4818, s_async_globals, async_globals);

HOOK_DECLARE(0x005085A0, async_main);
HOOK_DECLARE(0x005085C0, async_task_add);
HOOK_DECLARE(0x00508A20, internal_async_yield_until_done);
HOOK_DECLARE(0x00508A40, internal_async_yield_until_done_attributed);
HOOK_DECLARE(0x00508A60, internal_async_yield_until_done_with_networking);

bool __cdecl async_busy_hint()
{
	return INVOKE(0x00508460, async_busy_hint);
}

bool __cdecl async_category_in_queue(e_async_category category)
{
	return INVOKE(0x00508470, async_category_in_queue, category);
}

void __cdecl async_dispose()
{
	INVOKE(0x005084D0, async_dispose);
}

void __cdecl async_idle()
{
	INVOKE(0x005084E0, async_idle);

	//async_globals.cached_tasks_in_queue = async_tasks_in_queue();
}

void __cdecl async_initialize()
{
	INVOKE(0x00508520, async_initialize);

	//async_helpers_initialize();
	//csmemset(async_globals.free_list_blocks, 0, sizeof(async_globals.free_list_blocks));
	//for (int32 i = 0; i < NUMBEROF(async_globals.free_list_blocks) - 1; i++)
	//{
	//	s_async_queue_element* task = &async_globals.free_list_blocks[i];
	//	s_async_queue_element* next_task = &async_globals.free_list_blocks[i + 1];
	//	task->next = next_task;
	//	next_task->task_id = INVALID_ASYNC_TASK_ID;
	//}
	//async_globals.free_list_blocks[NUMBEROF(async_globals.free_list_blocks) - 1].next = NULL;
	//async_globals.free_list = async_globals.free_list_blocks;
	//async_globals.work_list = NULL;
	//async_task_buffer_initialize();
	//start_thread(k_thread_async_io);
	//if (shell_application_type())
	//	thread_set_priority(k_thread_async_io, _thread_priority_above_normal);
}

uns32 __cdecl async_main(void* thread_params)
{
	//return INVOKE(0x005085A0, async_main, thread_params);

	async_work_function();
	return 0;
}

int32 __cdecl async_task_add(e_async_priority priority, s_async_task* task, e_async_category category, e_async_completion(*work_callback)(s_async_task*), c_synchronized_long* done)
{
	//return INVOKE(0x005085C0, async_task_add, priority, task, category, work_callback, done);

	return async_task_add_ex(priority, task, category, work_callback, done, true);
}

int32 __cdecl async_task_add_ex(e_async_priority priority, s_async_task* task, e_async_category category, e_async_completion(*work_callback)(s_async_task*), c_synchronized_long* done, bool a6)
{
	return INVOKE(0x00508660, async_task_add_ex, priority, task, category, work_callback, done, a6);

	//ASSERT(done);
	//async_globals.cached_tasks_in_queue++;
	//done = 0;
	//if (s_async_queue_element* element = free_list_get_and_remove(a6))
	//{
	//	element->task = *task;
	//	element->priority = priority;
	//	element->work_callback = work_callback;
	//	element->done = done;
	//	element->category = category;
	//	return work_list_add(element);
	//}
	//return NONE;
}

bool __cdecl async_task_change_priority(int32 task_id, e_async_priority priority)
{
	return INVOKE(0x005086D0, async_task_change_priority, task_id, priority);

	//bool result = false;
	//internal_mutex_take(k_mutex_async_work_list); // mutex_async_work_take
	//if (task_id < NUMBEROF(async_globals.free_list_blocks))
	//{
	//	s_async_queue_element* element = &async_globals.free_list_blocks[task_id];
	//	if (element->task_id == task_id)
	//	{
	//		work_list_remove_internal_assumes_locked_does_not_clear_id_does_not_suspend(element);
	//		element->priority = priority;
	//		work_list_add_internal_assumes_locked_does_not_set_id_does_not_resume(element);
	//		result = true;
	//	}
	//}
	//internal_mutex_release(k_mutex_async_work_list); // mutex_async_work_release
	//return result;
}

int32 __cdecl async_tasks_in_queue()
{
	return INVOKE(0x00508730, async_tasks_in_queue);
}

bool __cdecl async_test_completion_flag(c_synchronized_long* completion_flag)
{
	//return INVOKE(0x00508760, async_test_completion_flag, completion_flag);

	return completion_flag->peek() != 0;
}

bool __cdecl async_usable()
{
	//return INVOKE(0x00508780, async_usable);

	return !thread_has_crashed(k_thread_async_io);
}

bool __cdecl async_work_function()
{
	//INVOKE(0x005087A0, async_work_function);

	bool should_exit = false;
	while (!should_exit)
	{
		should_exit = current_thread_should_exit();
		if (should_exit)
			break;

		current_thread_update_test_functions();

		internal_semaphore_take(k_semaphore_async_work);

		async_globals.current_thread = work_list_get();

		//if (async_globals.async_work_delay_milliseconds > 0)
		//	sleep(async_globals.async_work_delay_milliseconds);

		if (async_globals.current_thread)
		{
			e_async_completion completion_status = async_globals.current_thread->work_callback(&async_globals.current_thread->work);
			//g_statistics.work_callbacks++;

			if (completion_status)
			{
				//g_statistics.work_retired++;
				if (completion_status == _async_completion_done)
				{
					if (async_globals.current_thread->done)
						*async_globals.current_thread->done = true;
				}

				work_list_remove(async_globals.current_thread);
				free_list_add(async_globals.current_thread);
			}
			else
			{
				//g_statistics.__unknown1C_work_list = internal_semaphore_release(k_semaphore_async_work);
				internal_semaphore_release(k_semaphore_async_work);
				sleep(0);
			}
		}
	}

	return should_exit;
}

void __cdecl async_yield_until_done_function(c_synchronized_long* done, bool(*yield_function)(c_synchronized_long*), bool idle, bool networking, bool spinner, e_yield_reason yield_reason)
{
	//INVOKE(0x005088D0, async_yield_until_done_function, done, yield_function, idle, networking, spinner, yield_reason);

	if (!yield_function(done))
	{
		int32 spinner_state_index = 0;

		while (!yield_function(done))
		{
			if (spinner)
				main_loading_enable_spinner(true);

			switch_to_thread();

			if (!async_usable()) // thread_has_crashed(k_thread_async_io)
				exceptions_update();

			//if (networking)
			//{
			//	network_receive();
			//	network_update();
			//	network_send();
			//}

			if (idle)
			{
				sound_idle();
			}

			main_loop_pregame();

			if (spinner)
			{
				main_loading_enable_spinner(false);
			}

			if (spinner)
			{
				const char* spinner_states[] = { "/", "-", "\\" };
				spinner_state_index = 8 * system_milliseconds() / 1000 % NUMBEROF(spinner_states);

				c_static_string<64> loading_text;
				loading_text.print("loading %s", spinner_states[spinner_state_index]);

				RENDER_THREAD_LOCK;
				main_render_pregame(_main_pregame_frame_loading_debug, loading_text.get_string());
				c_rasterizer::end_frame();
			}
		}
	}
}

void __cdecl free_list_add(s_async_queue_element* element)
{
	//INVOKE(0x00508950, free_list_add, element);

	// mutex_async_free_take
	internal_mutex_take(k_mutex_async_free_list);

	element->next = async_globals.free_list;
	async_globals.free_list = element;
	ASSERT(async_globals.free_list->next != async_globals.free_list);

	// mutex_async_free_release
	internal_mutex_release(k_mutex_async_free_list);
}

s_async_queue_element* __cdecl free_list_get_and_remove(bool block_if_task_list_is_full)
{
	//return INVOKE(0x00508980, free_list_get_and_remove, block_if_task_list_is_full);

	bool stalled = false;
	s_async_queue_element* free_list = NULL;
	while (!free_list)
	{
		// mutex_async_free_take
		internal_mutex_take(k_mutex_async_free_list);

		if (async_globals.free_list)
		{
			ASSERT(async_globals.free_list->next != async_globals.free_list);
			free_list = async_globals.free_list;
			async_globals.free_list = async_globals.free_list->next;
			ASSERT(async_globals.free_list->next != async_globals.free_list);
		}

		// mutex_async_free_release
		internal_mutex_release(k_mutex_async_free_list);

		if (!free_list)
		{
			if (!block_if_task_list_is_full)
				break;

			if (!stalled)
			{
				stalled = true;
				//g_statistics.free_list_get_stalls++;
			}

			main_loop_pregame();
			switch_to_thread();
		}
	}

	return free_list;
}

void __cdecl internal_async_yield_until_done(c_synchronized_long* done, bool idle_sound, bool show_debug_progress, const char* file, int32 line)
{
	//INVOKE(0x00508A20, internal_async_yield_until_done, done, idle, show_debug_progress, file, line);

	async_yield_until_done_function(done, simple_yield_function, idle_sound, false, show_debug_progress, _yield_for_unknown);
}

void __cdecl internal_async_yield_until_done_attributed(c_synchronized_long* done, bool idle_sound, bool show_debug_progress, e_yield_reason yield_reason, const char* file, int32 line)
{
	//INVOKE(0x00508A40, internal_async_yield_until_done_attributed, done, idle, show_debug_progress, yield_reason, file, line);

	async_yield_until_done_function(done, simple_yield_function, idle_sound, false, show_debug_progress, yield_reason);
}

void __cdecl internal_async_yield_until_done_with_networking(c_synchronized_long* done, bool idle_sound, bool show_debug_progress, const char* file, int32 line)
{
	//INVOKE(0x00508A60, internal_async_yield_until_done_with_networking, done, idle_sound, show_debug_progress, file, line);

	async_yield_until_done_function(done, simple_yield_function, idle_sound, true, show_debug_progress, _yield_for_unknown);
}

bool __cdecl simple_yield_function(c_synchronized_long* done)
{
	//return INVOKE(0x00508A80, simple_yield_function, done);

	return async_test_completion_flag(done);
}

int32 __cdecl work_list_add(s_async_queue_element* element)
{
	return INVOKE(0x00508AA0, work_list_add, element);
}

void __cdecl work_list_add_internal_assumes_locked_does_not_set_id_does_not_resume(s_async_queue_element* element)
{
	INVOKE(0x00508B00, work_list_add_internal_assumes_locked_does_not_set_id_does_not_resume, element);
}

s_async_queue_element* __cdecl work_list_get()
{
	//return INVOKE(0x00508BD0, work_list_get);

	work_list_lock_internal();
	s_async_queue_element* work_list = async_globals.work_list;
	work_list_unlock();
	return work_list;
}

void __cdecl work_list_lock_internal()
{
	//INVOKE(0x00508BF0, work_list_lock_internal);

	internal_mutex_take(k_mutex_async_work_list);
}

void __cdecl work_list_remove(s_async_queue_element* element)
{
	//INVOKE(0x00508C00, work_list_remove, element);

	work_list_lock_internal();
	work_list_remove_internal_assumes_locked_does_not_clear_id_does_not_suspend(element);
	element->task_id = INVALID_ASYNC_TASK_ID;
	work_list_unlock();
}

void __cdecl work_list_remove_internal_assumes_locked_does_not_clear_id_does_not_suspend(s_async_queue_element* element)
{
	//INVOKE(0x00508C30, work_list_remove_internal_assumes_locked_does_not_clear_id_does_not_suspend, element);

	ASSERT(async_globals.work_list != NULL);
	if (async_globals.work_list == element)
	{
		async_globals.work_list = element->next;
	}
	else
	{
		s_async_queue_element* work_list = async_globals.work_list;
		while (work_list->next != element)
			work_list = work_list->next;
		work_list->next = element->next;
	}
}

void __cdecl work_list_unlock()
{
	//INVOKE(0x00508C80, work_list_unlock);

	internal_mutex_release(k_mutex_async_work_list);
}

