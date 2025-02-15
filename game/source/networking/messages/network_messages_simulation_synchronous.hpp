#pragma once

#include "cseries/cseries.hpp"
#include "simulation/simulation.hpp"

enum
{
	_network_message_type_variable_length_bit = 0,
	_network_message_type_message_without_body_bit,

	k_network_message_type_flags_count
};

struct s_network_message_synchronous_update
{
	struct simulation_update update;
	s_simulation_update_metadata metadata;
};
static_assert(sizeof(s_network_message_synchronous_update) == 0x1668);
static_assert(0x0000 == OFFSETOF(s_network_message_synchronous_update, update));
static_assert(0x1658 == OFFSETOF(s_network_message_synchronous_update, metadata));

enum e_network_synchronous_playback_control
{
	_network_synchronous_playback_control_revert = 0,
	_network_synchronous_playback_control_end_playback,

	k_network_synchronous_playback_control_count
};

struct s_network_message_synchronous_playback_control
{
	e_network_synchronous_playback_control type;
	long identifier;
	long update_number;
};
static_assert(sizeof(s_network_message_synchronous_playback_control) == 0xC);

struct s_network_message_synchronous_actions
{
	long action_number;
	long current_update_number;
	dword valid_user_mask;
	player_action user_actions[4];
};
static_assert(sizeof(s_network_message_synchronous_actions) == 0x210);

struct s_network_message_synchronous_acknowledge
{
	long current_update_number;
};
static_assert(sizeof(s_network_message_synchronous_acknowledge) == 0x4);

struct c_bitstream;

class c_network_message_synchronous_update
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
	static bool __cdecl compare(long, void*, void*);
	static void __cdecl dispose(long, void*);
};

class c_network_message_synchronous_playback_control
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_synchronous_actions
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
	static bool __cdecl compare(long, void*, void*);
	static void __cdecl dispose(long, void*);
};

class c_network_message_synchronous_acknowledge
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_synchronous_gamestate
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

struct c_network_message_type_collection;
extern void __cdecl network_message_types_register_simulation_synchronous(c_network_message_type_collection* message_collection);

