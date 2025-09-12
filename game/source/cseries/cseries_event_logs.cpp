#include "cseries/cseries_event_logs.hpp"

#include "config/version.hpp"
#include "cseries/cseries_events.hpp"
#include "multithreading/synchronization.hpp"
#include "multithreading/threads.hpp"

#include <stdlib.h>

s_event_log_globals event_log_globals;
s_event_log_cache g_event_log_cache;

c_file_output_buffer::c_file_output_buffer() :
	m_file(NULL),
	m_buffer(),
	m_buffer_length(0)
{
}

c_file_output_buffer::~c_file_output_buffer()
{
	flush();
}

void c_file_output_buffer::flush()
{
	if (m_buffer_length > 0)
	{
		ASSERT(m_file);

		file_write(m_file, (uns32)m_buffer_length, m_buffer);
		m_buffer_length = 0;
	}
}

void c_file_output_buffer::initialize_from_reference(s_file_reference* reference)
{
	ASSERT(reference);

	flush();

	m_file = reference;
	m_buffer_length = 0;
}

void c_file_output_buffer::release_reference()
{
	if (m_file)
	{
		if (m_buffer_length > 0)
		{
			file_write(m_file, (uns32)m_buffer_length, m_buffer);
			m_buffer_length = 0;
		}

		m_file = NULL;
		m_buffer_length = 0;
	}
}

void c_file_output_buffer::write(int32 buffer_size, const void* buffer)
{
	if (m_file)
	{
		if ((buffer_size + m_buffer_length) > 0x4000)
		{
			flush();
		}

		if ((m_buffer_length + buffer_size) > 0x4000)
		{
			ASSERT(m_buffer_length == 0);
			file_write(m_file, buffer_size, buffer);
		}
		else
		{
			csmemcpy(&m_buffer[m_buffer_length], buffer, buffer_size);
			m_buffer_length += buffer_size;
		}
	}
}

s_event_log* event_log_get(int32 event_log_index)
{
	ASSERT(event_log_index != NONE);

	s_event_log* event_log = &event_log_globals.event_logs[event_log_index];
	return event_log;
}

int32 event_log_new(const char* event_log_name, c_event_log_flags event_log_flags)
{
	if (event_logs_usable())
	{
		internal_critical_section_enter(k_crit_section_event_logs);

		int32 event_log_index = event_log_globals.event_log_count++;
		s_event_log* event_log = event_log_get(event_log_index);

		csstrnzcpy(event_log->name, event_log_name, NUMBEROF(event_log->name));
		event_log->event_log_flags = event_log_flags;

		internal_critical_section_leave(k_crit_section_event_logs);

		return event_log_index;
	}

	return NONE;
}

void event_logs_close()
{
	flush_event_log_cache();
	
	for (int32 event_log_index = 0; event_log_index < event_log_globals.event_log_count; event_log_index++)
	{
		write_event_log_cache_entry(false, event_log_index, "", true);
	}
}

void event_logs_dispose()
{
	flush_event_log_cache();
	stop_thread(k_thread_network_block_detection);
	event_log_globals.event_log_count = 0;
	event_log_globals.initialized = 0;
}

void event_logs_dispose_atexit()
{
	if (event_log_globals.initialized)
	{
		flush_event_log_cache();
	}
}

void event_logs_flush()
{
	if (event_logs_usable())
	{
		flush_event_log_cache();
	}
}

bool event_logs_get_active_subfolder(const char** p_subfolder)
{
	*p_subfolder = event_log_globals.subfolder;
	return event_logs_using_subfolder();
}

void event_logs_initialize()
{
	event_log_globals.cache_event_log_output = false;
	g_event_log_cache.last_flush_time = system_milliseconds();
	atexit(event_logs_dispose_atexit);
	start_thread(k_thread_network_block_detection);
	event_log_globals.initialized = true;
}

void event_logs_obtain_report_directory_lock()
{

}

void event_logs_release_report_directory_lock()
{

}

