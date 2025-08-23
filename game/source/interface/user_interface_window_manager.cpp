#include "interface/user_interface_window_manager.hpp"

#include "bink/bink_playback.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/c_gui_model_widget.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/c_gui_widget.hpp"
#include "interface/gui_custom_bitmap_storage.hpp"
#include "interface/gui_screens/boot_betrayer/gui_screen_boot_betrayer.hpp"
#include "interface/gui_screens/campaign/gui_screen_campaign_select_difficulty.hpp"
#include "interface/gui_screens/campaign/gui_screen_campaign_select_level.hpp"
#include "interface/gui_screens/campaign/gui_screen_campaign_settings.hpp"
#include "interface/gui_screens/error_dialogs/screen_error_dialog_ok.hpp"
#include "interface/gui_screens/game_details/gui_game_details.hpp"
#include "interface/gui_screens/main_menu/gui_screen_main_menu.hpp"
#include "interface/gui_screens/player_select/gui_player_select_screen_widget.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_campaign.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_mapeditor.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_matchmaking.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_multiplayer.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby_theater.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_selection.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_switch_lobby.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/gui_screens/start_menu/gui_screen_start_menu.hpp"
#include "interface/gui_screens/start_menu/panes/game_campaign/start_menu_game_campaign.hpp"
#include "interface/gui_screens/start_menu/panes/game_editor/start_menu_game_editor.hpp"
#include "interface/gui_screens/start_menu/panes/game_multiplayer/start_menu_game_multiplayer.hpp"
#include "interface/gui_screens/start_menu/panes/game_saved_film/start_menu_game_saved_film.hpp"
#include "interface/gui_screens/start_menu/panes/settings/start_menu_settings.hpp"
#include "interface/gui_screens/start_menu/panes/settings_appearance_emblem/start_menu_settings_appearance_emblem.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_utilities.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "text/font_cache.hpp"

//#define ISWIP

REFERENCE_DECLARE(0x05260F34, c_window_manager, g_window_manager);
#if defined(ISWIP)
HOOK_DECLARE_CLASS_MEMBER(0x00AA8E00, c_window_manager, allocate_named_screen);
HOOK_DECLARE_CLASS(0x00AABFD0, c_window_manager, named_screen_defined_in_code);
#endif
HOOK_DECLARE(0x00AAD730, window_manager_add_widgets_to_render_list_recursive);

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
	//return UI_MALLOC(c_gui_screen_codeless_widget, screen_name);
}

