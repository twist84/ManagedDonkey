#pragma once

#include "interface/c_gui_screen_widget.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"

struct s_message_of_the_day_popup;
struct s_player_identifier;
struct c_gui_screen_pregame_lobby :
	public c_gui_screen_widget
{
public:
	bool __thiscall handle_controller_input_message_(c_controller_input_message* message);
	bool __thiscall handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);
	void __thiscall initialize_();

public:
	virtual ~c_gui_screen_pregame_lobby();
	virtual void initialize() override;
	virtual void post_initialize() override;
	virtual void dispose() override;
	virtual c_gui_bitmap_widget* create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition) override;
	virtual c_gui_list_widget* create_list_widget(const s_list_widget_block* definition) override;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;
	virtual e_render_data_size get_render_data_size() override;
	virtual e_gui_location get_gui_location() const override;
	virtual void initialize_datasource() override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual bool handle_dialog_result(const c_dialog_result_message* message) override;
	virtual bool team_switching_allowed();
	virtual bool postgame_stats_enabled();
	virtual int32 get_lobby_header() = 0;
	virtual int32 get_lobby_title() = 0;
	virtual int32 get_start_button_name() = 0;
	virtual e_gui_game_mode get_gui_game_mode() = 0;
	virtual void start_fade_during_countdown();
	virtual void stop_fade_during_countdown();

public:
	c_gui_screen_pregame_lobby(int32 name);
	void load_game_variant_editing_screen(e_controller_index controller_index);

//private:
	bool select_player_in_roster(e_controller_index controller_index, const s_player_identifier* target_player_identifier);

public:
	void update_status();

protected:
	int32 m_last_countdown_value;
	bool m_is_fading;
	bool m_change_teams_visible;
	bool m_postgame_stats_visible;
	bool m_vidmaster_popup_downloaded;
	c_http_blf_simple_downloader<s_message_of_the_day_popup, 4665> m_vidmaster_popup_downloader;
	bool m_vidmaster_popup_shown;
	int32 m_url_key;
};
static_assert(sizeof(c_gui_screen_pregame_lobby) == sizeof(c_gui_screen_widget) + 0x18F0);

