#include "networking/session/network_session_parameters.hpp"

#include "networking/messages/network_messages_session_parameters.hpp"

bool c_network_session_parameters::handle_update(s_network_message_parameters_update const* update)
{
	return INVOKE_CLASS_MEMBER(0x004898A0, c_network_session_parameters, handle_update, update);
}

