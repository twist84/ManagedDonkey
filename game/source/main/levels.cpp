#include "main/levels.hpp"

#include "cache/cache_files.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "networking/tools/network_blf.hpp"
#include "tag_files/files_windows.hpp"
#include "text/unicode.hpp"

HOOK_DECLARE(0x0054A2A0, levels_add_campaign);
HOOK_DECLARE(0x0054A4E0, levels_add_map_from_scripting);
HOOK_DECLARE(0x0054A6A0, levels_add_fake_map_from_scripting);
HOOK_DECLARE(0x0054A6C0, levels_add_level);
HOOK_DECLARE(0x0054AD80, levels_begin_dvd_enumeration);
//HOOK_DECLARE(0x0054ADF0, levels_dispose);
//HOOK_DECLARE(0x0054AEA0, levels_dispose_from_old_map);
HOOK_DECLARE(0x0054AEC0, levels_dvd_enumeration_callback2);
HOOK_DECLARE(0x0054B250, levels_dvd_enumeration_callback);
//HOOK_DECLARE(0x0054B610, levels_get_available_map_mask);
//HOOK_DECLARE(0x0054B670, levels_get_campaign_count);
//HOOK_DECLARE(0x0054B6A0, levels_get_campaign_id_from_path);
//HOOK_DECLARE(0x0054B7B0, levels_get_campaign_level_count);
//HOOK_DECLARE(0x0054B8A0, levels_get_campaign_level_index);
//HOOK_DECLARE(0x0054B9D0, levels_get_campaign_map_by_display_name);
//HOOK_DECLARE(0x0054BAA0, levels_get_campaign_map_ids);
//HOOK_DECLARE(0x0054BB80, levels_get_campaign_next_map_id);
//HOOK_DECLARE(0x0054BCA0, levels_get_default_multiplayer_map_id);
//HOOK_DECLARE(0x0054BDB0, levels_get_checksum);
//HOOK_DECLARE(0x0054BE10, levels_get_multiplayer_map_by_display_name);
//HOOK_DECLARE(0x0054BEE0, levels_get_multiplayer_map_ids);
//HOOK_DECLARE(0x0054C040, levels_get_path);
//HOOK_DECLARE(0x0054C110, levels_initialize);
//HOOK_DECLARE(0x0054C2D0, levels_initialize_for_new_map);
HOOK_DECLARE(0x0054C320, levels_map_id_is_fake);
//HOOK_DECLARE(0x0054C330, levels_open_dlc);
//HOOK_DECLARE(0x0054C360, levels_path_is_dlc);
HOOK_DECLARE(0x0054C3C0, levels_process_campaign_configuration_file);
HOOK_DECLARE(0x0054C530, levels_process_level_configuration_file);
//HOOK_DECLARE(0x0054C820, levels_delete);
//HOOK_DECLARE(0x0054C910, levels_try_and_get_by_map_id);
//HOOK_DECLARE(0x0054C9E0, levels_try_and_get_campaign_insertion);
//HOOK_DECLARE(0x0054CAB0, levels_try_and_get_campaign_map);
//HOOK_DECLARE(0x0054CAD0, levels_try_and_get_main_menu_map);
//HOOK_DECLARE(0x0054CB00, levels_try_and_get_multiplayer_map);
//HOOK_DECLARE(0x0054CB20, levels_update);

void __cdecl levels_add_campaign(s_blf_chunk_campaign const* campaign, bool byte_swap, wchar_t const* maps_path, bool is_dlc)
{
	FUNCTION_BEGIN(true);

	HOOK_INVOKE(, levels_add_campaign, campaign, byte_swap, maps_path, is_dlc);
}

void __cdecl levels_add_map_from_scripting(long map_id, char const* scenario_path)
{
	FUNCTION_BEGIN(true);

	HOOK_INVOKE(, levels_add_map_from_scripting, map_id, scenario_path);
}

void __cdecl levels_add_fake_map_from_scripting(char const* scenario_path)
{
	FUNCTION_BEGIN(true);

	//HOOK_INVOKE(, levels_add_fake_map_from_scripting, scenario_path);

	levels_add_map_from_scripting(-2, scenario_path);
}

void __cdecl levels_add_level(s_blf_chunk_scenario const* scenario, bool byte_swap, wchar_t const* maps_path, bool is_dlc)
{
	FUNCTION_BEGIN(true);

	HOOK_INVOKE(, levels_add_level, scenario, byte_swap, maps_path, is_dlc);
}

