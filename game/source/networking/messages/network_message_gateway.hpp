#pragma once

#include "cseries/cseries.hpp"
#include "memory/bitstream.hpp"
#include "networking/messages/network_out_of_band_consumer.hpp"
#include "networking/transport/transport_address.hpp"

struct c_network_link;
struct c_network_message_type_collection;
struct c_network_message_handler;
enum e_network_message_type;
struct c_network_message_gateway :
	public c_network_out_of_band_consumer
{
public:
	static bool __cdecl _read_packet_header(c_bitstream* packet);
	static bool __fastcall _receive_out_of_band_packet(c_network_message_gateway* _this, void* unused, transport_address const* incoming_address, c_bitstream* packet);
	static void __fastcall _write_packet_header(c_network_message_gateway* _this, void* unused);

	void __cdecl attach_handler(c_network_message_handler* message_handler);
	void __cdecl destroy_gateway();
	bool __cdecl initialize_gateway(c_network_link* link, c_network_message_type_collection const* message_types);

private:
	static bool __cdecl read_packet_header(c_bitstream* packet);

public:
	bool __cdecl receive_out_of_band_packet(transport_address const* incoming_address, c_bitstream* packet);
	void __cdecl send_all_pending_messages();
	bool __cdecl send_message_broadcast(e_network_message_type message_type, long data_size, void const* data, word port);
	bool __cdecl send_message_directed(transport_address const* outgoing_address, e_network_message_type message_type, long data_size, void const* data);
	void __cdecl send_pending_messages_to_address(transport_address const* address);

private:
	void __cdecl write_packet_header();

protected:
	bool m_initialized;
	c_network_link* m_link;
	c_network_message_type_collection const* m_message_types;
	c_network_message_handler* m_message_handler;
	bool m_outgoing_packet_pending;
	byte m_outgoing_packet_storage[0x5BE + 1];
	transport_address m_outgoing_packet_address;
	c_bitstream m_outgoing_packet;
};
static_assert(sizeof(c_network_message_gateway) == 0x688);

