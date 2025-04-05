#include "interface/gui_screens/campaign/gui_screen_campaign_select_difficulty.hpp"

//void c_gui_screen_campaign_select_difficulty::setup(e_gui_campaign_difficulty_setup_mode campaign_setup_mode, e_campaign_id campaign_id, e_map_id map_id, e_campaign_difficulty_level difficulty)
void c_gui_screen_campaign_select_difficulty::setup(e_gui_campaign_difficulty_setup_mode campaign_setup_mode, long campaign_id, long map_id, e_campaign_difficulty_level difficulty)
{
	//INVOKE_CLASS_MEMBER(0x00AFC830, c_gui_screen_campaign_select_difficulty, setup, campaign_setup_mode, campaign_id, map_id, difficulty);

	m_campaign_setup_mode = campaign_setup_mode;
	m_campaign_id = campaign_id;
	m_map_id = map_id;
	m_difficulty = difficulty;
}

