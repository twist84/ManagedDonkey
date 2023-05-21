#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

#include "ai/ai_globals.hpp"
#include "cutscene/cinematics_definitions.hpp"
#include "game/campaign_metagame_definitions.hpp"
#include "game/game_options.hpp"
#include "game/game_progression.hpp"
#include "game/materials_definitions.hpp"
#include "math/function_definitions.hpp"
#include "render/render_visibility_collection.hpp"
#include "sound/sound_definitions.hpp"
#include "tag_files/tag_groups.hpp"
#include "text/text_group.hpp"

struct game_globals_storage
{
	bool initializing;
	bool map_active;
	byte : 8;
	byte : 8;

	dword active_structure_bsp_mask;
	dword active_designer_zone_mask;
	dword active_cinematic_zone_mask;

	game_options options;

	// ODST
	dword active_game_progression_level; // string id
	s_campaign_game_progression active_game_progression;

	bool game_in_progress;

	bool game_lost;
	bool game_revert;
	bool prepare_for_game_progression; // false if `active_game_progression_level` is `_invalid_string_id`
	long game_lost_wait_time;

	bool game_finished;
	byte : 8;
	byte : 8;
	byte : 8;
	long game_finished_wait_time;

	c_flags<e_campaign_skulls_primary, dword, k_campaign_skulls_primary_count> active_primary_skulls;
	c_flags<e_campaign_skulls_secondary, dword, k_campaign_skulls_secondary_count> active_secondary_skulls;

	byte : 8;

	// game_frame
	// game_had_an_update_tick_this_frame
	bool update_tick_this_frame;

	byte : 8;
	byte : 8;

	s_game_cluster_bit_vectors cluster_pvs;
	s_game_cluster_bit_vectors cluster_pvs_local;
	s_game_cluster_bit_vectors cluster_activation;

	// game_pvs_enable_scripted_camera_pvs
	// game_pvs_clear_scripted_camera_pvs
	bool scripted_camera_pvs;
	byte : 8;

	// game_pvs_scripted_clear
	// game_pvs_scripted_set_object
	// game_pvs_scripted_set_camera_point
	word scripted;

	// game_pvs_scripted_set_object
	// game_pvs_scripted_set_camera_point
	// scenario_group, scenario_block, scenario_cutscene_camera_point_block, camera_point_index
	datum_index scripted_object_index; // scenario_group.scenario_block.cutscene_camera_points[camera_point_index]

	long game_ragdoll_count;

	byte __data25204[4];
};
static_assert(sizeof(game_globals_storage) == 0x25208);

extern game_globals_storage* game_globals_get();

extern long get_map_minor_version();

struct s_damage_globals_definition;
struct s_game_globals_camera;
struct s_game_globals_difficulty_information;
struct s_game_globals_falling_damage;
struct s_game_globals_grenade;
struct s_game_globals_havok_cleanup_resources;
struct s_game_globals_interface_tag_references;
struct s_game_globals_player_control;
struct s_game_globals_player_information;
struct s_game_globals_player_representation;
struct s_game_globals_shield_boost;
template<tag group_tag>
struct s_game_globals_tag_reference;
struct s_game_globals
{
	struct sound_block;

	// pad
	byte YJLTWDSL[0xAC];

	c_enum<e_language, long, _language_invalid, k_language_count> language;
	c_typed_tag_block<s_game_globals_havok_cleanup_resources> havok_cleanup_resources;
	c_typed_tag_block<s_sound_globals_definition> sound_globals;

	// AI globals
	// I have moved the ai globals out of this tag, and into its own tag which is referenced here.
	c_typed_tag_block<s_ai_globals_data, 'slap'> DEPRECATED; // ai globals;
	c_typed_tag_reference<'aigl'> ai_globals_ref;

	c_typed_tag_block<s_damage_globals_definition> damage_table;
	c_typed_tag_block<g_null_block> empty;
	c_typed_tag_block<sound_block> sounds;
	c_typed_tag_block<s_game_globals_camera> camera;
	c_typed_tag_block<s_game_globals_player_control> player_control;
	c_typed_tag_block<s_game_globals_difficulty_information> difficulty;
	c_typed_tag_block<s_game_globals_grenade> grenades;
	c_typed_tag_block<g_null_block> what;
	c_typed_tag_block<s_game_globals_interface_tag_references> interface_tags;

	// @weapon list (update _weapon_list enum in game_globals.h)
	c_typed_tag_block<s_game_globals_tag_reference<'weap'>> cheat_weapons;
	c_typed_tag_block<s_game_globals_tag_reference<'eqip'>> cheat_powerups;

	c_typed_tag_block<s_game_globals_player_information> player_information;
	c_typed_tag_block<s_game_globals_player_representation> player_representation;
	c_typed_tag_block<s_game_globals_player_representation> player_representation_debug;

	c_typed_tag_block<s_game_globals_falling_damage> falling_damage;
	c_typed_tag_block<s_game_globals_shield_boost> shield_boost;
	c_typed_tag_block<s_global_material_definition, 'sort'> materials;

	c_typed_tag_reference<'mulg'> multiplayer_globals;
	c_typed_tag_reference<'smdt'> survival_mode_globals;
	c_typed_tag_block<cinematics_globals_block> cinematics_globals;
	c_typed_tag_block<s_campaign_metagame_globals> campaign_metagame_globals;

	c_language_pack language_packs[k_language_count];

	// Rasterizer globals
	c_typed_tag_reference<'rasg'> rasterizer_globals_ref;

	// Default camera fx settings
	c_typed_tag_reference<'cfxs'> default_camera_fx_settings;

	// halo online
	c_typed_tag_reference<'pdm!'> podium_definition;

	// Default wind settings
	c_typed_tag_reference<'wind'> default_wind_settings;

	// Default collision damage
	c_typed_tag_reference<'jpt!'> collision_damage_effect;
	c_typed_tag_reference<'cddf'> collision_damage;

	// global water material
	// the default value for what material type water is
	c_string_id global_water_material; // 'sted'
	short global_water_material_type;

	// pad
	byte pad_water_material[2];

	c_typed_tag_reference<'effg'> effect_globals;
	c_typed_tag_reference<'gpdt'> game_progression;
	c_typed_tag_reference<'achi'> game_achievements;

	// halo online

	c_typed_tag_reference<'inpg'> input_globals;
	real __unknown5C0;
	real __unknown5C4;
	real biped_speed_reference;
	real vehicle_speed_reference;
	c_function_definition speed_to_maximum_camo;
	real __unknown5E8;
	real __unknown5EC;
	real __unknown5F0;
	real __unknown5F4;
	real __unknown5F8;
	s_tag_block damage_reporting_types;
	real __unknown604;

	struct sound_block
	{
		s_tag_reference sound; // OBSOLETE
	};
	static_assert(sizeof(sound_block) == sizeof(s_tag_reference));
};
static_assert(sizeof(s_game_globals) == 0x608);

struct s_game_globals_player_representation
{
	c_string_id name;
	char_enum model_choice;
	char_enum _class;
	byte __pad6[2];
	s_tag_reference first_person_hands;
	s_tag_reference first_person_body;
	s_tag_reference first_person_unit;
	c_string_id third_person_variant;
	s_tag_reference binoculars_zoom_in_sounds;
	s_tag_reference binoculars_zoom_out_sounds;
	s_tag_reference voice;
};
static_assert(sizeof(s_game_globals_player_representation) == 0x6C);

template<tag group_tag>
struct s_game_globals_tag_reference : s_tag_reference
{

};

