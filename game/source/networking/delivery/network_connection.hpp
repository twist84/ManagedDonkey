#pragma once

#include "cseries/cseries.hpp"
#include "memory/sliding_window.hpp"

//struct c_network_connection :
//	c_network_channel_client
//{
//	struct s_connection_incoming_packet
//	{
//		word_flags flags;
//	};
//	static_assert(sizeof(s_connection_incoming_packet) == 0x2);
//
//	struct s_connection_outgoing_packet
//	{
//		dword timestamp;
//		long size_on_wire;
//		dword round_trip_msec;
//		short unretired_window_size;
//		word_flags flags;
//	};
//	static_assert(sizeof(s_connection_outgoing_packet) == 0x10);
//
//	char const* m_name;
//
//	c_sliding_window m_incoming_window;
//	c_sliding_window m_outgoing_window;
//
//	long m_incoming_window_size_bytes;
//	long m_outgoing_window_size_bytes;
//
//	c_static_array<s_connection_incoming_packet, 128> m_incoming_packets;
//	c_static_array<s_connection_outgoing_packet, 128> m_outgoing_packets;
//
//	byte __data958[0x10];
//
//	dword m_packet_timeout;
//};
//static_assert(sizeof(c_network_connection) == 0x960);

