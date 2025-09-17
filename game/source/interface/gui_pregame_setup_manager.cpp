#include "interface/gui_pregame_setup_manager.hpp"

#include "interface/c_controller.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/gui_selected_items_difficulty.hpp"
#include "interface/gui_selected_items_game_variant.hpp"
#include "interface/gui_selected_items_hopper.hpp"
#include "interface/gui_selected_items_level.hpp"
#include "interface/gui_selected_items_map.hpp"
#include "interface/gui_selected_items_network_mode.hpp"
#include "interface/gui_selected_items_saved_film.hpp"
#include "interface/gui_selected_items_saved_screenshot.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_error_manager.hpp"
#include "interface/user_interface_matchmaking.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_utilities.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "main/levels.hpp"
#include "memory/module.hpp"
#include "networking/online/online.hpp"
#include "saved_games/saved_film_manager.hpp"

REFERENCE_DECLARE(0x0191D750, c_gui_pregame_setup_manager, g_gui_pregame_setup_manager);

HOOK_DECLARE_CLASS_MEMBER(0x00AA7DB0, c_gui_pregame_setup_manager, set_selected_item_);

void user_interface_squad_set_campaign_progress_hook_for_restore_game_setup_internal(e_campaign_id campaign_id, e_map_id map_id, int16 campaign_inserion_point, s_campaign_armaments* player_armaments, s_campaign_game_progression* game_progression, s_hub_progression* hub_progression)
{
	user_interface_squad_set_campaign_progress(campaign_id, _map_id_first, campaign_inserion_point, player_armaments, game_progression, hub_progression);
}
HOOK_DECLARE_CALL(0x00AA7570, user_interface_squad_set_campaign_progress_hook_for_restore_game_setup_internal);

void __thiscall c_gui_pregame_setup_manager::set_selected_item_(e_controller_index controller_index, const c_gui_selected_item* selected_item, bool selected_from_recent_list, int32 source_screen_index_to_dispose)
{
	switch (selected_item->m_selection_type)
	{
	//case _gui_selection_type_none:
	//case _gui_selection_type_level:
	//case _gui_selection_type_difficulty:
	case _gui_selection_type_map:
	case _gui_selection_type_game_variant:
	//case _gui_selection_type_hopper:
	//case _gui_selection_type_film:
	//case _gui_selection_type_screenshot:
	//case _gui_selection_type_network_mode:
	{
		HOOK_INVOKE_CLASS_MEMBER(, c_gui_pregame_setup_manager, set_selected_item_, controller_index, selected_item, selected_from_recent_list, source_screen_index_to_dispose);
	}
	return;
	}

	ASSERT(selected_item);

	s_gui_single_game_setup_storage game_setup_storage{};
	game_setup_storage.game_mode = user_interface_squad_get_ui_game_mode();

	bool valid = false;

	switch (selected_item->m_selection_type)
	{
	case _gui_selection_type_level:
	{
		valid = set_selected_item_type_level(controller_index, (const c_gui_level_selected_item*)selected_item, selected_from_recent_list, source_screen_index_to_dispose, game_setup_storage);
	}
	break;
	case _gui_selection_type_difficulty:
	{
		valid = set_selected_item_type_difficulty(controller_index, (const c_gui_difficulty_selected_item*)selected_item, selected_from_recent_list, source_screen_index_to_dispose, game_setup_storage);
	}
	break;
	case _gui_selection_type_map:
	{
		valid = set_selected_item_type_map(controller_index, (const c_gui_map_selected_item*)selected_item, selected_from_recent_list, source_screen_index_to_dispose, game_setup_storage);
	}
	break;
	case _gui_selection_type_game_variant:
	{
		valid = set_selected_item_type_game_variant(controller_index, (const c_gui_game_variant_selected_item*)selected_item, selected_from_recent_list, source_screen_index_to_dispose, game_setup_storage);
	}
	break;
	case _gui_selection_type_hopper:
	{
		valid = set_selected_item_type_hopper(controller_index, (const c_gui_hopper_selected_item*)selected_item, selected_from_recent_list, source_screen_index_to_dispose, game_setup_storage);
	}
	break;
	case _gui_selection_type_film:
	{
		valid = set_selected_item_type_film(controller_index, (const c_gui_saved_film_selected_item*)selected_item, selected_from_recent_list, source_screen_index_to_dispose, game_setup_storage);
	}
	break;
	case _gui_selection_type_screenshot:
	{
		valid = set_selected_item_type_screenshot(controller_index, (const c_gui_saved_screenshot_selected_item*)selected_item, selected_from_recent_list, source_screen_index_to_dispose, game_setup_storage);
	}
	break;
	case _gui_selection_type_network_mode:
	{
		if (!(valid = set_selected_item_type_network_mode(controller_index, (const c_gui_network_mode_selected_item*)selected_item, selected_from_recent_list, source_screen_index_to_dispose, game_setup_storage)))
		{
			return;
		}
	}
	break;
	default:
	{
		UNREACHABLE();
	}
	break;
	}

	if (valid)
	{
		global_preferences_update_last_game_setup(&game_setup_storage);
	}

	if (source_screen_index_to_dispose != NONE)
	{
		c_gui_screen_widget* source_screen = window_manager_get()->get_screen_by_screen_index(source_screen_index_to_dispose);
		if (source_screen)
		{
			source_screen->transition_out(_transition_out_normal);
		}
	}
}

