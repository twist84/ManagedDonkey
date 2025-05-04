#include "interface/user_interface_window_manager.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/c_gui_widget.hpp"
#include "interface/gui_screens/boot_betrayer/gui_screen_boot_betrayer.hpp"
#include "interface/gui_screens/campaign/gui_screen_campaign_select_difficulty.hpp"
#include "interface/gui_screens/campaign/gui_screen_campaign_select_level.hpp"
#include "interface/gui_screens/error_dialogs/screen_error_dialog_ok.hpp"
#include "interface/gui_screens/game_details/gui_game_details.hpp"
#include "interface/gui_screens/main_menu/gui_screen_main_menu.hpp"
#include "interface/gui_screens/player_select/gui_player_select_screen_widget.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_campaign.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_matchmaking.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_multiplayer.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_selection.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/gui_screens/start_menu/gui_screen_start_menu.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x05260F34, c_window_manager, g_window_manager);
//HOOK_DECLARE_CLASS_MEMBER(0x00AA8E00, c_window_manager, allocate_named_screen);
//HOOK_DECLARE_CLASS(0x00AABFD0, c_window_manager, named_screen_defined_in_code);

//.text:00AA8C50 ; public: c_window_manager::c_window_manager()
//.text:00AA8D20 ; public: c_window_manager::~c_window_manager()

c_gui_screen_widget* c_window_manager::allocate_codeless_screen(int32 screen_name)
{
	return INVOKE_CLASS_MEMBER(0x00AA8D90, c_window_manager, allocate_codeless_screen, screen_name);

	//if (c_window_manager::locate_screen_definition_tag_by_name(screen_name) == NONE)
	//{
	//	event(_event_warning, "ui: failed to allocate screen '%s': no tag definition found", string_id_get_string_const(screen_name));
	//	return NULL;
	//}
	//
	//return new c_gui_screen_codeless_widget(screen_name);
}

