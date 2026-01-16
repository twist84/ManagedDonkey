#pragma once

#include "cseries/cseries.hpp"

struct s_simulation_queue_decoded_creation_data
{
    // header
    int32 entity_type;
    int32 gamestate_index;

    uns64 initial_update_mask;

    byte creation_payload[0x4000];
    int32 creation_payload_size;

    byte update_data_payload[0x4000];
    int32 update_data_payload_size;
};
COMPILE_ASSERT(sizeof(s_simulation_queue_decoded_creation_data) == 0x8018);

struct s_simulation_queue_decoded_update_data
{
    // header
    int32 entity_type;
    int32 gamestate_index;

    uns64 update_mask;

    byte update_data_payload[0x4000];
    int32 update_data_payload_size;
};
COMPILE_ASSERT(sizeof(s_simulation_queue_decoded_update_data) == 0x4018);

