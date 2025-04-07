#include "interface/user_interface_window_manager.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_gui_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/gui_screens/error_dialogs/screen_error_dialog_ok.hpp"
#include "interface/gui_screens/main_menu/gui_screen_main_menu.hpp"
#include "interface/gui_screens/start_menu/gui_screen_start_menu.hpp"
#include "memory/module.hpp"
#include "shell/shell.hpp"

REFERENCE_DECLARE(0x05260F34, c_window_manager, g_window_manager);
HOOK_DECLARE_CLASS_MEMBER(0x00AA8E00, c_window_manager, allocate_named_screen);
HOOK_DECLARE_CLASS(0x00AABFD0, c_window_manager, named_screen_defined_in_code);

//.text:00AA8C50 ; public: c_window_manager::c_window_manager()
//.text:00AA8D20 ; public: c_window_manager::~c_window_manager()

c_gui_screen_widget* c_window_manager::allocate_codeless_screen(long screen_name)
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

c_gui_screen_widget* c_window_manager::allocate_named_screen(long screen_name)
{
	//return INVOKE_CLASS_MEMBER(0x00AA8E00, c_window_manager, allocate_named_screen, screen_name);

	if (c_window_manager::locate_screen_definition_tag_by_name(screen_name) == NONE && screen_name != _string_id_gui__message)
	{
		event(_event_warning, "ui: failed to allocate screen '%s': no tag definition found", string_id_get_string_const(screen_name));
		return NULL;
	}

	switch (screen_name)
	{
#define DECLARE_SCREEN_NAME_CASE(SCREEN_NAME, CLASS) case SCREEN_NAME: return new CLASS(SCREEN_NAME)
	DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, error_ok), c_error_dialog_ok_screen_widget);
	DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, main_menu), c_main_menu_screen_widget);
	DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu), c_start_menu_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_game_campaign), c_start_menu_game_campaign);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_game_multiplayer), c_start_menu_game_multiplayer);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_game_editor), c_start_menu_game_editor);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_game_saved_film), c_start_menu_game_saved_film);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_hq), c_start_menu_headquarters);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_hq_service_record), c_gui_start_menu_hq_service_record);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_hq_service_record_file_share), c_gui_start_menu_hq_service_record_file_share);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_hq_service_record_file_share_bungie), c_gui_start_menu_hq_service_record_file_share);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_hq_service_record_file_share_choose_category), c_gui_start_menu_hq_service_record_file_share_choose_category);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_hq_service_record_file_share_choose_item), c_gui_start_menu_hq_service_record_file_share_choose_item);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_hq_service_record_file_share_item_selected), c_gui_start_menu_hq_service_record_file_share_item_selected);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_hq_screenshots), c_gui_start_menu_hq_screenshots);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_hq_screenshots_item_options), c_gui_start_menu_hq_screenshots_options);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_hq_transfers), c_gui_start_menu_hq_transfers);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_hq_transfers_item_selected), c_gui_start_menu_hq_transfers_item_selected);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings), c_start_menu_settings);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_controls), c_start_menu_settings_controls);
	////DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_keyboard), c_start_menu_settings_keyboard);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_controls_button), c_start_menu_settings_controls_button);
	////DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_controls_keyboard), c_start_menu_settings_controls_keyboard);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_controls_thumbstick), c_start_menu_settings_controls_thumbstick);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_appearance), c_start_menu_settings_appearance);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_appearance_model), c_start_menu_settings_appearance_model);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_appearance_emblem), c_start_menu_settings_appearance_emblem);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_appearance_colors), c_start_menu_settings_appearance_colors);
	////DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_voice), c_start_menu_settings_voice);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_camera), c_start_menu_settings_camera);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_display), c_start_menu_settings_display);
	////DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_video), c_start_menu_settings_video);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_settings_film_autosave), c_start_menu_settings_film_autosave);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, screenshots_viewer), c_screenshots_viewer_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, screenshots_file_share_previewer), c_screenshots_file_share_previewer_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, gui_alert_nonblocking), c_gui_alert_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, gui_alert_toast), c_gui_alert_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, gui_alert_ingame_full), c_gui_alert_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, gui_alert_ingame_split), c_gui_alert_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, gui_dialog_two_items), c_gui_dialog_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, gui_dialog_four_items), c_gui_dialog_screen_widget);
	////DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, start_menu_confirm_resolution), c_start_menu_confirm_resolution);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, in_progress), c_gui_screen_in_progress);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, in_progress_mini), c_gui_screen_in_progress);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, in_progress_mini_me), c_gui_screen_in_progress);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, spartan_milestone_dialog), c_gui_spartan_milestone);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, spartan_rank_dialog), c_gui_spartan_rank);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, campaign_settings), c_gui_screen_campaign_settings);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, campaign_select_difficulty), c_gui_screen_campaign_select_difficulty);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, campaign_select_level), c_gui_screen_campaign_select_level);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, pregame_lobby_campaign), c_gui_screen_pregame_lobby_campaign);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, pregame_lobby_matchmaking), c_gui_screen_pregame_lobby_matchmaking);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, pregame_lobby_multiplayer), c_gui_screen_pregame_lobby_multiplayer);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, pregame_lobby_mapeditor), c_gui_screen_pregame_lobby_mapeditor);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, pregame_lobby_theater), c_gui_screen_pregame_lobby_theater);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, pregame_switch_lobby), c_gui_screen_pregame_switch_lobby);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, pregame_selection), c_gui_screen_pregame_selection);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, pregame_advanced_options_matchmaking), c_gui_screen_matchmaking_advanced_options);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, postgame_lobby), c_gui_screen_postgame_lobby);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, player_select), c_gui_player_select_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, game_browser), c_gui_game_browser);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, boot_betrayer), c_gui_screen_boot_betrayer);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, boot_betrayer_splitscreen), c_gui_screen_boot_betrayer);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, maximum_party_size), c_gui_screen_maximum_party_size);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, game_details), c_gui_game_details);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, game_options), c_gui_screen_game_options);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, alpha_legal), c_gui_screen_alpha_legal);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, alpha_motd), c_gui_screen_alpha_motd);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, alpha_locked_down), c_gui_screen_alpha_locked_down);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, forge_legal), c_gui_screen_forge_legal);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, carnage_report), c_gui_screen_carnage_report);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, carnage_report_player_details), c_gui_player_select_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, scoreboard), c_gui_screen_scoreboard);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, scoreboard_half), c_gui_screen_scoreboard);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, scoreboard_player_select), c_gui_player_select_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, scoreboard_player_select_splitscreen), c_gui_player_select_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, matchmaking_searching), c_gui_screen_matchmaking_searching);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, matchmaking_match_found), c_gui_screen_matchmaking_match_found);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, sandbox_budget_screen), c_sandbox_budget_summary_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, sandbox_budget_screen_splitscreen), c_sandbox_budget_summary_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, sandbox_objects_menu), c_sandbox_object_menu_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, sandbox_objects_menu_splitscreen), c_sandbox_object_menu_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, sandbox_object_properties_panel), c_sandbox_object_properties_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, sandbox_object_properties_panel_splitscreen), c_sandbox_object_properties_screen_widget);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, observer_camera_list_screen), c_observer_camera_list_screen);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, observer_camera_list_splitscreen), c_observer_camera_list_screen);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, saved_film_control_pad), c_saved_film_control_pad);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, saved_film_take_screenshot), c_saved_film_take_screenshot);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, saved_film_snippet), c_saved_film_snippet_screen);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, terminal_screen), c_gui_screen_terminal);
	//DECLARE_SCREEN_NAME_CASE(STRING_ID(gui, message), c_motd_popup_screen_widget);
