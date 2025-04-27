#pragma once

#include "cseries/cseries.hpp"

enum e_simulation_entity_type
{
	_simulation_entity_type_slayer = 0,
	_simulation_entity_type_ctf,
	_simulation_entity_type_assault,
	_simulation_entity_type_oddball,
	_simulation_entity_type_king,
	_simulation_entity_type_territories,
	_simulation_entity_type_juggernaut,
	_simulation_entity_type_sandbox,
	_simulation_entity_type_infection,
	_simulation_entity_type_vip,
	_simulation_entity_type_game_engine_player,
	_simulation_entity_type_game_statborg,
	_simulation_entity_type_breakable_surface_group,
	_simulation_entity_type_map_variant,
	_simulation_entity_type_unit,
	_simulation_entity_type_item,
	_simulation_entity_type_generic,
	_simulation_entity_type_generic_garbage,
	_simulation_entity_type_vehicle,
	_simulation_entity_type_projectile,
	_simulation_entity_type_weapon,
	_simulation_entity_type_device,

	k_simulation_entity_type_count,

	k_simulation_entity_type_none = -1,
	k_simulation_entity_type_maximum_count = 32
};

struct s_simulation_entity;
struct s_simulation_view_telemetry_data;
struct s_entity_update_data;
struct c_bitstream;
struct c_simulation_entity_definition
{
public:
	virtual e_simulation_entity_type entity_type() = 0;
	virtual char const* entity_type_name() = 0;
	virtual long state_data_size() = 0;
	virtual long creation_data_size() = 0;
	virtual long update_flag_count() = 0;
	virtual uint64& initial_update_mask(uint64& mask) = 0;
	virtual bool entity_replication_required_for_view_activation(s_simulation_entity const*) = 0;
	virtual bool entity_type_is_gameworld_object() = 0;
	virtual bool gameworld_attachment_valid(long) = 0;
	virtual bool entity_can_be_created(s_simulation_entity const*, s_simulation_view_telemetry_data const*) = 0;
	virtual void creation_minimum_required_bits(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long*) = 0;
	virtual void write_creation_description_to_string(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long, char*) = 0;
	virtual void update_minimum_required_bits(s_simulation_entity const*, s_entity_update_data const*, long*) = 0;
	virtual void entity_creation_encode(long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, bool) = 0;
	virtual bool entity_creation_decode(long, void*, c_bitstream*, bool) = 0;
	virtual bool entity_update_encode(bool, uint32, uint32*, long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, long, bool) = 0;
	virtual bool entity_update_decode(bool, uint32*, long, void*, c_bitstream*, bool) = 0;
	virtual bool entity_state_lossy_compare(void*, void*, long) = 0;
	virtual bool entity_creation_lossy_compare(void*, void*, long) = 0;
	virtual void build_creation_data(long, long, void*) = 0;
	virtual bool build_baseline_state_data(long, void const*, long, void*) = 0;
	virtual bool build_updated_state_data(s_simulation_entity const*, uint32*, long, void*) = 0;
	virtual void rotate_entity_indices(s_simulation_entity const*) = 0;
	virtual void prepare_creation_data_for_gameworld(long, void*) = 0;
	virtual void prepare_state_data_for_gameworld(uint32, long, void*) = 0;
	virtual bool create_game_entity(long, long, void const*, uint32, long, void const*) = 0;
	virtual bool update_game_entity(long, uint32, long, void const*) = 0;
	virtual bool delete_game_entity(long) = 0;
	virtual bool promote_game_entity_to_authority(long) = 0;
	virtual void write_update_description_to_string(s_simulation_entity const*, s_entity_update_data const*, long, char*) = 0;

	c_simulation_entity_definition()
	{
	};
};

