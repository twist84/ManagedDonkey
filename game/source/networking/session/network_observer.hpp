#pragma once

#include "cseries/cseries.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/logic/network_bandwidth.hpp"
#include "networking/transport/transport_security.hpp"
#include "networking/transport/transport_qos.hpp"
#include "networking/network_statistics.hpp"

long const k_network_maximum_observers = 34;

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
struct s_network_message_connect_request;
enum e_network_message_type;
struct c_network_observer
{
	struct s_channel_observer
	{
		c_network_channel channel;
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

		long __timeAF8;
		long __timeAFC;
		long __timeB00;
		byte __dataB04[0x4];

		c_static_array<c_network_time_statistics, 3> time_statistics;
		c_static_array<c_network_window_statistics, 2> window_statistics;

		byte __dataFB0[0x14];

		long bandwidth_event_counters[5];

		dword __timeFD8;
		byte __data[0xFC];
	};
	static_assert(sizeof(s_channel_observer) == 0x10D8);

	s_channel_observer const* find_observer_by_channel(c_network_channel const* channel) const;
	void destroy_observer();
	void handle_connect_request(transport_address const* incoming_address, s_network_message_connect_request const* connect_request);
	bool initialize_observer(c_network_link* link, c_network_message_type_collection* message_types, c_network_message_gateway* message_gateway, c_network_message_handler* message_handler, s_observer_configuration const* configuration);
	void monitor();
	long observer_channel_find_by_network_channel(long owner_type, c_network_channel* channel) const;
	void observer_channel_send_message(long owner_type, long observer_index, bool a3, e_network_message_type message_type, long data_size, void const* data);
	void observer_prioritize_upload_bandwidth(bool prioritize_upload_bandwidth);
	void set_online_network_environment(bool online_network_environment);

	c_network_link* m_link;
	c_network_message_gateway* m_message_gateway;
	c_network_message_handler* m_message_handler;
	c_network_message_type_collection* m_message_types;
	s_observer_configuration* m_configuration;
	s_channel_observer_owner m_owners[4];
	c_network_session* m_session;
	s_channel_observer m_channel_observers[k_network_maximum_observers];

	// All data beyond this point is related to bandwidth

	bool m_quality_statistics_are_set;
	long __unknown23CEC;
	s_network_quality_statistics m_quality_statistics;

	// some enum type used in a switch within `c_network_observer::stream_update`
	// c_network_observer::is_bandwidth_stable: `__unknown23D80 == 0`
	long __unknown23D80;

	long __unknown23DB4;
	bool m_prioritize_upload_bandwidth;
	bool m_online_network_environment;
	bool __unknown23DBA;
	bool __unknown23DBB;
	long __unknown23DBC;
	long __unknown23DC0;
	long __unknown23DC4;
	long __time23DC8;

	// bool? enum?
	// 0: UNSATISFIED
	// 1: SATIATED 
	byte __unknown23DD0_index;
	byte __data23DCD[0x3];
	long __unknown23DD0[2];

	long __unknown23DD8;
	long __unknown23DDC;
	long __unknown23DE0;
	long __time23DE4;
	long __time23DE8;
	long __time23DEC;
	long __time23DF0;
	long __time23DF4;
	long __unknown23DF8;
	long __unknown23DFC;
	long __unknown23E00;
	byte __data23E04[0x4];
	c_network_time_statistics time_statistics;
	long __unknown23EE0;
	long __unknown23EE4;
	long __unknown23EE8;
	long __unknown23EEC;
	long __unknown23EF0;
	long __unknown23EF4;
	long __time23EF8;
	long __time23EFC;
	bool __unknown23F00;
	bool __unknown23F01;
	bool __unknown23F02;
	byte __unknown23F03; // bool?
	bool __unknown23F04;
	byte __data23F05[0xB];
	long __time23F10;
	bool __unknown23F14;
	byte __data23F15[0x3];
	long __unknown23F18;
	byte __data23F1C[0x4];
};
static_assert(sizeof(c_network_observer) == 0x23F20);
static_assert(0x14 == OFFSETOF(c_network_observer, m_owners));

