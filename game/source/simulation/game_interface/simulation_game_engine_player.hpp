#pragma once

#include "cseries/cseries.hpp"
#include "saved_games/scenario_map_variant.hpp"
#include "simulation/game_interface/simulation_game_entities.hpp"

struct s_player_taunt_request_event_data
{
	short absolute_player_index;
	short __unknown2;
};
static_assert(sizeof(s_player_taunt_request_event_data) == 0x4);

struct s_player_respawn_target_request_event_data
{
	short player_index;
	short absolute_player_index;
};
static_assert(sizeof(s_player_respawn_target_request_event_data) == 0x4);

struct s_player_force_base_respawn_event_data
{
	short absolute_player_index;
};
static_assert(sizeof(s_player_force_base_respawn_event_data) == 0x2);

struct s_player_editor_request_event_data
{
	long editor_request_type;
	short editor_player_index;
	union
	{
		long edited_object_id;
		long object_definition_index;
	};
	real_point3d player_placement_point;
	real_vector3d player_placement_axes_forward;
	real_vector3d player_placement_axes_up;
	real_point3d placement_point;
	real_vector3d placement_axes_forward;
	real_vector3d placement_axes_up;
	real throw_force;
	long changed_gametype;
	byte minimum_count;
	byte maximum_count;
	bool editor_desired_mode;
	s_variant_multiplayer_object_properties_definition placement_properties;
};
static_assert(sizeof(s_player_editor_request_event_data) == 0x78);

struct c_simulation_game_engine_player_entity_definition :
	public c_simulation_entity_definition
{
public:
	//virtual e_simulation_entity_type entity_type();
	//virtual char const* entity_type_name();
	//virtual long state_data_size();
	//virtual long creation_data_size();
	//virtual long update_flag_count();
	//virtual qword& initial_update_mask(qword& mask);
	//virtual bool entity_replication_required_for_view_activation(s_simulation_entity const*);
	//virtual bool entity_type_is_gameworld_object();
	//virtual bool gameworld_attachment_valid(long);
	//virtual bool entity_can_be_created(s_simulation_entity const*, s_simulation_view_telemetry_data const*);
	//virtual void creation_minimum_required_bits(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long*);
	//virtual void write_creation_description_to_string(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long, char*);
	//virtual void update_minimum_required_bits(s_simulation_entity const*, s_entity_update_data const*, long*);
	//virtual void entity_creation_encode(long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, bool);
	//virtual bool entity_creation_decode(long, void*, c_bitstream*, bool);
	//virtual bool entity_update_encode(bool, dword, dword*, long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, long, bool);
	//virtual bool entity_update_decode(bool, dword*, long, void*, c_bitstream*, bool);
	//virtual bool entity_state_lossy_compare(void*, void*, long);
	//virtual bool entity_creation_lossy_compare(void*, void*, long);
	//virtual void build_creation_data(long, long, void*);
	//virtual bool build_baseline_state_data(long, void const*, long, void*);
	//virtual bool build_updated_state_data(s_simulation_entity const*, dword*, long, void*);
	//virtual void rotate_entity_indices(s_simulation_entity const*);
	//virtual void prepare_creation_data_for_gameworld(long, void*);
	//virtual void prepare_state_data_for_gameworld(dword, long, void*);
	//virtual bool create_game_entity(long, long, void const*, dword, long, void const*);
	//virtual bool update_game_entity(long, dword, long, void const*);
	//virtual bool delete_game_entity(long);
	//virtual bool promote_game_entity_to_authority(long);
	//virtual void write_update_description_to_string(s_simulation_entity const*, s_entity_update_data const*, long, char*);
};

extern c_simulation_game_engine_player_entity_definition& g_simulation_game_engine_player_entity_definition;