c_gui_screen_widget* c_window_manager::allocate_named_screen(int32 screen_name)
{
#if !defined(ISWIP)
	return INVOKE_CLASS_MEMBER(0x00AA8E00, c_window_manager, allocate_named_screen, screen_name);
#else
	//c_gui_screen_widget* result = NULL;
	//HOOK_INVOKE_CLASS_MEMBER(result =, c_window_manager, allocate_named_screen, screen_name);
	//return result;
	
	if (c_window_manager::locate_screen_definition_tag_by_name(screen_name) == NONE && screen_name != STRING_ID(gui, message))
	{
		event(_event_warning, "ui: failed to allocate screen '%s': no tag definition found", string_id_get_string_const(screen_name));
		return NULL;
	}
	
	switch (screen_name) // H3 OG
	{
	case STRING_ID(gui, error_ok):
		return UI_MALLOC(c_error_dialog_ok_screen_widget, screen_name);
	case STRING_ID(gui, main_menu):
		return UI_MALLOC(c_main_menu_screen_widget, screen_name);
	case STRING_ID(gui, start_menu):
		return UI_MALLOC(c_start_menu_screen_widget, screen_name);
	case STRING_ID(gui, start_menu_game_campaign):
		return UI_MALLOC(c_start_menu_game_campaign, screen_name);
	case STRING_ID(gui, start_menu_game_multiplayer):
		return UI_MALLOC(c_start_menu_game_multiplayer, screen_name);
	//case STRING_ID(gui, start_menu_game_editor):
	//	return UI_MALLOC(c_start_menu_game_editor, screen_name);
	case STRING_ID(gui, start_menu_game_saved_film):
		return UI_MALLOC(c_start_menu_game_saved_film, screen_name);
	//case STRING_ID(gui, start_menu_hq):
	//	return UI_MALLOC(c_start_menu_headquarters, screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record):
	//	return UI_MALLOC(c_gui_start_menu_hq_service_record, screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record_file_share):
	//	return UI_MALLOC(c_gui_start_menu_hq_service_record_file_share, screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_bungie):
	//	return UI_MALLOC(c_gui_start_menu_hq_service_record_file_share, screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_choose_category):
	//	return UI_MALLOC(c_gui_start_menu_hq_service_record_file_share_choose_category, screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_choose_item):
	//	return UI_MALLOC(c_gui_start_menu_hq_service_record_file_share_choose_item, screen_name);
	//case STRING_ID(gui, start_menu_hq_service_record_file_share_item_selected):
	//	return UI_MALLOC(c_gui_start_menu_hq_service_record_file_share_item_selected, screen_name);
	//case STRING_ID(gui, start_menu_hq_screenshots):
	//	return UI_MALLOC(c_gui_start_menu_hq_screenshots, screen_name);
	//case STRING_ID(gui, start_menu_hq_screenshots_item_options):
	//	return UI_MALLOC(c_gui_start_menu_hq_screenshots_options, screen_name);
	//case STRING_ID(gui, start_menu_hq_transfers):
	//	return UI_MALLOC(c_gui_start_menu_hq_transfers, screen_name);
	//case STRING_ID(gui, start_menu_hq_transfers_item_selected):
	//	return UI_MALLOC(c_gui_start_menu_hq_transfers_item_selected, screen_name);
	case STRING_ID(gui, start_menu_settings):
		return UI_MALLOC(c_start_menu_settings, screen_name);
	//case STRING_ID(gui, start_menu_settings_controls):
	//	return UI_MALLOC(c_start_menu_settings_controls, screen_name);
	//case STRING_ID(gui, start_menu_settings_controls_button):
	//	return UI_MALLOC(c_start_menu_settings_controls_button, screen_name);
	//case STRING_ID(gui, start_menu_settings_controls_thumbstick):
	//	return UI_MALLOC(c_start_menu_settings_controls_thumbstick, screen_name);
	//case STRING_ID(gui, start_menu_settings_appearance):
	//	return UI_MALLOC(c_start_menu_settings_appearance, screen_name);
	//case STRING_ID(gui, start_menu_settings_appearance_model):
	//	return UI_MALLOC(c_start_menu_settings_appearance_model, screen_name);
	case STRING_ID(gui, start_menu_settings_appearance_emblem):
		return UI_MALLOC(c_start_menu_settings_appearance_emblem, screen_name);
	//case STRING_ID(gui, start_menu_settings_appearance_colors):
	//	return UI_MALLOC(c_start_menu_settings_appearance_colors, screen_name);
	//case STRING_ID(gui, start_menu_settings_camera):
	//	return UI_MALLOC(c_start_menu_settings_camera, screen_name);
	//case STRING_ID(gui, start_menu_settings_display):
	//	return UI_MALLOC(c_start_menu_settings_display, screen_name);
	//case STRING_ID(gui, start_menu_settings_film_autosave):
	//	return UI_MALLOC(c_start_menu_settings_film_autosave, screen_name);
	//case STRING_ID(gui, screenshots_viewer):
	//	return UI_MALLOC(c_screenshots_viewer_screen_widget, screen_name);
	//case STRING_ID(gui, screenshots_file_share_previewer):
	//	return UI_MALLOC(c_screenshots_file_share_previewer_screen_widget, screen_name);
	//case STRING_ID(gui, gui_alert_nonblocking):
	//case STRING_ID(gui, gui_alert_toast):
	//case STRING_ID(gui, gui_alert_ingame_full):
	//case STRING_ID(gui, gui_alert_ingame_split):
	//	return UI_MALLOC(c_gui_alert_screen_widget, screen_name);
	//case STRING_ID(gui, gui_dialog_two_items):
	//case STRING_ID(gui, gui_dialog_four_items):
	//	return UI_MALLOC(c_gui_dialog_screen_widget, screen_name);
	//case STRING_ID(gui, in_progress):
	//case STRING_ID(gui, in_progress_mini):
	//case STRING_ID(gui, in_progress_mini_me):
	//	return UI_MALLOC(c_gui_screen_in_progress, screen_name);
	//case STRING_ID(gui, spartan_milestone_dialog):
	//	return UI_MALLOC(c_gui_spartan_milestone, screen_name);
	//case STRING_ID(gui, spartan_rank_dialog):
	//	return UI_MALLOC(c_gui_spartan_rank, screen_name);
	case STRING_ID(gui, campaign_settings):
		return UI_MALLOC(c_gui_screen_campaign_settings, screen_name);
	case STRING_ID(gui, campaign_select_difficulty):
		return UI_MALLOC(c_gui_screen_campaign_select_difficulty, screen_name);
	case STRING_ID(gui, campaign_select_level):
		return UI_MALLOC(c_gui_screen_campaign_select_level, screen_name);
	case STRING_ID(gui, pregame_lobby_campaign):
		return UI_MALLOC(c_gui_screen_pregame_lobby_campaign, screen_name);
	case STRING_ID(gui, pregame_lobby_matchmaking):
		return UI_MALLOC(c_gui_screen_pregame_lobby_matchmaking, screen_name);
	case STRING_ID(gui, pregame_lobby_multiplayer):
		return UI_MALLOC(c_gui_screen_pregame_lobby_multiplayer, screen_name);
	case STRING_ID(gui, pregame_lobby_mapeditor):
		return UI_MALLOC(c_gui_screen_pregame_lobby_mapeditor, screen_name);
	case STRING_ID(gui, pregame_lobby_theater):
		return UI_MALLOC(c_gui_screen_pregame_lobby_theater, screen_name);
	case STRING_ID(gui, pregame_switch_lobby):
		return UI_MALLOC(c_gui_screen_pregame_switch_lobby, screen_name);
	case STRING_ID(gui, pregame_selection):
		return UI_MALLOC(c_gui_screen_pregame_selection, screen_name);
	//case STRING_ID(gui, pregame_advanced_options_matchmaking):
	//	return UI_MALLOC(c_gui_screen_matchmaking_advanced_options, screen_name);
	//case STRING_ID(gui, postgame_lobby):
	//	return UI_MALLOC(c_gui_screen_postgame_lobby, screen_name);
	case STRING_ID(gui, player_select):
		return UI_MALLOC(c_gui_player_select_screen_widget, screen_name);
	//case STRING_ID(gui, game_browser):
	//	return UI_MALLOC(c_gui_game_browser, screen_name);
	case STRING_ID(gui, boot_betrayer):
	case STRING_ID(gui, boot_betrayer_splitscreen):
		return UI_MALLOC(c_gui_screen_boot_betrayer, screen_name);
	//case STRING_ID(gui, maximum_party_size):
	//	return UI_MALLOC(c_gui_screen_maximum_party_size, screen_name);
	case STRING_ID(gui, game_details):
		return UI_MALLOC(c_gui_game_details, screen_name);
	//case STRING_ID(gui, game_options):
	//	return UI_MALLOC(c_gui_screen_game_options, screen_name);
	//case STRING_ID(gui, alpha_legal):
	//	return UI_MALLOC(c_gui_screen_alpha_legal, screen_name);
	//case STRING_ID(gui, alpha_motd):
	//	return UI_MALLOC(c_gui_screen_alpha_motd, screen_name);
	//case STRING_ID(gui, alpha_locked_down):
	//	return UI_MALLOC(c_gui_screen_alpha_locked_down, screen_name);
	//case STRING_ID(gui, forge_legal):
	//	return UI_MALLOC(c_gui_screen_forge_legal, screen_name);
	//case STRING_ID(gui, carnage_report):
	//	return UI_MALLOC(c_gui_screen_carnage_report, screen_name);
	case STRING_ID(gui, carnage_report_player_details):
		return UI_MALLOC(c_gui_player_select_screen_widget, screen_name);
	case STRING_ID(gui, scoreboard):
	case STRING_ID(gui, scoreboard_half):
		return UI_MALLOC(c_gui_screen_scoreboard, screen_name);
	case STRING_ID(gui, scoreboard_player_select):
	case STRING_ID(gui, scoreboard_player_select_splitscreen):
		return UI_MALLOC(c_gui_player_select_screen_widget, screen_name);
	//case STRING_ID(gui, matchmaking_searching):
	//	return UI_MALLOC(c_gui_screen_matchmaking_searching, screen_name);
	//case STRING_ID(gui, matchmaking_match_found):
	//	return UI_MALLOC(c_gui_screen_matchmaking_match_found, screen_name);
	//case STRING_ID(gui, sandbox_budget_screen):
	//case STRING_ID(gui, sandbox_budget_screen_splitscreen):
	//	return UI_MALLOC(c_sandbox_budget_summary_screen_widget, screen_name);
	//case STRING_ID(gui, sandbox_objects_menu):
	//case STRING_ID(gui, sandbox_objects_menu_splitscreen):
	//	return UI_MALLOC(c_sandbox_object_menu_screen_widget, screen_name);
	//case STRING_ID(gui, sandbox_object_properties_panel):
	//case STRING_ID(gui, sandbox_object_properties_panel_splitscreen):
	//	return UI_MALLOC(c_sandbox_object_properties_screen_widget, screen_name);
	//case STRING_ID(gui, observer_camera_list_screen):
	//case STRING_ID(gui, observer_camera_list_splitscreen):
	//	return UI_MALLOC(c_observer_camera_list_screen, screen_name);
	//case STRING_ID(gui, saved_film_control_pad):
	//	return UI_MALLOC(c_saved_film_control_pad, screen_name);
	//case STRING_ID(gui, saved_film_take_screenshot):
	//	return UI_MALLOC(c_saved_film_take_screenshot, screen_name);
	//case STRING_ID(gui, saved_film_snippet):
	//	return UI_MALLOC(c_saved_film_snippet_screen, screen_name);
	//case STRING_ID(gui, terminal_screen):
	//	return UI_MALLOC(c_gui_screen_terminal, screen_name);
	case STRING_ID(gui, message):
		return UI_MALLOC(c_motd_popup_screen_widget, screen_name);
	}
	
	//switch (screen_name) // ODST
	//{
	//case STRING_ID(gui, campaign_save_in_progress):
	//	return UI_MALLOC(c_gui_screen_in_progress, screen_name);
	//case STRING_ID(gui, campaign_select_scoring):
	//case STRING_ID(gui, survival_select_scoring):
	//	return UI_MALLOC(c_gui_screen_campaign_select_scoring, screen_name);
	//case STRING_ID(gui, campaign_select_skulls):
	//case STRING_ID(gui, survival_select_skulls):
	//	return UI_MALLOC(c_gui_screen_campaign_select_skulls, screen_name);
	//case STRING_ID(gui, main_menu_select_difficulty):
	//case STRING_ID(gui, survival_select_difficulty):
	//	return UI_MALLOC(c_gui_screen_campaign_select_difficulty, screen_name);
	//case STRING_ID(gui, survival_select_level):
	//	return UI_MALLOC(c_gui_screen_campaign_select_level, screen_name);
	//case STRING_ID(gui, pregame_lobby_survival):
	//	return UI_MALLOC(c_gui_screen_pregame_lobby_survival, screen_name);
	//case STRING_ID(gui, pregame_players_selection):
	//	return UI_MALLOC(c_gui_screen_pregame_selection, screen_name);
	//case STRING_ID(gui, carnage_report_loading):
	//	return UI_MALLOC(c_gui_carnage_report_loading_screen, screen_name);
	//case STRING_ID(gui, campaign_carnage_report):
	//case STRING_ID(gui, survival_carnage_report):
	//	return UI_MALLOC(c_gui_screen_carnage_report, screen_name);
	//}
	
	//switch (screen_name) // H3 MCC
	//{
	//case STRING_ID(gui, start_menu_settings_keyboard):
	//	return UI_MALLOC(c_start_menu_settings_keyboard, screen_name);
	//case STRING_ID(gui, start_menu_settings_controls_keyboard):
	//	return UI_MALLOC(c_start_menu_settings_controls_keyboard, screen_name);
	//case STRING_ID(gui, start_menu_settings_voice):
	//	return UI_MALLOC(c_start_menu_settings_voice, screen_name);
	//case STRING_ID(gui, start_menu_settings_video):
	//	return UI_MALLOC(c_start_menu_settings_video, screen_name);
	//case STRING_ID(gui, start_menu_confirm_resolution):
	//	return UI_MALLOC(c_start_menu_confirm_resolution, screen_name);
	//}

	// $TODO: remove this when each of the above classes exist and have a constructor 
	c_gui_screen_widget* result = NULL;
	HOOK_INVOKE_CLASS_MEMBER(result =, c_window_manager, allocate_named_screen, screen_name);
	if (result == NULL)
	{
		event(_event_warning, "ui: no c_gui_screen_class registered for ui screen '%s'", string_id_get_string_const(screen_name));
		return NULL;
	}
	
	return result;
#endif
}

