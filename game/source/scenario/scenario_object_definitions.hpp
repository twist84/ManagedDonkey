#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

template<tag ...t_group_tags>
struct scenario_object_palette_entry
{
	c_typed_tag_reference<t_group_tags> name;

	byte GYFQQPUM[32]; // pad
};
static_assert(sizeof(scenario_object_palette_entry<'test'>) == 0x30);

template<tag ...t_group_tags>
struct scenario_object_palette_entry_with_string_id
{
	c_typed_tag_reference<t_group_tags> name;
	c_string_id display_name;
	long maximum_allowed;
	real price_per_instance;

	byte GYFQQPUM[20]; // pad
};
static_assert(sizeof(scenario_object_palette_entry_with_string_id<'test'>) == 0x30);

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