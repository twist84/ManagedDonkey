#include "main/levels.hpp"

#include "cache/cache_files.hpp"
#include "cseries/async.hpp"
#include "cseries/cseries.hpp"
#include "cseries/runtime_state.hpp"
#include "interface/user_interface.hpp"
#include "main/main.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "networking/tools/network_blf.hpp"
#include "tag_files/files_windows.hpp"
#include "text/unicode.hpp"

#include <string.h>

REFERENCE_DECLARE(0x0189E2D4, s_level_globals, g_level_globals);

HOOK_DECLARE(0x0054A2A0, levels_add_campaign);
HOOK_DECLARE(0x0054A4E0, levels_add_map_from_scripting);
HOOK_DECLARE(0x0054A6A0, levels_add_fake_map_from_scripting);
HOOK_DECLARE(0x0054A6C0, levels_add_level);
HOOK_DECLARE(0x0054AD80, levels_begin_dvd_enumeration);
HOOK_DECLARE(0x0054ADF0, levels_dispose);
//HOOK_DECLARE(0x0054AEA0, levels_dispose_from_old_map);
HOOK_DECLARE(0x0054AEC0, levels_dvd_enumeration_callback2);
HOOK_DECLARE(0x0054B250, levels_dvd_enumeration_callback);
//HOOK_DECLARE(0x0054B610, levels_get_available_map_mask);
HOOK_DECLARE(0x0054B670, levels_get_campaign_count);
//HOOK_DECLARE(0x0054B6A0, levels_get_campaign_id_from_path);
//HOOK_DECLARE(0x0054B7B0, levels_get_campaign_level_count);
//HOOK_DECLARE(0x0054B8A0, levels_get_campaign_level_index);
//HOOK_DECLARE(0x0054B9D0, levels_get_campaign_map_by_display_name);
//HOOK_DECLARE(0x0054BAA0, levels_get_campaign_map_ids);
//HOOK_DECLARE(0x0054BB80, levels_get_campaign_next_map_id);
HOOK_DECLARE(0x0054B3A0, levels_enumeration_in_progress);
HOOK_DECLARE(0x0054BCA0, levels_get_default_multiplayer_map_id);
HOOK_DECLARE(0x0054BDB0, levels_get_checksum);
HOOK_DECLARE(0x0054BE10, levels_get_multiplayer_map_by_display_name);
HOOK_DECLARE(0x0054BEE0, levels_get_multiplayer_map_ids);
HOOK_DECLARE(0x0054BFD0, levels_get_multiplayer_map_is_allowed);
HOOK_DECLARE(0x0054C040, levels_get_path);
HOOK_DECLARE(0x0054C110, levels_initialize);
//HOOK_DECLARE(0x0054C2D0, levels_initialize_for_new_map);
HOOK_DECLARE(0x0054C320, levels_map_id_is_fake);
//HOOK_DECLARE(0x0054C330, levels_open_dlc);
//HOOK_DECLARE(0x0054C360, levels_path_is_dlc);
HOOK_DECLARE(0x0054C3C0, levels_process_campaign_configuration_file);
HOOK_DECLARE(0x0054C530, levels_process_level_configuration_file);
HOOK_DECLARE(0x0054C820, levels_delete);
HOOK_DECLARE(0x0054C910, levels_try_and_get_by_map_id);
HOOK_DECLARE(0x0054C9E0, levels_try_and_get_campaign_insertion);
HOOK_DECLARE(0x0054CAB0, levels_try_and_get_campaign_map);
HOOK_DECLARE(0x0054CAD0, levels_try_and_get_main_menu_map);
HOOK_DECLARE(0x0054CB00, levels_try_and_get_multiplayer_map);
//HOOK_DECLARE(0x0054CB20, levels_update);

void __cdecl levels_add_campaign(s_blf_chunk_campaign const* campaign, bool byte_swap, wchar_t const* maps_path, bool is_dlc)
{
	HOOK_INVOKE(, levels_add_campaign, campaign, byte_swap, maps_path, is_dlc);
}

void __cdecl levels_add_map_from_scripting(long map_id, char const* scenario_path)
{
	//HOOK_INVOKE(, levels_add_map_from_scripting, map_id, scenario_path);

	if (g_level_globals.__unknownA14)
	{
		if (!levels_enumeration_in_progress())
			levels_begin_dvd_enumeration();

		while (levels_enumeration_in_progress())
			main_loop_pregame();
	}

	c_critical_section_scope critical_section_scope(_critical_section_levels);

	// called but never used, comment out for now
	//levels_try_and_get_by_map_id(g_level_globals.campaign_levels, map_id, &campaign_level);

	long campaign_level_index = datum_new(*g_level_globals.campaign_levels);
	if (campaign_level_index != NONE)
	{
		s_level_datum* level = (s_level_datum*)datum_try_and_get(*g_level_globals.campaign_levels, campaign_level_index);

		// 0x2C
		level->flags.set(_level_bit2, true);
		level->flags.set(_level_bit3, true);
		level->flags.set(_level_solo_bit, true);

		level->map_id = map_id;
		usnzprintf(level->name, NUMBEROF(level->name), L"%S", tag_name_strip_path(scenario_path));
		ustrnzcpy(level->description, L"This is a fake level!", NUMBEROF(level->description));

		level->presence_context_id = -1;
		level->sort_order = -1;

		level->multiplayer_minimum_desired_players = char(0xFF);
		level->multiplayer_maximum_desired_players = char(0xFF);

		csnzprintf(level->scenario_path, NUMBEROF(level->scenario_path), "%s%s", cache_files_map_directory(), scenario_path);

		c_data_iterator<s_campaign_datum> campaign_iterator;
		campaign_iterator.begin(*g_level_globals.campaigns);
		if (campaign_iterator.next())
		{
			s_campaign_datum* campaign = campaign_iterator.get_datum();

			while (campaign->campaign_id != 1)
			{
				if (!campaign_iterator.next())
					return;
			}

			long map_index = 0;
			long* map_ids = campaign->map_ids;
			while (*map_ids && *map_ids != NONE)
			{
				map_index++;
				map_ids++;
				if (map_index >= NUMBEROF(campaign->map_ids))
					return;
			}
			campaign->map_ids[map_index] = map_id;
		}
	}
}

