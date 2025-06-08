#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "text/unicode.hpp"

class c_scoreboard_load_screen_message :
	public c_load_screen_message
{
public:
	void set_is_interactive(bool is_interactive);

protected:
	bool m_is_interactive;
};
static_assert(sizeof(c_scoreboard_load_screen_message) == sizeof(c_load_screen_message) + 0x4);

class c_gui_screen_scoreboard :
	public c_gui_screen_widget
{
public:
	c_gui_screen_scoreboard(int32 name) :
		c_gui_screen_widget(name)
	{
		DECLFUNC(0x00AB2A90, void, __thiscall, c_gui_screen_scoreboard*, int32)(this, name);
	}

private:
	static void __cdecl translate_widget_recursive(c_gui_widget* widget, int32 x, int32 y);

public:
	enum e_scoreboard_mode;

	void set_is_interactive(bool is_interactive);
	static real32 __cdecl get_scoreboard_alpha(e_controller_index controller_index);
	static c_gui_screen_scoreboard* __cdecl get_scoreboard_screen(e_controller_index controller_index);
	static void __cdecl hide_scoreboard(e_controller_index controller_index);
	static bool __cdecl is_scoreboard_displayed(e_controller_index controller_index);
	static bool __cdecl is_scoreboard_interactive(e_controller_index controller_index);
	static void __cdecl show_scoreboard(e_controller_index controller_index, bool is_interactive);
	static void __cdecl update_scoreboard_alpha(e_controller_index controller_index);

	void __thiscall update_render_state_(uns32 current_milliseconds);

protected:
	int32 m_current_scoreboard_mode;
	bool m_is_interactive;

private:
	static real32(&m_scoreboard_alpha)[4];
	static real32& m_console_scoreboard_alpha;
};
static_assert(sizeof(c_gui_screen_scoreboard) == sizeof(c_gui_screen_widget) + 0x8);

class c_gui_scoreboard_data :
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
		_voice_state_none = 0,
		_voice_state_has_voice,
		_voice_state_talking,
		_voice_state_away_in_private_chat,
		_voice_state_muted,
	};

	struct s_player_row
	{
		e_player_row_type player_row_type;

		int32 game_player_index;
		int32 session_player_index;

		s_player_appearance player_appearance;
		c_static_wchar_string<48> player_name;
		c_static_wchar_string<48> service_tag;

		// `base_color_index` -> `color_list_index`
		// tint_color = user_interface_shared_tag_globals->tint_colors
		// color_list = tint_color->text_player[color_list_index]
		// color_list = tint_color->text_team[color_list_index]
		// color_list = tint_color->bitmap_player[color_list_index]
		// color_list = tint_color->bitmap_team[color_list_index]
		// color = color_list.color
		int32 base_color_index;

		int32 team_index;
		bool show_team;

		e_controller_index local_controller_index;
		e_voice_talking_state voice_state;
		int32 connectivity_rating;

		c_static_wchar_string<48> place;
		c_static_wchar_string<48> score;
		c_static_wchar_string<48> round_score;

		bool is_dead;
		bool left_game;
	};
	static_assert(sizeof(s_player_row) == 0x868);

	using scoreboard_sort_proc_t = int __cdecl(const void*, const void*);

	bool __cdecl add_player_internal(
		e_player_row_type player_row_type,
		int32 game_player_index,
		int32 session_player_index,
		const s_player_appearance* appearance,
		const wchar_t* player_name,
		const wchar_t* service_tag,
		int32 base_color_index,
		int32 team_index,
		bool show_team,
		e_controller_index controller_index,
		e_voice_talking_state voice_state,
		int32 connectivity_rating,
		const wchar_t* place,
		const wchar_t* score,
		const wchar_t* round_score,
		bool is_dead,
		bool left_game);

	void __thiscall update_for_scoreboard_mode_(bool use_session, bool include_score);
	static int __cdecl scoreboard_sort_proc_for_multiplayer(const void* a, const void* b);
	static int __cdecl scoreboard_sort_proc_for_session(const void* a, const void* b);

//protected:
	int32 m_current_scoreboard_mode;
	c_static_array<s_player_row, 25> m_player_rows;
	int32 m_player_row_count;
	c_enum<e_controller_index, int32, _controller0, k_number_of_controllers> m_controller_index;
};
static_assert(sizeof(c_gui_scoreboard_data) == sizeof(c_gui_ordered_data) + 0xD234);

extern c_scoreboard_load_screen_message* scoreboard_load_screen_message_ctor(c_scoreboard_load_screen_message* message,
	int32 screen_name,
	e_controller_index controller,
	e_window_index window,
	int32 layered_position,
	bool is_interactive);