//.text:00AAA7A0 ; public: bool c_window_manager::any_ui_active()
//.text:00AAA7D0 ; public: bool c_window_manager::any_ui_active_in_window(e_window_index)

void c_window_manager::begin_transition_out(c_gui_screen_widget* screen, e_screen_transition_type transition_type)
{
	INVOKE_CLASS_MEMBER(0x00AAA7F0, c_window_manager, begin_transition_out, screen, transition_type);
}

void c_window_manager::close_all_screens(const c_gui_screen_widget** except_these, int32 except_these_count)
{
	INVOKE_CLASS_MEMBER(0x00AAA870, c_window_manager, close_all_screens, except_these, except_these_count);
}

//.text:00AAA970 ; public: bool c_window_manager::controller_input_should_be_suppressed(e_controller_index)

void c_window_manager::debug_widget_state(e_gui_widget_type type, int32 name, e_widget_debug_state state, bool activate, bool include_children)
{
	INVOKE_CLASS_MEMBER(0x00AAA9F0, c_window_manager, debug_widget_state, type, name, state, activate, include_children);
}

void c_window_manager::debug_widget_state_for_screen(c_gui_screen_widget* screen, e_gui_widget_type type, int32 name, e_widget_debug_state state, bool activate, bool include_children)
{
	INVOKE_CLASS_MEMBER(0x00AAAA70, c_window_manager, debug_widget_state_for_screen, screen, type, name, state, activate, include_children);

	//c_gui_widget* child_widget = screen->get_child_widget(type, name);
	//if (child_widget)
	//{
	//	event(_event_warning, "ui: failed to find widget of type '%s' named '%s'",
	//		gui_widget_type_to_string(type),
	//		string_id_get_string_const(name));
	//	return;
	//}
	//
	//c_window_manager::debug_widget_state_for_widget(child_widget, state, activate, include_children);
}