#undef DECLARE_SCREEN_NAME_CASE
	}

	// $TODO: remove this when each of the above classes exist and have a constructor 
	c_gui_screen_widget* result = NULL;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_window_manager, allocate_named_screen, screen_name);
	if (result != NULL)
		return result;

	event(_event_warning, "ui: no c_gui_screen_class registered for ui screen '%s'", string_id_get_string_const(screen_name));
	return NULL;
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

c_gui_widget* c_gui_widget::get_children()
{
	return INVOKE_CLASS_MEMBER(0x00AAB0D0, c_gui_widget, get_children);
}

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

c_gui_screen_widget* c_window_manager::load_screen(e_controller_index controller_index, bool load_as_error, c_load_screen_message const* screen_message, long window_index)
{
	return INVOKE_CLASS_MEMBER(0x00AABC30, c_window_manager, load_screen, controller_index, load_as_error, screen_message, window_index);
	//return DECLFUNC(0x00AABC30, c_gui_screen_widget*, __thiscall, c_window_manager*, long, bool, c_load_screen_message const*, long)(&g_window_manager, controller_index, load_as_error, screen_message, window_index);
}

long __cdecl c_window_manager::locate_screen_definition_tag_by_name(long name)
{
	return INVOKE(0x00AABF50, c_window_manager::locate_screen_definition_tag_by_name, name);
}