void __cdecl levels_add_fake_map_from_scripting(char const* scenario_path)
{
	//HOOK_INVOKE(, levels_add_fake_map_from_scripting, scenario_path);

	levels_add_map_from_scripting(-2, scenario_path);
}

void __cdecl levels_add_multiplayer_map_from_scripting(long map_id, char const* scenario_path)
{
	if (g_level_globals.__unknownA14)
	{
		if (!levels_enumeration_in_progress())
			levels_begin_dvd_enumeration();

		while (levels_enumeration_in_progress())
			main_loop_pregame();
	}

	c_critical_section_scope critical_section_scope(_critical_section_levels);

	// called but never used, comment out for now
	//levels_try_and_get_by_map_id(g_level_globals.multiplayer_levels, map_id, &multiplayer_level);

	long multiplayer_level_index = datum_new(*g_level_globals.multiplayer_levels);
	if (multiplayer_level_index != NONE)
	{
		s_level_datum* level = (s_level_datum*)datum_try_and_get(*g_level_globals.multiplayer_levels, multiplayer_level_index);

		// 0x4C
		level->flags.set(_level_bit2, true);
		level->flags.set(_level_bit3, true);
		level->flags.set(_level_multi_bit, true);

		level->map_id = map_id;
		usnzprintf(level->name, NUMBEROF(level->name), L"%S", tag_name_strip_path(scenario_path));
		ustrnzcpy(level->description, L"This is a fake level!", NUMBEROF(level->description));

		level->presence_context_id = -1;
		level->sort_order = 0;

		level->multiplayer_minimum_desired_players = 16;
		level->multiplayer_maximum_desired_players = 16;
		
		level->engine_maximum_teams[_game_engine_type_none]        = 0;
		level->engine_maximum_teams[_game_engine_type_ctf]         = 2;
		level->engine_maximum_teams[_game_engine_type_slayer]      = 8;
		level->engine_maximum_teams[_game_engine_type_oddball]     = 8;
		level->engine_maximum_teams[_game_engine_type_king]        = 8;
		level->engine_maximum_teams[_game_engine_type_sandbox]     = 8;
		level->engine_maximum_teams[_game_engine_type_vip]         = 8;
		level->engine_maximum_teams[_game_engine_type_juggernaut]  = 8;
		level->engine_maximum_teams[_game_engine_type_territories] = 4;
		level->engine_maximum_teams[_game_engine_type_assault]     = 2;
		level->engine_maximum_teams[_game_engine_type_infection]   = 8;

		csnzprintf(level->scenario_path, NUMBEROF(level->scenario_path), "%s%s", cache_files_map_directory(), scenario_path);
	}
}

void __cdecl levels_add_fake_multiplayer_map_from_scripting(char const* scenario_path)
{
	levels_add_multiplayer_map_from_scripting(-2, scenario_path);
}

