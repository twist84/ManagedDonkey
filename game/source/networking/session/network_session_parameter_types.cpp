#include "networking/session/network_session_parameter_types.hpp"

#include "cseries/cseries.hpp"
#include "networking/network_globals.hpp"
#include "networking/session/network_session_parameter_type_collection.hpp"
#include "networking/session/network_session_parameters_session.hpp"

void __cdecl network_session_parameters_register_parameter_type_collection(c_network_session_parameter_type_collection* session_parameter_types)
{
	INVOKE(0x0048ACF0, network_session_parameters_register_parameter_type_collection, session_parameter_types);

	//g_network_parameter_types = session_parameter_types;
}

void __cdecl network_session_parameter_types_register(c_network_session_parameter_type_collection* session_parameter_types)
{
	INVOKE(0x004DB5F0, network_session_parameter_types_register, session_parameter_types);

	// $IMPLEMENT

	//session_parameter_types->register_session_parameter_type(
	//	_network_session_parameter_type_session_size,
	//	"session-size",
	//	session_size_encode_function,
	//	session_size_decode_function,
	//	sizeof(s_network_session_parameter_session_size),
	//	session_size_encode_function,
	//	session_size_decode_function,
	//	sizeof(s_network_session_parameter_session_size));
	//
	//session_parameter_types->register_session_parameter_type(
	//	_network_session_parameter_type_session_mode,
	//	"session-mode",
	//	session_mode_encode_function,
	//	session_mode_decode_function,
	//	sizeof(s_network_session_parameter_session_mode),
	//	session_mode_encode_function,
	//	session_mode_decode_function,
	//	sizeof(s_network_session_parameter_session_mode));

	// $TODO add the other parameter types

}

