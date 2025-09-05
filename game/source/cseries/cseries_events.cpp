#include "cseries/cseries_events.hpp"

#include "interface/interface.hpp"
#include "interface/interface_constants.hpp"
#include "main/console.hpp"
#include "main/main.hpp"
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

const char* const k_reports_directory_name = "reports\\";
const char* const k_reports_directory_root_name = "\\";

s_event_globals event_globals{};
bool g_events_initialized = false;

c_read_write_lock g_event_read_write_lock;

const char* const k_event_level_names[k_event_level_count + 1]
{
	"verbose",
	"status",
	"message",
	"warning",
	"error",
	"critical",
	"none"
};

const char* const k_event_level_severity_strings[k_event_level_count]
{
	"verbose",
	"status ",
	"message",
	"WARNING",
	"-ERROR-",
	"-FATAL-"
};

bool g_events_debug_render_enable = true;
const char* const k_primary_event_log_filename = "debug.txt";
const char* const k_primary_full_event_log_filename = "debug_full.txt";

s_file_reference* __cdecl create_report_file_reference(s_file_reference* info, const char* filename, bool use_sub_directory)
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

void __cdecl build_networking_buffer_for_log(char*, int32)
{

}

const s_event_category_default_configuration g_log_events[]
{
	{
		"lifecycle:",
		k_event_level_none,
		{ 1.0f, 1.0f, 1.0f },
		_event_message,
		NULL,
		NULL,
		_event_message
	},
	{
		"crash:",
		_event_message,
		{ 1.0f, 0.1f, 0.1f },
		_event_message,
		NULL,
		NULL,
		_event_message
	},
	{
		"animation:",
		k_event_level_none,
		{ 1.0f, 1.0f, 0.0f },
		k_event_level_none,
		"animation_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"ai:",
		k_event_level_none,
		{ 1.0f, 0.63f, 0.48f },
		k_event_level_none,
		"ai_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"shaders:",
		k_event_level_none,
		{ 0.0f, 0.0f, 1.0f },
		k_event_level_none,
		"shaders_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"geometry:",
		k_event_level_none,
		{ 0.0f, 0.0f, 0.8f },
		k_event_level_none,
		"geometry_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"environment:",
		k_event_level_none,
		{ 0.0f, 0.0f, 0.6f },
		k_event_level_none,
		"environment_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"objects:",
		k_event_level_none,
		{ 0.4f, 0.1f, 0.4f },
		k_event_level_none,
		"objects_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"networking:",
		k_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_message,
		"networking_debug.txt",
		build_networking_buffer_for_log,
		_event_message
	},
	{
		"networking:leaderboards:",
		k_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_message,
		"leaderboard_debug.txt",
		NULL,
		_event_message
	},
	{
		"networking:spawning:",
		k_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_message,
		"networking_debug.txt",
		build_networking_buffer_for_log,
		k_event_level_none
	},
	{
		"networking:camera:",
		k_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_message,
		"networking_debug.txt",
		build_networking_buffer_for_log,
		k_event_level_none
	},
	{
		"datamine:",
		_event_warning,
		{ 1.0f, 0.4f, 0.7f },
		_event_message,
		"networking_debug.txt",
		build_networking_buffer_for_log,
		_event_message
	},
	{
		"tags:",
		_event_error,
		{ 1.0f, 0.0f, 1.0f },
		_event_warning,
		"tag_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"ui:",
		k_event_level_none,
		{ 1.0f, 0.5f, 0.0f },
		k_event_level_none,
		"ui_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"ui:window_manager:",
		k_event_level_none,
		{ 1.0f, 0.5f, 0.0f },
		_event_message,
		"ui_debug.txt",
		NULL,
		_event_message
	},
	{
		"ui:memory_peak:",
		k_event_level_none,
		{ 1.0f, 0.5f, 0.0f },
		_event_message,
		"ui_debug.txt",
		NULL,
		_event_message
	},
	{
		"sound:",
		k_event_level_none,
		{ 0.7f, 0.7f, 0.7f },
		_event_warning,
		"sound_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"multiplayer:",
		k_event_level_none,
		{ 0.8f, 0.1f, 0.6f },
		k_event_level_none,
		"multiplayer_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"effects:",
		k_event_level_none,
		{ 0.0f, 0.0f, 0.8f },
		k_event_level_none,
		"effects_debug.txt",
		NULL,
		_event_error
	},
	{
		"animation_audio_content:",
		k_event_level_none,
		{ 0.7f, 0.7f, 0.5f },
		k_event_level_none,
		"animation_audio_content_debug_4tx_",
		NULL,
		k_event_level_none
	},
	{
		"environment_materials:",
		k_event_level_none,
		{ 0.0f, 0.0f, 0.6f },
		k_event_level_none,
		"environment_materials_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"object_materials:",
		k_event_level_none,
		{ 0.4f, 0.1f, 0.4f },
		k_event_level_none,
		"object_materials_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"design:",
		k_event_level_none,
		{ 1.0f, 0.63f, 0.48f },
		k_event_level_none,
		"design_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"localization:",
		k_event_level_none,
		{ 0.0f, 0.0f, 0.0f },
		k_event_level_none,
		"localization_debug.txt",
		NULL,
		k_event_level_none
	},
	{
		"hs:",
		_event_warning,
		{ 0.5f, 0.8f, 0.3f },
		_event_message,
		"hs_log.txt",
		NULL,
		k_event_level_none
	},
	{
		"tags:xsync:",
		k_event_level_none,
		{ 1.0f, 1.0f, 0.0f },
		_event_message,
		NULL,
		NULL,
		k_event_level_none
	},
	{
		"tags:cache_builder:",
		k_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_message,
		NULL,
		NULL,
		k_event_level_none
	},
	{
		"system:",
		_event_warning,
		{ 1.0f, 1.0f, 1.0f },
		_event_message,
		"system_log.txt",
		NULL,
		k_event_level_none
	},
	{
		"cache:",
		k_event_level_none,
		{ 1.0f, 0.4f, 0.7f },
		_event_message,
		NULL,
		NULL,
		k_event_level_none
	},
	{
		"debugger:",
		k_event_level_none,
		{ 1.0f, 1.0f, 1.0f },
		_event_message,
		NULL,
		NULL,
		k_event_level_none
	},
};

