#include "text/font_loading.hpp"

#include "cseries/async.hpp"
#include "cseries/cseries_events.hpp"
#include "main/global_preferences.hpp"
#include "memory/module.hpp"
#include "text/draw_string.hpp"
#include "text/font_fallback.hpp"
#include "text/font_group.hpp"
#include "text/font_package_cache.hpp"

HOOK_DECLARE(0x00509140, font_block_until_load_completes);
HOOK_DECLARE(0x005091A0, font_close_loaded_file);
HOOK_DECLARE(0x00509210, font_dispose);
HOOK_DECLARE(0x00509280, font_get_debug_name);
HOOK_DECLARE(0x005092C0, font_get_font_index);
HOOK_DECLARE(0x005092F0, font_get_header);
HOOK_DECLARE(0x00509330, font_get_header_internal);
HOOK_DECLARE(0x00509360, font_get_package_file_handle);
HOOK_DECLARE(0x00509380, font_get_package_header_internal);
HOOK_DECLARE(0x00509390, font_idle);
HOOK_DECLARE(0x005093C0, font_in_emergency_mode);
HOOK_DECLARE(0x005093D0, font_initialize);
HOOK_DECLARE(0x00509420, font_initialize_emergency);
HOOK_DECLARE(0x00509480, font_load);
HOOK_DECLARE(0x00509550, font_load_callback);
HOOK_DECLARE(0x005096B0, font_load_idle);
HOOK_DECLARE(0x005096F0, font_loading_idle);
HOOK_DECLARE(0x00509780, font_reload);
//HOOK_DECLARE(0x00509840, font_table_get_font_file_references);
HOOK_DECLARE(0x005099A0, fonts_begin_loading);
HOOK_DECLARE(0x00509A50, fonts_close);
HOOK_DECLARE(0x00509A90, fonts_copy_to_hard_drive);
HOOK_DECLARE(0x00509AA0, fonts_invalidate_cached_fonts);
HOOK_DECLARE(0x00509AC0, fonts_select_language);
HOOK_DECLARE(0x00509B50, get_active_font_directory);
HOOK_DECLARE(0x00509B90, get_dvd_font_directory);
HOOK_DECLARE(0x00509BB0, get_font_master_filename);
HOOK_DECLARE(0x00509C20, get_hard_drive_font_directory);

REFERENCE_DECLARE(0x022B7FAC, s_font_globals, g_font_globals);
REFERENCE_DECLARE(0x02457BB8, s_font_package_cache, g_font_package_cache);

REFERENCE_DECLARE(0x0189D358, const char*, k_hard_drive_font_directory);
REFERENCE_DECLARE(0x0189D35C, const char*, k_dvd_font_directory);
REFERENCE_DECLARE(0x0189D360, const char* const, k_font_package_base_name);
REFERENCE_DECLARE(0x0189D364, const char* const, k_font_package_suffix);

void __cdecl font_block_until_load_completes(s_font_loading_state* loading_state)
{
	//INVOKE(0x00509140, font_block_until_load_completes, loading_state);

	if (loading_state->started && !loading_state->finished)
	{
		async_task_change_priority(loading_state->task_id, _async_priority_blocking_generic);
		internal_async_yield_until_done_attributed(&loading_state->finished, false, false, _yield_for_font_cache, __FILE__, __LINE__);
	}
}

void __cdecl font_close_loaded_file(s_font_loading_state* loading_state)
{
	//INVOKE(0x005091A0, font_close_loaded_file, loading_state);

	font_block_until_load_completes(loading_state);

	if (loading_state->file_open)
		file_close(&loading_state->file_reference);
}

void __cdecl font_dispose()
{
	//INVOKE(0x00509210, font_dispose);

	if (g_font_globals.initialized)
	{
		font_close_loaded_file(&g_font_globals.package_loading_state);
		csmemset(&g_font_globals.package_loading_state, 0, sizeof(g_font_globals.package_loading_state));
		g_font_globals.font_package_header = NULL;
		font_cache_delete();
		font_package_cache_delete();
	}
	csmemset(&g_font_globals, 0, sizeof(g_font_globals));
}

const char* __cdecl font_get_debug_name(e_font_index internal_index)
{
	//return INVOKE(0x00509280, font_get_debug_name, internal_index);

	const s_font_header* header = font_get_header_internal(internal_index);
	if (header)
		return header->debug_name;

	if (internal_index == _font_index_fallback)
		return "fallback-font";

	return NULL;
}

