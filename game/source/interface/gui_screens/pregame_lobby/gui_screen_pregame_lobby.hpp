#pragma once

#include "interface/c_gui_screen_widget.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"

class c_game_variant;
class c_gui_roster_list_widget;
struct s_message_of_the_day_popup;
struct s_player_identifier;
class c_gui_screen_pregame_lobby :
	public c_gui_screen_widget
{
public:
	bool __thiscall handle_controller_input_message_(const c_controller_input_message* message);
	bool __thiscall handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);
	void __thiscall initialize_();
	void __thiscall initialize_datasource_();
	void __thiscall post_initialize_();
	void __thiscall update_widget_visiblility_();

public:
	virtual ~c_gui_screen_pregame_lobby() = default;
	virtual void initialize() override;
	virtual void post_initialize() override;
	virtual void dispose() override;
	virtual c_gui_bitmap_widget* create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition) override;
	virtual c_gui_list_widget* create_list_widget(const s_list_widget_block* definition) override;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;

protected:
	virtual e_render_data_size get_render_data_size() override;

	virtual e_gui_location get_gui_location() const override;
	virtual void initialize_datasource() override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual bool handle_dialog_result(const c_dialog_result_message* message) override;

protected:
	virtual bool team_switching_allowed();
	virtual bool postgame_stats_enabled();

public:
	virtual int32 get_lobby_header() = 0;
	virtual int32 get_lobby_title() = 0;
	virtual int32 get_start_button_name() = 0;
	virtual e_gui_game_mode get_gui_game_mode() = 0;
	virtual void start_fade_during_countdown();
	virtual void stop_fade_during_countdown();

public:
	c_gui_screen_pregame_lobby(int32 name);

private:
	bool advanced_options_enabled();
	void commit_team_change(int32 player_index, c_gui_roster_list_widget* roster_list_widget, int32 lying_end_time);

protected:
	static e_campaign_id __cdecl get_current_campaign_id();
	static e_map_id __cdecl get_current_map_id();
	static const c_game_variant* get_current_variant();

private:
	int32 get_options_button_name(bool is_leader);

protected:
	static int32 __cdecl get_start_status_text(bool is_leader);

private:
	bool handle_back_out(e_controller_index controller_index);

public:
	void load_game_variant_editing_screen(e_controller_index controller_index);

private:
	bool select_player_in_roster(e_controller_index controller_index, const s_player_identifier* target_player_identifier);

public:
	void update_status();

private:
	void update_widget_visiblility();

protected:
	int32 m_last_countdown_value;
	bool m_is_fading;
	bool m_change_teams_visible;
	bool m_postgame_stats_visible;
	bool m_vidmaster_popup_downloaded;
	c_http_blf_simple_downloader<s_message_of_the_day_popup, 4665> m_vidmaster_popup_downloader;
	bool m_vidmaster_popup_shown;

	// Added back for `update_widget_visiblility`
	bool m_advanced_options_visible;

	int32 m_url_key;
};
COMPILE_ASSERT(sizeof(c_gui_screen_pregame_lobby) == sizeof(c_gui_screen_widget) + 0x18F0);

