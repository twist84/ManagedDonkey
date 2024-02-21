#pragma once

#include "cseries/cseries.hpp"
#include "memory/sliding_window.hpp"

//struct c_network_message_type_collection;
//struct c_network_message_queue :
//	c_network_channel_client
//{
//	struct s_incoming_outgoing_record;
//	struct s_incoming_fragment_record;
//
//	char __data[4];
//
//	const char* name;
//	c_network_message_type_collection* m_message_types;
//
//	c_sliding_window m_outgoing_window;
//	c_static_array<s_incoming_outgoing_record*, 2> m_outgoing_fragments;
//
//	c_sliding_window m_incoming_window;
//	c_static_array<s_incoming_fragment_record*, 2> m_incoming_fragments;
//
//	long m_outgoing_payload_bytes;
//	long m_incoming_payload_bytes;
//};