struct s_event_spamming_list_add_result
{
	uns32 last_spam_time;
	int32 hit_count;
};

void events_clear()
{
	event_logs_flush();
	event_globals.message_buffer_size = 0;
	event_globals.message_buffer[0] = 0;
}

void events_debug_render()
{
	if (!g_events_debug_render_enable)
		return;

	rectangle2d display_title_safe_pixel_bounds{};
	interface_get_current_display_settings(nullptr, nullptr, nullptr, &display_title_safe_pixel_bounds);

	rectangle2d bounds{};
	bounds.x0 = display_title_safe_pixel_bounds.x0;
	bounds.y0 = display_title_safe_pixel_bounds.y0 + 70;
	bounds.x1 = SHRT_MAX;
	bounds.y1 = SHRT_MAX;

	real_argb_color color{};
	color.alpha = global_real_argb_red->alpha;

	real32 interpolation_factor = (system_milliseconds() % 1000) / 1000.0f;
	interpolate_linear(global_real_argb_red->red, global_real_argb_white->red, interpolation_factor);
	interpolate_linear(global_real_argb_red->green, global_real_argb_white->green, interpolation_factor);
	interpolate_linear(global_real_argb_red->blue, global_real_argb_white->blue, interpolation_factor);

	c_simple_font_draw_string draw_string;
	interface_set_bitmap_text_draw_mode(&draw_string, _terminal_font, _text_style_plain, _text_justification_left, 0, 5, 0);
	draw_string.set_color(&color);
	draw_string.set_tab_stops(nullptr, 0);
	draw_string.set_bounds(&bounds);

	bool first_event = true;
	for (int32 i = 0; i < event_globals.spamming_event_list.get_count(); i++)
	{
		s_spamming_event& spamming_event = event_globals.spamming_event_list[i];

		if (spamming_event.valid)
		{
			if ((system_milliseconds() - spamming_event.last_spam_time) > 3000)
				csmemset(&spamming_event, 0, sizeof(s_spamming_event));
		}

		if (spamming_event.valid && spamming_event.hit_count >= 2)
		{
			if (first_event)
			{
				draw_string.draw(NULL, spamming_event.spam_text);
				first_event = false;
			}
			else
			{
				draw_string.draw_more(NULL, spamming_event.spam_text);
			}
		}
	}
}

