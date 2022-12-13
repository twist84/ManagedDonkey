#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"

#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "game/game_engine.hpp"
#include "game/game_engine_display.hpp"
#include "game/game_engine_team.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "simulation/simulation.hpp"

#include <cstdlib> // qsort

REFERENCE_DECLARE_ARRAY(0x05269788, real, c_gui_screen_scoreboard::m_scoreboard_alpha, 4);
REFERENCE_DECLARE(0x05269798, real, c_gui_screen_scoreboard::m_console_scoreboard_alpha);

HOOK_DECLARE(0x00AB3DA0, gui_scoreboard_data_update);

real __cdecl c_gui_screen_scoreboard::get_scoreboard_alpha(e_controller_index controller_index)
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
}

void __cdecl c_gui_screen_scoreboard::update_scoreboard_alpha(e_controller_index controller_index)
{
	INVOKE(0x00AB4C50, c_gui_screen_scoreboard::update_scoreboard_alpha, controller_index);
}

bool __cdecl c_gui_scoreboard_data::add_player_internal(
	e_player_row_type row_type,
	long player_index,
	long network_player_index,
	s_player_appearance const* appearance,
	wchar_t const* player_name,
	wchar_t const* service_tag,
	long base_color,
	long multiplayer_team,
	bool team_game,
	e_controller_index controller_index,
	long voice_talking_state,
	long connectivity_rating,
	wchar_t const* place,
	wchar_t const* score,
	wchar_t const* round_score,
	bool dead,
	bool left
)
{
	bool result = m_player_row_count < 25;
	if (result)
	{
		m_player_rows[m_player_row_count].player_row_type = row_type;
		m_player_rows[m_player_row_count].player_index = player_index;
		m_player_rows[m_player_row_count].network_player_index = network_player_index;
		csmemcpy(&m_player_rows[m_player_row_count].player_appearance, appearance, sizeof(s_player_appearance));
		m_player_rows[m_player_row_count].player_name.set(player_name);
		m_player_rows[m_player_row_count].service_tag.set(service_tag);
		m_player_rows[m_player_row_count].base_color = base_color;
		m_player_rows[m_player_row_count].multiplayer_team = multiplayer_team;
		m_player_rows[m_player_row_count].team_game = team_game;
		m_player_rows[m_player_row_count].controller_index = controller_index;
		m_player_rows[m_player_row_count].voice_talking_state = voice_talking_state;
		m_player_rows[m_player_row_count].connectivity_rating = connectivity_rating;
		m_player_rows[m_player_row_count].place.set(place);
		m_player_rows[m_player_row_count].score.set(score);
		m_player_rows[m_player_row_count].round_score.set(round_score);
		m_player_rows[m_player_row_count].dead = dead;
		m_player_rows[m_player_row_count].left = left;

		m_player_row_count++;
	}

	return result;
}

