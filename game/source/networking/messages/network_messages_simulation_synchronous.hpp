#pragma once

#include "cseries/cseries.hpp"
#include "simulation/simulation.hpp"

struct s_network_message_synchronous_update
{
	simulation_update update;
	s_simulation_update_metadata metadata;
	byte __data[0x4];
};
static_assert(sizeof(s_network_message_synchronous_update) == 0x1668);

struct s_network_message_synchronous_playback_control
{
	long type;
	dword identifier;
	long update_number;
};
static_assert(sizeof(s_network_message_synchronous_playback_control) == 0xC);

struct s_network_message_synchronous_actions
{
	long action_number;
	long current_action_number;
	dword_flags user_flags;
	long : 32;
	c_static_array<player_action, 4> actions;
};
static_assert(sizeof(s_network_message_synchronous_actions) == 0x210);

struct s_network_message_synchronous_acknowledge
{
	long current_update_number;
};
static_assert(sizeof(s_network_message_synchronous_acknowledge) == 0x4);

struct s_network_message_synchronous_gamestate
{
	byte message_type;
	dword chunk_offset_next_update_compressed_checksum;
	dword chunk_size;
	dword decompressed_checksum;
#pragma warning(push)
#pragma warning(disable : 4200)
	byte additional_data[];
#pragma warning(pop)
};
static_assert(sizeof(s_network_message_synchronous_gamestate) == 0x10);

//struct s_network_message_synchronous_client_ready
//{
//};
//static_assert(sizeof(s_network_message_synchronous_client_ready) == 0x0);

