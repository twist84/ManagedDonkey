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

void __thiscall c_gui_scoreboard_data::update_for_scoreboard_mode_(bool use_session, bool include_score)
{
	c_gui_scoreboard_data::update_for_scoreboard_mode(use_session, include_score);
}

void __thiscall c_gui_screen_scoreboard::update_render_state_(uns32 current_milliseconds)
{
	c_gui_screen_scoreboard::update_render_state(current_milliseconds);
}

c_gui_screen_scoreboard::c_gui_screen_scoreboard(int32 name) :
	c_gui_screen_widget(name),
	m_current_scoreboard_mode(_scoreboard_mode_none),
	m_is_interactive(false)
{
	//DECLFUNC(0x00AB2A90, void, __thiscall, c_gui_screen_scoreboard*, int32)(this, name);

	m_render_in_screenshot = true;
}

c_scoreboard_load_screen_message::c_scoreboard_load_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 layered_position, bool is_interactive) :
	c_load_screen_message(screen_name, controller, window, layered_position),
	m_is_interactive(is_interactive)
{
	//DECLFUNC(0x00AB2AD0, void, __thiscall, c_scoreboard_load_screen_message*, int32, e_controller_index, e_window_index, int32, bool)(this, screen_name, controller, window, layered_position, is_interactive);
}

c_gui_scoreboard_data::s_player_row::s_player_row()
{
	DECLFUNC(0x00AB2B00, void, __thiscall, c_gui_scoreboard_data::s_player_row*)(this);
}

//.text:00AB2B40 ; public: virtual c_gui_scoreboard_data::~c_gui_scoreboard_data()
//.text:00AB2B50 ; public: virtual c_gui_screen_scoreboard::~c_gui_screen_scoreboard()
//.text:00AB2B60 ; public: virtual c_scoreboard_load_screen_message::~c_scoreboard_load_screen_message()
//.text:00AB2B70 ; 
//.text:00AB2B90 ; 
//.text:00AB2BA0 ; public: virtual void* c_gui_scoreboard_data::`vector deleting destructor'(unsigned int)
//.text:00AB2BD0 ; public: virtual void* c_gui_screen_scoreboard::`scalar deleting destructor'(unsigned int)
//.text:00AB2C00 ; public: virtual void* c_scoreboard_load_screen_message::`vector deleting destructor'(unsigned int)

