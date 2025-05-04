#include "simulation/simulation_queue_events.hpp"

#include "cseries/cseries_events.hpp"
#include "game_interface/simulation_game_events.hpp"
#include "memory/bitstream.hpp"
#include "memory/module.hpp"
#include "simulation/simulation.hpp"
#include "simulation/simulation_gamestate_entities.hpp"
#include "simulation/simulation_type_collection.hpp"

HOOK_DECLARE(0x004A8780, decode_event_from_buffer);

//#define ENABLE_ADDITION_ENTITY_TYPES

bool __cdecl decode_event_from_buffer(byte* data, int32 data_size, s_simulation_queue_decoded_event_data* decoded_event_data)
{
	//return INVOKE(0x004A8780, decode_event_from_buffer, data, data_size, decoded_event_data);

	c_bitstream packet(data, data_size);
	packet.begin_reading();

	decoded_event_data->event_type = packet.read_enum<e_simulation_event_type, 6>("event-type");
	decoded_event_data->reference_count = packet.read_integer("reference-count", 2);

#if defined(ENABLE_ADDITION_ENTITY_TYPES)
	if (decoded_event_data->event_type > k_simulation_event_type_count_plus_additional)
#else
	if (decoded_event_data->event_type > k_simulation_event_type_count)
#endif
	{
		event(_event_error, "networking:simulation:queue:events: failed to decode event (bad event type %d)",
			decoded_event_data->event_type);

		return false;
	}

	if (decoded_event_data->reference_count > k_max_decoded_event_data_reference_count)
	{
		event(_event_error, "networking:simulation:queue:events: crazy reference count during decode %d",
			decoded_event_data->reference_count);

		return false;
	}

	for (int32 reference_index = 0; reference_index < decoded_event_data->reference_count; reference_index++)
	{
		if (packet.read_bool("gamestate-index-exists"))
			simulation_object_glue_index_decode(&packet, &decoded_event_data->references[reference_index]);
		else
			decoded_event_data->references[reference_index] = NONE;
	}

	c_simulation_event_definition* event_definition = simulation_get_type_collection()->get_event_definition(decoded_event_data->event_type);
	decoded_event_data->payload_size = event_definition->payload_size();

	if (decoded_event_data->payload_size > k_max_decoded_event_data_payload_size)
	{
		event(_event_error, "networking:simulation:queue:events: invalid event payload size %d",
			decoded_event_data->payload_size);

		return false;
	}

	if (decoded_event_data->payload_size > 0 && !event_definition->event_payload_decode(decoded_event_data->payload_size, decoded_event_data->payload, &packet, false))
	{
		event(_event_warning, "networking:simulation:queue:events: failed to decode event payload type %d",
			decoded_event_data->event_type);
	}

	bool result = !packet.error_occurred();
	packet.finish_reading();

	return result;
}

bool __cdecl encode_event_to_buffer(byte* buffer, int32 buffer_size, int32* size_out, e_simulation_event_type event_type, int32 reference_count, int32 const* references, void const* payload, int32 payload_size)
{
	return INVOKE(0x004A88B0, encode_event_to_buffer, buffer, buffer_size, size_out, event_type, reference_count, references, payload, payload_size);
}

//.text:004A8A60 ; void __cdecl simulation_queue_event_apply(c_simulation_queue_element const*)
//.text:004A8AE0 ; void __cdecl simulation_queue_event_insert(e_simulation_event_type, int32, int32 const*, int32, void*)