void __cdecl levels_add_level(s_blf_chunk_scenario const* scenario, bool byte_swap, wchar_t const* dlc_path, bool is_dlc)
{
	//HOOK_INVOKE(, levels_add_level, scenario, byte_swap, maps_path, is_dlc);

	c_critical_section_scope critical_section_scope(_critical_section_levels);

	e_language language = get_current_language();

	s_blf_header scenario_header = scenario->header;
	if (byte_swap)
	{
		bswap_dword_inplace(scenario_header.chunk_type);
		bswap_dword_inplace(scenario_header.chunk_size);
		bswap_word_inplace(scenario_header.major_version);
		bswap_word_inplace(scenario_header.minor_version);
	}

	decltype(scenario->type_flags) type_flags = scenario->type_flags;
	if (byte_swap)
		type_flags.set_unsafe(bswap_dword(type_flags));

	bool type_is_bit0  = type_flags.test(_scenario_type_flag_bit0);
	bool type_is_bit1  = type_flags.test(_scenario_type_flag_bit1);
	bool type_is_bit2  = type_flags.test(_scenario_type_flag_bit2);
	bool type_is_bit3  = type_flags.test(_scenario_type_flag_bit3);
	bool type_is_ui    = type_flags.test(_scenario_type_flag_ui_bit);
	bool type_is_solo  = type_flags.test(_scenario_type_flag_solo_bit);
	bool type_is_multi = type_flags.test(_scenario_type_flag_multi_bit);
	bool type_is_dlc   = type_flags.test(_scenario_type_flag_dlc_bit);
	bool type_is_test  = type_flags.test(_scenario_type_flag_test_bit);
	bool type_is_temp  = type_flags.test(_scenario_type_flag_temp_bit);

	s_level_datum* level = NULL;

	if (type_is_ui)
	{
		level = &g_level_globals.mainmenu_level;
	}
	else if (type_is_solo || type_is_multi)
	{
		s_data_array* levels_data = type_is_solo ? *g_level_globals.campaign_levels : *g_level_globals.multiplayer_levels;
		long level_index = datum_new(levels_data);
		if (level_index != NONE)
			level = (s_level_datum*)datum_get(levels_data, level_index);
	}

	if (level != NULL)
	{
		level->flags.clear();
		level->flags.set(_level_bit0, is_dlc);
		level->flags.set(_level_bit1, type_is_bit1);
		level->flags.set(_level_bit2, type_is_bit2);
		level->flags.set(_level_ui_bit, type_is_ui);
		level->flags.set(_level_solo_bit, type_is_solo);
		level->flags.set(_level_multi_bit, type_is_multi);
		level->flags.set(_level_dlc_bit, type_is_dlc);
		level->flags.set(_level_test_bit, type_is_test);
		level->flags.set(_level_temp_bit, type_is_temp);

		level->map_id = scenario->map_id;

		wcsncpy_s(level->name, scenario->names[language].get_string(), NUMBEROF(level->name));
		wcsncpy_s(level->description, scenario->descriptions[language].get_string(), NUMBEROF(level->description));

		level->presence_context_id = scenario->precense_context_id;
		level->sort_order = scenario->sort_order;

		level->multiplayer_minimum_desired_players = scenario->multiplayer_minimum_desired_players;
		level->multiplayer_maximum_desired_players = scenario->multiplayer_maximum_desired_players;

		for (long engine_index = 0; engine_index < k_game_engine_type_count; engine_index++)
			level->engine_maximum_teams[engine_index] = scenario->engine_maximum_teams[engine_index];

		if (byte_swap)
		{
			bswap_dword_inplace(level->map_id);

			unicode_byte_swap_wchar_string(level->name, NUMBEROF(level->name), byte_swap_get_runtime_byte_order() != 1);
			unicode_byte_swap_wchar_string(level->description, NUMBEROF(level->description), byte_swap_get_runtime_byte_order() != 1);

			bswap_dword_inplace(level->presence_context_id);
			bswap_dword_inplace(level->sort_order);
		}

		level->allows_saved_films = scenario->allows_saved_films;

		if (type_is_solo)
		{
			long insertion_index = datum_new(*g_level_globals.campaign_insertions);
			if (insertion_index != NONE)
			{
				s_level_insertion_datum* level_insertion = (s_level_insertion_datum*)datum_get(*g_level_globals.campaign_insertions, insertion_index);
				csmemset(level_insertion, 0, sizeof(s_level_insertion_datum));

				short insertion_count = 0;
				switch (scenario_header.chunk_size)
				{
				case 0x4D50:
				{
					s_blf_chunk_scenario_halo3* scenario_halo3 = (s_blf_chunk_scenario_halo3*)scenario;
					s_blf_chunk_scenario_insertion_halo3 const* scenario_insertion = scenario_halo3->insertions;

					for (long i = 0; i < NUMBEROF(scenario_halo3->insertions); i++)
					{
						wcsncpy_s(level_insertion->names[i], scenario_halo3->insertions[i].names[language].get_string(), NUMBEROF(level_insertion->names[i]));
						wcsncpy_s(level_insertion->descriptions[i], scenario_halo3->insertions[i].descriptions[language].get_string(), NUMBEROF(level->description));

						level_insertion->zone_sets[i] = byte(scenario_insertion->zone_set);
						level_insertion->return_from_map_ids[i] = NONE;
						level_insertion->survival_presence_context_ids[i] = NONE;

						if (byte_swap)
						{
							unicode_byte_swap_wchar_string(level_insertion->names[i], NUMBEROF(level_insertion->names[i]), byte_swap_get_runtime_byte_order() != 1);
							unicode_byte_swap_wchar_string(level_insertion->descriptions[i], NUMBEROF(level_insertion->descriptions[i]), byte_swap_get_runtime_byte_order() != 1);
						}

						level_insertion->__flagsB9C[i] = scenario_insertion->flags;

						insertion_count++;
					}
				}
				break;
				case 0x98C0:
				{
					s_blf_chunk_scenario_atlas* scenario_atlas = (s_blf_chunk_scenario_atlas*)scenario;
					s_blf_chunk_scenario_insertion_atlas const* scenario_insertion = scenario_atlas->insertions;

					for (long i = 0; i < NUMBEROF(scenario_atlas->insertions); i++)
					{
						wcsncpy_s(level_insertion->names[i], scenario_atlas->insertions[i].names[language].get_string(), NUMBEROF(level_insertion->names[i]));
						wcsncpy_s(level_insertion->descriptions[i], scenario_atlas->insertions[i].descriptions[language].get_string(), NUMBEROF(level->description));

						level_insertion->zone_sets[i] = byte(scenario_insertion->zone_set);
						level_insertion->return_from_map_ids[i] = scenario_insertion->return_from_map_id;
						level_insertion->survival_presence_context_ids[i] = scenario_insertion->survival_presence_context_id;

						if (byte_swap)
						{
							unicode_byte_swap_wchar_string(level_insertion->names[i], NUMBEROF(level_insertion->names[i]), byte_swap_get_runtime_byte_order() != 1);
							unicode_byte_swap_wchar_string(level_insertion->descriptions[i], NUMBEROF(level_insertion->descriptions[i]), byte_swap_get_runtime_byte_order() != 1);

							bswap_dword_inplace(level_insertion->return_from_map_ids[i]);
							bswap_dword_inplace(level_insertion->survival_presence_context_ids[i]);
						}

						level_insertion->__flagsB9C[i] = scenario_insertion->flags;

						insertion_count++;
					}
				}
				break;
				default:
					ASSERT2("unreachable");
					break;
				}

				level_insertion->map_id = level->map_id;
				level_insertion->insertion_count = insertion_count;
			}
		}

		if (is_dlc)
		{
			csnzprintf(level->scenario_path, sizeof(level->scenario_path), "%ls\\%s", dlc_path, scenario->scenario_path.get_string());
			csnzprintf(level->image_file_base, sizeof(level->image_file_base), "%ls\\%s", dlc_path, scenario->image_file_base.get_string());
		}
		else
		{
			csnzprintf(level->scenario_path, sizeof(level->scenario_path), "%s%s", cache_files_map_directory(), scenario->scenario_path.get_string());
			csnzprintf(level->image_file_base, sizeof(level->image_file_base), "%simages\\%s", cache_files_map_directory(), scenario->image_file_base.get_string());
		}
	}
}

