#pragma once

#include "cseries/cseries.hpp"

#include "networking/delivery/network_link.hpp"
#include "networking/messages/network_message_gateway.hpp"
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

const uns16 k_broadcast_port = 11774; // port 1003 outside of Halo Online

// the maximum number of ports to try when the broadcast port is already in use
const uns16 k_broadcast_port_alt_ammount = 1000; // needs a better name

class c_fixed_memory_rockall_heap;
struct s_data_array;

struct s_network_shared_memory_globals
{
	int32 current_configuration;
	int32 physical_memory_size;
	void* physical_memory_low_address;
	void* physical_memory_high_address;
	bool distributed_simulation_available;
	c_simulation_distributed_world* simulation_distributed_world;
	s_data_array* simulation_view_data_array;
	s_data_array* simulation_distributed_view_data_array;
	int32 heap_size;
	c_fixed_memory_rockall_heap* heap;
	void* heap_buffer;
	uns32 : 32;

	// non-release builds only
	//s_network_memory_status total_status;
	//s_network_memory_status block_status[21];
};
static_assert(sizeof(s_network_shared_memory_globals) == 0x30);

struct s_network_base_memory_globals
{
	c_network_link link; // byte link[0x378];
	byte message_types[0x57C]; //c_network_message_type_collection message_types;
	c_network_message_gateway message_gateway;
	c_network_message_handler message_handler;
	c_network_observer observer;
	c_network_session_manager session_manager;
	c_network_session sessions[3];
	c_network_session_parameter_type_collection sesssion_parameter_types;
	c_simulation_world simulation_world;
	c_simulation_watcher simulation_watcher;
	c_simulation_type_collection simulation_types;
};
static_assert(sizeof(s_network_base_memory_globals) == 0x7506C8);

extern s_network_shared_memory_globals& network_shared_memory_globals;
extern s_network_base_memory_globals& network_base_memory_globals;
extern uns16& g_broadcast_port;

extern c_simulation_distributed_world* __cdecl network_allocate_simulation_distributed_world();
extern c_network_channel* __cdecl network_channel_iterate(const c_network_channel* channel);
extern void* __cdecl network_heap_allocate_block(int32 block_size);
extern char* __cdecl network_heap_describe(char* buffer, int32 buffer_size);
extern void __cdecl network_heap_free_block(void* block);
extern void __cdecl network_memory_shared_dispose();
extern void __cdecl network_memory_shared_initialize(e_map_memory_configuration memory_configuration);
extern bool __cdecl network_memory_simulation_initialize(c_simulation_world** world, c_simulation_watcher** watcher, c_simulation_type_collection** type_collection);

