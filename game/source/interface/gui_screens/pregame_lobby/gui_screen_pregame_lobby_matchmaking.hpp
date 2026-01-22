#pragma once

#include "cseries/cseries.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "networking/session/network_session_parameters_matchmaking.hpp"

struct s_dynamic_matchmaking_hopper_statistics;
class c_gui_screen_pregame_lobby_matchmaking :
	public c_gui_screen_pregame_lobby
{
public:
	//bool __thiscall handle_list_item_chosen_(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);
	void __thiscall update_nightmap_();
	void __thiscall update_population_();

public:
	virtual ~c_gui_screen_pregame_lobby_matchmaking() = default;
	virtual void initialize() override;
	virtual c_gui_bitmap_widget* create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition) override;
	virtual void update(uns32 current_milliseconds) override;
	virtual bool handle_controller_input_message(const c_controller_input_message* message) override;
	virtual bool handle_list_item_chosen(const c_controller_input_message* message, int32 list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource) override;
	virtual bool handle_dialog_result(const c_dialog_result_message* dialog_result_message) override;
	virtual int32 get_lobby_header() override;
	virtual int32 get_lobby_title() override;
	virtual int32 get_start_button_name() override;
	virtual e_gui_game_mode get_gui_game_mode() override;
	virtual void start_fade_during_countdown() override;
	virtual void stop_fade_during_countdown() override;

public:
	c_gui_screen_pregame_lobby_matchmaking(int32 name);
	bool is_lobby_in_live_mode();

private:
	void update_nightmap();
	void update_population();

public:
//protected:
	c_http_blf_simple_downloader<s_dynamic_matchmaking_hopper_statistics, 2465> m_hopper_statstics_downloader;
	int32 m_url_key;
	c_http_stored_buffer_downloader<102400> m_nightmap_downloader;
	byte pad[0x4];
	bool m_nightmap_download_completed;
	e_network_file_load_status m_last_hopper_catalog_load_status;
	uns16 m_statistics_current_hopper_id;
	s_matchmaking_single_hopper_statistics m_statistics_current_statistics;
};
COMPILE_ASSERT(sizeof(c_gui_screen_pregame_lobby_matchmaking) == sizeof(c_gui_screen_pregame_lobby) + 0x1A6F8);
COMPILE_ASSERT(0x03390 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_hopper_statstics_downloader));
COMPILE_ASSERT(0x043D8 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_url_key));
COMPILE_ASSERT(0x043DC == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_nightmap_downloader));
COMPILE_ASSERT(0x1DA70 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, pad));
COMPILE_ASSERT(0x1DA74 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_nightmap_download_completed));
COMPILE_ASSERT(0x1DA78 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_last_hopper_catalog_load_status));
COMPILE_ASSERT(0x1DA7C == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_statistics_current_hopper_id));
COMPILE_ASSERT(0x1DA80 == OFFSETOF(c_gui_screen_pregame_lobby_matchmaking, m_statistics_current_statistics));

extern bool __cdecl parse_xml_lobby_matchmaking_hopper(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_matchmaking_hopper_population(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_matchmaking_total_population(void* this_ptr, wchar_t* buffer, int32 buffer_length);