bool __cdecl levels_begin_dvd_enumeration()
{
	//bool result = false;
	//HOOK_INVOKE(result =, levels_begin_dvd_enumeration);
	//return result;

	if (shell_application_type() == _shell_application_type_client)
	{
		s_async_task task{};
		task.configuration_enumeration_task.enumeration_index = 0;
		task.configuration_enumeration_task.find_file_data = &g_level_globals.enumeration_task_data;
		g_level_globals.enumeration_task = async_task_add(_async_priority_important_non_blocking, &task, _async_category_saved_games, levels_dvd_enumeration_callback, &g_level_globals.enumeration_result);

		if (g_level_globals.enumeration_task != NONE)
		{
			g_level_globals.__unknownA14 = false;
			return true;
		}
	}

	return false;
}

void __cdecl levels_dispose()
{
	//INVOKE(0x0054ADF0, levels_dispose);

	if (levels_enumeration_in_progress())
		internal_async_yield_until_done(&g_level_globals.enumeration_result, false, false, __FILE__, __LINE__);

	g_level_globals.initialized = false;

	data_make_invalid(*g_level_globals.campaigns);
	data_make_invalid(*g_level_globals.campaign_levels);
	data_make_invalid(*g_level_globals.campaign_insertions);
	data_make_invalid(*g_level_globals.multiplayer_levels);

	if (*g_level_globals.campaigns)
		data_dispose(*g_level_globals.campaigns);

	if (*g_level_globals.campaign_levels)
		data_dispose(*g_level_globals.campaign_levels);

	if (*g_level_globals.campaign_insertions)
		data_dispose(*g_level_globals.campaign_insertions);

	if (*g_level_globals.multiplayer_levels)
		data_dispose(*g_level_globals.multiplayer_levels);
}

void __cdecl levels_dispose_from_old_map()
{
	INVOKE(0x0054AEA0, levels_dispose_from_old_map);
}

// searches for `campaign`, `mapinfo`, `xex` and `preorder_unlock.txt`
long __cdecl levels_dvd_enumeration_callback2(void* callback_data) // c_levels_dlc_examine_content_item_async::update?
{
	long result = 0;
	HOOK_INVOKE(result =, levels_dvd_enumeration_callback2, callback_data);

	return result;
}

e_async_completion __cdecl levels_dvd_enumeration_callback(s_async_task* task_data)
{
	c_static_string<256> found_file_name{};
	s_file_reference found_file{};

	if (task_data->configuration_enumeration_task.enumeration_index)
	{
		if (task_data->configuration_enumeration_task.enumeration_index == 1)
		{
			s_file_reference file{};
			s_file_last_modification_date date{};

			if (!find_files_next(task_data->configuration_enumeration_task.find_file_data, &file, &date))
			{
				find_files_end(task_data->configuration_enumeration_task.find_file_data);
				return e_async_completion(++task_data->configuration_enumeration_task.enumeration_index == 2);
			}

			wchar_t file_directory[256]{};
			wchar_t file_extension[256]{};
			wchar_t file_name_with_extension[256]{};

			file_reference_get_name_wide(&file, FLAG(_name_directory_bit), file_directory, NUMBEROF(file_directory));
			file_reference_get_name_wide(&file, FLAG(_name_extension_bit), file_extension, NUMBEROF(file_extension));
			file_reference_get_name_wide(&file, FLAG(_name_file_bit) | FLAG(_name_extension_bit), file_name_with_extension, NUMBEROF(file_name_with_extension));

			found_file_name.append_print("%ls", file_name_with_extension);

			if (!ustricmp(file_extension, L"campaign"))
			{
				levels_process_campaign_configuration_file(&file, file_directory, false);
			}
			else if (!ustricmp(file_extension, L"mapinfo"))
			{
				levels_process_level_configuration_file(&file, file_directory, false);
			}
		}
	}
	else
	{
		found_file_name.append_print("%sinfo", cache_files_map_directory());

		file_reference_create_from_path(&found_file, found_file_name.get_string(), true);

		find_files_start(task_data->configuration_enumeration_task.find_file_data, 0, &found_file);

		++task_data->configuration_enumeration_task.enumeration_index;
	}

	return e_async_completion(task_data->configuration_enumeration_task.enumeration_index == 2);
}

