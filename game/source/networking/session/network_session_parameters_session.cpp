#include "networking/session/network_session_parameters_session.hpp"

e_network_session_mode c_network_session_parameter_session_mode::get() const
{
	return INVOKE_CLASS_MEMBER(0x00458FE0, c_network_session_parameter_session_mode::get);
}

bool c_network_session_parameter_session_mode::request_change(e_network_session_mode session_mode)
{
	return INVOKE_CLASS_MEMBER(0x00459630, c_network_session_parameter_session_mode::request_change, session_mode);
}

bool c_network_session_parameter_session_mode::set(e_network_session_mode session_mode)
{
	return INVOKE_CLASS_MEMBER(0x00459A40, c_network_session_parameter_session_mode::set, session_mode);
}

