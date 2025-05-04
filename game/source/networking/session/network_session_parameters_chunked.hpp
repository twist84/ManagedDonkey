#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_parameters_generic.hpp"

template <typename t_type, int32 k_payload_size>
struct c_network_session_parameter_chunked :
	public c_network_session_parameter_base,
	c_generic_network_session_parameter_data<t_type>
{
protected:
	int32 m_transmitted_peer_update_chunk_indices[17];

	int32 m_change_request_desired_chunk;

	int32 m_update_chunk_index;
	int32 m_change_request_chunk_index;

	uns8 m_update_payload[k_payload_size];
	int32 m_update_encoded_size;
	int32 m_update_encoded_checksum;
	int32 m_update_chunk_count;

	uns8 m_change_request_payload[k_payload_size];
	int32 m_change_request_encoded_size;
	int32 m_change_request_encoded_checksum;
	int32 m_change_request_chunk_count;

	int32 m_update_max_chunk_count;
	int32 m_change_request_max_chunk_count;
};
