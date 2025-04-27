#pragma once

#include "cseries/cseries.hpp"

enum
{
    k_max_decoded_event_data_reference_count = 2,
    k_max_decoded_event_data_payload_size = 1024
};

enum e_simulation_event_type;
struct s_simulation_queue_decoded_event_data
{
    e_simulation_event_type event_type;

    long reference_count;
    long references[k_max_decoded_event_data_reference_count];

    uint8 payload[k_max_decoded_event_data_payload_size];
    long payload_size;
};
static_assert(sizeof(s_simulation_queue_decoded_event_data) == 0x414);

extern bool __cdecl decode_event_from_buffer(uint8* data, long data_size, s_simulation_queue_decoded_event_data* decoded_event_data);
extern bool __cdecl encode_event_to_buffer(uint8* buffer, long buffer_size, long* size_out, e_simulation_event_type event_type, long reference_count, long const* references, void const* payload, long payload_size);