e_font_index __cdecl font_get_font_index(e_font_id font)
{
	//return INVOKE(0x005092C0, font_get_font_index, font);

	e_font_index result = _font_index_none;
	if (g_font_globals.font_package_header && font >= 0 && font < 16)
		result = (e_font_index)g_font_globals.font_package_header->font_mapping[font];

	if (g_font_globals.emergency_mode || result == _font_index_none)
		return _font_index_fallback;

	return result;
}

const s_font_header* __cdecl font_get_header(e_font_id font)
{
	//return INVOKE(0x005092F0, font_get_header, font);

	e_font_index font_index = font_get_font_index(font);
	const s_font_header* result = font_get_header_internal(font_index);
	return result;
}

const s_font_header* __cdecl font_get_header_internal(e_font_index internal_index)
{
	//return INVOKE(0x00509330, font_get_header_internal, internal_index);

	if (g_font_globals.font_package_header && internal_index >= 0 && internal_index < g_font_globals.font_package_header->font_count)
	{
		int32 header_offset = g_font_globals.font_package_header->fonts[internal_index].header_offset;
		return (s_font_header*)offset_pointer(g_font_globals.font_package_header, header_offset);
	}

	if (internal_index == _font_index_fallback)
	{
		return fallback_font_get_header();
	}

	return NULL;
}

bool __cdecl font_get_package_file_handle(s_file_handle* out_file_handle)
{
	//return INVOKE(0x00509360, font_get_package_file_handle, out_file_handle);

	return g_font_globals.package_loading_state.file_open && file_reference_get_file_handle(&g_font_globals.package_loading_state.file_reference, out_file_handle);
}

const s_font_package_file_header* __cdecl font_get_package_header_internal()
{
	//return INVOKE(0x00509380, font_get_package_header_internal);

	return g_font_globals.font_package_header;
}

void __cdecl font_idle()
{
	//INVOKE(0x00509390, font_idle);

	if (g_font_globals.initialized && !g_font_globals.emergency_mode)
	{
		font_loading_idle();
		font_cache_idle();
		font_package_cache_idle();
	}
}

bool __cdecl font_in_emergency_mode()
{
	//return INVOKE(0x005093C0, font_in_emergency_mode);

	return g_font_globals.emergency_mode;
}

void __cdecl font_initialize()
{
	//INVOKE(0x005093D0, font_initialize);

	csmemset(&g_font_globals, 0, sizeof(g_font_globals));
	g_font_globals.language = _language_invalid;
	fallback_font_initialize();
	fonts_select_language();
	fonts_copy_to_hard_drive();
	fonts_begin_loading(false);
	font_package_cache_new();
	font_cache_new();
	g_font_globals.initialized = true;
}

void __cdecl font_initialize_emergency()
{
	//INVOKE(0x00509420, font_initialize_emergency);

	if (!g_font_globals.initialized)
	{
		csmemset(&g_font_globals, 0, sizeof(g_font_globals));
		g_font_globals.language = _language_invalid;
		fallback_font_initialize();
		font_cache_new();
		font_package_cache_new();
		g_font_globals.initialized = true;
	}
	
	c_font_cache_mt_safe font_cache{};
	g_font_globals.emergency_mode = true;
}

void __cdecl font_load(struct s_font_loading_state* loading_state, e_font_index font_index, const char* filename, bool blocking)
{
	//INVOKE(0x00509480, font_load, loading_state, font_index, filename, blocking);

	e_async_priority priority = blocking ? _async_priority_blocking_generic : _async_priority_important_non_blocking;

	ASSERT(!loading_state->started);
	ASSERT(!loading_state->finished);
	ASSERT(!loading_state->failed);
	
	csstrnzcpy(loading_state->debug_filename, filename, 32);
	loading_state->font_index = font_index;
	
	get_active_font_directory(&loading_state->file_reference);
	file_reference_set_name(&loading_state->file_reference, filename);
	loading_state->started = true;

	s_font_loading_task task{};
	task.loading_state = loading_state;
	
	loading_state->task_id = async_task_add(priority, (s_async_task*)&task, _async_category_text, (e_async_completion(__cdecl*)(s_async_task*))font_load_callback, &loading_state->finished);
	
	if (blocking)
		font_block_until_load_completes(loading_state);
}

