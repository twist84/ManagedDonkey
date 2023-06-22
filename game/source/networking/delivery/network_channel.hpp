#pragma once

#include "cseries/cseries.hpp"
#include "memory/bitstream.hpp"
#include "networking/delivery/network_connection.hpp"
#include "networking/messages/network_message_queue.hpp"

enum e_network_channel_closure_reason;
enum e_network_read_result;
struct c_network_channel_client
{
public:
	virtual char const* get_client_name() const;
	virtual bool connection_lost(e_network_channel_closure_reason*) const;
	virtual bool has_data_to_transmit(bool*) const;
	virtual long space_required_bits(long, long);
	virtual bool write_to_packet(long, c_bitstream*, long, long);
	virtual e_network_read_result read_from_packet(long*, c_bitstream*);
	virtual void notify_packet_acknowledged(long);
	virtual void notify_packet_retired(long, bool);

	bool m_allocated;
	bool read_simulation_data_expected;
	byte __unknownA;
	byte __unknownB;
};
static_assert(sizeof(c_network_channel_client) == 0x8);

struct c_network_connection : c_network_channel_client
{
	struct s_connection_incoming_packet
	{
		word_flags flags;
	};
	static_assert(sizeof(s_connection_incoming_packet) == 0x2);

	struct s_connection_outgoing_packet
	{
		dword timestamp;
		long size_on_wire;
		dword round_trip_msec;
		short unretired_window_size;
		word_flags flags;
	};
	static_assert(sizeof(s_connection_outgoing_packet) == 0x10);

	const char* m_name;

	c_sliding_window m_incoming_window;
	c_sliding_window m_outgoing_window;

	long m_incoming_window_size_bytes;
	long m_outgoing_window_size_bytes;

	c_static_array<s_connection_incoming_packet, 128> m_incoming_packets;
	c_static_array<s_connection_outgoing_packet, 128> m_outgoing_packets;

	byte __data958[0x10];

	dword m_packet_timeout;
};
static_assert(sizeof(c_network_connection) == 0x960);

struct c_network_message_type_collection;
struct c_network_message_queue : c_network_channel_client
{
	struct s_incoming_outgoing_record;
	struct s_incoming_fragment_record;

	char __data[4];

	const char* name;
	c_network_message_type_collection* m_message_types;

	c_sliding_window m_outgoing_window;
	c_static_array<s_incoming_outgoing_record*, 2> m_outgoing_fragments;

	c_sliding_window m_incoming_window;
	c_static_array<s_incoming_fragment_record*, 2> m_incoming_fragments;

	long m_outgoing_payload_bytes;
	long m_incoming_payload_bytes;
};
struct s_network_channel_client_info
{
	dword flags;
	c_network_channel_client* client;
};
static_assert(sizeof(s_network_channel_client_info) == 0x8);

struct c_network_channel_simulation_interface
{
	bool m_initialized;
	void* m_simulation_context;
	void(__cdecl* m_closure_callback)(void*);
	long m_client_count;
	s_network_channel_client_info m_clients[4];
	bool m_authority;
	bool m_established;
	byte __unknown32;
	byte __unknown33;
};
static_assert(sizeof(c_network_channel_simulation_interface) == 0x34);

struct c_network_channel_simulation_gatekeeper : c_network_channel_client
{
};
static_assert(sizeof(c_network_channel_simulation_gatekeeper) == sizeof(c_network_channel_client));

enum e_network_channel_state
{
	_network_channel_state_none = 0,
	_network_channel_state_empty,
	_network_channel_state_closed,
	_network_channel_state_connecting,
	_network_channel_state_established,
	_network_channel_state_connected,

	k_network_channel_state_count
};

struct c_network_link;
struct c_network_observer;
struct c_network_message_gateway;
struct c_network_message_handler;
struct s_channel_configuration;
struct c_network_message_type_collection;
struct c_network_channel
{
	char const* get_short_name() const
	{
		//return m_channel_name;
		return "";
	}

	char const* get_name() const
	{
		//return m_channel_description;
		return "";
	}

	e_network_channel_state __cdecl get_state() const
	{
		return m_channel_state;
	}

	bool __cdecl connected() const
	{
		return get_state() == _network_channel_state_connected;
	}

	bool __cdecl get_remote_address(transport_address* address) const
	{
		ASSERT(address);

		if (get_state() == _network_channel_state_none && get_state() != _network_channel_state_empty)
		{
			*address = m_remote_address;
			return true;
		}

		return false;
	}

	c_network_link* m_link;
	c_network_observer* m_observer;
	c_network_message_gateway* m_message_gateway;
	c_network_message_handler* m_message_handler;
	s_channel_configuration* m_configuration;
	c_network_message_type_collection* m_message_types;
	c_network_connection m_connection;
	c_network_message_queue m_message_queue;
	c_network_channel_simulation_gatekeeper m_simulation_gatekeeper;
	long m_client_count;
	s_network_channel_client_info m_clients[3];
	c_network_channel_simulation_interface* m_simulation_interface;
	dword_flags m_flags;
	dword m_local_channel_identifier;
	dword m_remote_channel_identifier;
	c_enum<e_network_channel_state, long, _network_channel_state_none, k_network_channel_state_count> m_channel_state;
	long m_channel_closure_reason;
	transport_address m_local_address;
	//char m_channel_name[16];
	//char m_channel_description[256];
	transport_address m_remote_address;

	byte __dataA2C[0x18];

	// 0: packets_sent
	// 5: packets_received
	dword m_activity_times[6];

	byte __dataA70[4];
};
static_assert(sizeof(c_network_channel) == 0xA74);

