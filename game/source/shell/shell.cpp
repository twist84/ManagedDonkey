#include "shell/shell.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

char const* k_network_session_mode_names[k_network_session_mode_count]
{
	"none",
	"idle",
	"setup",
	"in_game",
	"end_game",
	"post_game",
	"matchmaking_start",
	"matchmaking_searching",
	"matchmaking_gathering",
	"matchmaking_slave",
	"matchmaking_disbanding",
	"matchmaking_arbitrating",
	"matchmaking_choosing_game"
};

char const* k_ui_game_mode_names[k_ui_game_mode_count]
{
	"campaign",
	"matchmaking",
	"multiplayer",
	"map_editor",
	"theater",
	"survival"
};

char const* k_gui_network_session_advertisement_mode_names[k_gui_network_session_advertisement_mode_count]
{
	"open_to_public",
	"open_to_friends",
	"invite_only",
	"system_link",
	"offline",
	"full"
};

void __cdecl shell_halt_with_message(char const* message)
{
    INVOKE(0x0042E4B0, shell_halt_with_message, message);
}

bool __cdecl shell_get_system_identifier(char* system_identifier, long system_identifier_len)
{
    return INVOKE(0x0051CE40, shell_get_system_identifier, system_identifier, system_identifier_len);
}

char const* network_session_mode_get_name(long session_mode)
{
	if (session_mode < _network_session_mode_none || session_mode >= k_network_session_mode_count)
		return "<invalid 'network_session_mode'>";

	return k_network_session_mode_names[session_mode];
}

char const* ui_game_mode_get_name(long ui_game_mode)
{
	if (ui_game_mode < _ui_game_mode_campaign || ui_game_mode >= k_ui_game_mode_count)
		return "<invalid 'ui_game_mode'>";

	return k_ui_game_mode_names[ui_game_mode];
}

char const* gui_network_session_advertisement_mode_get_name(long advertisement_mode)
{
	if (advertisement_mode < _gui_network_session_advertisement_mode_open_to_public || advertisement_mode >= k_gui_network_session_advertisement_mode_count)
		return "<invalid 'ui_game_mode'>";

	return k_gui_network_session_advertisement_mode_names[advertisement_mode];
}