e_async_completion __cdecl font_load_callback(s_font_loading_task* in_task)
{
	//return INVOKE(0x00509550, font_load_callback, in_task);

	s_font_loading_state* loading_state = in_task->loading_state;
	e_async_completion completion = _async_completion_retry;

	uns32 unused_error_code = 0;
	if (loading_state->file_open)
	{
		bool loaded_font_file = false;
		if (file_read(&loading_state->file_reference, sizeof(s_font_package_file), false, &g_font_globals.package_file))
		{
			font_package_file_header_byteswap(&g_font_globals.package_file.header);

			if (font_package_file_header_validate(&g_font_globals.package_file.header))
			{
				bool load_package_file_failed = false;
				for (int32 font_index = 0; font_index < g_font_globals.package_file.header.font_count && !load_package_file_failed; ++font_index)
				{
					s_font_package_font& font = g_font_globals.package_file.header.fonts[font_index];
					if (font.header_offset < (int32)sizeof(s_font_package_file_header) &&
						font.header_offset + (int32)sizeof(s_font_header) > g_font_globals.package_file.header.header_data_offset + g_font_globals.package_file.header.header_data_size)
					{
						event(_event_error, "fonts: package file font #%d header offset %d (size %d) not valid in [%d, %d+%d]",
							font_index,
							font.header_offset,
							font.header_size,
							g_font_globals.package_file.header.header_data_offset,
							g_font_globals.package_file.header.header_data_offset,
							g_font_globals.package_file.header.header_data_size);

						load_package_file_failed = true;
					}
					else
					{
						s_font_header* header = (s_font_header*)offset_pointer(&g_font_globals.package_file, g_font_globals.package_file.header.fonts[font_index].header_offset);
						font_header_byteswap(header);

						if (font_header_validate(header))
						{
							if (header->kerning_pair_count > 0)
							{
								s_kerning_pair* kerning_pair = (s_kerning_pair*)offset_pointer(header, header->kerning_pairs_offset);
								font_kerning_pairs_byteswap(kerning_pair, header->kerning_pair_count);
							}
						}
						else
						{
							event(_event_error, "fonts: package file font #%d header failed to validate",
								font_index);

							load_package_file_failed = true;
						}
					}
				}

				s_font_package_entry* entry = (s_font_package_entry*)offset_pointer(&g_font_globals.package_file, g_font_globals.package_file.header.package_table_offset);
				font_package_entries_byteswap(entry, g_font_globals.package_file.header.package_table_count);

				if (!load_package_file_failed)
				{
					g_font_globals.font_package_header = &g_font_globals.package_file.header;
					event(_event_message, "fonts: loaded package file '%s' (%d fonts)",
						loading_state->debug_filename,
						g_font_globals.package_file.header.font_count);

					loaded_font_file = true;
				}
			}
			else
			{
				event(_event_error, "fonts: package header failed to validate");
			}

			loaded_font_file = true;
		}
		else
		{
			event(_event_error, "fonts: couldn't read package file header (%d bytes)",
				sizeof(s_font_package_file));
		}

		if (!loaded_font_file)
		{
			event(_event_critical, "fonts: unable to load font file '%s', fonts may be unavailable",
				loading_state->debug_filename);

			loading_state->failed = true;
		}

		completion = _async_completion_done;
	}
	else if (file_open(&loading_state->file_reference, FLAG(_file_open_flag_desired_access_read), &unused_error_code))
	{
		loading_state->file_open = true;
	}
	else
	{
		event(_event_error, "fonts: couldn't open font file '%s'", loading_state->debug_filename);
		loading_state->failed = true;
		completion = _async_completion_done;
	}

	return completion;
}

void __cdecl font_load_idle(s_font_loading_state* loading_state, bool* out_failure_reported)
{
	//INVOKE(0x005096B0, font_load_idle, loading_state, out_failure_reported);

	if (loading_state->finished && loading_state->failed && !*out_failure_reported)
	{
		event(_event_critical, "fonts: font file '%s' failed to load, flushing fonts",
			loading_state->debug_filename);
		*out_failure_reported = true;
	}
}

void __cdecl font_loading_idle()
{
	//INVOKE(0x005096F0, font_loading_idle);

	font_load_idle(&g_font_globals.package_loading_state, &g_font_globals.fonts_unavailable);
	if (g_font_globals.fonts_unavailable)
	{
		if (g_font_globals.cached_to_hard_drive)
		{
			event(_event_error, "fonts: failed to load fonts from hard drive, marking cached fonts as invalid");
			fonts_invalidate_cached_fonts();
			g_font_globals.cached_to_hard_drive = false;
		}

		if (g_font_globals.failure_retry_count < 3)
		{
			g_font_globals.failure_retry_count++;
			event(_event_error, "fonts: failed to load, attempting reload (retry count #%d)",
				g_font_globals.failure_retry_count);
			font_reload();
		}
		else
		{
			if (!g_font_globals.fonts_unavailable)
			{
				event(_event_critical, "fonts: fonts are unavailable after %d retries, marking fonts as permanently unavailable",
					g_font_globals.failure_retry_count);
				g_font_globals.fonts_unavailable = true;
			}

			//damaged_media_halt_and_display_error();
		}
	}
}

