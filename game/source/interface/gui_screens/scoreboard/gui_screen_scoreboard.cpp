#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"

#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "game/game_engine.hpp"
#include "game/game_engine_display.hpp"
#include "game/game_engine_team.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/module.hpp"

#include <cstdlib> // qsort

REFERENCE_DECLARE_ARRAY(0x05269788, real, c_gui_screen_scoreboard::m_scoreboard_alpha, 4);
REFERENCE_DECLARE(0x05269798, real, c_gui_screen_scoreboard::m_console_scoreboard_alpha);

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
	long voice_output,
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
        m_player_rows[m_player_row_count].voice_output = voice_output;
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

// #TODO: reimplement missing functionality
void __cdecl c_gui_scoreboard_data::update_for_scoreboard_mode(bool a1, bool a2)
{
	c_static_wchar_string<256> place;
	c_static_wchar_string<256> score;
	c_static_wchar_string<256> round_score;

	place.set(L" ");
	score.set(L" ");
	round_score.set(L" ");

	m_player_row_count = 0;
	m_player_rows.clear();

	if (a1)
	{
		for (long session_player_index = 0; session_player_index < 16; ++session_player_index)
			user_interface_squad_is_player_valid(session_player_index);
	}

	bool v5 = a2 && game_engine_has_teams();
	if (v5)
	{
		s_player_appearance player_appearance;
		c_static_wchar_string<256> team_name;

		csmemset(&player_appearance, 0, sizeof(s_player_appearance));
		team_name.clear();

		long v8 = 1;
		for (long team_index = 0; team_index < 8; team_index++)
		{
			if (game_engine_is_team_ever_active(team_index))
			{
				if (a2)
				{
					long team_place = 0;
					long player_score = 0;
					long team_score = 0;
					if (game_engine_is_team_ever_active(team_index))
					{
						team_place = game_engine_get_team_place(team_index);
						player_score = game_engine_get_team_score_for_display(team_index, 1);
						team_score = game_engine_get_team_score_for_display(team_index, 0);
					}
					game_engine_get_place_string(team_place, &place);
					game_engine_get_score_string(player_score, &score);
					game_engine_get_score_string(team_score, &round_score);
				}
				game_engine_get_team_name(team_index, &team_name);
				add_player_internal(_player_row_type_team_bar, -1, -1, &player_appearance, team_name.get_string(), L" ", -1, team_index, 1, k_no_controller, 0, -1, place.get_string(), score.get_string(), round_score.get_string(), 0, 0);
			}

			v8 = __ROL4__(v8, 1);
		}
	}

	int(__cdecl* scoreboard_sort_proc)(void const*, void const*) = reinterpret_cast<decltype(scoreboard_sort_proc)>(a1 ? 0x00AB3A00 : 0x00AB38A0);
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
HOOK_DECLARE(0x00AB3DA0, gui_scoreboard_data_update);

