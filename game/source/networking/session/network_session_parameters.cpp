#include "networking/session/network_session_parameters.hpp"

#include "networking/messages/network_messages_session_parameters.hpp"

bool c_network_session_parameters::handle_update(s_network_message_parameters_update const* update)
{
	return INVOKE_CLASS_MEMBER(0x004898A0, c_network_session_parameters, handle_update, update);
}

bool c_network_session_parameters::parameters_transmitted_to_peers(uns64 parameter_mask) const
{
	return INVOKE_CLASS_MEMBER(0x0048C3B0, c_network_session_parameters, parameters_transmitted_to_peers, parameter_mask);
}

