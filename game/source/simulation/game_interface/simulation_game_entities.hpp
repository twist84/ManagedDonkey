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
	virtual const char* entity_type_name() = 0;
	virtual int32 state_data_size() = 0;
	virtual int32 creation_data_size() = 0;
	virtual int32 update_flag_count() = 0;
	virtual uns64& initial_update_mask(uns64& mask) = 0;
	virtual bool entity_replication_required_for_view_activation(const s_simulation_entity*) = 0;
	virtual bool entity_type_is_gameworld_object() = 0;
	virtual bool gameworld_attachment_valid(int32) = 0;
	virtual bool entity_can_be_created(const s_simulation_entity*, const s_simulation_view_telemetry_data*) = 0;
	virtual void creation_minimum_required_bits(const s_simulation_entity*, const s_simulation_view_telemetry_data*, int32*) = 0;
	virtual void write_creation_description_to_string(const s_simulation_entity*, const s_simulation_view_telemetry_data*, int32, char*) = 0;
	virtual void update_minimum_required_bits(const s_simulation_entity*, const s_entity_update_data*, int32*) = 0;
	virtual void entity_creation_encode(int32, const void*, const s_simulation_view_telemetry_data*, c_bitstream*, bool) = 0;
	virtual bool entity_creation_decode(int32, void*, c_bitstream*, bool) = 0;
	virtual bool entity_update_encode(bool, uns32, uns32*, int32, const void*, const s_simulation_view_telemetry_data*, c_bitstream*, int32, bool) = 0;
	virtual bool entity_update_decode(bool, uns32*, int32, void*, c_bitstream*, bool) = 0;
	virtual bool entity_state_lossy_compare(void*, void*, int32) = 0;
	virtual bool entity_creation_lossy_compare(void*, void*, int32) = 0;
	virtual void build_creation_data(int32, int32, void*) = 0;
	virtual bool build_baseline_state_data(int32, const void*, int32, void*) = 0;
	virtual bool build_updated_state_data(const s_simulation_entity*, uns32*, int32, void*) = 0;
	virtual void rotate_entity_indices(const s_simulation_entity*) = 0;
	virtual void prepare_creation_data_for_gameworld(int32, void*) = 0;
	virtual void prepare_state_data_for_gameworld(uns32, int32, void*) = 0;
	virtual bool create_game_entity(int32, int32, const void*, uns32, int32, const void*) = 0;
	virtual bool update_game_entity(int32, uns32, int32, const void*) = 0;
	virtual bool delete_game_entity(int32) = 0;
	virtual bool promote_game_entity_to_authority(int32) = 0;
	virtual void write_update_description_to_string(const s_simulation_entity*, const s_entity_update_data*, int32, char*) = 0;

	c_simulation_entity_definition()
	{
	};
};

