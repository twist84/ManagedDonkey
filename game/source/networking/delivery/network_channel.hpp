#pragma once

#include "cseries/cseries.hpp"
#include "networking/delivery/network_connection.hpp"
#include "networking/messages/network_message_queue.hpp"
#include "networking/transport/transport_address.hpp"

enum e_network_read_result;
class c_bitstream;
class c_network_message_type_collection;
class c_network_link;
class c_network_observer;
class c_network_message_gateway;
class c_network_message_handler;
class c_network_message_type_collection;
struct s_channel_configuration;

enum e_network_channel_activity
{
	_network_channel_activity_send = 0,
	_network_channel_activity_send_sequenced,
	_network_channel_activity_send_voice,
	_network_channel_activity_receive,
	_network_channel_activity_receive_acknowledgement,
	_network_channel_activity_receive_voice,

	k_network_channel_activity_count,
};

class c_network_channel_client
{
public:
	virtual const char* get_client_name() const;
	virtual bool connection_lost(e_network_channel_closure_reason*) const;
	virtual bool has_data_to_transmit(bool*) const;
	virtual int32 space_required_bits(int32, int32);
	virtual bool write_to_packet(int32, c_bitstream*, int32, int32);
	virtual e_network_read_result read_from_packet(int32*, c_bitstream*);
	virtual void notify_packet_acknowledged(int32);
	virtual void notify_packet_retired(int32, bool);
};
COMPILE_ASSERT(sizeof(c_network_channel_client) == sizeof(void*));

class c_network_connection :
	public c_network_channel_client
{
public:
	struct s_connection_incoming_packet
	{
		uns16 flags;
	};
	COMPILE_ASSERT(sizeof(s_connection_incoming_packet) == 0x2);

	struct s_connection_outgoing_packet
	{
		uns32 timestamp;
		int32 size_on_wire;
		uns32 round_trip_msec;
		int16 unretired_window_size;
		uns16 flags;
	};
	COMPILE_ASSERT(sizeof(s_connection_outgoing_packet) == 0x10);

//private:
	bool m_allocated;
	bool m_connection_lost;
	const char* m_channel_name;
	c_sliding_window m_incoming_window;
	c_sliding_window m_outgoing_window;
	int32 m_incoming_window_size_bytes;
	int32 m_outgoing_window_size_bytes;
	c_static_array<s_connection_incoming_packet, 128> m_incoming_packets;
	c_static_array<s_connection_outgoing_packet, 128> m_outgoing_packets;
	int32 m_highest_outgoing_acknowledged_sequence_number;
	int32 m_highest_incoming_acknowledged_sequence_number;
	int32 m_highest_incoming_known_retired_sequence_number;
	bool m_incoming_window_fully_retired;
	bool m_outgoing_window_fully_retired;
	int32 m_current_timeout_msec;
};
COMPILE_ASSERT(sizeof(c_network_connection) == 0x960);

