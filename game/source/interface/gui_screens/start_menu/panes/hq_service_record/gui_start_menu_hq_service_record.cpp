#include "interface/gui_screens/start_menu/panes/hq_service_record/gui_start_menu_hq_service_record.hpp"

#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"

//.text:00AECB80 ; public: virtual void* c_gui_start_menu_hq_service_record_pane::`scalar deleting destructor'(unsigned int)

bool c_start_menu_pane_screen_widget::allow_pane_tab_change()
{
	//return INVOKE_CLASS_MEMBER(0x00AECBB0, c_start_menu_pane_screen_widget, allow_pane_tab_change);

	return false;
}

