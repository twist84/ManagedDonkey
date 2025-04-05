#include "interface/gui_screens/campaign/gui_screen_campaign_select_level.hpp"

void c_gui_screen_campaign_select_level::setup(e_gui_campaign_level_setup_mode campaign_setup_mode, long campaign_id, long map_id, short campaign_insertion_point)
{
	//INVOKE_CLASS_MEMBER(0x00AFDBA0, c_gui_screen_campaign_select_level, setup, campaign_setup_mode, campaign_id, map_id, campaign_insertion_point);

	m_campaign_setup_mode = campaign_setup_mode;
	m_campaign_id = campaign_id;
	m_map_id = map_id;
	m_campaign_insertion_point = campaign_insertion_point;
}

