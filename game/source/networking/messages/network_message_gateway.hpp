#pragma once

#include "cseries/cseries.hpp"
#include "memory/bitstream.hpp"
#include "networking/messages/network_out_of_band_consumer.hpp"
#include "networking/transport/transport_address.hpp"

struct c_network_link;
struct c_network_message_type_collection;
struct c_network_message_handler;

struct c_network_message_gateway :
	public c_network_out_of_band_consumer
{
public:
	virtual bool receive_out_of_band_packet(transport_address const* incoming_address, c_bitstream* packet) override;

	c_network_message_gateway();
	~c_network_message_gateway();

	void attach_handler(c_network_message_handler* message_handler);
	void destroy_gateway();
	bool initialize_gateway(c_network_link* link, c_network_message_type_collection const* message_types);

private:
	static bool __cdecl read_packet_header(c_bitstream* packet);

public:
	void send_all_pending_messages();
	bool send_message_broadcast(e_network_message_type message_type, int32 data_size, void const* data, uns16 port);
	bool send_message_directed(transport_address const* outgoing_address, e_network_message_type message_type, int32 data_size, void const* data);
	void send_pending_messages_to_address(transport_address const* address);

private:
	void write_packet_header();

protected:
	bool m_initialized;
	c_network_link* m_link;
	c_network_message_type_collection const* m_message_types;
	c_network_message_handler* m_message_handler;
	bool m_outgoing_packet_pending;
	byte m_outgoing_packet_storage[0x5BE /* k_network_link_maximum_game_data_size */];
	byte __pad5D3[0x1];
	transport_address m_outgoing_packet_address;
	c_bitstream m_outgoing_packet;
};
static_assert(sizeof(c_network_message_gateway) == 0x688);

extern char const k_network_message_packet_header[];
extern char const k_network_message_experimental_packet_header[];
extern bool net_experimental;

