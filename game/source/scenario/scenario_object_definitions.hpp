#pragma once

#include "cseries/cseries.hpp"
#include "objects/multiplayer_game_objects.hpp"
#include "objects/object_definitions.hpp"
#include "tag_files/tag_groups.hpp"

template<tag ...t_group_tags>
struct scenario_object_palette_entry
{
	c_typed_tag_reference<t_group_tags> name;

	byte GYFQQPUM[0x20]; // pad

	void update_reference_names()
	{
		UPDATE_REFERENCE_NAME(name);
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
		UPDATE_REFERENCE_NAME(name);
	}
};
static_assert(sizeof(scenario_object_palette_entry_with_string_id<'test'>) == 0x30);

struct s_scenario_object_datum
{
	dword_flags placement_flags;
	real_point3d position;
	real_euler_angles3d rotation;
	real scale;
	s_tag_block node_orientations;
	word_flags transform_flags;
	word manual_bsp_flags; // word_block_flags
	c_string_id light_airprobe_name;
	c_object_identifier object_id;
	char bsp_policy; // char_enum
	char editing_bound_to_bsp; // char_block_index
	short editor_folder; // short_block_index
	s_scenario_multiplayer_scenario_object_parent parent_id;
	word can_attach_to_bsp_flags; // word_block_flags

	// pad
	byte asdf[0x2];
};
static_assert(sizeof(s_scenario_object_datum) == 0x50);

struct s_scenario_object_permutation
{
	string_id variant_name;
	dword_flags active_change_colors;
	rgb_color change_colors[5];
};
static_assert(sizeof(s_scenario_object_permutation) == 0x1C);

struct s_scenario_object
{
	short type;
	short name;
	s_scenario_object_datum object_data;
	s_scenario_object_permutation permutation_data;
};
static_assert(sizeof(s_scenario_object) == 0x70);

enum e_scenario_unit_datum_flags
{
	_scenario_unit_datum_dead_bit = 0,
	_scenario_unit_datum_closed_bit,
	_scenario_unit_datum_not_enterable_by_player_bit,

	k_scenario_unit_datum_flags
};

struct s_scenario_unit_datum
{
	real body_vitality;
	c_flags<e_scenario_unit_datum_flags, dword, k_scenario_unit_datum_flags> flags;
};
static_assert(sizeof(s_scenario_unit_datum) == 0x8);

struct s_scenario_unit :
	s_scenario_object
{
	s_scenario_unit_datum unit_data;
	s_scenario_multiplayer_object_properties multiplayer_data;
};
static_assert(sizeof(s_scenario_unit) == 0xAC);

struct s_scenario_arg_device;
struct s_scenario_crate;
struct s_scenario_creature;
struct s_scenario_terminal;

struct s_scenario_biped :
	s_scenario_unit
{
};
static_assert(sizeof(s_scenario_biped) == sizeof(s_scenario_unit));

struct s_scenario_control;

enum e_device_group_flags
{
	_device_group_flag_can_change_only_once_bit = 0,

	k_device_group_flags
};

struct scenario_device_group
{
	c_static_string<k_tag_string_length> name;
	real initial_value; // [0,1]
	c_flags<e_device_group_flags, dword, k_device_group_flags> flags;
	short editor_folder; // short_block_index

	// pad
	byte WAFNDIO[0x2];
};
static_assert(sizeof(scenario_device_group) == 0x2C);

struct s_scenario_effect_scenery;
struct s_scenario_equipment;

struct scenario_giant_datum
{
	short_enum pathfinding_policy;

	// pad
	byte post_pathfinding[0x2];

	s_tag_block pathfinding_references;
};
static_assert(sizeof(scenario_giant_datum) == 0x10);

struct s_scenario_giant :
	s_scenario_unit
{
	scenario_giant_datum giant_data;
};
static_assert(sizeof(s_scenario_giant) == sizeof(s_scenario_unit) + sizeof(scenario_giant_datum));

struct scenario_light_block;
struct s_scenario_machine;
struct s_scenario_scenery;
struct s_scenario_sound_scenery;

struct s_scenario_vehicle :
	s_scenario_unit
{
};
static_assert(sizeof(s_scenario_vehicle) == sizeof(s_scenario_unit));

enum e_scenario_weapon_datum_flags
{
	_scenario_weapon_datum_initially_at_rest_bit = 0, // doesn't fall
	_scenario_weapon_datum_obsolete_bit,
	_scenario_weapon_datum_does_accelerate_bit, // moves due to explosions

	k_scenario_weapon_datum_flags
};

struct s_scenario_weapon_datum
{
	short rounds_left;
	short rounds_loaded;
	c_flags<e_scenario_weapon_datum_flags, dword, k_scenario_weapon_datum_flags> flags;
};
static_assert(sizeof(s_scenario_weapon_datum) == 0x8);

struct s_scenario_weapon :
	s_scenario_object
{
	s_scenario_weapon_datum weapon_data;
	s_scenario_multiplayer_object_properties multiplayer_data;
};
static_assert(sizeof(s_scenario_weapon) == 0xAC);

