#pragma once

/*
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/thread_local.hpp"
#include "simulation/simulation_type_collection.hpp"

struct s_simulation_hs_script_wake_event_data
{
	char script_name[32];
	long script_index;
};
static_assert(sizeof(s_simulation_hs_script_wake_event_data) == 0x24);

bool __cdecl hs_wake_by_name(char const* script_name)
{
	return INVOKE(0x00599250, hs_wake_by_name, script_name);
}

long __cdecl hs_find_thread_by_name(char const* script_name)
{
	return INVOKE(0x00596070, hs_find_thread_by_name, script_name);
}

hs_thread* __cdecl hs_thread_get(long thread_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(hs_thread_deterministic_data);

	hs_thread* result = static_cast<hs_thread*>(datum_try_and_get(*hs_thread_deterministic_data, thread_index));
	return result;
}

void __cdecl hs_find_dormant_script(char const* dormant_script_name, long* script_index_out)
{
	ASSERT(dormant_script_name);
	ASSERT(script_index_out);

	*script_index_out = NONE;

	long thread_index = hs_find_thread_by_name(dormant_script_name);
	if (thread_index == NONE)
		return;

	hs_thread* thread = hs_thread_get(thread_index);

	if (global_scenario_get()->scripts[thread->script_index].script_type == _hs_script_type_dormant)
		*script_index_out = thread->script_index;
}

struct c_simulation_hs_script_wake_event_definition :
	public c_simulation_event_definition
{
	virtual e_simulation_event_type __cdecl event_type() override
	{
		return _event_simulation_type_hs_script_wake;
	}

	virtual char const* __cdecl event_type_name() override
	{
		return "hs_script_wake";
	}

	virtual long __cdecl payload_size() override
	{
		return sizeof(s_simulation_hs_script_wake_event_data);
	}

	virtual long __cdecl number_of_entity_references() override
	{
		return 0;
	}

	virtual bool __cdecl reference_delays_entity_deletion() override
	{
		return 0;
	}

	virtual bool __cdecl event_can_be_transmitted(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*) override
	{
		return true;
	}

	virtual long __cdecl minimum_required_bits(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, long*) override
	{
		return 42;
	}

	virtual real __cdecl calculate_relevance(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real) override
	{
		return 0.0f;
	}

	virtual void __cdecl write_description_to_string(c_replication_outgoing_event const* a1, s_simulation_view_telemetry_data const* a2, real a3, long buffer_count, char* buffer) override
	{
		csnzprintf(buffer, buffer_count, "hs-script-wake : relevance=%5.3f", calculate_relevance(a1, a2, a3));
	}

	// #TODO: figure it out
	virtual void* __cdecl __func9(dword* out_flags, void*) override
	{
		*out_flags = 0x5500;
	}

	virtual void __cdecl event_payload_encode(long payload_size, void const* payload, c_bitstream* packet) override
	{
		ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
		ASSERT(payload);
		ASSERT(packet);

		s_simulation_hs_script_wake_event_data const* definition = static_cast<s_simulation_hs_script_wake_event_data const*>(payload);

		packet->write_integer("script-index", definition->script_index, 10);
		packet->write_string("script-name", definition->script_name, 32);
	}

	virtual bool __cdecl event_payload_decode(long payload_size, void* payload, c_bitstream* packet) override
	{
		ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
		ASSERT(payload);
		ASSERT(packet);

		s_simulation_hs_script_wake_event_data* definition = static_cast<s_simulation_hs_script_wake_event_data*>(payload);

		definition->script_index = packet->read_integer("script-index", 10);
		packet->read_string("script-name", definition->script_name, 32);

		return definition->script_index != NONE && csstrnlen(definition->script_name, 32);
	}

	virtual void __cdecl prepare_event_data_for_gameworld(long, void*) override
	{
	}

	virtual bool __cdecl apply_game_event(long entity_reference_count, long const*, long payload_size, void const* payload) override
	{
		ASSERT(entity_reference_count == number_of_entity_references());
		ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
		ASSERT(payload);

		s_simulation_hs_script_wake_event_data const* definition = static_cast<s_simulation_hs_script_wake_event_data const*>(payload);

		if (definition->script_index == NONE)
		{
			generate_event(_event_level_warning, "networking:simulation:hs: invalid script index in hs wake event");
			return;
		}

		long script_index = NONE;
		hs_find_dormant_script(definition->script_name, &script_index);
		if (script_index != definition->script_index)
		{
			generate_event(_event_level_warning, "networking:simulation:hs: script index mismatch in hs wake event [0x%08X != 0x%08X]", script_index, definition->script_index);
		}

		hs_wake_by_name(definition->script_name);
	}
};
*/
