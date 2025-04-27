#pragma once

#include "networking/delivery/network_channel.hpp"

enum e_network_read_result;
struct c_bitstream;

struct s_replication_outgoing_request;
struct s_replication_incoming_request;
struct c_replication_scheduler_client
{
public:
	virtual bool has_data_to_transmit();
	virtual long build_outgoing_requests(void const* telemetry_data, long maximum_number_of_requests, s_replication_outgoing_request* requests);
	virtual long terminator_required_bits(void) const;
	virtual void write_to_packet(long request_identifier, long request_type, void const* telemetry_data, long packet_sequence_number, c_bitstream* packet, long must_leave_space_bits);
	virtual void write_terminator_to_packet(long packet_sequence_number, c_bitstream* packet);
	virtual e_network_read_result read_from_packet(long packet_sequence_number, c_bitstream* packet, long maximum_number_of_requests, s_replication_incoming_request* requests, long* out_number_of_requests);
	virtual void process_incoming_request(s_replication_incoming_request*);
	virtual void notify_packet_acknowledged(long outgoing_packet_sequence_number);
	virtual void notify_packet_retired(long outgoing_packet_sequence_number, bool delivered);

public:
	long m_client_index;
};
static_assert(sizeof(c_replication_scheduler_client) == 0x8);

struct c_replication_telemetry_provider;
struct c_replication_scheduler :
	public c_network_channel_client
{
	bool m_initialized;
	long m_view_index;

	// non-release builds only
	//char m_view_description[32];

	c_replication_scheduler_client* m_clients[3];
	long m_client_terminator_bits[3];
	long m_space_required_bits;
	c_replication_telemetry_provider* m_telemetry_provider;
};
static_assert(sizeof(c_replication_scheduler) == sizeof(c_network_channel_client) + 0x28);

