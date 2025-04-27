#pragma once

#include "cseries/cseries.hpp"

struct s_simulation_queue_decoded_creation_data
{
    // header
    long entity_type;
    long gamestate_index;

    uint64 initial_update_mask;

    uint8 creation_payload[0x4000];
    long creation_payload_size;

    uint8 update_data_payload[0x4000];
    long update_data_payload_size;
};
static_assert(sizeof(s_simulation_queue_decoded_creation_data) == 0x8018);

struct s_simulation_queue_decoded_update_data
{
    // header
    long entity_type;
    long gamestate_index;

    uint64 update_mask;

    uint8 update_data_payload[0x4000];
    long update_data_payload_size;
};
static_assert(sizeof(s_simulation_queue_decoded_update_data) == 0x4018);