void c_gui_pregame_setup_manager::change_advertisement_mode(e_gui_network_session_advertisement_mode advertisement_mode, e_network_session_class current_session_class, e_network_session_class desired_session_class, int32 source_screen_index_to_dispose)
{
	INVOKE_CLASS_MEMBER(0x00AA6B90, c_gui_pregame_setup_manager, change_advertisement_mode, advertisement_mode, current_session_class, desired_session_class, source_screen_index_to_dispose);
}

c_gui_pregame_setup_manager* __cdecl c_gui_pregame_setup_manager::get()
{
	//return INVOKE(0x00AA6C80, get);

	return &g_gui_pregame_setup_manager;
}

//.text:00AA6C90 ; 
//.text:00AA6CA0 ; 
//.text:00AA6CB0 ; 
//.text:00AA6CC0 ; 
//.text:00AA6CF0 ; 
//.text:00AA6D00 ; 
//.text:00AA6D10 ; 
//.text:00AA6D20 ; 
//.text:00AA6D30 ; 
//.text:00AA6D40 ; 
//.text:00AA6D50 ; 
//.text:00AA6D60 ; 
//.text:00AA6D70 ; 
//.text:00AA6D80 ; 
//.text:00AA6D90 ; 
//.text:00AA6DA0 ; public: int32 c_gui_pregame_setup_manager::get_selected_item_category_id(e_gui_selected_item_type)

bool __cdecl c_gui_pregame_setup_manager::get_selected_item_title(wchar_t* buffer, int32 buffer_count, e_gui_selected_item_type selection_type)
{
	return INVOKE(0x00AA6EA0, c_gui_pregame_setup_manager::get_selected_item_title, buffer, buffer_count, selection_type);
}

//.text:00AA70C0 ; 
//.text:00AA70D0 ; public: bool c_gui_pregame_setup_manager::handle_dialog_result(const c_dialog_result_message*)
//.text:00AA7130 ; public: bool c_gui_pregame_setup_manager::initialize_game_mode(e_gui_game_mode, bool)

void c_gui_pregame_setup_manager::restore_from_last(e_gui_game_mode lobby)
{
	INVOKE_CLASS_MEMBER(0x00AA7190, c_gui_pregame_setup_manager, restore_from_last, lobby);
}

void c_gui_pregame_setup_manager::restore_game_setup(const s_gui_single_game_setup_storage* storage)
{
	//INVOKE_CLASS_MEMBER(0x00AA7270, c_gui_pregame_setup_manager, restore_game_setup, storage);

	ASSERT(storage);

	csmemcpy(&m_storage_to_set_on_next_update, storage, sizeof(m_storage_to_set_on_next_update));
	m_start_countdown_when_ready = false;
}

//.text:00AA72A0 ; private: void c_gui_pregame_setup_manager::restore_game_setup_internal(e_gui_game_mode, s_gui_game_setup_storage*)
//.text:00AA7B00 ; public: static void __cdecl c_gui_pregame_setup_manager::save_game_setup(const s_gui_game_setup_storage*, s_gui_single_game_setup_storage*)
//.text:00AA7D90 ; 

