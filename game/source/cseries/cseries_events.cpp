#include "cseries/cseries_events.hpp"

#include "interface/interface.hpp"
#include "interface/interface_constants.hpp"
#include "memory/module.hpp"
#include "multithreading/threads.hpp"
#include "rasterizer/rasterizer_main.hpp"
#include "shell/shell.hpp"
#include "tag_files/files.hpp"
#include "text/draw_string.hpp"

#include <string.h>
#include <climits>
#include <ctype.h>
#include <wtypes.h>

HOOK_DECLARE(0x000D858D0, network_debug_print);

char const* const k_reports_directory_name = "reports\\";
char const* const k_reports_directory_root_name = "\\";

thread_local bool g_generating_event = false;

s_event_globals event_globals{};
bool g_events_initialized = false;

c_read_write_lock g_event_read_write_lock;

char const* const k_event_level_names[k_event_level_count + 1]
{
	"verbose",
	"status",
	"message",
	"warning",
	"error",
	"critical",
	"none"
};

char const* const k_event_level_severity_strings[k_event_level_count]
{
	"verbose",
	"status ",
	"message",
	"WARNING",
	"-ERROR-",
	"-FATAL-"
};

bool g_events_debug_render_enable = true;
char const* const k_primary_event_log_filename = "debug.txt";
char const* const k_primary_full_event_log_filename = "debug_full.txt";

s_file_reference* __cdecl create_report_file_reference(s_file_reference* info, char const* filename, bool use_sub_directory)
{
	c_static_string<256> reports_file_path;
	reports_file_path.print("%s", use_sub_directory ? k_reports_directory_name : k_reports_directory_root_name);
	reports_file_path.append(filename);

	s_file_reference* result = file_reference_create_from_path(info, reports_file_path.get_string(), false);
	ASSERT(result != NULL);

	if (result)
		file_create_parent_directories_if_not_present(result);

	return result;
}

void __cdecl build_networking_buffer_for_log(char*, long)
{

}

s_event const g_log_events[]
{
	{
		"lifecycle:",
		_event_level_none,
		{ 1.0f, 1.0f, 1.0f },
		_event_level_message,
		NULL,
		NULL,
		_event_level_message
	},
	{
		"crash:",
		_event_level_message,
		{ 1.0f, 0.1f, 0.1f },
		_event_level_message,
		NULL,
		NULL,
		_event_level_message
	},
	{
		"animation:",
		_event_level_none,
		{ 1.0f, 1.0f, 0.0f },
		_event_level_none,
		"animation_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"ai:",
		_event_level_none,
		{ 1.0f, 0.63f, 0.48f },
		_event_level_none,
		"ai_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"shaders:",
		_event_level_none,
		{ 0.0f, 0.0f, 1.0f },
		_event_level_none,
		"shaders_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"geometry:",
		_event_level_none,
		{ 0.0f, 0.0f, 0.8f },
		_event_level_none,
		"geometry_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"environment:",
		_event_level_none,
		{ 0.0f, 0.0f, 0.6f },
		_event_level_none,
		"environment_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"objects:",
		_event_level_none,
		{ 0.4f, 0.1f, 0.4f },
		_event_level_none,
		"objects_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"networking:",
		_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_level_message,
		"networking_debug.txt",
		build_networking_buffer_for_log,
		_event_level_message
	},
	{
		"networking:leaderboards:",
		_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_level_message,
		"leaderboard_debug.txt",
		NULL,
		_event_level_message
	},
	{
		"networking:spawning:",
		_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_level_message,
		"networking_debug.txt",
		build_networking_buffer_for_log,
		_event_level_none
	},
	{
		"networking:camera:",
		_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_level_message,
		"networking_debug.txt",
		build_networking_buffer_for_log,
		_event_level_none
	},
	{
		"datamine:",
		_event_level_warning,
		{ 1.0f, 0.4f, 0.7f },
		_event_level_message,
		"networking_debug.txt",
		build_networking_buffer_for_log,
		_event_level_message
	},
	{
		"tags:",
		_event_level_error,
		{ 1.0f, 0.0f, 1.0f },
		_event_level_warning,
		"tag_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"ui:",
		_event_level_none,
		{ 1.0f, 0.5f, 0.0f },
		_event_level_none,
		"ui_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"ui:window_manager:",
		_event_level_none,
		{ 1.0f, 0.5f, 0.0f },
		_event_level_message,
		"ui_debug.txt",
		NULL,
		_event_level_message
	},
	{
		"ui:memory_peak:",
		_event_level_none,
		{ 1.0f, 0.5f, 0.0f },
		_event_level_message,
		"ui_debug.txt",
		NULL,
		_event_level_message
	},
	{
		"sound:",
		_event_level_none,
		{ 0.7f, 0.7f, 0.7f },
		_event_level_warning,
		"sound_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"multiplayer:",
		_event_level_none,
		{ 0.8f, 0.1f, 0.6f },
		_event_level_none,
		"multiplayer_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"effects:",
		_event_level_none,
		{ 0.0f, 0.0f, 0.8f },
		_event_level_none,
		"effects_debug.txt",
		NULL,
		_event_level_error
	},
	{
		"animation_audio_content:",
		_event_level_none,
		{ 0.7f, 0.7f, 0.5f },
		_event_level_none,
		"animation_audio_content_debug_4tx_",
		NULL,
		_event_level_none
	},
	{
		"environment_materials:",
		_event_level_none,
		{ 0.0f, 0.0f, 0.6f },
		_event_level_none,
		"environment_materials_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"object_materials:",
		_event_level_none,
		{ 0.4f, 0.1f, 0.4f },
		_event_level_none,
		"object_materials_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"design:",
		_event_level_none,
		{ 1.0f, 0.63f, 0.48f },
		_event_level_none,
		"design_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"localization:",
		_event_level_none,
		{ 0.0f, 0.0f, 0.0f },
		_event_level_none,
		"localization_debug.txt",
		NULL,
		_event_level_none
	},
	{
		"hs:",
		_event_level_warning,
		{ 0.5f, 0.8f, 0.3f },
		_event_level_message,
		"hs_log.txt",
		NULL,
		_event_level_none
	},
	{
		"tags:xsync:",
		_event_level_none,
		{ 1.0f, 1.0f, 0.0f },
		_event_level_message,
		NULL,
		NULL,
		_event_level_none
	},
	{
		"tags:cache_builder:",
		_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_level_message,
		NULL,
		NULL,
		_event_level_none
	},
	{
		"system:",
		_event_level_warning,
		{ 1.0f, 1.0f, 1.0f },
		_event_level_message,
		"system_log.txt",
		NULL,
		_event_level_none
	},
	{
		"cache:",
		_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_level_message,
		NULL,
		NULL,
		_event_level_none
	},
	{
		"debugger:",
		_event_level_none,
		{ 1.0f, 1.0f, 1.0f },
		_event_level_message,
		NULL,
		NULL,
		_event_level_none
	},
};

