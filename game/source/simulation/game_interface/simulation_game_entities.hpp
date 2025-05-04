#pragma once

#include "cseries/cseries.hpp"

struct s_simulation_entity;
struct s_simulation_view_telemetry_data;
struct s_entity_update_data;
struct c_bitstream;
struct c_simulation_entity_definition
{
public:
	virtual e_simulation_entity_type entity_type() = 0;
	virtual char const* entity_type_name() = 0;
	virtual int32 state_data_size() = 0;
	virtual int32 creation_data_size() = 0;
	virtual int32 update_flag_count() = 0;
	virtual uns64& initial_update_mask(uns64& mask) = 0;
	virtual bool entity_replication_required_for_view_activation(s_simulation_entity const*) = 0;
	virtual bool entity_type_is_gameworld_object() = 0;
	virtual bool gameworld_attachment_valid(int32) = 0;
	virtual bool entity_can_be_created(s_simulation_entity const*, s_simulation_view_telemetry_data const*) = 0;
	virtual void creation_minimum_required_bits(s_simulation_entity const*, s_simulation_view_telemetry_data const*, int32*) = 0;
	virtual void write_creation_description_to_string(s_simulation_entity const*, s_simulation_view_telemetry_data const*, int32, char*) = 0;
	virtual void update_minimum_required_bits(s_simulation_entity const*, s_entity_update_data const*, int32*) = 0;
	virtual void entity_creation_encode(int32, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, bool) = 0;
	virtual bool entity_creation_decode(int32, void*, c_bitstream*, bool) = 0;
	virtual bool entity_update_encode(bool, uns32, uns32*, int32, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, int32, bool) = 0;
	virtual bool entity_update_decode(bool, uns32*, int32, void*, c_bitstream*, bool) = 0;
	virtual bool entity_state_lossy_compare(void*, void*, int32) = 0;
	virtual bool entity_creation_lossy_compare(void*, void*, int32) = 0;
	virtual void build_creation_data(int32, int32, void*) = 0;
	virtual bool build_baseline_state_data(int32, void const*, int32, void*) = 0;
	virtual bool build_updated_state_data(s_simulation_entity const*, uns32*, int32, void*) = 0;
	virtual void rotate_entity_indices(s_simulation_entity const*) = 0;
	virtual void prepare_creation_data_for_gameworld(int32, void*) = 0;
	virtual void prepare_state_data_for_gameworld(uns32, int32, void*) = 0;
	virtual bool create_game_entity(int32, int32, void const*, uns32, int32, void const*) = 0;
	virtual bool update_game_entity(int32, uns32, int32, void const*) = 0;
	virtual bool delete_game_entity(int32) = 0;
	virtual bool promote_game_entity_to_authority(int32) = 0;
	virtual void write_update_description_to_string(s_simulation_entity const*, s_entity_update_data const*, int32, char*) = 0;

	c_simulation_entity_definition()
	{
	};
};