void event_logs_snapshot()
{
	s_file_reference debug_file_reference_list[64];
	csmemset(debug_file_reference_list, 0, sizeof(debug_file_reference_list));
	//upload_error_file_start(NUMBEROF(debug_file_reference_list), debug_file_reference_list);
}

void event_logs_specify_subfolder(const char* subfolder)
{
	if (subfolder)
	{
		csstrnzcpy(event_log_globals.subfolder, subfolder, NUMBEROF(event_log_globals.subfolder));
	}
	else
	{
		event_log_globals.subfolder[0] = 0;
	}
}

bool event_logs_usable()
{
	return !thread_has_crashed(k_thread_network_block_detection);
}

bool event_logs_using_subfolder()
{
	return event_log_globals.subfolder[0] != 0;
}

void event_logs_work_function()
{
	suppress_file_errors(true);
	while (!current_thread_should_exit())
	{
		current_thread_update_test_functions();
		if (internal_event_wait_timeout(0, 1000))
		{
			internal_critical_section_enter(k_crit_section_event_logs);
			flush_event_log_cache();
			internal_critical_section_leave(k_crit_section_event_logs);
		}
		switch_to_thread();
	}
	suppress_file_errors(false);
}

void flush_event_log_cache()
{
	if (!event_log_globals.initialized || events_force_no_log)
	{
		return;
	}

	// $IMPLEMENT
}

s_file_reference* acquire_report_file_reference(int32 event_log_index)
{
	s_event_log* event_log = event_log_get(event_log_index);
	if (!event_log->stream_open)
	{
		create_report_file_reference(&event_log->stream, event_log->name, !event_log->event_log_flags.test(_event_log_place_in_root_bit));
		if (!file_exists(&event_log->stream))
		{
			file_create_parent_directories_if_not_present(&event_log->stream);
		}

		bool trim = !event_log->opened_at_least_once && !event_log->event_log_flags.test(_event_log_disable_file_trimming);
		if (!event_log->event_log_flags.test(_event_log_append_to_file_bit) && trim || !file_exists(&event_log->stream))
		{
			file_create(&event_log->stream);
		}

		if (trim)
		{
			file_trim(&event_log->stream, 0x40000);
		}

		constexpr uns32 open_flags
			= FLAG(_file_open_flag_desired_access_write)
			| FLAG(_file_open_flag_set_file_end_and_close)
			| FLAG(_file_open_flag_share_mode_read);

		uns32 error_code = 0;
		if (file_open(&event_log->stream, open_flags, &error_code))
		{
			event_log->stream_open = true;
			event_log->opened_at_least_once = true;
		}
	}

	return &event_log->stream;
}

void release_report_file_reference(int32 event_log_index, bool close_reference)
{
	s_event_log* event_log = event_log_get(event_log_index);
	if (close_reference && event_log->stream_open)
	{
		file_close(&event_log->stream);
		event_log->stream_open = false;
	}
}

void write_raw_to_error_file(c_file_output_buffer* file_buffer, int32 event_log_index, const char* string, bool flush)
{
	if (!events_force_no_log)
	{
		ASSERT(string != NULL);

		uns32 string_length = (uns32)strlen_debug(string);
		if (file_buffer)
		{
			file_buffer->write(string_length, string);
		}
		else if (s_file_reference* reference = acquire_report_file_reference(event_log_index))
		{
			file_write(reference, string_length, string);
			release_report_file_reference(event_log_index, flush);
		}
	}
}

