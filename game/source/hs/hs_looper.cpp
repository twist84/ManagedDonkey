#include "hs/hs_looper.hpp"

#include "cseries/cseries_events.hpp"
#include "hs/hs_runtime.hpp"
#include "hs_compile.hpp"

static c_hs_looper g_hs_looper{};

c_hs_looper::c_hs_looper() :
	m_loop_expression(),
	m_loop_thread_index(NONE),
	m_loop_enabled(false)
{
}

void c_hs_looper::clear()
{
	m_loop_expression.clear();

	if (m_loop_thread_index != NONE)
	{
		hs_thread_try_to_delete(m_loop_thread_index, true);
		m_loop_thread_index = NONE;
	}
}

int32 c_hs_looper::create_suspended_loop_thread()
{
	int32 loop_thread_index = NONE;
	if (!m_loop_expression.is_empty())
	{
		char valid_loop_expression[4096]{};
		hs_validify_expression(m_loop_expression.get_string(), valid_loop_expression, NUMBEROF(valid_loop_expression));

		if (hs_runtime_safe_to_gc())
		{
			hs_node_gc();
		}

		const char* error_message = nullptr;
		const char* error_source = nullptr;
		int32 loop_expression_index = hs_compile_expression(csstrnlen(valid_loop_expression, sizeof(valid_loop_expression)), valid_loop_expression, &error_message, &error_source);
		if (loop_expression_index != NONE)
		{
			int32 thread_index = hs_thread_new(_hs_thread_type_runtime_evaluate, NONE, false);
			if (thread_index != NONE)
			{
				hs_thread* thread = hs_thread_get(thread_index);
				thread->flags |= FLAG(_hs_thread_display_expression_bit);

				hs_destination_pointer destination{};
				destination.destination_type = _hs_destination_thread_result;
				hs_evaluate(thread_index, loop_expression_index, destination, nullptr);
				if (TEST_BIT(thread->flags, _hs_thread_in_function_call_bit))
				{
					loop_thread_index = thread_index;
				}
				else
				{
					hs_thread_delete(thread_index, true);
				}
			}
			else
			{
				event(_event_critical, "hs_runtime_evaluate: could not allocate thread to execute a command!");
			}
		}
		else if (error_message)
		{
			hs_compile_source_error(nullptr, error_message, error_source, valid_loop_expression);
			m_loop_enabled = false;
		}
	}
	return loop_thread_index;
}

void c_hs_looper::game_tick()
{
	if (m_loop_thread_index == NONE && m_loop_enabled)
	{
		int32 suspended_loop_thread = c_hs_looper::create_suspended_loop_thread();
		m_loop_thread_index = suspended_loop_thread;
		if (suspended_loop_thread != NONE)
		{
			hs_thread_main(suspended_loop_thread);
		}
	}
}

void c_hs_looper::handle_thread_delete(int32 thread_index)
{
	if (m_loop_thread_index == thread_index)
	{
		m_loop_thread_index = NONE;
	}
}

void c_hs_looper::reboot_loop()
{
	m_loop_enabled = true;
}

void c_hs_looper::reinitialize()
{
	m_loop_thread_index = NONE;
	m_loop_enabled = true;
}

void c_hs_looper::restore_from_saved_game()
{
	m_loop_thread_index = NONE;
}

void c_hs_looper::set_loop_expression(const char* new_loop_expression)
{
	c_hs_looper::clear();
	m_loop_expression.set(new_loop_expression);
	m_loop_enabled = true;
}

void hs_looper_clear()
{
	g_hs_looper.clear();
}

void hs_looper_game_tick()
{
	g_hs_looper.game_tick();
}

void hs_looper_handle_thread_delete(int32 thread_index)
{
	g_hs_looper.handle_thread_delete(thread_index);
}

void hs_looper_reboot()
{
	g_hs_looper.reboot_loop();
}

void hs_looper_reinitialize()
{
	g_hs_looper.reinitialize();
}

void hs_looper_restore_from_saved_game()
{
	g_hs_looper.restore_from_saved_game();
}

void hs_looper_setup(const char* expression)
{
	g_hs_looper.set_loop_expression(expression);
}

