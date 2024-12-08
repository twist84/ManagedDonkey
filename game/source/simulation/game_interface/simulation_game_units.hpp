#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "motor/sync_action.hpp"
#include "objects/damage_reporting.hpp"
#include "simulation/game_interface/simulation_game_entities.hpp"

struct s_simulation_unit_board_vehicle_event_data
{
	long boarding_seat_index;
};
static_assert(sizeof(s_simulation_unit_board_vehicle_event_data) == 0x4);

struct s_simulation_unit_pickup_event_data
{
	short unit_pickup_type;
	long unit_pickup_definition_index;
	long count;
};
static_assert(sizeof(s_simulation_unit_pickup_event_data) == 0xC);

struct s_simulation_unit_exit_vehicle_event_data
{
	long parent_seat_index;
	bool detach_on_exit;
};
static_assert(sizeof(s_simulation_unit_exit_vehicle_event_data) == 0x8);

struct s_simulation_unit_melee_damage_event_data
{
	real_point3d position;
	real_vector3d impact_normal;
	long damage_definition_index;
	long clang_damage_definition_index;
	short breakable_surface_set_index;
	short breakable_surface_index;
	long instanced_geometry_instance_index;
	long surface_index;
	long global_material_index;
	long object_material_index;
	real scale;
	s_damage_reporting_info damage_reporting_info;
};
static_assert(sizeof(s_simulation_unit_melee_damage_event_data) == 0x3C);

struct s_simulation_unit_assassinate_event_data
{
	s_animation_sync_action sync_action;
	s_simulation_unit_melee_damage_event_data melee_damage;
};
static_assert(sizeof(s_simulation_unit_assassinate_event_data) == 0x64);

struct s_simulation_unit_throw_initiate_event_data
{
	// 0: grenade type
	// 1: item
	// 2: projectile
	short throw_type;

	union
	{
		long grenade_type;
		long bomb_run_throw_definition_index;
		long unit_throw_definition_index;
	};
};
static_assert(sizeof(s_simulation_unit_throw_initiate_event_data) == 0x8);

struct s_simulation_unit_melee_initiate_event_data
{
	short melee_type;
	short prediction_type;
};
static_assert(sizeof(s_simulation_unit_melee_initiate_event_data) == 0x4);

struct s_simulation_unit_throw_release_event_data
{
	// 0: grenade type
	// 1: item
	// 2: projectile
	short throw_type;

	union
	{
		long grenade_type;
		long bomb_run_throw_definition_index;
		long unit_throw_definition_index;
	};

	real_point3d position;
	real_vector3d aiming_vector;
	c_aim_target_simulation_entity simulation_entity;
	real_vector3d lead_vector;
};
static_assert(sizeof(s_simulation_unit_throw_release_event_data) == 0x40);

struct s_simulation_unit_melee_clang_event_data
{
	long clang_effect_index;
	real_point3d position;
	real_vector3d forward;
};
static_assert(sizeof(s_simulation_unit_melee_clang_event_data) == 0x1C);

struct s_simulation_unit_enter_vehicle_event_data
{
	long parent_seat_index;
};
static_assert(sizeof(s_simulation_unit_enter_vehicle_event_data) == 0x4);

struct c_simulation_unit_entity_definition :
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

extern c_simulation_unit_entity_definition& g_simulation_unit_entity_definition;