c_gui_screen_widget* c_window_manager::allocate_named_screen(int32 screen_name)
{
	return INVOKE_CLASS_MEMBER(0x00AA8E00, c_window_manager, allocate_named_screen, screen_name);

	//c_gui_screen_widget* result = NULL;
	//HOOK_INVOKE_CLASS_MEMBER(result =, c_window_manager, allocate_named_screen, screen_name);
	//return result;
	
	if (c_window_manager::locate_screen_definition_tag_by_name(screen_name) == NONE && screen_name != _string_id_gui__message)
	{
		event(_event_warning, "ui: failed to allocate screen '%s': no tag definition found", string_id_get_string_const(screen_name));
		return NULL;
	}
	
	switch (screen_name) // H3 OG
	{
	case STRING_ID(gui, error_ok):
		return new c_error_dialog_ok_screen_widget(screen_name);
	case STRING_ID(gui, main_menu):
		return new c_main_menu_screen_widget(screen_name);
	case STRING_ID(gui, start_menu):
		return new c_start_menu_screen_widget(screen_name);
	//case STRING_ID(gui, start_menu_game_campaign):
	//	return new c_start_menu_game_campaign(screen_name);
	//case STRING_ID(gui, start_menu_game_multiplayer):
	//	return new c_start_menu_game_multiplayer(screen_name);
	//case STRING_ID(gui, start_menu_game_editor):
	//	return new c_start_menu_game_editor(screen_name);
	//case STRING_ID(gui, start_menu_game_saved_film):
	//	return new c_start_menu_game_saved_film(screen_name);
	//case STRING_ID(gui, start_menu_hq):
	//	return new c_start_menu_headquarters(screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record):
	//	return new c_gui_start_menu_hq_service_record(screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record_file_share):
	//	return new c_gui_start_menu_hq_service_record_file_share(screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_bungie):
	//	return new c_gui_start_menu_hq_service_record_file_share(screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_choose_category):
	//	return new c_gui_start_menu_hq_service_record_file_share_choose_category(screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_choose_item):
	//	return new c_gui_start_menu_hq_service_record_file_share_choose_item(screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_item_selected):
	//	return new c_gui_start_menu_hq_service_record_file_share_item_selected(screen_name);
	//case STRING_ID(gui, start_menu_hq_screenshots):
	//	return new c_gui_start_menu_hq_screenshots(screen_name);
	//case STRING_ID(gui, start_menu_hq_screenshots_item_options):
	//	return new c_gui_start_menu_hq_screenshots_options(screen_name);
	//case STRING_ID(gui, start_menu_hq_transfers):
	//	return new c_gui_start_menu_hq_transfers(screen_name);
	//case STRING_ID(gui, start_menu_hq_transfers_item_selected):
	//	return new c_gui_start_menu_hq_transfers_item_selected(screen_name);
	//case STRING_ID(gui, start_menu_settings):
	//	return new c_start_menu_settings(screen_name);
	//case STRING_ID(gui, start_menu_settings_controls):
	//	return new c_start_menu_settings_controls(screen_name);
	//case STRING_ID(gui, start_menu_settings_controls_button):
	//	return new c_start_menu_settings_controls_button(screen_name);
	//case STRING_ID(gui, start_menu_settings_controls_thumbstick):
	//	return new c_start_menu_settings_controls_thumbstick(screen_name);
	//case STRING_ID(gui, start_menu_settings_appearance):
	//	return new c_start_menu_settings_appearance(screen_name);
	//case STRING_ID(gui, start_menu_settings_appearance_model):
	//	return new c_start_menu_settings_appearance_model(screen_name);
	//case STRING_ID(gui, start_menu_settings_appearance_emblem):
	//	return new c_start_menu_settings_appearance_emblem(screen_name);
	//case STRING_ID(gui, start_menu_settings_appearance_colors):
	//	return new c_start_menu_settings_appearance_colors(screen_name);
	//case STRING_ID(gui, start_menu_settings_camera):
	//	return new c_start_menu_settings_camera(screen_name);
	//case STRING_ID(gui, start_menu_settings_display):
	//	return new c_start_menu_settings_display(screen_name);
	//case STRING_ID(gui, start_menu_settings_film_autosave):
	//	return new c_start_menu_settings_film_autosave(screen_name);
	//case STRING_ID(gui, screenshots_viewer):
	//	return new c_screenshots_viewer_screen_widget(screen_name);
	//case STRING_ID(gui, screenshots_file_share_previewer):
	//	return new c_screenshots_file_share_previewer_screen_widget(screen_name);
	//case STRING_ID(gui, gui_alert_nonblocking):
	//case STRING_ID(gui, gui_alert_toast):
	//case STRING_ID(gui, gui_alert_ingame_full):
	//case STRING_ID(gui, gui_alert_ingame_split):
	//	return new c_gui_alert_screen_widget(screen_name);
	//case STRING_ID(gui, gui_dialog_two_items):
	//case STRING_ID(gui, gui_dialog_four_items):
	//	return new c_gui_dialog_screen_widget(screen_name);
	//case STRING_ID(gui, in_progress):
	//case STRING_ID(gui, in_progress_mini):
	//case STRING_ID(gui, in_progress_mini_me):
	//	return new c_gui_screen_in_progress(screen_name);
	//case STRING_ID(gui, spartan_milestone_dialog):
	//	return new c_gui_spartan_milestone(screen_name);
	//case STRING_ID(gui, spartan_rank_dialog):
	//	return new c_gui_spartan_rank(screen_name);
	//case STRING_ID(gui, campaign_settings):
	//	return new c_gui_screen_campaign_settings(screen_name);
	case STRING_ID(gui, campaign_select_difficulty):
		return new c_gui_screen_campaign_select_difficulty(screen_name);
	case STRING_ID(gui, campaign_select_level):
		return new c_gui_screen_campaign_select_level(screen_name);
	case STRING_ID(gui, pregame_lobby_campaign):
		return new c_gui_screen_pregame_lobby_campaign(screen_name);
	case STRING_ID(gui, pregame_lobby_matchmaking):
		return new c_gui_screen_pregame_lobby_matchmaking(screen_name);
	case STRING_ID(gui, pregame_lobby_multiplayer):
		return new c_gui_screen_pregame_lobby_multiplayer(screen_name);
	//case STRING_ID(gui, pregame_lobby_mapeditor):
	//	return new c_gui_screen_pregame_lobby_mapeditor(screen_name);
	//case STRING_ID(gui, pregame_lobby_theater):
	//	return new c_gui_screen_pregame_lobby_theater(screen_name);
	//case STRING_ID(gui, pregame_switch_lobby):
	//	return new c_gui_screen_pregame_switch_lobby(screen_name);
	case STRING_ID(gui, pregame_selection):
		return new c_gui_screen_pregame_selection(screen_name);
	//case STRING_ID(gui, pregame_advanced_options_matchmaking):
	//	return new c_gui_screen_matchmaking_advanced_options(screen_name);
	//case STRING_ID(gui, postgame_lobby):
	//	return new c_gui_screen_postgame_lobby(screen_name);
	case STRING_ID(gui, player_select):
		return new c_gui_player_select_screen_widget(screen_name);
	//case STRING_ID(gui, game_browser):
	//	return new c_gui_game_browser(screen_name);
	case STRING_ID(gui, boot_betrayer):
	case STRING_ID(gui, boot_betrayer_splitscreen):
		return new c_gui_screen_boot_betrayer(screen_name);
	//case STRING_ID(gui, maximum_party_size):
	//	return new c_gui_screen_maximum_party_size(screen_name);
	case STRING_ID(gui, game_details):
		return new c_gui_game_details(screen_name);
	//case STRING_ID(gui, game_options):
	//	return new c_gui_screen_game_options(screen_name);
	//case STRING_ID(gui, alpha_legal):
	//	return new c_gui_screen_alpha_legal(screen_name);
	//case STRING_ID(gui, alpha_motd):
	//	return new c_gui_screen_alpha_motd(screen_name);
	//case STRING_ID(gui, alpha_locked_down):
	//	return new c_gui_screen_alpha_locked_down(screen_name);
	//case STRING_ID(gui, forge_legal):
	//	return new c_gui_screen_forge_legal(screen_name);
	//case STRING_ID(gui, carnage_report):
	//	return new c_gui_screen_carnage_report(screen_name);
	case STRING_ID(gui, carnage_report_player_details):
		return new c_gui_player_select_screen_widget(screen_name);
	case STRING_ID(gui, scoreboard):
	case STRING_ID(gui, scoreboard_half):
		return new c_gui_screen_scoreboard(screen_name);
	case STRING_ID(gui, scoreboard_player_select):
	case STRING_ID(gui, scoreboard_player_select_splitscreen):
		return new c_gui_player_select_screen_widget(screen_name);
	//case STRING_ID(gui, matchmaking_searching):
	//	return new c_gui_screen_matchmaking_searching(screen_name);
	//case STRING_ID(gui, matchmaking_match_found):
	//	return new c_gui_screen_matchmaking_match_found(screen_name);
	//case STRING_ID(gui, sandbox_budget_screen):
	//case STRING_ID(gui, sandbox_budget_screen_splitscreen):
	//	return new c_sandbox_budget_summary_screen_widget(screen_name);
	//case STRING_ID(gui, sandbox_objects_menu):
	//case STRING_ID(gui, sandbox_objects_menu_splitscreen):
	//	return new c_sandbox_object_menu_screen_widget(screen_name);
	//case STRING_ID(gui, sandbox_object_properties_panel):
	//case STRING_ID(gui, sandbox_object_properties_panel_splitscreen):
	//	return new c_sandbox_object_properties_screen_widget(screen_name);
	//case STRING_ID(gui, observer_camera_list_screen):
	//case STRING_ID(gui, observer_camera_list_splitscreen):
	//	return new c_observer_camera_list_screen(screen_name);
	//case STRING_ID(gui, saved_film_control_pad):
	//	return new c_saved_film_control_pad(screen_name);
	//case STRING_ID(gui, saved_film_take_screenshot):
	//	return new c_saved_film_take_screenshot(screen_name);
	//case STRING_ID(gui, saved_film_snippet):
	//	return new c_saved_film_snippet_screen(screen_name);
	//case STRING_ID(gui, terminal_screen):
	//	return new c_gui_screen_terminal(screen_name);
	//case STRING_ID(gui, message):
	//	return new c_motd_popup_screen_widget(screen_name);
	}
	
	//switch (screen_name) // ODST
	//{
	//case STRING_ID(gui, campaign_save_in_progress):
	//	return new c_gui_screen_in_progress(screen_name);
	//case STRING_ID(gui, campaign_select_scoring):
	//case STRING_ID(gui, survival_select_scoring):
	//	return new c_gui_screen_campaign_select_scoring(screen_name);
	//case STRING_ID(gui, campaign_select_skulls):
	//case STRING_ID(gui, survival_select_skulls):
	//	return new c_gui_screen_campaign_select_skulls(screen_name);
	//case STRING_ID(gui, main_menu_select_difficulty):
	//case STRING_ID(gui, survival_select_difficulty):
	//	return new c_gui_screen_campaign_select_difficulty(screen_name);
	//case STRING_ID(gui, survival_select_level):
	//	return new c_gui_screen_campaign_select_level(screen_name);
	//case STRING_ID(gui, pregame_lobby_survival):
	//	return new c_gui_screen_pregame_lobby_survival(screen_name);
	//case STRING_ID(gui, pregame_players_selection):
	//	return new c_gui_screen_pregame_selection(screen_name);
	//case STRING_ID(gui, carnage_report_loading):
	//	return new c_gui_carnage_report_loading_screen(screen_name);
	//case STRING_ID(gui, campaign_carnage_report):
	//case STRING_ID(gui, survival_carnage_report):
	//	return new c_gui_screen_carnage_report(screen_name);
	//}
	
	//switch (screen_name) // H3 MCC
	//{
	//case STRING_ID(gui, start_menu_settings_keyboard):
	//	return new c_start_menu_settings_keyboard(screen_name);
	//case STRING_ID(gui, start_menu_settings_controls_keyboard):
	//	return new c_start_menu_settings_controls_keyboard(screen_name);
	//case STRING_ID(gui, start_menu_settings_voice):
	//	return new c_start_menu_settings_voice(screen_name);
	//case STRING_ID(gui, start_menu_settings_video):
	//	return new c_start_menu_settings_video(screen_name);
	//case STRING_ID(gui, start_menu_confirm_resolution):
	//	return new c_start_menu_confirm_resolution(screen_name);
	//}
	
	//// $TODO: remove this when each of the above classes exist and have a constructor 
	//c_gui_screen_widget* result = NULL;
	//HOOK_INVOKE_CLASS_MEMBER(result =, c_window_manager, allocate_named_screen, screen_name);
	//if (result != NULL)
	//	return result;
	
	event(_event_warning, "ui: no c_gui_screen_class registered for ui screen '%s'", string_id_get_string_const(screen_name));
	return NULL;
}

