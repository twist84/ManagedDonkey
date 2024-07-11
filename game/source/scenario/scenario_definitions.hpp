#pragma once

#include "ai/ai_scenario_definitions.hpp"
#include "ai/cs_scenario_definitions.hpp"
#include "cseries/cseries.hpp"
#include "cutscene/recorded_animation_definitions.hpp"
#include "editor/editor_scenario_definitions.hpp"
#include "game/game_engine_spawn_influencer.hpp"
#include "hs/hs_scenario_definitions.hpp"
#include "scenario/scenario_cubemap_definitions.hpp"
#include "scenario/scenario_decorator_definitions.hpp"
#include "scenario/scenario_designer_zones_definitions.hpp"
#include "scenario/scenario_object_definitions.hpp"
#include "scenario/scenario_resource_definitions.hpp"
#include "scenario/scenario_trigger_volumes.hpp"
#include "scenario/scenario_zone_debugger_definitions.hpp"
#include "shell/shell.hpp"
#include "tag_files/tag_groups.hpp"
#include "units/units.hpp"

enum e_scenario_type
{
	_scenario_type_solo = 0,
	_scenario_type_multiplayer,
	_scenario_type_main_menu,
	_scenario_type_multiplayer_shared,
	_scenario_type_single_player_shared,

	k_scenario_type_count
};

enum e_scenario_campaign_type
{
	_scenario_campaign_type_unknown = 0,
	_scenario_campaign_type_hub,
	_scenario_campaign_type_linear,
	_scenario_campaign_type_scene,
	_scenario_campaign_type_cinematic,

	k_scenario_campaign_type_count
};

enum e_scenario_flags
{
	/* H2EK
	0	"cortana hack#sort cortana in front of other transparent geometry",
	1	"always draw sky#always draw sky 0, even if no +sky polygons are visible",
	2	"don't strip pathfinding#always leave pathfinding in, even for a multiplayer scenario",
	3	"symmetric multiplayer map",
	4	"quick loading (cinematic only) scenario",
	5	"characters use previous mission weapons",
	6	"lightmaps smooth palettes with neighbors",
	7	"snap to white at start"
	*/

	/* H3ODSTEK
	0	"don't strip pathfinding#always leave pathfinding in, even for a multiplayer scenario",
	1	"symmetric multiplayer map",
	2	"quick loading (cinematic only) scenario",
	3	"characters use previous mission weapons",
	4	"override globals!",
	5	"big vehicle use center point for light sampling",
	6	"don't use campaign sharing",
	7	"use eldo-like objects lighting sampling#sample from hi-poly poops",
	8	"survival",
	9	"force load objects in mode specific globals#forces attempt_to_load_missing_objects for MP and Survival"
	*/

	_scenario_flag_0_bit = 0,
	_scenario_flag_1_bit,
	_scenario_flag_2_bit,
	_scenario_flag_3_bit,
	_scenario_flag_4_bit,

	// TagTool has this here, making its index the same as H2EK
	_scenario_characters_use_previous_mission_weapons_bit,

	k_scenario_flags
};

struct ai_scenario_mission_dialogue;
struct ai_scene;
struct scenario_device_group;
struct editor_comment_definition;
struct editor_scenario_data_definition;
struct orders_definition;
struct pathfinding_data;
struct s_ai_reference_frame_definition;
struct s_campaign_metagame_scenario;
struct s_cinematic_reference;
struct s_flock_instance;
struct s_flock_palette_entry;
struct s_game_audibility;
struct s_game_globals_player_representation;
struct s_objective;
struct s_scenario_acoustics_palette_entry;
struct s_scenario_acoustics_volume;
struct s_scenario_airprobe_info;
struct s_scenario_budget_reference;
struct s_scenario_cinematic_lighting_palette_entry;
struct s_scenario_cluster_data;
struct s_scenario_cortana_effect;
struct s_scenario_cutscene_title;
struct s_scenario_interpolator;
struct s_scenario_kill_trigger_volume;
struct s_scenario_lighting_zone_set;
struct s_scenario_safe_zone_trigger_volume;
struct s_scenario_sky_reference;
struct s_scenario_soft_ceiling;
struct s_scenario_spawn_data;
struct s_scenario_zone_set;
struct s_scenario_zone_set_pvs;
struct s_scenario_zone_set_switch_trigger_volume;
struct s_soft_surfaces_definition;
struct s_squad_definition;
struct s_squad_patrol_definition;
struct scenario_campaign_player_representation_names_block;
struct scenario_cutscene_camera_point;
struct scenario_cutscene_flag;
struct scenario_decal;
struct scenario_decal_palette_entry;
struct scenario_detail_object_collection_palette_entry;
struct scenario_object_name;
struct scenario_player;
struct scenario_starting_profile;
struct scenario_structure_bsp_reference;
struct squad_group_definition;
struct structure_atmosphere_palette_entry;
struct structure_background_sound_palette_entry;
struct structure_camera_fx_palette_entry;
struct structure_sound_environment_palette_entry;
struct structure_weather_palette_entry;
struct style_palette_entry;
struct trigger_definition;
struct user_hint_data;
struct zone_definition;
struct s_background_bitmap_reference_definition;
struct s_scenario
{
	static tag const k_group_tag = SCENARIO_TAG;

