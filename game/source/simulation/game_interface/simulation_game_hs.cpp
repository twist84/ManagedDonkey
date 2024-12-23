#include "simulation/game_interface/simulation_game_hs.hpp"

#include "cseries/cseries_events.hpp"
#include "hs/hs_runtime.hpp"
#include "memory/bitstream.hpp"

c_simulation_hs_script_wake_event_definition g_simulation_hs_script_wake_event_definition{};

e_simulation_event_type c_simulation_hs_script_wake_event_definition::event_type()
{
	return _simulation_event_type_hs_script_wake;
}

char const* c_simulation_hs_script_wake_event_definition::event_type_name()
{
	return "hs_script_wake";
}

long c_simulation_hs_script_wake_event_definition::payload_size()
{
	return sizeof(s_simulation_hs_script_wake_event_data);
}

long c_simulation_hs_script_wake_event_definition::number_of_entity_references()
{
	return 0;
}

bool c_simulation_hs_script_wake_event_definition::reference_delays_entity_deletion()
{
	return 0;
}

bool c_simulation_hs_script_wake_event_definition::event_can_be_transmitted(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*)
{
	return true;
}

long c_simulation_hs_script_wake_event_definition::minimum_required_bits(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, long*)
{
	return 42;
}

real c_simulation_hs_script_wake_event_definition::calculate_relevance(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real)
{
	return 0.0f;
}

void c_simulation_hs_script_wake_event_definition::write_description_to_string(c_replication_outgoing_event const* a1, s_simulation_view_telemetry_data const* a2, real a3, long buffer_count, char* buffer)
{
	csnzprintf(buffer, buffer_count, "hs-script-wake : relevance=%5.3f", calculate_relevance(a1, a2, a3));
}

long* c_simulation_hs_script_wake_event_definition::maximum_required_bits(long* out_bits, c_replication_outgoing_event*, long)
{
	*out_bits = 0x5500;

	return out_bits;
}

void c_simulation_hs_script_wake_event_definition::event_payload_encode(long payload_size, void const* payload, c_bitstream* packet)
{
	ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
	ASSERT(payload);
	ASSERT(packet);

	s_simulation_hs_script_wake_event_data const* definition = static_cast<s_simulation_hs_script_wake_event_data const*>(payload);

	packet->write_integer("script-index", definition->script_index, 10);
	packet->write_string("script-name", definition->script_name, 32);
}

bool c_simulation_hs_script_wake_event_definition::event_payload_decode(long payload_size, void* payload, c_bitstream* packet, bool a4)
{
	ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
	ASSERT(payload);
	ASSERT(packet);

	s_simulation_hs_script_wake_event_data* definition = static_cast<s_simulation_hs_script_wake_event_data*>(payload);

	definition->script_index = packet->read_integer("script-index", 10);
	packet->read_string("script-name", definition->script_name, 32);

	return definition->script_index != NONE && csstrnlen(definition->script_name, 32);
}

void c_simulation_hs_script_wake_event_definition::prepare_event_data_for_gameworld(long, void*)
{
}

bool c_simulation_hs_script_wake_event_definition::apply_game_event(long entity_reference_count, long const*, long payload_size, void const* payload)
{
	ASSERT(entity_reference_count == number_of_entity_references());
	ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
	ASSERT(payload);

	s_simulation_hs_script_wake_event_data const* definition = static_cast<s_simulation_hs_script_wake_event_data const*>(payload);

	if (definition->script_index == NONE)
	{
		WARNING_EVENT("networking:simulation:hs: invalid script index in hs wake event");
		return true;
	}

	long script_index = NONE;
	hs_find_dormant_script(definition->script_name, &script_index);
	if (script_index != definition->script_index)
	{
		WARNING_EVENT("networking:simulation:hs: script index mismatch in hs wake event [0x%08X != 0x%08X]", script_index, definition->script_index);
	}

	hs_wake_by_name(definition->script_name);

	return true;
}