//.text:00AAA7A0 ; public: bool c_window_manager::any_ui_active()
//.text:00AAA7D0 ; public: bool c_window_manager::any_ui_active_in_window(e_window_index)
//.text:00AAA7F0 ; public: void c_window_manager::begin_transition_out(c_gui_screen_widget*, e_screen_transition_type)
//.text:00AAA870 ; 
//.text:00AAA970 ; public: bool c_window_manager::controller_input_should_be_suppressed(e_controller_index)
//.text:00AAA9F0 ; public: void c_window_manager::debug_widget_state(e_gui_widget_type, int32, c_window_manager::e_widget_debug_state, bool, bool)
//.text:00AAAA70 ; public: void c_window_manager::debug_widget_state_for_screen(c_gui_screen_widget*, e_gui_widget_type, int32, c_window_manager::e_widget_debug_state, bool, bool)
//.text:00AAAAA0 ; public: void c_window_manager::debug_widget_state_for_widget(c_gui_widget*, c_window_manager::e_widget_debug_state, bool, bool)
//.text:00AAABB0 ; public: void c_window_manager::dispose()
//.text:00AAABE0 ; public: void c_window_manager::dispose_from_old_map()
//.text:00AAAE80 ; public: void c_window_manager::dispose_screen(c_gui_screen_widget*)
//.text:00AAB090 ; 
//.text:00AAB0A0 ; public: c_gui_screen_widget* c_window_manager::get_bottommost_screen(e_window_index)