void c_window_manager::debug_widget_state_for_widget(c_gui_widget* widget, e_widget_debug_state state, bool activate, bool include_children)
{
	INVOKE_CLASS_MEMBER(0x00AAAAA0, c_window_manager, debug_widget_state_for_widget, widget, state, activate, include_children);

	//switch (state)
	//{
	//case _debug_name:
	//{
	//	widget->set_debug_name(activate);
	//}
	//break;
	//case _debug_bounds:
	//{
	//	widget->set_debug_bounds(activate);
	//}
	//break;
	//case _debug_animation_state:
	//{
	//	widget->set_debug_animation_state(activate);
	//}
	//break;
	//case _debug_rotation_origin:
	//{
	//	widget->set_debug_rotation_origin(activate);
	//}
	//break;
	//default:
	//{
	//	VASSERT("unreachable");
	//}
	//break;
	//}
	//
	//if (!include_children)
	//{
	//	return;
	//}
	//
	//for (c_gui_widget* child_widget = widget->get_children(); child_widget; child_widget = widget->get_next())
	//{
	//	c_window_manager::debug_widget_state_for_widget(child_widget, state, activate, include_children);
	//}
}

void c_window_manager::dispose()
{
	INVOKE_CLASS_MEMBER(0x00AAABB0, c_window_manager, dispose);

	//c_gui_custom_bitmap_storage_manager::get()->dispose();
	//if (m_active_screens)
	//{
	//	ASSERT(m_active_screens->actual_count == 0);
	//	data_dispose(m_active_screens);
	//	m_active_screens = NULL;
	//}
}

void c_window_manager::dispose_from_old_map()
{
	INVOKE_CLASS_MEMBER(0x00AAABE0, c_window_manager, dispose_from_old_map);

	//FONT_CACHE_SCOPE_LOCK;
	//
	//for (int32 window_index = 0; window_index < k_number_of_render_windows; window_index++)
	//{
	//	for (int32 channel_count = 0; channel_count < m_current_channel_count[window_index].peek(); channel_count++)
	//	{
	//		c_gui_screen_widget* screen = m_channels[window_index][channel_count];
	//		if (!screen)
	//		{
	//			continue;
	//		}
	//		c_window_manager::dispose_screen(screen);
	//	}
	//}
}

void c_window_manager::dispose_screen(c_gui_screen_widget* screen)
{
	INVOKE_CLASS_MEMBER(0x00AAAE80, c_window_manager, dispose_screen, screen);
}

//.text:00AAB090 ; public: bool c_load_screen_message::get_applies_even_to_codeless_screens() const
//.text:00AAB0A0 ; public: c_gui_screen_widget* c_window_manager::get_bottommost_screen(e_window_index)

c_gui_widget* c_gui_widget::get_children()
{
	//return INVOKE_CLASS_MEMBER(0x00AAB0D0, c_gui_widget, get_children);

	return m_children.get_value();
}

//.text:00AAB100 ; 
//.text:00AAB110 ; 
//.text:00AAB120 ; public: bool c_window_manager::get_fading() const
//.text:00AAB130 ; public: real32 c_window_manager::get_fading_amount() const
//.text:00AAB140 ; public: bool c_window_manager::get_fading_max() const
//.text:00AAB160 ; public: bool c_window_manager::get_fading_min() const
//.text:00AAB180 ; public: int32 c_load_screen_message::get_focus_on_load_column_name() const
//.text:00AAB190 ; public: int32 c_load_screen_message::get_focus_on_load_column_value() const
//.text:00AAB1A0 ; public: int32 c_load_screen_message::get_focus_on_load_element_handle() const
//.text:00AAB1B0 ; public: int32 c_load_screen_message::get_focus_on_load_list_name() const
//.text:00AAB1C0 ; public: int32 c_message::get_game_time_at_creation() const

e_gui_location c_gui_screen_widget::get_gui_location() const
{
	//return INVOKE_CLASS_MEMBER(0x00AAB1D0, c_gui_screen_widget, get_gui_location);

	return _gui_location_none;
}

//.text:00AAB1E0 ; public: int32 c_load_screen_message::get_layered_position() const

c_gui_widget* c_gui_widget::get_next()
{
	//return INVOKE_CLASS_MEMBER(0x00AAB1F0, c_gui_widget, get_next);

	return m_next.get_value();
}

c_gui_widget* c_gui_widget::get_parent()
{
	//return INVOKE_CLASS_MEMBER(0x00AAB220, c_gui_widget, get_parent);

	return m_parent.get_value();
}

//.text:00AAB250 ; public: int32 c_load_screen_message::get_parent_screen_index() const

e_render_data_size c_gui_screen_widget::get_render_data_size()
{
	//return INVOKE_CLASS_MEMBER(0x00AAB260, c_gui_screen_widget, get_render_data_size);

	return k_render_data_size_normal;
}

s_window_manager_screen_render_data* c_gui_screen_widget::get_render_state()
{
	//return INVOKE_CLASS_MEMBER(0x00AAB270, c_gui_screen_widget, get_render_state);

	if (!m_render_data.valid())
	{
		return NULL;
	}

	return &m_render_data;
}

e_window_index c_window_manager::get_render_window_for_screen(c_gui_screen_widget* screen)
{
	return INVOKE_CLASS_MEMBER(0x00AAB290, c_window_manager, get_render_window_for_screen, screen);
}

//.text:00AAB300 ; public: real32 c_gui_widget::get_rendered_depth()
//.text:00AAB310 ; public: bool c_load_screen_message::get_respond_to_controller_events() const
//.text:00AAB320 ; public: c_gui_screen_widget* c_window_manager::get_responding_screen(e_controller_index, e_window_index*)
//.text:00AAB3F0 ; public: c_gui_screen_widget* c_window_manager::get_responding_screen_in_window(e_controller_index, e_window_index)
//.text:00AAB480 ; public: bool c_gui_screen_widget::get_responds_to_controller_events() const

c_gui_screen_widget* c_window_manager::get_screen_above(e_window_index window_index, const c_gui_screen_widget* screen)
{
	return INVOKE_CLASS_MEMBER(0x00AAB490, c_window_manager, get_screen_above, window_index, screen);
}

c_gui_screen_widget* c_window_manager::get_screen_below(e_window_index window_index, const c_gui_screen_widget* screen_widget)
{
	return INVOKE_CLASS_MEMBER(0x00AAB4E0, c_window_manager, get_screen_below, window_index, screen_widget);
}