void __cdecl events_debug_render()
{
	if (!g_events_debug_render_enable)
		return;

	c_rasterizer_draw_string draw_string;
	c_font_cache_mt_safe font_cache;

	short_rectangle2d display_title_safe_pixel_bounds{};
	interface_get_current_display_settings(nullptr, nullptr, nullptr, &display_title_safe_pixel_bounds);

	short_rectangle2d bounds{};
	bounds.x0 = display_title_safe_pixel_bounds.x0;
	bounds.y0 = display_title_safe_pixel_bounds.y0 + 70;
	bounds.x1 = SHRT_MAX;
	bounds.y1 = SHRT_MAX;

	real_argb_color color{};
	color.alpha = global_real_argb_red->alpha;

	real interpolation_factor = (system_milliseconds() % 1000) / 1000.0f;
	interpolate_linear(global_real_argb_red->color.red, global_real_argb_white->color.red, interpolation_factor);
	interpolate_linear(global_real_argb_red->color.green, global_real_argb_white->color.green, interpolation_factor);
	interpolate_linear(global_real_argb_red->color.blue, global_real_argb_white->color.blue, interpolation_factor);

	interface_set_bitmap_text_draw_mode(&draw_string, 0, -1, 0, 0, 5, 0);
	draw_string.set_color(&color);
	draw_string.set_tab_stops(nullptr, 0);
	draw_string.set_bounds(&bounds);

	bool first_event = true;
	for (long i = 0; i < event_globals.spamming_events.get_count(); i++)
	{
		s_spamming_event& spamming_event = event_globals.spamming_events[i];

		if (spamming_event.valid)
		{
			if ((system_milliseconds() - spamming_event.hit_time) > 3000)
				csmemset(&spamming_event, 0, sizeof(s_spamming_event));
		}

		if (spamming_event.valid && spamming_event.hit_count >= 2)
		{
			if (first_event)
			{
				draw_string.draw(&font_cache, spamming_event.text);
				first_event = false;
			}
			else
			{
				draw_string.draw_more(&font_cache, spamming_event.text);
			}
		}
	}
}

char const* __cdecl events_get()
{
	return event_globals.error_message_buffer;
}

bool __cdecl event_level_query(e_event_level event_level)
{
	return event_level >= event_globals.query_level;
}

bool __cdecl event_thread_query()
{
	return _bittest(&event_globals.thread_query_flags, get_current_thread_index());
}

