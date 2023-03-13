#include "networking/session/network_session_parameters_session.hpp"

e_network_session_mode c_network_session_parameter_session_mode::get() const
{
	return DECLFUNC(0x00458FE0, e_network_session_mode, __thiscall, c_network_session_parameter_session_mode const*)(this);
}

bool c_network_session_parameter_session_mode::request_change(e_network_session_mode session_mode)
{
	return DECLFUNC(0x00459630, e_network_session_mode, __thiscall, c_network_session_parameter_session_mode const*, e_network_session_mode)(this, session_mode);
}

bool c_network_session_parameter_session_mode::set(e_network_session_mode session_mode)
{
	return DECLFUNC(0x00459A40, e_network_session_mode, __thiscall, c_network_session_parameter_session_mode const*, e_network_session_mode)(this, session_mode);
}

