#include "networking/session/network_session_parameters_session.hpp"

bool __cdecl session_mode_decode_function(c_bitstream* packet, void* data, int32 data_size)
{
	return INVOKE(0x00459710, session_mode_decode_function, packet, data, data_size);
}

void __cdecl session_mode_encode_function(c_bitstream* packet, const void* data, int32 data_size)
{
	INVOKE(0x00459770, session_mode_encode_function, packet, data, data_size);
}

bool __cdecl session_size_decode_function(c_bitstream* packet, void* data, int32 data_size)
{
	return INVOKE(0x00459840, session_size_decode_function, packet, data, data_size);
}

void __cdecl session_size_encode_function(c_bitstream* packet, const void* data, int32 data_size)
{
	INVOKE(0x00459890, session_size_encode_function, packet, data, data_size);
}

e_network_session_mode c_network_session_parameter_session_mode::get() const
{
	return INVOKE_CLASS_MEMBER(0x00458FE0, c_network_session_parameter_session_mode, get);
}

bool c_network_session_parameter_session_mode::request_change(e_network_session_mode session_mode)
{
	return INVOKE_CLASS_MEMBER(0x00459630, c_network_session_parameter_session_mode, request_change, session_mode);
}

bool c_network_session_parameter_session_mode::set(e_network_session_mode session_mode)
{
	return INVOKE_CLASS_MEMBER(0x00459A40, c_network_session_parameter_session_mode, set, session_mode);
}

