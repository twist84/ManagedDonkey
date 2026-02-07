#include "main/levels.hpp"

#include "cache/cache_files.hpp"
#include "cseries/async.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "cseries/runtime_state.hpp"
#include "input/controllers.hpp"
#include "interface/user_interface.hpp"
#include "main/main.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "networking/tools/network_blf.hpp"
#include "saved_games/content_catalogue.hpp"
#include "shell/shell.hpp"
#include "tag_files/files_windows.hpp"
#include "text/unicode.hpp"

#include <string.h>

REFERENCE_DECLARE(0x0189E2D4, s_level_globals, g_level_globals);

HOOK_DECLARE(0x0054A2A0, levels_add_campaign_from_configuration_file);
HOOK_DECLARE(0x0054A4E0, levels_add_map_from_scripting);
HOOK_DECLARE(0x0054A6A0, levels_add_fake_map_from_scripting);
HOOK_DECLARE(0x0054A6C0, levels_add_level_from_configuration_file);
HOOK_DECLARE(0x0054AD00, levels_begin_dlc_enumeration);
HOOK_DECLARE(0x0054AD80, levels_begin_dvd_enumeration);
//HOOK_DECLARE(0x0054ADF0, levels_dispose);
//HOOK_DECLARE(0x0054AEA0, levels_dispose_from_old_map);
HOOK_DECLARE(0x0054AEC0, levels_dlc_enumeration_callback);
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
//HOOK_DECLARE(0x0054C110, levels_initialize);
//HOOK_DECLARE(0x0054C2D0, levels_initialize_for_new_map);
HOOK_DECLARE(0x0054C320, levels_map_id_is_fake);
//HOOK_DECLARE(0x0054C330, levels_open_dlc);
//HOOK_DECLARE(0x0054C360, levels_path_is_dlc);
HOOK_DECLARE(0x0054C3C0, levels_process_campaign_configuration_file);
HOOK_DECLARE(0x0054C530, levels_process_level_configuration_file);
HOOK_DECLARE(0x0054C820, levels_remove_dlc);
HOOK_DECLARE(0x0054C910, levels_try_and_get_by_map_id);
HOOK_DECLARE(0x0054C9E0, levels_try_and_get_campaign_insertion);
HOOK_DECLARE(0x0054CAB0, levels_try_and_get_campaign_map);
HOOK_DECLARE(0x0054CAD0, levels_try_and_get_main_menu_map);
HOOK_DECLARE(0x0054CB00, levels_try_and_get_multiplayer_map);
//HOOK_DECLARE(0x0054CB20, levels_update);

void __cdecl levels_add_campaign_from_configuration_file(const s_blf_chunk_campaign* campaign_data, bool must_byte_swap, const wchar_t* source_directory_path, bool dlc_content)
{
	HOOK_INVOKE(, levels_add_campaign_from_configuration_file, campaign_data, must_byte_swap, source_directory_path, dlc_content);
}