void __cdecl font_reload()
{
	//INVOKE(0x00509780, font_reload);

	c_font_cache_mt_safe font_cache;

	fonts_invalidate_cached_fonts();
	font_cache_flush();
	font_package_cache_flush();
	fonts_close();
	fonts_select_language();
	fonts_copy_to_hard_drive();
	fonts_begin_loading(true);
}

int32 __cdecl font_table_get_font_file_references(const char* text, const s_file_reference* directory, s_file_reference* references, int32 max_references, int32* font_id_mapping, int32 max_font_ids)
{
	return INVOKE(0x00509840, font_table_get_font_file_references, text, directory, references, max_references, font_id_mapping, max_font_ids);
}

bool __cdecl fonts_begin_loading(bool load_blocking)
{
	//INVOKE(0x005099A0, fonts_begin_loading, load_blocking);

	char font_package_filename[256]{};
	get_font_master_filename(g_font_globals.language, font_package_filename, sizeof(font_package_filename));
	font_load(&g_font_globals.package_loading_state, _font_index_none, font_package_filename, load_blocking);

	return true;
}

void __cdecl fonts_close()
{
	//INVOKE(0x00509A50, fonts_close);

	font_close_loaded_file(&g_font_globals.package_loading_state);
	csmemset(&g_font_globals.package_loading_state, 0, sizeof(g_font_globals.package_loading_state));
	g_font_globals.font_package_header = NULL;
}

void __cdecl fonts_copy_to_hard_drive()
{
	//INVOKE(0x00509A90, fonts_copy_to_hard_drive);

	g_font_globals.cached_to_hard_drive = false;
}

void __cdecl fonts_invalidate_cached_fonts()
{
	//INVOKE(0x00509AA0, fonts_invalidate_cached_fonts);

	if (global_preferences_get_last_font_language() != _language_invalid)
	{
		global_preferences_set_last_font_language(_language_invalid);
		global_preferences_flush();
	}
}

void __cdecl fonts_select_language()
{
	//INVOKE(0x00509AC0, fonts_select_language);

	e_language current_language = get_current_language();
	if (current_language)
	{
		char fonts_package_filename[256]{};
		s_file_reference dvd_font_directory_file{};
		s_file_reference fonts_package_file{};

		get_font_master_filename(current_language, fonts_package_filename, 256);
		file_reference_create_from_path(&dvd_font_directory_file, k_dvd_font_directory, true);
		file_reference_copy(&fonts_package_file, &dvd_font_directory_file);
		file_reference_set_name(&fonts_package_file, fonts_package_filename);
		if (!file_exists(&fonts_package_file))
			current_language = k_language_default;
	}
	g_font_globals.language = current_language;
}

void __cdecl get_active_font_directory(s_file_reference* file)
{
	//INVOKE(0x00509B50, get_active_font_directory, file);

	if (g_font_globals.cached_to_hard_drive)
		get_hard_drive_font_directory(file);
	else
		get_dvd_font_directory(file);
}

void __cdecl get_dvd_font_directory(s_file_reference* file)
{
	//INVOKE(0x00509B90, get_dvd_font_directory, file);

	file_reference_create_from_path(file, k_dvd_font_directory, true);
}

void __cdecl get_font_master_filename(e_language language, char* buffer, int32 buffer_size)
{
	//INVOKE(0x00509BB0, get_font_master_filename, language, buffer, buffer_size);

	csstrnzcpy(buffer, k_font_package_base_name, buffer_size);
	if (language)
	{
		const char* suffix = get_language_suffix(language, true);
		if (*suffix)
		{
			csnzappendf(buffer, buffer_size, "_");
			csnzappendf(buffer, buffer_size, suffix);
		}
	}
	
	csnzappendf(buffer, buffer_size, k_font_package_suffix);
}

void __cdecl get_hard_drive_font_directory(s_file_reference* file)
{
	//INVOKE(0x00509C20, get_hard_drive_font_directory, file);

	file_reference_create_from_path(file, k_hard_drive_font_directory, true);
}

