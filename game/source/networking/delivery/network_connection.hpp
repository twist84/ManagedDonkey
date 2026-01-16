#pragma once

#include "cseries/cseries.hpp"
#include "memory/sliding_window.hpp"

//class c_network_connection :
//	c_network_channel_client
//{
//public:
//	struct s_connection_incoming_packet
//	{
//		uns16 flags;
//	};
//	COMPILE_ASSERT(sizeof(s_connection_incoming_packet) == 0x2);
//
//	struct s_connection_outgoing_packet
//	{
//		uns32 timestamp;
//		int32 size_on_wire;
//		uns32 round_trip_msec;
//		int16 unretired_window_size;
//		uns16 flags;
//	};
//	COMPILE_ASSERT(sizeof(s_connection_outgoing_packet) == 0x10);
//
//	const char* m_name;
//
//	c_sliding_window m_incoming_window;
//	c_sliding_window m_outgoing_window;
//
//	int32 m_incoming_window_size_bytes;
//	int32 m_outgoing_window_size_bytes;
//
//	c_static_array<s_connection_incoming_packet, 128> m_incoming_packets;
//	c_static_array<s_connection_outgoing_packet, 128> m_outgoing_packets;
//
//	byte __data958[0x10];
//
//	uns32 m_packet_timeout;
//};
//COMPILE_ASSERT(sizeof(c_network_connection) == 0x960);

