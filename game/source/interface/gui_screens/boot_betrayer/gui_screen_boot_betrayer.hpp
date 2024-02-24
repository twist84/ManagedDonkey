#pragma once

#include "game/players.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface_messages.hpp"

struct c_gui_screen_boot_betrayer :
	public c_gui_screen_widget
{
public:
	void set_current_player_id(s_player_identifier const* current_player_id);

	s_player_identifier const* get_target_player_id();
	void set_target_player_id(s_player_identifier const* target_player_id);

protected:
	s_player_identifier m_current_player_id;
	s_player_identifier m_target_player_id;
	bool m_target_player_booted;

	byte __pad1AB1[7];
};
static_assert(sizeof(c_gui_screen_boot_betrayer) == sizeof(c_gui_screen_widget) + 0x18);

struct c_load_boot_betrayer_screen_message :
	public c_load_screen_message
{
public:
	c_load_boot_betrayer_screen_message(e_controller_index controller, e_window_index window, long layered_position, s_player_identifier const* current_player_id, s_player_identifier const* target_player_id);
	virtual ~c_load_boot_betrayer_screen_message();
	virtual void apply_initial_state(c_gui_screen_widget* screen) const;

protected:
	s_player_identifier m_current_player_id;
	s_player_identifier m_target_player_id;
};
static_assert(sizeof(c_load_boot_betrayer_screen_message) == sizeof(c_load_screen_message) + 0x10);

