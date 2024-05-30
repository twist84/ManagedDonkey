#include "networking/session/network_session_parameter_types.hpp"

#include "cseries/cseries.hpp"

void __cdecl network_session_parameters_register_parameter_type_collection(c_network_session_parameter_type_collection* session_parameter_types)
{
	INVOKE(0x0048ACF0, network_session_parameters_register_parameter_type_collection, session_parameter_types);
}

void __cdecl network_session_parameter_types_register(c_network_session_parameter_type_collection* session_parameter_types)
{
	INVOKE(0x004DB5F0, network_session_parameter_types_register, session_parameter_types);
}

