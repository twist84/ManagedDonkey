#pragma once

#include "memory/bitstream.hpp"

enum e_network_session_parameter_type
{
	k_network_session_parameter_type_count = 49
};

struct c_network_session_parameter_type_collection
{
	struct s_network_session_parameter_type
	{
		using encode_t = void __cdecl(c_bitstream*, void const*, long);
		using decode_t = bool __cdecl(c_bitstream*, void*, long);

		bool initialized;
		char const* session_parameter_name;

		encode_t* update_encode;
		decode_t* update_decode;
		long update_size;

		encode_t* change_request_encode;
		decode_t* change_request_decode;
		long change_request_size;
	};

public:
	void register_session_parameter_type(
		e_network_session_parameter_type session_parameter_type,
		char const* session_parameter_name, 
		s_network_session_parameter_type::encode_t* update_encode,
		s_network_session_parameter_type::decode_t* update_decode,
		long update_size, 
		s_network_session_parameter_type::encode_t* change_request_encode,
		s_network_session_parameter_type::decode_t* change_request_decode,
		long change_request_size
	);

protected:
	s_network_session_parameter_type m_parameters[k_network_session_parameter_type_count];
};
static_assert(sizeof(c_network_session_parameter_type_collection) == 0x620);
