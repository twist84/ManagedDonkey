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

REFERENCE_DECLARE(0x022B4818, s_async_globals, g_async_globals);

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
}

void __cdecl async_initialize()
{
	INVOKE(0x00508520, async_initialize);
}

dword __cdecl async_main(void* thread_params)
{
	return INVOKE(0x005085A0, async_main, thread_params);
}

long __cdecl async_task_add(e_async_priority priority, s_async_task* task, e_async_category category, e_async_completion(*work_callback)(s_async_task*), c_synchronized_long* done)
{
	//return INVOKE(0x005085C0, async_task_add, priority, task, category, work_callback, done);

	return async_task_add_ex(priority, task, category, work_callback, done, true);
}

long __cdecl async_task_add_ex(e_async_priority priority, s_async_task* task, e_async_category category, e_async_completion(*work_callback)(s_async_task*), c_synchronized_long* done, bool a6)
{
	return INVOKE(0x00508660, async_task_add_ex, priority, task, category, work_callback, done, a6);

	//g_async_globals.__unknown3788++;
	//done = 0;
	//if (s_async_task_element* element = async_task_add_free_list(a6))
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
	//if (task_id < NUMBEROF(g_async_globals.task_list))
	//{
	//	s_async_task_element* element = &g_async_globals.task_list[task_id];
	//	if (element->task_id == task_id)
	//	{
	//		sub_508C30(element);
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
	return INVOKE(0x00508780, async_usable);
}

//.text:005087A0 ; async_work_function

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
				main_render_pregame(2, loading_text.get_string());
				c_rasterizer::end_frame();
			}
		}
	}
}

//.text:00508980 ; async_task_add_free_list

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
//.text:00508B00 ; 
//.text:00508C30 ; 

