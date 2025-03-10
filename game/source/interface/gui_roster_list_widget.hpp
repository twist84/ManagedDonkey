#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_list_widget.hpp"

struct c_gui_roster_list_widget :
	c_gui_list_widget
{
public:
	void __thiscall update(dword a1);
	void __thiscall update_render_state(dword a1);

	void update_team_mode();

protected:
	bool m_game_roster_is_local_party;
	bool m_is_team_game_internal;
	bool m_team_switching_enabled;
	bool m_show_party_bar;

	struct
	{
		bool temporary_team_change_active;
		long temporary_team_index;
		dword lying_end_time;
		dword lying_begin_time;
	} m_temporary_team[16];

	byte __data[4];
};
static_assert(sizeof(c_gui_roster_list_widget) == sizeof(c_gui_list_widget) + 0x108);