bool c_gui_scoreboard_data::add_player_internal(
	e_player_row_type player_row_type,
	int32 game_player_index,
	int32 session_player_index,
	const s_player_appearance* appearance,
	const wchar_t* name,
	const wchar_t* service_tag,
	int32 base_color_index,
	int32 team_index,
	bool show_team,
	e_controller_index local_controller_index,
	e_voice_talking_state voice_state,
	int32 connectivity_rating,
	const wchar_t* place,
	const wchar_t* score,
	const wchar_t* round_score,
	bool is_dead,
	bool left_game)
{
	//return INVOKE_CLASS_MEMBER(0x00AB2C30, c_gui_scoreboard_data, add_player_internal,
	//	player_row_type,
	//	game_player_index,
	//	session_player_index,
	//	appearance,
	//	name,
	//	service_tag,
	//	base_color_index,
	//	team_index,
	//	show_team,
	//	local_controller_index,
	//	voice_state,
	//	connectivity_rating,
	//	place,
	//	score,
	//	round_score,
	//	is_dead,
	//	left_game);

	if (m_player_row_count >= 25)
	{
		return false;
	}

	m_player_rows[m_player_row_count].player_row_type = player_row_type;
	m_player_rows[m_player_row_count].game_player_index = game_player_index;
	m_player_rows[m_player_row_count].session_player_index = session_player_index;
	csmemcpy(&m_player_rows[m_player_row_count].appearance, appearance, sizeof(s_player_appearance));
	m_player_rows[m_player_row_count].name = name;
	m_player_rows[m_player_row_count].service_tag = service_tag;
	m_player_rows[m_player_row_count].base_color_index = base_color_index;
	m_player_rows[m_player_row_count].team_index = team_index;
	m_player_rows[m_player_row_count].show_team = show_team;
	m_player_rows[m_player_row_count].local_controller_index = local_controller_index;
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

void c_scoreboard_load_screen_message::apply_initial_state(c_gui_screen_widget* screen_widget) const
{
	//INVOKE_CLASS_MEMBER(0x00AB2E10, c_scoreboard_load_screen_message, apply_initial_state, screen_widget);

	c_gui_screen_scoreboard* scoreboard = (c_gui_screen_scoreboard*)screen_widget;
	scoreboard->set_is_interactive(m_is_interactive);
}

void c_gui_scoreboard_data::get_column_names(int32* const column_names, int32* column_count)
{
	INVOKE_CLASS_MEMBER(0x00AB2E30, c_gui_scoreboard_data, get_column_names, column_names, column_count);
}

//.text:00AB2EE0 ; public: static int32 c_static_array<c_gui_scoreboard_data::s_player_row, 25>::get_count()

int32 c_gui_scoreboard_data::get_current_item_count_internal()
{
	//return INVOKE_CLASS_MEMBER(0x00AB2EF0, c_gui_scoreboard_data, get_current_item_count_internal);

	return m_player_row_count;
}

//.text:00AB2F00 ; 

bool c_gui_scoreboard_data::get_integer_value(int32 element_handle, int32 value_name, int32* value)
{
	return INVOKE_CLASS_MEMBER(0x00AB2F10, c_gui_scoreboard_data, get_integer_value, element_handle, value_name, value);
}

//.text:00AB3070 ; public: bool c_gui_screen_scoreboard::get_is_interactive() const

bool c_gui_scoreboard_data::get_player_appearance(int32 element_handle, s_player_appearance* appearance)
{
	return INVOKE_CLASS_MEMBER(0x00AB3090, c_gui_scoreboard_data, get_player_appearance, element_handle, appearance);
}

e_render_data_size c_gui_screen_scoreboard::get_render_data_size()
{
	//return INVOKE_CLASS_MEMBER(0x00AB30E0, c_gui_screen_scoreboard, get_render_data_size);

	return k_render_data_size_large;
}

real32 __cdecl c_gui_screen_scoreboard::get_scoreboard_alpha(e_controller_index controller_index)
{
	return INVOKE(0x00AB30F0, c_gui_screen_scoreboard::get_scoreboard_alpha, controller_index);
}

c_gui_screen_scoreboard* __cdecl c_gui_screen_scoreboard::get_scoreboard_screen(e_controller_index controller_index)
{
	return INVOKE(0x00AB3120, c_gui_screen_scoreboard::get_scoreboard_screen, controller_index);
}

bool c_gui_scoreboard_data::get_text_value(int32 element_handle, int32 value_name, c_static_wchar_string<1024>* buffer)
{
	return INVOKE_CLASS_MEMBER(0x00AB31A0, c_gui_scoreboard_data, get_text_value, element_handle, value_name, buffer);
}

bool c_gui_screen_scoreboard::handle_controller_input_message(const c_controller_input_message* message)
{
	return INVOKE_CLASS_MEMBER(0x00AB3230, c_gui_screen_scoreboard, handle_controller_input_message, message);
}

bool c_gui_screen_scoreboard::handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource)
{
	return INVOKE_CLASS_MEMBER(0x00AB3470, c_gui_screen_scoreboard, handle_list_item_chosen, message, list_name, list_item_widget, datasource);
}

void __cdecl c_gui_screen_scoreboard::hide_scoreboard(e_controller_index controller_index)
{
	//return INVOKE(0x00AB3560, c_gui_screen_scoreboard::hide_scoreboard, controller_index);

	c_gui_screen_scoreboard* scoreboard = c_gui_screen_scoreboard::get_scoreboard_screen(controller_index);
	if (scoreboard && !scoreboard->transitioning_out())
	{
		scoreboard->transition_out(_transition_out_normal);
	}
}

void c_gui_screen_scoreboard::initialize_datasource()
{
	INVOKE_CLASS_MEMBER(0x00AB3590, c_gui_screen_scoreboard, initialize_datasource);
}

