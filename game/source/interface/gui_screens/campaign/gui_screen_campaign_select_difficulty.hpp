#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"

enum e_campaign_difficulty_level;
enum e_campaign_id;
enum e_gui_campaign_difficulty_setup_mode;
enum e_map_id;
struct c_gui_screen_campaign_select_difficulty :
	public c_gui_screen_widget
{
public:
	c_gui_screen_campaign_select_difficulty(long name) :
		c_gui_screen_widget(name)
	{
		DECLFUNC(0x00AFC2D0, c_gui_screen_campaign_select_difficulty*, __thiscall, c_gui_screen_campaign_select_difficulty*, long)(this, name);
	}

	void setup(e_gui_campaign_difficulty_setup_mode campaign_setup_mode, e_campaign_id campaign_id, e_map_id map_id, e_campaign_difficulty_level difficulty);

//protected:
	e_gui_campaign_difficulty_setup_mode m_campaign_setup_mode;
	e_campaign_id m_campaign_id;
	e_map_id m_map_id;
	e_campaign_difficulty_level m_difficulty;
};
static_assert(sizeof(c_gui_screen_campaign_select_difficulty) == sizeof(c_gui_screen_widget) + 0x10);

