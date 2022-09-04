#pragma once

#include "memory/bitstream.hpp"

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
	char __unknownA;
	char __unknownB;
};
static_assert(sizeof(c_network_channel_client) == 0x8);

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
	bool __unknown30;
	bool __unknown31;
	char __pad32[2];
};
static_assert(sizeof(c_network_channel_simulation_interface) == 0x34);

struct c_network_channel;