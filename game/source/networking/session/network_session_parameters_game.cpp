#include "networking/session/network_session_parameters_game.hpp"

int32 c_network_session_parameter_countdown_timer::get_countdown_timer()
{
	if (get_allowed() && m_data.delayed_reason)
		return m_data.countdown_timer;

    return NONE;
}

bool c_network_session_parameter_game_start_status::set(const s_network_session_parameter_game_start_status* game_status)
{
	return INVOKE_CLASS_MEMBER(0x00453840, c_network_session_parameter_game_start_status, set, game_status);
}

