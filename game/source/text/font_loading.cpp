#include "text/font_loading.hpp"

#include "main/global_preferences.hpp"
#include "text/draw_string.hpp"

REFERENCE_DECLARE(0x022B7FAC, s_font_globals, g_font_globals);
REFERENCE_DECLARE(0x02457BB8, s_font_package_cache, g_font_package_cache);

REFERENCE_DECLARE(0x0189D358, char const* const, k_hard_drive_font_directory);
REFERENCE_DECLARE(0x0189D35C, char const* const, k_dvd_font_directory);
REFERENCE_DECLARE(0x0189D360, char const* const, k_font_package_base_name);
REFERENCE_DECLARE(0x0189D364, char const* const, k_font_package_suffix);

void __cdecl font_block_until_load_completes(s_font_loading_state* loading_state)
{
	INVOKE(0x00509140, font_block_until_load_completes, loading_state);
}

void __cdecl fonts_close_internal(s_font_loading_state* loading_state)
{
	INVOKE(0x005091A0, fonts_close_internal, loading_state);
}

void __cdecl font_dispose()
{
	INVOKE(0x00509210, font_dispose);
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
	return INVOKE(0x00509360, font_get_package_file_handle, out_file_handle);
}

s_font_package_file_header const* __cdecl font_get_package_header_internal()
{
	//return INVOKE(0x00509380, font_get_package_header_internal);

	return g_font_globals.font_package_header;
}

void __cdecl font_idle()
{
	INVOKE(0x00509390, font_idle);

	//if (g_font_globals.initialized && !g_font_globals.emergency_mode)
	//{
	//	font_loading_idle();
	//	font_cache_idle();
	//	font_package_cache_idle();
	//}
}

bool __cdecl font_in_emergency_mode()
{
	//return INVOKE(0x005093C0, font_in_emergency_mode);

	return g_font_globals.emergency_mode;
}

void __cdecl font_initialize()
{
	INVOKE(0x005093D0, font_initialize);

	//csmemset(&g_font_globals, 0, sizeof(g_font_globals));
	//g_font_globals.language = _language_invalid;
	//fonts_select_language();
	//fonts_copy_to_hard_drive();
	//
	//char font_package_filename[256]{};
	//get_font_master_filename(g_font_globals.language, font_package_filename, 256);
	//
	//font_load(&g_font_globals.loading_state, _font_index_none, font_package_filename, false);
	//font_package_cache_new();
	//font_cache_new();
	//g_font_globals.initialized = true;
}

void __cdecl font_initialize_emergency()
{
	INVOKE(0x00509420, font_initialize_emergency);

	//if (!g_font_globals.initialized)
	//{
	//	csmemset(&g_font_globals, 0, sizeof(g_font_globals));
	//	g_font_globals.language = _language_invalid;
	//	font_cache_new();
	//	font_package_cache_new();
	//	g_font_globals.initialized = true;
	//}
	//
	//c_font_cache_mt_safe font_cache{};
	//g_font_globals.emergency_mode = true;
}

//void font_load(struct s_font_loading_state* loading_state, e_font_index font_index, char const* filename, bool load_blocking)
void __cdecl font_load(s_font_loading_state* loading_state, long font_index, char const* filename, bool load_blocking)
{
	INVOKE(0x00509480, font_load, loading_state, font_index, filename, load_blocking);

	//ASSERT(!loading_state->started.peek());
	//ASSERT(!loading_state->finished.peek());
	//ASSERT(!loading_state->failed.peek());
	//
	//loading_state->filename.set(filename);
	//loading_state->font_index = font_index;
	//
	//char const* font_directory = g_font_globals.load_font_from_hard_drive ? k_hard_drive_font_directory : k_dvd_font_directory;
	//file_reference_create_from_path(&loading_state->font_file, font_directory, true);
	//file_reference_set_name(&loading_state->font_file, filename);
	//loading_state->started = true;
	//
	//byte async_task[220]{};
	//REFERENCE_DECLARE(async_task + 0, long, async_task_loading_state);
	//
	//loading_state->async_task = async_task_add(4 * load_blocking + 5, &async_task, 7, font_load_callback, &loading_state->finished);
	//
	//if (load_blocking)
	//	font_block_until_load_completes(loading_state);
}

//enum e_async_completion font_load_callback(void*)
long __cdecl font_load_callback(void* callback_data)
{
	return INVOKE(0x00509550, font_load_callback, callback_data);
}

void __cdecl font_loading_idle()
{
	INVOKE(0x005096F0, font_loading_idle);
}

void __cdecl font_reload()
{
	INVOKE(0x00509780, font_reload);
}

// specific to halo online
void __cdecl font_load_wrapper(bool load_blocking)
{
	//INVOKE(0x005099A0, font_load_wrapper, load_blocking);

	char font_package_filename[256]{};
	get_font_master_filename(g_font_globals.language, font_package_filename, 256);

	font_load(&g_font_globals.loading_state, _font_index_none, font_package_filename, false);
}

void __cdecl fonts_close()
{
	//INVOKE(0x00509A50, fonts_close);

	fonts_close_internal(&g_font_globals.loading_state);
	csmemset(&g_font_globals.loading_state, 0, sizeof(g_font_globals.loading_state));
	g_font_globals.font_package_header = nullptr;
}

void __cdecl fonts_copy_to_hard_drive()
{
	//INVOKE(0x00509A90, fonts_copy_to_hard_drive);

	g_font_globals.load_font_from_hard_drive = false;
}

void __cdecl font_invalidate_cached_fonts()
{
	//INVOKE(0x00509AA0, font_invalidate_cached_fonts);

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

void __cdecl get_font_master_filename(e_language language, char* buffer, long buffer_size)
{
	INVOKE(0x00509BB0, get_font_master_filename, language, buffer, buffer_size);

	//csstrnzcpy(buffer, k_font_package_base_name, buffer_size);
	//if (language)
	//{
	//	char const* suffix = get_language_suffix(language, true);
	//	if (*suffix)
	//	{
	//		csnzappendf(buffer, buffer_size, "_");
	//		csnzappendf(buffer, buffer_size, suffix);
	//	}
	//}
	//
	//csnzappendf(buffer, buffer_size, k_font_package_suffix);
}

