#pragma once

#include "cseries/cseries.hpp"

struct s_simulation_queue_decoded_event_data
{
    long event_type;

    long reference_count;
    long references[2];

    byte payload[0x400];
    long payload_size;
};
static_assert(sizeof(s_simulation_queue_decoded_event_data) == 0x414);

