#pragma once

#include "cseries/cseries.hpp"
#include "simulation/simulation_type_collection.hpp"

struct s_simulation_hs_script_wake_event_data
{
	char script_name[32];
	long script_index;
};
static_assert(sizeof(s_simulation_hs_script_wake_event_data) == 0x24);

struct c_simulation_hs_script_wake_event_definition :
	public c_simulation_event_definition
{
public:
	virtual e_simulation_event_type event_type() override;
	virtual char const* event_type_name() override;
	virtual long payload_size() override;
	virtual long number_of_entity_references() override;
	virtual bool reference_delays_entity_deletion() override;
	virtual bool event_can_be_transmitted(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*) override;
	virtual long minimum_required_bits(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, long*) override;
	virtual real calculate_relevance(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real) override;
	virtual void write_description_to_string(c_replication_outgoing_event const* a1, s_simulation_view_telemetry_data const* a2, real a3, long buffer_count, char* buffer) override;
	virtual long* maximum_required_bits(long* out_bits, c_replication_outgoing_event*, long) override;
	virtual void event_payload_encode(long payload_size, void const* payload, c_bitstream* packet) override;
	virtual bool event_payload_decode(long payload_size, void* payload, c_bitstream* packet, bool a4) override;
	virtual void prepare_event_data_for_gameworld(long, void*) override;
	virtual bool apply_game_event(long entity_reference_count, long const*, long payload_size, void const* payload) override;

	c_simulation_hs_script_wake_event_definition() :
		c_simulation_event_definition()
	{
	};
};

extern c_simulation_hs_script_wake_event_definition g_simulation_hs_script_wake_event_definition;

