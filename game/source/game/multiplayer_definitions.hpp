#pragma once

#include "tag_files/tag_groups.hpp"

struct s_multiplayer_universal_globals_definition;
struct s_multiplayer_runtime_globals_definition;
struct s_multiplayer_globals_definition
{
	c_typed_tag_block<s_multiplayer_universal_globals_definition> universal;
	c_typed_tag_block<s_multiplayer_runtime_globals_definition> runtime;
};
static_assert(sizeof(s_multiplayer_globals_definition) == 0x18);

struct s_multiplayer_customized_model_character;
struct s_multiplayer_equipment;
struct s_multiplayer_energy_regeneration;
struct s_multiplayer_weapon_selection;
struct s_multiplayer_vehicle_selection;
struct s_multiplayer_grenade_selection;
struct s_multiplayer_weapon_set;
struct s_multiplayer_vehicle_set;
struct s_multiplayer_podium_animation;
struct s_multiplayer_universal_globals_definition
{
	c_typed_tag_reference<'unic'> random_player_names;
	c_typed_tag_reference<'unic'> team_names;
	c_typed_tag_block<s_multiplayer_customized_model_character> customized_spartan_characters;
	c_typed_tag_block<s_multiplayer_customized_model_character> customized_elite_characters;
	c_typed_tag_block<s_multiplayer_equipment> equipment;
	c_typed_tag_block<s_multiplayer_energy_regeneration> energy_regeneration;
	c_typed_tag_reference<'unic'> multiplayer_text;
	c_typed_tag_reference<'unic'> sandbox_text;
	c_typed_tag_reference<'jmrq'> sandbox_object_properties_values;

	// Weapon Selections Halo3
	// Define the weapons you want to be available in multiplayer games here (does not include mp objects like ball or flag)
	c_typed_tag_block<s_multiplayer_weapon_selection> weapon_selections;

	// Vehicle Selections Halo3
	// Define the vehicles you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_vehicle_selection> vehicle_selections;

	// Grenade Selections Halo3
	// Define the grenades you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_grenade_selection> grenade_selections;

	// Weapon Sets Halo3
	// Define the weapon sets you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_weapon_set> weapon_sets;

	// Vehicle Sets Halo3
	// Define the vehicle sets you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_vehicle_set> vehicle_sets;

	c_typed_tag_block<s_multiplayer_podium_animation> podium_animations;

	// Game Engine Settings Halo3
	// This tag is only placeholder, until the Excel/XML setup system is up and running
	c_typed_tag_reference<'wezr'> halo3_game_engine_settings;
};
static_assert(sizeof(s_multiplayer_universal_globals_definition) == 0xD8);

struct s_multiplayer_customized_model_selection;
struct s_multiplayer_customized_model_character
{
	c_string_id armor_region;
	c_string_id biped_region;
	c_typed_tag_block<s_multiplayer_customized_model_selection> customized_areas;
};
static_assert(sizeof(s_multiplayer_customized_model_character) == 0x14);

struct s_multiplayer_customized_model_selection
{
	c_string_id selection_name;
	tag_reference third_person_armor_object;
	tag_reference first_person_armor_object;

	short __unknown24;
	byte __pad26[2];

	c_string_id parent_attach_marker;
	c_string_id child_attach_marker;
};
static_assert(sizeof(s_multiplayer_customized_model_selection) == 0x30);

struct s_multiplayer_equipment
{
	c_string_id name;
	tag_reference object;
	short type;
	byte __pad[2];
};
static_assert(sizeof(s_multiplayer_equipment) == 0x18);

struct s_multiplayer_energy_regeneration
{
	long duration;
	long energy_level;
};
static_assert(sizeof(s_multiplayer_energy_regeneration) == 0x8);

struct s_multiplayer_weapon_selection
{
	c_string_id name;

	// used only for random weapon set
	real random_weapon_set_weight;

	c_typed_tag_reference<'weap'> weapon_tag;
};
static_assert(sizeof(s_multiplayer_weapon_selection) == 0x18);

struct s_multiplayer_vehicle_selection
{
	c_string_id name;
	c_typed_tag_reference<'vehi'> vehicle_tag;
};
static_assert(sizeof(s_multiplayer_vehicle_selection) == 0x14);

struct s_multiplayer_grenade_selection
{
	c_string_id name;
	c_typed_tag_reference<'eqip'> grenade_tag;
};
static_assert(sizeof(s_multiplayer_grenade_selection) == 0x14);

struct s_multiplayer_object_remap_entry;
struct s_multiplayer_weapon_set
{
	c_string_id name;
	c_typed_tag_block<s_multiplayer_object_remap_entry> remap_table;
};
static_assert(sizeof(s_multiplayer_weapon_set) == 0x10);

