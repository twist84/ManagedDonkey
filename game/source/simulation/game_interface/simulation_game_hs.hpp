#pragma once

#include "cseries/cseries.hpp"
#include "simulation/simulation_type_collection.hpp"

struct s_simulation_hs_script_wake_event_data
{
	char script_name[32];
	int32 script_index;
};
COMPILE_ASSERT(sizeof(s_simulation_hs_script_wake_event_data) == 0x24);

class c_simulation_hs_script_wake_event_definition :
	public c_simulation_event_definition
{
public:
	virtual e_simulation_event_type event_type() override;
	virtual const char* event_type_name() override;
	virtual int32 payload_size() override;
	virtual int32 number_of_entity_references() override;
	virtual bool reference_delays_entity_deletion() override;
	virtual bool event_can_be_transmitted(const c_replication_outgoing_event*, const s_simulation_view_telemetry_data*) override;
	virtual int32 minimum_required_bits(const c_replication_outgoing_event*, const s_simulation_view_telemetry_data*, int32*) override;
	virtual real32 calculate_relevance(const c_replication_outgoing_event*, const s_simulation_view_telemetry_data*, real32) override;
	virtual void write_description_to_string(const c_replication_outgoing_event* a1, const s_simulation_view_telemetry_data* a2, real32 a3, int32 buffer_count, char* buffer) override;
	virtual int32* maximum_required_bits(int32* out_bits, c_replication_outgoing_event*, int32) override;
	virtual void event_payload_encode(int32 payload_size, const void* payload, c_bitstream* packet) override;
	virtual bool event_payload_decode(int32 payload_size, void* payload, c_bitstream* packet, bool a4) override;
	virtual void prepare_event_data_for_gameworld(int32, void*) override;
	virtual bool apply_game_event(int32 entity_reference_count, const int32*, int32 payload_size, const void* payload) override;

	c_simulation_hs_script_wake_event_definition() :
		c_simulation_event_definition()
	{
	};
};

extern c_simulation_hs_script_wake_event_definition g_simulation_hs_script_wake_event_definition;

