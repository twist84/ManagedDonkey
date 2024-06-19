#include "text/font_loading.hpp"

#include "cseries/async.hpp"
#include "cseries/cseries_events.hpp"
#include "main/global_preferences.hpp"
#include "memory/module.hpp"
#include "text/draw_string.hpp"
#include "text/font_group.hpp"
#include "text/font_package_cache.hpp"

HOOK_DECLARE(0x00509140, font_block_until_load_completes);
HOOK_DECLARE(0x005091A0, font_close_loaded_file);
HOOK_DECLARE(0x00509210, font_dispose);
HOOK_DECLARE(0x00509280, font_get_debug_name);
HOOK_DECLARE(0x005092C0, font_get_font_index);
HOOK_DECLARE(0x005092F0, font_get_header);
HOOK_DECLARE(0x00509330, font_get_loaded_header);
HOOK_DECLARE(0x00509360, font_get_package_file_handle);
HOOK_DECLARE(0x00509380, font_get_package_header_internal);
HOOK_DECLARE(0x00509390, font_idle);
HOOK_DECLARE(0x005093C0, font_in_emergency_mode);
HOOK_DECLARE(0x005093D0, font_initialize);
HOOK_DECLARE(0x00509420, font_initialize_emergency);
HOOK_DECLARE(0x00509480, font_load);
HOOK_DECLARE(0x00509550, font_load_callback);
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

REFERENCE_DECLARE(0x0189D358, char const*, k_hard_drive_font_directory);
REFERENCE_DECLARE(0x0189D35C, char const*, k_dvd_font_directory);
REFERENCE_DECLARE(0x0189D360, char const* const, k_font_package_base_name);
REFERENCE_DECLARE(0x0189D364, char const* const, k_font_package_suffix);

void __cdecl font_block_until_load_completes(s_font_loading_state* loading_state)
{
	//INVOKE(0x00509140, font_block_until_load_completes, loading_state);

	if (loading_state->started.peek() && !loading_state->finished.peek())
	{
		async_task_change_priority(loading_state->async_task, _async_priority_blocking_generic);
		internal_async_yield_until_done_attributed(&loading_state->finished, false, false, (e_yield_reason)4, __FILE__, __LINE__);
	}
}

void __cdecl font_close_loaded_file(s_font_loading_state* loading_state)
{
	//INVOKE(0x005091A0, font_close_loaded_file, loading_state);

	font_block_until_load_completes(loading_state);

	if (loading_state->font_file_loaded)
		file_close(&loading_state->font_file);
}

void __cdecl font_dispose()
{
	//INVOKE(0x00509210, font_dispose);

	if (g_font_globals.initialized)
	{
		font_close_loaded_file(&g_font_globals.loading_state);
		csmemset(&g_font_globals.loading_state, 0, sizeof(g_font_globals.loading_state));
		g_font_globals.font_package_header = NULL;
		font_cache_delete();
		font_package_cache_delete();
	}
	csmemset(&g_font_globals, 0, sizeof(g_font_globals));
}

//char const* __cdecl font_get_debug_name(e_font_index font_index);
char const* __cdecl font_get_debug_name(long font_index)
{
	//return INVOKE(0x00509280, font_get_debug_name, font_index);

	if (g_font_globals.font_package_header && font_index >= 0 && font_index < g_font_globals.font_package_header->font_count)
	{
		dword header_offset = g_font_globals.font_package_header->fonts[font_index].offset;
		s_font_header* header = reinterpret_cast<s_font_header*>((char*)g_font_globals.font_package_header + header_offset);
		if (header)
			return header->name.get_string();
	}

	return nullptr;
}

//e_font_index font_get_font_index(e_font_id font_id);
long __cdecl font_get_font_index(long font_id)
{
	//return INVOKE(0x005092C0, font_get_font_index, font_id);

	if (g_font_globals.font_package_header && font_id >= 0 && font_id < 16)
		return g_font_globals.font_package_header->font_index_mapping[font_id];

	return _font_index_none;
}

