#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

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

// `add_player_internal` function is used only once in `update_for_scoreboard_mode`
// `add_player_internal` is only setup for teams and not players, TODO reimplement `update_for_scoreboard_mode` for all types
bool __fastcall gui_scoreboard_data_add_player_internal(
	c_gui_scoreboard_data* _this,
	void* unused,
	c_gui_scoreboard_data::e_player_row_type row_type,
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
	bool result = _this->add_player_internal(
		row_type,
		player_index,
		network_player_index,
		appearance,
		player_name,
		service_tag,
		base_color,
		multiplayer_team,
		team_game,
		controller_index,
		voice_output,
		connectivity_rating,
		place,
		score,
		round_score,
		dead,
		left
	);
	return result;
}
HOOK_DECLARE(0x00AB2C30, gui_scoreboard_data_add_player_internal);