bool __cdecl c_gui_screen_scoreboard::is_scoreboard_displayed(e_controller_index controller_index)
{
	return INVOKE(0x00AB3690, c_gui_screen_scoreboard::is_scoreboard_displayed, controller_index);
}

bool __cdecl c_gui_screen_scoreboard::is_scoreboard_interactive(e_controller_index controller_index)
{
	//return INVOKE(0x00AB3740, c_gui_screen_scoreboard::is_scoreboard_interactive, controller_index);

	c_gui_screen_scoreboard* scoreboard = c_gui_screen_scoreboard::get_scoreboard_screen(controller_index);
	return scoreboard && scoreboard->m_is_interactive && !scoreboard->transitioning_out();
}

//.text:00AB3770 ; uns64 __cdecl user_interface_session_player_get_usable_player_xuid(const s_player_identifier*)

void c_gui_screen_scoreboard::post_initialize()
{
	INVOKE_CLASS_MEMBER(0x00AB3810, c_gui_screen_scoreboard, post_initialize);
}

int __cdecl c_gui_scoreboard_data::scoreboard_sort_proc_for_multiplayer(const void* a, const void* b)
{
	return INVOKE(0x00AB38A0, c_gui_scoreboard_data::scoreboard_sort_proc_for_multiplayer, a, b);
}

int __cdecl c_gui_scoreboard_data::scoreboard_sort_proc_for_session(const void* a, const void* b)
{
	return INVOKE(0x00AB3A00, c_gui_scoreboard_data::scoreboard_sort_proc_for_session, a, b);
}

//.text:00AB3A20 ; private: bool c_gui_screen_scoreboard::select_player_in_scoreboard(e_controller_index, const s_player_identifier*)

void c_gui_screen_scoreboard::set_is_interactive(bool value)
{
	//INVOKE_CLASS_MEMBER(0x00AB3B30, c_gui_screen_scoreboard, set_is_interactive, value);

	m_is_interactive = value;
}

void c_gui_scoreboard_data::set_scoreboard_mode(c_gui_screen_scoreboard::e_scoreboard_mode value)
{
	//INVOKE_CLASS_MEMBER(0x00AB3B40, c_gui_scoreboard_data, set_scoreboard_mode, value);

	m_current_scoreboard_mode = value;
}

void c_gui_screen_scoreboard::set_scoreboard_mode(e_scoreboard_mode scoreboard_mode)
{
	//INVOKE_CLASS_MEMBER(0x00AB3B50, c_gui_screen_scoreboard, set_scoreboard_mode, scoreboard_mode);

	c_gui_scoreboard_data* scoreboard_data = (c_gui_scoreboard_data*)c_gui_screen_widget::get_data(STRING_ID(gui, scoreboard), NULL);
	if (scoreboard_data)
	{
		m_current_scoreboard_mode = scoreboard_mode;
		scoreboard_data->set_scoreboard_mode(scoreboard_mode);
	}
}

//.text:00AB3B80 ; 
//.text:00AB3B90 ; 
//.text:00AB3BA0 ; private: bool c_gui_screen_scoreboard::should_show_round_score(e_controller_index, e_scoreboard_mode) const
//.text:00AB3C30 ; public: static bool __cdecl c_gui_screen_scoreboard::should_show_score(e_scoreboard_mode)

void __cdecl c_gui_screen_scoreboard::show_scoreboard(e_controller_index controller_index, bool is_interactive)
{
	//INVOKE(0x00AB3C60, c_gui_screen_scoreboard::show_scoreboard, controller_index, is_interactive);

	if (c_gui_screen_scoreboard::is_scoreboard_displayed(controller_index))
	{
		if (c_gui_screen_scoreboard* scoreboard_screen = c_gui_screen_scoreboard::get_scoreboard_screen(controller_index))
		{
			scoreboard_screen->set_is_interactive(is_interactive);
		}
	}
	else
	{
		bool half_screen = false;
		if (controller_index == k_any_controller)
		{
			half_screen = true;
		}
		else
		{
			int32 user_index = controller_get(controller_index)->get_user_index();
			if (user_index != NONE)
			{
				int32 horizontal_window_count = 0;
				int32 vertical_window_count = 0;
				user_interface_get_number_of_render_windows(user_index, &horizontal_window_count, &vertical_window_count);
				half_screen = vertical_window_count == 1;
			}
		}
	
		if (c_scoreboard_load_screen_message* message = UI_MALLOC(c_scoreboard_load_screen_message, !half_screen + STRING_ID(gui, scoreboard),
			controller_index,
			user_interface_get_window_for_controller(controller_index),
			STRING_ID(gui, top_most),
			is_interactive))
		{
			user_interface_messaging_post(message);
		}
	}
}

