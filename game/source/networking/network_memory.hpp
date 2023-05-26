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

enum e_network_message_type;
struct c_network_message_gateway : c_network_out_of_band_consumer
{
public:
	static bool __fastcall _receive_out_of_band_packet(c_network_message_gateway* _this, void* unused, transport_address const* incoming_address, c_bitstream* packet);

	bool __cdecl send_message_broadcast(e_network_message_type message_type, long data_size, void const* data, word port);
	bool __cdecl send_message_directed(transport_address const* outgoing_address, e_network_message_type message_type, long data_size, void const* data);

	bool __cdecl receive_out_of_band_packet_(transport_address const* incoming_address, c_bitstream* packet);

private:
	static bool __cdecl read_packet_header(c_bitstream* packet);

protected:
	bool m_initialized;
	c_network_link* m_link;
	c_network_message_type_collection* m_message_types;
	c_network_message_handler* m_message_handler;
	bool m_outgoing_packet_pending;
	byte m_outgoing_packet_storage[0x5BE + 1];
	transport_address m_outgoing_packet_address;
	c_bitstream m_outgoing_packet;
};
static_assert(sizeof(c_network_message_gateway) == 0x688);

struct s_data_array;
struct c_rockall_heap;
struct s_network_shared_memory_globals
{
	bool initialized;
	long allocation_size;
	void* allocation_low_address;
	void* allocation_high_address;
	bool distributed_simulation_available;
	c_simulation_distributed_world* simulation_distributed_world;
	s_data_array* simulation_view_data_array;
	s_data_array* simulation_distributed_view_data_array;
	long heap_size;
	c_rockall_heap* heap;
	void* heap_buffer;
	dword : 32;

	// not in release builds
	//s_network_memory_status total_status;
	//s_network_memory_status block_status[21];
};
static_assert(sizeof(s_network_shared_memory_globals) == 0x30);

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

word const k_broadcast_port = 11774; // port 1001 outside of Halo Online
word const k_broadcast_port_alt_ammount = 1000; // needs a better name

extern s_network_shared_memory_globals& network_shared_memory_globals;
extern s_network_base_memory_globals& network_base_memory_globals;
extern word& g_broadcast_port;