void c_gui_pregame_setup_manager::set_selected_item(e_controller_index controller_index, const c_gui_selected_item* selected_item, bool selected_from_recent_list, int32 source_screen_index_to_dispose)
{
	INVOKE_CLASS_MEMBER(0x00AA7DB0, c_gui_pregame_setup_manager, set_selected_item, controller_index, selected_item, selected_from_recent_list, source_screen_index_to_dispose);
}

bool c_gui_pregame_setup_manager::set_selected_item_type_difficulty(e_controller_index controller_index, const c_gui_difficulty_selected_item* difficulty_selected_item, bool selected_from_recent_list, int32& source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage)
{
	const s_gui_game_setup_storage* last_game_setup = global_preferences_get_last_game_setup();

	if (!user_interface_squad_set_campaign_difficulty(difficulty_selected_item->m_difficulty))
	{
		user_interface_play_sound(_ui_global_sound_effect_failure, NONE);
		return false;
	}

	game_setup_storage.campaign = *last_game_setup->get_campaign();
	game_setup_storage.campaign.difficulty = difficulty_selected_item->m_difficulty;
	m_selected_from_recent_list[difficulty_selected_item->m_selection_type] = selected_from_recent_list;
	return true;
}

bool c_gui_pregame_setup_manager::set_selected_item_type_film(e_controller_index controller_index, const c_gui_saved_film_selected_item* film_selected_item, bool selected_from_recent_list, int32 & source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage)
{
	const s_gui_game_setup_storage* last_game_setup = global_preferences_get_last_game_setup();

	csmemcpy(&game_setup_storage.theater, last_game_setup->get_theater(), sizeof(s_gui_game_setup_storage::s_theater_settings));
	ASSERT(film_selected_item->get_location() == _gui_stored_item_location_saved_game_file || film_selected_item->get_location() == _gui_stored_item_location_autosave_queue);

	if (film_selected_item->get_film_details(&game_setup_storage.theater.game_options, &game_setup_storage.theater.length_in_ticks, &game_setup_storage.theater.start_tick))
	{
		game_setup_storage.theater.location = film_selected_item->get_location();
		controller_get(controller_index)->get_player_identifier(&game_setup_storage.theater.owner);
		game_setup_storage.theater.film_description.controller_index = controller_index;
		game_setup_storage.theater.film_description.category = film_selected_item->m_category;
		game_setup_storage.theater.film_description.campaign_id = film_selected_item->m_metadata.campaign_id;
		game_setup_storage.theater.film_description.difficulty = film_selected_item->m_metadata.campaign_difficulty;
		game_setup_storage.theater.film_description.map_id = film_selected_item->m_metadata.map_id;
		game_setup_storage.theater.film_description.length_seconds = film_selected_item->m_metadata.length_seconds;

		zero_array(game_setup_storage.theater.film_description.film_path);
		gui_selected_item_get_file_path(film_selected_item, game_setup_storage.theater.film_description.film_path, 256);

		zero_array(game_setup_storage.theater.film_description.film_name);
		ustrnzcpy(game_setup_storage.theater.film_description.film_name, film_selected_item->get_title(), 128);

		m_selected_from_recent_list[film_selected_item->m_selection_type] = selected_from_recent_list;

		c_static_string<256> film_path;
		film_path.print("%ls", game_setup_storage.theater.film_description.film_path);

		if (saved_film_manager_film_valid(controller_index, film_path.get_string()))
		{
			user_interface_squad_set_film(&game_setup_storage.theater.film_description);
			user_interface_squad_set_saved_film_game_options(game_setup_storage.theater.length_in_ticks, game_setup_storage.theater.start_tick, &game_setup_storage.theater.game_options);

			if (game_setup_storage.theater.game_options.campaign_id != _campaign_id_none)
			{
				user_interface_squad_set_campaign_level(game_setup_storage.theater.game_options.campaign_id, game_setup_storage.theater.game_options.map_id);
				user_interface_squad_set_campaign_difficulty((e_campaign_difficulty_level)game_setup_storage.theater.game_options.campaign_difficulty);
				user_interface_squad_set_campaign_active_primary_skulls(game_setup_storage.theater.game_options.campaign_active_primary_skulls);
				user_interface_squad_set_campaign_active_secondary_skulls(game_setup_storage.theater.game_options.campaign_active_secondary_skulls);
				user_interface_squad_set_campaign_metagame_scoring((e_campaign_metagame_scoring)game_setup_storage.theater.game_options.campaign_metagame_scoring);
				user_interface_squad_set_campaign_insertion_point(game_setup_storage.theater.game_options.campaign_insertion_point);

				return true;
			}

			user_interface_squad_set_game_variant(&game_setup_storage.theater.game_options.multiplayer_variant);
			if (game_setup_storage.theater.game_options.map_variant.m_map_id != _map_id_none)
			{
				if (game_setup_storage.theater.game_options.map_id == _map_id_none)
				{
					c_map_variant default_variant{};
					default_variant.create_default(game_setup_storage.theater.game_options.map_id);
					if (user_interface_squad_set_multiplayer_map(&default_variant))
					{
						return true;
					}
				}
				else
				{
					if (user_interface_squad_set_multiplayer_map(&game_setup_storage.theater.game_options.map_variant))
					{
						return true;
					}
				}
			}
		}
	}

	s_saved_film_description invalid_film{};
	csmemset(&invalid_film, 0, sizeof(s_saved_film_description));
	invalid_film.category = _saved_film_category_invalid;
	user_interface_squad_set_film(&invalid_film);

	if (user_interface_squad_local_peer_is_leader())
	{
		user_interface_error_manager_get()->post_error(STRING_ID(gui_alert, pregame_selected_film_invalid), k_any_controller, false);
	}
	source_screen_index_to_dispose = NONE;

	return false;
}