	c_enum<e_scenario_type, char, _scenario_type_solo, k_scenario_type_count> type;
	c_enum<e_scenario_campaign_type, char, _scenario_campaign_type_unknown, k_scenario_campaign_type_count> campaign_type;
	c_flags<e_scenario_flags, word, k_scenario_flags> flags;

	long campaign_id;
	long map_id;

	angle local_north;
	real sandbox_budget;

	c_typed_tag_block<scenario_structure_bsp_reference> structure_bsps;
	c_typed_tag_reference<STRUCTURE_SEAMS_TAG> structure_seams;
	c_typed_tag_block<s_scenario_sky_reference> sky_references;
	c_typed_tag_block<s_scenario_zone_set_pvs> zone_set_pvs;
	c_typed_tag_block<s_game_audibility> zone_set_audibility;
	c_typed_tag_block<s_scenario_zone_set> zone_sets;
	c_typed_tag_block<s_scenario_lighting_zone_set> lighting_zone_sets;

	// campaign players
	// specify 4 names for the 4 coop players in campaign.
	// These names will be used to match with names of player representations in globals.globals
	c_typed_tag_block<scenario_campaign_player_representation_names_block> campaign_players;

	s_tag_block __unknown78;
	s_tag_block __unknown84;
	s_tag_block __unknown90;

	c_typed_tag_data<editor_scenario_data_definition> editor_scenario_data;
	c_typed_tag_block<editor_comment_definition> comments;
	c_typed_tag_block<scenario_object_name, 'sort'> object_names;

	c_typed_tag_block<s_scenario_scenery> scenery;
	c_typed_tag_block<scenario_object_palette_entry<SCENERY_TAG>, 'sort'> scenery_palette;

	c_typed_tag_block<s_scenario_biped> bipeds;
	c_typed_tag_block<scenario_object_palette_entry<BIPED_TAG>, 'sort'> biped_palette;

	c_typed_tag_block<s_scenario_vehicle> vehicles;
	c_typed_tag_block<scenario_object_palette_entry<VEHICLE_TAG>, 'sort'> vehicle_palette;

	c_typed_tag_block<s_scenario_equipment> equipment;
	c_typed_tag_block<scenario_object_palette_entry<EQUIPMENT_TAG>, 'sort'> equipment_palette;

	c_typed_tag_block<s_scenario_weapon> weapons;
	c_typed_tag_block<scenario_object_palette_entry<WEAPON_TAG>, 'sort'> weapon_palette;

	c_typed_tag_block<scenario_device_group> device_groups;

	c_typed_tag_block<s_scenario_machine> machines;
	c_typed_tag_block<scenario_object_palette_entry<DEVICE_MACHINE_TAG>, 'sort'> machine_palette;

	c_typed_tag_block<s_scenario_terminal> terminals;
	c_typed_tag_block<scenario_object_palette_entry<DEVICE_TERMINAL_TAG>, 'sort'> terminal_palette;

	c_typed_tag_block<s_scenario_arg_device> arg_devices;
	c_typed_tag_block<scenario_object_palette_entry<DEVICE_ARG_DEVICE_TAG>, 'sort'> arg_device_palette;

