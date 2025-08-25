#include "interface/gui_pregame_setup_manager.hpp"

#include "interface/c_controller.hpp"
#include "interface/gui_selected_items.hpp"
#include "interface/gui_selected_items_saved_film.hpp"
#include "interface/user_interface_error_manager.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"
#include "saved_games/saved_film_manager.hpp"

REFERENCE_DECLARE(0x0191D750, c_gui_pregame_setup_manager, g_gui_pregame_setup_manager);

HOOK_DECLARE_CLASS_MEMBER(0x00AA7DB0, c_gui_pregame_setup_manager, set_selected_item_);

void __thiscall c_gui_pregame_setup_manager::set_selected_item_(e_controller_index controller_index, const c_gui_selected_item* selected_item, bool selected_from_recent_list, int32 source_screen_index_to_dispose)
{
	if (selected_item->m_selection_type != _gui_selection_type_film)
	{
		HOOK_INVOKE_CLASS_MEMBER(, c_gui_pregame_setup_manager, set_selected_item_, controller_index, selected_item, selected_from_recent_list, source_screen_index_to_dispose);
		return;
	}

	ASSERT(selected_item);

	const s_gui_game_setup_storage* last_game_setup = global_preferences_get_last_game_setup();

	s_gui_single_game_setup_storage game_setup_storage{};
	game_setup_storage.game_mode = user_interface_squad_get_ui_game_mode();

	c_gui_saved_film_selected_item* film_selected_item = (c_gui_saved_film_selected_item*)selected_item;
	csmemcpy(&game_setup_storage.theater, last_game_setup->get_theater(), sizeof(s_gui_game_setup_storage::s_theater_settings));
	ASSERT(film_selected_item->get_location() == _gui_stored_item_location_saved_game_file || film_selected_item->get_location() == _gui_stored_item_location_autosave_queue);

	bool valid = false;

	if (film_selected_item->get_film_details(&game_setup_storage.theater.game_options,
		&game_setup_storage.theater.length_in_ticks,
		&game_setup_storage.theater.start_tick))
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

		m_selected_from_recent_list[selected_item->m_selection_type] = selected_from_recent_list;

		c_static_string<256> film_path;
		film_path.print("%ls", game_setup_storage.theater.film_description.film_path);

		if (saved_film_manager_film_valid(controller_index, film_path.get_string()))
		{
			user_interface_squad_set_film(
				&game_setup_storage.theater.film_description);
			user_interface_squad_set_saved_film_game_options(
				game_setup_storage.theater.length_in_ticks,
				game_setup_storage.theater.start_tick,
				&game_setup_storage.theater.game_options);

			if (game_setup_storage.theater.game_options.campaign_id != _campaign_id_none)
			{
				// $IMPLEMENT

				return;

				//s_campaign_armaments player_armaments{};
				//s_campaign_game_progression game_progression{};
				//s_hub_progression hub_progression{};
				//
				//user_interface_squad_set_campaign_level(
				//	game_setup_storage.theater.game_options.campaign_id,
				//	game_setup_storage.theater.game_options.map_id,
				//	game_setup_storage.theater.game_options.campaign_insertion_point,
				//	&player_armaments,
				//	&game_progression,
				//	&hub_progression);
				//user_interface_squad_set_campaign_difficulty(
				//	(e_campaign_difficulty_level)game_setup_storage.theater.game_options.campaign_difficulty);
				//user_interface_squad_set_campaign_active_primary_skulls(
				//	game_setup_storage.theater.game_options.campaign_active_primary_skulls);
				//user_interface_squad_set_campaign_active_secondary_skulls(
				//	game_setup_storage.theater.game_options.campaign_active_secondary_skulls);
				//user_interface_squad_set_campaign_metagame_scoring(
				//	(e_campaign_metagame_scoring)game_setup_storage.theater.game_options.campaign_metagame_scoring);
				//user_interface_squad_set_campaign_insertion_point(
				//	game_setup_storage.theater.game_options.campaign_insertion_point);

				valid = true;
			}
			else
			{
				user_interface_squad_set_game_variant(
					&game_setup_storage.theater.game_options.multiplayer_variant);
				if (game_setup_storage.theater.game_options.map_variant.m_map_id != _map_id_none)
				{
					if (game_setup_storage.theater.game_options.map_id == _map_id_none)
					{
						c_map_variant default_variant{};
						default_variant.create_default(game_setup_storage.theater.game_options.map_id);
						valid = user_interface_squad_set_multiplayer_map(&default_variant);
					}
					else
					{
						valid = user_interface_squad_set_multiplayer_map(&game_setup_storage.theater.game_options.map_variant);
					}
				}
			}
		}
	}

	if (!valid)
	{
		s_saved_film_description invalid_film{};
		csmemset(&invalid_film, 0, sizeof(s_saved_film_description));
		invalid_film.category = _saved_film_category_invalid;
		user_interface_squad_set_film(&invalid_film);

		if (user_interface_squad_local_peer_is_leader())
		{
			user_interface_error_manager_get()->post_error(STRING_ID(gui_alert, pregame_selected_film_invalid), k_any_controller, false);
		}
		source_screen_index_to_dispose = NONE;
	}

	global_preferences_update_last_game_setup(&game_setup_storage);
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

//.text:00AA7270 ; public: void c_gui_pregame_setup_manager::restore_game_setup(e_gui_game_mode, s_gui_game_setup_storage*)
//.text:00AA72A0 ; private: void c_gui_pregame_setup_manager::restore_game_setup_internal(e_gui_game_mode, s_gui_game_setup_storage*)
//.text:00AA7B00 ; public: static void __cdecl c_gui_pregame_setup_manager::save_game_setup(const s_gui_game_setup_storage*, s_gui_single_game_setup_storage*)
//.text:00AA7D90 ; 

void c_gui_pregame_setup_manager::set_selected_item(e_controller_index controller_index, const c_gui_selected_item* selected_item, bool selected_from_recent_list, int32 source_screen_index_to_dispose)
{
	INVOKE_CLASS_MEMBER(0x00AA7DB0, c_gui_pregame_setup_manager, set_selected_item, controller_index, selected_item, selected_from_recent_list, source_screen_index_to_dispose);
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

