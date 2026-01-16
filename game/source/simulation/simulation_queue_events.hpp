#pragma once

struct s_simulation_queue_decoded_event_data
{
    e_simulation_event_type event_type;

    int32 reference_count;
    int32 references[k_max_decoded_event_data_reference_count];

    byte payload[k_max_decoded_event_data_payload_size];
    int32 payload_size;
};
COMPILE_ASSERT(sizeof(s_simulation_queue_decoded_event_data) == 0x414);

extern bool __cdecl decode_event_from_buffer(byte* data, int32 data_size, s_simulation_queue_decoded_event_data* decoded_event_data);
extern bool __cdecl encode_event_to_buffer(byte* buffer, int32 buffer_size, int32* size_out, e_simulation_event_type event_type, int32 reference_count, const int32* references, const void* payload, int32 payload_size);

