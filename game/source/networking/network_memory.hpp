#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_parameter_type_collection.hpp"

struct s_network_base_memory_globals
{
	//c_network_link link;
	byte link[0x378];

	//c_network_message_type_collection message_types;
	byte message_types[0x57C];
	dword : 32;

	//c_network_message_gateway message_gateway;
	byte message_gateway[0x688];

	//c_network_message_handler message_handler;
	byte message_handler[0x18];

	//c_network_observer observer;
	byte observer[0x23F20];

	//c_network_session_manager session_manager;
	byte session_manager[0xC];
	dword : 32;

	//c_network_session sessions[3];
	byte sessions[0x25BC40 * 3];

	c_network_session_parameter_type_collection sesssion_parameter_types;

	//c_simulation_world simulation_world;
	byte simulation_world[0x1540];

	//c_simulation_watcher simulation_watcher;
	byte simulation_watcher[0x16658];

	//c_simulation_type_collection simulation_types;
	byte simulation_types[0x188];
};
static_assert(sizeof(s_network_base_memory_globals) == 0x7506C8);

extern s_network_base_memory_globals& network_base_memory_globals;