void __cdecl c_gui_scoreboard_data::update_for_scoreboard_mode(bool a1, bool include_score)
{
	c_static_wchar_string<256> place;
	c_static_wchar_string<256> score;
	c_static_wchar_string<256> round_score;

	place.set(L" ");
	score.set(L" ");
	round_score.set(L" ");

	m_player_row_count = 0;
	m_player_rows.clear();

	bool team_game = game_engine_has_teams();
	bool include_team_score = include_score && team_game;
	long team_round_score = 0;
	dword_flags team_flags = 0;

	if (a1)
	{
		for (long session_player_index = 0; session_player_index < 16; ++session_player_index)
		{
			if (user_interface_squad_is_player_valid(session_player_index))
			{
				s_player_configuration* player_data = user_interface_session_get_player_data(session_player_index);
				if (player_data->host.team_index != -1)
				{
					long player_rating = -1;
					e_controller_index controller_index = k_no_controller;
					if (user_interface_session_is_local_player(session_player_index))
						controller_index = user_interface_session_get_controller_index(session_player_index);

					long base_color = -1;
					long voice_for_player = 0;

					add_player_internal(
						/* row_type             */ _player_row_type_player,
						/* player_index         */ -1,
						/* network_player_index */ session_player_index,
						/* appearance           */ &player_data->host.appearance,
						/* player_name          */ player_data->host.name.get_string(),
						/* service_tag          */ player_data->host.appearance.service_tag.get_string(),
						/* base_color           */ base_color,
						/* multiplayer_team     */ -1,
						/* team_game            */ false,
						/* controller_index     */ controller_index,
						/* voice_talking_state  */ voice_for_player,
						/* connectivity_rating  */ player_rating,
						/* place                */ place.get_string(),
						/* score                */ score.get_string(),
						/* round_score          */ round_score.get_string(),
						/* dead                 */ false,
						/* left                 */ false
					);
				}
			}
		}
	}
	else
	{
		s_data_iterator player_iterator(get_tls()->player_data);
		player_datum* player = (player_datum*)data_iterator_next(&player_iterator);
		if (player)
		{
			while (true)
			{
				long network_player_index = user_interface_squad_get_player_index(&player->player_identifier);
				if (player->configuration.host.team_index != -1)
				{
					bool dead = !TEST_BIT(player->player_flags, 3) && player->unit_index == -1 && game_in_progress() && !game_is_finished() && !simulation_starting_up() && game_engine_in_round();

					if (include_score)
					{
						long finalized_place = game_engine_get_player_place(player_iterator.index);
						if (include_team_score)
							finalized_place = game_engine_get_team_place(player->configuration.host.team_index);

						game_engine_get_place_string(finalized_place, &place);
						long player_score = game_engine_get_player_score_for_display(player_iterator.index, true);
						game_engine_get_score_string(player_score, &score);
						long player_round_score = game_engine_get_player_score_for_display(player_iterator.index, false);
						game_engine_get_score_string(player_round_score, &round_score);
					}

					if (include_team_score)
						team_flags |= 1 << player->configuration.host.team_index;

					if (network_player_index != -1)
					{
						long player_rating = -1;

						e_controller_index controller_index = k_no_controller;
						if (user_interface_session_is_local_player(network_player_index))
							controller_index = user_interface_session_get_controller_index(network_player_index);

						long base_color = -1;
						long voice_for_player = 0;

						add_player_internal(
							/* row_type             */ _player_row_type_player,
							/* player_index         */ -1,
							/* network_player_index */ network_player_index,
							/* appearance           */ &player->configuration.host.appearance,
							/* player_name          */ player->configuration.host.name.get_string(),
							/* service_tag          */ player->configuration.host.appearance.service_tag.get_string(),
							/* base_color           */ base_color,
							/* multiplayer_team     */ player->configuration.host.team_index,
							/* team_game            */ team_game,
							/* controller_index     */ controller_index,
							/* voice_talking_state  */ voice_for_player,
							/* connectivity_rating  */ player_rating,
							/* place                */ place.get_string(),
							/* score                */ score.get_string(),
							/* round_score          */ round_score.get_string(),
							/* dead                 */ dead,
							/* left                 */ false
						);
						team_round_score = 0;
					}
					else
					{
						long base_color = -1;
						long player_index = player_iterator.index;
						bool left = TEST_BIT(player->player_flags, 1);
						team_round_score = 0;

						c_gui_scoreboard_data::add_player_internal(
							/* row_type             */ _player_row_type_player,
							/* player_index         */ player_index,
							/* network_player_index */ -1,
							/* appearance           */ &player->configuration.host.appearance,
							/* player_name          */ player->configuration.host.name.get_string(),
							/* service_tag          */ player->configuration.host.appearance.service_tag.get_string(),
							/* base_color           */ base_color,
							/* multiplayer_team     */ player->configuration.host.team_index,
							/* team_game            */ team_game,
							/* controller_index     */ k_no_controller,
							/* voice_talking_state  */ 0,
							/* connectivity_rating  */ -1,
							/* place                */ place.get_string(),
							/* score                */ score.get_string(),
							/* round_score          */ round_score.get_string(),
							/* dead                 */ dead,
							/* left                 */ left
						);
					}
				}

				player = (player_datum*)data_iterator_next(&player_iterator);
				if (!player)
					break;
			}
		}

		if (include_team_score)
		{
			s_player_appearance player_appearance; // for emblem?
			c_static_wchar_string<256> team_name;

			csmemset(&player_appearance, 0, sizeof(s_player_appearance));
			team_name.clear();

			long team_mask = 1;
			for (long team_index = 0; team_index < 8; team_index++)
			{
				if (game_engine_is_team_ever_active(team_index) || (team_mask & team_flags) != 0)
				{
					if (include_score)
					{
						long team_place = 0;
						long team_score = 0;

						if (game_engine_is_team_ever_active(team_index))
						{
							team_place = game_engine_get_team_place(team_index);
							team_score = game_engine_get_team_score_for_display(team_index, true);
							team_round_score = game_engine_get_team_score_for_display(team_index, false);
						}

						game_engine_get_place_string(team_place, &place);
						game_engine_get_score_string(team_score, &score);
						game_engine_get_score_string(team_round_score, &round_score);
					}
					game_engine_get_team_name(team_index, &team_name);

					add_player_internal(
						/* row_type             */ _player_row_type_team_bar,
						/* player_index         */ -1,
						/* network_player_index */ -1,
						/* appearance           */ &player_appearance,
						/* player_name          */ team_name.get_string(),
						/* service_tag          */ L" ",
						/* base_color           */ -1,
						/* multiplayer_team     */ team_index,
						/* team_game            */ true,
						/* controller_index     */ k_no_controller,
						/* voice_talking_state  */ 0,
						/* connectivity_rating  */ -1,
						/* place                */ place.get_string(),
						/* score                */ score.get_string(),
						/* round_score          */ round_score.get_string(),
						/* dead                 */ false,
						/* left                 */ false
					);
				}

				team_mask = __ROL4__(team_mask, 1);
			}
		}
	}

	int(__cdecl * scoreboard_sort_proc)(void const*, void const*) = reinterpret_cast<decltype(scoreboard_sort_proc)>(a1 ? 0x00AB3A00 : 0x00AB38A0);
	qsort(m_player_rows.m_storage, m_player_row_count, sizeof(s_player_row), scoreboard_sort_proc);
}

void __fastcall gui_scoreboard_data_update(c_gui_scoreboard_data* _this)
{
	long mode = _this->m_current_scoreboard_mode;
	if (mode == 1 || mode == 2)
	{
		if (game_in_progress() && game_is_multiplayer())
			_this->update_for_scoreboard_mode(false, true);
		else
			_this->update_for_scoreboard_mode(false, false);
	}
	else if (mode == 3 || mode == 4)
	{
		_this->update_for_scoreboard_mode(true, false);
	}
}

