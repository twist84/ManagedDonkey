#pragma once

#include "cseries/cseries.hpp"
#include "memory/sliding_window.hpp"

//enum e_network_message_type;
//struct c_network_message_type_collection;
//struct c_network_message_queue :
//	public c_network_channel_client
//{
//	struct s_incoming_outgoing_record;
//	struct s_incoming_fragment_record;
//
//public:
//	void send_message(e_network_message_type message_type, long raw_message_size, void const* raw_message_payload);
//
//	char __data[4];
//
//	char const* name;
//	c_network_message_type_collection* m_message_types;
//
//	c_sliding_window m_outgoing_window;
//	s_incoming_fragment_record* m_outgoing_fragment_list_head;
//	s_incoming_fragment_record* m_outgoing_fragment_list_tail;
//
//	c_sliding_window m_incoming_window;
//	s_incoming_fragment_record* m_incoming_fragment_list_head;
//	s_incoming_fragment_record* m_incoming_fragment_list_tail;
//
//	long m_outgoing_payload_bytes;
//	long m_incoming_payload_bytes;
//};

