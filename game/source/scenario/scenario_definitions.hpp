#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

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

	k_scenario_flag_count
};

struct scenario_structure_bsp_reference;
struct s_scenario_sky_reference;
struct s_scenario_zone_set_pvs;
struct s_game_audibility;
struct s_scenario_zone_set;
struct s_scenario_lighting_zone_set;
struct scenario_campaign_player_representation_names_block;
struct editor_scenario_data_definition;
struct editor_comment_definition;
struct scenario_object_name;
struct scenario_object_palette_entry;
struct scenario_scenery_block;
struct scenario_biped_block;
struct scenario_vehicle_block;
struct scenario_equipment_block;
struct scenario_weapon_block;
struct device_group_block;
struct scenario_machine_block;
struct scenario_terminal_block;
struct scenario_arg_device_block;
struct scenario_control_block;
struct scenario_sound_scenery_block;
struct scenario_giant_block;
struct scenario_effect_scenery_block;
struct scenario_light_block;
struct scenario_object_palette_entry_with_string_id;
struct s_scenario_soft_ceiling;
struct scenario_starting_profile;
struct scenario_player;
struct scenario_trigger_volume_block;
struct recorded_animation_definition;
struct s_scenario_zone_set_switch_trigger_volume;
struct s_player_spawn_influence;
struct s_weapon_spawn_influence;
struct s_vehicle_spawn_influence;
struct s_projectile_spawn_influence;
struct s_equipment_spawn_influence;
struct s_netgame_goal_spawn_influence;
struct scenario_decal;
struct scenario_decal_palette_entry;
struct scenario_detail_object_collection_palette_entry;
struct style_palette_entry;
struct squad_group_definition;
struct s_squad_definition;
struct zone_definition;
struct s_squad_patrol_definition;
struct ai_scene;
struct character_palette_entry;
struct pathfinding_data;
struct user_hint_data;
struct ai_recording_reference_definition;
struct hs_string_data_definition;
struct hs_script;
struct hs_global_internal;
struct hs_tag_reference;
struct hs_source_file;
struct cs_script_data;
struct scenario_cutscene_flag;
struct scenario_cutscene_camera_point;
struct s_scenario_cutscene_title;
struct scenario_resources_definition;
struct s_hs_unit_seat_mapping;
struct s_scenario_kill_trigger_volume;
struct s_scenario_safe_zone_trigger_volume;
struct hs_syntax_node;
struct orders_definition;
struct trigger_definition;
struct s_scenario_acoustics_palette_entry;
struct structure_background_sound_palette_entry;
struct structure_sound_environment_palette_entry;
struct structure_weather_palette_entry;
struct structure_atmosphere_palette_entry;
struct structure_camera_fx_palette_entry;
struct g_null_block;
struct g_null_block;
struct g_null_block;
struct s_scenario_cluster_data;
struct s_scenario_acoustics_volume;
struct s_scenario_spawn_data;
struct s_scenario_crate;
struct s_flock_palette_entry;
struct s_flock_instance;
struct s_scenario_creature;
struct s_scenario_editor_folder;
struct ai_scenario_mission_dialogue;
struct s_scenario_interpolator;
struct s_ai_reference_frame_definition;
struct s_objective;
struct s_scenario_designer_zone;
struct s_scenario_zone_debugger_definition;
struct s_scenario_new_decorator_definition;
struct s_cinematic_reference;
struct s_scenario_cinematic_lighting_palette_entry;
struct s_game_globals_player_representation;
struct s_campaign_metagame_scenario;
struct s_soft_surfaces_definition;
struct s_scenario_cubemap_info;
struct s_scenario_cortana_effect;
struct s_scenario_airprobe_info;
struct s_scenario_budget_reference;
struct s_effect_gpu_data;

struct scenario
{
	c_enum<e_scenario_type, char, k_scenario_type_count> type;
	c_enum<e_scenario_campaign_type, char, k_scenario_campaign_type_count> campaign_type;
	c_flags<e_scenario_flags, word, k_scenario_flag_count> flags;

	long campaign_id;
	long map_id;

	angle local_north;
	real sandbox_budget;

	c_typed_tag_block<scenario_structure_bsp_reference> structure_bsps;

	//tag_reference scenario_pda;
	//tag_block pda_definitions;

	c_typed_tag_reference<'stse'> structure_seams;
	c_typed_tag_block<s_scenario_sky_reference> sky_references;
	c_typed_tag_block<s_scenario_zone_set_pvs> zone_set_pvs;
	c_typed_tag_block<s_game_audibility> zone_set_audibility;
	c_typed_tag_block<s_scenario_zone_set> zone_sets;
	c_typed_tag_block<s_scenario_lighting_zone_set> lighting_zone_sets;

