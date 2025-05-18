#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_list_widget.hpp"

struct c_gui_roster_list_widget :
	c_gui_list_widget
{
public:
	void __thiscall update_(uns32 current_milliseconds);
	void __thiscall update_render_state_(uns32 current_milliseconds);

public:
	virtual ~c_gui_roster_list_widget();
	virtual void update(uns32 current_milliseconds) override;
	virtual void update_render_state(uns32 current_milliseconds) override;
	
	c_gui_roster_list_widget(bool game_roster_is_local_party, bool remote_team_game);
	void update_team_mode();
	
protected:
	bool m_game_roster_is_local_party_internal;
	bool m_is_team_game_internal;
	bool m_team_switching_enabled;
	bool m_show_party_bar_internal;

	struct
	{
		bool temporary_team_change_active;
		int32 desired_team;
		int32 set_time;
		int32 lying_end_time;
	} m_temporary_team[16];

	byte __data[0x4];
};
static_assert(sizeof(c_gui_roster_list_widget) == sizeof(c_gui_list_widget) + 0x108);

