#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"

#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "game/game_engine.hpp"
#include "game/game_engine_display.hpp"
#include "game/game_engine_team.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_list_item_widget.hpp"
#include "interface/c_gui_list_widget.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_mapping.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_session.hpp"
#include "interface/user_interface_utilities.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "simulation/simulation.hpp"
#include "tag_files/string_ids.hpp"

#include <cstdlib> // qsort

REFERENCE_DECLARE_ARRAY(0x05269788, real32, c_gui_screen_scoreboard::m_scoreboard_alpha, 4);
REFERENCE_DECLARE(0x05269798, real32, c_gui_screen_scoreboard::m_console_scoreboard_alpha);

HOOK_DECLARE_CLASS_MEMBER(0x00AB42F0, c_gui_scoreboard_data, update_for_scoreboard_mode_);
HOOK_DECLARE_CLASS_MEMBER(0x00AB4920, c_gui_screen_scoreboard, update_render_state_);

void c_scoreboard_load_screen_message::set_is_interactive(bool is_interactive)
{
	m_is_interactive = is_interactive;
}

void c_gui_screen_scoreboard::set_is_interactive(bool is_interactive)
{
	m_is_interactive = is_interactive;
}

void __cdecl c_gui_screen_scoreboard::translate_widget_recursive(c_gui_widget* widget, long x, long y)
{
	INVOKE(0x00AB2870, c_gui_screen_scoreboard::translate_widget_recursive, widget, x, y);
}

// c_scoreboard_load_screen_message::c_scoreboard_load_screen_message
c_scoreboard_load_screen_message* scoreboard_load_screen_message_ctor(c_scoreboard_load_screen_message* message,
	long screen_name,
	e_controller_index controller,
	e_window_index window,
	long layered_position,
	bool is_interactive)
{
	return DECLFUNC(0x00AB2AD0, c_scoreboard_load_screen_message*, __thiscall, c_scoreboard_load_screen_message*, long, e_controller_index, e_window_index, long, bool)(message, screen_name, controller, window, layered_position, is_interactive);
}

real32 __cdecl c_gui_screen_scoreboard::get_scoreboard_alpha(e_controller_index controller_index)
{
	return INVOKE(0x00AB30F0, c_gui_screen_scoreboard::get_scoreboard_alpha, controller_index);
}

c_gui_screen_scoreboard* __cdecl c_gui_screen_scoreboard::get_scoreboard_screen(e_controller_index controller_index)
{
	return INVOKE(0x00AB3120, c_gui_screen_scoreboard::get_scoreboard_screen, controller_index);
}

void __cdecl c_gui_screen_scoreboard::hide_scoreboard(e_controller_index controller_index)
{
	return INVOKE(0x00AB3560, c_gui_screen_scoreboard::hide_scoreboard, controller_index);
}

bool __cdecl c_gui_screen_scoreboard::is_scoreboard_displayed(e_controller_index controller_index)
{
	return INVOKE(0x00AB3690, c_gui_screen_scoreboard::is_scoreboard_displayed, controller_index);
}

bool __cdecl c_gui_screen_scoreboard::is_scoreboard_interactive(e_controller_index controller_index)
{
	return INVOKE(0x00AB3740, c_gui_screen_scoreboard::is_scoreboard_interactive, controller_index);
}

void __cdecl c_gui_screen_scoreboard::show_scoreboard(e_controller_index controller_index, bool is_interactive)
{
	INVOKE(0x00AB3C60, c_gui_screen_scoreboard::show_scoreboard, controller_index, is_interactive);

	//if (is_scoreboard_displayed(controller_index))
	//{
	//	if (c_gui_screen_scoreboard* scoreboard_screen = get_scoreboard_screen(controller_index))
	//	{
	//		scoreboard_screen->set_is_interactive(is_interactive);
	//	}
	//}
	//else
	//{
	//	bool half_screen = false;
	//	if (controller_index == k_any_controller)
	//	{
	//		half_screen = true;
	//	}
	//	else
	//	{
	//		long user_index = controller_get(controller_index)->get_user_index();
	//		if (user_index != NONE)
	//		{
	//			long v1 = 0;
	//			long v2 = 0;
	//			user_interface_get_number_of_render_windows(user_index, &v1, &v2);
	//			half_screen = v2 == 1;
	//		}
	//	}
	//
	//	c_scoreboard_load_screen_message* message = (c_scoreboard_load_screen_message*)user_interface_malloc_tracked(sizeof(c_scoreboard_load_screen_message), __FILE__, __LINE__);
	//	if (scoreboard_load_screen_message_ctor(
	//		message,
	//		!half_screen + STRING_ID(gui, scoreboard),
	//		controller_index,
	//		user_interface_get_window_for_controller(controller_index),
	//		STRING_ID(gui, top_most),
	//		is_interactive))
	//	{
	//		user_interface_messaging_post(message);
	//	}
	//}
}