long event_parse_categories(char const* event_name, long category_substring_count, long maximum_characters, char(*category_names)[8][64])
{
	char const* category_substring = event_name;
	long category_index = 0;
	long category_count = 0;
	bool succeeded = false;
	bool failed = false;
	do
	{
		char const* category_name = category_substring;
		long category_name_length = 0;
		bool category_found = false;
		do
		{
			char temp = *category_substring;
			if (category_name_length < maximum_characters)
			{
				if (isalnum(temp) || temp == '-' || temp == '_')
				{
					category_substring++;
					category_name_length++;
				}
				else
				{
					category_found = true;
					if (temp == ':')
					{
						category_substring++;
						category_count++;
					}
				}
			}
			else
			{
				generate_event(_event_level_error, "network event category substring #%d '%s' exceeded %d characters", category_index, category_name, maximum_characters);

				failed = true;
			}
		} while (!category_found && !failed);

		if (category_found)
		{
			if (category_name_length)
			{
				if (category_index < category_substring_count)
				{
					csstrnzcpy((*category_names)[category_index], category_name, maximum_characters);
					(*category_names)[category_index][category_name_length] = 0;
					ascii_strnlwr((*category_names)[category_index++], category_name_length);
				}
				else
				{
					generate_event(_event_level_error, "network event category #%d '%s' exceeded %d category substrings", category_index, category_substring, category_substring_count);

					failed = true;
				}
			}
			else
			{
				succeeded = true;
			}
		}
	} while (!succeeded && !failed);

	if (failed) // #TODO: asserts with string builder
		c_console::write_line("failed to parse network event '%s'", event_name);

	if (category_index > category_count)
		category_index = category_count;

	return category_index;
}

s_event_category* get_writeable_category(long category_index)
{
	return &event_globals.categories[category_index];
}

long event_find_category_recursive(long category_index, bool create_category, long category_count, char(*category_names)[8][64])
{
	ASSERT(category_count > 0);
	ASSERT(csstrnlen((*category_names)[0], NUMBEROF((*category_names)[0])) > 0);

	char const* category_name = (*category_names)[0];

	s_event_category* category = get_writeable_category(category_index);
	long next_category_index = NONE;

	s_event_category* temp_category = nullptr;
	for (long i = category->__unknown17C; i != NONE; i = temp_category->__unknown180)
	{
		temp_category = get_writeable_category(i);

		if (temp_category->name.is_equal(category_name))
		{
			next_category_index = i;
			break;
		}
	}

	if (next_category_index == NONE && create_category)
	{
		next_category_index = NONE;
		if (event_globals.category_count < event_globals.categories.get_count())
			next_category_index = event_globals.category_count++;

		if (next_category_index == NONE)
		{
			generate_event(_event_level_error, "event_find_category_recursive: ran out of categories creating '%s'", (*category_names)[0]);
		}
		else
		{
			s_event_category* next_category = get_writeable_category(next_category_index);
			category->name.set(category_name);
			category->log_file.set(category->log_file.get_string());
			next_category->display_level = category->display_level;
			next_category->color = category->color;
			next_category->log_level = category->log_level;
			next_category->remote_log_level = category->remote_log_level;
			next_category->debugger_break_level = category->debugger_break_level;
			next_category->halt_level = category->halt_level;
			next_category->__unknown0 = category->__unknown0 + 1;
			next_category->parent_index = category_index;
			next_category->__unknown17C = NONE;
			next_category->__unknown180 = category->__unknown17C;
			next_category->event_log_index = category->event_log_index;
			next_category->build_buffer_for_log_proc = category->build_buffer_for_log_proc;
			next_category->registered_event_listeners_flags = category->registered_event_listeners_flags;
			category->__unknown17C = next_category_index;
		}
	}

	if (next_category_index != NONE && category_count > 1)
		next_category_index = event_find_category_recursive(next_category_index, create_category, category_count - 1, category_names + 1);

	return next_category_index;
}

long event_find_category(bool write, long a2, char(*category_names)[8][64])
{
	if (a2 <= 0)
		return 0;

	g_event_read_write_lock.read_lock();
	long category_index = event_find_category_recursive(0, false, a2, category_names);
	g_event_read_write_lock.read_unlock();
	if (category_index == NONE && write)
	{
		g_event_read_write_lock.write_lock();
		category_index = event_find_category_recursive(0, true, a2, category_names);
		g_event_read_write_lock.write_unlock();
	}

	return category_index;
}

long event_category_from_name(char const* event_name, bool write)
{
	ASSERT(event_name);

	char category_names[8][64]{};

	long v6 = event_parse_categories(event_name, 8, 64, &category_names);
	return event_find_category(write, v6, &category_names);
}

