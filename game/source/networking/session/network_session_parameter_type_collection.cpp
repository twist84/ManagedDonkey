#include "networking/session/network_session_parameter_type_collection.hpp"
#include "networking/session/network_session_parameters_base.hpp"

#include "cseries/cseries.hpp"

#include <cassert>

void c_network_session_parameter_type_collection::register_session_parameter_type(
	e_network_session_parameter_type session_parameter_type,
	char const* session_parameter_name,
	s_network_session_parameter_type::encode_t* update_encode,
	s_network_session_parameter_type::decode_t* update_decode,
	long update_size,
	s_network_session_parameter_type::encode_t* change_request_encode,
	s_network_session_parameter_type::decode_t* change_request_decode,
	long change_request_size
)
{
	s_network_session_parameter_type* parameter = &m_parameters[session_parameter_type];
	assert(session_parameter_name);
	assert(update_encode);
	assert(update_decode);
	assert(update_size > 0);
	assert(update_size <= k_network_session_parameters_update_payload_size);
	assert(change_request_encode);
	assert(change_request_decode);
	assert(change_request_size > 0);
	assert(change_request_size <= k_network_session_parameters_request_payload_size);
	assert(!parameter->initialized);

	parameter->session_parameter_name = session_parameter_name;
	parameter->update_encode = update_encode;
	parameter->update_decode = update_decode;
	parameter->update_size = update_size;
	parameter->change_request_encode = change_request_encode;
	parameter->change_request_decode = change_request_decode;
	parameter->change_request_size = change_request_size;
	parameter->initialized = true;


	DECLFUNC(0x004D27A0,
		void, __thiscall, c_network_session_parameter_type_collection const*,
		e_network_session_parameter_type,
		char const*,
		s_network_session_parameter_type::encode_t*,
		s_network_session_parameter_type::decode_t*,
		long,
		s_network_session_parameter_type::encode_t*,
		s_network_session_parameter_type::decode_t*,
		long change_request_size
	)(
		this,
		session_parameter_type,
		session_parameter_name,
		update_encode,
		update_decode,
		update_size,
		change_request_encode,
		change_request_decode,
		change_request_size
		);
}