	c_typed_tag_block<s_scenario_control> controls;
	c_typed_tag_block<scenario_object_palette_entry<DEVICE_CONTROL_TAG>, 'sort'> control_palette;

	c_typed_tag_block<s_scenario_sound_scenery> sound_scenery;
	c_typed_tag_block<scenario_object_palette_entry<SOUND_SCENERY_TAG>, 'sort'> sound_scenery_palette;

	c_typed_tag_block<s_scenario_giant> giants;
	c_typed_tag_block<scenario_object_palette_entry<GIANT_TAG>, 'sort'> giant_palette;

	c_typed_tag_block<s_scenario_effect_scenery> effect_scenery;
	c_typed_tag_block<scenario_object_palette_entry<EFFECT_SCENERY_TAG>, 'sort'> effect_scenery_palette;

	c_typed_tag_block<scenario_light_block> light_volumes;
	c_typed_tag_block<scenario_object_palette_entry<LIGHT_TAG>, 'sort'> light_volume_palette;

	c_typed_tag_block<scenario_object_palette_entry_with_string_id<VEHICLE_TAG>, 'sort'> map_variant_vehicle_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id<WEAPON_TAG>, 'sort'> map_variant_weapon_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id<EQUIPMENT_TAG>, 'sort'> map_variant_equipment_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id<SCENERY_TAG>, 'sort'> map_variant_scenery_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id<OBJECT_TAG, VEHICLE_TAG, SCENERY_TAG, DEVICE_ARG_DEVICE_TAG>, 'sort'> map_variant_teleporters_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id<OBJECT_TAG, VEHICLE_TAG, SCENERY_TAG, DEVICE_ARG_DEVICE_TAG>, 'sort'> map_variant_goals_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id<OBJECT_TAG, VEHICLE_TAG, SCENERY_TAG, DEVICE_ARG_DEVICE_TAG>, 'sort'> map_variant_spawners_palette;

	c_typed_tag_block<s_scenario_soft_ceiling> soft_ceilings;
	c_typed_tag_block<scenario_starting_profile> player_starting_profile;
	c_typed_tag_block<scenario_player> player_starting_locations;
	c_typed_tag_block<scenario_trigger_volume> trigger_volumes;
	c_typed_tag_block<recorded_animation_definition> recorded_animations;
	c_typed_tag_block<s_scenario_zone_set_switch_trigger_volume> zone_set_trigger_volumes;

	// SPAWN INFLUENCE OVERRIDES
	// You can use the following to override multiplayer global spawn influencers for the scenario

	c_typed_tag_block<s_player_spawn_influence> enemy_forbid_influence;
	c_typed_tag_block<s_player_spawn_influence> enemy_bias_influence;
	c_typed_tag_block<s_player_spawn_influence> ally_bias_influence;
	c_typed_tag_block<s_player_spawn_influence> selected_ally_bias_influence;
	c_typed_tag_block<s_player_spawn_influence> dead_teammate_influence;

	c_typed_tag_block<s_weapon_spawn_influence> weapon_spawn_influencers;
	c_typed_tag_block<s_vehicle_spawn_influence> vehicle_spawn_influencers;
	c_typed_tag_block<s_projectile_spawn_influence> projectile_spawn_influencers;
	c_typed_tag_block<s_equipment_spawn_influence> equipment_spawn_influencers;

	c_typed_tag_block<s_netgame_goal_spawn_influence> koth_hill_influencer;
	c_typed_tag_block<s_netgame_goal_spawn_influence> oddball_influencer;
	c_typed_tag_block<s_netgame_goal_spawn_influence> ctf_flag_away_influencer;
	c_typed_tag_block<s_netgame_goal_spawn_influence> territories_ally_influencer;
	c_typed_tag_block<s_netgame_goal_spawn_influence> territories_enemy_influencer;
	c_typed_tag_block<s_netgame_goal_spawn_influence> infection_safe_zone_human_influencer;
	c_typed_tag_block<s_netgame_goal_spawn_influence> infection_safe_zone_zombie_influencer;
	c_typed_tag_block<s_netgame_goal_spawn_influence> vip_influencer;

	// RENDER FLUFF
	// Pretty