bool __cdecl levels_enumeration_in_progress()
{
	//return INVOKE(0x0054B3A0, levels_enumeration_in_progress);

	return g_level_globals.enumeration_task != NONE && !g_level_globals.enumeration_result.peek();
}

//.text:0054B3C0 ; 
//.text:0054B400 ; wchar_t* __cdecl levels_get_active_session_map_name(wchar_t*, long)
//.text:0054B500 ; wchar_t* __cdecl levels_get_active_session_campaign_name(wchar_t*, long)

dword __cdecl levels_get_available_map_mask()
{
	return INVOKE(0x0054B610, levels_get_available_map_mask);

	//dword available_map_mask = 0;
	//
	//long map_ids[32]{};
	//long count = NUMBEROF(map_ids);
	//levels_get_multiplayer_map_ids(map_ids, &count);
	//for (long i = 0; i < count; i++)
	//{
	//	long map_index = network_configuration_get_map_index_from_map_id(map_ids[i]);
	//	if (map_index != NONE)
	//		SET_BIT(available_map_mask, map_index, true);
	//}
	//
	//return available_map_mask;
}

long __cdecl levels_get_campaign_count()
{
	//return INVOKE(0x0054B670, levels_get_campaign_count);

	return g_level_globals.initialized && !levels_enumeration_in_progress() ? g_level_globals.campaigns->actual_count : 0;
}

//e_campaign_id __cdecl levels_get_campaign_id_from_path(char const* path)
long __cdecl levels_get_campaign_id_from_path(char const* path)
{
	return INVOKE(0x0054B6A0, levels_get_campaign_id_from_path, path);
}

//long __cdecl levels_get_campaign_level_count(e_campaign_id campaign_id)
long __cdecl levels_get_campaign_level_count(long campaign_id)
{
	return INVOKE(0x0054B7B0, levels_get_campaign_level_count, campaign_id);
}

//long __cdecl levels_get_campaign_level_index(e_campaign_id campaign_id, e_map_id map_id)
long __cdecl levels_get_campaign_level_index(long campaign_id, long map_id)
{
	return INVOKE(0x0054B8A0, levels_get_campaign_level_index, campaign_id, map_id);
}

//e_map_id __cdecl levels_get_campaign_map_by_display_name(wchar_t* display_name)
long __cdecl levels_get_campaign_map_by_display_name(wchar_t* display_name)
{
	return INVOKE(0x0054B9D0, levels_get_campaign_map_by_display_name, display_name);
}

//void __cdecl levels_get_campaign_map_ids(e_campaign_id campaign_id, e_map_id* out_map_ids, long* in_out_count)
void __cdecl levels_get_campaign_map_ids(long campaign_id, long* out_map_ids, long* in_out_count)
{
	INVOKE(0x0054BAA0, levels_get_campaign_map_ids, campaign_id, out_map_ids, in_out_count);
}

//e_map_id __cdecl levels_get_campaign_next_map_id(e_campaign_id campaign_id, e_map_id map_id)
long __cdecl levels_get_campaign_next_map_id(long campaign_id, long map_id)
{
	return INVOKE(0x0054BB80, levels_get_campaign_next_map_id, campaign_id, map_id);
}

//e_map_id levels_get_default_multiplayer_map_id()
long __cdecl levels_get_default_multiplayer_map_id()
{
	//return INVOKE(0x0054BCA0, levels_get_default_multiplayer_map_id);

	long map_id = NONE;

	if (g_level_globals.initialized)
	{
		c_critical_section_scope critical_section_scope(_critical_section_levels);
		c_data_iterator<s_level_datum> level_iter{};

		level_iter.begin(*g_level_globals.multiplayer_levels);
		while (level_iter.next())
		{
			s_level_datum* level = level_iter.get_datum();
			if (level->map_id > 0 && levels_get_multiplayer_map_is_allowed(level->map_id))
			{
				if (map_id == NONE || map_id > level->map_id)
					map_id = level->map_id;
			}
		}
	}

	return map_id;
}

dword __cdecl levels_get_checksum()
{
	//return INVOKE(0x0054BDB0, levels_get_checksum);

	return g_level_globals.checksum;
}

//.text:0054BDC0 ; levels_get_campaign_insertion_zone_set_by_map_id_and_index?

//e_map_id __cdecl levels_get_multiplayer_map_by_display_name(wchar_t* display_name)
long __cdecl levels_get_multiplayer_map_by_display_name(wchar_t const* display_name)
{
	//return INVOKE(0x0054BE10, levels_get_multiplayer_map_by_display_name, display_name);

	if (g_level_globals.initialized)
	{
		c_critical_section_scope critical_section_scope(_critical_section_levels);
		c_data_iterator<s_level_datum> level_iter{};

		level_iter.begin(*g_level_globals.multiplayer_levels);
		while (level_iter.next())
		{
			s_level_datum* level = level_iter.get_datum();
			if (ustrnicmp(display_name, level->name, NUMBEROF(level->name)))
				return level->map_id;
		}
	}

	return NONE;
}