void event_initialize_categories()
{
	ASSERT(event_globals.category_count == 0);

	s_event_category* category = get_writeable_category(0);
	category->name.set("");
	category->log_file.set("");
	category->display_level = _event_level_none;
	category->log_level = _event_level_none;
	category->remote_log_level = _event_level_none;
	category->debugger_break_level = _event_level_none;
	category->halt_level = _event_level_none;
	category->__unknown0 = 0;
	category->parent_index = NONE;
	category->__unknown17C = NONE;
	category->__unknown180 = NONE;
	category->color = global_real_argb_white->color;
	category->event_log_index = NONE;
	category->build_buffer_for_log_proc = nullptr;
	category->registered_event_listeners_flags = 0;
	event_globals.category_count++;

	for (long i = 0; i < NUMBEROF(g_log_events); i++)
	{
		s_event const* log_event = &g_log_events[i];
		long category_index = event_category_from_name(log_event->name, true);
		s_event_category* next_category = get_writeable_category(category_index);
		next_category->display_level = log_event->display_level;
		next_category->color = log_event->color;
		if (log_event->log_file)
		{
			next_category->log_file.set(log_event->log_file);
			next_category->event_log_index = NONE;// event_log_new(event_->log_file, FLAG(3), true);
		}
		next_category->log_level = log_event->log_level;
		next_category->remote_log_level = log_event->remote_log_level;
		next_category->debugger_break_level = _event_level_none;
		next_category->halt_level = _event_level_none;
		next_category->build_buffer_for_log_proc = log_event->build_buffer_for_log_proc;
	}
}

bool events_initialize_if_possible()
{
	static bool run_once = false;
	if (!run_once && is_main_thread() /*&& synchronization_objects_initialized()*/)
	{
		g_event_read_write_lock.setup(6, 1);
		run_once = true;

		event_globals.enabled = true;
		event_globals.suppression_disabled = false;
		event_globals.spam_suppression_enabled = shell_application_type() == _shell_application_type_client;
		event_globals.__unknown81B62 = shell_application_type() != _shell_application_type_none;
		event_globals.display_level = _event_level_warning;
		event_globals.log_level = _event_level_warning;
		event_globals.remote_log_level = _event_level_warning;
		event_globals.query_level = _event_level_none;
		event_globals.__unknown10 = NONE;

		event_globals.event_listener_count = 0;
		event_globals.event_listeners.set_all(nullptr);

		event_globals.__unknown61018_time = 0;
		event_globals.__unknown6101C_level = _event_level_verbose;
		event_globals.__unknown61020_time = 0;

		csmemset(event_globals.spamming_events.get_elements(), 0, event_globals.spamming_events.get_total_element_size());

		event_globals.thread_query_flags = 0xFFFFFBF7; // ~(FLAG(3) | FLAG(10));
		event_globals.event_log_flags_bit3_enabled = false;
		event_globals.__unknown81B65 = false;

		// clear function
		event_globals.error_message_length = 0;
		event_globals.error_message_buffer[0] = 0;

		// event_logs_initialize();
		// function

		event_globals.category_count = 0;

		event_initialize_categories();

		g_events_initialized = true;
	}

	return g_events_initialized;
}

void __cdecl events_initialize()
{
	events_initialize_if_possible();

	ASSERT(g_events_initialized);
	generate_event(_event_level_message, "lifecycle: events initalize");
}

long __cdecl event_interlocked_compare_exchange(c_interlocked_long& value, long ExChange, long Comperand)
{
	return value.set_if_equal(ExChange, Comperand);
}

c_event::c_event(e_event_level event_level, long category_index, dword_flags event_flags) :
	m_event_level(event_level),
	m_category_index(category_index),
	m_event_flags(event_flags)
{
}

bool c_event::query()
{
	return event_thread_query() && event_level_query(m_event_level);
}

dword_flags event_query(e_event_level event_level, long category_index, dword_flags event_flags)
{
	ASSERT(g_events_initialized);

	dword_flags flags = 0;

	if (event_globals.display_level != _event_level_none)
		flags = event_level >= event_globals.display_level;

	if (event_globals.log_level != _event_level_none)
	{
		if (event_level < event_globals.log_level)
			flags &= ~FLAG(1);
		else
			flags |= FLAG(1);
	}

	if (event_globals.remote_log_level != _event_level_none)
	{
		if (event_level < event_globals.remote_log_level)
			flags &= ~FLAG(2);
		else
			flags |= FLAG(2);
	}

	if (category_index != -1)
	{
		s_event_category* category = get_writeable_category(category_index);
		if (category->display_level != -1)
		{
			if (event_level < category->display_level)
				flags &= ~FLAG(0);
			else
				flags |= FLAG(0);
		}

		if (category->log_level != _event_level_none)
		{
			if (event_level < category->log_level)
				flags &= ~FLAG(1);
			else
				flags |= FLAG(1);
		}

		if (category->remote_log_level != _event_level_none)
		{
			if (event_level < category->remote_log_level)
				flags &= ~FLAG(2);
			else
				flags |= FLAG(2);
		}

		if (category->debugger_break_level != _event_level_none)
		{
			if (event_level < category->debugger_break_level)
				flags &= ~FLAG(3);
			else
				flags |= FLAG(3);
		}

		if (category->halt_level != _event_level_none)
		{
			if (event_level < category->halt_level)
				flags &= ~FLAG(4);
			else
				flags |= FLAG(4);
		}
	}

	if (!event_globals.suppression_disabled && TEST_BIT(event_flags, 0))
		flags &= ~FLAG(0);

	if (event_globals.__unknown81B65)
		flags &= ~FLAG(1);


	return flags;
}