	c_typed_tag_block<scenario_decal> decals;
	c_typed_tag_block<scenario_decal_palette_entry, 'sort'> decal_palette;
	c_typed_tag_block<scenario_detail_object_collection_palette_entry> detail_object_collection_palette;
	c_typed_tag_block<style_palette_entry, 'sort'> style_palette;
	c_typed_tag_block<squad_group_definition> squad_groups;
	c_typed_tag_block<s_squad_definition> squads;
	c_typed_tag_block<zone_definition> zones;
	c_typed_tag_block<s_squad_patrol_definition> squad_patrols;
	c_typed_tag_block<ai_scene> mission_scenes;
	c_typed_tag_block<character_palette_entry, 'sort'> character_palette;
	c_typed_tag_block<pathfinding_data> ai_pathfinding_data;
	c_typed_tag_block<user_hint_data> ai_user_hint_data;
	c_typed_tag_block<ai_recording_reference_definition> ai_recording_references;

	c_typed_tag_data<hs_string_data_definition> script_string_data;
	c_typed_tag_block<hs_script> scripts;
	c_typed_tag_block<hs_global_internal> globals;
	c_typed_tag_block<hs_tag_reference> references;
	c_typed_tag_block<hs_source_file> source_files;
	c_typed_tag_block<cs_script_data> scripting_data;

	c_typed_tag_block<scenario_cutscene_flag> cutscene_flags;
	c_typed_tag_block<scenario_cutscene_camera_point> cutscene_camera_points;
	c_typed_tag_block<s_scenario_cutscene_title> cutscene_titles;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> custom_object_names;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> chapter_title_text;

	c_typed_tag_block<scenario_resources_definition> scenario_resources;
	c_typed_tag_block<s_hs_unit_seat_mapping> hs_unit_seats;
	c_typed_tag_block<s_scenario_kill_trigger_volume> scenario_kill_triggers;
	c_typed_tag_block<s_scenario_safe_zone_trigger_volume> scenario_safe_zone_triggers;
	c_typed_tag_block<hs_syntax_node> hs_syntax_datums;
	c_typed_tag_block<orders_definition> orders;
	c_typed_tag_block<trigger_definition> triggers;

	c_typed_tag_block<s_scenario_acoustics_palette_entry, 'sort'> acoustics_palette; // background_sound_palette
	c_typed_tag_block<structure_background_sound_palette_entry, 'sort'> old_background_sound_palette;
	c_typed_tag_block<structure_sound_environment_palette_entry, 'sort'> sound_environment_palette;

	c_typed_tag_block<structure_weather_palette_entry, 'sort'> weather_palette;
	c_typed_tag_block<structure_atmosphere_palette_entry, 'sort'> atmosphere;
	c_typed_tag_block<structure_camera_fx_palette_entry, 'sort'> camera_fx_palette;

	c_typed_tag_block<g_null_block> unused2;
	c_typed_tag_block<g_null_block> unused3;
	c_typed_tag_block<g_null_block> unused4;

	c_typed_tag_block<s_scenario_cluster_data> scenario_cluster_data;
	c_typed_tag_block<s_scenario_acoustics_volume> acoustic_spaces;

	struct // object_salt_storage_array
	{
		long salt;
	} object_salts[32];

	c_typed_tag_block<s_scenario_spawn_data> spawn_data;
	c_typed_tag_reference<SOUND_EFFECT_COLLECTION_TAG> sound_effect_collection;

	c_typed_tag_block<s_scenario_crate> crates;
	c_typed_tag_block<scenario_object_palette_entry<CRATE_TAG>, 'sort'> crate_palette;

	c_typed_tag_block<s_flock_palette_entry, 'sort'> flock_palette;
	c_typed_tag_block<s_flock_instance> flocks;

	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> subtitles;

	c_typed_tag_block<s_scenario_creature> creatures;
	c_typed_tag_block<scenario_object_palette_entry<CREATURE_TAG>, 'sort'> creature_palette;

	c_typed_tag_block<s_scenario_editor_folder> editor_folders;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> game_engine_strings;

	byte QVUC[0x8]; // pad

	c_typed_tag_block<ai_scenario_mission_dialogue> mission_dialogue;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> objectives;

	c_typed_tag_block<s_scenario_interpolator> interpolators;

	s_tag_block shared_references;
	s_tag_block screen_effect_references;
	s_tag_block simulation_definition_table;

