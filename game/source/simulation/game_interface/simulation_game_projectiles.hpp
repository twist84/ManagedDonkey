#pragma once

#include "cseries/location.hpp"
#include "simulation/game_interface/simulation_game_interface.hpp"

struct s_simulation_projectile_attached_event_data
{
	bool attached_to_object;
	int32 node_index;
	int32 position_encoding_type;
	real_point3d node_position;
	s_location location;
	byte __data1A[0x2];
};
COMPILE_ASSERT(sizeof(s_simulation_projectile_attached_event_data) == 0x1C);

struct s_simulation_projectile_detonate_event_data
{
	int32 projectile_definition_index;
	real_point3d position;
	real_vector3d forward;
	real32 damage_scale;
	uns8 projectile_effect_flag;
	bool collided_with_invalid_material;
	int32 material_index;

	bool location_valid;
	uns8 : 8;
	s_location location;

	real_vector3d hit_normal;
	int32 player_index0; // from?
	int32 player_index1; // to?
};
COMPILE_ASSERT(sizeof(s_simulation_projectile_detonate_event_data) == 0x40);

struct s_simulation_projectile_impact_effect_event_data
{
	int32 projectile_definition_index;
	real32 material_effect_scale;
	real32 material_effect_error;
	real_vector3d impact_direction_normal;
	real_point3d position;
	real_vector3d collision_plane_normal;
	int32 material_index;
	bool from_impact;
};
COMPILE_ASSERT(sizeof(s_simulation_projectile_impact_effect_event_data) == 0x38);

struct s_simulation_projectile_object_impact_effect_event_data :
	s_simulation_projectile_impact_effect_event_data
{
	bool detonation_timer_started;
	int32 collision_node_index;
	int32 position_encoding_type;
};
COMPILE_ASSERT(sizeof(s_simulation_projectile_object_impact_effect_event_data) == 0x44);

struct s_simulation_projectile_supercombine_request_event_data
{
	int32 projectile_definition_index;
};
COMPILE_ASSERT(sizeof(s_simulation_projectile_supercombine_request_event_data) == 0x4);

class c_simulation_projectile_entity_definition :
	public c_simulation_game_engine_globals_definition
{
public:
	//virtual e_simulation_entity_type entity_type();
	//virtual const char* entity_type_name();
	//virtual int32 state_data_size();
	//virtual int32 creation_data_size();
	//virtual int32 update_flag_count();
	//virtual uns64& initial_update_mask(uns64& mask);
	//virtual bool entity_replication_required_for_view_activation(const s_simulation_entity*);
	//virtual bool entity_type_is_gameworld_object();
	//virtual bool gameworld_attachment_valid(int32);
	//virtual bool entity_can_be_created(const s_simulation_entity*, const s_simulation_view_telemetry_data*);
	//virtual void creation_minimum_required_bits(const s_simulation_entity*, const s_simulation_view_telemetry_data*, int32*);
	//virtual void write_creation_description_to_string(const s_simulation_entity*, const s_simulation_view_telemetry_data*, int32, char*);
	//virtual void update_minimum_required_bits(const s_simulation_entity*, const s_entity_update_data*, int32*);
	//virtual void entity_creation_encode(int32, const void*, const s_simulation_view_telemetry_data*, c_bitstream*, bool);
	//virtual bool entity_creation_decode(int32, void*, c_bitstream*, bool);
	//virtual bool entity_update_encode(bool, uns32, uns32*, int32, const void*, const s_simulation_view_telemetry_data*, c_bitstream*, int32, bool);
	//virtual bool entity_update_decode(bool, uns32*, int32, void*, c_bitstream*, bool);
	//virtual bool entity_state_lossy_compare(void*, void*, int32);
	//virtual bool entity_creation_lossy_compare(void*, void*, int32);
	//virtual void build_creation_data(int32, int32, void*);
	//virtual bool build_baseline_state_data(int32, const void*, int32, void*);
	//virtual bool build_updated_state_data(const s_simulation_entity*, uns32*, int32, void*);
	//virtual void rotate_entity_indices(const s_simulation_entity*);
	//virtual void prepare_creation_data_for_gameworld(int32, void*);
	//virtual void prepare_state_data_for_gameworld(uns32, int32, void*);
	//virtual bool create_game_entity(int32, int32, const void*, uns32, int32, const void*);
	//virtual bool update_game_entity(int32, uns32, int32, const void*);
	//virtual bool delete_game_entity(int32);
	//virtual bool promote_game_entity_to_authority(int32);
	//virtual void write_update_description_to_string(const s_simulation_entity*, const s_entity_update_data*, int32, char*);
};

extern c_simulation_projectile_entity_definition& g_simulation_projectile_entity_definition;

