#include "interface/gui_screens/campaign/gui_screen_campaign_select_difficulty.hpp"

//.text:00AFC2D0 ; public: c_gui_screen_campaign_select_difficulty::c_gui_screen_campaign_select_difficulty(int32)
//.text:00AFC310 ; public: virtual c_gui_screen_campaign_select_difficulty::~c_gui_screen_campaign_select_difficulty()
//.text:00AFC320 ; public: virtual void* c_gui_screen_campaign_select_difficulty::`scalar deleting destructor'(unsigned int)
//.text:00AFC350 ; private: void c_gui_screen_campaign_select_difficulty::difficulty_chosen_immediate(e_controller_index)
//.text:00AFC5A0 ; public: virtual bool c_gui_screen_campaign_select_difficulty::handle_dialog_result(const c_dialog_result_message*)
//.text:00AFC620 ; public: virtual bool c_gui_screen_campaign_select_difficulty::handle_list_item_chosen(const c_controller_input_message*, int32, c_gui_list_item_widget*, c_gui_data*)
//.text:00AFC770 ; public: virtual void c_gui_screen_campaign_select_difficulty::initialize()
//.text:00AFC790 ; public: virtual void c_gui_screen_campaign_select_difficulty::initialize_datasource()

void c_gui_screen_campaign_select_difficulty::setup(e_gui_campaign_difficulty_setup_mode campaign_setup_mode, e_campaign_id campaign_id, e_map_id map_id, e_campaign_difficulty_level difficulty)
{
	//INVOKE_CLASS_MEMBER(0x00AFC830, c_gui_screen_campaign_select_difficulty, setup, campaign_setup_mode, campaign_id, map_id, difficulty);

	m_campaign_setup_mode = campaign_setup_mode;
	m_campaign_id = campaign_id;
	m_map_id = map_id;
	m_difficulty = difficulty;
}

//.text:00AFC860 ; public: virtual void c_gui_screen_campaign_select_difficulty::update(uns32)

