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
};
static_assert(sizeof(c_network_channel_client) == sizeof(void*));

struct c_network_connection :
	public c_network_channel_client
{
public:
	struct s_connection_incoming_packet
	{
		uint16 flags;
	};
	static_assert(sizeof(s_connection_incoming_packet) == 0x2);

	struct s_connection_outgoing_packet
	{
		uint32 timestamp;
		long size_on_wire;
		uint32 round_trip_msec;
		short unretired_window_size;
		uint16 flags;
	};
	static_assert(sizeof(s_connection_outgoing_packet) == 0x10);

//private:
	bool m_allocated;
	bool m_connection_lost;
	char const* m_channel_name;
	c_sliding_window m_incoming_window;
	c_sliding_window m_outgoing_window;
	long m_incoming_window_size_bytes;
	long m_outgoing_window_size_bytes;
	c_static_array<s_connection_incoming_packet, 128> m_incoming_packets;
	c_static_array<s_connection_outgoing_packet, 128> m_outgoing_packets;
	long m_highest_outgoing_acknowledged_sequence_number;
	long m_highest_incoming_acknowledged_sequence_number;
	long m_highest_incoming_known_retired_sequence_number;
	bool m_incoming_window_fully_retired;
	bool m_outgoing_window_fully_retired;
	long m_current_timeout_msec;
};
static_assert(sizeof(c_network_connection) == 0x960);

enum e_network_message_type;
struct c_network_message_type_collection;
struct c_network_message_queue :
	public c_network_channel_client
{
public:
	struct s_outgoing_message_description
	{
		e_network_message_type message_type;
		uint16 message_number;
		uint16 message_raw_size_bytes;
		uint16 message_encoded_size_bits;
		uint32 timestamp;
	};
	static_assert(sizeof(s_outgoing_message_description) == 0x10);

	struct s_outgoing_fragment_record
	{
		uint8 flags;
		uint8 fragment_size_bytes;
		uint16 fragment_size_bits;
		long packet_sequence_number;
		s_outgoing_fragment_record* next_fragment;
		s_outgoing_message_description message_description;
		__pragma(warning(disable : 4200)) uint8 payload[];
	};
	static_assert(sizeof(s_outgoing_fragment_record) == 0x1C);

	struct s_incoming_fragment_record
	{
		uint8 flags;
		uint8 fragment_size_bytes;
		uint16 fragment_size_bits;
		long sequence_number;
		s_incoming_fragment_record* next_fragment;
		__pragma(warning(disable : 4200)) uint8 payload[];
	};
	static_assert(sizeof(s_incoming_fragment_record) == 0xC);

public:
	void send_message(e_network_message_type message_type, long raw_message_size, void const* raw_message_payload);
	bool has_channel_been_used() const;

	bool m_allocated;
	bool m_connection_lost;
	bool m_first_fragment_reserved;
	bool m_second_fragment_sent;
	bool m_establishment_received;
	char const* m_channel_name;

	c_network_message_type_collection const* m_message_types;

	c_sliding_window m_outgoing_window;
	s_outgoing_fragment_record* m_outgoing_fragment_list_head;
	s_outgoing_fragment_record* m_outgoing_fragment_list_tail;

	c_sliding_window m_incoming_window;
	s_incoming_fragment_record* m_incoming_fragment_list_head;
	s_incoming_fragment_record* m_incoming_fragment_list_tail;

	long m_outgoing_payload_bytes;
	long m_incoming_payload_bytes;
};
static_assert(sizeof(c_network_message_queue) == 0x64);

struct s_network_channel_client_info
{
	uint32 flags;
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
	bool m_simulation_is_authority;
	bool m_established;
};
static_assert(sizeof(c_network_channel_simulation_interface) == 0x34);

struct c_network_channel_simulation_gatekeeper :
	public c_network_channel_client
{
	bool m_write_simulation_data_available;
	bool m_read_simulation_data_expected;
};
static_assert(sizeof(c_network_channel_simulation_gatekeeper) == 0x8);

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

	struct s_activity_timer
	{
		uint32 timestamp;
	};
	static_assert(sizeof(s_activity_timer) == 0x4);

	char const* get_short_name() const;
	char const* get_name() const;
	static char const* __cdecl get_closure_reason_string(e_network_channel_closure_reason reason);
	static char const* __cdecl get_state_string(e_network_channel_state state);
	bool get_remote_address(transport_address* address) const;
	uint32 get_remote_identifier() const;
	uint32 get_identifier() const;
	c_network_message_queue const* network_message_queue_get() const;
	c_network_connection const* network_connection_get() const;
	bool allocated() const;
	e_network_channel_state get_state() const;
	bool closed() const;
	bool established() const;
	bool connected() const;
	void close(e_network_channel_closure_reason reason);
	void establish(uint32 remote_channel_identifier);
	void open(transport_address const* remote_address, bool send_connect_packets, long channel_identifier);

	void send_message(e_network_message_type message_type, long raw_message_size, void const* raw_message_payload);

