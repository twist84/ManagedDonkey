#include "networking/session/network_session_parameter_type_collection.hpp"
#include "networking/session/network_session_parameters_base.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"


void c_network_session_parameter_type_collection::register_session_parameter_type(
	e_network_session_parameter_type session_parameter_type,
	char const* session_parameter_name,
	encode_t* update_encode,
	decode_t* update_decode,
	long update_size,
	encode_t* change_request_encode,
	decode_t* change_request_decode,
	long change_request_size
)
{
	ASSERT(session_parameter_name);
	ASSERT(update_encode);
	ASSERT(update_decode);
	ASSERT(update_size > 0);
	ASSERT(update_size <= k_network_session_parameters_update_payload_size);
	ASSERT(change_request_encode);
	ASSERT(change_request_decode);
	ASSERT(change_request_size > 0);
	ASSERT(change_request_size <= k_network_session_parameters_request_payload_size);

	s_network_session_parameter_type* parameter = &m_parameters[session_parameter_type];
	ASSERT(!parameter->initialized);

	parameter->session_parameter_name = session_parameter_name;
	parameter->update_encode = update_encode;
	parameter->update_decode = update_decode;
	parameter->update_size = update_size;
	parameter->change_request_encode = change_request_encode;
	parameter->change_request_decode = change_request_decode;
	parameter->change_request_size = change_request_size;
	parameter->initialized = true;

	//DECLFUNC(0x004D27A0,
	//	void, __thiscall, c_network_session_parameter_type_collection const*,
	//	e_network_session_parameter_type,
	//	char const*,
	//	encode_t*,
	//	decode_t*,
	//	long,
	//	encode_t*,
	//	decode_t*,
	//	long
	//)(
	//	this,
	//	session_parameter_type,
	//	session_parameter_name,
	//	update_encode,
	//	update_decode,
	//	update_size,
	//	change_request_encode,
	//	change_request_decode,
	//	change_request_size
	//);
}