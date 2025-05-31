#include "simulation/game_interface/simulation_game_hs.hpp"

#include "cseries/cseries_events.hpp"
#include "hs/hs_runtime.hpp"
#include "memory/bitstream.hpp"

c_simulation_hs_script_wake_event_definition g_simulation_hs_script_wake_event_definition{};

e_simulation_event_type c_simulation_hs_script_wake_event_definition::event_type()
{
	return _simulation_event_type_hs_script_wake;
}

const char* c_simulation_hs_script_wake_event_definition::event_type_name()
{
	return "hs_script_wake";
}

int32 c_simulation_hs_script_wake_event_definition::payload_size()
{
	return sizeof(s_simulation_hs_script_wake_event_data);
}

int32 c_simulation_hs_script_wake_event_definition::number_of_entity_references()
{
	return 0;
}

bool c_simulation_hs_script_wake_event_definition::reference_delays_entity_deletion()
{
	return 0;
}

bool c_simulation_hs_script_wake_event_definition::event_can_be_transmitted(const c_replication_outgoing_event*, const s_simulation_view_telemetry_data*)
{
	return true;
}

int32 c_simulation_hs_script_wake_event_definition::minimum_required_bits(const c_replication_outgoing_event*, const s_simulation_view_telemetry_data*, int32*)
{
	return 42;
}

real32 c_simulation_hs_script_wake_event_definition::calculate_relevance(const c_replication_outgoing_event*, const s_simulation_view_telemetry_data*, real32)
{
	return 0.0f;
}

void c_simulation_hs_script_wake_event_definition::write_description_to_string(const c_replication_outgoing_event* a1, const s_simulation_view_telemetry_data* a2, real32 a3, int32 buffer_count, char* buffer)
{
	csnzprintf(buffer, buffer_count, "hs-script-wake : relevance=%5.3f", calculate_relevance(a1, a2, a3));
}

int32* c_simulation_hs_script_wake_event_definition::maximum_required_bits(int32* out_bits, c_replication_outgoing_event*, int32)
{
	*out_bits = 0x5500;

	return out_bits;
}

void c_simulation_hs_script_wake_event_definition::event_payload_encode(int32 payload_size, const void* payload, c_bitstream* packet)
{
	ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
	ASSERT(payload);
	ASSERT(packet);

	const s_simulation_hs_script_wake_event_data* definition = (const s_simulation_hs_script_wake_event_data*)payload;

	packet->write_integer("script-index", definition->script_index, 10);
	packet->write_string("script-name", definition->script_name, 32);
}

bool c_simulation_hs_script_wake_event_definition::event_payload_decode(int32 payload_size, void* payload, c_bitstream* packet, bool a4)
{
	ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
	ASSERT(payload);
	ASSERT(packet);

	s_simulation_hs_script_wake_event_data* definition = (s_simulation_hs_script_wake_event_data*)payload;

	definition->script_index = packet->read_integer("script-index", 10);
	packet->read_string("script-name", definition->script_name, 32);

	return definition->script_index != NONE && csstrnlen(definition->script_name, 32);
}

void c_simulation_hs_script_wake_event_definition::prepare_event_data_for_gameworld(int32, void*)
{
}

bool c_simulation_hs_script_wake_event_definition::apply_game_event(int32 entity_reference_count, const int32*, int32 payload_size, const void* payload)
{
	ASSERT(entity_reference_count == number_of_entity_references());
	ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
	ASSERT(payload);

	const s_simulation_hs_script_wake_event_data* definition = (const s_simulation_hs_script_wake_event_data*)payload;

	if (definition->script_index == NONE)
	{
		event(_event_warning, "networking:simulation:hs: invalid script index in hs wake event");
		return true;
	}

	int32 script_index = NONE;
	hs_find_dormant_script(definition->script_name, &script_index);
	if (script_index != definition->script_index)
	{
		event(_event_warning, "networking:simulation:hs: script index mismatch in hs wake event [0x%08X != 0x%08X]", script_index, definition->script_index);
	}

	hs_wake_by_name(definition->script_name);

	return true;
}