const char* __cdecl events_get()
{
	return event_globals.message_buffer;
}

bool __cdecl event_level_query(e_event_level event_level)
{
	return event_level >= event_globals.current_minimum_level;
}

bool __cdecl event_thread_query()
{
	return _bittest(&event_globals.permitted_thread_bits, get_current_thread_index());
}

int32 event_parse_categories(const char* event_name, int32 max_categories, int32 category_name_max_length, char(*category_names)[64])
{
	const char* category_substring = event_name;
	int32 category_index = 0;
	int32 category_count = 0;
	bool succeeded = false;
	bool failed = false;
	do
	{
		const char* category_name = category_substring;
		int32 category_name_length = 0;
		bool category_found = false;
		do
		{
			char temp = *category_substring;
			if (category_name_length < category_name_max_length)
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
				VASSERT(c_string_builder("network event category substring #%d '%s' exceeded %d characters", category_index, category_name, category_name_max_length).get_string());

				failed = true;
			}
		} while (!category_found && !failed);

		if (category_found)
		{
			if (category_name_length)
			{
				if (category_index < max_categories)
				{
					csstrnzcpy(category_names[category_index], category_name, category_name_max_length);
					category_names[category_index][category_name_length] = 0;
					ascii_strnlwr(category_names[category_index++], category_name_length);
				}
				else
				{
					event(_event_error, "network event category #%d '%s' exceeded %d category substrings", category_index, category_substring, max_categories);

					failed = true;
				}
			}
			else
			{
				succeeded = true;
			}
		}
	} while (!succeeded && !failed);

	if (failed)
		VASSERT(c_string_builder("failed to parse network event '%s'", event_name).get_string());

	if (category_index > category_count)
		category_index = category_count;

	return category_index;
}

s_event_category* event_category_get(int32 category_index)
{
	return &event_globals.categories[category_index];
}