bool __cdecl c_window_manager::named_screen_defined_in_code(long screen_name)
{
	//return INVOKE(0x00AABFD0, c_window_manager::named_screen_defined_in_code, screen_name);

	return screen_name == STRING_ID(gui, error_ok) ||
		screen_name == STRING_ID(gui, main_menu) ||
		screen_name == STRING_ID(gui, start_menu) ||
		screen_name == STRING_ID(gui, start_menu_game_campaign) ||
		screen_name == STRING_ID(gui, start_menu_game_multiplayer) ||
		screen_name == STRING_ID(gui, start_menu_game_editor) ||
		screen_name == STRING_ID(gui, start_menu_game_saved_film) ||
		screen_name == STRING_ID(gui, start_menu_hq) ||
		screen_name == STRING_ID(gui, start_menu_hq_service_record) ||
		screen_name == STRING_ID(gui, start_menu_hq_service_record_file_share) ||
		screen_name == STRING_ID(gui, start_menu_hq_service_record_file_share_bungie) ||
		screen_name == STRING_ID(gui, start_menu_hq_service_record_file_share_choose_category) ||
		screen_name == STRING_ID(gui, start_menu_hq_service_record_file_share_choose_item) ||
		screen_name == STRING_ID(gui, start_menu_hq_service_record_file_share_item_selected) ||
		screen_name == STRING_ID(gui, start_menu_hq_screenshots) ||
		screen_name == STRING_ID(gui, start_menu_hq_screenshots_item_options) ||
		screen_name == STRING_ID(gui, start_menu_hq_transfers) ||
		screen_name == STRING_ID(gui, start_menu_hq_transfers_item_selected) ||
		screen_name == STRING_ID(gui, start_menu_settings) ||
		screen_name == STRING_ID(gui, start_menu_settings_controls) ||
		//screen_name == STRING_ID(gui, start_menu_settings_keyboard) ||
		screen_name == STRING_ID(gui, start_menu_settings_controls_button) ||
		//screen_name == STRING_ID(gui, start_menu_settings_controls_keyboard) ||
		screen_name == STRING_ID(gui, start_menu_settings_controls_thumbstick) ||
		screen_name == STRING_ID(gui, start_menu_settings_appearance) ||
		screen_name == STRING_ID(gui, start_menu_settings_appearance_model) ||
		screen_name == STRING_ID(gui, start_menu_settings_appearance_emblem) ||
		screen_name == STRING_ID(gui, start_menu_settings_appearance_colors) ||
		//screen_name == STRING_ID(gui, start_menu_settings_voice) ||
		screen_name == STRING_ID(gui, start_menu_settings_camera) ||
		screen_name == STRING_ID(gui, start_menu_settings_display) ||
		//screen_name == STRING_ID(gui, start_menu_settings_video) ||
		screen_name == STRING_ID(gui, start_menu_settings_film_autosave) ||
		screen_name == STRING_ID(gui, screenshots_viewer) ||
		screen_name == STRING_ID(gui, screenshots_file_share_previewer) ||
		screen_name == STRING_ID(gui, gui_alert_nonblocking) ||
		screen_name == STRING_ID(gui, gui_alert_toast) ||
		screen_name == STRING_ID(gui, gui_alert_ingame_full) ||
		screen_name == STRING_ID(gui, gui_alert_ingame_split) ||
		screen_name == STRING_ID(gui, gui_dialog_two_items) ||
		screen_name == STRING_ID(gui, gui_dialog_four_items) ||
		//screen_name == STRING_ID(gui, start_menu_confirm_resolution) ||
		screen_name == STRING_ID(gui, in_progress) ||
		screen_name == STRING_ID(gui, in_progress_mini) ||
		screen_name == STRING_ID(gui, in_progress_mini_me) ||
		screen_name == STRING_ID(gui, spartan_milestone_dialog) ||
		screen_name == STRING_ID(gui, spartan_rank_dialog) ||
		screen_name == STRING_ID(gui, campaign_settings) ||
		screen_name == STRING_ID(gui, campaign_select_difficulty) ||
		screen_name == STRING_ID(gui, campaign_select_level) ||
		screen_name == STRING_ID(gui, pregame_lobby_campaign) ||
		screen_name == STRING_ID(gui, pregame_lobby_matchmaking) ||
		screen_name == STRING_ID(gui, pregame_lobby_multiplayer) ||
		screen_name == STRING_ID(gui, pregame_lobby_mapeditor) ||
		screen_name == STRING_ID(gui, pregame_lobby_theater) ||
		screen_name == STRING_ID(gui, pregame_switch_lobby) ||
		screen_name == STRING_ID(gui, pregame_selection) ||
		screen_name == STRING_ID(gui, pregame_advanced_options_matchmaking) ||
		screen_name == STRING_ID(gui, postgame_lobby) ||
		screen_name == STRING_ID(gui, player_select) ||
		screen_name == STRING_ID(gui, game_browser) ||
		screen_name == STRING_ID(gui, boot_betrayer) ||
		screen_name == STRING_ID(gui, boot_betrayer_splitscreen) ||
		screen_name == STRING_ID(gui, maximum_party_size) ||
		screen_name == STRING_ID(gui, game_details) ||
		screen_name == STRING_ID(gui, game_options) ||
		screen_name == STRING_ID(gui, alpha_legal) ||
		screen_name == STRING_ID(gui, alpha_motd) ||
		screen_name == STRING_ID(gui, alpha_locked_down) ||
		screen_name == STRING_ID(gui, forge_legal) ||
		screen_name == STRING_ID(gui, carnage_report) ||
		screen_name == STRING_ID(gui, carnage_report_player_details) ||
		screen_name == STRING_ID(gui, scoreboard) ||
		screen_name == STRING_ID(gui, scoreboard_half) ||
		screen_name == STRING_ID(gui, scoreboard_player_select) ||
		screen_name == STRING_ID(gui, scoreboard_player_select_splitscreen) ||
		screen_name == STRING_ID(gui, matchmaking_searching) ||
		screen_name == STRING_ID(gui, matchmaking_match_found) ||
		screen_name == STRING_ID(gui, sandbox_budget_screen) ||
		screen_name == STRING_ID(gui, sandbox_budget_screen_splitscreen) ||
		screen_name == STRING_ID(gui, sandbox_objects_menu) ||
		screen_name == STRING_ID(gui, sandbox_objects_menu_splitscreen) ||
		screen_name == STRING_ID(gui, sandbox_object_properties_panel) ||
		screen_name == STRING_ID(gui, sandbox_object_properties_panel_splitscreen) ||
		screen_name == STRING_ID(gui, observer_camera_list_screen) ||
		screen_name == STRING_ID(gui, observer_camera_list_splitscreen) ||
		screen_name == STRING_ID(gui, saved_film_control_pad) ||
		screen_name == STRING_ID(gui, saved_film_take_screenshot) ||
		screen_name == STRING_ID(gui, saved_film_snippet) ||
		screen_name == STRING_ID(gui, terminal_screen) ||
		screen_name == STRING_ID(gui, message);
}