	c_typed_tag_reference<CAMERA_FX_SETTINGS_TAG> camera_effects;

	// halo online
	c_typed_tag_reference<PODIUM_SETTINGS_TAG> podium_override;

	// ignores the falloff curves
	c_typed_tag_reference<AREA_SCREEN_EFFECT_TAG> global_screen_effect;

	c_typed_tag_reference<SKY_ATM_PARAMETERS_TAG> atmospheric;
	c_typed_tag_reference<CHOCOLATE_MOUNTAIN_NEW_TAG> chocalate_mountain;
	c_typed_tag_reference<SCENARIO_LIGHTMAP_TAG> new_lightmaps;
	c_typed_tag_reference<PERFORMANCE_THROTTLES_TAG> performance_throttles;

	c_typed_tag_block<s_ai_reference_frame_definition> reference_frames;
	c_typed_tag_block<s_objective> ai_objectives;
	c_typed_tag_block<s_scenario_designer_zone> designer_zones;
	c_typed_tag_block<s_scenario_zone_debugger_definition> zone_debugger;
	c_typed_tag_block<s_scenario_new_decorator_definition> decorators;
	c_typed_tag_block<s_cinematic_reference> cinematics;
	c_typed_tag_block<s_scenario_cinematic_lighting_palette_entry> cinematic_lighting_palette;
	c_typed_tag_block<s_game_globals_player_representation> override_player_representations;
	c_typed_tag_block<s_campaign_metagame_scenario> campaign_metagame;
	c_typed_tag_block<s_soft_surfaces_definition> soft_surfaces;
	c_typed_tag_block<s_structure_cluster_cubemap_info> cubemaps;
	c_typed_tag_block<s_scenario_cortana_effect> cortana_effects;
	c_typed_tag_block<s_scenario_airprobe_info> airprobes;
	c_typed_tag_block<s_scenario_budget_reference> budget_references;

	// HACK
	c_typed_tag_reference<EFFECT_TAG> tchou_shader_effect;

	c_typed_tag_reference<VISION_MODE_TAG> vision_mode_override;
	c_typed_tag_block<s_background_bitmap_reference_definition> background_bitmap_references;
	
	void update_reference_names();
};
static_assert(sizeof(s_scenario) == 0x824);

enum e_scenario_structure_size
{
	_scenario_structure_size_1024x1024 = 0,
	_scenario_structure_size_768x768,
	_scenario_structure_size_512x512,
	_scenario_structure_size_153x153,
	_scenario_structure_size_256x256,
	_scenario_structure_size_384x384,

	k_scenario_structure_size_count
};

enum e_scenario_structure_bsp_reference_flags
{
	_scenario_structure_bsp_reference_flag_default_sky_enabled_bit,
	_scenario_structure_bsp_reference_flag_do_not_compress_lightmaps_bit,
	_scenario_structure_bsp_reference_flag_generate_fake_small_lightmaps_bit,
	_scenario_structure_bsp_reference_flag_ray_trace_adjacent_bsps_on_sky_hits_bit,
	_scenario_structure_bsp_reference_flag_lightmaps_use_conservative_subcharts_bit,
	_scenario_structure_bsp_reference_flag_lightmaps_reduce_stretch_hack_bit,

	// very_slow
	_scenario_structure_bsp_reference_flag_lightmaps_use_extended_gathering_bit,

	_scenario_structure_bsp_reference_flag_lightmaps_final_gather_ignores_backfacing_hits_bit,
	_scenario_structure_bsp_reference_flag_lightmaps_use_more_samples_for_large_sky_lights_bit,

	// incredibly_slow
	_scenario_structure_bsp_reference_flag_lightmaps_use_more_precise_extended_gathering_bit,

	_scenario_structure_bsp_reference_flag_no_ai_attachment_bit,

	// check_this_on_shared_BSPs
	_scenario_structure_bsp_reference_flag_not_a_normally_playable_space_in_an_mp_map_bit,

	k_scenario_structure_bsp_reference_flags
};

struct scenario_structure_bsp_reference
{
	c_typed_tag_reference<SCENARIO_STRUCTURE_BSP_TAG> structure_bsp;
	c_typed_tag_reference<STRUCTURE_DESIGN_TAG> structure_design;
	c_typed_tag_reference<SCENARIO_STRUCTURE_LIGHTING_INFO_TAG> structure_lighting_info;