//void __cdecl levels_get_multiplayer_map_ids(e_map_id* out_map_ids, long* in_out_count)
void __cdecl levels_get_multiplayer_map_ids(long* out_map_ids, long* in_out_count)
{
	//INVOKE(0x0054BEE0, levels_get_multiplayer_map_ids, out_map_ids, in_out_count);

	ASSERT(out_map_ids != NULL);
	ASSERT(in_out_count != NULL);

	long count = 0;
	if (g_level_globals.initialized)
	{
		c_critical_section_scope critical_section_scope(_critical_section_levels);
		c_data_iterator<s_level_datum> level_iter{};

		long maximum_count = *in_out_count;
		ASSERT(maximum_count > 0);
		csmemset(out_map_ids, NONE, sizeof(long) * maximum_count);

		level_iter.begin(*g_level_globals.multiplayer_levels);
		while (level_iter.next() && count < maximum_count)
		{
			long map_id = level_iter.get_datum()->map_id;
			bool has_map_id = true;

			for (long i = 0; i < count; i++)
				has_map_id = map_id != out_map_ids[i];

			if (has_map_id)
				out_map_ids[count++] = map_id;
		}
	}
	*in_out_count = count;
}

//bool __cdecl levels_get_multiplayer_map_is_allowed(e_map_id map_id)
bool __cdecl levels_get_multiplayer_map_is_allowed(long map_id)
{
	//return INVOKE(0x0054BFD0, levels_get_multiplayer_map_is_allowed, map_id);

	if (get_alpha_is_external_beta())
		return map_id == 310 || map_id == 340 || map_id == 360;

	if (get_alpha_is_internal_beta())
		return map_id == 310 || map_id == 340 || map_id == 360 || map_id == 350 || map_id == 30;

	return true;
}

//bool __cdecl levels_find_path(s_data_array* data, e_map_id map_id, char* path, long maximum_characters)
bool __cdecl levels_find_path(s_data_array* data, long map_id, char* path, long maximum_characters)
{
	c_data_iterator<s_level_datum> level_iter{};

	level_iter.begin(data);
	while (level_iter.next())
	{
		s_level_datum* level = level_iter.get_datum();
		if (level->map_id == map_id)
		{
			csstrnzcpy(path, level->scenario_path, maximum_characters);
			return true;
		}
	}

	return false;
}

//char* __cdecl levels_get_path(e_campaign_id campaign_id, e_map_id map_id, char* path, long maximum_characters)
char* __cdecl levels_get_path(long campaign_id, long map_id, char* path, long maximum_characters)
{
	//return INVOKE(0x0054C040, levels_get_path, campaign_id, map_id, path, maximum_characters);

	ASSERT(path != NULL);
	ASSERT(maximum_characters > 0);

	c_critical_section_scope critical_section_scope(_critical_section_levels);

	if (map_id == 0x10231971)
	{
		csstrnzcpy(path, g_level_globals.mainmenu_level.scenario_path, maximum_characters);
	}
	else if (campaign_id == NONE)
	{
		levels_find_path(*g_level_globals.multiplayer_levels, map_id, path, maximum_characters);
	}
	else
	{
		levels_find_path(*g_level_globals.campaign_levels, map_id, path, maximum_characters);
	}

	return path;
}

void __cdecl levels_initialize()
{
	//INVOKE(0x0054C110, levels_initialize);

	csmemset(&g_level_globals, 0, sizeof(s_level_globals));
	g_level_globals.enumeration_task = NONE;

	g_level_globals.campaigns = data_new("campaigns", 4, sizeof(s_campaign_datum), 0, g_runtime_state_allocation);
	g_level_globals.campaign_levels = data_new("campaign levels", 64, sizeof(s_level_datum), 0, g_runtime_state_allocation);
	g_level_globals.campaign_insertions = data_new("campaign insertions", 32, sizeof(s_level_insertion_datum), 0, g_runtime_state_allocation);
	g_level_globals.multiplayer_levels = data_new("multiplayer levels", 50, sizeof(s_level_datum), 0, g_runtime_state_allocation);

	data_make_valid(*g_level_globals.campaigns);
	data_make_valid(*g_level_globals.campaign_levels);
	data_make_valid(*g_level_globals.campaign_insertions);
	data_make_valid(*g_level_globals.multiplayer_levels);

	g_level_globals.mainmenu_level.flags.clear();
	g_level_globals.mainmenu_level.flags.set(_level_ui_bit, true);
	g_level_globals.mainmenu_level.map_id = 0x10231971;
	csnzprintf(g_level_globals.mainmenu_level.scenario_path, sizeof(g_level_globals.mainmenu_level.scenario_path), "%s%s", cache_files_map_directory(), "mainmenu");

	g_level_globals.initialized = true;
	g_level_globals.controller_mask = 0;
	g_level_globals.__unknownA14 = true;
	g_level_globals.checksum = NONE;

	levels_begin_dvd_enumeration();
	levels_update();
}

void __cdecl levels_initialize_for_new_map()
{
	INVOKE(0x0054C2D0, levels_initialize_for_new_map);
}