c_gui_widget* c_gui_widget::get_children()
{
	return INVOKE_CLASS_MEMBER(0x00AAB0D0, c_gui_widget, get_children);
}

//.text:00AAB100 ; 
//.text:00AAB110 ; 
//.text:00AAB120 ; public: bool c_window_manager::get_fading() const
//.text:00AAB130 ; public: real32 c_window_manager::get_fading_amount() const
//.text:00AAB140 ; public: bool c_window_manager::get_fading_max() const
//.text:00AAB160 ; public: bool c_window_manager::get_fading_min() const
//.text:00AAB180 ; 
//.text:00AAB190 ; 
//.text:00AAB1A0 ; 
//.text:00AAB1B0 ; 
//.text:00AAB1C0 ; 
//.text:00AAB1D0 ; public: virtual e_gui_location c_gui_screen_widget::get_gui_location() const
//.text:00AAB1E0 ; 

c_gui_widget* c_gui_widget::get_next()
{
	return INVOKE_CLASS_MEMBER(0x00AAB1F0, c_gui_widget, get_next);
}

c_gui_widget* c_gui_widget::get_parent()
{
	return INVOKE_CLASS_MEMBER(0x00AAB220, c_gui_widget, get_parent);
}

//.text:00AAB250 ; 
//.text:00AAB260 ; protected: virtual e_render_data_size c_gui_screen_widget::get_render_data_size()
//.text:00AAB270 ; public: s_window_manager_screen_render_data* c_gui_screen_widget::get_render_state()
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
//.text:00AAB550 ; public: c_gui_screen_widget* c_window_manager::get_screen_by_name(e_window_index, int32)
//.text:00AAB5B0 ; public: c_gui_screen_widget* c_window_manager::get_screen_by_screen_index(int32)
//.text:00AAB620 ; 