//.text:00AAC430 ; 
//.text:00AAC510 ; 
//.text:00AAC650 ; 
//.text:00AAC6C0 ; 
//.text:00AAC760 ; private: void c_window_manager::process_unhandled_events(dword)
//.text:00AAC910 ; public: void c_window_manager::render(e_window_index, long, rectangle2d const*, bool)
//.text:00AACB10 ; public: void c_window_manager::render_fade()
//.text:00AACBD0 ; public: void c_window_manager::reset_screens()
//.text:00AACE40 ; public: void c_window_manager::run_screen_hs_script(long)
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
//.text:00AAD730 ; void __cdecl window_manager_add_widgets_to_render_list_recursive(rectangle2d const*, c_gui_widget*, s_window_manager_screen_render_data*, e_controller_index, long)
//.text:00AAD8B0 ; void __cdecl window_manager_build_render_data_for_screen(rectangle2d const*, c_gui_screen_widget*, s_window_manager_screen_render_data*)

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

//.text:00AADA20 ; void __cdecl window_manager_render_screen_internal(s_window_manager_static_render_data*, long, rectangle2d*, bool)

void __cdecl window_manager_reset_screens()
{
	INVOKE(0x00AADAC0, window_manager_reset_screens);
}

//.text:00AADAE0 ; bool __cdecl window_manager_screen_is_alert_or_dialog(c_gui_screen_widget const*)
//.text:00AADB10 ; void __cdecl window_manager_storage_devices_changed()