//s_font_header const* font_get_header(e_font_id font_id)
s_font_header const* __cdecl font_get_header(long font_id)
{
	//return INVOKE(0x005092F0, font_get_header, font_id);

	long font_index = _font_index_none;
	if (!g_font_globals.font_package_header)
		return nullptr;

	if (font_id >= 0 && font_id < 16)
		font_index = g_font_globals.font_package_header->font_index_mapping[font_id];

	if (g_font_globals.font_package_header && font_index >= 0 && font_index < g_font_globals.font_package_header->font_count)
	{
		dword header_offset = g_font_globals.font_package_header->fonts[font_index].offset;
		return reinterpret_cast<s_font_header*>((char*)g_font_globals.font_package_header + header_offset);
	}

	return nullptr;
}

s_font_header const* __cdecl font_get_loaded_header(long font_index)
{
	//return INVOKE(0x00509330, font_get_loaded_header, font_index);

	if (g_font_globals.font_package_header && font_index >= 0 && font_index < g_font_globals.font_package_header->font_count)
	{
		dword header_offset = g_font_globals.font_package_header->fonts[font_index].offset;
		return reinterpret_cast<s_font_header*>((char*)g_font_globals.font_package_header + header_offset);
	}

	return nullptr;
}

bool __cdecl font_get_package_file_handle(s_file_handle* out_file_handle)
{
	//return INVOKE(0x00509360, font_get_package_file_handle, out_file_handle);

	return g_font_globals.loading_state.font_file_loaded && file_reference_get_file_handle(&g_font_globals.loading_state.font_file, out_file_handle);
}

s_font_package_file_header const* __cdecl font_get_package_header_internal()
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
		font_cache_new();
		font_package_cache_new();
		g_font_globals.initialized = true;
	}
	
	c_font_cache_mt_safe font_cache{};
	g_font_globals.emergency_mode = true;
}

//void font_load(struct s_font_loading_state* loading_state, e_font_index font_index, char const* filename, bool load_blocking)
void __cdecl font_load(s_font_loading_state* loading_state, long font_index, char const* filename, bool load_blocking)
{
	//INVOKE(0x00509480, font_load, loading_state, font_index, filename, load_blocking);

	e_async_priority priority = load_blocking ? _async_priority_blocking_generic : _async_priority_important_non_blocking;

	ASSERT(!loading_state->started);
	ASSERT(!loading_state->finished);
	ASSERT(!loading_state->failed);
	
	loading_state->filename.set(filename);
	loading_state->font_index = font_index;
	
	get_active_font_directory(&loading_state->font_file);
	file_reference_set_name(&loading_state->font_file, filename);
	loading_state->started = true;

	s_async_task task{};
	task.font_loading_task.loading_state = loading_state;
	
	loading_state->async_task = async_task_add(priority, &task, _async_category_text, font_load_callback, &loading_state->finished);
	
	if (load_blocking)
		font_block_until_load_completes(loading_state);
}

e_async_completion __cdecl font_load_callback(s_async_task* task)
{
	//return INVOKE(0x00509550, font_load_callback, task);

	s_font_loading_state* loading_state = task->font_loading_task.loading_state;
	e_async_completion completion = _async_completion_unknown0;

	dword error = 0;
	if (loading_state->font_file_loaded)
	{
		bool loaded_font_file = false;
		if (file_read(&loading_state->font_file, sizeof(s_font_package_file), false, &g_font_globals.font_package))
		{
			s_font_package_file_header* package_header = &g_font_globals.font_package.header;

			//font_package_file_header_byteswap(package_header);
			if (font_package_file_header_validate(package_header))
			{
				bool load_package_file_failed = false;
				for (long font_index = 0; font_index < package_header->font_count && !load_package_file_failed; ++font_index)
				{
					dword offset = package_header->fonts[font_index].offset;
					if (offset < sizeof(s_font_package_file_header) &&
						offset + sizeof(s_font_header) > package_header->package_file_font_offset + package_header->package_file_font_size)
					{
						generate_event(_event_level_error, "package file font #%d header offset %d (size %d) not valid in [%d, %d+%d]",
							font_index,
							offset,
							package_header->package_file_font_offset,
							g_font_globals.font_package.header.package_file_font_offset,
							g_font_globals.font_package.header.package_file_font_offset,
							g_font_globals.font_package.header.package_file_font_size);

						load_package_file_failed = true;
					}
					else
					{
						s_font_header* header = (s_font_header*)offset_pointer(package_header, package_header->fonts[font_index].offset);
						//font_header_byteswap(header);
						if (font_header_validate(header))
						{
							if (header->kerning_pair_count > 0)
							{
								s_kerning_pair* kerning_pair = (s_kerning_pair*)offset_pointer(header, header->kerning_pairs_offset);
								//font_kerning_pairs_byteswap(kerning_pair, header->kerning_pair_count);
							}
						}
						else
						{
							generate_event(_event_level_error, "package file font #%d header failed to validate",
								font_index);

							load_package_file_failed = true;
						}
					}
				}

				s_font_package_entry* entry = (s_font_package_entry*)offset_pointer(package_header, package_header->first_package_entry.first_character_key);
				//font_package_entries_byteswap(entry, package_header->first_package_entry.last_character_key);

				if (!load_package_file_failed)
				{
					g_font_globals.font_package_header = package_header;
					generate_event(_event_level_message, "loaded package file '%s' (%d fonts)",
						loading_state->filename.get_string(),
						package_header->font_count);

					loaded_font_file = true;
				}
			}
			else
			{
				generate_event(_event_level_error, "package header failed to validate");
			}

			loaded_font_file = true;
		}
		else
		{
			generate_event(_event_level_error, "fonts: couldn't read package file header (%d bytes)",
				sizeof(s_font_package_file));
		}

		if (!loaded_font_file)
		{
			generate_event(_event_level_critical, "fonts: unable to load font file '%s', fonts may be unavailable",
				loading_state->filename.get_string());

			loading_state->failed = 1;
		}

		completion = _async_completion_done;
	}
	else if (file_open(&loading_state->font_file, FLAG(_file_open_flag_desired_access_read), &error))
	{
		loading_state->font_file_loaded = 1;
	}
	else
	{
		generate_event(_event_level_error, "fonts: couldn't open font file '%s'", loading_state->filename.get_string());
		loading_state->failed = 1;
		completion = _async_completion_done;
	}

	return completion;
}

