#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_parameters_generic.hpp"

template <typename t_type, long k_payload_size>
struct c_network_session_parameter_chunked :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<t_type>
{
protected:
	long m_transmitted_peer_update_chunk_indices[17];

	long m_change_request_desired_chunk;

	long m_update_chunk_index;
	long m_change_request_chunk_index;

	byte m_update_payload[k_payload_size];
	long m_update_encoded_size;
	long m_update_encoded_checksum;
	long m_update_chunk_count;

	byte m_change_request_payload[k_payload_size];
	long m_change_request_encoded_size;
	long m_change_request_encoded_checksum;
	long m_change_request_chunk_count;

	long m_update_max_chunk_count;
	long m_change_request_max_chunk_count;
};
