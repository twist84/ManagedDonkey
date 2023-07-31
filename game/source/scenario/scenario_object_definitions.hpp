#pragma once

#include "cseries/cseries.hpp"
#include "objects/multiplayer_game_objects.hpp"
#include "tag_files/tag_groups.hpp"

template<tag ...t_group_tags>
struct scenario_object_palette_entry
{
	c_typed_tag_reference<t_group_tags> name;

	byte GYFQQPUM[0x20]; // pad

	void update_reference_names()
	{
		name.get_name();
	}
};
static_assert(sizeof(scenario_object_palette_entry<'test'>) == 0x30);

template<tag ...t_group_tags>
struct scenario_object_palette_entry_with_string_id
{
	c_typed_tag_reference<t_group_tags> name;
	c_string_id display_name;
	long maximum_allowed;
	real price_per_instance;

	byte GYFQQPUM[0x14]; // pad

	void update_reference_names()
	{
		name.get_name();
	}
};
static_assert(sizeof(scenario_object_palette_entry_with_string_id<'test'>) == 0x30);

struct s_scenario_multiplayer_scenario_object_parent
{
	byte der[2];

	// if an object with this name exists, we attach to it as a child
	short parent_object; // short_block_index

	c_string_id parent_marker;
	c_string_id connection_marker;
};
static_assert(sizeof(s_scenario_multiplayer_scenario_object_parent) == 0xC);

enum e_multiplayer_object_placement_spawn_flags
{
	_multiplayer_object_placement_spawn_flag_unique_spawn_bit = 0,
	_multiplayer_object_placement_spawn_flag_not_initially_placed_bit,

	k_multiplayer_object_placement_spawn_flags
};

struct s_scenario_multiplayer_object_properties
{
	// Multiplayer Data
	// object data for multiplayer game use

	long_enum game_engine_symmetric_placement;
	c_flags<e_global_game_engine_type_flags, word_flags, k_global_game_engine_type_flags> game_engine_flags;
	short_enum owner_team;
	char spawn_order; // -1 for random
	char quota_minimum;
	char quota_maximum; // <=0 for unlimited

	c_flags<e_multiplayer_object_placement_spawn_flags, byte_flags, k_multiplayer_object_placement_spawn_flags> spawn_flags;
	short spawn_time; // seconds
	short abandonment_time; // seconds

	char_enum remapping_policy;
	char_enum boundary_shape;
	char_enum teleporter_channel;
	byte blah[1];

	s_scenario_multiplayer_scenario_object_parent map_variant_parent;

	union { real boundary_width; real boundary_radius; };
	real boundary_box_length;
	real boundary_positive_height;
	real boundary_negative_height;

	// Player Respawn Weight
	// This is valid only for objects which are used as player respawn locations
	real natural_respawn_weight;
};
static_assert(sizeof(s_scenario_multiplayer_object_properties) == 0x34);

struct s_scenario_arg_device;
struct s_scenario_crate;
struct s_scenario_creature;
struct s_scenario_terminal;
struct scenario_biped_block;
struct scenario_control_block;
struct scenario_device_group;
struct scenario_effect_scenery_block;
struct scenario_equipment_block;
struct scenario_giant_block;
struct scenario_light_block;
struct scenario_machine_block;
struct scenario_scenery_block;
struct scenario_sound_scenery_block;
struct scenario_vehicle_block;
struct scenario_weapon_block;