c_gui_screen_widget* c_window_manager::get_screen_by_name(e_window_index window_index, int32 screen_name)
{
	return INVOKE_CLASS_MEMBER(0x00AAB550, c_window_manager, get_screen_by_name, window_index, screen_name);
}

//.text:00AAB5B0 ; public: c_gui_screen_widget* c_window_manager::get_screen_by_screen_index(int32)
//.text:00AAB620 ; public: int32 c_gui_screen_widget::get_screen_index() const

c_gui_screen_widget* c_window_manager::get_topmost_screen(e_window_index window_index)
{
	return INVOKE_CLASS_MEMBER(0x00AAB630, c_window_manager, get_topmost_screen, window_index);
}

//.text:00AAB680 ; 
//.text:00AAB690 ; public: c_gui_widget* c_robust_pointer<c_gui_widget>::get_value()
//.text:00AAB6B0 ; public: void* c_robust_void_pointer::get_value()
//.text:00AAB6D0 ; 
//.text:00AAB750 ; int __cdecl gui_widget_depth_sort_proc(const void*, const void*)

void c_window_manager::handle_global_controller_event(s_event_record* event_record)
{
	INVOKE_CLASS_MEMBER(0x00AAB7D0, c_window_manager, handle_global_controller_event, event_record);
}

//.text:00AAB9C0 ; public: void c_window_manager::initialize()
//.text:00AABA00 ; public: void c_window_manager::initialize_for_new_map()
//.text:00AABA40 ; private: void c_window_manager::insert_screen_after_named_screen(e_window_index, c_gui_screen_widget*, int32)
//.text:00AABBB0 ; private: bool __cdecl c_robust_void_pointer::internally_consistent() const
//.text:00AABBD0 ; public: bool c_window_manager::is_screen_queued_to_be_loaded(int32, e_window_index)

c_gui_screen_widget* c_window_manager::load_screen(e_controller_index controller_index, bool load_as_error, const c_load_screen_message* screen_message, int32 window_index)
{
	return INVOKE_CLASS_MEMBER(0x00AABC30, c_window_manager, load_screen, controller_index, load_as_error, screen_message, window_index);
}

int32 __cdecl c_window_manager::locate_screen_definition_tag_by_name(int32 name)
{
	return INVOKE(0x00AABF50, c_window_manager::locate_screen_definition_tag_by_name, name);
}

bool __cdecl c_window_manager::named_screen_defined_in_code(int32 screen_name)
{
#if !defined(ISWIP)
	return INVOKE(0x00AABFD0, c_window_manager::named_screen_defined_in_code, screen_name);
#else
	switch (screen_name) // H3 OG
	{
	case STRING_ID(gui, error_ok):
	case STRING_ID(gui, main_menu):
	case STRING_ID(gui, start_menu):
	case STRING_ID(gui, start_menu_game_campaign):
	case STRING_ID(gui, start_menu_game_multiplayer):
	case STRING_ID(gui, start_menu_game_editor):
	case STRING_ID(gui, start_menu_game_saved_film):
	case STRING_ID(gui, start_menu_hq):
	case STRING_ID(gui, start_menu_hq_service_record):
	case STRING_ID(gui, start_menu_hq_service_record_file_share):
	case STRING_ID(gui, start_menu_hq_service_record_file_share_bungie):
	case STRING_ID(gui, start_menu_hq_service_record_file_share_choose_category):
	case STRING_ID(gui, start_menu_hq_service_record_file_share_choose_item):
	case STRING_ID(gui, start_menu_hq_service_record_file_share_item_selected):
	case STRING_ID(gui, start_menu_hq_screenshots):
	case STRING_ID(gui, start_menu_hq_screenshots_item_options):
	case STRING_ID(gui, start_menu_hq_transfers):
	case STRING_ID(gui, start_menu_hq_transfers_item_selected):
	case STRING_ID(gui, start_menu_settings):
	case STRING_ID(gui, start_menu_settings_controls):
	case STRING_ID(gui, start_menu_settings_controls_button):
	case STRING_ID(gui, start_menu_settings_controls_thumbstick):
	case STRING_ID(gui, start_menu_settings_appearance):
	case STRING_ID(gui, start_menu_settings_appearance_model):
	case STRING_ID(gui, start_menu_settings_appearance_emblem):
	case STRING_ID(gui, start_menu_settings_appearance_colors):
	case STRING_ID(gui, start_menu_settings_camera):
	case STRING_ID(gui, start_menu_settings_display):
	case STRING_ID(gui, start_menu_settings_film_autosave):
	case STRING_ID(gui, screenshots_viewer):
	case STRING_ID(gui, screenshots_file_share_previewer):
	case STRING_ID(gui, gui_alert_nonblocking):
	case STRING_ID(gui, gui_alert_toast):
	case STRING_ID(gui, gui_alert_ingame_full):
	case STRING_ID(gui, gui_alert_ingame_split):
	case STRING_ID(gui, gui_dialog_two_items):
	case STRING_ID(gui, gui_dialog_four_items):
	case STRING_ID(gui, in_progress):
	case STRING_ID(gui, in_progress_mini):
	case STRING_ID(gui, in_progress_mini_me):
	case STRING_ID(gui, spartan_milestone_dialog):
	case STRING_ID(gui, spartan_rank_dialog):
	case STRING_ID(gui, campaign_settings):
	case STRING_ID(gui, campaign_select_difficulty):
	case STRING_ID(gui, campaign_select_level):
	case STRING_ID(gui, pregame_lobby_campaign):
	case STRING_ID(gui, pregame_lobby_matchmaking):
	case STRING_ID(gui, pregame_lobby_multiplayer):
	case STRING_ID(gui, pregame_lobby_mapeditor):
	case STRING_ID(gui, pregame_lobby_theater):
	case STRING_ID(gui, pregame_switch_lobby):
	case STRING_ID(gui, pregame_selection):
	case STRING_ID(gui, pregame_advanced_options_matchmaking):
	case STRING_ID(gui, postgame_lobby):
	case STRING_ID(gui, player_select):
	case STRING_ID(gui, game_browser):
	case STRING_ID(gui, boot_betrayer):
	case STRING_ID(gui, boot_betrayer_splitscreen):
	case STRING_ID(gui, maximum_party_size):
	case STRING_ID(gui, game_details):
	case STRING_ID(gui, game_options):
	case STRING_ID(gui, alpha_legal):
	case STRING_ID(gui, alpha_motd):
	case STRING_ID(gui, alpha_locked_down):
	case STRING_ID(gui, forge_legal):
	case STRING_ID(gui, carnage_report):
	case STRING_ID(gui, carnage_report_player_details):
	case STRING_ID(gui, scoreboard):
	case STRING_ID(gui, scoreboard_half):
	case STRING_ID(gui, scoreboard_player_select):
	case STRING_ID(gui, scoreboard_player_select_splitscreen):
	case STRING_ID(gui, matchmaking_searching):
	case STRING_ID(gui, matchmaking_match_found):
	case STRING_ID(gui, sandbox_budget_screen):
	case STRING_ID(gui, sandbox_budget_screen_splitscreen):
	case STRING_ID(gui, sandbox_objects_menu):
	case STRING_ID(gui, sandbox_objects_menu_splitscreen):
	case STRING_ID(gui, sandbox_object_properties_panel):
	case STRING_ID(gui, sandbox_object_properties_panel_splitscreen):
	case STRING_ID(gui, observer_camera_list_screen):
	case STRING_ID(gui, observer_camera_list_splitscreen):
	case STRING_ID(gui, saved_film_control_pad):
	case STRING_ID(gui, saved_film_take_screenshot):
	case STRING_ID(gui, saved_film_snippet):
	case STRING_ID(gui, terminal_screen):
	case STRING_ID(gui, message):
		return true;
	}
	
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
	
	//switch (screen_name) // H3 MCC
	//{
	//case STRING_ID(gui, start_menu_settings_keyboard):
	//case STRING_ID(gui, start_menu_settings_controls_keyboard):
	//case STRING_ID(gui, start_menu_settings_voice):
	//case STRING_ID(gui, start_menu_settings_video):
	//case STRING_ID(gui, start_menu_confirm_resolution):
	//	return true;
	//}
	
	return false;
#endif
}

