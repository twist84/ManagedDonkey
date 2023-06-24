#include "networking/session/network_session_parameters_game.hpp"

long c_network_session_parameter_countdown_timer::get_countdown_timer()
{
	if (get_allowed() && m_data.delayed_reason)
		return m_data.countdown_timer;

    return -1;
}