struct s_hit_result
{
	dword hit_time;
	long hit_count;
};

void add_event_to_spamming_list(char const* event_text, s_hit_result* result_out)
{
	ASSERT(event_text);
	ASSERT(result_out);

	long event_index = NONE;
	bool event_exists = false;

	for (long i = 0; i < event_globals.spamming_events.get_count(); i++)
	{
		s_spamming_event* spamming_event = &event_globals.spamming_events[i];
		if (spamming_event->valid)
		{
			if (csmemcmp(spamming_event->text, event_text, sizeof(spamming_event->text)) == 0)
			{
				result_out->hit_time = spamming_event->hit_time;
				result_out->hit_count = spamming_event->hit_count++;
				event_exists = true;
			}
		}
		else if (event_index == NONE)
		{
			event_index = i;
		}
	}

	if (!event_exists && event_index != NONE)
	{
		s_spamming_event* spamming_event = &event_globals.spamming_events[event_index];
		ASSERT(!spamming_event->valid);

		spamming_event->valid = true;
		spamming_event->hit_time = system_milliseconds();
		spamming_event->hit_count = 1;
		csstrnzcpy(spamming_event->text, event_text, sizeof(spamming_event->text));
	}
}

dword_flags sub_82894C80(dword_flags flags, e_event_level event_level, long category_index, char const* event_text)
{
	if (event_globals.spam_suppression_enabled && event_level != _event_level_critical && TEST_BIT(flags, 0))
	{
		s_event_category* category = get_writeable_category(category_index);

		dword time = system_milliseconds();
		if (time > category->__time58 + 10000)
			category->__unknown5C = 0;

		category->__unknown5C++;
		category->__time58 = time;

		if (category->__unknown5C >= 5)
		{
			s_hit_result result{};
			add_event_to_spamming_list(event_text, &result);
			if (result.hit_count > 1)
				flags = 0;
		}
	}
	return flags;
}

void event_generate(e_event_level event_level, long category_index, dword_flags event_flags, char const* format, va_list list)
{
	ASSERT(g_events_initialized);

	dword_flags flags = 0;
	c_static_string<2048> event_text;

	//c_font_cache font_cache{};
	g_event_read_write_lock.read_lock();
	flags = event_query(event_level, category_index, event_flags);
	g_event_read_write_lock.read_unlock();

	if (flags)
	{
		char event_log_string[2048]{};

		event_text.print_va(format, list);
		c_console::write_line(event_text.get_string());

		g_event_read_write_lock.write_lock();

		s_event_category* category = get_writeable_category(category_index);
		flags = sub_82894C80(flags, event_level, category_index, event_text.get_string());

		if (TEST_BIT(flags, 0))
		{
			//char buffer0[2048]{};
			//char event_context1[256]{};
			//char buffer2[2048]{};
			//
			//cvsnzprintf(buffer0, sizeof(buffer0), format, list);
			//
			//if (event_context_get(0, event_context1, sizeof(event_context1)))
			//	csnzprintf(buffer2, sizeof(buffer2), "%s (%s) %s", k_event_level_severity_strings[event_level], event_context1, buffer0);
			//else
			//	csnzprintf(buffer2, sizeof(buffer2), "%s %s", k_event_level_severity_strings[event_level], buffer0);
			//
			//write_to_console(category_index, event_level, buffer2);
		}

		if (TEST_BIT(flags, 1))
		{
			long event_log_indices[6]{};
			long event_log_count = 0;

			if (category->event_log_index != NONE)
			{
				if (category->build_buffer_for_log_proc)
				{
					char buffer[512]{};
					category->build_buffer_for_log_proc(buffer, sizeof(buffer));
					//format_event_for_log(event_log_string, sizeof(event_log_string), event_level, ...)
					//write_to_event_log(&category->event_log_index, 1, event_log_string);
				}
				else
				{
					event_log_indices[event_log_count++] = category->event_log_index;
				}
			}

			if (event_level >= event_globals.log_level)
			{
				event_log_indices[event_log_count++] = event_globals.external_primary_event_log_index;
				event_log_indices[event_log_count++] = event_globals.internal_primary_event_log_index;
				event_log_indices[event_log_count++] = event_globals.subfolder_internal_primary_event_log_index;
				event_log_indices[event_log_count++] = event_globals.subfolder_internal_primary_full_event_log_index;
			}
			event_log_indices[event_log_count++] = event_globals.internal_primary_full_event_log_index;

			ASSERT(event_log_count <= NUMBEROF(event_log_indices));
			//format_event_for_log(event_log_string, sizeof(event_log_string), event_level, list)
			//write_to_event_log(&category->event_log_index, 1, event_log_string);
		}

		if (TEST_BIT(flags, 2))
		{
			//event_context_get(2u, (int)v38, 2048);
			//event_write_to_datamine(a1, v38, format, list);
		}

		if (TEST_BIT(flags, 3) /*&& !byte_841DD295*/)
		{
			//cvsnzprintf(v39, 2048, format, list);
			//csnzprintf(v38, 2048, "critical event encountered: %s", v39);
		}

		if (TEST_BIT(flags, 4) /*&& !byte_841DD294*/)
		{
			//cvsnzprintf(v39, 2048, format, list);
			//csnzprintf(v38, 2048, "critical event encountered: %s", v39);
		}

		//for (long event_listener_index = 0; event_globals.event_listeners.get_count(); event_listener_index++)
		//{
		//	if (TEST_BIT(category->registered_event_listeners_flags, event_listener_index))
		//	{
		//		ASSERT(event_globals.event_listeners[event_listener_index]);
		//		struct c_event_listener* event_listener = event_globals.event_listeners[event_listener_index];
		//		event_listener->log(event_level, event_string.get_string());
		//	}
		//}

		event_globals.event_listener_count++;
		g_event_read_write_lock.write_unlock();
	}
}