void __thiscall c_gui_screen_scoreboard::update_render_state_(uint32 a1)
{
	// 0x00AB4920
	//HOOK_INVOKE_CLASS(, c_gui_screen_scoreboard, _update_render_state, void(__thiscall*)(c_gui_screen_scoreboard*, uint32), _this, a2);

	c_gui_list_widget* child_list_widget = get_child_list_widget(STRING_ID(gui, scoreboard));
	c_gui_data* data = get_data(STRING_ID(gui, scoreboard), 0);

	INVOKE_CLASS_MEMBER(0x00ABB1A0, c_gui_widget, update_render_state, a1);

	if (child_list_widget)
	{
		if (data)
		{
			bool has_teams = game_engine_has_teams();

			for (c_gui_list_item_widget* list_item_widget = static_cast<c_gui_list_item_widget*>(child_list_widget->get_first_child_widget_by_type(_gui_list_item));
				list_item_widget;
				list_item_widget = list_item_widget->get_next_list_item_widget(true))
			{
				c_gui_bitmap_widget* base_color_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, base_color));
				c_gui_text_widget* name_text_widget = list_item_widget->get_child_text_widget(STRING_ID(global, name));
				c_gui_text_widget* team_bar_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, team_bar));
				c_gui_text_widget* service_tag_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, service_tag));
				c_gui_text_widget* service_tag_hilite_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, service_tag_hilite));
				c_gui_text_widget* place_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, place));
				c_gui_text_widget* score_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, score));
				c_gui_text_widget* round_score_text_widget = list_item_widget->get_child_text_widget(STRING_ID(gui, round_score));
				c_gui_bitmap_widget* team_bar_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, team_bar));
				c_gui_bitmap_widget* observer_bitmap_widget = list_item_widget->get_child_bitmap_widget(STRING_ID(gui, observer));

				long element_handle = list_item_widget->get_element_handle();

				long player_row_type = 0;
				long base_color = 0;
				long team_color = NONE;

				if (data->get_integer_value(element_handle, STRING_ID(gui, player_row_type), &player_row_type)
					&& data->get_integer_value(element_handle, STRING_ID(gui, base_color), &base_color)
					&& data->get_integer_value(element_handle, STRING_ID(gui, team_color), &team_color))
				{
					long color_list_index = base_color;
					if (has_teams)
					{
						color_list_index = team_color;
						tint_widget_to_change_color(base_color_bitmap_widget, color_list_index, has_teams);
					}
					else
					{
						base_color |= (255 << 24);
						tint_widget_to_change_argb_color(base_color_bitmap_widget, { .value = static_cast<uint32>(base_color) });
					}

					tint_widget_to_change_color((c_gui_widget*)name_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)team_bar_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)service_tag_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)service_tag_hilite_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)place_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)score_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color((c_gui_widget*)round_score_text_widget, color_list_index, has_teams);
					tint_widget_to_change_color(team_bar_bitmap_widget, color_list_index, true);
					tint_widget_to_change_color(observer_bitmap_widget, 16, true);
				}
			}
		}

		c_gui_widget* button_key_child_list_widget = get_first_child_widget_by_type(_gui_button_key);
		if (button_key_child_list_widget)
		{
			long v18 = 0;
			long y19 = 0;
			for (c_gui_list_item_widget* list_item_widget = static_cast<c_gui_list_item_widget*>(child_list_widget->get_first_child_widget_by_type(_gui_list_item));
				list_item_widget;
				list_item_widget = list_item_widget->get_next_list_item_widget(false))
			{
				real_rectangle2d current_bounds{};
				list_item_widget->get_current_bounds(&current_bounds);

				if (list_item_widget->get_element_handle() != NONE)
					y19 = (long)current_bounds.y0;

				v18 = (long)current_bounds.y0;
			}

			if (y19 != v18)
				translate_widget_recursive(button_key_child_list_widget, 0, y19 - v18);
		}
	}
}