//.text:00AAC430 ; public: bool c_window_manager::non_dead_cam_ui_active_for_game_player_controller(e_controller_index)

void __cdecl c_window_manager::play_controller_sound(int32 optional_sound_tag_override_index, e_event_type event_type, e_controller_component controller_component)
{
	//INVOKE(0x00AAC510, c_window_manager::play_controller_sound, optional_sound_tag_override_index, event_type, controller_component);

	switch (event_type)
	{
	case _event_type_tab_up:
	case _event_type_tab_down:
	case _event_type_alt_stick_up:
	case _event_type_alt_stick_down:
	case _event_type_alt_tab_up:
	case _event_type_alt_tab_down:
	{
		user_interface_play_sound(_ui_global_sound_effect_cursor_up_down, optional_sound_tag_override_index);
	}
	break;
	case _event_type_tab_left:
	case _event_type_tab_right:
	case _event_type_alt_stick_left:
	case _event_type_alt_stick_right:
	case _event_type_alt_tab_left:
	case _event_type_alt_tab_right:
	{
		user_interface_play_sound(_ui_global_sound_effect_cursor_left_right, optional_sound_tag_override_index);
	}
	break;
	case _event_type_button_press:
	{
		switch (controller_component)
		{
		case _controller_component_button_a:
		{
			user_interface_play_sound(_ui_global_sound_effect_button_a, optional_sound_tag_override_index);
		}
		break;
		case _controller_component_button_b:
		{
			user_interface_play_sound(_ui_global_sound_effect_button_b, optional_sound_tag_override_index);
		}
		break;
		case _controller_component_button_x:
		{
			user_interface_play_sound(_ui_global_sound_effect_button_x, optional_sound_tag_override_index);
		}
		break;
		case _controller_component_button_y:
		{
			user_interface_play_sound(_ui_global_sound_effect_button_y, optional_sound_tag_override_index);
		}
		break;
		case _controller_component_button_right_shoulder:
		{
			user_interface_play_sound(_ui_global_sound_effect_button_right_shoulder, optional_sound_tag_override_index);
		}
		break;
		case _controller_component_button_left_shoulder:
		{
			user_interface_play_sound(_ui_global_sound_effect_button_left_shoulder, optional_sound_tag_override_index);
		}
		break;
		case _controller_component_button_left_trigger:
		{
			user_interface_play_sound(_ui_global_sound_effect_button_left_trigger, optional_sound_tag_override_index);
		}
		break;
		case _controller_component_button_right_trigger:
		{
			user_interface_play_sound(_ui_global_sound_effect_button_right_trigger, optional_sound_tag_override_index);
		}
		break;
		case _controller_component_button_dpad_up:
		case _controller_component_button_dpad_down:
		{
			user_interface_play_sound(_ui_global_sound_effect_cursor_up_down, optional_sound_tag_override_index);
		}
		break;
		case _controller_component_button_dpad_left:
		case _controller_component_button_dpad_right:
		{
			user_interface_play_sound(_ui_global_sound_effect_cursor_left_right, optional_sound_tag_override_index);
		}
		break;
		case _controller_component_button_start:
		{
			user_interface_play_sound(_ui_global_sound_effect_button_start, optional_sound_tag_override_index);
		}
		break;
		case _controller_component_button_back:
		{
			user_interface_play_sound(_ui_global_sound_effect_button_back, optional_sound_tag_override_index);
		}
		break;
		}
	}
	break;
	}
}

//.text:00AAC650 ; c_window_manager::predict_immediate_resources
//.text:00AAC6C0 ; c_window_manager::predict_pending_resources
//.text:00AAC730 ; public: void c_window_manager::prepare_for_tag_reload()

void __cdecl c_window_manager::print_active_screen_strings_tag_name()
{
	//INVOKE(0x00AAC740, c_window_manager, print_active_screen_strings_tag_name);

	c_gui_screen_widget* topmost_screen = window_manager_get()->get_topmost_screen(k_number_of_player_windows);
	if (!topmost_screen)
	{
		return;
	}

	const char* tag_name = topmost_screen->get_multilingual_unicode_string_list_tag_name();
	if (!tag_name)
	{
		tag_name = "unavailable";
	}

	const char* topmost_screen_name_string = string_id_get_string_const(topmost_screen->m_name);
	console_printf("ui:debug: screen '%s' uses tag file '%s'",
		topmost_screen_name_string,
		tag_name);
}