void write_event_log_cache_entry(bool use_report_buffers, int32 event_log_index, const char* string, bool flush)
{
	c_file_output_buffer* output_buffer = NULL;

	internal_critical_section_enter(k_crit_section_event_logs);

	s_event_log* event_log = event_log_get(event_log_index);
	if (!event_log->event_log_flags.test(_event_log_only_for_custom_subfolder) || !event_logs_using_subfolder())
	{
		if (use_report_buffers)
		{
			output_buffer = &event_log->output_buffer;
		}

		if (!event_log->first_line_displayed)
		{
			event_log->first_line_displayed = true;
			write_raw_to_error_file(output_buffer, event_log_index, "\r\n\r\n", flush);
			write_raw_to_error_file(output_buffer, event_log_index, "============================================================================================\r\n", flush);
		}

		write_raw_to_error_file(output_buffer, event_log_index, version_get_full_string() /*shell_get_version()*/, flush);
		write_raw_to_error_file(output_buffer, event_log_index, "\r\n", flush);
		write_raw_to_error_file(output_buffer, event_log_index, "============================================================================================\r\n", flush);
		write_raw_to_error_file(output_buffer, event_log_index, "\r\n\r\n", flush);

		write_raw_to_error_file(output_buffer, event_log_index, string, flush);
	}

	internal_critical_section_leave(k_crit_section_event_logs);
}

void write_to_event_log(const int32* event_log_indices, int32 event_log_count, const char* string)
{
	ASSERT(event_log_globals.initialized);
	if (event_logs_usable())
	{
		ASSERT(event_log_indices);
		ASSERT(event_log_count > 0);
		write_to_event_log_cache(event_log_indices, event_log_count, string);
	}
}

void write_to_event_log_cache(const long* event_log_indices, long event_log_count, const char* string)
{
	ASSERT(string);
	ASSERT(event_log_indices);
	ASSERT(event_log_count > 0);

	internal_critical_section_enter(k_crit_section_event_logs);

	int32 string_size = (int32)strlen_debug(string) + 1;

	suppress_file_errors(true);

	if (!event_log_globals.cache_event_log_output)
	{
		ASSERT(g_event_log_cache.entry_stack.empty());

		for (int32 event_log_num = 0; event_log_num < event_log_count; event_log_num++)
		{
			if (event_log_indices[event_log_num] != NONE)
			{
				write_event_log_cache_entry(false, event_log_indices[event_log_num], string, false);
			}
		}
	}
	else
	{
		if ((g_event_log_cache.entry_string_cache_size + string_size) > sizeof(g_event_log_cache.entry_string_cache))
		{
			flush_event_log_cache();
		}

		ASSERT(!g_event_log_cache.entry_stack.full());

		g_event_log_cache.entry_stack.push();

		s_event_log_cache_entry* entry = g_event_log_cache.entry_stack.get_top();
		if (!entry)
		{
			for (int32 event_log_num = 0; event_log_num < event_log_count; event_log_num++)
			{
				if (event_log_indices[event_log_num] != NONE)
				{
					write_event_log_cache_entry(false, event_log_indices[event_log_num], string, false);
				}
			}
		}
		else
		{
			for (int32 event_log_num = 0; event_log_num < event_log_count; event_log_num++)
			{
				int32 event_log_index = event_log_indices[event_log_num];
				entry->event_log_indices.set(event_log_index, true);
				g_event_log_cache.cached_categories.set(event_log_index, true);
			}

			// copy string into entry_string_cache
			if ((g_event_log_cache.entry_string_cache_size + string_size) <= sizeof(g_event_log_cache.entry_string_cache))
			{
				size_t offset = g_event_log_cache.entry_string_cache_size;
				csstrnzcpy(&g_event_log_cache.entry_string_cache[offset], string, string_size);
				entry->error_string = &g_event_log_cache.entry_string_cache[offset];
				g_event_log_cache.entry_string_cache_size += string_size;

				if (g_event_log_cache.entry_stack.full() || g_event_log_cache.entry_string_cache_size >= 30720)
				{
					internal_event_set(k_event_cseries_event_log);
				}
			}
			else
			{
				entry->error_string = NULL;

				for (int32 event_log_num = 0; event_log_num < event_log_count; event_log_num++)
				{
					if (event_log_indices[event_log_num] != NONE)
					{
						write_event_log_cache_entry(false, event_log_indices[event_log_num], string, false);
					}
				}
			}
		}
	}

	suppress_file_errors(false);
	internal_critical_section_leave(k_crit_section_event_logs);
}