void __cdecl c_gui_screen_scoreboard::update_scoreboard_alpha(e_controller_index controller_index)
{
	INVOKE(0x00AB4C50, c_gui_screen_scoreboard::update_scoreboard_alpha, controller_index);
}

bool __cdecl c_gui_scoreboard_data::add_player_internal(
	e_player_row_type player_row_type,
	long game_player_index,
	long session_player_index,
	s_player_appearance const* appearance,
	wchar_t const* player_name,
	wchar_t const* service_tag,
	long base_color_index,
	long team_index,
	bool show_team,
	e_controller_index controller_index,
	e_voice_talking_state voice_state,
	long connectivity_rating,
	wchar_t const* place,
	wchar_t const* score,
	wchar_t const* round_score,
	bool is_dead,
	bool left_game
)
{
	//return INVOKE_CLASS_MEMBER(0x00B24940, c_gui_scoreboard_data, add_player_internal,
	//	player_row_type,
	//	game_player_index,
	//	session_player_index,
	//	appearance,
	//	player_name,
	//	service_tag,
	//	base_color_index,
	//	team_index,
	//	show_team,
	//	controller_index,
	//	voice_state,
	//	connectivity_rating,
	//	place,
	//	score,
	//	round_score,
	//	is_dead,
	//	left_game);

	if (m_player_row_count < 25)
	{
		m_player_rows[m_player_row_count].player_row_type = player_row_type;
		m_player_rows[m_player_row_count].game_player_index = game_player_index;
		m_player_rows[m_player_row_count].session_player_index = session_player_index;
		csmemcpy(&m_player_rows[m_player_row_count].player_appearance, appearance, sizeof(s_player_appearance));
		m_player_rows[m_player_row_count].player_name = player_name;
		m_player_rows[m_player_row_count].service_tag = service_tag;
		m_player_rows[m_player_row_count].base_color_index = base_color_index;
		m_player_rows[m_player_row_count].team_index = team_index;
		m_player_rows[m_player_row_count].show_team = show_team;
		m_player_rows[m_player_row_count].local_controller_index = controller_index;
		m_player_rows[m_player_row_count].voice_state = voice_state;
		m_player_rows[m_player_row_count].connectivity_rating = connectivity_rating;
		m_player_rows[m_player_row_count].place = place;
		m_player_rows[m_player_row_count].score = score;
		m_player_rows[m_player_row_count].round_score = round_score;
		m_player_rows[m_player_row_count].is_dead = is_dead;
		m_player_rows[m_player_row_count].left_game = left_game;
		m_player_row_count++;

		return true;
	}

	return false;
}