void __cdecl levels_add_map_from_scripting(e_map_id map_id, const char* scenario_path)
{
	//HOOK_INVOKE(, levels_add_map_from_scripting, map_id, scenario_path);

	if (g_level_globals.need_to_enumerate_dvd)
	{
		if (!levels_enumeration_in_progress())
			levels_begin_dvd_enumeration();

		while (levels_enumeration_in_progress())
			main_loop_pregame();
	}

	c_critical_section_scope critical_section_scope(k_crit_section_levels);

	// called but never used, comment out for now
	//levels_try_and_get_by_map_id(g_level_globals.campaign_levels, map_id, &campaign_level);

	int32 campaign_level_index = datum_new(g_level_globals.campaign_levels);
	if (campaign_level_index != NONE)
	{
		s_level_datum* level = DATUM_TRY_AND_GET(g_level_globals.campaign_levels, s_level_datum, campaign_level_index);

		// 0x2C
		level->flags.set(_level_visible_in_ui, true);
		level->flags.set(_level_allows_saved_films, true);
		level->flags.set(_level_is_campaign_bit, true);

		level->map_id = map_id;
		usnzprintf(level->name, NUMBEROF(level->name), L"%S", tag_name_strip_path(scenario_path));
		ustrnzcpy(level->description, L"This is a fake level!", NUMBEROF(level->description));

		level->presence_context_id = -1;
		level->sort_order = -1;

		level->minimum_desired_players = char(0xFF);
		level->maximum_desired_players = char(0xFF);

		csnzprintf(level->scenario_file, NUMBEROF(level->scenario_file), "%s%s", cache_files_map_directory(), scenario_path);

		c_data_iterator<s_campaign_datum> campaign_iterator;
		campaign_iterator.begin(g_level_globals.campaigns);
		if (campaign_iterator.next())
		{
			s_campaign_datum* campaign = campaign_iterator.get_datum();

			while (campaign->campaign_id != _campaign_id_default)
			{
				if (!campaign_iterator.next())
					return;
			}

			int32 map_index = 0;
			e_map_id* map_ids = campaign->map_ids;
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

void __cdecl levels_add_fake_map_from_scripting(const char* scenario_path)
{
	//HOOK_INVOKE(, levels_add_fake_map_from_scripting, scenario_path);

	levels_add_map_from_scripting(_map_id_use_scenario_path, scenario_path);
}

void __cdecl levels_add_multiplayer_map_from_scripting(e_map_id map_id, const char* scenario_path)
{
	if (g_level_globals.need_to_enumerate_dvd)
	{
		if (!levels_enumeration_in_progress())
			levels_begin_dvd_enumeration();

		while (levels_enumeration_in_progress())
			main_loop_pregame();
	}

	c_critical_section_scope critical_section_scope(k_crit_section_levels);

	// called but never used, comment out for now
	//levels_try_and_get_by_map_id(g_level_globals.multiplayer_levels, map_id, &multiplayer_level);

	int32 multiplayer_level_index = datum_new(g_level_globals.multiplayer_levels);
	if (multiplayer_level_index != NONE)
	{
		s_level_datum* level = DATUM_TRY_AND_GET(g_level_globals.multiplayer_levels, s_level_datum, multiplayer_level_index);

		// 0x4C
		level->flags.set(_level_visible_in_ui, true);
		level->flags.set(_level_allows_saved_films, true);
		level->flags.set(_level_is_multiplayer_bit, true);

		level->map_id = map_id;
		usnzprintf(level->name, NUMBEROF(level->name), L"%S", tag_name_strip_path(scenario_path));
		ustrnzcpy(level->description, L"This is a fake level!", NUMBEROF(level->description));

		level->presence_context_id = -1;
		level->sort_order = 0;

		level->minimum_desired_players = 16;
		level->maximum_desired_players = 16;
		
		level->maximum_teams[_game_engine_type_none]        = 0;
		level->maximum_teams[_game_engine_type_ctf]         = 2;
		level->maximum_teams[_game_engine_type_slayer]      = 8;
		level->maximum_teams[_game_engine_type_oddball]     = 8;
		level->maximum_teams[_game_engine_type_king]        = 8;
		level->maximum_teams[_game_engine_type_sandbox]     = 8;
		level->maximum_teams[_game_engine_type_vip]         = 8;
		level->maximum_teams[_game_engine_type_juggernaut]  = 8;
		level->maximum_teams[_game_engine_type_territories] = 4;
		level->maximum_teams[_game_engine_type_assault]     = 2;
		level->maximum_teams[_game_engine_type_infection]   = 8;

		csnzprintf(level->scenario_file, NUMBEROF(level->scenario_file), "%s%s", cache_files_map_directory(), scenario_path);
	}
}

void __cdecl levels_add_fake_multiplayer_map_from_scripting(const char* scenario_path)
{
	levels_add_multiplayer_map_from_scripting(_map_id_use_scenario_path, scenario_path);
}

void __cdecl levels_add_level_from_configuration_file(const s_blf_chunk_scenario* level_data, bool must_byte_swap, const wchar_t* source_directory_path, bool dlc_content)
{
	//HOOK_INVOKE(, levels_add_level_from_configuration_file, level_data, must_byte_swap, maps_path, dlc_content);

	c_critical_section_scope critical_section_scope(k_crit_section_levels);

	e_language language = get_current_language();

	int32 level_chunk_size = must_byte_swap ? bswap_uns32(level_data->header.chunk_size) : level_data->header.chunk_size;

	decltype(level_data->flags) flags = level_data->flags;
	if (must_byte_swap)
		flags.set_unsafe(bswap_uns32(flags.get_unsafe()));

	s_level_datum* level = nullptr;

	if (flags.test(_level_is_main_menu_bit))
	{
		level = &g_level_globals.main_menu;
	}
	else if (flags.test(_level_is_campaign_bit) || flags.test(_level_is_multiplayer_bit))
	{
		s_data_array* levels_data = flags.test(_level_is_campaign_bit) ? g_level_globals.campaign_levels : g_level_globals.multiplayer_levels;
		int32 level_index = datum_new(levels_data);
		if (level_index != NONE)
			level = DATUM_GET(levels_data, s_level_datum, level_index);
	}

	if (level != nullptr)
	{
		level->flags.clear();
		level->flags.set(_level_from_dlc_bit, dlc_content);
		level->flags.set(_level_unlockable_bit, flags.test(_level_unlockable_bit));
		level->flags.set(_level_visible_in_ui, flags.test(_level_visible_in_ui));
		level->flags.set(_level_allows_saved_films, flags.test(_level_allows_saved_films));
		level->flags.set(_level_is_main_menu_bit, flags.test(_level_is_main_menu_bit));
		level->flags.set(_level_is_campaign_bit, flags.test(_level_is_campaign_bit));
		level->flags.set(_level_is_multiplayer_bit, flags.test(_level_is_multiplayer_bit));
		level->flags.set(_level_in_dlc_directory_bit, flags.test(_level_in_dlc_directory_bit));
		level->flags.set(_level_in_test_directory_bit, flags.test(_level_in_test_directory_bit));
		level->flags.set(_level_in_temp_directory_bit, flags.test(_level_in_temp_directory_bit));

		if (must_byte_swap)
		{
			level->map_id = (e_map_id)bswap_uns32(level_data->map_id);
			level->presence_context_id = bswap_uns32(level_data->presence_context_id);
			level->sort_order = bswap_uns32(level_data->sort_order);

			ustrnzcpy(level->name, level_data->name[language], NUMBEROF(level->name));
			unicode_byte_swap_wchar_string(level->name, NUMBEROF(level->name), byte_swap_get_runtime_byte_order() != 1);

			ustrnzcpy(level->description, level_data->description[language], NUMBEROF(level->description));
			unicode_byte_swap_wchar_string(level->description, NUMBEROF(level->description), byte_swap_get_runtime_byte_order() != 1);

			level->minimum_desired_players = level_data->mp_minimum_desired_players;
			level->maximum_desired_players = level_data->mp_maximum_desired_players;

			for (int32 engine_index = 0; engine_index < k_game_engine_type_count; engine_index++)
				level->maximum_teams[engine_index] = level_data->maximum_teams[engine_index];

			level->allows_saved_films = level_data->allows_saved_films;
		}
		else
		{
			level->map_id = level_data->map_id;
			level->presence_context_id = level_data->presence_context_id;
			level->sort_order = level_data->sort_order;

			ustrnzcpy(level->name, level_data->name[language], NUMBEROF(level->name));
			ustrnzcpy(level->description, level_data->description[language], NUMBEROF(level->description));

			level->minimum_desired_players = level_data->mp_minimum_desired_players;
			level->maximum_desired_players = level_data->mp_maximum_desired_players;

			for (int32 engine_index = 0; engine_index < k_game_engine_type_count; engine_index++)
				level->maximum_teams[engine_index] = level_data->maximum_teams[engine_index];

			level->allows_saved_films = level_data->allows_saved_films;
		}

		if (flags.test(_level_is_campaign_bit))
		{
			int32 insertion_index = datum_new(g_level_globals.campaign_insertions);
			if (insertion_index != NONE)
			{
				s_level_insertion_datum* level_insertion = DATUM_GET(g_level_globals.campaign_insertions, s_level_insertion_datum, insertion_index);
				csmemset(level_insertion, 0, sizeof(s_level_insertion_datum));

				int16 insertion_count = 0;
				switch (level_chunk_size)
				{
				case 0x4D50:
				{
					s_blf_chunk_scenario_halo3* scenario = (s_blf_chunk_scenario_halo3*)level_data;
					const s_scenario_insertion_point_halo3* scenario_insertion = scenario->insertions;

					for (int32 i = 0; i < NUMBEROF(scenario->insertions); i++)
					{
						if (!scenario->insertions[i].valid)
						{
							continue;
						}

						if (must_byte_swap)
						{
							ustrnzcpy(level_insertion->insertion_point_names[i], scenario->insertions[i].names[language], NUMBEROF(level_insertion->insertion_point_names[i]));
							unicode_byte_swap_wchar_string(level_insertion->insertion_point_names[i], NUMBEROF(level_insertion->insertion_point_names[i]), byte_swap_get_runtime_byte_order() != 1);

							ustrnzcpy(level_insertion->insertion_point_descriptions[i], scenario->insertions[i].descriptions[language], NUMBEROF(level_insertion->insertion_point_descriptions[i]));
							unicode_byte_swap_wchar_string(level_insertion->insertion_point_descriptions[i], NUMBEROF(level_insertion->insertion_point_descriptions[i]), byte_swap_get_runtime_byte_order() != 1);

							level_insertion->insertion_point_initial_zone_set[i] = uns8(bswap_uns16(scenario_insertion->zone_set));
							level_insertion->return_from_map_ids[i] = NONE;
							level_insertion->survival_presence_context_ids[i] = NONE;

							level_insertion->__flagsB9C[i] = scenario_insertion->flags;
						}
						else
						{
							ustrnzcpy(level_insertion->insertion_point_names[i], scenario->insertions[i].names[language], NUMBEROF(level_insertion->insertion_point_names[i]));
							ustrnzcpy(level_insertion->insertion_point_descriptions[i], scenario->insertions[i].descriptions[language], NUMBEROF(level_insertion->insertion_point_descriptions[i]));

							level_insertion->insertion_point_initial_zone_set[i] = uns8(scenario_insertion->zone_set);
							level_insertion->return_from_map_ids[i] = NONE;
							level_insertion->survival_presence_context_ids[i] = NONE;
						}

						insertion_count++;
					}
				}
				break;
				case 0x98C0:
				{
					s_blf_chunk_scenario_atlas* scenario = (s_blf_chunk_scenario_atlas*)level_data;
					const s_scenario_insertion_point_atlas* scenario_insertion = scenario->insertions;

					for (int32 i = 0; i < NUMBEROF(scenario->insertions); i++)
					{
						if (!scenario->insertions[i].valid)
						{
							continue;
						}

						if (must_byte_swap)
						{
							ustrnzcpy(level_insertion->insertion_point_names[i], scenario->insertions[i].names[language], NUMBEROF(level_insertion->insertion_point_names[i]));
							unicode_byte_swap_wchar_string(level_insertion->insertion_point_names[i], NUMBEROF(level_insertion->insertion_point_names[i]), byte_swap_get_runtime_byte_order() != 1);

							ustrnzcpy(level_insertion->insertion_point_descriptions[i], scenario->insertions[i].descriptions[language], NUMBEROF(level_insertion->insertion_point_descriptions[i]));
							unicode_byte_swap_wchar_string(level_insertion->insertion_point_descriptions[i], NUMBEROF(level_insertion->insertion_point_descriptions[i]), byte_swap_get_runtime_byte_order() != 1);

							level_insertion->return_from_map_ids[i] = bswap_uns32(scenario_insertion->return_from_map_id);
							level_insertion->survival_presence_context_ids[i] = bswap_uns32(scenario_insertion->survival_presence_context_id);
							level_insertion->insertion_point_initial_zone_set[i] = uns8(bswap_uns16(scenario_insertion->zone_set));
							level_insertion->__flagsB9C[i] = scenario_insertion->flags;
						}
						else
						{
							ustrnzcpy(level_insertion->insertion_point_names[i], scenario->insertions[i].names[language], NUMBEROF(level_insertion->insertion_point_names[i]));
							ustrnzcpy(level_insertion->insertion_point_descriptions[i], scenario->insertions[i].descriptions[language], NUMBEROF(level_insertion->insertion_point_descriptions[i]));

							level_insertion->return_from_map_ids[i] = scenario_insertion->return_from_map_id;
							level_insertion->survival_presence_context_ids[i] = scenario_insertion->survival_presence_context_id;
							level_insertion->insertion_point_initial_zone_set[i] = uns8(scenario_insertion->zone_set);
							level_insertion->__flagsB9C[i] = scenario_insertion->flags;
						}

						insertion_count++;
					}
				}
				break;
				default:
					UNREACHABLE();
					break;
				}

				level_insertion->map_id = level->map_id;
				level_insertion->insertion_point_count = insertion_count;
			}
		}

		if (dlc_content)
		{
			csnzprintf(level->scenario_file, sizeof(level->scenario_file), "%ls\\%s", source_directory_path, level_data->scenario_file);
			csnzprintf(level->image_file_base, sizeof(level->image_file_base), "%ls\\%s", source_directory_path, level_data->image_file_base);
		}
		else
		{
			csnzprintf(level->scenario_file, sizeof(level->scenario_file), "%s%s", cache_files_map_directory(), level_data->scenario_file);
			csnzprintf(level->image_file_base, sizeof(level->image_file_base), "%simages\\%s", cache_files_map_directory(), level_data->image_file_base);
		}
	}
}

bool __cdecl levels_begin_dlc_enumeration()
{
	//bool result = false;
	//HOOK_INVOKE(result =, levels_begin_dlc_enumeration);
	//return result;

	//ASSERT(!g_level_globals.enumeration_in_progress());

	s_dlc_enumeration_task task{};
	task.data.controller_index = _controller0;
	task.data.stage = _dlc_begin_next_content_catalogue_stage;
	task.data.content_item_index = NONE;
	task.data.enumeration_data = &g_level_globals.find_files_data;
	g_level_globals.enumeration_task = async_task_add(_async_priority_important_non_blocking, &task._force_size, _async_category_saved_games, levels_dlc_enumeration_callback, &g_level_globals.finished);
	return g_level_globals.enumeration_task != NONE;
}

bool __cdecl levels_begin_dvd_enumeration()
{
	//bool result = false;
	//HOOK_INVOKE(result =, levels_begin_dvd_enumeration);
	//return result;

	if (shell_application_type() == _shell_application_game)
	{
		s_configuration_enumeration_task task{};
		task.data.stage = _dvd_find_files_start_stage;
		task.data.enumeration_data = &g_level_globals.find_files_data;
		g_level_globals.enumeration_task = async_task_add(_async_priority_important_non_blocking, &task._force_size, _async_category_saved_games, levels_dvd_enumeration_callback, &g_level_globals.finished);

		if (g_level_globals.enumeration_task != NONE)
		{
			g_level_globals.need_to_enumerate_dvd = false;
			return true;
		}
	}

	return false;
}

void __cdecl levels_dispose()
{
	//INVOKE(0x0054ADF0, levels_dispose);

	if (levels_enumeration_in_progress())
		internal_async_yield_until_done(&g_level_globals.finished, false, false, __FILE__, __LINE__);

	g_level_globals.initialized = false;

	data_make_invalid(g_level_globals.campaigns);
	data_make_invalid(g_level_globals.campaign_levels);
	data_make_invalid(g_level_globals.campaign_insertions);
	data_make_invalid(g_level_globals.multiplayer_levels);

	if (g_level_globals.campaigns)
		data_dispose(g_level_globals.campaigns);

	if (g_level_globals.campaign_levels)
		data_dispose(g_level_globals.campaign_levels);

	if (g_level_globals.campaign_insertions)
		data_dispose(g_level_globals.campaign_insertions);

	if (g_level_globals.multiplayer_levels)
		data_dispose(g_level_globals.multiplayer_levels);
}

void __cdecl levels_dispose_from_old_map()
{
	INVOKE(0x0054AEA0, levels_dispose_from_old_map);
}

e_async_completion __cdecl levels_dlc_enumeration_callback(s_async_task* work)
{
	e_async_completion result = _async_completion_retry;
	HOOK_INVOKE(result =, levels_dlc_enumeration_callback, work);
	return result;

	//s_file_reference found_file{};
	//switch (work->dlc_enumeration_task.stage)
	//{
	//case _dlc_begin_next_content_catalogue_stage:
	//{
	//	c_content_catalogue* content_catalogue = content_catalogue_get_interface(work->dlc_enumeration_task.controller_index);
	//	work->dlc_enumeration_task.content_item_index = NONE;
	//	if (content_catalogue->valid())
	//	{
	//		work->dlc_enumeration_task.stage = _dlc_find_next_content_catalogue_stage;
	//	}
	//	else if (!content_catalogue->busy())
	//	{
	//		work->dlc_enumeration_task.stage = _dlc_find_next_content_item;
	//	}
	//}
	//break;
	//case _dlc_find_next_content_item:
	//{
	//	c_content_catalogue* content_catalogue = content_catalogue_get_interface(work->dlc_enumeration_task.controller_index);
	//	if (work->dlc_enumeration_task.content_item_index == NONE)
	//		work->dlc_enumeration_task.content_item_index = content_catalogue->first_content_item_index();
	//	else
	//		work->dlc_enumeration_task.content_item_index = content_catalogue->next_content_item_index(work->dlc_enumeration_task.content_item_index);
	//
	//	if (work->dlc_enumeration_task.content_item_index == NONE)
	//	{
	//		work->dlc_enumeration_task.stage = _dlc_find_files_start_stage;
	//		break;
	//	}
	//
	//	if (content_catalogue->content_item_get_game_content_type(work->dlc_enumeration_task.content_item_index) ||
	//		!content_catalogue->content_item_mount(work->dlc_enumeration_task.content_item_index, true))
	//	{
	//		work->dlc_enumeration_task.stage = _dlc_find_next_content_item;
	//		break;
	//	}
	//
	//	work->dlc_enumeration_task.stage = _dlc_find_files_start_stage;
	//}
	//break;
	//case _dlc_find_files_start_stage:
	//{
	//	c_content_catalogue* content_catalogue = content_catalogue_get_interface(work->dlc_enumeration_task.controller_index);
	//
	//	if (content_catalogue->content_item_get_directory_file_reference(work->dlc_enumeration_task.content_item_index, &found_file))
	//	{
	//		find_files_start(work->dlc_enumeration_task.enumeration_data, 0, &found_file);
	//		work->dlc_enumeration_task.stage = _dlc_find_next_file_stage;
	//	}
	//	else
	//	{
	//		content_catalogue->content_item_unmount(work->dlc_enumeration_task.content_item_index, true);
	//		work->dlc_enumeration_task.stage = _dlc_find_next_content_item;
	//	}
	//}
	//break;
	//case _dlc_find_next_file_stage:
	//{
	//	c_content_catalogue* content_catalogue = content_catalogue_get_interface(work->dlc_enumeration_task.controller_index);
	//
	//	s_file_reference file{};
	//	s_file_last_modification_date date{};
	//	if (!find_files_next(work->dlc_enumeration_task.enumeration_data, &file, &date))
	//	{
	//		find_files_end(work->dlc_enumeration_task.enumeration_data);
	//		content_catalogue->content_item_unmount(work->dlc_enumeration_task.content_item_index, true);
	//		work->dlc_enumeration_task.stage = _dlc_find_next_content_item;
	//		break;
	//	}
	//
	//	wchar_t file_directory[256]{};
	//	wchar_t file_extension[256]{};
	//	wchar_t file_name_with_extension[256]{};
	//
	//	file_reference_get_name_wide(&file, FLAG(_name_directory_bit), file_directory, NUMBEROF(file_directory));
	//	file_reference_get_name_wide(&file, FLAG(_name_extension_bit), file_extension, NUMBEROF(file_extension));
	//	file_reference_get_name_wide(&file, FLAG(_name_file_bit) | FLAG(_name_extension_bit), file_name_with_extension, NUMBEROF(file_name_with_extension));
	//
	//	if (ustricmp(file_extension, L"campaign") == 0)
	//	{
	//		levels_process_campaign_configuration_file(&file, file_directory, false);
	//	}
	//	else if (ustricmp(file_extension, L"mapinfo") == 0)
	//	{
	//		levels_process_level_configuration_file(&file, file_directory, false);
	//	}
	//}
	//break;
	//case _dlc_find_next_content_catalogue_stage:
	//{
	//	work->dlc_enumeration_task.controller_index = get_first_signed_in_controller_index();
	//	if (VALID_CONTROLLER(work->dlc_enumeration_task.controller_index))
	//	{
	//		work->dlc_enumeration_task.stage = _dlc_begin_next_content_catalogue_stage;
	//		break;
	//	}
	//
	//	return _async_completion_done;
	//}
	//break;
	//}
	//
	//return _async_completion_retry;
}

e_async_completion __cdecl levels_dvd_enumeration_callback(s_async_task* work)
{
	c_static_string<256> found_file_name{};
	s_file_reference found_file{};

	s_configuration_enumeration_task* task = (s_configuration_enumeration_task*)work;

	switch (task->data.stage)
	{
	case _dvd_find_files_start_stage:
	{
		found_file_name.append_print("%sinfo", cache_files_map_directory());
		file_reference_create_from_path(&found_file, found_file_name.get_string(), true);
		find_files_start(task->data.enumeration_data, 0, &found_file);
		task->data.stage = _dvd_find_next_file_stage;
	}
	break;
	case _dvd_find_next_file_stage:
	{
		s_file_reference file{};
		s_file_last_modification_date date{};
		if (!find_files_next(task->data.enumeration_data, &file, &date))
		{
			find_files_end(task->data.enumeration_data);
			return _async_completion_done;
		}

		wchar_t file_directory[256]{};
		wchar_t name_file[256]{};
		wchar_t file_extension[256]{};
		wchar_t file_name_with_extension[256]{};

		file_reference_get_name_wide(&file, FLAG(_name_directory_bit), file_directory, NUMBEROF(file_directory));
		file_reference_get_name_wide(&file, FLAG(_name_file_bit), name_file, NUMBEROF(name_file));
		file_reference_get_name_wide(&file, FLAG(_name_extension_bit), file_extension, NUMBEROF(file_extension));
		file_reference_get_name_wide(&file, FLAG(_name_file_bit) | FLAG(_name_extension_bit), file_name_with_extension, NUMBEROF(file_name_with_extension));

		found_file_name.append_print("%ls", file_name_with_extension);

		if (ustricmp(file_extension, L"campaign") == 0)
		{
			levels_process_campaign_configuration_file(&file, file_directory, false);
		}
		else if (ustricmp(file_extension, L"mapinfo") == 0)
		{
			static bool x_show_all_maps = false;
			if (!x_show_all_maps)
			{
				s_file_reference map_file{};
				file_reference_create_from_path(&map_file, c_string_builder("%s%ls.map", cache_files_map_directory(), name_file).get_string(), false);
				if (!file_exists(&map_file))
				{
					break;
				}
			}

			levels_process_level_configuration_file(&file, file_directory, false);
		}
	}
	break;
	}

	return _async_completion_retry;
}

bool __cdecl levels_enumeration_in_progress()
{
	//return INVOKE(0x0054B3A0, levels_enumeration_in_progress);

	return g_level_globals.enumeration_task != NONE && !g_level_globals.finished.peek();
}

//.text:0054B3C0 ; 

wchar_t* __cdecl levels_get_active_session_map_name(wchar_t* map_name_buffer, int32 maximum_characters)
{
	return INVOKE(0x0054B400, levels_get_active_session_map_name, map_name_buffer, maximum_characters);
}

//.text:0054B500 ; wchar_t* __cdecl levels_get_active_session_campaign_name(wchar_t*, int32)

uns32 __cdecl levels_get_available_map_mask()
{
	return INVOKE(0x0054B610, levels_get_available_map_mask);

	//uns32 available_map_mask = 0;
	//
	//int32 map_ids[32]{};
	//int32 count = NUMBEROF(map_ids);
	//levels_get_multiplayer_map_ids(map_ids, &count);
	//for (int32 i = 0; i < count; i++)
	//{
	//	int32 map_index = network_configuration_get_map_index_from_map_id(map_ids[i]);
	//	if (map_index != NONE)
	//		SET_BIT(available_map_mask, map_index, true);
	//}
	//
	//return available_map_mask;
}

int32 __cdecl levels_get_campaign_count()
{
	//return INVOKE(0x0054B670, levels_get_campaign_count);

	return g_level_globals.initialized && !levels_enumeration_in_progress() ? g_level_globals.campaigns->actual_count : 0;
}

e_campaign_id __cdecl levels_get_campaign_id_from_path(const char* path)
{
	return INVOKE(0x0054B6A0, levels_get_campaign_id_from_path, path);
}

int32 __cdecl levels_get_campaign_level_count(e_campaign_id campaign_id)
{
	return INVOKE(0x0054B7B0, levels_get_campaign_level_count, campaign_id);
}

int16 __cdecl levels_get_campaign_level_index(e_campaign_id campaign_id, e_map_id map_id)
{
	return INVOKE(0x0054B8A0, levels_get_campaign_level_index, campaign_id, map_id);
}

e_map_id __cdecl levels_get_campaign_map_by_display_name(wchar_t* display_name)
{
	return INVOKE(0x0054B9D0, levels_get_campaign_map_by_display_name, display_name);
}

void __cdecl levels_get_campaign_map_ids(e_campaign_id campaign_id, e_map_id* out_map_ids, int32* in_out_count)
{
	INVOKE(0x0054BAA0, levels_get_campaign_map_ids, campaign_id, out_map_ids, in_out_count);
}

e_map_id __cdecl levels_get_campaign_next_map_id(e_campaign_id campaign_id, e_map_id map_id)
{
	return INVOKE(0x0054BB80, levels_get_campaign_next_map_id, campaign_id, map_id);
}

e_map_id levels_get_default_multiplayer_map_id()
{
	//return INVOKE(0x0054BCA0, levels_get_default_multiplayer_map_id);

	e_map_id map_id = _map_id_none;

	if (g_level_globals.initialized)
	{
		c_critical_section_scope critical_section_scope(k_crit_section_levels);
		c_data_iterator<s_level_datum> level_iter{};

		level_iter.begin(g_level_globals.multiplayer_levels);
		while (level_iter.next())
		{
			s_level_datum* level = level_iter.get_datum();
			if (level->map_id > 0 && levels_get_multiplayer_map_is_allowed(level->map_id))
			{
				if (map_id == _map_id_none || map_id > level->map_id)
					map_id = level->map_id;
			}
		}
	}

	return map_id;
}

uns32 __cdecl levels_get_checksum()
{
	//return INVOKE(0x0054BDB0, levels_get_checksum);

	return g_level_globals.checksum;
}

//.text:0054BDC0 ; levels_get_campaign_insertion_zone_set_by_map_id_and_index?

e_map_id __cdecl levels_get_multiplayer_map_by_display_name(const wchar_t* display_name)
{
	//return INVOKE(0x0054BE10, levels_get_multiplayer_map_by_display_name, display_name);

	if (g_level_globals.initialized)
	{
		c_critical_section_scope critical_section_scope(k_crit_section_levels);
		c_data_iterator<s_level_datum> level_iter{};

		level_iter.begin(g_level_globals.multiplayer_levels);
		while (level_iter.next())
		{
			s_level_datum* level = level_iter.get_datum();
			if (ustrnicmp(display_name, level->name, NUMBEROF(level->name)))
				return level->map_id;
		}
	}

	return _map_id_none;
}

void __cdecl levels_get_multiplayer_map_ids(e_map_id* out_map_ids, int32* in_out_count)
{
	//INVOKE(0x0054BEE0, levels_get_multiplayer_map_ids, out_map_ids, in_out_count);

	ASSERT(out_map_ids != nullptr);
	ASSERT(in_out_count != nullptr);

	int32 count = 0;
	if (g_level_globals.initialized)
	{
		c_critical_section_scope critical_section_scope(k_crit_section_levels);
		c_data_iterator<s_level_datum> level_iter{};

		int32 maximum_count = *in_out_count;
		ASSERT(maximum_count > 0);
		csmemset(out_map_ids, NONE, sizeof(int32) * maximum_count);

		level_iter.begin(g_level_globals.multiplayer_levels);
		while (level_iter.next() && count < maximum_count)
		{
			e_map_id map_id = level_iter.get_datum()->map_id;
			bool has_map_id = true;

			for (int32 i = 0; i < count; i++)
				has_map_id = map_id != out_map_ids[i];

			if (has_map_id)
				out_map_ids[count++] = map_id;
		}
	}
	*in_out_count = count;
}

bool __cdecl levels_get_multiplayer_map_is_allowed(e_map_id map_id)
{
	//return INVOKE(0x0054BFD0, levels_get_multiplayer_map_is_allowed, map_id);

	if (get_alpha_is_external_beta())
		return map_id == 310 || map_id == 340 || map_id == 360;

	if (get_alpha_is_internal_beta())
		return map_id == 310 || map_id == 340 || map_id == 360 || map_id == 350 || map_id == 30;

	return true;
}

bool __cdecl levels_find_path(s_data_array* data, e_map_id map_id, char* path, int32 maximum_characters)
{
	c_data_iterator<s_level_datum> level_iter{};

	level_iter.begin(data);
	while (level_iter.next())
	{
		s_level_datum* level = level_iter.get_datum();
		if (level->map_id == map_id)
		{
			csstrnzcpy(path, level->scenario_file, maximum_characters);
			return true;
		}
	}

	return false;
}

char* __cdecl levels_get_path(e_campaign_id campaign_id, e_map_id map_id, char* path, int32 maximum_characters)
{
	//return INVOKE(0x0054C040, levels_get_path, campaign_id, map_id, path, maximum_characters);

	ASSERT(path != nullptr);
	ASSERT(maximum_characters > 0);

	c_critical_section_scope critical_section_scope(k_crit_section_levels);

	if (map_id == _map_id_mainmenu)
	{
		csstrnzcpy(path, g_level_globals.main_menu.scenario_file, maximum_characters);
	}
	else if (campaign_id == _campaign_id_none)
	{
		levels_find_path(g_level_globals.multiplayer_levels, map_id, path, maximum_characters);
	}
	else
	{
		levels_find_path(g_level_globals.campaign_levels, map_id, path, maximum_characters);
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

	data_make_valid(g_level_globals.campaigns);
	data_make_valid(g_level_globals.campaign_levels);
	data_make_valid(g_level_globals.campaign_insertions);
	data_make_valid(g_level_globals.multiplayer_levels);

	g_level_globals.main_menu.flags.clear();
	g_level_globals.main_menu.flags.set(_level_is_main_menu_bit, true);
	g_level_globals.main_menu.map_id = _map_id_mainmenu;
	csnzprintf(g_level_globals.main_menu.scenario_file, sizeof(g_level_globals.main_menu.scenario_file), "%s%s", cache_files_map_directory(), "mainmenu");

	g_level_globals.initialized = true;
	g_level_globals.preorder_unlock_controller_mask = 0;
	g_level_globals.need_to_enumerate_dvd = true;
	g_level_globals.checksum = NONE;

	levels_begin_dvd_enumeration();
	levels_update();
}

void __cdecl levels_initialize_for_new_map()
{
	INVOKE(0x0054C2D0, levels_initialize_for_new_map);
}

//.text:0054C2E0 ; 

bool __cdecl levels_map_id_is_fake(e_map_id map_id)
{
	//bool result = false;
	//HOOK_INVOKE(result =, levels_map_id_is_fake, map_id);
	//return result;

	return map_id == _map_id_use_scenario_path;
}

void __cdecl levels_open_dlc(const char* scenario_path, bool blocking)
{
	INVOKE(0x0054C330, levels_open_dlc, scenario_path, blocking);
}

bool __cdecl levels_path_is_dlc(const char* scenario_path)
{
	return INVOKE(0x0054C360, levels_path_is_dlc, scenario_path);
}

void __cdecl levels_process_campaign_configuration_file(s_file_reference* file, const wchar_t* source_directory_path, bool dlc_content)
{
	//HOOK_INVOKE(, levels_process_campaign_configuration_file, file, source_directory_path, dlc_content);

	static char file_buffer[sizeof(s_blf_chunk_campaign)]{};
	const s_blf_chunk_campaign* campaign = nullptr;
	bool must_byte_swap = false;

	levels_find_campaign_chunk(file, file_buffer, &campaign, &must_byte_swap);
	if (campaign)
		levels_add_campaign_from_configuration_file(campaign, must_byte_swap, source_directory_path, dlc_content);
}

void __cdecl levels_process_level_configuration_file(s_file_reference* file, const wchar_t* source_directory_path, bool dlc_content)
{
	//HOOK_INVOKE(, levels_process_level_configuration_file, file, source_directory_path, dlc_content);

	static char file_buffer[sizeof(s_blf_chunk_scenario_atlas)]{};
	const s_blf_chunk_scenario* level = nullptr;
	bool must_byte_swap = false;

	levels_find_scenario_chunk(file, file_buffer, &level, &must_byte_swap);
	if (level)
		levels_add_level_from_configuration_file(level, must_byte_swap, source_directory_path, dlc_content);
}

void __cdecl levels_remove_dlc()
{
	//INVOKE(0x0054C820, levels_remove_dlc);

	c_data_iterator<s_campaign_datum> campaign_iter{};
	c_data_iterator<s_level_datum> campaign_level_iter{};
	c_data_iterator<s_level_datum> multiplayer_level_iter{};

	campaign_iter.begin(g_level_globals.campaigns);
	while (campaign_iter.next())
	{
		if (campaign_iter.get_datum()->flags.test(_campaign_from_dlc_bit))
			datum_delete(g_level_globals.campaigns, campaign_iter.get_index());
	}

	campaign_level_iter.begin(g_level_globals.campaign_levels);
	while (campaign_level_iter.next())
	{
		if (campaign_level_iter.get_datum()->flags.test(_level_from_dlc_bit))
			datum_delete(g_level_globals.campaign_levels, campaign_level_iter.get_index());
	}

	multiplayer_level_iter.begin(g_level_globals.multiplayer_levels);
	while (multiplayer_level_iter.next())
	{
		if (multiplayer_level_iter.get_datum()->flags.test(_level_from_dlc_bit))
			datum_delete(g_level_globals.multiplayer_levels, multiplayer_level_iter.get_index());
	}
}

bool __cdecl levels_try_and_get_by_map_id(s_data_array* data, e_map_id map_id, s_level_datum* level)
{
	//return INVOKE(0x0054C910, levels_try_and_get_by_map_id, data, map_id, level);

	if (g_level_globals.initialized && map_id != NONE)
	{
		c_critical_section_scope critical_section_scope(k_crit_section_levels);
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

bool __cdecl levels_try_and_get_campaign_insertion(e_map_id map_id, s_level_insertion_datum* insertion)
{
	//return INVOKE(0x0054C9E0, levels_try_and_get_campaign_insertion, map_id, insertion);

	ASSERT(insertion);

	if (g_level_globals.initialized && map_id != NONE)
	{
		c_critical_section_scope critical_section_scope(k_crit_section_levels);
		c_data_iterator<s_level_insertion_datum> insertion_iter{};

		insertion_iter.begin(g_level_globals.campaign_insertions);
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

bool __cdecl levels_try_and_get_campaign_map(e_map_id map_id, s_level_datum* level)
{
	//return INVOKE(0x0054CAB0, levels_try_and_get_campaign_map, map_id, level);

	ASSERT(level != nullptr);

	return levels_try_and_get_by_map_id(g_level_globals.campaign_levels, map_id, level);
}

bool __cdecl levels_try_and_get_main_menu_map(s_level_datum* level)
{
	//return INVOKE(0x0054CAD0, levels_try_and_get_main_menu_map, level);

	ASSERT(level != nullptr);

	c_critical_section_scope critical_section_scope(k_crit_section_levels);
	*level = g_level_globals.main_menu;

	return true;
}

bool __cdecl levels_try_and_get_multiplayer_map(e_map_id map_id, s_level_datum* level)
{
	//return INVOKE(0x0054CB00, levels_try_and_get_multiplayer_map, map_id, level);

	ASSERT(level != nullptr);

	return levels_try_and_get_by_map_id(g_level_globals.multiplayer_levels, map_id, level);
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
	INVOKE(0x0054CC00, sub_54CC00);
}

//.text:0054CC10 ; 
//.text:0054CC20 ; 
//.text:0054CC40 ; 
//.text:0054CC60 ; 
//.text:0054CC80 ; 
//.text:0054CCA0 ; 
//.text:0054CCC0 ; 

void levels_find_campaign_chunk(s_file_reference* file, char* const file_buffer, const s_blf_chunk_campaign** out_campaign, bool* must_byte_swap)
{
	*out_campaign = nullptr;
	*must_byte_swap = false;

	bool file_added = false;
	uns32 error = 0;
	uns32 file_size = 0;
	int32 chunk_size = 0;
	const char* found_chunk = nullptr;
	bool eof_chunk = false;
	const s_blf_chunk_campaign* campaign = nullptr;
	bool byte_swap = false;

	if (!file_open(file, FLAG(_permission_read_bit), &error))
	{
		event(_event_warning, "levels: failed to open campaign info file");
		file_close(file);
	}
	else if (!file_get_size(file, &file_size))
	{
		event(_event_warning, "levels: failed to get file size of campaign info file");
	}
	else if (file_size > 0x18AC)
	{
		// .campaign file size
		// - 0x1459, Halo 3 to Halo Reach
		// - 0x18AC, Specific to Halo Online?
		// - 0x1C19, Halo 4

		event(_event_warning, "levels: unexpected file size for campaign info file");
	}
	else if (!file_read(file, file_size, 0, file_buffer))
	{
		event(_event_warning, "levels: failed to read campaign info file");
	}
	else if (!network_blf_verify_start_of_file(file_buffer, file_size, &byte_swap, &chunk_size))
	{
		event(_event_warning, "levels: failed to verify blf start of file for campaign info file");
	}
	else if (!network_blf_find_chunk(file_buffer, file_size, byte_swap, s_blf_chunk_campaign::k_chunk_type, s_blf_chunk_campaign::k_chunk_major_version, &chunk_size, &found_chunk, nullptr, nullptr, &eof_chunk))
	{
		event(_event_warning, "levels: failed to find blf campaign chunk");
	}
	else if (found_chunk != nullptr)
	{
		campaign = reinterpret_cast<const s_blf_chunk_campaign*>(found_chunk - sizeof(s_blf_header));
		if (found_chunk != (const char*)sizeof(s_blf_header) && network_blf_find_chunk(file_buffer, file_size, byte_swap, s_blf_chunk_end_of_file::k_chunk_type, s_blf_chunk_end_of_file::k_chunk_major_version, &chunk_size, &found_chunk, nullptr, nullptr, &eof_chunk))
		{
			if (found_chunk)
			{
				if (chunk_size == sizeof(s_blf_chunk_end_of_file_with_rsa) && network_blf_verify_end_of_file(file_buffer, file_size, byte_swap, found_chunk - sizeof(s_blf_header), _blf_file_authentication_type_rsa))
				{
					file_added = true;
				}
				else
				{
					event(_event_warning, "levels: failed to find expected blf end of file chunk");
				}
			}
			else
			{
				event(_event_warning, "levels: failed to verify blf end of file chunk");
			}
		}
	}

	if (file_added)
	{
		*out_campaign = campaign;
		*must_byte_swap = byte_swap;
		file_close(file);
	}
	else
	{
		// $TODO file_reference_get_name
		event(_event_warning, "levels: failed to add campaign file '%s'", file->path);
	}
}

void levels_find_scenario_chunk(s_file_reference* file, char* const file_buffer, const s_blf_chunk_scenario** out_scenario, bool* must_byte_swap)
{
	*out_scenario = nullptr;
	*must_byte_swap = false;

	bool file_added = false;
	uns32 error = 0;
	uns32 file_size = 0;
	int32 chunk_size = 0;
	const char* chunk_buffer = nullptr;
	bool eof_chunk = false;
	const s_blf_chunk_scenario* scenario = nullptr;
	bool byte_swap = false;

	if (!file_open(file, FLAG(_file_open_flag_desired_access_read), &error))
	{
		event(_event_warning, "levels: failed to open level info file");
		file_close(file);
		goto function_end;
	}

	if (!file_get_size(file, &file_size))
	{
		event(_event_warning, "levels: failed to get file size of level info file");
		goto function_finish;
	}

	// .mapinfo file size
	// - 0x4E91, Halo 3
	// - 0x9A01, Halo 3: ODST
	// - 0xCDD9, Halo Reach
	// - 0x11F19, Halo 4
	if (file_size > 0xCDD9)
	{
		event(_event_warning, "levels: unexpected file size for level info file");
		goto function_finish;
	}

	if (!file_read(file, file_size, false, file_buffer))
	{
		event(_event_warning, "levels: failed to read level info file");
		goto function_finish;
	}

	if (!network_blf_verify_start_of_file(file_buffer, file_size, &byte_swap, &chunk_size))
	{
		event(_event_warning, "levels: failed to verify blf start of file");
		goto function_finish;
	}

	if (!network_blf_find_chunk(file_buffer, file_size, byte_swap, s_blf_chunk_scenario::k_chunk_type, s_blf_chunk_scenario::k_chunk_major_version, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
	{
		event(_event_warning, "levels: failed to find blf scenario chunk");
		goto function_finish;
	}

	if (chunk_buffer)
	{
		scenario = reinterpret_cast<const s_blf_chunk_scenario*>(chunk_buffer - sizeof(s_blf_header));
		if (chunk_buffer != (const char*)sizeof(s_blf_header) && network_blf_find_chunk(file_buffer, file_size, byte_swap, s_blf_chunk_end_of_file::k_chunk_type, s_blf_chunk_end_of_file::k_chunk_major_version, &chunk_size, &chunk_buffer, nullptr, nullptr, &eof_chunk))
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

			event(_event_warning, "levels: failed to verify blf end of file chunk");
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
		uns32 flags = FLAG(0) | FLAG(2) | FLAG(3);
		char filename[256]{};
		file_reference_get_name(file, flags, filename, NUMBEROF(filename));
		event(_event_warning, "levels: failed to add level file '%s'", file->path.get_string());
	}
}