class c_network_message_queue :
	public c_network_channel_client
{
public:
	struct s_outgoing_message_description
	{
		e_network_message_type message_type;
		uns16 message_number;
		uns16 message_raw_size_bytes;
		uns16 message_encoded_size_bits;
		uns32 timestamp;
	};
	COMPILE_ASSERT(sizeof(s_outgoing_message_description) == 0x10);

	struct s_outgoing_fragment_record
	{
		uns8 flags;
		uns8 fragment_size_bytes;
		uns16 fragment_size_bits;
		int32 packet_sequence_number;
		s_outgoing_fragment_record* next_fragment;
		s_outgoing_message_description message_description;
		__pragma(warning(disable : 4200)) byte payload[];
	};
	COMPILE_ASSERT(sizeof(s_outgoing_fragment_record) == 0x1C);

	struct s_incoming_fragment_record
	{
		uns8 flags;
		uns8 fragment_size_bytes;
		uns16 fragment_size_bits;
		int32 sequence_number;
		s_incoming_fragment_record* next_fragment;
		__pragma(warning(disable : 4200)) byte payload[];
	};
	COMPILE_ASSERT(sizeof(s_incoming_fragment_record) == 0xC);

public:
	bool has_channel_been_used() const;
	void send_message(e_network_message_type message_type, int32 raw_message_size, const void* raw_message_payload);

	bool m_allocated;
	bool m_connection_lost;
	bool m_first_fragment_reserved;
	bool m_second_fragment_sent;
	bool m_establishment_received;
	const char* m_channel_name;

	const c_network_message_type_collection* m_message_types;

	c_sliding_window m_outgoing_window;
	s_outgoing_fragment_record* m_outgoing_fragment_list_head;
	s_outgoing_fragment_record* m_outgoing_fragment_list_tail;

	c_sliding_window m_incoming_window;
	s_incoming_fragment_record* m_incoming_fragment_list_head;
	s_incoming_fragment_record* m_incoming_fragment_list_tail;

	int32 m_outgoing_payload_bytes;
	int32 m_incoming_payload_bytes;
};
COMPILE_ASSERT(sizeof(c_network_message_queue) == 0x64);

struct s_network_channel_client_info
{
	uns32 flags;
	c_network_channel_client* client;
};
COMPILE_ASSERT(sizeof(s_network_channel_client_info) == 0x8);

class c_network_channel_simulation_interface
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
	int32 m_client_count;
	s_network_channel_client_info m_clients[4];
	bool m_simulation_is_authority;
	bool m_established;
};
COMPILE_ASSERT(sizeof(c_network_channel_simulation_interface) == 0x34);

class c_network_channel_simulation_gatekeeper :
	public c_network_channel_client
{
public:
	bool m_write_simulation_data_available;
	bool m_read_simulation_data_expected;
};
COMPILE_ASSERT(sizeof(c_network_channel_simulation_gatekeeper) == 0x8);

class c_network_channel
{
public:
	static int32 const k_network_channel_name_size;

