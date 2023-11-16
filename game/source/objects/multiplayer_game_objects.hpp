#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

enum e_global_game_engine_type_flags
{
	_global_game_engine_type_flag_ctf_bit = 0,
	_global_game_engine_type_flag_slayer_bit,
	_global_game_engine_type_flag_oddball_bit,
	_global_game_engine_type_flag_king_bit,
	_global_game_engine_type_flag_juggernaut_bit,
	_global_game_engine_type_flag_territories_bit,
	_global_game_engine_type_flag_assault_bit,
	_global_game_engine_type_flag_vip_bit,
	_global_game_engine_type_flag_infection_bit,
	_global_game_engine_type_flag_target_training_bit,

	k_global_game_engine_type_flags
};

enum e_multiplayer_object_type
{
	_multiplayer_object_type_ordinary = 0,
	_multiplayer_object_type_weapon,
	_multiplayer_object_type_grenade,
	_multiplayer_object_type_projectile,
	_multiplayer_object_type_powerup,
	_multiplayer_object_type_equipment,
	_multiplayer_object_type_light_land_vehicle,
	_multiplayer_object_type_heavy_land_vehicle,
	_multiplayer_object_type_flying_vehicle,
	_multiplayer_object_type_teleporter_two_way,
	_multiplayer_object_type_teleporter_sender,
	_multiplayer_object_type_teleporter_receiver,
	_multiplayer_object_type_player_spawn_location,
	_multiplayer_object_type_player_respawn_zone,
	_multiplayer_object_type_oddball_spawn_location,
	_multiplayer_object_type_ctf_flag_spawn_location,
	_multiplayer_object_type_target_spawn_location,
	_multiplayer_object_type_ctf_flag_return_area,
	_multiplayer_object_type_koth_hill_area,
	_multiplayer_object_type_infection_safe_area,
	_multiplayer_object_type_territory_area,
	_multiplayer_object_type_vip_influence_area,
	_multiplayer_object_type_vip_destination_zone,
	_multiplayer_object_type_juggernaut_destination_zone,

	k_multiplayer_object_type_count
};

enum e_teleporter_passability_flags
{
	_teleporter_passability_flag_disallow_players_bit = 0,
	_teleporter_passability_flag_allow_light_land_vehicles_bit,
	_teleporter_passability_flag_allow_heavy_land_vehicles_bit,
	_teleporter_passability_flag_allow_flying_vehicles_bit,
	_teleporter_passability_flag_allow_projectiles_bit,

	k_teleporter_passability_flags
};

enum e_multiplayer_object_flags
{
	_multiplayer_object_flag_only_visible_in_editor_bit = 0,
	_multiplayer_object_flag_valid_initial_player_spawn_bit,
	_multiplayer_object_flag_fixed_boundary_orientation_bit,
	_multiplayer_object_flag_candy_monitor_should_ignore_bit,

	k_multiplayer_object_flags
};

enum e_multiplayer_object_boundary_shape
{
	_multiplayer_object_boundary_shape_unused = 0,
	_multiplayer_object_boundary_shape_sphere,
	_multiplayer_object_boundary_shape_cylinder,
	_multiplayer_object_boundary_shape_box,

	k_multiplayer_object_boundary_shape_count
};

enum e_multiplayer_object_spawn_timer_type
{
	_multiplayer_object_spawn_timer_type_starts_on_death = 0,
	_multiplayer_object_spawn_timer_type_starts_on_disturbance,

	k_multiplayer_object_spawn_timer_type_count
};

struct s_multiplayer_object_boundary_geometry_data
{
	//long boundary_shape;
	c_enum<e_multiplayer_object_boundary_shape, long, _multiplayer_object_boundary_shape_unused, k_multiplayer_object_boundary_shape_count> boundary_shape;

	// shader used for boundary geometry
	union { long standard_shader_index; long opaque_shader_index; };

