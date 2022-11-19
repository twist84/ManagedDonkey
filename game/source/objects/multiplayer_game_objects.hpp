#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_multiplayer_object_boundary_geometry_data
{
	long boundary_shape;

	// shader used for boundary geometry
	long standard_shader_or_opaque_shader_index;

	real boundary_width;
	real boundary_box_length;
	real boundary_positive_height;
	real boundary_negative_height;
	real_matrix4x3 boundary_matrix;
	real boundary_raius;
};
static_assert(sizeof(s_multiplayer_object_boundary_geometry_data) == 0x50);

struct s_multiplayer_object_properties_definition
{
	// GAME ENGINE INCLUSION
	// Set which game engines you desire this object to appear in

	word_flags game_engine_flags;

	// TYPE AND FLAGS
	// Type pertains to how you want the game engine to handle this object
	// Certain flags applicable only to certain object types, should be self evident

	char_enum type;
	byte_flags teleporter_passability;
	word_flags flags;

	// GOAL
	// These fields are only used for goal area objects with boundaries, and for respawn zones

	char_enum boundary_shape;

	// SPAWNING DATA
	// These fields are used for default spawning times and remapping

	char_enum spawn_timer_type;
	short default_spawn_time; // seconds
	short default_abandonment_time; // seconds

	// MORE GOAL
	// These fields are only used for goal area objects with boundaries, and for respawn zones

	union { real boundary_width; real boundary_radius; };
	real boundary_box_length;
	real boundary_positive_height; // boundary +height
	real boundary_negative_height; // boundary -height

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

	c_typed_tag_reference<'obje'> spawned_object;
	c_string_id NYI_boundary_material;

	// BOUNDARY SHADER - DEFAULT
	// These are default shaders used for boundary geometry

	c_typed_tag_reference<'rm '> boundary_standard_shader;
	c_typed_tag_reference<'rm '> boundary_opaque_shader;

	// BOUNDARY SHADER - SPHERE
	// These are custom shaders used for spherical geometry

	c_typed_tag_reference<'rm '> sphere_standard_shader;
	c_typed_tag_reference<'rm '> sphere_opaque_shader;

	// BOUNDARY SHADER - CYLINDER
	// These are custom shaders used for cylindrical boundary geometry

	c_typed_tag_reference<'rm '> cylinder_standard_shader;
	c_typed_tag_reference<'rm '> cylinder_opaque_shader;

	// BOUNDARY SHADER - BOX
	// These are custom shaders used for box geometry

	c_typed_tag_reference<'rm '> box_standard_shader;
	c_typed_tag_reference<'rm '> box_opaque_shader;
};
static_assert(sizeof(s_multiplayer_object_properties_definition) == 0xC4);