	struct s_activity_timer
	{
		uns32 timestamp;
	};
	COMPILE_ASSERT(sizeof(s_activity_timer) == 0x4);

public:
	const char* get_short_name() const;
	const char* get_name() const;
	static const char* __cdecl get_closure_reason_string(e_network_channel_closure_reason reason);
	static const char* __cdecl get_state_string(e_network_channel_state state);
	bool get_remote_address(transport_address* address) const;
	uns32 get_remote_identifier() const;
	uns32 get_identifier() const;
	bool is_channel_remote_address(const transport_address* address) const;
	const c_network_message_queue* network_message_queue_get() const;
	const c_network_connection* network_connection_get() const;
	bool allocated() const;
	e_network_channel_state get_state() const;
	bool closed() const;
	bool established() const;
	bool connected() const;
	void close(e_network_channel_closure_reason reason);
	void establish(uns32 remote_channel_identifier);
	bool idle();
	void notify_destination_unreachable();
	void open(const transport_address* remote_address, bool send_connect_packets, int32 channel_identifier);
	bool receive_packet(c_bitstream* packet, int32 packet_size_on_wire, bool consistency_check_only);
	void send_message(e_network_message_type message_type, int32 raw_message_size, const void* raw_message_payload);

private:
	void send_packet(c_bitstream* packet, bool sequenced_packet, bool force_fill_packet, int32 voice_data_size, const void* voice_data, int32* out_bytes_on_wire, int32* out_bytes_game_data, int32* out_bytes_voice_data);

public:
	void update_activity(e_network_channel_activity activity_type);

private:
	void update_packet_delivery();

public:
//private:
	c_network_link* m_link;
	c_network_observer* m_observer;
	c_network_message_gateway* m_message_gateway;
	c_network_message_handler* m_message_handler;
	s_channel_configuration* m_configuration;
	c_network_message_type_collection* m_type_collection;
	c_network_connection m_connection;
	c_network_message_queue m_message_queue;
	c_network_channel_simulation_gatekeeper m_simulation_gatekeeper;
	int32 m_client_count;
	s_network_channel_client_info m_clients[3];
	c_network_channel_simulation_interface* m_simulation_interface;
	uns32 m_channel_flags;
	uns32 m_channel_identifier;
	uns32 m_remote_channel_identifier;
	c_enum<e_network_channel_state, int32, _network_channel_state_none, k_network_channel_state_count> m_channel_state;
	c_enum<e_network_channel_closure_reason, int32, _network_channel_reason_none, k_network_channel_reason_count> m_channel_closure_reason;
	transport_address m_channel_closure_address;
	transport_address m_remote_address;
	bool m_send_connect_packets;
	uns32 m_first_connect_packet_timestamp;
	uns32 m_next_connect_packet_timestamp;
	int32 m_sent_connect_packet_count;
	uns32 m_established_timestamp;
	uns32 m_connected_timestamp;
	s_activity_timer m_activity_timers[k_network_channel_activity_count];
	bool m_destination_unreachable;
};
COMPILE_ASSERT(sizeof(c_network_channel) == 0xA74);
COMPILE_ASSERT(0x000 == OFFSETOF(c_network_channel, m_link));
COMPILE_ASSERT(0x004 == OFFSETOF(c_network_channel, m_observer));
COMPILE_ASSERT(0x008 == OFFSETOF(c_network_channel, m_message_gateway));
COMPILE_ASSERT(0x00C == OFFSETOF(c_network_channel, m_message_handler));
COMPILE_ASSERT(0x010 == OFFSETOF(c_network_channel, m_configuration));
COMPILE_ASSERT(0x014 == OFFSETOF(c_network_channel, m_type_collection));
COMPILE_ASSERT(0x018 == OFFSETOF(c_network_channel, m_connection));
COMPILE_ASSERT(0x978 == OFFSETOF(c_network_channel, m_message_queue));
COMPILE_ASSERT(0x9DC == OFFSETOF(c_network_channel, m_simulation_gatekeeper));
COMPILE_ASSERT(0x9E4 == OFFSETOF(c_network_channel, m_client_count));
COMPILE_ASSERT(0x9E8 == OFFSETOF(c_network_channel, m_clients));
COMPILE_ASSERT(0xA00 == OFFSETOF(c_network_channel, m_simulation_interface));
COMPILE_ASSERT(0xA04 == OFFSETOF(c_network_channel, m_channel_flags));
COMPILE_ASSERT(0xA08 == OFFSETOF(c_network_channel, m_channel_identifier));
COMPILE_ASSERT(0xA0C == OFFSETOF(c_network_channel, m_remote_channel_identifier));
COMPILE_ASSERT(0xA10 == OFFSETOF(c_network_channel, m_channel_state));
COMPILE_ASSERT(0xA14 == OFFSETOF(c_network_channel, m_channel_closure_reason));
COMPILE_ASSERT(0xA18 == OFFSETOF(c_network_channel, m_channel_closure_address));
COMPILE_ASSERT(0xA2C == OFFSETOF(c_network_channel, m_remote_address));
COMPILE_ASSERT(0xA40 == OFFSETOF(c_network_channel, m_send_connect_packets));
COMPILE_ASSERT(0xA44 == OFFSETOF(c_network_channel, m_first_connect_packet_timestamp));
COMPILE_ASSERT(0xA48 == OFFSETOF(c_network_channel, m_next_connect_packet_timestamp));
COMPILE_ASSERT(0xA4C == OFFSETOF(c_network_channel, m_sent_connect_packet_count));
COMPILE_ASSERT(0xA50 == OFFSETOF(c_network_channel, m_established_timestamp));
COMPILE_ASSERT(0xA54 == OFFSETOF(c_network_channel, m_connected_timestamp));
COMPILE_ASSERT(0xA58 == OFFSETOF(c_network_channel, m_activity_timers));
COMPILE_ASSERT(0xA70 == OFFSETOF(c_network_channel, m_destination_unreachable));