int32 event_find_category_recursive(int32 parent_category_index, bool create_category, int32 category_count, char(*category_names)[64])
{
	while (true)
	{
		const char* category_name = category_names[0];
		int32 category_index = parent_category_index;

		ASSERT(category_count > 0);
		ASSERT(csstrnlen(category_names[0], NUMBEROF(category_names[0])) > 0);

		int32 next_category_index = NONE;
		g_event_read_write_lock.read_lock();
		
		ASSERT(category_index != NONE);

		s_event_category* temp_category = nullptr;
		for (next_category_index = event_category_get(category_index)->first_child_index;
			next_category_index != NONE;
			next_category_index = temp_category->sibling_index)
		{
			temp_category = event_category_get(next_category_index);
			if (csstrnicmp(temp_category->name, category_name, sizeof(temp_category->name)) == 0)
				break;
		}

		g_event_read_write_lock.read_unlock();

		if (next_category_index == NONE)
		{
			if (!create_category)
				return next_category_index;

			g_event_read_write_lock.write_lock();

			s_event_category* _RDI = NULL;
			int32 category_index_ = event_globals.category_count;
			if (event_globals.category_count < 1024
				&& (next_category_index = event_globals.category_count++,
					(_RDI = event_category_get(category_index_)) != NULL))
			{
				s_event_category* _RBX = event_category_get(parent_category_index);

				csstrnzcpy(_RDI->name, category_name, sizeof(_RDI->name));
				csstrnzcpy(_RDI->log_name, _RBX->log_name, sizeof(_RDI->log_name));
				_RDI->current_display_level = _RBX->current_display_level;
				_RDI->current_display_color = _RBX->current_display_color;
				_RDI->current_log_level = _RBX->current_log_level;
				_RDI->current_remote_log_level = _RBX->current_remote_log_level;
				_RDI->current_debugger_break_level = _RBX->current_debugger_break_level;
				_RDI->current_halt_level = _RBX->current_halt_level;
				_RDI->current_force_display_level = _RBX->current_force_display_level;
				_RDI->first_child_index = NONE;
				_RDI->depth++;
				_RDI->parent_index = parent_category_index;
				_RDI->sibling_index = _RBX->first_child_index;
				_RDI->event_log_index = _RBX->event_log_index;
				_RDI->log_format_func = _RBX->log_format_func;
				_RDI->event_listeners = _RBX->event_listeners;
				_RBX->first_child_index = next_category_index;
			}
			else
			{
				console_printf("event_find_category_recursive: ran out of categories creating '%s'", category_name);
			}

			g_event_read_write_lock.write_unlock();

			if (next_category_index == NONE)
				return next_category_index;
		}

		if (category_count <= 1)
			return next_category_index;

		category_names++;
		category_count--;
		parent_category_index = next_category_index;
	}
}

int32 event_find_category(bool create_category, int32 category_count, char(*category_names)[64])
{
	if (category_count > 0)
		return event_find_category_recursive(0, create_category, category_count, category_names);

	return 0;
}

int32 event_category_from_name(const char* event_name, bool create_category)
{
	ASSERT(event_name);

	char categories[8][64]{};

	int32 category_count = event_parse_categories(event_name, 8, 64, categories);
	if (category_count > 0)
		return event_find_category(create_category, category_count, categories);

	return 0;
}

void event_initialize_categories()
{
	ASSERT(event_globals.category_count == 0);

	s_event_category* category = event_category_get(0);
	csstrnzcpy(category->name, "", sizeof(category->name));
	csstrnzcpy(category->log_name, "", sizeof(category->log_name));
	category->current_display_level = k_event_level_none;
	category->current_log_level = k_event_level_none;
	category->current_remote_log_level = k_event_level_none;
	category->current_debugger_break_level = k_event_level_none;
	category->current_halt_level = k_event_level_none;
	category->depth = 0;
	category->parent_index = NONE;
	category->first_child_index = NONE;
	category->sibling_index = NONE;
	category->current_display_color = global_real_argb_white->rgb;
	category->event_log_index = NONE;
	category->log_format_func = nullptr;
	category->event_listeners = 0;
	event_globals.category_count++;

	for (int32 i = 0; i < NUMBEROF(g_log_events); i++)
	{
		const s_event_category_default_configuration* log_event = &g_log_events[i];
		int32 category_index = event_category_from_name(log_event->name, true);
		s_event_category* next_category = event_category_get(category_index);
		next_category->current_display_level = log_event->initial_display_level;
		next_category->current_display_color = log_event->initial_display_color;
		if (log_event->log_name)
		{
			csstrnzcpy(next_category->log_name, log_event->log_name, sizeof(next_category->log_name));
			next_category->event_log_index = NONE;// event_log_new(event_->log_name, FLAG(3), true);
		}
		next_category->current_log_level = log_event->initial_log_level;
		next_category->current_remote_log_level = log_event->initial_remote_log_level;
		next_category->current_debugger_break_level = k_event_level_none;
		next_category->current_halt_level = k_event_level_none;
		next_category->current_force_display_level = k_event_level_none;
		next_category->log_format_func = log_event->log_format_func;
	}
}

