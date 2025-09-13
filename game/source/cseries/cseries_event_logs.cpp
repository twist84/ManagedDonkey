#include "cseries/cseries_event_logs.hpp"

#include "config/version.hpp"
#include "cseries/cseries_events.hpp"
#include "multithreading/synchronization.hpp"
#include "multithreading/threads.hpp"
#include "networking/network_time.hpp"
#include "profiler/profiler.hpp"

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

		if (m_file)
		{
			file_write(m_file, (uns32)m_buffer_length, m_buffer);
		}
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
		flush();
		m_file = NULL;
		m_buffer_length = 0;
	}
}

void c_file_output_buffer::write(int32 buffer_size, const void* buffer)
{
	if (m_file)
	{
		if (m_buffer_length + buffer_size > sizeof(m_buffer))
		{
			flush();
		}

		if (m_buffer_length + buffer_size > sizeof(m_buffer))
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
	int32 event_log_index = NONE;
	if (event_logs_usable() && event_log_globals.event_logs.get_count() >= 0)
	{
		c_critical_section_scope section_scope(k_crit_section_event_logs);

		event_log_index = event_log_globals.event_log_count++;
		s_event_log* event_log = event_log_get(event_log_index);

		csstrnzcpy(event_log->name, event_log_name, NUMBEROF(event_log->name));
		event_log->event_log_flags = event_log_flags;
	}

	return event_log_index;
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

extern const char* const k_crash_info_output_filename;

bool copy_file_to_error_snapshot(const s_file_reference* source_file_reference, s_file_reference* file_reference_out)
{
	c_static_string<260> source_file_name;
	c_static_string<260> destination_file_path;
	c_debug_output_path debug_output_path;

	constexpr int32 name_flags = FLAG(_name_parent_directory_bit) | FLAG(_name_file_bit) | FLAG(_name_extension_bit);
	file_reference_get_name(source_file_reference, name_flags, source_file_name.get_buffer(), source_file_name.element_count);
	destination_file_path.print("%s%s", debug_output_path.get_path(k_error_snapshot_directory), source_file_name.get_string());
	file_reference_create_from_path(file_reference_out, destination_file_path.get_string(), false);
	file_create_parent_directories_if_not_present(file_reference_out);
	return file_copy_to(source_file_reference, file_reference_out, true);
}

int32 upload_error_file_start(int32 maximum_file_count, s_file_reference* out_file_list)
{
	c_critical_section_scope section_scope(k_crit_section_event_logs);

	s_file_reference temp_file_reference{};

	int32 file_count = 0;
	if (event_logs_usable())
	{
		event_logs_close();
		directory_create_or_delete_contents(k_error_snapshot_directory);
		
		for (int32 event_log_index = 0; event_log_index < event_log_globals.event_log_count; event_log_index++)
		{
			s_event_log* event_log = event_log_get(event_log_index);
			if (!event_log->event_log_flags.test(_event_log_only_for_custom_subfolder) || event_logs_using_subfolder())
			{
				create_report_file_reference(&temp_file_reference, event_log->name, !event_log->event_log_flags.test(_event_log_place_in_root_bit));
				if (copy_file_to_error_snapshot(&temp_file_reference, &out_file_list[file_count]))
				{
					file_count++;
				}
			}
		}

		if (file_count < maximum_file_count && copy_file_to_error_snapshot(&temp_file_reference, &out_file_list[file_count]))
		{
			file_count++;
		}
	}
	return file_count;
}

void event_logs_snapshot()
{
	s_file_reference debug_file_reference_list[64];
	csmemset(debug_file_reference_list, 0, sizeof(debug_file_reference_list));
	upload_error_file_start(NUMBEROF(debug_file_reference_list), debug_file_reference_list);
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
		PROFILER(event_logs_work_function)
		{
			current_thread_update_test_functions();
			if (internal_event_wait_timeout(k_event_cseries_event_log, 1000))
			{
				c_critical_section_scope section_scope(k_crit_section_event_logs);
				flush_event_log_cache();
			}
			switch_to_thread();
		}
	}
	suppress_file_errors(false);
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

void flush_event_log_cache()
{
	if (event_log_globals.initialized && !events_force_no_log)
	{
		uns32 flush_log_start_time = network_time_get();

		if (synchronization_objects_initialized())
		{
			internal_critical_section_enter(k_crit_section_event_logs);
		}

		uns32 event_log_mask = 0;
		for (int32 event_log_index = 0; event_log_index < event_log_globals.event_log_count; event_log_index++)
		{
			if (g_event_log_cache.cached_categories.test(event_log_index))
			{
				s_event_log* event_log = event_log_get(event_log_index);
				if (event_log->event_log_flags.test(_event_log_only_for_custom_subfolder) || event_logs_using_subfolder())
				{
					event_log->output_buffer.initialize_from_reference(acquire_report_file_reference(event_log_index));
					event_log_mask |= FLAG(event_log_index);
				}
			}
		}

		int32 entry_count = g_event_log_cache.entry_stack.count();
		for (int32 entry_index = 0; entry_index < entry_count; entry_index++)
		{
			s_event_log_cache_entry* entry = g_event_log_cache.entry_stack.get(entry_index);
			for (int32 event_log_index = 0; event_log_index < 32; event_log_index++)
			{
				if (entry->event_log_indices.test(event_log_index))
				{
					s_event_log* event_log = event_log_get(event_log_index);
					ASSERT(event_log);
					ASSERT(g_event_log_cache.cached_categories.test(event_log_index));
					write_event_log_cache_entry(true, event_log_index, entry->error_string, false);
				}
			}
		}

		for (int32 event_log_index = 0; event_log_index < event_log_globals.event_log_count; event_log_index++)
		{
			if (TEST_BIT(event_log_mask, event_log_index))
			{
				s_event_log* event_log = event_log_get(event_log_index);
				event_log->output_buffer.release_reference();
				release_report_file_reference(event_log_index, !event_log_globals.cache_event_log_output);
			}
		}

		g_event_log_cache.last_flush_time = system_milliseconds();
		g_event_log_cache.entry_stack.clear();
		g_event_log_cache.entry_string_cache_size = 0;
		g_event_log_cache.cached_categories.clear();

		if (synchronization_objects_initialized())
		{
			internal_event_reset(k_event_cseries_event_log);
			internal_critical_section_leave(k_crit_section_event_logs);
		}

		if (network_time_since(flush_log_start_time) > 100)
		{
			event(_event_error, "events: flush_event_log_cache took %d msecs to flush",
				network_time_since(flush_log_start_time));
		}
	}
}

void write_raw_to_error_file(c_file_output_buffer* file_buffer, int32 event_log_index, const char* string, bool flush)
{
	if (!events_force_no_log)
	{
		ASSERT(string != NULL);

		if (file_buffer)
		{
			file_buffer->write((uns32)strlen_debug(string), string);
		}
		else if (s_file_reference* reference = acquire_report_file_reference(event_log_index))
		{
			file_write(reference, (uns32)strlen_debug(string), string);
			release_report_file_reference(event_log_index, flush);
		}
	}
}

void write_event_log_cache_entry(bool use_report_buffers, int32 event_log_index, const char* string, bool flush)
{
	c_file_output_buffer* output_buffer = NULL;

	c_critical_section_scope section_scope(k_crit_section_event_logs);

	s_event_log* event_log = event_log_get(event_log_index);
	if (!event_log->event_log_flags.test(_event_log_only_for_custom_subfolder) || event_logs_using_subfolder())
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
			write_raw_to_error_file(output_buffer, event_log_index, version_get_full_string() /*shell_get_version()*/, flush);
			write_raw_to_error_file(output_buffer, event_log_index, "\r\n", flush);
			write_raw_to_error_file(output_buffer, event_log_index, "============================================================================================\r\n", flush);
			write_raw_to_error_file(output_buffer, event_log_index, "\r\n\r\n", flush);
		}

		write_raw_to_error_file(output_buffer, event_log_index, string, flush);
	}
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

	c_critical_section_scope section_scope(k_crit_section_event_logs);

	enum
	{
		k_entry_string_cache_maximum_size = sizeof(g_event_log_cache.entry_string_cache),
	};

	int32 string_size = (int32)strlen_debug(string) + 1;

	suppress_file_errors(true);

	if (!event_log_globals.cache_event_log_output
		|| g_event_log_cache.entry_stack.full()
		|| g_event_log_cache.entry_string_cache_size + string_size > k_entry_string_cache_maximum_size)
	{
		flush_event_log_cache();
	}

	ASSERT(!g_event_log_cache.entry_stack.full());

	if (event_log_globals.cache_event_log_output
		&& g_event_log_cache.entry_string_cache_size + string_size <= k_entry_string_cache_maximum_size)
	{
		c_flags<int32, uns32, 32> category;
		int32 entry_string_cache_size = g_event_log_cache.entry_string_cache_size;

		g_event_log_cache.entry_stack.push();
		s_event_log_cache_entry* entry_string = g_event_log_cache.entry_stack.get_top();

		for (int32 event_log_num = 0; event_log_num < event_log_count; event_log_num++)
		{
			int32 event_log_index = event_log_indices[event_log_num];
			if (event_log_index == NONE)
			{
				event_log_get(event_log_index);
				category.set(event_log_index, true);
			}
		}
		entry_string->event_log_indices = category;
		g_event_log_cache.cached_categories |= category;
		char* error_string = &g_event_log_cache.entry_string_cache[entry_string_cache_size];
		csstrnzcpy(&g_event_log_cache.entry_string_cache[entry_string_cache_size], string, string_size);
		entry_string->error_string = error_string;
		entry_string_cache_size += string_size;
		g_event_log_cache.entry_string_cache_size += string_size;

		if (!k_tracked_build
			|| g_event_log_cache.entry_stack.full()
			|| g_event_log_cache.entry_string_cache_size >= 30720)
		{
			internal_event_set(k_event_cseries_event_log);
		}
	}
	else
	{
		ASSERT(g_event_log_cache.entry_stack.empty());
		for (int32 event_log_num = 0; event_log_num < event_log_count; event_log_num++)
		{
			int32 event_log_index = event_log_indices[event_log_num];
			write_event_log_cache_entry(false, event_log_index, string, false);
		}
	}

	suppress_file_errors(false);
}