struct s_multiplayer_vehicle_set
{
	c_string_id name;
	c_typed_tag_block<s_multiplayer_object_remap_entry> remap_table;
};
static_assert(sizeof(s_multiplayer_vehicle_set) == 0x10);

struct s_multiplayer_object_remap_entry
{
	c_string_id placed_object_name;
	c_string_id remapped_object_name;
};
static_assert(sizeof(s_multiplayer_object_remap_entry) == 0x8);

struct s_multiplayer_podium_stance_animation;
struct s_multiplayer_podium_move_animation;
struct s_multiplayer_podium_animation
{
	c_typed_tag_reference<'jmad'> animation_tag;
	c_string_id default_unarmed;
	c_string_id default_armed;
	c_typed_tag_block<s_multiplayer_podium_stance_animation> stance_animations;
	c_typed_tag_block<s_multiplayer_podium_move_animation> move_animations;
};
static_assert(sizeof(s_multiplayer_podium_animation) == 0x30);

struct s_multiplayer_podium_stance_animation
{
	string name;
	c_string_id base_animation;
	c_string_id loop_animation;
	c_string_id unarmed_transition;
	c_string_id armed_transition;
	real camera_distance_offset;
};
static_assert(sizeof(s_multiplayer_podium_stance_animation) == 0x34);

struct s_multiplayer_podium_move_animation
{
	string name;
	c_string_id in_animation;
	c_string_id loop_animation;
	c_string_id out_transition;
	real offset;
	c_typed_tag_reference<'weap'> primary_weapon;
	c_typed_tag_reference<'weap'> secondary_weapon;
};
static_assert(sizeof(s_multiplayer_podium_move_animation) == 0x50);

struct s_multiplayer_runtime_globals_definition
{
	c_typed_tag_reference<'bipd'> editor_biped;
	c_typed_tag_reference<'scen'> editor_helper;
	c_typed_tag_reference<'weap'> flag;
	c_typed_tag_reference<'weap'> ball;
	c_typed_tag_reference<'weap'> assault_bomb;
	c_typed_tag_reference<'bloc'> vip_influence_area;
	c_typed_tag_reference<'unic'> in_game_text;

	c_typed_tag_reference<'proj'> __reference70;
	c_typed_tag_reference<'effe'> __reference80;
	c_typed_tag_reference<'effe'> __reference90;

	// reach: megalo sounds (english)?
	c_typed_tag_reference<'snd!'> __referenceA0;

	// reach: communication sounds (comm english)?
	c_typed_tag_reference<'snd!'> __referenceB0;

	tag_block sounds;
	tag_block looping_sounds;
	tag_block earn_wp_events;
	tag_block general_events;
	tag_block flavor_events;
	tag_block slayer_events;
	tag_block ctf_events;
	tag_block oddball_events;
	tag_block king_events;
	tag_block vip_events;
	tag_block juggernaut_events;
	tag_block territories_events;
	tag_block assault_events;
	tag_block infection_events;

	long maximum_frag_count;
	long maximum_plasma_count;

	tag_block multiplayer_constants;
	tag_block state_responses;

	c_typed_tag_reference<'bitm'> scoreboard_emblem_bitmap;
	c_typed_tag_reference<'bitm'> scoreboard_dead_emblem_bitmap;

	c_typed_tag_reference<'rmhg'> hill_shader;

	c_typed_tag_reference<'chdt'> unused_intro_message;
	c_typed_tag_reference<'chdt'> ctf_intro_message;
	c_typed_tag_reference<'chdt'> slayer_intro_message;
	c_typed_tag_reference<'chdt'> oddball_intro_message;
	c_typed_tag_reference<'chdt'> king_intro_message;
	c_typed_tag_reference<'chdt'> sandbox_intro_message;
	c_typed_tag_reference<'chdt'> vip_intro_message;
	c_typed_tag_reference<'chdt'> juggernaut_intro_message;
	c_typed_tag_reference<'chdt'> territories_intro_message;
	c_typed_tag_reference<'chdt'> assault_intro_message;
	c_typed_tag_reference<'chdt'> infection_intro_message;

	c_typed_tag_reference<'lsnd'> menu_music_a;
	c_typed_tag_reference<'lsnd'> menu_music_b;
	c_typed_tag_reference<'lsnd'> menu_music_c;
	c_typed_tag_reference<'lsnd'> menu_music_d;
};
static_assert(sizeof(s_multiplayer_runtime_globals_definition) == 0x2A8);

extern s_multiplayer_runtime_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_runtime_data();
extern s_multiplayer_universal_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_universal_data();

extern s_multiplayer_universal_globals_definition* multiplayer_globals_try_and_get_universal_data(s_multiplayer_globals_definition* multiplayer_globals);

