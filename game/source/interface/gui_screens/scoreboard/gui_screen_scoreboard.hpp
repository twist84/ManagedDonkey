#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface_data.hpp"
#include "interface/user_interface_messages.hpp"
#include "text/unicode.hpp"

class c_gui_screen_scoreboard :
	public c_gui_screen_widget
{
public:
	enum e_scoreboard_mode
	{
		_scoreboard_mode_none = 0,
		_scoreboard_mode_game,
		_scoreboard_mode_game_film,
		_scoreboard_mode_session,
		_scoreboard_mode_session_film,
	};

public:
	void __thiscall update_render_state_(uns32 current_milliseconds);

public:
	virtual ~c_gui_screen_scoreboard() = default;
	virtual void post_initialize() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual void update_render_state(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;

protected:
	virtual e_render_data_size get_render_data_size() override;

	virtual void initialize_datasource() override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;

public:
	c_gui_screen_scoreboard(int32 name);

private:
	static void __cdecl translate_widget_recursive(c_gui_widget* widget, int32 x, int32 y);

public:
	static real32 __cdecl get_scoreboard_alpha(e_controller_index controller_index);
	static c_gui_screen_scoreboard* __cdecl get_scoreboard_screen(e_controller_index controller_index);
	static void __cdecl hide_scoreboard(e_controller_index controller_index);
	static bool __cdecl is_scoreboard_displayed(e_controller_index controller_index);
	static bool __cdecl is_scoreboard_interactive(e_controller_index controller_index);
	void set_is_interactive(bool value);

private:
	void set_scoreboard_mode(e_scoreboard_mode scoreboard_mode);

public:
	static void __cdecl show_scoreboard(e_controller_index controller_index, bool is_interactive);
	static void __cdecl update_scoreboard_alpha(e_controller_index controller_index);

protected:
	int32 m_current_scoreboard_mode;
	bool m_is_interactive;

private:
	static real32(&m_scoreboard_alpha)[4];
	static real32& m_console_scoreboard_alpha;
};
COMPILE_ASSERT(sizeof(c_gui_screen_scoreboard) == 0x1AA8);
COMPILE_ASSERT(sizeof(c_gui_screen_scoreboard) == sizeof(c_gui_screen_widget) + 0x8);

class c_scoreboard_load_screen_message :
	public c_load_screen_message
{
public:
	virtual ~c_scoreboard_load_screen_message() = default;
	virtual void apply_initial_state(c_gui_screen_widget* screen_widget) const override;

public:
	c_scoreboard_load_screen_message(int32 screen_name, e_controller_index controller, e_window_index window, int32 layered_position, bool is_interactive);

protected:
	bool m_is_interactive;
};
COMPILE_ASSERT(sizeof(c_scoreboard_load_screen_message) == 0x40);
COMPILE_ASSERT(sizeof(c_scoreboard_load_screen_message) == sizeof(c_load_screen_message) + 0x4);

class c_gui_scoreboard_data :
	public c_gui_ordered_data
{
public:
	void __thiscall update_for_scoreboard_mode_(bool use_session, bool include_score);

public:
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
		s_player_row();

		e_player_row_type player_row_type;
		int32 game_player_index;
		int32 session_player_index;
		s_player_appearance appearance;
		c_static_wchar_string<48> name;
		c_static_wchar_string<48> service_tag;
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
	COMPILE_ASSERT(sizeof(s_player_row) == 0x868);

	using scoreboard_sort_proc_t = int __cdecl(const void*, const void*);

public:
	virtual ~c_gui_scoreboard_data() = default;
	virtual void update() override;
	virtual void get_column_names(string_id* const column_names, int32* column_count) override;
	virtual bool get_integer_value(int32 element_handle, string_id value_name, int32* value) override;
	virtual bool get_text_value(int32 element_handle, string_id value_name, c_static_wchar_string<1024>* buffer) override;
	virtual bool get_player_appearance(int32 element_handle, s_player_appearance* appearance) override;

protected:
	virtual int32 get_current_item_count_internal() override;

private:
	bool add_player_internal(
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

	static int __cdecl scoreboard_sort_proc_for_multiplayer(const void* a, const void* b);
	static int __cdecl scoreboard_sort_proc_for_session(const void* a, const void* b);

public:
	void set_scoreboard_mode(c_gui_screen_scoreboard::e_scoreboard_mode value);
	void update_for_scoreboard_mode(bool use_session, bool include_score);

//protected:
	c_gui_screen_scoreboard::e_scoreboard_mode m_current_scoreboard_mode;
	c_static_array<s_player_row, 25> m_player_rows;
	int32 m_player_row_count;
	e_controller_index m_driving_controller;
};
COMPILE_ASSERT(sizeof(c_gui_scoreboard_data) == 0xD340);
COMPILE_ASSERT(sizeof(c_gui_scoreboard_data) == sizeof(c_gui_ordered_data) + 0xD234);