void __thiscall c_gui_scoreboard_data::update_for_scoreboard_mode_(bool use_session, bool include_score)
{
	static c_static_wchar_string<256> place_string;
	static c_static_wchar_string<256> score_string;
	static c_static_wchar_string<256> round_score_string;

	place_string.set(L" ");
	score_string.set(L" ");
	round_score_string.set(L" ");

	m_player_row_count = 0;
	m_player_rows.clear();

	bool include_team_score = include_score && game_engine_has_teams();
	long round_score = 0;
	uint32 team_flags = 0;

	if (use_session)
	{
		ASSERT(!include_score);

		for (long player_index = 0; player_index < k_maximum_players; player_index++)
		{
			if (user_interface_squad_is_player_valid(player_index))
			{
				s_player_configuration* player_data = user_interface_session_get_player_data(player_index);
				if (player_data->host.team_index != _game_team_none)
				{
					long player_netdebug_filled_bar_count = user_interface_session_get_player_netdebug_filled_bar_count(player_index);

					e_controller_index local_controller_index = k_no_controller;
					if (user_interface_session_is_local_player(player_index))
						local_controller_index = user_interface_session_get_controller_index(player_index);

					long base_color_index = player_data->host.armor.loadouts[player_data->host.armor.loadout_index].colors[0].value;

					add_player_internal(
						/* player_row_type        */ _player_row_type_player,
						/* game_player_index      */ NONE,
						/* session_player_index   */ player_index,
						/* appearance             */ &player_data->host.appearance,
						/* player_name            */ player_data->host.name.get_string(),
						/* service_tag            */ player_data->host.appearance.service_tag.get_string(),
						/* base_color_index       */ base_color_index,
						/* team_index             */ _game_team_none,
						/* show_team              */ false,
						/* local_controller_index */ local_controller_index,
						/* voice_state            */ _voice_state_none,
						/* connectivity_rating    */ player_netdebug_filled_bar_count,
						/* place                  */ place_string.get_string(),
						/* score                  */ score_string.get_string(),
						/* round_score            */ round_score_string.get_string(),
						/* is_dead                */ false,
						/* left_game              */ false
					);
				}
			}
		}
	}
	else
	{
		c_player_in_game_iterator player_iterator;
		player_iterator.begin();
		while (player_iterator.next())
		{
			player_datum* player = player_iterator.get_datum();
			long player_iterator_index = player_iterator.get_index();

			long player_index = user_interface_squad_get_player_index(&player->player_identifier);
			if (player->configuration.host.team_index != _game_team_none)
			{
				bool is_dead = !TEST_BIT(player->flags, _player_initial_spawn_bit)
					&& player->unit_index == NONE
					&& game_in_progress()
					&& !game_is_finished()
					&& !simulation_starting_up()
					&& game_engine_in_round();

				if (include_score)
				{
					long finalized_place = game_engine_get_finalized_player_place(player_iterator_index);
					if (include_team_score)
						finalized_place = game_engine_get_finalized_team_place(player->configuration.host.team_index);

					game_engine_get_place_string(finalized_place, &place_string);
					long player_score = game_engine_get_player_score_for_display(player_iterator_index, true);
					game_engine_get_score_string(player_score, &score_string);
					long player_round_score = game_engine_get_player_score_for_display(player_iterator_index, false);
					game_engine_get_score_string(player_round_score, &round_score_string);
				}

				if (include_team_score)
					team_flags |= FLAG(player->configuration.host.team_index);

				if (player_index == NONE)
				{
					long base_color_index = player->configuration.host.armor.loadouts[player->armor_loadout_index].colors[0].value;
					long player_index = player_iterator_index;
					round_score = 0;

					c_gui_scoreboard_data::add_player_internal(
						/* player_row_type        */ _player_row_type_player,
						/* game_player_index      */ player_index,
						/* session_player_index   */ NONE,
						/* appearance             */ &player->configuration.host.appearance,
						/* player_name            */ player->configuration.host.name.get_string(),
						/* service_tag            */ player->configuration.host.appearance.service_tag.get_string(),
						/* base_color_index       */ base_color_index,
						/* team_index             */ player->configuration.host.team_index,
						/* show_team              */ game_engine_has_teams(),
						/* local_controller_index */ k_no_controller,
						/* voice_state            */ _voice_state_none,
						/* connectivity_rating    */ NONE,
						/* place                  */ place_string.get_string(),
						/* score                  */ score_string.get_string(),
						/* round_score            */ round_score_string.get_string(),
						/* is_dead                */ is_dead,
						/* left_game              */ TEST_BIT(player->flags, _player_left_game_bit)
					);
				}
				else
				{
					long player_netdebug_filled_bar_count = user_interface_session_get_player_netdebug_filled_bar_count(player_index);

					e_controller_index controller_index = k_no_controller;
					if (user_interface_session_is_local_player(player_index))
						controller_index = user_interface_session_get_controller_index(player_index);

					long base_color_index = player->configuration.host.armor.loadouts[player->armor_loadout_index].colors[0].value;

					add_player_internal(
						/* player_row_type        */ _player_row_type_player,
						/* game_player_index      */ NONE,
						/* session_player_index   */ player_index,
						/* appearance             */ &player->configuration.host.appearance,
						/* player_name            */ player->configuration.host.name.get_string(),
						/* service_tag            */ player->configuration.host.appearance.service_tag.get_string(),
						/* base_color_index       */ base_color_index,
						/* team_index             */ player->configuration.host.team_index,
						/* show_team              */ game_engine_has_teams(),
						/* local_controller_index */ controller_index,
						/* voice_state            */ _voice_state_none,
						/* connectivity_rating    */ player_netdebug_filled_bar_count,
						/* place                  */ place_string.get_string(),
						/* score                  */ score_string.get_string(),
						/* round_score            */ round_score_string.get_string(),
						/* is_dead                */ is_dead,
						/* left_game              */ false
					);
					round_score = 0;
				}
			}
		}

		if (include_team_score)
		{
			static s_player_appearance player_appearance; // for emblem?
			static c_static_wchar_string<256> team_name;

			csmemset(&player_appearance, 0, sizeof(s_player_appearance));
			team_name.clear();

			for (long team_index = 0; team_index < k_multiplayer_team_count; team_index++)
			{
				if (game_engine_is_team_ever_active(team_index) || TEST_BIT(team_flags, team_index))
				{
					if (include_score)
					{
						long team_place = 0;
						long team_score = 0;

						if (game_engine_is_team_ever_active(team_index))
						{
							team_place = game_engine_get_team_place(team_index);
							team_score = game_engine_get_team_score_for_display(team_index, true);
							round_score = game_engine_get_team_score_for_display(team_index, false);
						}

						game_engine_get_place_string(team_place, &place_string);
						game_engine_get_score_string(team_score, &score_string);
						game_engine_get_score_string(round_score, &round_score_string);
					}
					game_engine_get_team_name(team_index, &team_name);

					add_player_internal(
						/* player_row_type        */ _player_row_type_team_bar,
						/* game_player_index      */ NONE,
						/* session_player_index   */ NONE,
						/* appearance             */ &player_appearance,
						/* player_name            */ team_name.get_string(),
						/* service_tag            */ L" ",
						/* base_color_index       */ NONE,
						/* team_index             */ team_index,
						/* show_team              */ true,
						/* local_controller_index */ k_no_controller,
						/* voice_state            */ _voice_state_none,
						/* connectivity_rating    */ NONE,
						/* place                  */ place_string.get_string(),
						/* score                  */ score_string.get_string(),
						/* round_score            */ round_score_string.get_string(),
						/* is_dead                */ false,
						/* left_game              */ false
					);
				}

				rotate_left(team_flags, 1);
			}
		}
	}

	scoreboard_sort_proc_t* scoreboard_sort_proc = use_session ? scoreboard_sort_proc_for_session : scoreboard_sort_proc_for_multiplayer;
	qsort(m_player_rows.get_elements(), m_player_row_count, sizeof(s_player_row), scoreboard_sort_proc);
}

int __cdecl c_gui_scoreboard_data::scoreboard_sort_proc_for_multiplayer(void const* a, void const* b)
{
	return INVOKE(0x00AB38A0, c_gui_scoreboard_data::scoreboard_sort_proc_for_multiplayer, a, b);
}

int __cdecl c_gui_scoreboard_data::scoreboard_sort_proc_for_session(void const* a, void const* b)
{
	return INVOKE(0x00AB3A00, c_gui_scoreboard_data::scoreboard_sort_proc_for_session, a, b);
}

//.text:00AB3A20 ; private: bool c_gui_screen_scoreboard::select_player_in_scoreboard(e_controller_index, s_player_identifier const*)
//.text:00AB3B30 ; public: void c_gui_screen_scoreboard::set_is_interactive(bool)
//.text:00AB3B80 ; 
//.text:00AB3B90 ; 
//.text:00AB3B40 ; 
//.text:00AB3BA0 ; private: bool __cdecl c_gui_screen_scoreboard::should_show_round_score(e_controller_index, c_gui_screen_scoreboard::e_scoreboard_mode) const
//.text:00AB3C30 ; public: static bool __cdecl c_gui_screen_scoreboard::should_show_score(c_gui_screen_scoreboard::e_scoreboard_mode)