	// campaign players
	// specify 4 names for the 4 coop players in campaign.
	// These names will be used to match with names of player representations in globals.globals
	c_typed_tag_block<scenario_campaign_player_representation_names_block> campaign_players;

	tag_block __unknown78;
	tag_block __unknown84;
	tag_block __unknown90;

	c_typed_tag_data<editor_scenario_data_definition> editor_scenario_data;
	c_typed_tag_block<editor_comment_definition> comments;
	c_typed_tag_block<scenario_object_name, 'sort'> object_names;

	c_typed_tag_block<scenario_scenery_block> scenery;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> scenery_palette;

	c_typed_tag_block<scenario_biped_block> bipeds;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> biped_palette;

	c_typed_tag_block<scenario_vehicle_block> vehicles;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> vehicle_palette;

	c_typed_tag_block<scenario_equipment_block> equipment;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> equipment_palette;

	c_typed_tag_block<scenario_weapon_block> weapons;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> weapon_palette;

	c_typed_tag_block<device_group_block> device_groups;

	c_typed_tag_block<scenario_machine_block> machines;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> machine_palette;

	c_typed_tag_block<scenario_terminal_block> terminals;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> terminal_palette;

	c_typed_tag_block<scenario_arg_device_block> arg_devices;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> arg_device_palette;

	c_typed_tag_block<scenario_control_block> controls;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> control_palette;

	c_typed_tag_block<scenario_sound_scenery_block> sound_scenery;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> sound_scenery_palette;

	c_typed_tag_block<scenario_giant_block> giants;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> giant_palette;

	c_typed_tag_block<scenario_effect_scenery_block> effect_scenery;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> effect_scenery_palette;

	c_typed_tag_block<scenario_light_block> light_volumes;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> light_volume_palette;

	c_typed_tag_block<scenario_object_palette_entry_with_string_id, 'sort'> map_variant_vehicle_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id, 'sort'> map_variant_weapon_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id, 'sort'> map_variant_equipment_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id, 'sort'> map_variant_scenery_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id, 'sort'> map_variant_teleporters_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id, 'sort'> map_variant_goals_palette;
	c_typed_tag_block<scenario_object_palette_entry_with_string_id, 'sort'> map_variant_spawners_palette;

	c_typed_tag_block<s_scenario_soft_ceiling> soft_ceilings;
	c_typed_tag_block<scenario_starting_profile> player_starting_profile;
	c_typed_tag_block<scenario_player> player_starting_locations;
	c_typed_tag_block<scenario_trigger_volume_block> trigger_volumes;
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
	c_typed_tag_reference<'unic'> custom_object_names;
	c_typed_tag_reference<'unic'> chapter_title_text;

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
	c_typed_tag_reference<'sfx+'> sound_effect_collection;

	c_typed_tag_block<s_scenario_crate> crates;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> crate_palette;

	c_typed_tag_block<s_flock_palette_entry, 'sort'> flock_palette;
	c_typed_tag_block<s_flock_instance> flocks;

	c_typed_tag_reference<'unic'> subtitles;

	c_typed_tag_block<s_scenario_creature> creatures;
	c_typed_tag_block<scenario_object_palette_entry, 'sort'> creature_palette;

	c_typed_tag_block<s_scenario_editor_folder> editor_folders;
	c_typed_tag_reference<'unic'> game_engine_strings;

	byte QVUC[8]; // pad

	c_typed_tag_block<ai_scenario_mission_dialogue> mission_dialogue;
	c_typed_tag_reference<'unic'> objectives;

	c_typed_tag_block<s_scenario_interpolator> interpolators;

	tag_block __unknown6BC;
	tag_block __unknown6C8;

	tag_block simulation_definition_table;

	c_typed_tag_reference<'cfxs'> camera_effects;
	c_typed_tag_reference<'sefc'> global_screen_effect;

	tag_reference unknown_screen_fx;

	c_typed_tag_reference<'skya'> atmospheric;
	c_typed_tag_reference<'chmt'> chocalate_mountain;
	c_typed_tag_reference<'sLdT'> new_lightmaps;
	c_typed_tag_reference<'perf'> performance_throttles;

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
	c_typed_tag_block<s_scenario_cubemap_info> cubemaps;
	c_typed_tag_block<s_scenario_cortana_effect> cortana_effects;
	c_typed_tag_block<s_scenario_airprobe_info> airprobes;
	c_typed_tag_block<s_scenario_budget_reference> budget_references;

	c_typed_tag_reference<'effe'> tchou_shader_effect;
	c_typed_tag_reference<'vmdx'> vision_mode_override;
	c_typed_tag_block<s_effect_gpu_data> effect_gpu_data;
};
static_assert(sizeof(scenario) == 0x824);

