#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

struct c_gui_screen_scoreboard :
	c_gui_screen_widget
{
private:
	static void __cdecl translate_widget_recursive(c_gui_widget* widget, long a2, long a3);

public:
	enum e_scoreboard_mode;

	static real __cdecl get_scoreboard_alpha(e_controller_index controller_index);
	static c_gui_screen_scoreboard* __cdecl get_scoreboard_screen(e_controller_index controller_index);
	static void __cdecl hide_scoreboard(e_controller_index controller_index);
	static bool __cdecl is_scoreboard_displayed(e_controller_index controller_index);
	static bool __cdecl is_scoreboard_interactive(e_controller_index controller_index);
	static void __cdecl show_scoreboard(e_controller_index controller_index, bool is_interactive);
	static void __cdecl update_scoreboard_alpha(e_controller_index controller_index);

	void __thiscall _update_render_state(dword a1);

protected:
	long m_current_scoreboard_mode;
	bool m_is_interactive;

private:
	static real(&m_scoreboard_alpha)[4];
	static real& m_console_scoreboard_alpha;
};

struct c_gui_scoreboard_data :
	c_gui_ordered_data
{
	enum e_player_row_type
	{
		_player_row_type_player = 0,
		_player_row_type_team_bar,

		k_player_row_type_count
	};

	enum e_voice_talking_state
	{
	};

	struct s_player_row
	{
		c_enum<e_player_row_type, long, _player_row_type_player, k_player_row_type_count> player_row_type;

		long player_index;
		long network_player_index;

		s_player_appearance player_appearance;
		c_static_wchar_string<48> player_name;
		c_static_wchar_string<48> service_tag;

		// `base_color` -> `color_list_index`
		// tint_color = user_interface_shared_tag_globals->tint_colors
		// color_list = tint_color->text_player[color_list_index]
		// color_list = tint_color->text_team[color_list_index]
		// color_list = tint_color->bitmap_player[color_list_index]
		// color_list = tint_color->bitmap_team[color_list_index]
		// color = color_list.color
		long base_color;

		long multiplayer_team;
		bool team_game;

		e_controller_index controller_index;
		long voice_talking_state;
		long connectivity_rating;

		c_static_wchar_string<48> place;
		c_static_wchar_string<48> score;
		c_static_wchar_string<48> round_score;

		bool dead;
		bool left;
	};
	static_assert(sizeof(s_player_row) == 0x868);

	bool __cdecl add_player_internal(
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
		bool left);

	void __cdecl update_for_scoreboard_mode(bool a1, bool a2);
	void __thiscall _update();

//protected:
	long m_current_scoreboard_mode;
	c_static_array<s_player_row, 25> m_player_rows;
	long m_player_row_count;
	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> m_controller_index;
};
static_assert(sizeof(c_gui_scoreboard_data) == 0xD340);

