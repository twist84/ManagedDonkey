#pragma once

#include "networking/delivery/network_channel.hpp"

enum e_network_read_result;
class c_bitstream;

struct s_replication_outgoing_request;
struct s_replication_incoming_request;
class c_replication_scheduler_client
{
public:
	virtual bool has_data_to_transmit();
	virtual int32 build_outgoing_requests(const void* telemetry_data, int32 maximum_number_of_requests, s_replication_outgoing_request* requests);
	virtual int32 terminator_required_bits(void) const;
	virtual void write_to_packet(int32 request_identifier, int32 request_type, const void* telemetry_data, int32 packet_sequence_number, c_bitstream* packet, int32 must_leave_space_bits);
	virtual void write_terminator_to_packet(int32 packet_sequence_number, c_bitstream* packet);
	virtual e_network_read_result read_from_packet(int32 packet_sequence_number, c_bitstream* packet, int32 maximum_number_of_requests, s_replication_incoming_request* requests, int32* out_number_of_requests);
	virtual void process_incoming_request(s_replication_incoming_request*);
	virtual void notify_packet_acknowledged(int32 outgoing_packet_sequence_number);
	virtual void notify_packet_retired(int32 outgoing_packet_sequence_number, bool delivered);

public:
	int32 m_client_index;
};
COMPILE_ASSERT(sizeof(c_replication_scheduler_client) == 0x8);

class c_replication_telemetry_provider;
class c_replication_scheduler :
	public c_network_channel_client
{
public:
	bool m_initialized;
	int32 m_view_index;

	// non-release builds only
	//char m_view_description[32];

	c_replication_scheduler_client* m_clients[3];
	int32 m_client_terminator_bits[3];
	int32 m_space_required_bits;
	c_replication_telemetry_provider* m_telemetry_provider;
};
COMPILE_ASSERT(sizeof(c_replication_scheduler) == sizeof(c_network_channel_client) + 0x28);

