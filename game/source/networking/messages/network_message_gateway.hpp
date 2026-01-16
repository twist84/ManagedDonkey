#pragma once

#include "cseries/cseries.hpp"
#include "memory/bitstream.hpp"
#include "networking/messages/network_out_of_band_consumer.hpp"
#include "networking/transport/transport_address.hpp"

class c_network_link;
class c_network_message_type_collection;
class c_network_message_handler;

class c_network_message_gateway :
	public c_network_out_of_band_consumer
{
public:
	virtual bool receive_out_of_band_packet(const transport_address* incoming_address, c_bitstream* packet) override;

	c_network_message_gateway();
	~c_network_message_gateway();

	void attach_handler(c_network_message_handler* message_handler);
	void destroy_gateway();
	bool initialize_gateway(c_network_link* link, const c_network_message_type_collection* message_types);

private:
	static bool __cdecl read_packet_header(c_bitstream* packet);

public:
	void send_all_pending_messages();
	bool send_message_broadcast(e_network_message_type message_type, int32 data_size, const void* data, uns16 port);
	bool send_message_directed(const transport_address* outgoing_address, e_network_message_type message_type, int32 data_size, const void* data);
	void send_pending_messages_to_address(const transport_address* address);

private:
	void write_packet_header();

protected:
	bool m_initialized;
	c_network_link* m_link;
	const c_network_message_type_collection* m_message_types;
	c_network_message_handler* m_message_handler;
	bool m_outgoing_packet_pending;
	byte m_outgoing_packet_storage[0x5BE /* k_network_link_maximum_game_data_size */];
	byte __pad5D3[0x1];
	transport_address m_outgoing_packet_address;
	c_bitstream m_outgoing_packet;
};
COMPILE_ASSERT(sizeof(c_network_message_gateway) == 0x688);

extern char const k_network_message_packet_header[];
extern char const k_network_message_experimental_packet_header[];
extern bool net_experimental;