bool __cdecl levels_begin_dvd_enumeration()
{
	FUNCTION_BEGIN(true);

	bool result = false;
	HOOK_INVOKE(result =, levels_begin_dvd_enumeration);
	return result;
}

// searches for `campaign`, `mapinfo`, `xex` and `preorder_unlock.txt`
long __cdecl levels_dvd_enumeration_callback2(void* userdata)
{
	FUNCTION_BEGIN(true);

	long result = 0;
	HOOK_INVOKE(result =, levels_dvd_enumeration_callback2, userdata);

	return result;
}

// searches for `map`
long __cdecl levels_dvd_enumeration_callback(s_levels_dvd_enumeration_callback_data* userdata)
{
	FUNCTION_BEGIN(true);

	c_static_string<256> found_file{};
	static s_file_reference map_directory_file{};

	c_static_wchar_string<256> maps_path{};
	c_static_wchar_string<256> extension{};

	s_file_reference file{};
	s_file_last_modification_date date{};

	if (userdata->enumeration_index)
	{
		if (userdata->enumeration_index == 1)
		{
			if (!find_files_next(userdata->find_file_data, &file, &date))
			{
				find_files_end(userdata->find_file_data);
				return ++userdata->enumeration_index == 2;
			}

			file_reference_get_name(&file, 12, &maps_path, 256);
			found_file.append_print("%ls", maps_path.get_string());
			file_reference_get_name(&file, 1, &maps_path, 256);
			file_reference_get_name(&file, 8, &extension, 256);

			if (ustricmp(extension.get_string(), L"campaign"))
			{
				if (!ustricmp(extension.get_string(), L"mapinfo"))
				{
					levels_process_level_configuration_file(&file, maps_path.get_string(), 0);
				}
			}
			else
			{
				levels_process_campaign_configuration_file(&file, maps_path.get_string(), 0);
			}
		}
	}
	else
	{
		found_file.append_print("%sinfo", cache_files_map_directory());
		file_reference_create_from_path(&map_directory_file, found_file.get_string(), true);
		find_files_start(userdata->find_file_data, 0, &map_directory_file);

		++userdata->enumeration_index;
	}

	return userdata->enumeration_index == 2;
}

//bool __cdecl levels_map_id_is_fake(e_map_id map_id)
bool __cdecl levels_map_id_is_fake(long map_id)
{
	FUNCTION_BEGIN(true);

	//bool result = false;
	//HOOK_INVOKE(result =, levels_map_id_is_fake, map_id);
	//return result;

	return map_id == -2;
}

void __cdecl levels_process_campaign_configuration_file(s_file_reference* file, wchar_t const* maps_path, bool is_dlc)
{
	FUNCTION_BEGIN(true);

	//HOOK_INVOKE(, levels_process_campaign_configuration_file, file, maps_path, is_dlc);

	static char file_buffer[sizeof(s_blf_chunk_campaign)]{};
	s_blf_chunk_campaign const* campaign = nullptr;
	bool byte_swap = false;

	levels_find_campaign_chunk(file, file_buffer, &campaign, &byte_swap);
	if (campaign)
		levels_add_campaign(campaign, byte_swap, maps_path, is_dlc);
}

void __cdecl levels_process_level_configuration_file(s_file_reference* file, wchar_t const* maps_path, bool is_dlc)
{
	FUNCTION_BEGIN(true);

	//HOOK_INVOKE(, levels_process_level_configuration_file, file, maps_path, is_dlc);

	static char file_buffer[sizeof(s_blf_chunk_scenario)]{};
	s_blf_chunk_scenario const* level = nullptr;
	bool byte_swap = false;

	levels_find_scenario_chunk(file, file_buffer, &level, &byte_swap);
	if (level)
		levels_add_level(level, byte_swap, maps_path, is_dlc);
}

