#pragma once

#include "cseries/cseries.hpp"
#include "networking/delivery/network_connection.hpp"
#include "networking/messages/network_message_queue.hpp"
#include "networking/transport/transport_address.hpp"
#include "shell/shell.hpp"

enum e_network_read_result;
struct c_bitstream;
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
	bool m_first_fragment_reserved;
	bool m_has_channel_been_used;
};
static_assert(sizeof(c_network_channel_client) == 0x8);

struct c_network_connection :
	public c_network_channel_client
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

	char const* m_name;

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

enum e_network_message_type;
struct c_network_message_type_collection;
struct c_network_message_queue :
	public c_network_channel_client
{
	struct s_incoming_outgoing_record;
	struct s_incoming_fragment_record;

public:
	void send_message(e_network_message_type message_type, long raw_message_size, void const* raw_message_payload);
	bool has_channel_been_used() const;

	char __data[4];

	char const* name;
	c_network_message_type_collection* m_message_types;

	c_sliding_window m_outgoing_window;
	s_incoming_fragment_record* m_outgoing_fragment_list_head;
	s_incoming_fragment_record* m_outgoing_fragment_list_tail;

	c_sliding_window m_incoming_window;
	s_incoming_fragment_record* m_incoming_fragment_list_head;
	s_incoming_fragment_record* m_incoming_fragment_list_tail;

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
public:
	void __cdecl notify_closed() const
	{
		ASSERT(m_initialized);
		ASSERT(m_simulation_context != NULL);
		ASSERT(m_simulation_closure_callback != NULL);

		m_simulation_closure_callback(m_simulation_context);
	}

protected:
	bool m_initialized;
	void* m_simulation_context;
	void(__cdecl* m_simulation_closure_callback)(void*);
	long m_client_count;
	s_network_channel_client_info m_clients[4];
	bool m_authority;
	bool m_established;
	byte __unknown32;
	byte __unknown33;
};
static_assert(sizeof(c_network_channel_simulation_interface) == 0x34);

struct c_network_channel_simulation_gatekeeper :
	public c_network_channel_client
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
public:
	static long const k_network_channel_name_size;

	char const* get_short_name() const;
	char const* get_name() const;
	static char const* __cdecl get_closure_reason_string(e_network_channel_closure_reason reason);
	static char const* __cdecl get_state_string(e_network_channel_state state);
	bool __cdecl get_remote_address(transport_address* address) const;
	dword __cdecl get_remote_identifier() const;
	dword __cdecl get_identifier() const;
	c_network_message_queue const* __cdecl network_message_queue_get() const;
	c_network_connection const* __cdecl network_connection_get() const;
	bool __cdecl allocated() const;
	e_network_channel_state __cdecl get_state() const;
	bool __cdecl closed() const;
	bool __cdecl established() const;
	bool __cdecl connected() const;
	void __cdecl close(e_network_channel_closure_reason reason);
	void __cdecl establish(dword remote_channel_identifier);
	void __cdecl open(transport_address const* remote_address, bool send_connect_packets, long channel_identifier);

	void send_message(e_network_message_type message_type, long raw_message_size, void const* raw_message_payload);

protected:
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
	c_enum<e_network_channel_closure_reason, long, _network_channel_reason_none, k_network_channel_reason_count> m_channel_closure_reason;
	transport_address m_local_address;
	//char m_channel_name[16];
	//char m_channel_description[256];
	transport_address m_remote_address;

	bool m_send_connect_packets;
	dword m_connect_identifier;
	dword m_connect_timestamp;
	dword m_connect_unknown;
	byte __dataA50[0x8];

	// 0: packets_sent
	// 5: packets_received
	dword m_activity_times[6];

	byte __dataA70[4];
};
static_assert(sizeof(c_network_channel) == 0xA74);