//.text:0054C2E0 ; 

//bool __cdecl levels_map_id_is_fake(e_map_id map_id)
bool __cdecl levels_map_id_is_fake(long map_id)
{
	//bool result = false;
	//HOOK_INVOKE(result =, levels_map_id_is_fake, map_id);
	//return result;

	return map_id == -2;
}

void __cdecl levels_open_dlc(char const* scenario_path, bool blocking)
{
	INVOKE(0x0054C330, levels_open_dlc, scenario_path, blocking);
}

bool __cdecl levels_path_is_dlc(char const* scenario_path)
{
	return INVOKE(0x0054C360, levels_path_is_dlc, scenario_path);
}

void __cdecl levels_process_campaign_configuration_file(s_file_reference* file, wchar_t const* maps_path, bool is_dlc)
{
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
	//HOOK_INVOKE(, levels_process_level_configuration_file, file, maps_path, is_dlc);

	static char file_buffer[sizeof(s_blf_chunk_scenario_atlas)]{};
	s_blf_chunk_scenario const* level = nullptr;
	bool byte_swap = false;

	levels_find_scenario_chunk(file, file_buffer, &level, &byte_swap);
	if (level)
		levels_add_level(level, byte_swap, maps_path, is_dlc);
}

void __cdecl levels_delete()
{
	//INVOKE(0x0054C820, levels_delete);

	c_data_iterator<s_campaign_datum> campaign_iter{};
	c_data_iterator<s_level_datum> campaign_level_iter{};
	c_data_iterator<s_level_datum> multiplayer_level_iter{};

	campaign_iter.begin(*g_level_globals.campaigns);
	while (campaign_iter.next())
	{
		if (TEST_BIT(campaign_iter.get_datum()->flags, 0))
			datum_delete(*g_level_globals.campaigns, campaign_iter.get_index());
	}

	campaign_level_iter.begin(*g_level_globals.campaign_levels);
	while (campaign_level_iter.next())
	{
		if (campaign_level_iter.get_datum()->flags.test(_level_bit0))
			datum_delete(*g_level_globals.campaign_levels, campaign_level_iter.get_index());
	}

	multiplayer_level_iter.begin(*g_level_globals.multiplayer_levels);
	while (multiplayer_level_iter.next())
	{
		if (multiplayer_level_iter.get_datum()->flags.test(_level_bit0))
			datum_delete(*g_level_globals.multiplayer_levels, multiplayer_level_iter.get_index());
	}
}

//bool __cdecl levels_try_and_get_by_map_id(s_data_array* data, e_map_id map_id, s_level_datum* level)
bool __cdecl levels_try_and_get_by_map_id(s_data_array* data, long map_id, s_level_datum* level)
{
	//return INVOKE(0x0054C910, levels_try_and_get_by_map_id, data, map_id, level);

	if (g_level_globals.initialized && map_id != NONE)
	{
		c_critical_section_scope critical_section_scope(_critical_section_levels);
		c_data_iterator<s_level_datum> level_iter{};

		level_iter.begin(data);
		while (level_iter.next())
		{
			if (level_iter.get_datum()->map_id == map_id)
			{
				*level = *level_iter.get_datum();
				return true;
			}
		}
	}

	return false;
}

//bool __cdecl levels_try_and_get_campaign_insertion(e_map_id map_id, s_level_insertion_datum* insertion)
bool __cdecl levels_try_and_get_campaign_insertion(long map_id, s_level_insertion_datum* insertion)
{
	//return INVOKE(0x0054C9E0, levels_try_and_get_campaign_insertion, map_id, insertion);

	ASSERT(insertion);

	if (g_level_globals.initialized && map_id != NONE)
	{
		c_critical_section_scope critical_section_scope(_critical_section_levels);
		c_data_iterator<s_level_insertion_datum> insertion_iter{};

		insertion_iter.begin(*g_level_globals.campaign_insertions);
		while (insertion_iter.next())
		{
			if (insertion_iter.get_datum()->map_id == map_id)
			{
				*insertion = *insertion_iter.get_datum();
				return true;
			}
		}
	}

	return false;
}

//bool __cdecl levels_try_and_get_campaign_map(e_map_id map_id, s_level_datum* level)
bool __cdecl levels_try_and_get_campaign_map(long map_id, s_level_datum* level)
{
	//return INVOKE(0x0054CAB0, levels_try_and_get_campaign_map, map_id, level);

	ASSERT(level != NULL);

	return levels_try_and_get_by_map_id(*g_level_globals.campaign_levels, map_id, level);
}

//bool __cdecl levels_try_and_get_main_menu_map(s_level_datum* level)
bool __cdecl levels_try_and_get_main_menu_map(s_level_datum* level)
{
	//return INVOKE(0x0054CAD0, levels_try_and_get_main_menu_map, level);

	ASSERT(level != NULL);

	c_critical_section_scope critical_section_scope(_critical_section_levels);
	*level = g_level_globals.mainmenu_level;

	return true;
}

//bool __cdecl levels_try_and_get_multiplayer_map(e_map_id map_id, s_level_datum* level)
bool __cdecl levels_try_and_get_multiplayer_map(long map_id, s_level_datum* level)
{
	//return INVOKE(0x0054CB00, levels_try_and_get_multiplayer_map, map_id, level);

	ASSERT(level != NULL);

	return levels_try_and_get_by_map_id(*g_level_globals.multiplayer_levels, map_id, level);
}

