#pragma once

#include "cseries/cseries.hpp"

struct s_simulation_queue_decoded_creation_data
{
    // header
    long entity_type;
    long gamestate_index;

    qword initial_update_mask;

    byte creation_payload[0x4000];
    long creation_payload_size;

    byte update_data_payload[0x4000];
    long update_data_payload_size;
};
static_assert(sizeof(s_simulation_queue_decoded_creation_data) == 0x8018);