	// Size Class
	// Tells lightmapper desired res for structure bitmaps.
	// Numbers in parens are final sizes after compression
	c_enum<e_scenario_structure_size, long, _scenario_structure_size_1024x1024, k_scenario_structure_size_count> size_class;

	real hacky_ambient_min_luminance;
	real direct_or_draft_ambient_min_luminance;

	// this is the most that we can sink a soft surface link snow in the structure_bsp via vertex painting.
	real structure_vertex_sink;

	c_flags<e_scenario_structure_bsp_reference_flags, word, k_scenario_structure_bsp_reference_flags> flags;

	// s_scenario_sky_reference
	short default_sky; // short_block_index

	// >end, pixel size we start stippling out. code default 36
	short default_instance_fade_start_pixels;

	// <start, pixel size where we no longer render. code default 30
	short default_instance_fade_end_pixels;

	c_typed_tag_reference<BITMAP_TAG> cubemap_bitmap_group_reference;
	c_typed_tag_reference<WIND_TAG> wind;

	// Clones
	// Describes which other bsps are physical 'clones' of this bsp
	// This is used to determine how to attach 'position-only' elements, like decorators, to the bsps:
	// Each clone gets a separate copy of decorators that are in both.
	// Non-cloned bsps cannot split decorators this way - the decorator will be given to the lowest numbered bsp
	dword_flags clone_bsp_flags; // long_block_flags
	
	void update_reference_names();
};
static_assert(sizeof(scenario_structure_bsp_reference) == 0x6C);

struct s_scenario_sky_reference
{
	c_typed_tag_reference<SCENERY_TAG> sky;

	// scenario_object_name
	short name;                // short_block_index

	// scenario_structure_bsp_reference_block
	word_flags active_on_bsps; // word_block_flags
	
	void update_reference_names();
};
static_assert(sizeof(s_scenario_sky_reference) == 0x14);

enum e_scenario_zone_set_flags
{
	_scenario_zone_set_begin_loading_next_level_bit = 0,
	_scenario_zone_set_debug_purposes_only_bit,
	_scenario_zone_set_interal_zone_set_bit,

	k_scenario_zone_set_flags
};

struct s_scenario_zone_set
{
	c_string_id name;
	long pvs_index;                       // long_block_index
	c_flags<e_scenario_zone_set_flags, dword, k_scenario_zone_set_flags> flags;
	dword_flags bsp_zone_flags;           // long_block_flags

	// designer_zone_flags
	dword_flags required_designer_zones;  // long_block_flags

	dword_flags forbidden_designer_zones; // long_block_flags
	dword_flags cinematic_zones;          // long_block_flags
	long hint_previous_zone_set;          // long_block_index
	long audibility_index;                // long_block_index
};
static_assert(sizeof(s_scenario_zone_set) == 0x24);

struct s_scenario_cinematic_lighting_palette_entry
{
	c_string_id name;
	c_typed_tag_reference<NEW_CINEMATIC_LIGHTING_TAG> cinematic_lighting_tag;
};
static_assert(sizeof(s_scenario_cinematic_lighting_palette_entry) == 0x14);

struct s_scenario_budget_reference
{
	c_typed_tag_reference<MODEL_ANIMATION_GRAPH_TAG, SOUND_LOOPING_TAG, SOUND_TAG> reference;
};
static_assert(sizeof(s_scenario_budget_reference) == sizeof(s_tag_reference));

struct s_scenario_lighting_zone_set
{
	c_string_id name;

	// scenario_structure_bsp_reference
	long rendered_bsp_flags; // long_block_flags

	// scenario_structure_bsp_reference
	long extra_bsp_flags; // long_block_flags
};
static_assert(sizeof(s_scenario_lighting_zone_set) == 0xC);

struct scenario_campaign_player_representation_names_block
{
	c_string_id name;
};
static_assert(sizeof(scenario_campaign_player_representation_names_block) == 0x4);

struct scenario_object_name
{
	c_static_string<k_tag_string_length> name;
	short object_type; // short_block_index_custom_search
	short scenario_datum_index; // short_block_index_custom_search
};
static_assert(sizeof(scenario_object_name) == 0x24);