bool c_gui_pregame_setup_manager::set_selected_item_type_game_variant(e_controller_index controller_index, const c_gui_game_variant_selected_item* game_variant_selected_item, bool selected_from_recent_list, int32 & source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage)
{
	const s_gui_game_setup_storage* last_game_setup = global_preferences_get_last_game_setup();

	// $IMPLEMENT

	return false;
}

bool c_gui_pregame_setup_manager::set_selected_item_type_hopper(e_controller_index controller_index, const c_gui_hopper_selected_item* hopper_selected_item, bool selected_from_recent_list, int32 & source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage)
{
	const s_gui_game_setup_storage* last_game_setup = global_preferences_get_last_game_setup();

	if (!user_interface_matchmaking_set_hopper(hopper_selected_item->m_hopper_information.hopper_identifier))
	{
		user_interface_play_sound(_ui_global_sound_effect_failure, NONE);
		return false;
	}

	game_setup_storage.matchmaking = *last_game_setup->get_matchmaking();
	user_interface_matchmaking_squad_set_search_preference(_matchmaking_search_preference_none);
	game_setup_storage.matchmaking.hopper_id = hopper_selected_item->m_hopper_information.hopper_identifier;
	m_selected_from_recent_list[hopper_selected_item->m_selection_type] = selected_from_recent_list;
	return true;
}

bool c_gui_pregame_setup_manager::set_selected_item_type_level(e_controller_index controller_index, const c_gui_level_selected_item* level_selected_item, bool selected_from_recent_list, int32 & source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage)
{
	const s_gui_game_setup_storage* last_game_setup = global_preferences_get_last_game_setup();

	s_level_datum level_data{};
	if (!levels_try_and_get_campaign_map(level_selected_item->m_map_id, &level_data)
		|| !level_data.flags.test(_level_visible_in_ui)
		|| !user_interface_squad_set_campaign_level(level_selected_item->m_campaign_id, level_selected_item->m_map_id))
	{
		user_interface_play_sound(_ui_global_sound_effect_failure, NONE);
		return false;
	}

	game_setup_storage.campaign = *last_game_setup->get_campaign();
	game_setup_storage.campaign.campaign_id = level_selected_item->m_campaign_id;
	m_selected_from_recent_list[level_selected_item->m_selection_type] = selected_from_recent_list;
	user_interface_squad_set_campaign_insertion_point(level_selected_item->m_insertion_point);
	return true;
}

bool c_gui_pregame_setup_manager::set_selected_item_type_map(e_controller_index controller_index, const c_gui_map_selected_item* map_selected_item, bool selected_from_recent_list, int32 & source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage)
{
	const s_gui_game_setup_storage* last_game_setup = global_preferences_get_last_game_setup();

	// $IMPLEMENT

	return false;
}