	real boundary_width;
	real boundary_box_length;
	real boundary_positive_height;
	real boundary_negative_height;
	real_matrix4x3 boundary_matrix;
	real boundary_radius;
};
static_assert(sizeof(s_multiplayer_object_boundary_geometry_data) == 0x50);

struct s_multiplayer_object_properties_definition
{
	// GAME ENGINE INCLUSION
	// Set which game engines you desire this object to appear in
	c_flags<e_global_game_engine_type_flags, word_flags, k_global_game_engine_type_flags> game_engine_flags;

	// TYPE AND FLAGS
	// Type pertains to how you want the game engine to handle this object
	// Certain flags applicable only to certain object types, should be self evident
	c_enum<e_multiplayer_object_type, char, _multiplayer_object_type_ordinary, k_multiplayer_object_type_count> type;
	c_flags<e_teleporter_passability_flags, byte_flags, k_teleporter_passability_flags> teleporter_passability; // used only for teleporters
	c_flags<e_multiplayer_object_flags, word_flags, k_multiplayer_object_flags> flags;

	// GOAL
	// These fields are only used for goal area objects with boundaries, and for respawn zones
	c_enum<e_multiplayer_object_boundary_shape, char, _multiplayer_object_boundary_shape_unused, k_multiplayer_object_boundary_shape_count> boundary_shape;

	// SPAWNING DATA
	// These fields are used for default spawning times and remapping
	c_enum<e_multiplayer_object_spawn_timer_type, char, _multiplayer_object_spawn_timer_type_starts_on_death, k_multiplayer_object_spawn_timer_type_count> spawn_timer_type;
	short default_spawn_time; // seconds
	short default_abandonment_time; // seconds

	// MORE GOAL
	// These fields are only used for goal area objects with boundaries, and for respawn zones
	union { real boundary_width; real boundary_radius; };
	real boundary_box_length;
	real boundary_positive_height; // boundary +height
	real boundary_negative_height; // boundary +height

	// RESPAWN ZONE DATA
	// These are respawn zone weights, used only for respawn zones
	real normal_weight; // aka natural weight
	real flag_away_weight;
	real flag_at_home_weight;

	// MARKER DATA
	// These fields are only used for defining certain, special markers to use for positional locations if the default position (object origin) is not sufficient
	c_string_id boundary_center_marker;
	c_string_id spawned_object_marker_name;

	// SPAWNED OBJECT
	// If this object is intended to spawn something, the object it spawns should be specified here
	c_typed_tag_reference<OBJECT_TAG> spawned_object;
	c_string_id nyi_boundary_material;

	// BOUNDARY SHADER - DEFAULT
	// These are default shaders used for boundary geometry
	c_typed_tag_reference<RENDER_METHOD_TAG> boundary_standard_shader;
	c_typed_tag_reference<RENDER_METHOD_TAG> boundary_opaque_shader;

	// BOUNDARY SHADER - SPHERE
	// These are custom shaders used for spherical boundary geometry
	c_typed_tag_reference<RENDER_METHOD_TAG> sphere_standard_shader;
	c_typed_tag_reference<RENDER_METHOD_TAG> sphere_opaque_shader;

	// BOUNDARY SHADER - CYLINDER
	// These are custom shaders used for cylindrical boundary geometry
	c_typed_tag_reference<RENDER_METHOD_TAG> cylinder_standard_shader;
	c_typed_tag_reference<RENDER_METHOD_TAG> cylinder_opaque_shader;

	// BOUNDARY SHADER - BOX
	// These are custom shaders used for box boundary geometry
	c_typed_tag_reference<RENDER_METHOD_TAG> box_standard_shader;
	c_typed_tag_reference<RENDER_METHOD_TAG> box_opaque_shader;

	void update_reference_names();
};
static_assert(sizeof(s_multiplayer_object_properties_definition) == 0xC4);

extern void __cdecl multiplayer_object_render_debug_boundary(s_multiplayer_object_boundary_geometry_data const* geometry_data, real_argb_color const* color);

extern void debug_multiplayer_object_boundary_geometry(bool should_debug);