//protected:
	c_network_link* m_link;
	c_network_observer* m_observer;
	c_network_message_gateway* m_message_gateway;
	c_network_message_handler* m_message_handler;
	s_channel_configuration* m_configuration;
	c_network_message_type_collection* m_type_collection;
	c_network_connection m_connection;
	c_network_message_queue m_message_queue;
	c_network_channel_simulation_gatekeeper m_simulation_gatekeeper;
	long m_client_count;
	s_network_channel_client_info m_clients[3];
	c_network_channel_simulation_interface* m_simulation_interface;
	uint32 m_channel_flags;
	uint32 m_channel_identifier;
	uint32 m_remote_channel_identifier;
	c_enum<e_network_channel_state, long, _network_channel_state_none, k_network_channel_state_count> m_channel_state;
	c_enum<e_network_channel_closure_reason, long, _network_channel_reason_none, k_network_channel_reason_count> m_channel_closure_reason;
	transport_address m_channel_closure_address;
	transport_address m_remote_address;
	bool m_send_connect_packets;
	uint32 m_first_connect_packet_timestamp;
	uint32 m_next_connect_packet_timestamp;
	long m_sent_connect_packet_count;
	uint32 m_established_timestamp;
	uint32 m_connected_timestamp;
	s_activity_timer m_activity_times[6];
	bool m_destination_unreachable;
};
static_assert(sizeof(c_network_channel) == 0xA74);
static_assert(0x000 == OFFSETOF(c_network_channel, m_link));
static_assert(0x004 == OFFSETOF(c_network_channel, m_observer));
static_assert(0x008 == OFFSETOF(c_network_channel, m_message_gateway));
static_assert(0x00C == OFFSETOF(c_network_channel, m_message_handler));
static_assert(0x010 == OFFSETOF(c_network_channel, m_configuration));
static_assert(0x014 == OFFSETOF(c_network_channel, m_type_collection));
static_assert(0x018 == OFFSETOF(c_network_channel, m_connection));
static_assert(0x978 == OFFSETOF(c_network_channel, m_message_queue));
static_assert(0x9DC == OFFSETOF(c_network_channel, m_simulation_gatekeeper));
static_assert(0x9E4 == OFFSETOF(c_network_channel, m_client_count));
static_assert(0x9E8 == OFFSETOF(c_network_channel, m_clients));
static_assert(0xA00 == OFFSETOF(c_network_channel, m_simulation_interface));
static_assert(0xA04 == OFFSETOF(c_network_channel, m_channel_flags));
static_assert(0xA08 == OFFSETOF(c_network_channel, m_channel_identifier));
static_assert(0xA0C == OFFSETOF(c_network_channel, m_remote_channel_identifier));
static_assert(0xA10 == OFFSETOF(c_network_channel, m_channel_state));
static_assert(0xA14 == OFFSETOF(c_network_channel, m_channel_closure_reason));
static_assert(0xA18 == OFFSETOF(c_network_channel, m_channel_closure_address));
static_assert(0xA2C == OFFSETOF(c_network_channel, m_remote_address));
static_assert(0xA40 == OFFSETOF(c_network_channel, m_send_connect_packets));
static_assert(0xA44 == OFFSETOF(c_network_channel, m_first_connect_packet_timestamp));
static_assert(0xA48 == OFFSETOF(c_network_channel, m_next_connect_packet_timestamp));
static_assert(0xA4C == OFFSETOF(c_network_channel, m_sent_connect_packet_count));
static_assert(0xA50 == OFFSETOF(c_network_channel, m_established_timestamp));
static_assert(0xA54 == OFFSETOF(c_network_channel, m_connected_timestamp));
static_assert(0xA58 == OFFSETOF(c_network_channel, m_activity_times));
static_assert(0xA70 == OFFSETOF(c_network_channel, m_destination_unreachable));