void c_window_manager::print_active_screens()
{
	//INVOKE_CLASS_MEMBER(0x00AAC750, c_window_manager, print_active_screens);

	const char* window_strings[k_number_of_render_windows]
	{
		"player 1",
		"player 2",
		"player 3",
		"player 4",
		"console",
	};

	for (int32 window_index = 0; window_index < k_number_of_render_windows; window_index++)
	{
		const char* window_string = window_strings[window_index];

		for (int32 channel_count = 0; channel_count < m_current_channel_count[window_index].peek(); channel_count++)
		{
			c_gui_screen_widget* screen = m_channels[window_index][channel_count];
			if (!screen)
			{
				continue;
			}

			int32 channel_name = screen->m_name;
			const char* channel_name_string = string_id_get_string_const(channel_name);
			console_printf("ui:debug: window '%s' name '%s'",
				window_string,
				channel_name_string);
		}
	}
}

//.text:00AAC760 ; private: void c_window_manager::process_unhandled_events(uns32)

void c_window_manager::render(e_window_index window_index, int32 user_index, const rectangle2d* viewport_bounds, bool is_screenshot)
{
	//INVOKE_CLASS_MEMBER(0x00AAC910, c_window_manager, render, window_index, user_index, viewport_bounds, is_screenshot);

	ASSERT(VALID_INDEX(window_index, k_number_of_render_windows));
	
	if (bink_playback_ui_rendering_inhibited())
	{
		return;
	}
	
	int32 screen_index_array[k_maximum_number_of_channels_per_render_window]{};
	int32 screen_count = 0;
	
	{
		FONT_CACHE_SCOPE_LOCK;
	
		c_gui_custom_bitmap_storage_manager::get()->update_render();
		m_last_known_viewport_bounds[window_index] = *viewport_bounds;
		for (int32 channel_count = 0; channel_count < m_current_channel_count[window_index].peek(); channel_count++)
		{
			if (c_gui_screen_widget* channel = m_channels[window_index][channel_count])
			{
				ASSERT(screen_count < NUMBEROF(screen_index_array));
				screen_index_array[screen_count++] = channel->m_screen_index;
			}
		}
	}
	ASSERT(VALID_COUNT(screen_count, NUMBEROF(screen_index_array)));
	
	bool is_rendering_fade = false;
	if (window_index == k_number_of_player_windows && m_render_fade)
	{
		render_fade();
		is_rendering_fade = true;
	}
	
	for (int32 screen_num = 0; screen_num < screen_count; screen_num++)
	{
		s_window_manager_static_render_data render_data{};
		render_data.current_count = 0;
	
		{
			FONT_CACHE_SCOPE_LOCK;
	
			int32 screen_index = screen_index_array[screen_num];
			if (s_screen_handle_datum* active_screen = DATUM_TRY_AND_GET(m_active_screens, s_screen_handle_datum, screen_index))
			{
				if (c_gui_screen_widget* screen = active_screen->screen)
				{
					screen->update_render();
					s_window_manager_screen_render_data* screen_render_data = &screen->m_render_data;
					if (screen_render_data->valid())
					{
						if (screen_render_data && equal_rectangle2d(&screen_render_data->built_for_viewport_bounds, viewport_bounds))
						{
							ASSERT(sizeof(render_data.render_data_buffer) >= screen_render_data->render_data_buffer_count);
	
							csmemcpy(&render_data, screen_render_data->render_data_buffer, screen_render_data->render_data_buffer_count);
							render_data.render_data_buffer_count = screen_render_data->render_data_buffer_count;
							csmemcpy(render_data.render_list, screen_render_data->render_list, sizeof(s_depth_sorted_render_widget) * s_window_manager_static_render_data::k_maximum_rendered_child_widgets_per_screen);
							render_data.current_count = screen_render_data->current_count;
						}
					}
				}
			}
		}
	
		if (render_data.current_count > 0)
		{
			window_manager_render_screen_internal(&render_data, user_index, viewport_bounds, is_screenshot);
		}
	}
	
	if (window_index == k_number_of_player_windows && !is_rendering_fade)
	{
		render_fade();
	}
}


void c_window_manager::render_fade()
{
	INVOKE_CLASS_MEMBER(0x00AACB10, c_window_manager, render_fade);
}

void c_window_manager::reset_screens()
{
	INVOKE_CLASS_MEMBER(0x00AACBD0, c_window_manager, reset_screens);
}

void c_window_manager::run_screen_hs_script(int32 script_index)
{
	INVOKE_CLASS_MEMBER(0x00AACE40, c_window_manager, run_screen_hs_script, script_index);
}

//.text:00AACEA0 ; 
//.text:00AACEC0 ; 
//.text:00AACEF0 ; 
//.text:00AACF10 ; 

void c_window_manager::set_fade_out_and_quit_campaign(bool value, bool render_fade)
{
	//INVOKE_CLASS_MEMBER(0x00AACF40, c_window_manager, set_fade_out_and_quit_campaign, value, render_fade);

	m_fade_in = true;
	m_fade_out_and_quit_campaign = value;
	m_render_fade = render_fade;
}

void c_window_manager::set_fading(bool fade_state)
{
	//INVOKE_CLASS_MEMBER(0x00AACF60, c_window_manager, set_fading, fade_state);

	m_fade_in = fade_state;
}

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

