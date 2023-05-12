#pragma once

#include "cseries/cseries.hpp"

enum e_dynamic_spawn_influencer_shape
{
	_dynamic_spawn_influencer_shape_cylinder = 0,
	_dynamic_spawn_influencer_shape_pill,
	_dynamic_spawn_influencer_shape_cone,
	_dynamic_spawn_influencer_shape_box,
	_dynamic_spawn_influencer_shape_sphere,

	k_dynamic_spawn_influencer_shapes
};

enum e_dynamic_spawn_influencer_type
{
	_dynamic_spawn_influencer_type_enemy_forbid = 0,
	_dynamic_spawn_influencer_type_enemy_bias,
	_dynamic_spawn_influencer_type_ally_bias,
	_dynamic_spawn_influencer_type_selected_ally_bias,
	_dynamic_spawn_influencer_type_dead_teammate,
	_dynamic_spawn_influencer_type_respawn_zone,
	_dynamic_spawn_influencer_type_weapon,
	_dynamic_spawn_influencer_type_vehicle,
	_dynamic_spawn_influencer_type_projectile,
	_dynamic_spawn_influencer_type_equipment,
	_dynamic_spawn_influencer_type_koth_hill,
	_dynamic_spawn_influencer_type_oddball,
	_dynamic_spawn_influencer_type_ctf_flag_away,
	_dynamic_spawn_influencer_type_territories_ally,
	_dynamic_spawn_influencer_type_territories_enemy,
	_dynamic_spawn_influencer_type_infection_safe_zone_human,
	_dynamic_spawn_influencer_type_infection_safe_zone_zombie,
	_dynamic_spawn_influencer_type_vip,

	k_dynamic_spawn_influencer_types,
};

struct s_dynamic_spawn_influencer
{
	c_enum<e_dynamic_spawn_influencer_shape, byte, _dynamic_spawn_influencer_shape_cylinder, k_dynamic_spawn_influencer_shapes> shape;
	c_enum<e_dynamic_spawn_influencer_type, byte, _dynamic_spawn_influencer_type_enemy_forbid, k_dynamic_spawn_influencer_types> type;
	word team; // e_multiplayer_team_designator
	real full_weight_radius;
	real fall_off_radius;
	real total_height;
	real_matrix4x3 matrix;
	real weight;
};
static_assert(sizeof(s_dynamic_spawn_influencer) == 0x48);

struct s_dynamic_spawn_influencer_collection
{
	short player_index;
	short count;
	c_static_array<s_dynamic_spawn_influencer, 446> influencers;
};
static_assert(sizeof(s_dynamic_spawn_influencer_collection) == 0x7D74);

extern bool __cdecl game_engine_should_spawn_player(long player_index);

