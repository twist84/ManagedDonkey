#pragma once

#include "cseries/cseries.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/logic/network_bandwidth.hpp"
#include "networking/transport/transport_security.hpp"
#include "networking/transport/transport_qos.hpp"

struct c_network_session;
struct s_channel_observer_owner
{
	c_network_session* owner;
	long managed_session_index;
};
static_assert(sizeof(s_channel_observer_owner) == 0x8);

struct c_network_link;
struct c_network_message_gateway;
struct c_network_message_handler;
struct c_network_message_type_collection;
struct s_observer_configuration;
struct c_network_observer
{
	struct s_channel_observer : c_network_channel
	{
		long state;

		dword __unknownA78;

		byte_flags flags;
		byte_flags owner_flags;

		word __unknownA7E;

		dword owner_connection_identifier;
		s_transport_secure_address secure_address;
		dword_flags __unknownA94_flags;
		dword __unknownA94_flags_bit;
		transport_address secure_connection_address;
		s_transport_secure_identifier secure_connection_identifier;
		byte __dataAC0[0x8];

		bool observer_is_bad_client;
		byte __dataAC9[0x3];

		dword estimated_upstream_bandwidth_bps;
		dword estimated_downstream_bandwidth_bps;
		dword qos_attempt_index;
		s_transport_qos_result qos_attempt_result;

		dword __timeAF8;
		byte __dataAFC[0xC];

		c_static_array<c_network_time_statistics, 3> time_statistics;
		c_static_array<c_network_window_statistics, 2> window_statistics;

		byte __dataFB0[0x28];
		dword __timeFD8;
		byte __data[0xFC];
	};
	static_assert(sizeof(s_channel_observer) == 0x10D8);

	c_network_link* m_link;
	c_network_message_gateway* m_message_gateway;
	c_network_message_handler* m_message_handler;
	c_network_message_type_collection* m_message_types;
	s_observer_configuration* m_configuration;
	s_channel_observer_owner m_owners[4];
	byte __data34[4];
	s_channel_observer m_channel_observers[34];
	bool m_quality_statistics_are_set;
	int __unknown23CEC;
	s_network_quality_statistics m_quality_statistics;
	byte __data23DB0[0x9];
	bool m_online_network_environment;
	byte __data[0x166];
};
static_assert(sizeof(c_network_observer) == 0x23F20);
