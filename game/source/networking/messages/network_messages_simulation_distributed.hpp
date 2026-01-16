#pragma once

#include "cseries/cseries.hpp"
#include "game/game_results.hpp"

struct s_network_message_distributed_game_results
{
	int32 establishment_identifier;
	int32 update_number;
	s_game_results_incremental_update update;
};
COMPILE_ASSERT(sizeof(s_network_message_distributed_game_results) == 0x1B7A8);

class c_bitstream;

class c_network_message_distributed_game_results
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_type_collection;
extern void __cdecl network_message_types_register_simulation_distributed(c_network_message_type_collection* message_collection);

