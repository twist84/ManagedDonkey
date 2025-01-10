#include "interface/user_interface_window_manager.hpp"

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

REFERENCE_DECLARE(0x05260F34, c_window_manager, g_window_manager);

//.text:00AA8C50 ; public: c_window_manager::c_window_manager()
//.text:00AA8D20 ; public: c_window_manager::~c_window_manager()

c_gui_screen_widget* c_window_manager::allocate_codeless_screen(long screen_name)
{
	return INVOKE_CLASS_MEMBER(0x00AA8D90, c_window_manager, allocate_codeless_screen, screen_name);
}

c_gui_screen_widget* c_window_manager::allocate_named_screen(long screen_name)
{
	return INVOKE_CLASS_MEMBER(0x00AA8E00, c_window_manager, allocate_named_screen, screen_name);
}

//.text:00AAA7A0 ; public: bool c_window_manager::any_ui_active()
//.text:00AAA7D0 ; public: bool c_window_manager::any_ui_active_in_window(e_window_index)
//.text:00AAA7F0 ; public: void c_window_manager::begin_transition_out(c_gui_screen_widget*, e_screen_transition_type)
//.text:00AAA870 ; 
//.text:00AAA970 ; public: bool c_window_manager::controller_input_should_be_suppressed(e_controller_index)
//.text:00AAA9F0 ; public: void c_window_manager::debug_widget_state(e_gui_widget_type, long, c_window_manager::e_widget_debug_state, bool, bool)
//.text:00AAAA70 ; public: void c_window_manager::debug_widget_state_for_screen(c_gui_screen_widget*, e_gui_widget_type, long, c_window_manager::e_widget_debug_state, bool, bool)
//.text:00AAAAA0 ; public: void c_window_manager::debug_widget_state_for_widget(c_gui_widget*, c_window_manager::e_widget_debug_state, bool, bool)
//.text:00AAABB0 ; public: void c_window_manager::dispose()
//.text:00AAABE0 ; public: void c_window_manager::dispose_from_old_map()
//.text:00AAAE80 ; public: void c_window_manager::dispose_screen(c_gui_screen_widget*)
//.text:00AAB090 ; 
//.text:00AAB0A0 ; public: c_gui_screen_widget* c_window_manager::get_bottommost_screen(e_window_index)
//.text:00AAB0D0 ; 
//.text:00AAB100 ; 
//.text:00AAB110 ; 
//.text:00AAB120 ; public: bool c_window_manager::get_fading() const
//.text:00AAB130 ; public: real c_window_manager::get_fading_amount() const
//.text:00AAB140 ; public: bool c_window_manager::get_fading_max() const
//.text:00AAB160 ; public: bool c_window_manager::get_fading_min() const
//.text:00AAB180 ; 
//.text:00AAB190 ; 
//.text:00AAB1A0 ; 
//.text:00AAB1B0 ; 
//.text:00AAB1C0 ; 
//.text:00AAB1D0 ; public: virtual e_gui_location __cdecl c_gui_screen_widget::get_gui_location() const
//.text:00AAB1E0 ; 
//.text:00AAB1F0 ; 
//.text:00AAB220 ; 
//.text:00AAB250 ; 
//.text:00AAB260 ; protected: virtual e_render_data_size __cdecl c_gui_screen_widget::get_render_data_size()
//.text:00AAB270 ; public: s_window_manager_screen_render_data* __cdecl c_gui_screen_widget::get_render_state()
//.text:00AAB290 ; public: e_window_index c_window_manager::get_render_window_for_screen(c_gui_screen_widget*)
//.text:00AAB300 ; 
//.text:00AAB310 ; 
//.text:00AAB320 ; public: c_gui_screen_widget* c_window_manager::get_responding_screen(e_controller_index, e_window_index*)
//.text:00AAB3F0 ; public: c_gui_screen_widget* c_window_manager::get_responding_screen_in_window(e_controller_index, e_window_index)
//.text:00AAB480 ; 

c_gui_screen_widget* c_window_manager::get_screen_above(e_window_index window_index, c_gui_screen_widget* screen)
{
	return INVOKE_CLASS_MEMBER(0x00AAB490, c_window_manager, get_screen_above, window_index, screen);
}

