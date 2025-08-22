#include "interface/gui_screens/boot_betrayer/gui_screen_boot_betrayer.hpp"

#include "interface/user_interface_mapping.hpp"
#include "tag_files/string_ids.hpp"

//00AB5450
c_load_boot_betrayer_screen_message::c_load_boot_betrayer_screen_message(e_controller_index controller, e_window_index window, int32 layered_position, const s_player_identifier* current_player_id, const s_player_identifier* target_player_id) :
	c_load_screen_message(STRING_ID(gui, boot_betrayer) + (user_interface_get_local_user_window_count() > 1), controller, window, layered_position),
	m_current_player_id(*current_player_id),
	m_target_player_id(*target_player_id)
{
}

//.text:00AB54B0 ; public: virtual c_gui_screen_boot_betrayer::~c_gui_screen_boot_betrayer()
//.text:00AB54C0 ; 
//.text:00AB54D0 ; public: virtual void* c_gui_screen_boot_betrayer::`vector deleting destructor'(unsigned int)
//.text:00AB5500 ; public: virtual void* c_load_boot_betrayer_screen_message::`vector deleting destructor'(unsigned int)

void c_load_boot_betrayer_screen_message::apply_initial_state(c_gui_screen_widget* screen_widget) const
{
	//INVOKE_CLASS_MEMBER(0x00AB5530, c_load_boot_betrayer_screen_message, apply_initial_state, screen);

	c_gui_screen_boot_betrayer* boot_betrayer = (c_gui_screen_boot_betrayer*)screen_widget;

	boot_betrayer->set_current_player_id(&m_current_player_id);
	boot_betrayer->set_target_player_id(&m_target_player_id);
}

//.text:00AB5560 ; private: void c_gui_screen_boot_betrayer::boot_player()
//.text:00AB5660 ; public: virtual void c_gui_screen_boot_betrayer::dispose()

const s_player_identifier* c_gui_screen_boot_betrayer::get_target_player_id() const
{
	//return INVOKE_CLASS_MEMBER(0x00AB5690, c_gui_screen_boot_betrayer, get_target_player_id);

	return &m_target_player_id;
}

//.text:00AB56A0 ; public: virtual bool c_gui_screen_boot_betrayer::handle_controller_input_message(const c_controller_input_message*)
//.text:00AB5760 ; public: virtual bool c_gui_screen_boot_betrayer::handle_dialog_result(const c_dialog_result_message*)
//.text:00AB57A0 ; public: virtual void c_gui_screen_boot_betrayer::initialize()
//.text:00AB5820 ; parse_griefer_name

void c_gui_screen_boot_betrayer::set_current_player_id(const s_player_identifier* current_player_id)
{
	//INVOKE_CLASS_MEMBER(0x00AB5890, c_gui_screen_boot_betrayer, set_current_player_id, current_player_id);

	m_current_player_id = *current_player_id;
}

void c_gui_screen_boot_betrayer::set_target_player_id(const s_player_identifier* target_player_id)
{
	//INVOKE_CLASS_MEMBER(0x00AB58B0, c_gui_screen_boot_betrayer, set_target_player_id, target_player_id);

	m_target_player_id = *target_player_id;
}