bool events_initialize_if_possible()
{
	static bool run_once = false;
	if (!run_once && is_main_thread() /*&& synchronization_objects_initialized()*/)
	{
		g_event_read_write_lock.setup(k_crit_section_event_rw_lock, 1);
		run_once = true;

		event_globals.enable_events = true;
		event_globals.disable_event_suppression = false;
		event_globals.enable_spam_suppression = shell_application_type() == _shell_application_game;
		event_globals.dump_to_stderr = shell_application_type() != _shell_application_tool;
		event_globals.current_display_level = _event_warning;
		event_globals.current_log_level = _event_warning;
		event_globals.current_remote_log_level = _event_warning;
		event_globals.current_minimum_level = k_event_level_none;
		event_globals.current_minimum_category_level = k_event_level_none;

		event_globals.event_index = 0;
		event_globals.event_listeners.set_all(nullptr);

		event_globals.console_suppression_old_time = 0;
		event_globals.console_suppression_count = 0;
		event_globals.console_suppression_old_line_check_time = 0;

		csmemset(event_globals.spamming_event_list.get_elements(), 0, event_globals.spamming_event_list.get_total_element_size());

		event_globals.permitted_thread_bits = 0xFFFFFBF7; // ~(FLAG(3) | FLAG(10));
		event_globals.disable_event_log_trimming = false;
		event_globals.disable_event_logging = false;

		// clear function
		event_globals.message_buffer_size = 0;
		event_globals.message_buffer[0] = 0;

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
	event(_event_message, "lifecycle: events initalize");
}

int32 __cdecl event_interlocked_compare_exchange(int32 volatile* destination, int32 exchange, int32 comperand)
{
	return (int32)_InterlockedCompareExchange(destination, exchange, comperand);
}

void event_logs_flush()
{
	//if (!thread_has_crashed(k_thread_network_block_detection))
	//	flush_event_log_cache();
}

c_event::c_event(e_event_level event_level, int32 event_category_index, uns32 event_response_suppress_flags) :
	m_event_level(event_level),
	m_event_category_index(event_category_index),
	m_event_response_suppress_flags(event_response_suppress_flags)
{
}

bool c_event::query()
{
	return event_thread_query() && event_level_query(m_event_level);
}

uns32 event_query(e_event_level event_level, int32 category_index, uns32 event_response_suppress_flags)
{
	ASSERT(g_events_initialized);

	g_event_read_write_lock.read_lock();

	uns32 flags = 0;
	if (event_globals.current_display_level != k_event_level_none)
		flags = event_level >= event_globals.current_display_level;

	if (event_globals.current_log_level != k_event_level_none)
		SET_BIT(flags, _category_properties_log_level_bit, event_level >= event_globals.current_log_level);

	if (event_globals.current_remote_log_level != k_event_level_none)
		SET_BIT(flags, _category_properties_remote_log_level_bit, event_level >= event_globals.current_remote_log_level);

	if (category_index != NONE)
	{
		s_event_category* category = event_category_get(category_index);
		if (category->current_display_level != k_event_level_none)
			SET_BIT(flags, _category_properties_display_level_bit, event_level >= category->current_display_level);

		if (category->current_remote_log_level != k_event_level_none)
			SET_BIT(flags, _category_properties_remote_log_level_bit, event_level >= category->current_remote_log_level);

		if (category->current_debugger_break_level != k_event_level_none)
			SET_BIT(flags, _category_properties_debugger_break_level_bit, event_level >= category->current_debugger_break_level);

		if (category->current_halt_level != k_event_level_none)
			SET_BIT(flags, _category_properties_halt_level_bit, event_level >= category->current_halt_level);

		if (category->current_force_display_level != k_event_level_none)
			SET_BIT(flags, _category_properties_force_display_level_bit, event_level >= category->current_force_display_level);

	}

	if (!event_globals.disable_event_suppression && TEST_BIT(event_response_suppress_flags, 0))
		flags &= ~FLAG(_category_properties_display_level_bit);

	if (event_globals.disable_event_logging)
		flags &= ~FLAG(_category_properties_log_level_bit);

	g_event_read_write_lock.read_unlock();

	return flags;
}

void add_event_to_spamming_list(const char* event_text, s_event_spamming_list_add_result* result_out)
{
	ASSERT(event_text);
	ASSERT(result_out);

	int32 event_index = NONE;
	bool event_exists = false;

	for (int32 i = 0; i < event_globals.spamming_event_list.get_count(); i++)
	{
		s_spamming_event* spamming_event = &event_globals.spamming_event_list[i];
		if (spamming_event->valid)
		{
			if (csmemcmp(spamming_event->spam_text, event_text, sizeof(spamming_event->spam_text)) == 0)
			{
				result_out->last_spam_time = spamming_event->last_spam_time;
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
		s_spamming_event* spamming_event = &event_globals.spamming_event_list[event_index];
		ASSERT(!spamming_event->valid);

		spamming_event->valid = true;
		spamming_event->last_spam_time = system_milliseconds();
		spamming_event->hit_count = 1;
		csstrnzcpy(spamming_event->spam_text, event_text, sizeof(spamming_event->spam_text));
	}
}

uns32 event_update_spam_prevention(uns32 response_flags, e_event_level event_level, int32 category_index, const char* event_text)
{
	if (event_globals.enable_spam_suppression
		&& event_level != _event_critical
		&& TEST_BIT(response_flags, 0)
		&& !TEST_BIT(response_flags, 1))
	{
		g_event_read_write_lock.write_lock();
		s_event_category* category = event_category_get(category_index);

		uns32 time = system_milliseconds();
		if (time > category->last_event_time + 10000)
			category->possible_spam_event_count = 0;

		category->possible_spam_event_count++;
		category->last_event_time = time;

		bool a1 = category->possible_spam_event_count >= 5;

		g_event_read_write_lock.write_unlock();

		if (a1)
		{
			s_event_spamming_list_add_result result{ .hit_count = 0 };
			add_event_to_spamming_list(event_text, &result);
			if (result.hit_count > 1)
				return 0;
		}
	}
	return response_flags;
}

bool console_update_spam_prevention(e_event_level event_level)
{
	if (!event_globals.enable_spam_suppression || event_level == _event_critical)
	{
		return false;
	}

	int32 current_time = (int32)system_milliseconds();
	int32 last_check_time = event_globals.console_suppression_old_line_check_time;
	
	if (!event_globals.console_suppression_count)
	{
		last_check_time = current_time;
	}

	int32 new_suppression_count = event_globals.console_suppression_count + 1;
	event_globals.console_suppression_old_line_check_time = last_check_time;
	event_globals.console_suppression_count = new_suppression_count;

	if (new_suppression_count < 5 && current_time > last_check_time + 2000)
	{
		int elapsed_intervals = (current_time - last_check_time) / 2000;
		new_suppression_count -= elapsed_intervals;
		if (new_suppression_count < 0)
		{
			new_suppression_count = 0;
		}
		event_globals.console_suppression_count = new_suppression_count;
		event_globals.console_suppression_old_line_check_time = last_check_time + elapsed_intervals + 4 * elapsed_intervals;
	}

	if (current_time > event_globals.console_suppression_old_time + 10000)
	{
		event_globals.console_suppression_count = 0;
		event_globals.console_suppression_old_time = current_time;
		return false;
	}

	bool print_suppression_message = false;
	if (new_suppression_count == 5)
	{
		print_suppression_message = true;
	}
	else if (new_suppression_count < 5)
	{
		return false;
	}

	if (print_suppression_message)
	{
		terminal_printf(global_real_argb_white, "too many errors, only printing to debug.txt");
	}

	status_printf("suppressing events");
	return true;
}

void write_to_console(e_event_level event_level, int32 category_index, const char* string)
{
	enum
	{
		copy_size = 34,
	};

	bool should_update = console_update_spam_prevention(event_level);
	//display_debug_string(string);
	c_console::write_line(string);

	if (!should_update)
	{
		char buffer[1040]{};
		csstrnzcpy(buffer, string, 1027);

		if (event_globals.dump_to_stderr)
		{
			fprintf(stderr, "%s\r\n", buffer);
		}
		else
		{
			real_argb_color color = *global_real_argb_white;

			if (event_level == _event_critical)
			{
				color = *global_real_argb_red;
			}
			else
			{
				g_event_read_write_lock.read_lock();
				s_event_category* category = event_category_get(category_index);
				color.rgb = category->current_display_color;
				g_event_read_write_lock.read_unlock();
			}

			terminal_printf(&color, "%s", buffer);
		}

		csstrnzcat(buffer, "\r\n", 1027);
		g_event_read_write_lock.write_lock();

		int16 new_size = (int16)csstrnlen(buffer, sizeof(buffer)-1);
		int16 message_buffer_size = event_globals.message_buffer_size;

		if (message_buffer_size + new_size >= k_error_message_buffer_maximum_size)
		{
			int16 new_position = 0;
			int16 calculated_size = new_size + 1058;

			if (calculated_size > 0)
			{
				if (calculated_size >= event_globals.message_buffer_size - 1)
				{
					new_position = event_globals.message_buffer_size - 1;
				}
				else
				{
					new_position = calculated_size;
				}
			}

			char* newline_pos = strchr(&event_globals.message_buffer[new_position], '\n');
			int16 chars_to_keep = newline_pos ?
				(newline_pos - event_globals.message_buffer + 1) :
				event_globals.message_buffer_size;

			int16 prefix_size = event_globals.message_buffer_size - chars_to_keep;
			ASSERT(prefix_size + copy_size + new_size < k_error_message_buffer_maximum_size);
			ASSERT(prefix_size >= 0 && copy_size >= 0 && new_size >= 0);
			ASSERT(VALID_INDEX(prefix_size + copy_size, NUMBEROF(event_globals.message_buffer)));

			csmemcpy(event_globals.message_buffer, "\r\n", 2);
			if (prefix_size > 0 && newline_pos)
			{
				memmove(&event_globals.message_buffer[copy_size], newline_pos, prefix_size);
			}

			message_buffer_size = prefix_size + copy_size;
			event_globals.message_buffer[prefix_size + copy_size] = 0;
			event_globals.message_buffer_size = message_buffer_size;
		}

		if (message_buffer_size + new_size < k_error_message_buffer_maximum_size)
		{
			csstrnzcpy(&event_globals.message_buffer[message_buffer_size],
				buffer,
				k_error_message_buffer_maximum_size - message_buffer_size);
			event_globals.message_buffer_size += new_size;
		}

		g_event_read_write_lock.write_unlock();
	}
}

void event_generated_handle_console(e_event_level event_level, int32 category_index, const char* event_text, bool force)
{
	char context_text[256]{};
	char final_text[2048]{};

	if (event_globals.suppress_console_display_and_show_spinner)
	{
		if (!force)
			return;
	}
	else if (force)
	{
		event_level = _event_critical;
	}

	csstrnzcpy(context_text, "", sizeof(context_text));
	bool has_context = false;
	if (g_event_context_stack_depth > 0)
	{
		for (int32 context_index = 0; context_index < g_event_context_stack_depth; context_index++)
		{
			s_event_context* context = &g_event_context_stack[context_index];

			if (context->display_to_console)
			{
				csstrnzcat(context_text, context->description, sizeof(context_text));
				if (context_index < g_event_context_stack_depth - 1)
				{
					csstrnzcat(context_text, ":", sizeof(context_text));
				}
				has_context = true;
			}
		}
	}

	if (has_context)
	{
		csnzprintf(final_text, sizeof(final_text), "%s (%s) %s", k_event_level_severity_strings[event_level], context_text, event_text);
	}
	else
	{
		csnzprintf(final_text, sizeof(final_text), "%s %s", k_event_level_severity_strings[event_level], event_text);
	}

	write_to_console(event_level, category_index, final_text);
}

void event_generated_handle_log(e_event_level event_level, int32 category_index, const char* event_text)
{
}

void event_generated_handle_datamine(e_event_level event_level, const char* format, va_list argument_list)
{
}

void event_generated_handle_debugger_break(const char* event_text)
{
	//static bool x_disable_debugger = false;
	//if (x_disable_debugger)
	//{
	//	char debugstring[2048];
	//	csnzprintf(debugstring, sizeof(debugstring), "critical event encountered: %s", event_text);
	//}
}

void event_generated_handle_halt(const char* event_text)
{
	static bool x_disable_halt = false;
	if (x_disable_halt)
	{
		char debugstring[2048];
		csnzprintf(debugstring, sizeof(debugstring), "critical event encountered: %s", event_text);
	}
}

void event_generate(e_event_level event_level, int32 category_index, uns32 event_response_suppress_flags, const char* format, va_list argument_list)
{
	ASSERT(g_events_initialized);

	if (uns32 flags = event_query(event_level, category_index, event_response_suppress_flags))
	{
		char event_text[2048]{};

		cvsnzprintf(event_text, sizeof(event_text), format, argument_list);

		s_event_category* category = event_category_get(category_index);

		if (uns32 undated = event_update_spam_prevention(flags, event_level, category_index, event_text))
		{
			if (TEST_BIT(undated, _category_properties_display_level_bit) || TEST_BIT(undated, _category_properties_force_display_level_bit))
				event_generated_handle_console(event_level, category_index, event_text, TEST_BIT(undated, _category_properties_force_display_level_bit));

			if (TEST_BIT(undated, _category_properties_log_level_bit))
				event_generated_handle_log(event_level, category_index, event_text);

			if (TEST_BIT(undated, _category_properties_remote_log_level_bit))
				event_generated_handle_datamine(event_level, format, argument_list);

			if (TEST_BIT(undated, _category_properties_debugger_break_level_bit))
				event_generated_handle_debugger_break(event_text);

			if (TEST_BIT(undated, _category_properties_halt_level_bit))
				event_generated_handle_halt(event_text);
		}

		g_event_read_write_lock.read_lock();
		for (int32 event_listener_index = 0; event_listener_index < event_globals.event_listeners.get_count(); event_listener_index++)
		{
			if (TEST_BIT(category->event_listeners, event_listener_index))
			{
				ASSERT(event_globals.event_listeners[event_listener_index]);
				event_globals.event_listeners[event_listener_index]->handle_event(event_level, event_text);
			}
		}
		g_event_read_write_lock.read_unlock();

		g_event_read_write_lock.write_lock();
		event_globals.event_index++;
		g_event_read_write_lock.write_unlock();
	}
}

int32 c_event::generate(const char* format, ...)
{
	va_list list;
	va_start(list, format);

	if (g_recursion_lock)
	{
		//if (events_initialize_if_possible() && event_globals.enable_events)
		//	event_generate_handle_recursive(m_event_level, m_event_category_index, m_event_response_suppress_flags, format, list);
	}
	else
	{
		g_recursion_lock = true;
		main_loop_pregame_disable(true);

		if (events_initialize_if_possible() && event_globals.enable_events)
		{
			if (m_event_category_index == NONE)
				m_event_category_index = event_category_from_name(format, true);

			event_generate(m_event_level, m_event_category_index, m_event_response_suppress_flags, format, list);
		}

		main_loop_pregame_disable(false);
		g_recursion_lock = false;
	}

	va_end(list);

	return m_event_category_index;
}

// used inplace of `c_event::generate`
// net::REMOTE_BINLOGGER
void __cdecl network_debug_print(const char* format, ...)
{
	int32 format_address = (int32)format;

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