//.text:00AAB4E0 ; public: c_gui_screen_widget* c_window_manager::get_screen_below(e_window_index, c_gui_screen_widget*)
//.text:00AAB550 ; public: c_gui_screen_widget* c_window_manager::get_screen_by_name(e_window_index, long)
//.text:00AAB5B0 ; public: c_gui_screen_widget* c_window_manager::get_screen_by_screen_index(long)
//.text:00AAB620 ; 

c_gui_screen_widget* c_window_manager::get_topmost_screen(e_window_index window_index)
{
	return INVOKE_CLASS_MEMBER(0x00AAB630, c_window_manager, get_topmost_screen, window_index);
}

//.text:00AAB680 ; 
//.text:00AAB690 ; 
//.text:00AAB6B0 ; 
//.text:00AAB6D0 ; 
//.text:00AAB750 ; 

void c_window_manager::handle_global_controller_event(s_event_record* event_record)
{
	INVOKE_CLASS_MEMBER(0x00AAB7D0, c_window_manager, handle_global_controller_event, event_record);
}

//.text:00AAB9C0 ; public: void c_window_manager::initialize()
//.text:00AABA00 ; public: void c_window_manager::initialize_for_new_map()
//.text:00AABA40 ; private: void c_window_manager::insert_screen_after_named_screen(e_window_index, c_gui_screen_widget*, long)
//.text:00AABBB0 ; 
//.text:00AABBD0 ; public: bool c_window_manager::is_screen_queued_to_be_loaded(long, e_window_index)

c_gui_screen_widget* c_window_manager::load_screen(long controller_index, bool unused, c_load_screen_message const* screen_message, long window_index)
{
	return DECLFUNC(0x00AABC30, c_gui_screen_widget*, __thiscall, c_window_manager*, long, bool, c_load_screen_message const*, long)(&g_window_manager, controller_index, unused, screen_message, window_index);
}

//.text:00AABF50 ; public: long c_window_manager::locate_screen_definition_tag_by_name(long)

bool c_window_manager::named_screen_defined_in_code(long screen_name)
{
	return INVOKE_CLASS_MEMBER(0x00AABFD0, c_window_manager, named_screen_defined_in_code, screen_name);
}

//.text:00AAC430 ; 
//.text:00AAC510 ; 
//.text:00AAC650 ; 
//.text:00AAC6C0 ; 
//.text:00AAC760 ; private: void c_window_manager::process_unhandled_events(dword)
//.text:00AAC910 ; public: void c_window_manager::render(e_window_index, long, rectangle2d const*, bool)
//.text:00AACB10 ; 
//.text:00AACBD0 ; public: void c_window_manager::reset_screens()
//.text:00AACE40 ; 
//.text:00AACE40 ; c_window_manager::run_screen_hs_script_by_index?
//.text:00AACEA0 ; 
//.text:00AACEC0 ; 
//.text:00AACEF0 ; 
//.text:00AACF10 ; 
//.text:00AACF40 ; public: void c_window_manager::set_fade_out_and_quit_campaign(bool,bool)
//.text:00AACF60 ; public: void c_window_manager::set_fading(bool)
//.text:00AACF70 ; public: void c_window_manager::set_fading_instant(bool)
//.text:00AACFB0 ; 
//.text:00AACFD0 ; 
//.text:00AAD000 ; 
//.text:00AAD010 ; 
//.text:00AAD020 ; 

void c_window_manager::update(dword milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00AAD050, c_window_manager, update, milliseconds);
}

//.text:00AAD540 ; public: void c_window_manager::update_fade(dword)
//.text:00AAD670 ; 
//.text:00AAD730 ; 
//.text:00AAD8B0 ; 

c_window_manager* __cdecl window_manager_get()
{
	//return INVOKE(0x00AAD930, window_manager_get);

	return &g_window_manager;
}

//.text:00AAD940 ; bool __cdecl window_manager_handle_dialog_result_message(c_dialog_result_message const*)

void __cdecl window_manager_load_screen_hs(long screen_name)
{
	INVOKE(0x00AAD9A0, window_manager_load_screen_hs, screen_name);
}

//.text:00AADA20 ; 

void __cdecl window_manager_reset_screens()
{
	INVOKE(0x00AADAC0, window_manager_reset_screens);
}

//.text:00AADAE0 ; 
//.text:00AADB10 ; 