enum e_scenario_zone_set_flags
{
	_scenario_zone_set_begin_loading_next_level_bit = 0,
	_scenario_zone_set_debug_purposes_only_bit,
	_scenario_zone_set_interal_zone_set_bit,

	k_scenario_zone_set_flag_count
};

struct s_scenario_sky_reference
{
	c_typed_tag_reference<'scen'> sky;

	// scenario_object_name
	short name;                // short_block_index

	// scenario_structure_bsp_reference_block
	word_flags active_on_bsps; // word_block_flags
};
static_assert(sizeof(s_scenario_sky_reference) == 0x14);

struct s_scenario_zone_set
{
	c_string_id name;
	long pvs_index;                       // long_block_index
	c_flags<e_scenario_zone_set_flags, dword, k_scenario_zone_set_flag_count> flags;
	dword_flags bsp_zone_flags;           // long_block_flags

	// designer_zone_flags
	dword_flags required_designer_zones;  // long_block_flags

	dword_flags forbidden_designer_zones; // long_block_flags
	dword_flags cinematic_zones;          // long_block_flags
	long hint_previous_zone_set;          // long_block_index
	long audibility_index;                // long_block_index
};
static_assert(sizeof(s_scenario_zone_set) == 0x24);

struct scenario_campaign_player_representation_names_block
{
	c_string_id name;
};
static_assert(sizeof(scenario_campaign_player_representation_names_block) == 0x4);

struct scenario_object_name
{
	string name;
	short object_type; // short_block_index_custom_search
	short scenario_datum_index; // short_block_index_custom_search
};
static_assert(sizeof(scenario_object_name) == 0x24);

struct scenario_object_palette_entry
{
	c_typed_tag_reference<'obje'> name;

	byte GYFQQPUM[32]; // pad
};
static_assert(sizeof(scenario_object_palette_entry) == 0x30);

struct scenario_object_palette_entry_with_string_id
{
	c_typed_tag_reference<'obje'> name;
	c_string_id display_name;
	long maximum_allowed;
	real price_per_instance;

	byte GYFQQPUM[20]; // pad
};
static_assert(sizeof(scenario_object_palette_entry_with_string_id) == 0x30);

enum e_scenario_soft_ceiling_flags
{
	_scenario_soft_ceiling_flag_ignore_bipeds_bit = 0,
	_scenario_soft_ceiling_flag_ignore_vehicles_bit,
	_scenario_soft_ceiling_flag_ignore_camera_bit,
	_scenario_soft_ceiling_flag_ignore_huge_vehicles_bit,

	k_scenario_soft_ceiling_flag_count
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
	c_flags<e_scenario_soft_ceiling_flags, word, k_scenario_soft_ceiling_flag_count> flags;
	c_flags<e_scenario_soft_ceiling_flags, word, k_scenario_soft_ceiling_flag_count> runtime_flags;
	c_string_id name;
	c_enum<e_soft_ceiling_type, short, k_soft_ceiling_type_count> type;

	// pad
	byte my_name_is[2];
};
static_assert(sizeof(s_scenario_soft_ceiling) == 0xC);

struct scenario_starting_profile
{
	string name;

	// [0,1]
	real_fraction starting_health_damage;

	// [0,1]
	real_fraction starting_shield_damage;

	c_typed_tag_reference<'weap'> primary_weapon;
	short primary_rounds_loaded;
	short primary_rounds_total;

	c_typed_tag_reference<'weap'> secondary_weapon;
	short secondary_rounds_loaded;
	short secondary_rounds_total;

	long __unknown50;
	long __unknown54;

	char starting_fragmentation_grenade_count;
	char starting_plasma_grenade_count;
	char starting_claymore_grenade_count;
	char starting_firebomb_grenade_count;
	
	// s_scenario_editor_folder
	short editor_folder; // short_block_index

	// pad
	byte AHDVHJE[2];
};
static_assert(sizeof(scenario_starting_profile) == 0x60);

struct s_player_spawn_influence
{
	real override_full_weight_radius; // wu
	real override_fall_off_radius; // wu
	real override_upper_height; // wu
	real override_lower_height; // wu
	real override_weight;
};
static_assert(sizeof(s_player_spawn_influence) == 0x14);

struct s_netgame_goal_spawn_influence
{
	real override_full_weight_radius; // wu
	real override_fall_off_radius; // wu
	real override_upper_cylinder_height; // wu
	real override_lower_cylinder_height; // wu
	real override_weight;
};
static_assert(sizeof(s_netgame_goal_spawn_influence) == 0x14);