void levels_find_campaign_chunk(s_file_reference* file, char* const file_buffer, s_blf_chunk_campaign const** out_campaign, bool* must_byte_swap)
{
	*out_campaign = nullptr;
	*must_byte_swap = false;

	bool file_added = false;
	dword error = 0;
	dword file_size = 0;
	long chunk_size = 0;
	char const* chunk_buffer = nullptr;
	bool eof_chunk = false;
	s_blf_chunk_campaign const* campaign = nullptr;
	bool byte_swap = false;

	if (!file_open(file, 1, &error))
	{
		c_console::write_line("levels: failed to open campaign info file");
		file_close(file);
		goto function_end;
	}

	if (!file_get_size(file, &file_size))
	{
		c_console::write_line("levels: failed to get file size of campaign info file");
		goto function_finish;
	}

	if (file_size > 0x18AC)
	{
		c_console::write_line("levels: unexpected file size for campaign info file");
	}
	else
	{
		if (!file_read(file, file_size, 0, file_buffer))
		{
			c_console::write_line("levels: failed to read campaign info file");
		}
		else
		{
			if (!network_blf_verify_start_of_file(file_buffer, file_size, &byte_swap, &chunk_size))
			{
				c_console::write_line("levels: failed to verify blf start of file for campaign info file");
				goto function_finish;
			}

			if (!network_blf_find_chunk(file_buffer, file_size, byte_swap, 'cmpn', 1, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
			{
				c_console::write_line("levels: failed to find blf campaign chunk");
				goto function_finish;
			}

			if (chunk_buffer)
			{
				campaign = reinterpret_cast<s_blf_chunk_campaign const*>(chunk_buffer - 0xC);
				if (chunk_buffer != (char const*)0xC &&
					network_blf_find_chunk(file_buffer, file_size, byte_swap, '_eof', 1, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
				{
					if (chunk_buffer && chunk_size == sizeof(s_blf_chunk_end_of_file_with_rsa) &&
						network_blf_verify_end_of_file(file_buffer, file_size, byte_swap, chunk_buffer - 0xC, _blf_file_authentication_type_rsa))
					{
						file_added = true;
						goto function_finish;
					}

					c_console::write_line("levels: failed to verify blf end of file chunk");
				}
			}
		}
	}

function_finish:
	if (file_added)
	{
		*out_campaign = campaign;
		*must_byte_swap = byte_swap;
		file_close(file);
	}
function_end:

	if (!file_added)
	{
		// #TODO: file_reference_get_name
		c_console::write_line("levels: failed to add campaign file '%s'", file->path);
	}
}

void levels_find_scenario_chunk(s_file_reference* file, char* const file_buffer, s_blf_chunk_scenario const** out_scenario, bool* must_byte_swap)
{
	*out_scenario = nullptr;
	*must_byte_swap = false;

	bool file_added = false;
	dword error = 0;
	dword file_size = 0;
	long chunk_size = 0;
	char const* chunk_buffer = nullptr;
	bool eof_chunk = false;
	s_blf_chunk_scenario const* scenario = nullptr;
	bool byte_swap = false;

	if (!file_open(file, 1, &error))
	{
		c_console::write_line("levels: failed to open level info file");
		file_close(file);
		goto function_end;
	}

	if (!file_get_size(file, &file_size))
	{
		c_console::write_line("levels: failed to get file size of level info file");
		goto function_finish;
	}

	if (file_size > 0xCDD9)
	{
		c_console::write_line("levels: unexpected file size for level info file");
	}
	else
	{
		if (!file_read(file, file_size, 0, file_buffer))
		{
			c_console::write_line("levels: failed to read level info file");
		}
		else
		{
			if (!network_blf_verify_start_of_file(file_buffer, file_size, &byte_swap, &chunk_size))
			{
				c_console::write_line("levels: failed to verify blf start of file");
				goto function_finish;
			}

			if (!network_blf_find_chunk(file_buffer, file_size, byte_swap, 'levl', 3, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
			{
				c_console::write_line("levels: failed to find blf scenario chunk");
				goto function_finish;
			}

			if (chunk_buffer)
			{
				scenario = reinterpret_cast<s_blf_chunk_scenario const*>(chunk_buffer - 0xC);
				if (chunk_buffer != (char const*)0xC &&
					network_blf_find_chunk(file_buffer, file_size, byte_swap, '_eof', 1, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
				{
					if (chunk_buffer && chunk_size == sizeof(s_blf_chunk_end_of_file_with_rsa) &&
						network_blf_verify_end_of_file(file_buffer, file_size, byte_swap, chunk_buffer - 0xC, _blf_file_authentication_type_rsa))
					{
						file_added = true;
						goto function_finish;
					}

					c_console::write_line("levels: failed to verify blf end of file chunk");
				}
			}
		}
	}

function_finish:
	if (file_added)
	{
		*out_scenario = scenario;
		*must_byte_swap = byte_swap;
		file_close(file);
	}
function_end:

	if (!file_added)
	{
		// #TODO: file_reference_get_name
		c_console::write_line("levels: failed to add level file '%s'", file->path);
	}
}