void __cdecl levels_update()
{
	INVOKE(0x0054CB20, levels_update);

	//if (!levels_enumeration_in_progress())
	//	sub_54CBA0();
}

void __cdecl sub_54CBA0()
{
	INVOKE(0x0054CBA0, sub_54CBA0);
}

void __cdecl sub_54CC00()
{
	INVOKE(0x0054CC00, sub_54CBA0);
}

//.text:0054CC10 ; 
//.text:0054CC20 ; 
//.text:0054CC40 ; 
//.text:0054CC60 ; 
//.text:0054CC80 ; 
//.text:0054CCA0 ; 
//.text:0054CCC0 ; 

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

	if (!file_open(file, FLAG(_file_open_flag_desired_access_read), &error))
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

	// .campaign file size
	// - 0x1459, Halo 3 to Halo Reach
	// - 0x18AC, Specific to Halo Online?
	// - 0x1C19, Halo 4
	if (file_size > 0x18AC)
	{
		c_console::write_line("levels: unexpected file size for campaign info file");
		goto function_finish;
	}

	if (!file_read(file, file_size, 0, file_buffer))
	{
		c_console::write_line("levels: failed to read campaign info file");
		goto function_finish;
	}

	if (!network_blf_verify_start_of_file(file_buffer, file_size, &byte_swap, &chunk_size))
	{
		c_console::write_line("levels: failed to verify blf start of file for campaign info file");
		goto function_finish;
	}

	if (!network_blf_find_chunk(file_buffer, file_size, byte_swap, s_blf_chunk_campaign::k_chunk_type, s_blf_chunk_campaign::k_version_major, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
	{
		c_console::write_line("levels: failed to find blf campaign chunk");
		goto function_finish;
	}

	if (chunk_buffer)
	{
		campaign = reinterpret_cast<s_blf_chunk_campaign const*>(chunk_buffer - sizeof(s_blf_header));
		if (chunk_buffer != (char const*)sizeof(s_blf_header) && network_blf_find_chunk(file_buffer, file_size, byte_swap, s_blf_chunk_end_of_file::k_chunk_type, s_blf_chunk_end_of_file::k_version_major, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
		{
			if (chunk_buffer)
			{
				if (chunk_size == sizeof(s_blf_chunk_end_of_file_with_rsa) && network_blf_verify_end_of_file(file_buffer, file_size, byte_swap, chunk_buffer - sizeof(s_blf_header), _blf_file_authentication_type_rsa))
				{
					file_added = true;
				}

				if (file_added)
					goto function_finish;
			}

			c_console::write_line("levels: failed to verify blf end of file chunk");
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

	if (!file_open(file, FLAG(_file_open_flag_desired_access_read), &error))
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

	// .mapinfo file size
	// - 0x4E91, Halo 3
	// - 0x9A01, Halo 3: ODST
	// - 0xCDD9, Halo Reach
	// - 0x11F19, Halo 4
	if (file_size > 0xCDD9)
	{
		c_console::write_line("levels: unexpected file size for level info file");
		goto function_finish;
	}

	if (!file_read(file, file_size, false, file_buffer))
	{
		c_console::write_line("levels: failed to read level info file");
		goto function_finish;
	}

	if (!network_blf_verify_start_of_file(file_buffer, file_size, &byte_swap, &chunk_size))
	{
		c_console::write_line("levels: failed to verify blf start of file");
		goto function_finish;
	}

	if (!network_blf_find_chunk(file_buffer, file_size, byte_swap, s_blf_chunk_scenario::k_chunk_type, s_blf_chunk_scenario::k_version_major, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
	{
		c_console::write_line("levels: failed to find blf scenario chunk");
		goto function_finish;
	}

	if (chunk_buffer)
	{
		scenario = reinterpret_cast<s_blf_chunk_scenario const*>(chunk_buffer - sizeof(s_blf_header));
		if (chunk_buffer != (char const*)sizeof(s_blf_header) && network_blf_find_chunk(file_buffer, file_size, byte_swap, s_blf_chunk_end_of_file::k_chunk_type, s_blf_chunk_end_of_file::k_version_major, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
		{
			if (eof_chunk)
			{
				if (chunk_size == sizeof(s_blf_chunk_end_of_file) && network_blf_verify_end_of_file(file_buffer, file_size, byte_swap, chunk_buffer - sizeof(s_blf_header), s_blf_chunk_end_of_file::k_authentication_type))
				{
					file_added = true;
				}
				else if (chunk_size == sizeof(s_blf_chunk_end_of_file_with_rsa) && network_blf_verify_end_of_file(file_buffer, file_size, byte_swap, chunk_buffer - sizeof(s_blf_header), s_blf_chunk_end_of_file_with_rsa::k_authentication_type))
				{
					file_added = true;
				}

				if (file_added)
					goto function_finish;
			}

			c_console::write_line("levels: failed to verify blf end of file chunk");
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
		dword_flags flags = FLAG(0) | FLAG(2) | FLAG(3);
		char filename[256]{};
		file_reference_get_name(file, flags, filename, NUMBEROF(filename));
		c_console::write_line("levels: failed to add level file '%s'", file->path.get_string());
	}
}