void __cdecl window_manager_add_widgets_to_render_list_recursive(const rectangle2d* window_bounds, c_gui_widget* root_widget, s_window_manager_screen_render_data* render_data, e_controller_index local_controller_index)
{
	//INVOKE(0x00AAD730, window_manager_add_widgets_to_render_list_recursive, window_bounds, root_widget, render_data, local_controller_index);

	for (c_gui_widget* child_widget = root_widget->get_children(); child_widget; child_widget = child_widget->get_next())
	{
		if (child_widget->m_type == _gui_screen || !child_widget->should_render(NULL))
		{
			continue;
		}

		window_manager_add_widgets_to_render_list_recursive(window_bounds, child_widget, render_data, local_controller_index);
	}

	bool add_to_render_list = false;
	if (!root_widget->should_render(&add_to_render_list) || !add_to_render_list)
	{
		return;
	}

	if (render_data->current_count >= s_window_manager_screen_render_data::k_maximum_rendered_child_widgets_per_screen)
	{
		int32 widget_name = root_widget->m_name;
		if (c_gui_screen_widget* parent_screen = root_widget->get_parent_screen())
		{
			widget_name = parent_screen->m_name;
		}
		event(_event_error, "ui: '%s' is trying to render too many widgets! (max= %ld)",
			string_id_get_string_const(widget_name),
			s_window_manager_screen_render_data::k_maximum_rendered_child_widgets_per_screen);

		return;
	}

	int32 size_needed = 0;
	switch (root_widget->m_type)
	{
	case _gui_text:
	{
		unsigned int text_buffer_size = ((c_gui_text_widget*)root_widget)->get_text_buffer_size();
		switch (text_buffer_size)
		{
		case 48:
		{
			size_needed = sizeof(s_gui_text_widget_small_render_data);
		}
		break;
		case 256:
		{
			size_needed = sizeof(s_gui_text_widget_large_render_data);
		}
		break;
		case 1024:
		{
			size_needed = sizeof(s_gui_text_widget_extra_large_render_data);
		}
		break;
		default:
		{
			VASSERT("unhandled text widget buffer size!");
		}
		break;
		}
	}
	break;
	case _gui_bitmap:
	{
		size_needed = sizeof(s_gui_bitmap_widget_render_data);
	}
	break;
	case _gui_model:
	{
		size_needed = sizeof(s_gui_model_widget_render_data);
	}
	break;
	default:
	{
		size_needed = sizeof(s_gui_widget_render_data);
	}
	break;
	}

	ASSERT(render_data->render_data_buffer_count >= 0 && size_needed >= 0 && (render_data->render_data_buffer_count + size_needed >= render_data->render_data_buffer_count));
	if ((render_data->render_data_buffer_count + size_needed) >= render_data->render_data_buffer_length)
	{
		int32 widget_name = root_widget->m_name;
		if (c_gui_screen_widget* parent_screen = root_widget->get_parent_screen())
		{
			widget_name = parent_screen->m_name;
		}
		event(_event_error, "ui: no render buffer space for '%s'",
			string_id_get_string_const(widget_name));

		return;
	}

	s_gui_widget_render_data* render_data_assemble = (s_gui_widget_render_data*)&render_data->render_data_buffer[render_data->render_data_buffer_count];
	root_widget->assemble_render_data(render_data_assemble, window_bounds, local_controller_index, true, true, true);

	render_data->render_list[render_data->current_count].type = root_widget->m_type;
	render_data->render_list[render_data->current_count].render_data_offset = render_data->render_data_buffer_count;
	render_data->render_list[render_data->current_count].depth = root_widget->m_animated_state.position.z;
	render_data->render_list[render_data->current_count].depth_bias = root_widget->get_render_depth_bias();

	ASSERT(render_data_assemble == (s_gui_widget_render_data*)&render_data->render_data_buffer[render_data->render_list[render_data->current_count].render_data_offset]);
	render_data->render_data_buffer_count += size_needed;
	render_data->current_count++;
}

void __cdecl window_manager_build_render_data_for_screen(const rectangle2d* viewport_bounds, c_gui_screen_widget* screen, s_window_manager_screen_render_data* render_data)
{
	INVOKE(0x00AAD8B0, window_manager_build_render_data_for_screen, viewport_bounds, screen, render_data);

	//render_data->clear();
	//render_data->built_for_viewport_bounds = *viewport_bounds;
	//
	//if (screen->should_render(NULL))
	//{
	//	window_manager_add_widgets_to_render_list_recursive(viewport_bounds, screen, render_data, screen->get_driving_controller());
	//}
	//
	//if (render_data->current_count > 0)
	//{
	//	qsort(render_data->render_list, render_data->current_count, sizeof(s_depth_sorted_render_widget), gui_widget_depth_sort_proc);
	//}
}

c_window_manager* __cdecl window_manager_get()
{
	//return INVOKE(0x00AAD930, window_manager_get);

	return &g_window_manager;
}

//.text:00AAD940 ; bool __cdecl window_manager_handle_dialog_result_message(const c_dialog_result_message*)

void __cdecl window_manager_load_screen_hs(int32 screen_name)
{
	INVOKE(0x00AAD9A0, window_manager_load_screen_hs, screen_name);

	//if (user_interface_memory_initialized())
	//{
	//	if (c_load_screen_message* screen_message = UI_MALLOC(c_load_screen_message, screen_name, k_any_controller, k_number_of_player_windows, STRING_ID(gui, top_most)))
	//	{
	//		user_interface_messaging_post(screen_message);
	//	}
	//}
}

void __cdecl window_manager_render_screen_internal(s_window_manager_static_render_data* render_data, int32 user_index, const rectangle2d* viewport_bounds, bool is_screenshot)
{
	//INVOKE(0x00AADA20, window_manager_render_screen_internal, render_data, user_index, viewport_bounds, is_screenshot);

	real32 near_clip_plane_distance;
	real32 projection_plane_distance;
	real32 far_clip_plane_distance;
	user_interface_get_projection_plane_distances(&near_clip_plane_distance, &projection_plane_distance, &far_clip_plane_distance);

	real32 near_depth = near_clip_plane_distance - projection_plane_distance;
	real32 far_depth = far_clip_plane_distance - projection_plane_distance;
	near_clip_plane_distance = near_clip_plane_distance - projection_plane_distance;
	far_clip_plane_distance = far_clip_plane_distance - projection_plane_distance;

	ASSERT(VALID_COUNT(render_data->current_count, NUMBEROF(render_data->render_list)));
	int32 current_count = render_data->current_count & 0x1FF;
	for (int32 render_index = 0; render_index < current_count; render_index++)
	{
		s_depth_sorted_render_widget* render_item = &render_data->render_list[render_index];
		if (render_item->depth < near_depth || far_depth < render_item->depth)
		{
			continue;
		}

		if (s_gui_widget_render_data* widget_render_data = (s_gui_widget_render_data*)offset_pointer(render_data->render_data_buffer, render_item->render_data_offset))
		{
			c_gui_widget::render(user_index, widget_render_data, viewport_bounds, is_screenshot);

			far_depth = far_clip_plane_distance;
			near_depth = near_clip_plane_distance;
		}
	}
}

void __cdecl window_manager_reset_screens()
{
	INVOKE(0x00AADAC0, window_manager_reset_screens);

	//g_window_manager.reset_screens();
	//user_interface_error_manager_get()->clear_all_errors();
}

//.text:00AADAE0 ; bool __cdecl window_manager_screen_is_alert_or_dialog(const c_gui_screen_widget*)
//.text:00AADB10 ; void __cdecl window_manager_storage_devices_changed()


