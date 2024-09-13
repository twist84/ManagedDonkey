#include "simulation/simulation_queue_events.hpp"

#include "memory/module.hpp"
#include "game_interface/simulation_game_events.hpp"

t_value_type<byte> const simulation_event_type_count = { .value = k_simulation_event_type_count_plus_additional };
DATA_PATCH_DECLARE(0x004A87E0, simulation_event_type_count, simulation_event_type_count.bytes);

//.text:004A8780 ; bool __cdecl decode_event_from_buffer(byte*, long, s_simulation_queue_decoded_event_data*)

