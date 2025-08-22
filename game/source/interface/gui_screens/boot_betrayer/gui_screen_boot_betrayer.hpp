#pragma once

#include "game/players.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface_messages.hpp"

class c_gui_screen_boot_betrayer :
	public c_gui_screen_widget
{
public:
	c_gui_screen_boot_betrayer(int32 name) :
		c_gui_screen_widget(name)
	{
		DECLFUNC(0x00AB5410, void, __thiscall, c_gui_screen_boot_betrayer*, int32)(this, name);
	}

	void set_current_player_id(const s_player_identifier* current_player_id);

	const s_player_identifier* get_target_player_id() const;
	void set_target_player_id(const s_player_identifier* target_player_id);

protected:
	s_player_identifier m_current_player_id;
	s_player_identifier m_target_player_id;
	bool m_target_player_booted;

	byte __pad1AB1[0x7];
};
static_assert(sizeof(c_gui_screen_boot_betrayer) == sizeof(c_gui_screen_widget) + 0x18);

class c_load_boot_betrayer_screen_message :
	public c_load_screen_message
{
public:
	virtual ~c_load_boot_betrayer_screen_message() = default;
	virtual void apply_initial_state(c_gui_screen_widget* screen_widget) const override;

public:
	c_load_boot_betrayer_screen_message(e_controller_index controller, e_window_index window, int32 layered_position, const s_player_identifier* current_player_id, const s_player_identifier* target_player_id);

protected:
	s_player_identifier m_current_player_id;
	s_player_identifier m_target_player_id;
};
static_assert(sizeof(c_load_boot_betrayer_screen_message) == sizeof(c_load_screen_message) + 0x10);