c_gui_screen_widget* c_window_manager::get_topmost_screen(e_window_index window_index)
{
	return INVOKE_CLASS_MEMBER(0x00AAB630, c_window_manager, get_topmost_screen, window_index);
}

//.text:00AAB680 ; 
//.text:00AAB690 ; public: c_gui_widget* c_robust_pointer<c_gui_widget>::get_value()
//.text:00AAB6B0 ; public: void* c_robust_void_pointer::get_value()
//.text:00AAB6D0 ; 
//.text:00AAB750 ; int __cdecl gui_widget_depth_sort_proc(void const*, void const*)

void c_window_manager::handle_global_controller_event(s_event_record* event_record)
{
	INVOKE_CLASS_MEMBER(0x00AAB7D0, c_window_manager, handle_global_controller_event, event_record);
}

//.text:00AAB9C0 ; public: void c_window_manager::initialize()
//.text:00AABA00 ; public: void c_window_manager::initialize_for_new_map()
//.text:00AABA40 ; private: void c_window_manager::insert_screen_after_named_screen(e_window_index, c_gui_screen_widget*, int32)
//.text:00AABBB0 ; private: bool __cdecl c_robust_void_pointer::internally_consistent() const
//.text:00AABBD0 ; public: bool c_window_manager::is_screen_queued_to_be_loaded(int32, e_window_index)

c_gui_screen_widget* c_window_manager::load_screen(e_controller_index controller_index, bool load_as_error, c_load_screen_message const* screen_message, int32 window_index)
{
	return INVOKE_CLASS_MEMBER(0x00AABC30, c_window_manager, load_screen, controller_index, load_as_error, screen_message, window_index);
	//return DECLFUNC(0x00AABC30, c_gui_screen_widget*, __thiscall, c_window_manager*, int32, bool, c_load_screen_message const*, int32)(&g_window_manager, controller_index, load_as_error, screen_message, window_index);
}

int32 __cdecl c_window_manager::locate_screen_definition_tag_by_name(int32 name)
{
	return INVOKE(0x00AABF50, c_window_manager::locate_screen_definition_tag_by_name, name);
}