enum e_scenario_soft_ceiling_flags
{
	_scenario_soft_ceiling_flag_ignore_bipeds_bit = 0,
	_scenario_soft_ceiling_flag_ignore_vehicles_bit,
	_scenario_soft_ceiling_flag_ignore_camera_bit,
	_scenario_soft_ceiling_flag_ignore_huge_vehicles_bit,

	k_scenario_soft_ceiling_flags
};

enum e_soft_ceiling_type
{
	_soft_ceiling_type_acceleration = 0,
	_soft_ceiling_type_soft_kill,
	_soft_ceiling_type_slip_surface,

	k_soft_ceiling_type_count
};

struct s_scenario_soft_ceiling
{
	c_flags<e_scenario_soft_ceiling_flags, word, k_scenario_soft_ceiling_flags> flags;
	c_flags<e_scenario_soft_ceiling_flags, word, k_scenario_soft_ceiling_flags> runtime_flags;
	c_string_id name;
	c_enum<e_soft_ceiling_type, short, _soft_ceiling_type_acceleration, k_soft_ceiling_type_count> type;

	// pad
	byte my_name_is[2];
};
static_assert(sizeof(s_scenario_soft_ceiling) == 0xC);

struct starting_weapon_info
{
	c_typed_tag_reference<WEAPON_TAG> weapon;
	short rounds_loaded;
	short rounds_total;
};
static_assert(sizeof(starting_weapon_info) == 0x14);

struct scenario_starting_profile
{
	c_static_string<k_tag_string_length> name;

	// [0,1]
	real_fraction starting_health_damage;

	// [0,1]
	real_fraction starting_shield_damage;

	// multiplayer_globals->universal[0]->weapon_selections
	starting_weapon_info starting_weapons[k_weapon_set_count];

	// multiplayer_globals->universal[0]->equipment
	short starting_equipment[4]; // short_block_index

	// multiplayer_globals->universal[0]->grenade_selections
	char starting_grenades[4]; // short_block_index
	
	// s_scenario_editor_folder
	short editor_folder; // short_block_index

	// pad
	byte AHDVHJE[2];

	void update_reference_names();
};
static_assert(sizeof(scenario_starting_profile) == 0x60);

enum e_scenario_player_flags
{
	_scenario_player_flag_survival_mode_bit = 0,

	k_scenario_player_flags
};

struct scenario_player
{
	real_point3d position;
	angle facing; // degrees
	angle pitch; // degrees
	short insertion_point_index;
	c_flags<e_scenario_player_flags, word, k_scenario_player_flags> flags;

	// s_scenario_editor_folder
	short editor_folder; // short_block_index

	// pad
	byte ANDYNDGE[2];
};
static_assert(sizeof(scenario_player) == 0x1C);

struct squad_group_definition
{
	c_static_string<k_tag_string_length> name;
	short parent;
	short initial_objective;

	// pad
	byte soon_to_be_an_initial_task[0x2];

	short editor_folder;
};
static_assert(sizeof(squad_group_definition) == 0x28);

struct s_squad_definition_internal
{
	s_tag_block cells;
};
static_assert(sizeof(s_squad_definition_internal) == sizeof(s_tag_block));

struct s_squad_definition
{
	c_static_string<k_tag_string_length> name;
	dword_flags flags;
	c_enum<e_campaign_team, short, _campaign_team_default, k_number_of_campaign_teams> team;
	short parent;
	short initial_zone;
	short initial_objective;
	short initial_task;
	short editor_folder;
	s_tag_block spawn_formations;
	s_tag_block spawn_points;
	word_flags excluded_placement_flags;

	// pad
	byte post_excluded_placement_flags[0x2];

	long squad_template_index;

	// Merge
	s_squad_definition_internal designer;
	s_squad_definition_internal templated;
};
static_assert(sizeof(s_squad_definition) == 0x68);

struct area_definition
{
	// Generate
	// Clear Auto
	// Clear Manual

	c_static_string<k_tag_string_length> name;
	dword_flags area_flags;

	// c_ai_point3d
	real_point3d runtime_relative_mean_point;
	short runtime_reference_frame;
	short structure_index;

	real runtime_standard_deviation;
	short runtime_starting_index;
	short runtime_count;
	short manual_reference_frame;
	short bsp_index;
	c_static_flags<256> cluster_occupancy;
	s_tag_block flight_hints;
	s_tag_block points;


