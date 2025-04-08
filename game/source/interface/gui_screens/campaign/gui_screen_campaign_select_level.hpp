#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_screen_widget.hpp"

enum e_campaign_id;
enum e_controller_index;
enum e_gui_campaign_level_setup_mode;
enum e_map_id;

struct c_gui_screen_campaign_select_level :
	public c_gui_screen_widget
{
public:
	void __thiscall post_initialize_();

	c_gui_screen_campaign_select_level(long name) :
		c_gui_screen_widget(name)
	{
		DECLFUNC(0x00AFCA00, c_gui_screen_campaign_select_level*, __thiscall, c_gui_screen_campaign_select_level*, long)(this, name);
	}

	void setup(e_gui_campaign_level_setup_mode campaign_setup_mode, e_campaign_id campaign_id, e_map_id map_id, short campaign_insertion_point);

//protected:
	e_gui_campaign_level_setup_mode m_campaign_setup_mode;
	e_campaign_id m_campaign_id;
	e_map_id m_map_id;
	short m_campaign_insertion_point;
	e_controller_index m_controller_index;
	bool m_saved_game;
	bool m_coop_saved_game;
};
static_assert(sizeof(c_gui_screen_campaign_select_level) == sizeof(c_gui_screen_widget) + 0x18);

