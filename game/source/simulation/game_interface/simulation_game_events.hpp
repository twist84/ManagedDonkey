#pragma once

#include "cseries/cseries.hpp"

struct c_replication_outgoing_event;
struct s_simulation_view_telemetry_data;
struct c_bitstream;
struct c_simulation_event_definition
{
public:
	virtual e_simulation_event_type event_type() = 0;
	virtual char const* event_type_name() = 0;
	virtual int32 payload_size() = 0;
	virtual int32 number_of_entity_references() = 0;
	virtual bool reference_delays_entity_deletion() = 0;
	virtual bool event_can_be_transmitted(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*) = 0;
	virtual int32 minimum_required_bits(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, int32*) = 0;
	virtual real32 calculate_relevance(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real32) = 0;
	virtual void write_description_to_string(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real32, int32, char*) = 0;
	virtual int32* maximum_required_bits(int32* out_bits, c_replication_outgoing_event*, int32) = 0; // unsure of the name, pc only?
	virtual void event_payload_encode(int32, void const*, c_bitstream*) = 0;
	virtual bool event_payload_decode(int32, void*, c_bitstream*, bool a4) = 0;
	virtual void prepare_event_data_for_gameworld(int32, void*) = 0;
	virtual bool apply_game_event(int32, int32 const*, int32, void const*) = 0;

	c_simulation_event_definition()
	{
	};
};