	// Generation Properties

	short preset;
	byte PAD1[2];
	dword_flags flags;
	real extrusion;
	real sink;
	angle firing_point_orientation;
	angle grid_orientation;
	real spacing;
	real airborne_spacing;
	real min_cover_length;
	real cover_spacing;
	real cover_offset_distance;
	real too_close_distance;
};
static_assert(sizeof(area_definition) == 0xA8);

struct zone_definition
{
	c_static_string<k_tag_string_length> name;
	word_flags flags;
	word_flags runtime_bsp_flags;
	s_tag_block firing_positions;
	c_typed_tag_block<area_definition> areas;
};
static_assert(sizeof(zone_definition) == 0x3C);

struct scenario_cutscene_flag
{
	// pad
	byte MMNGQBXC[0x4];

	c_string_id name;
	real_point3d position;
	euler_angles2d facing;
	short editor_folder;

	// pad
	byte IWERHADF[0x2];
};
static_assert(sizeof(scenario_cutscene_flag) == 0x20);

enum e_scenario_camera_flags
{
	_scenario_camera_flag_edit_as_relative_bit = 0,
	_scenario_camera_flag_bit1,
	_scenario_camera_flag_bit2,

	k_scenario_camera_flags
};

enum e_scenario_camera_type
{
	_scenario_camera_type_normal = 0,
	_scenario_camera_type_ignore_target_orientation,
	_scenario_camera_type_dolly,
	_scenario_camera_type_ignore_target_updates,

	k_scenario_camera_type_count
};

struct scenario_cutscene_camera_point
{
	c_flags<e_scenario_camera_flags, word, k_scenario_camera_flags> flags;
	c_enum<e_scenario_camera_type, short, _scenario_camera_type_normal, k_scenario_camera_type_count> type;
	c_static_string<k_tag_string_length> name;

	// pad
	byte pad[0x4];

	real_point3d position;
	euler_angles3d orientation;
};
static_assert(sizeof(scenario_cutscene_camera_point) == 0x40);

enum e_text_justification
{
	_text_justification_left = 0,
	_text_justification_right,
	_text_justification_center,

	k_text_justification_count
};

enum e_text_vertical_justification
{
	_text_vertical_justification_default = 0,
	_text_vertical_justification_top,
	_text_vertical_justification_center,
	_text_vertical_justification_bottom,

	k_text_vertical_justification_count
};

enum e_font_id
{
	_font_id_terminal_font = 0,
	_font_id_body_text_font,
	_font_id_title_font,
	_font_id_super_large_font,
	_font_id_large_body_text_font,
	_font_id_split_screen_hud_message_font,
	_font_id_full_screen_hud_message_font,
	_font_id_english_body_text_font,
	_font_id_hud_number_font,
	_font_id_subtitle_font,
	_font_id_main_menu_font,

	k_font_id_count
};

struct s_scenario_cutscene_title
{
	c_string_id name;
	short_rectangle2d text_bounds; // on screen
	c_enum<e_text_justification, short, _text_justification_left, k_text_justification_count> justification;
	c_enum<e_text_vertical_justification, short, _text_vertical_justification_default, k_text_vertical_justification_count > vertical_justification;
	c_enum<e_font_id, short, _font_id_terminal_font, k_font_id_count> font;

	// pad
	byte padding[0x2];

	rgb_color text_color;
	rgb_color shadow_color;
	real fade_in_time; // seconds
	real up_time; // seconds
	real fade_out_time; // seconds
};
static_assert(sizeof(s_scenario_cutscene_title) == 0x28);

struct s_background_bitmap_reference_definition
{
	c_typed_tag_reference<BITMAP_TAG> background_bitmap_group_reference;

	void update_reference_names();
};
static_assert(sizeof(s_background_bitmap_reference_definition) == sizeof(s_tag_reference));

struct scenario_decal_palette_entry
{
	c_typed_tag_reference<DECAL_SYSTEM_TAG> reference;
};
static_assert(sizeof(scenario_decal_palette_entry) == sizeof(s_tag_reference));

extern long __cdecl scenario_cinematic_lighting_palette_entry_get_by_name(s_scenario const* scenario, string_id name);