//.text:00AB3D80 ; public: void c_static_array<c_gui_scoreboard_data::s_player_row, 25>::sort(int32, int(__cdecl*)(const void*, const void*))

void c_gui_scoreboard_data::update()
{
	//INVOKE_CLASS_MEMBER(0x00AB3DA0, c_gui_scoreboard_data, update);

	switch (m_current_scoreboard_mode)
	{
	case c_gui_screen_scoreboard::_scoreboard_mode_game:
	case c_gui_screen_scoreboard::_scoreboard_mode_game_film:
	{
		c_gui_scoreboard_data::update_for_scoreboard_mode(false, game_in_progress() && game_is_multiplayer());
	}
	break;
	case c_gui_screen_scoreboard::_scoreboard_mode_session:
	case c_gui_screen_scoreboard::_scoreboard_mode_session_film:
	{
		c_gui_scoreboard_data::update_for_scoreboard_mode(true, false);
	}
	break;
	}
}

void c_gui_screen_scoreboard::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00AB3E10, c_gui_screen_scoreboard, update, current_milliseconds);
}

//.text:00AB4060 ; private: void c_gui_screen_scoreboard::update_button_key()

void c_gui_scoreboard_data::update_for_scoreboard_mode(bool use_session, bool include_score)
{
	//INVOKE_CLASS_MEMBER(0x00AB42F0, c_gui_scoreboard_data, update_for_scoreboard_mode, use_session, include_score);

	static c_static_wchar_string<256> place_string;
	static c_static_wchar_string<256> score_string;
	static c_static_wchar_string<256> round_score_string;

	place_string.set(L" ");
	score_string.set(L" ");
	round_score_string.set(L" ");

	m_player_row_count = 0;
	m_player_rows.clear();

	bool include_team_score = include_score && game_engine_has_teams();
	int32 round_score = 0;
	uns32 team_flags = 0;

	if (use_session)
	{
		ASSERT(!include_score);

		for (int32 player_index = 0; player_index < k_maximum_players; player_index++)
		{
			if (user_interface_squad_is_player_valid(player_index))
			{
				s_player_configuration* player_data = user_interface_session_get_player_data(player_index);
				if (player_data->host.team_index != _game_team_none)
				{
					int32 player_netdebug_filled_bar_count = user_interface_session_get_player_netdebug_filled_bar_count(player_index);

					e_controller_index local_controller_index = k_no_controller;
					if (user_interface_session_is_local_player(player_index))
						local_controller_index = user_interface_session_get_controller_index(player_index);

					int32 base_color_index = player_data->host.armor.loadouts[player_data->host.armor.loadout_index].colors[0].value;

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
			int32 player_iterator_index = player_iterator.get_index();

			int32 player_index = user_interface_squad_get_player_index(&player->player_identifier);
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
					int32 finalized_place = game_engine_get_finalized_player_place(player_iterator_index);
					if (include_team_score)
						finalized_place = game_engine_get_finalized_team_place(player->configuration.host.team_index);

					game_engine_get_place_string(finalized_place, &place_string);
					int32 player_score = game_engine_get_player_score_for_display(player_iterator_index, true);
					game_engine_get_score_string(player_score, &score_string);
					int32 player_round_score = game_engine_get_player_score_for_display(player_iterator_index, false);
					game_engine_get_score_string(player_round_score, &round_score_string);
				}

				if (include_team_score)
					team_flags |= FLAG(player->configuration.host.team_index);

				if (player_index == NONE)
				{
					int32 base_color_index = player->configuration.host.armor.loadouts[player->armor_loadout_index].colors[0].value;
					int32 player_index = player_iterator_index;
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
					int32 player_netdebug_filled_bar_count = user_interface_session_get_player_netdebug_filled_bar_count(player_index);

					e_controller_index controller_index = k_no_controller;
					if (user_interface_session_is_local_player(player_index))
						controller_index = user_interface_session_get_controller_index(player_index);

					int32 base_color_index = player->configuration.host.armor.loadouts[player->armor_loadout_index].colors[0].value;

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

			for (int32 team_index = 0; team_index < k_multiplayer_team_count; team_index++)
			{
				if (game_engine_is_team_ever_active(team_index) || TEST_BIT(team_flags, team_index))
				{
					if (include_score)
					{
						int32 team_place = 0;
						int32 team_score = 0;

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

//.text:00AB44E0 ; private: void c_gui_screen_scoreboard::update_player_list()

void c_gui_screen_scoreboard::update_render_state(uns32 current_milliseconds)
{
	//INVOKE_CLASS_MEMBER(0x00AB4920, c_gui_screen_scoreboard, update_render_state, current_milliseconds);

	c_gui_widget::update_render_state(current_milliseconds);

	c_gui_list_widget* scoreboard_list_widget = get_child_list_widget(STRING_ID(gui, scoreboard));
	if (!scoreboard_list_widget)
	{
		return;
	}

	c_gui_scoreboard_data* scoreboard_data = (c_gui_scoreboard_data*)c_gui_screen_widget::get_data(STRING_ID(gui, scoreboard), NULL);
	if (scoreboard_data)
	{
		bool has_teams = game_engine_has_teams();

		for (c_gui_list_item_widget* list_item_widget = (c_gui_list_item_widget*)scoreboard_list_widget->get_first_child_widget_by_type(_gui_list_item);
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

			int32 element_handle = list_item_widget->get_element_handle();

			int32 player_row_type = 0;
			int32 base_color = 0;
			int32 team_color = NONE;

			if (!scoreboard_data->get_integer_value(element_handle, STRING_ID(gui, player_row_type), &player_row_type)
				|| !scoreboard_data->get_integer_value(element_handle, STRING_ID(gui, base_color), &base_color)
				|| !scoreboard_data->get_integer_value(element_handle, STRING_ID(gui, team_color), &team_color))
			{
				continue;
			}

			int32 color_list_index = base_color;
			if (has_teams)
			{
				color_list_index = team_color;
				tint_widget_to_change_color(base_color_bitmap_widget, color_list_index, has_teams);
			}
			else
			{
				base_color |= (255 << 24);
				tint_widget_to_change_argb_color(base_color_bitmap_widget, { .value = static_cast<uns32>(base_color) });
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

	c_gui_widget* button_key_child_list_widget = get_first_child_widget_by_type(_gui_button_key);
	if (button_key_child_list_widget)
	{
		int32 v18 = 0;
		int32 y19 = 0;
		for (c_gui_list_item_widget* list_item_widget = (c_gui_list_item_widget*)scoreboard_list_widget->get_first_child_widget_by_type(_gui_list_item);
			list_item_widget;
			list_item_widget = list_item_widget->get_next_list_item_widget(false))
		{
			real_rectangle2d current_bounds{};
			list_item_widget->get_current_bounds(&current_bounds);

			if (list_item_widget->get_element_handle() != NONE)
			{
				y19 = (int32)current_bounds.y0;
			}

			v18 = (int32)current_bounds.y0;
		}

		if (y19 != v18)
		{
			translate_widget_recursive(button_key_child_list_widget, 0, y19 - v18);
		}
	}
}

//.text:00AB4B90 ; private: void c_gui_screen_scoreboard::update_score_message()

void __cdecl c_gui_screen_scoreboard::update_scoreboard_alpha(e_controller_index controller_index)
{
	INVOKE(0x00AB4C50, c_gui_screen_scoreboard::update_scoreboard_alpha, controller_index);
}