long c_event::generate(char const* event_name, ...)
{
	va_list list;
	va_start(list, event_name);

	if (!g_generating_event && events_initialize_if_possible() && event_globals.enabled)
	{
		g_generating_event = true;

		if (m_category_index == NONE)
			m_category_index = event_category_from_name(event_name, true);

		event_generate(m_event_level, m_category_index, m_event_flags, event_name, list);

		g_generating_event = false;
	}

	va_end(list);

	return m_category_index;
}

// used inplace of `c_event::generate`
// net::REMOTE_BINLOGGER
void __cdecl network_debug_print(char const* format, ...)
{
	long format_address = (long)format;

	// no print switch
	switch (format_address)
	{
	case 0x01614860: // "MP/NET/OBSERVER,CTRL: c_network_observer::stream_balance_all_stream_bandwidth: bandwidth balancing with a total bandwidth cap of %d bps between %d streams (%d simulation, %d load-bearing, %d loaded) grow=%08X, start=%08X"
	case 0x01610560: // "MP/NET/SESSION,PARAMS: c_generic_network_session_parameter<enum e_network_rough_quality>::set: [%s] parameter type %d [%s] being set"
	case 0x01611EE8: // "MP/NET/LINK,RCV: c_network_link::decode_packet: Received a game-data-only packet that was too big (%d > %d bytes)."
	case 0x01612028: // "MP/NET/LINK,RCV: c_network_link::read_data_immediate: Read %d-byte packet from invalid address '%s'."
		return;
	}

	// format replacement switch
	// occurrences of `%LX` need to be replaced with `%llX`
	// occurrences of `%Lx` need to be replaced with `%llx`
	switch (format_address)
	{
	case 0x0160E6F0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_session_interface_set_local_user_xuid: local user %d set player xuid=%LX"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_session_interface_set_local_user_xuid: local user %d set player xuid=%llX";
		break;
	case 0x01616488: // "MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_properties: [%s] peer-properties accepted for peer #%d [%s], name=%S, session name=%S, map=%d, map status=%s, map progress=%d, game instance=0x%LX, ready hopper=%d, start error=%s, hdd=%d, upstream=%d bps (reliable %d), downstream=%d bps, nat=%d, ratings (conn, host, client)=%d, %d, %d, connectivity=%04X (complete=%04X), flags=%X"
		format = "MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_properties: [%s] peer-properties accepted for peer #%d [%s], name=%S, session name=%S, map=%d, map status=%s, map progress=%d, game instance=0x%llX, ready hopper=%d, start error=%s, hdd=%d, upstream=%d bps (reliable %d), downstream=%d bps, nat=%d, ratings (conn, host, client)=%d, %d, %d, connectivity=%04X (complete=%04X), flags=%X";
		break;
	case 0x016216A8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_online_files_deleter::update: Deleted item from share %Lx, slot %d"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_online_files_deleter::update: Deleted item from share %llx, slot %d";
		break;
	case 0x01621718: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_online_files_deleter::update: Could not delete item from share %Lx, slot %d"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_online_files_deleter::update: Could not delete item from share %llx, slot %d";
		break;
	case 0x01625120: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: remove_from_player_list: unable to find player [0x%Lx] in the list to remove from the session"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: remove_from_player_list: unable to find player [0x%llx] in the list to remove from the session";
		break;
	case 0x01627DD8: // "MP/NET/SESSION,PARAMS: c_network_session_parameters::check_to_send_updates: [%s] sending session parameter update [cleared 0x%LX updated 0x%LX initial:%s]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameters::check_to_send_updates: [%s] sending session parameter update [cleared 0x%llX updated 0x%llX initial:%s]";
		break;
	case 0x01628578: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameters::handle_update: [%s] handling session parameter update [cleared 0x%LX updated 0x%LX initial:%s]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameters::handle_update: [%s] handling session parameter update [cleared 0x%llX updated 0x%llX initial:%s]";
		break;
	case 0x016338F0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::session_membership_matches_arbitration_registration_results: registrant %d machine %Lx xuid %Lx trust %d"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::session_membership_matches_arbitration_registration_results: registrant %d machine %llx xuid %llx trust %d";
		break;
	case 0x0163D7D8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_replication_entity_manager::process_creation: remote entity creation %lx type %d"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_replication_entity_manager::process_creation: remote entity creation %lx type %d";
		break;
	case 0x0163E5C8: // "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_local_entity: local entity marked for deletion %lx"
		format = "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_local_entity: local entity marked for deletion %lx";
		break;
	case 0x0163E640: // "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::notify_delete_remote_entity: remote entity deleted %lx"
		format = "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::notify_delete_remote_entity: remote entity deleted %lx";
		break;
	case 0x0163E6B0: // "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::acknowledge_entity_deletion: local entity deleted %lx after view %d acknowledge"
		format = "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::acknowledge_entity_deletion: local entity deleted %lx after view %d acknowledge";
		break;
	case 0x0163E820: // "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_entity_internal: entity deleted %lx"
		format = "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_entity_internal: entity deleted %lx";
		break;
	case 0x01647B28: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_banhammer_update_controllers: user file for controller %d / %L / %S contains auto download for user %LX slot %d server id %LX"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_banhammer_update_controllers: user file for controller %d / %L / %S contains auto download for user %llX slot %d server id %llX";
		break;
	}

	// format replacement switch
	// occurrences of `%8X` need to be replaced with `%08X`
	// this isn't a necessary replacement, but it makes logs nicer to read
	switch (format_address)
	{
	case 0x01623838: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host: create host to session 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host: create host to session 0x%08X";
		break;
	case 0x016243C8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_process_pending_operations: modify session 0x%8X [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_process_pending_operations: modify session 0x%08X [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x016244E0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting session 0x%8X that we haven't created yet"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting session 0x%08X that we haven't created yet";
		break;
	case 0x01624568: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting offline session 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting offline session 0x%08X";
		break;
	case 0x016245D8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting system-link session 0x%8X [%s]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting system-link session 0x%08X [%s]";
		break;
	case 0x01624650: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: async deleting session 0x%8X [%s]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: async deleting session 0x%08X [%s]";
		break;
	case 0x01624948: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: creating %s offline session 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: creating %s offline session 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x016249F0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: creating %s keys (system link) 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: creating %s keys (system link) 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x01624AA0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: async creating %s session 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: async creating %s session 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x01624BC0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: creating offline host-migration session 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: creating offline host-migration session 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x01624C80: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: creating host-migration keys (system link) 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: creating host-migration keys (system link) 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x01624D40: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: async doing host migration as %s, session 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: async doing host migration as %s, session 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x01624E00: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting host migration description that we haven't created yet 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting host migration description that we haven't created yet 0x%08X";
		break;
	case 0x01624EA0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting offline host-migration session 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting offline host-migration session 0x%08X";
		break;
	case 0x01624F28: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting system-link host migration description 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting system-link host migration description 0x%08X";
		break;
	case 0x01624FB8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: async deleting host migration 0x%8X [%s]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: async deleting host migration 0x%08X [%s]";
		break;
	case 0x01625038: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_build_removal_xuid_list: session 0x%8X, operating on %d xuids"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_build_removal_xuid_list: session 0x%08X, operating on %d xuids";
		break;
	case 0x016250B0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_build_add_xuid_list: session 0x%8X, operating on %d xuids"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_build_add_xuid_list: session 0x%08X, operating on %d xuids";
		break;
	case 0x01625308: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_creation_complete: session 0x%8X, async session create failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_creation_complete: session 0x%08X, async session create failed";
		break;
	case 0x01625380: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_creation_complete: session 0x%8X, async session create completely successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_creation_complete: session 0x%08X, async session create completely successfully";
		break;
	case 0x01625408: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_deletion_complete: session 0x%8X, async session delete failed, forcing a successful completion"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_deletion_complete: session 0x%08X, async session delete failed, forcing a successful completion";
		break;
	case 0x016254A0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_deletion_complete: session 0x%8X, async session delete completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_deletion_complete: session 0x%08X, async session delete completed successfully";
		break;
	case 0x01625590: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_host_migration_creation_complete: session 0x%8X, active migration session is '%s'"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_host_migration_creation_complete: session 0x%08X, active migration session is '%s'";
		break;
	case 0x016256D8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%8X, async host-migration wound up with a zero session descripton, bailing..."
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%08X, async host-migration wound up with a zero session descripton, bailing...";
		break;
	case 0x01625780: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%8X, async host-migration create completed successfully [%s]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%08X, async host-migration create completed successfully [%s]";
		break;
	case 0x01625818: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%8X, host migration failed (%08X), reset"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%08X, host migration failed (%08X), reset";
		break;
	case 0x016258A0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_modify_complete: session 0x%8X, async session modify completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_modify_complete: session 0x%08X, async session modify completed successfully";
		break;
	case 0x01625930: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_modify_complete: session 0x%8X, async session modify failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_modify_complete: session 0x%08X, async session modify failed";
		break;
	case 0x016259A8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_add_complete: session 0x%8X, player add completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_add_complete: session 0x%08X, player add completed successfully";
		break;
	case 0x01625A30: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_add_complete: session 0x%8X, operating xuid operation (%d) complete"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_add_complete: session 0x%08X, operating xuid operation (%d) complete";
		break;
	case 0x01625AC0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_add_complete: session 0x%8X, player add failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_add_complete: session 0x%08X, player add failed";
		break;
	case 0x01625B30: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_add_complete: session 0x%8X, operating xuid operation (%d) complete"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_add_complete: session 0x%08X, operating xuid operation (%d) complete";
		break;
	case 0x01625BB8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_remove_complete: session 0x%8X, player remove completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_remove_complete: session 0x%08X, player remove completed successfully";
		break;
	case 0x01625C48: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_remove_complete: session 0x%8X, operating xuid operation (%d) complete"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_remove_complete: session 0x%08X, operating xuid operation (%d) complete";
		break;
	case 0x01625CD8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_remove_complete: session 0x%8X, player remove failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_remove_complete: session 0x%08X, player remove failed";
		break;
	case 0x01625D50: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_remove_complete: session 0x%8X, operating xuid operation (%d) complete"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_remove_complete: session 0x%08X, operating xuid operation (%d) complete";
		break;
	case 0x01625DD8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_game_start_complete: session 0x%8X, game start completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_game_start_complete: session 0x%08X, game start completed successfully";
		break;
	case 0x01625E60: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_game_start_complete: session 0x%8X, game start failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_game_start_complete: session 0x%08X, game start failed";
		break;
	case 0x01625ED0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_game_end_complete: session 0x%8X, game end completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_game_end_complete: session 0x%08X, game end completed successfully";
		break;
	case 0x01625F58: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_game_end_complete: session 0x%8X, game end failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_game_end_complete: session 0x%08X, game end failed";
		break;
	case 0x01628730: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameters::handle_update: [%s] failed to handle update (params 0x%8X/cleared 0x%08X/initial %s)"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameters::handle_update: [%s] failed to handle update (params 0x%08X/cleared 0x%08X/initial %s)";
		break;
	case 0x01628A98: // "MP/NET/SESSION,PARAMS: c_network_session_parameters::handle_change_request: [%s] parameter %d [%s] change request size out of bounds of payload size [0x%8X 0x%8X 0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameters::handle_change_request: [%s] parameter %d [%s] change request size out of bounds of payload size [0x%08X 0x%08X 0x%08X]";
		break;
	case 0x01628D88: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x01629270: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%08X != %0x%08X]";
		break;
	case 0x016294F0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x016299F0: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%08X != %0x%08X]";
		break;
	case 0x01629E28: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x0162A310: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%08X != %0x%08X]";
		break;
	case 0x0162A590: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x0162AA90: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%08X != %0x%08X]";
		break;
	case 0x0162AEE0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x0162B410: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%08X != %0x%08X]";
		break;
	case 0x0162B6B0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x0162BC00: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%08X != %0x%08X]";
		break;
	case 0x01639450: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: creation failed for managed session [client] 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: creation failed for managed session [client] 0x%08X";
		break;
	case 0x016394D0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: failed to get security information from managed session 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: failed to get security information from managed session 0x%08X";
		break;
	case 0x01639788: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: managed session creation neither succeeded nor failed...but is no longer happening [client] 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: managed session creation neither succeeded nor failed...but is no longer happening [client] 0x%08X";
		break;
	}

	va_list list;
	va_start(list, format);
	c_console::write_line_va(format, list);
	va_end(list);
}