bool __cdecl c_window_manager::named_screen_defined_in_code(int32 screen_name)
{
	return INVOKE(0x00AABFD0, c_window_manager::named_screen_defined_in_code, screen_name);

	//switch (screen_name) // H3 OG
	//{
	//case STRING_ID(gui, error_ok):
	//case STRING_ID(gui, main_menu):
	//case STRING_ID(gui, start_menu):
	//case STRING_ID(gui, start_menu_game_campaign):
	//case STRING_ID(gui, start_menu_game_multiplayer):
	//case STRING_ID(gui, start_menu_game_editor):
	//case STRING_ID(gui, start_menu_game_saved_film):
	//case STRING_ID(gui, start_menu_hq):
	//case STRING_ID(gui, start_menu_hq_service_record):
	//case STRING_ID(gui, start_menu_hq_service_record_file_share):
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_bungie):
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_choose_category):
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_choose_item):
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_item_selected):
	//case STRING_ID(gui, start_menu_hq_screenshots):
	//case STRING_ID(gui, start_menu_hq_screenshots_item_options):
	//case STRING_ID(gui, start_menu_hq_transfers):
	//case STRING_ID(gui, start_menu_hq_transfers_item_selected):
	//case STRING_ID(gui, start_menu_settings):
	//case STRING_ID(gui, start_menu_settings_controls):
	//case STRING_ID(gui, start_menu_settings_controls_button):
	//case STRING_ID(gui, start_menu_settings_controls_thumbstick):
	//case STRING_ID(gui, start_menu_settings_appearance):
	//case STRING_ID(gui, start_menu_settings_appearance_model):
	//case STRING_ID(gui, start_menu_settings_appearance_emblem):
	//case STRING_ID(gui, start_menu_settings_appearance_colors):
	//case STRING_ID(gui, start_menu_settings_camera):
	//case STRING_ID(gui, start_menu_settings_display):
	//case STRING_ID(gui, start_menu_settings_film_autosave):
	//case STRING_ID(gui, screenshots_viewer):
	//case STRING_ID(gui, screenshots_file_share_previewer):
	//case STRING_ID(gui, gui_alert_nonblocking):
	//case STRING_ID(gui, gui_alert_toast):
	//case STRING_ID(gui, gui_alert_ingame_full):
	//case STRING_ID(gui, gui_alert_ingame_split):
	//case STRING_ID(gui, gui_dialog_two_items):
	//case STRING_ID(gui, gui_dialog_four_items):
	//case STRING_ID(gui, in_progress):
	//case STRING_ID(gui, in_progress_mini):
	//case STRING_ID(gui, in_progress_mini_me):
	//case STRING_ID(gui, spartan_milestone_dialog):
	//case STRING_ID(gui, spartan_rank_dialog):
	//case STRING_ID(gui, campaign_settings):
	//case STRING_ID(gui, campaign_select_difficulty):
	//case STRING_ID(gui, campaign_select_level):
	//case STRING_ID(gui, pregame_lobby_campaign):
	//case STRING_ID(gui, pregame_lobby_matchmaking):
	//case STRING_ID(gui, pregame_lobby_multiplayer):
	//case STRING_ID(gui, pregame_lobby_mapeditor):
	//case STRING_ID(gui, pregame_lobby_theater):
	//case STRING_ID(gui, pregame_switch_lobby):
	//case STRING_ID(gui, pregame_selection):
	//case STRING_ID(gui, pregame_advanced_options_matchmaking):
	//case STRING_ID(gui, postgame_lobby):
	//case STRING_ID(gui, player_select):
	//case STRING_ID(gui, game_browser):
	//case STRING_ID(gui, boot_betrayer):
	//case STRING_ID(gui, boot_betrayer_splitscreen):
	//case STRING_ID(gui, maximum_party_size):
	//case STRING_ID(gui, game_details):
	//case STRING_ID(gui, game_options):
	//case STRING_ID(gui, alpha_legal):
	//case STRING_ID(gui, alpha_motd):
	//case STRING_ID(gui, alpha_locked_down):
	//case STRING_ID(gui, forge_legal):
	//case STRING_ID(gui, carnage_report):
	//case STRING_ID(gui, carnage_report_player_details):
	//case STRING_ID(gui, scoreboard):
	//case STRING_ID(gui, scoreboard_half):
	//case STRING_ID(gui, scoreboard_player_select):
	//case STRING_ID(gui, scoreboard_player_select_splitscreen):
	//case STRING_ID(gui, matchmaking_searching):
	//case STRING_ID(gui, matchmaking_match_found):
	//case STRING_ID(gui, sandbox_budget_screen):
	//case STRING_ID(gui, sandbox_budget_screen_splitscreen):
	//case STRING_ID(gui, sandbox_objects_menu):
	//case STRING_ID(gui, sandbox_objects_menu_splitscreen):
	//case STRING_ID(gui, sandbox_object_properties_panel):
	//case STRING_ID(gui, sandbox_object_properties_panel_splitscreen):
	//case STRING_ID(gui, observer_camera_list_screen):
	//case STRING_ID(gui, observer_camera_list_splitscreen):
	//case STRING_ID(gui, saved_film_control_pad):
	//case STRING_ID(gui, saved_film_take_screenshot):
	//case STRING_ID(gui, saved_film_snippet):
	//case STRING_ID(gui, terminal_screen):
	//case STRING_ID(gui, message):
	//	return true;
	//}
	//
	//switch (screen_name) // ODST
	//{
	//case STRING_ID(gui, campaign_save_in_progress):
	//case STRING_ID(gui, campaign_select_scoring):
	//case STRING_ID(gui, survival_select_scoring):
	//case STRING_ID(gui, campaign_select_skulls):
	//case STRING_ID(gui, survival_select_skulls):
	//case STRING_ID(gui, main_menu_select_difficulty):
	//case STRING_ID(gui, survival_select_difficulty):
	//case STRING_ID(gui, survival_select_level):
	//case STRING_ID(gui, pregame_lobby_survival):
	//case STRING_ID(gui, pregame_players_selection):
	//case STRING_ID(gui, carnage_report_loading):
	//case STRING_ID(gui, campaign_carnage_report):
	//case STRING_ID(gui, survival_carnage_report):
	//	return true;
	//}
	//
	////switch (screen_name) // H3 MCC
	////{
	////case STRING_ID(gui, start_menu_settings_keyboard):
	////case STRING_ID(gui, start_menu_settings_controls_keyboard):
	////case STRING_ID(gui, start_menu_settings_voice):
	////case STRING_ID(gui, start_menu_settings_video):
	////case STRING_ID(gui, start_menu_confirm_resolution):
	////	return true;
	////}
	//
	//return false;
}