bool c_gui_pregame_setup_manager::set_selected_item_type_network_mode(e_controller_index controller_index, const c_gui_network_mode_selected_item* network_mode_selected_item, bool selected_from_recent_list, int32 & source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage)
{
	const s_gui_game_setup_storage* last_game_setup = global_preferences_get_last_game_setup();

	e_network_session_class session_class = user_interface_squad_get_session_class();
	switch (network_mode_selected_item->get_network_choice())
	{
	case _network_mode_choice_xbox_live_advertise_public:
	{
		if (online_has_any_silver_or_gold_live_users() && !online_is_connected_to_live())
		{
			user_interface_error_manager_get()->post_error(
				STRING_ID(gui_alert, switched_to_live_with_no_live_connection),
				controller_index,
				false);
		}
		else
		{
			c_gui_pregame_setup_manager::change_advertisement_mode(
				_network_advertise_xbox_live_public,
				session_class,
				_network_session_class_xbox_live,
				source_screen_index_to_dispose);
		}
	}
	break;
	case _network_mode_choice_xbox_live_advertise_friends:
	{
		if (online_has_any_silver_or_gold_live_users() && !online_is_connected_to_live())
		{
			user_interface_error_manager_get()->post_error(
				STRING_ID(gui_alert, switched_to_live_with_no_live_connection),
				controller_index,
				false);
		}
		else
		{
			c_gui_pregame_setup_manager::change_advertisement_mode(
				_network_advertise_xbox_live_friends_only,
				session_class,
				_network_session_class_xbox_live,
				source_screen_index_to_dispose);
		}
	}
	break;
	case _network_mode_choice_xbox_live_advertise_invite_only:
	{
		if (online_has_any_silver_or_gold_live_users() && !online_is_connected_to_live())
		{
			user_interface_error_manager_get()->post_error(
				STRING_ID(gui_alert, switched_to_live_with_no_live_connection),
				controller_index,
				false);
		}
		else
		{
			c_gui_pregame_setup_manager::change_advertisement_mode(
				_network_advertise_xbox_live_invite_only,
				session_class,
				_network_session_class_xbox_live,
				source_screen_index_to_dispose);
		}
	}
	break;
	case _network_mode_choice_system_link_advertise:
	{
		if (get_alpha_is_internal_beta())
		{
			user_interface_error_manager_get()->post_error(
				STRING_ID(gui_alert, alpha_network_mode_disabled),
				controller_index,
				false);
		}
		else
		{
			c_gui_pregame_setup_manager::change_advertisement_mode(
				_network_advertise_system_link,
				session_class,
				_network_session_class_system_link,
				source_screen_index_to_dispose);
		}
	}
	break;
	case _network_mode_choice_system_link_browse:
	{
		if (get_alpha_is_internal_beta())
		{
			user_interface_error_manager_get()->post_error(
				STRING_ID(gui_alert, alpha_network_mode_disabled),
				controller_index,
				false);
		}
		else
		{
			if (c_load_game_browser_screen_message* message = UI_MALLOC(c_load_game_browser_screen_message,
				STRING_ID(gui, game_browser),
				controller_index,
				k_number_of_player_windows,
				STRING_ID(gui, top_most),
				0,
				_browse_system_link))
			{
				user_interface_messaging_post(message);
			}

			return true;
		}
	}
	break;
	case _network_mode_choice_offline:
	{
		if (get_alpha_is_internal_beta())
		{
			user_interface_error_manager_get()->post_error(
				STRING_ID(gui_alert, alpha_network_mode_disabled),
				controller_index,
				false);
		}
		else
		{
			c_gui_pregame_setup_manager::change_advertisement_mode(
				_network_advertise_nothing,
				session_class,
				_network_session_class_offline,
				source_screen_index_to_dispose);
		}
	}
	break;
	}

	return false;
}

bool c_gui_pregame_setup_manager::set_selected_item_type_screenshot(e_controller_index controller_index, const c_gui_saved_screenshot_selected_item* screenshot_selected_item, bool selected_from_recent_list, int32 & source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage)
{
	const s_gui_game_setup_storage* last_game_setup = global_preferences_get_last_game_setup();

	// $IMPLEMENT

	return false;
}

//.text:00AA8760 ; 
//.text:00AA8840 ; 
//.text:00AA88E0 ; 
//.text:00AA89C0 ; public: bool c_gui_pregame_setup_manager::try_to_get_last_set_game_mode(e_gui_game_mode*) const

void c_gui_pregame_setup_manager::update()
{
	INVOKE_CLASS_MEMBER(0x00AA8A00, c_gui_pregame_setup_manager, update);
}

//.text:00AA8A40 ; 

