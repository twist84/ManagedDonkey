#pragma once

#include "cseries/cseries.hpp"
#include "simulation/game_interface/simulation_game_interface.hpp"

struct s_simulation_projectile_attached_event_data
{
	bool attached_to_object;
	long node_index;
	long position_encoding_type;
	real_point3d node_position;
	s_location location;
	uint8 __data1A[0x2];
};
static_assert(sizeof(s_simulation_projectile_attached_event_data) == 0x1C);

struct s_simulation_projectile_detonate_event_data
{
	long projectile_definition_index;
	real_point3d position;
	real_vector3d forward;
	real32 damage_scale;
	uint8 projectile_effect_flag;
	bool collided_with_invalid_material;
	long material_index;

	bool location_valid;
	uint8 : 8;
	s_location location;

	real_vector3d hit_normal;
	long player_index0; // from?
	long player_index1; // to?
};
static_assert(sizeof(s_simulation_projectile_detonate_event_data) == 0x40);

struct s_simulation_projectile_impact_effect_event_data
{
	long projectile_definition_index;
	real32 material_effect_scale;
	real32 material_effect_error;
	real_vector3d impact_direction_normal;
	real_point3d position;
	real_vector3d collision_plane_normal;
	long material_index;
	bool from_impact;
};
static_assert(sizeof(s_simulation_projectile_impact_effect_event_data) == 0x38);

struct s_simulation_projectile_object_impact_effect_event_data :
	s_simulation_projectile_impact_effect_event_data
{
	bool detonation_timer_started;
	long collision_node_index;
	long position_encoding_type;
};
static_assert(sizeof(s_simulation_projectile_object_impact_effect_event_data) == 0x44);

struct s_simulation_projectile_supercombine_request_event_data
{
	long projectile_definition_index;
};
static_assert(sizeof(s_simulation_projectile_supercombine_request_event_data) == 0x4);

struct c_simulation_projectile_entity_definition :
	public c_simulation_game_engine_globals_definition
{
public:
	//virtual e_simulation_entity_type entity_type();
	//virtual char const* entity_type_name();
	//virtual long state_data_size();
	//virtual long creation_data_size();
	//virtual long update_flag_count();
	//virtual uint64& initial_update_mask(uint64& mask);
	//virtual bool entity_replication_required_for_view_activation(s_simulation_entity const*);
	//virtual bool entity_type_is_gameworld_object();
	//virtual bool gameworld_attachment_valid(long);
	//virtual bool entity_can_be_created(s_simulation_entity const*, s_simulation_view_telemetry_data const*);
	//virtual void creation_minimum_required_bits(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long*);
	//virtual void write_creation_description_to_string(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long, char*);
	//virtual void update_minimum_required_bits(s_simulation_entity const*, s_entity_update_data const*, long*);
	//virtual void entity_creation_encode(long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, bool);
	//virtual bool entity_creation_decode(long, void*, c_bitstream*, bool);
	//virtual bool entity_update_encode(bool, uint32, uint32*, long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, long, bool);
	//virtual bool entity_update_decode(bool, uint32*, long, void*, c_bitstream*, bool);
	//virtual bool entity_state_lossy_compare(void*, void*, long);
	//virtual bool entity_creation_lossy_compare(void*, void*, long);
	//virtual void build_creation_data(long, long, void*);
	//virtual bool build_baseline_state_data(long, void const*, long, void*);
	//virtual bool build_updated_state_data(s_simulation_entity const*, uint32*, long, void*);
	//virtual void rotate_entity_indices(s_simulation_entity const*);
	//virtual void prepare_creation_data_for_gameworld(long, void*);
	//virtual void prepare_state_data_for_gameworld(uint32, long, void*);
	//virtual bool create_game_entity(long, long, void const*, uint32, long, void const*);
	//virtual bool update_game_entity(long, uint32, long, void const*);
	//virtual bool delete_game_entity(long);
	//virtual bool promote_game_entity_to_authority(long);
	//virtual void write_update_description_to_string(s_simulation_entity const*, s_entity_update_data const*, long, char*);
};

extern c_simulation_projectile_entity_definition& g_simulation_projectile_entity_definition;