//.text:00AAC430 ; public: bool c_window_manager::non_dead_cam_ui_active_for_game_player_controller(e_controller_index)
//.text:00AAC510 ; public: static void __cdecl c_window_manager::play_controller_sound(int32, e_event_type, e_controller_component)
//.text:00AAC650 ; c_window_manager::predict_immediate_resources
//.text:00AAC6C0 ; c_window_manager::predict_pending_resources
//.text:00AAC730 ; public: void c_window_manager::prepare_for_tag_reload()
//.text:00AAC740 ; public: static void __cdecl c_window_manager::print_active_screen_strings_tag_name()
//.text:00AAC750 ; public: void c_window_manager::print_active_screens()
//.text:00AAC760 ; private: void c_window_manager::process_unhandled_events(uns32)
//.text:00AAC910 ; public: void c_window_manager::render(e_window_index, int32, rectangle2d const*, bool)
//.text:00AACB10 ; public: void c_window_manager::render_fade()
//.text:00AACBD0 ; public: void c_window_manager::reset_screens()
//.text:00AACE40 ; public: void c_window_manager::run_screen_hs_script(int32)
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

void c_window_manager::update(uns32 milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00AAD050, c_window_manager, update, milliseconds);
}

//.text:00AAD540 ; public: void c_window_manager::update_fade(uns32)
//.text:00AAD670 ; 
//.text:00AAD730 ; void __cdecl window_manager_add_widgets_to_render_list_recursive(rectangle2d const*, c_gui_widget*, s_window_manager_screen_render_data*, e_controller_index, int32)
//.text:00AAD8B0 ; void __cdecl window_manager_build_render_data_for_screen(rectangle2d const*, c_gui_screen_widget*, s_window_manager_screen_render_data*)

c_window_manager* __cdecl window_manager_get()
{
	//return INVOKE(0x00AAD930, window_manager_get);

	return &g_window_manager;
}

//.text:00AAD940 ; bool __cdecl window_manager_handle_dialog_result_message(c_dialog_result_message const*)

void __cdecl window_manager_load_screen_hs(int32 screen_name)
{
	INVOKE(0x00AAD9A0, window_manager_load_screen_hs, screen_name);
}

//.text:00AADA20 ; void __cdecl window_manager_render_screen_internal(s_window_manager_static_render_data*, int32, rectangle2d*, bool)

void __cdecl window_manager_reset_screens()
{
	INVOKE(0x00AADAC0, window_manager_reset_screens);
}

//.text:00AADAE0 ; bool __cdecl window_manager_screen_is_alert_or_dialog(c_gui_screen_widget const*)
//.text:00AADB10 ; void __cdecl window_manager_storage_devices_changed()