//.text:005096B0 ; 

void __cdecl font_loading_idle()
{
	//INVOKE(0x005096F0, font_loading_idle);

	if (g_font_globals.loading_state.finished.peek() && g_font_globals.loading_state.failed.peek() ||
		g_font_globals.permanently_unavailable)
	{
		if (g_font_globals.load_font_from_hard_drive)
		{
			fonts_invalidate_cached_fonts();
			g_font_globals.load_font_from_hard_drive = false;
		}

		if (g_font_globals.reload_retry_count < 3)
		{
			g_font_globals.reload_retry_count++;
			font_reload();
		}
		else
		{
			if (!g_font_globals.permanently_unavailable)
				g_font_globals.permanently_unavailable = true;

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

long __cdecl font_table_get_font_file_references(char const* text, s_file_reference const* directory, s_file_reference* references, long max_references, long* font_id_mapping, long max_font_ids)
{
	return INVOKE(0x00509840, font_table_get_font_file_references, text, directory, references, max_references, font_id_mapping, max_font_ids);
}

bool __cdecl fonts_begin_loading(bool load_blocking)
{
	//INVOKE(0x005099A0, fonts_begin_loading, load_blocking);

	char font_package_filename[256]{};
	get_font_master_filename(g_font_globals.language, font_package_filename, sizeof(font_package_filename));
	font_load(&g_font_globals.loading_state, _font_index_none, font_package_filename, load_blocking);

	return true;
}

void __cdecl fonts_close()
{
	//INVOKE(0x00509A50, fonts_close);

	font_close_loaded_file(&g_font_globals.loading_state);
	csmemset(&g_font_globals.loading_state, 0, sizeof(g_font_globals.loading_state));
	g_font_globals.font_package_header = nullptr;
}

void __cdecl fonts_copy_to_hard_drive()
{
	//INVOKE(0x00509A90, fonts_copy_to_hard_drive);

	g_font_globals.load_font_from_hard_drive = false;
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

	if (g_font_globals.load_font_from_hard_drive)
		get_dvd_font_directory(file);
	else
		get_hard_drive_font_directory(file);
}

void __cdecl get_dvd_font_directory(s_file_reference* file)
{
	//INVOKE(0x00509B90, get_dvd_font_directory, file);

	file_reference_create_from_path(file, k_dvd_font_directory, true);
}

void __cdecl get_font_master_filename(e_language language, char* buffer, long buffer_size)
{
	//INVOKE(0x00509BB0, get_font_master_filename, language, buffer, buffer_size);

	csstrnzcpy(buffer, k_font_package_base_name, buffer_size);
	if (language)
	{
		char const* suffix = get_language_suffix(language, true);
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

