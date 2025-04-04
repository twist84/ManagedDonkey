#include "cseries/cseries.hpp"

#include "game/players.hpp"
#include "interface/gui_screens/pregame_lobby/gui_screen_pregame_lobby.hpp"

enum e_controller_index;
enum e_network_session_class;
struct c_gui_screen_pregame_lobby_campaign :
	public c_gui_screen_pregame_lobby
{
public:
	bool __thiscall handle_list_item_chosen_(c_controller_input_message const* message, long list_name, c_gui_list_item_widget* list_item_widget, c_gui_data* datasource);

//protected:
	long m_last_player_count;
	e_network_session_class m_session_class;
	s_player_identifier m_last_player_identifiers[16];
	e_controller_index m_use_saved_game_from_controller;
	dword m_progress_dialog_start_time_milliseconds;
	bool m_catalogue_enumeration_active;
};
static_assert(sizeof(c_gui_screen_pregame_lobby_campaign) == sizeof(c_gui_screen_pregame_lobby) + 0x94);
static_assert(0x03390 == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_last_player_count));
static_assert(0x03394 == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_session_class));
static_assert(0x03398 == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_last_player_identifiers));
static_assert(0x03418 == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_use_saved_game_from_controller));
static_assert(0x0341C == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_progress_dialog_start_time_milliseconds));
static_assert(0x03420 == OFFSETOF(c_gui_screen_pregame_lobby_campaign, m_catalogue_enumeration_active));

