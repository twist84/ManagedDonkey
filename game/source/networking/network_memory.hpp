#pragma once

#include "cseries/cseries.hpp"

#include "networking/delivery/network_link.hpp"
#include "networking/messages/network_message_handler.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/session/network_observer.hpp"
#include "networking/session/network_session.hpp"
#include "networking/session/network_session_manager.hpp"
#include "networking/session/network_session_parameter_type_collection.hpp"
#include "networking/transport/transport_address.hpp"
#include "simulation/simulation_type_collection.hpp"
#include "simulation/simulation_watcher.hpp"
#include "simulation/simulation_world.hpp"

struct c_network_out_of_band_consumer
{
public:
	virtual bool __cdecl receive_out_of_band_packet(transport_address const* address, c_bitstream* packet);
};
static_assert(sizeof(c_network_out_of_band_consumer) == 0x4);

struct c_network_message_gateway : c_network_out_of_band_consumer
{
	bool m_initialized;
	c_network_link* m_link;
	c_network_message_type_collection* m_message_types;
	c_network_message_handler* m_message_handler;
	byte __data14[0x1];
	byte m_stream_data[0x5BE];
	byte __data5D4[0x15];
	c_bitstream m_stream;
};
static_assert(sizeof(c_network_message_gateway) == 0x688);

struct s_network_base_memory_globals
{
	c_network_link link;
	c_network_message_type_collection message_types;
	dword : 32;
	c_network_message_gateway message_gateway;
	c_network_message_handler message_handler;
	c_network_observer observer;
	c_network_session_manager session_manager;
	dword : 32;
	c_network_session sessions[3];
	c_network_session_parameter_type_collection sesssion_parameter_types;
	c_simulation_world simulation_world;
	c_simulation_watcher simulation_watcher;
	c_simulation_type_collection simulation_types;
};
static_assert(sizeof(s_network_base_memory_globals) == 0x7506C8);

extern s_network_base_memory_globals& network_base_memory_globals;