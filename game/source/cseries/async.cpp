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

	//async_globals.tasks_in_queue = async_tasks_in_queue();
}

void __cdecl async_initialize()
{
	INVOKE(0x00508520, async_initialize);
}

dword __cdecl async_main(void* thread_params)
{
	//return INVOKE(0x005085A0, async_main, thread_params);

	async_work_function();
	return 0;
}

long __cdecl async_task_add(e_async_priority priority, s_async_task* task, e_async_category category, e_async_completion(*work_callback)(s_async_task*), c_synchronized_long* done)
{
	//return INVOKE(0x005085C0, async_task_add, priority, task, category, work_callback, done);

	return async_task_add_ex(priority, task, category, work_callback, done, true);
}

long __cdecl async_task_add_ex(e_async_priority priority, s_async_task* task, e_async_category category, e_async_completion(*work_callback)(s_async_task*), c_synchronized_long* done, bool a6)
{
	return INVOKE(0x00508660, async_task_add_ex, priority, task, category, work_callback, done, a6);

	//ASSERT(done);
	//async_globals.tasks_in_queue++;
	//done = 0;
	//if (s_async_queue_element* element = free_list_get_and_remove(a6))
	//{
	//	element->task = *task;
	//	element->priority = priority;
	//	element->work_callback = work_callback;
	//	element->done = done;
	//	element->category = category;
	//	return async_task_add_work_list(element);
	//}
	//return NONE;
}

bool __cdecl async_task_change_priority(long task_id, e_async_priority priority)
{
	return INVOKE(0x005086D0, async_task_change_priority, task_id, priority);

	//bool result = false;
	//internal_mutex_take(_synchronization_mutex_async_work); // mutex_async_work_take
	//if (task_id < NUMBEROF(async_globals.task_list))
	//{
	//	s_async_queue_element* element = &async_globals.task_list[task_id];
	//	if (element->task_id == task_id)
	//	{
	//		work_list_remove_internal_assumes_locked_does_not_clear_id_does_not_suspend(element);
	//		element->priority = priority;
	//		sub_508B00(element);
	//		result = true;
	//	}
	//}
	//internal_mutex_release(_synchronization_mutex_async_work); // mutex_async_work_release
	//return result;
}

long __cdecl async_tasks_in_queue()
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

		internal_semaphore_take(_synchronization_semaphore_async_work);

		async_globals.temp_list = sub_508BD0();

		//if (async_globals.work_delay_milliseconds > 0)
		//	sleep(async_globals.work_delay_milliseconds);

		if (async_globals.temp_list)
		{
			e_async_completion completion_status = async_globals.temp_list->work_callback(&async_globals.temp_list->task);
			//g_statistics.work_callbacks++;

			if (completion_status)
			{
				//g_statistics.work_retired++;
				if (completion_status == _async_completion_done)
				{
					if (async_globals.temp_list->done)
						*async_globals.temp_list->done = true;
				}

				sub_508C00(async_globals.temp_list);
				free_list_add(async_globals.temp_list);
			}
			else
			{
				//g_statistics.__unknown1C_work_list = internal_semaphore_release(_synchronization_semaphore_async_work);
				internal_semaphore_release(_synchronization_semaphore_async_work);
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
		long spinner_state_index = 0;

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
				sound_idle();

			main_loop_pregame();

			if (spinner)
				main_loading_enable_spinner(false);

			if (spinner)
			{
				char const* spinner_states[] = { "/", "-", "\\" };
				spinner_state_index = 8 * system_milliseconds() / 1000 % NUMBEROF(spinner_states);

				c_static_string<64> loading_text;
				loading_text.print("loading %s", spinner_states[spinner_state_index]);

				c_wait_for_render_thread wait_for_render_thread(__FILE__, __LINE__);
				main_render_pregame(_main_pregame_frame_progress_report, loading_text.get_string());
				c_rasterizer::end_frame();
			}
		}
	}
}

void __cdecl free_list_add(s_async_queue_element* element)
{
	//INVOKE(0x00508950, free_list_add, element);

	// mutex_async_free_take
	internal_mutex_take(_synchronization_mutex_async_free);

	element->next = async_globals.free_list;
	async_globals.free_list = element;
	ASSERT(async_globals.free_list->next != async_globals.free_list);

	// mutex_async_free_release
	internal_mutex_release(_synchronization_mutex_async_free);
}

s_async_queue_element* __cdecl free_list_get_and_remove(bool a1)
{
	//return INVOKE(0x00508980, free_list_get_and_remove, a1);

	bool stalled = false;
	s_async_queue_element* free_list = NULL;
	while (!free_list)
	{
		// mutex_async_free_take
		internal_mutex_take(_synchronization_mutex_async_free);

		if (async_globals.free_list)
		{
			ASSERT(async_globals.free_list->next != async_globals.free_list);
			free_list = async_globals.free_list;
			async_globals.free_list = async_globals.free_list->next;
			ASSERT(async_globals.free_list->next != async_globals.free_list);
		}

		// mutex_async_free_release
		internal_mutex_release(_synchronization_mutex_async_free);

		if (!free_list)
		{
			if (!a1)
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

void __cdecl internal_async_yield_until_done(c_synchronized_long* done, bool idle, bool spinner, char const* file, long line)
{
	//INVOKE(0x00508A20, internal_async_yield_until_done, done, idle, spinner, file, line);

	async_yield_until_done_function(done, simple_yield_function, idle, false, spinner, (e_yield_reason)0);
}

void __cdecl internal_async_yield_until_done_attributed(c_synchronized_long* done, bool idle, bool spinner, e_yield_reason yield_reason, char const* file, long line)
{
	//INVOKE(0x00508A40, internal_async_yield_until_done_attributed, done, idle, spinner, yield_reason, file, line);

	async_yield_until_done_function(done, simple_yield_function, idle, false, spinner, yield_reason);
}

void __cdecl internal_async_yield_until_done_with_networking(c_synchronized_long* done, bool idle, bool spinner, char const* file, long line)
{
	//INVOKE(0x00508A60, internal_async_yield_until_done_with_networking, done, idle, spinner, file, line);

	async_yield_until_done_function(done, simple_yield_function, idle, true, spinner, (e_yield_reason)0);
}

bool __cdecl simple_yield_function(c_synchronized_long* done)
{
	//return INVOKE(0x00508A80, simple_yield_function, done);

	return async_test_completion_flag(done);
}

//.text:00508AA0 ; async_task_add_work_list

void __cdecl work_list_add_internal_assumes_locked_does_not_set_id_does_not_resume(s_async_queue_element* element)
{
	INVOKE(0x00508B00, work_list_add_internal_assumes_locked_does_not_set_id_does_not_resume, element);
}

s_async_queue_element* __cdecl sub_508BD0()
{
	//return INVOKE(0x00508BD0, sub_508BD0);

	internal_mutex_take(_synchronization_mutex_async_work);
	s_async_queue_element* work_list = async_globals.work_list;
	internal_mutex_release(_synchronization_mutex_async_work);
	return work_list;
}

void __cdecl sub_508C00(s_async_queue_element* element)
{
	//INVOKE(0x00508C00, sub_508C00, element);

	internal_mutex_take(_synchronization_mutex_async_work);
	work_list_remove_internal_assumes_locked_does_not_clear_id_does_not_suspend(element);
	element->task_id = NONE;
	internal_mutex_release(_synchronization_mutex_async_work);
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

