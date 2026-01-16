#pragma once

#include "cseries/cseries.hpp"
#include "saved_games/scenario_map_variant.hpp"
#include "simulation/game_interface/simulation_game_entities.hpp"

struct s_player_taunt_request_event_data
{
	int16 absolute_player_index;
	int16 __unknown2;
};
COMPILE_ASSERT(sizeof(s_player_taunt_request_event_data) == 0x4);

struct s_player_respawn_target_request_event_data
{
	int16 player_index;
	int16 absolute_player_index;
};
COMPILE_ASSERT(sizeof(s_player_respawn_target_request_event_data) == 0x4);

struct s_player_force_base_respawn_event_data
{
	int16 absolute_player_index;
};
COMPILE_ASSERT(sizeof(s_player_force_base_respawn_event_data) == 0x2);

struct s_player_editor_request_event_data
{
	int32 editor_request_type;
	int16 editor_player_index;
	union
	{
		int32 edited_object_id;
		int32 object_definition_index;
	};
	real_point3d player_placement_point;
	real_vector3d player_placement_axes_forward;
	real_vector3d player_placement_axes_up;
	real_point3d placement_point;
	real_vector3d placement_axes_forward;
	real_vector3d placement_axes_up;
	real32 throw_force;
	int32 changed_gametype;
	uns8 minimum_count;
	uns8 maximum_count;
	bool editor_desired_mode;
	s_variant_multiplayer_object_properties_definition placement_properties;
};
COMPILE_ASSERT(sizeof(s_player_editor_request_event_data) == 0x78);

class c_simulation_game_engine_player_entity_definition :
	public c_simulation_entity_definition
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

extern c_simulation_game_engine_player_entity_definition& g_simulation_game_engine_player_entity_definition;

