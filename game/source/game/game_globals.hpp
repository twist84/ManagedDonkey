#pragma once

#include "ai/ai_globals.hpp"
#include "camera/camera_globals.hpp"
#include "cseries/cseries.hpp"
#include "cutscene/cinematics_definitions.hpp"
#include "game/campaign_metagame_definitions.hpp"
#include "game/game_options.hpp"
#include "game/game_progression.hpp"
#include "game/materials_definitions.hpp"
#include "math/function_definitions.hpp"
#include "memory/data.hpp"
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

struct s_game_globals_havok_cleanup_resources;
struct s_damage_globals_definition;
struct s_game_globals_camera;
struct s_game_globals_difficulty_information;
struct s_game_globals_falling_damage;
struct s_game_globals_grenade;
struct s_game_globals_interface_tag_references;
struct s_game_globals_player_control;
struct s_game_globals_player_information;
struct s_game_globals_player_representation;
struct s_game_globals_shield_boost;
struct s_campaign_metagame_globals_block;
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
	c_typed_tag_block<s_ai_globals_definition, 'slap'> ai_globals; // DEPRECATED
	c_typed_tag_reference<AI_GLOBALS_TAG> ai_globals_ref;

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
	c_typed_tag_block<s_game_globals_tag_reference<WEAPON_TAG>> cheat_weapons;
	c_typed_tag_block<s_game_globals_tag_reference<EQUIPMENT_TAG>> cheat_powerups;

	c_typed_tag_block<s_game_globals_player_information> player_information;
	c_typed_tag_block<s_game_globals_player_representation> player_representation;
	c_typed_tag_block<s_game_globals_player_representation> player_representation_debug;

	c_typed_tag_block<s_game_globals_falling_damage> falling_damage;
	c_typed_tag_block<s_game_globals_shield_boost> shield_boost;
	c_typed_tag_block<s_global_material_definition, 'sort'> materials;

	c_typed_tag_reference<MULTIPLAYER_GLOBALS_TAG> multiplayer_globals;
	c_typed_tag_reference<SURVIVAL_MODE_GLOBALS_TAG> survival_mode_globals;
	c_typed_tag_block<cinematics_globals_block> cinematics_globals;
	c_typed_tag_block<s_campaign_metagame_globals_block> campaign_metagame_globals;

	c_static_array<c_language_pack, k_language_count> language_packs;

	// Rasterizer globals
	c_typed_tag_reference<RASTERIZER_GLOBALS_TAG> rasterizer_globals_ref;

	// Default camera fx settings
	c_typed_tag_reference<CAMERA_FX_SETTINGS_TAG> default_camera_fx_settings;

	// halo online
	c_typed_tag_reference<PODIUM_SETTINGS_TAG> podium_definition;

	// Default wind settings
	c_typed_tag_reference<WIND_TAG> default_wind_settings;

	// Default collision damage
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> collision_damage_effect;
	c_typed_tag_reference<COLLISION_DAMAGE_TAG> collision_damage;

	// global water material
	// the default value for what material type water is
	c_string_id global_water_material; // 'sted'
	short global_water_material_type;

	// pad
	byte pad_water_material[2];

	c_typed_tag_reference<EFFECT_GLOBALS_TAG> effect_globals;
	c_typed_tag_reference<GAME_PROGRESSION_TAG> game_progression;
	c_typed_tag_reference<ACHIEVEMENTS_TAG> game_achievements;

	// halo online

	c_typed_tag_reference<INPUT_GLOBALS_TAG> input_globals;
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

		void update_reference_names();
	};
	static_assert(sizeof(sound_block) == sizeof(s_tag_reference));

	void update_reference_names();
};
static_assert(sizeof(s_game_globals) == 0x608);

struct s_game_globals_havok_cleanup_resources
{
	c_typed_tag_reference<EFFECT_TAG> object_cleanup_effect;

	void update_reference_names();
};
static_assert(sizeof(s_game_globals_havok_cleanup_resources) == 0x10);

struct s_damage_group_definition;
struct s_damage_globals_definition
{
	c_typed_tag_block<s_damage_group_definition> damage_groups;
};
static_assert(sizeof(s_damage_globals_definition) == 0xC);

struct s_armor_modifier_definition;
struct s_damage_group_definition
{
	c_string_id name;
	c_typed_tag_block<s_armor_modifier_definition> armor_modifiers;
};
static_assert(sizeof(s_damage_group_definition) == 0x10);

struct s_armor_modifier_definition
{
	c_string_id name;
	real damage_multiplier;
};
static_assert(sizeof(s_armor_modifier_definition) == 0x8);

struct s_game_globals_camera
{
	c_typed_tag_reference<CAMERA_TRACK_TAG> default_unit_camera_track;

	// CAMERA UNIVERSALS

	real field_of_view; // degrees
	real yaw_scale;
	real pitch_scale;
	real forward_scale;
	real side_scale;
	real up_scale;


	// DEAD CAMERA

	// time it takes for the camera to move from the initial distance to the final distance
	real transition_time; // seconds

	// time it takes for the camera to move to its final position during a falling death
	real falling_death_transition_time; // seconds

	// on the first frame after death, this is how far out of the body the camera will be
	real initial_distance; // wu

	// how far from the body the camera will settle
	real final_distance; // wu

	// how far above the body the camera focuses on
	real dead_cam_z_offset; // wu

	// the highest angle the camera can raise to(prevents it from flipping over the vertical axis)
	real dead_cam_maximum_elevation; // radians

	// delay in tracking the killer
	real dead_cam_movement_delay; // seconds

	// how long the death camera lasts before switching to orbiting camera
	real time; // seconds

	// minimum velocity to switch to fell to death behavior(when biped is not actually falling to death)
	real dead_camera_minimum_falling_velocity;


	// FLYING CAMERA

	// the scaling factor for the left stick when the left trigger is fully depressed
	real maximum_boost_speed;

	// seconds. while pegging boost, time to reach maximum speed
	real time_to_maximum_boost;

	c_enum<e_global_transition_function, short, _global_transition_function_linear, k_global_transition_function_count> boost_function;
	byte hoist[2];

	// field of view when zoomed
	real zoomed_field_of_view; // degrees

	// scaling factor for look speed when zoomed
	real zoomed_look_speed;

	// radius of sphere for collision
	real bounding_sphere_radius; // wu

	// how quickly the camera responds to the user's input
	real flying_cam_movement_delay; // seconds

	// how long it takes to zoom in or out
	real zoom_transition_time; // seconds

	real vertical_movement_time_to;
	c_enum<e_global_transition_function, short, _global_transition_function_linear, k_global_transition_function_count> vertical_movement_function;
	byte moist[2];

	// how long it takes in survival mode before switching to flying camera
	real survival_switch_time; // seconds


	// ORBITING CAMERA

	real minimum_distance; // wu
	real maximum_distance; // wu

	// how quickly the camera responds to the user's input
	real orbit_cam_movement_delay; // seconds

	// how far above the object's root node to position the camera's focus point
	real orbit_cam_z_offset; // wu

	// lowest angle the camera can be moved to
	real orbit_cam_minimum_elevation; // radians

	// highest angle the camera can be moved to
	real orbit_cam_maximum_elevation; // radians


	// SAVED FILMS

	// how fast the film plays when the trigger is fully depressed
	real max_playback_speed;

	// how long it takes for the screen to fade out when rewinding
	real fade_out_time; // seconds

	// see above
	real fade_in_time; // seconds


	// IN GAME

	// how long it takes the camera to move from first to third person when entering a vehicle
	real enter_vehicle_transition_time; // seconds

	// see above
	real exit_vehicle_transition_time; // seconds

	void update_reference_names();
};

struct s_look_function_block
{
	real scale;
};
static_assert(sizeof(s_look_function_block) == sizeof(real));

struct s_game_globals_player_control
{
	real_fraction magnetism_friction;
	real_fraction magnetism_adhesion;
	real_fraction inconsequential_target_scale;

	// crosshair
	real_point2d crosshair_location;

	// sprinting
	real seconds_to_start;
	real seconds_to_full_speed;
	real decay_rate;
	real full_speed_multiplier;
	real pegged_magnitude;
	real pegged_angular_threshold;
	real stamina_deplete_restore_time;
	real cooldown_time;

	// looking
	real look_default_pitch_rate;
	real look_default_yaw_rate;
	real_fraction look_peg_threshold;
	real look_yaw_acceleration_time;
	real look_yaw_acceleration_scale;
	real look_pitch_acceleration_time;
	real look_pitch_acceleration_scale;
	real look_autolevelling_scale;
	char TMIDI[8]; // pad
	real gravity_scale;
	char VM[2]; // pad
	short minimum_autolevelling_ticks;
	c_typed_tag_block<s_look_function_block> look_function;
};
static_assert(sizeof(s_game_globals_player_control) == 0x70);

struct s_game_globals_difficulty_information
{
	// health
	// 
	// scale values for enemy health and damage settings
	real enemy_damage[k_campaign_difficulty_levels_count];    // enemy damage multiplier
	real enemy_vitality[k_campaign_difficulty_levels_count];  // enemy maximum body vitality scale
	real enemy_shield[k_campaign_difficulty_levels_count];    // enemy maximum shield vitality scale
	real enemy_recharge[k_campaign_difficulty_levels_count];  // enemy shield recharge scale
	real friend_damage[k_campaign_difficulty_levels_count];   // friend damage multiplier
	real friend_vitality[k_campaign_difficulty_levels_count]; // friend maximum body vitality scale
	real friend_shield[k_campaign_difficulty_levels_count];   // friend maximum shield vitality scale
	real friend_recharge[k_campaign_difficulty_levels_count]; // friend shield recharge scale
	real infection_forms[k_campaign_difficulty_levels_count]; // toughness of infection forms (may be negative)
	byte $[sizeof(real) * k_campaign_difficulty_levels_count];

	// ranged fire
	// 
	// difficulty-affecting values for enemy ranged combat settings
	real rate_of_fire[k_campaign_difficulty_levels_count];       // enemy rate of fire scale
	real projectile_error[k_campaign_difficulty_levels_count];   // enemy projectile error scale, as a fraction of their base firing error.
	real burst_error[k_campaign_difficulty_levels_count];        // enemy burst error scale; reduces intra-burst shot distance.
	real new_target_delay[k_campaign_difficulty_levels_count];   // enemy new-target delay scale factor.
	real burst_separation[k_campaign_difficulty_levels_count];   // delay time between bursts scale factor for enemies.
	real target_tracking[k_campaign_difficulty_levels_count];    // additional target tracking fraction for enemies.
	real target_leading[k_campaign_difficulty_levels_count];     // additional target leading fraction for enemies.
	real overcharge_chance[k_campaign_difficulty_levels_count];  // overcharge chance scale factor for enemies.
	real special_fire_delay[k_campaign_difficulty_levels_count]; // delay between special-fire shots (overcharge, banshee bombs) scale factor for enemies.
	real guidance_vs_player[k_campaign_difficulty_levels_count]; // guidance velocity scale factor for all projectiles targeted on a player.
	real melee_delay_base[k_campaign_difficulty_levels_count];   // delay period added to all melee attacks, even when berserk.
	real melee_delay_scale[k_campaign_difficulty_levels_count];  // multiplier for all existing non-berserk melee delay times.
	byte $$[sizeof(real) * k_campaign_difficulty_levels_count];

	// grenades
	// 
	// difficulty-affecting values for enemy grenade behavior
	real grenade_chance_scale[k_campaign_difficulty_levels_count]; // scale factor affecting the desicions to throw a grenade.
	real grenade_timer_scale[k_campaign_difficulty_levels_count];  // scale factor affecting the delay period between grenades thrown from the same encounter (lower is more often).
	byte $$$[sizeof(real) * k_campaign_difficulty_levels_count];
	byte $$$$[sizeof(real) * k_campaign_difficulty_levels_count];
	byte $$$$$[sizeof(real) * k_campaign_difficulty_levels_count];

	// placement
	// 
	// difficulty-affecting values for enemy placement
	real major_upgrade_normal[k_campaign_difficulty_levels_count]; // fraction of actors upgraded to their major variant.
	real major_upgrade_few[k_campaign_difficulty_levels_count];    // fraction of actors upgraded to their major variant when mix = normal.
	real major_upgrade_many[k_campaign_difficulty_levels_count];   // fraction of actors upgraded to their major variant when mix = many.

	// vehicles
	// 
	// difficulty-affecting values for vehicle driving/combat
	real player_vehicle_ram_chance[k_campaign_difficulty_levels_count]; // Chance of deciding to ram the player in a vehicle
	byte $$$$$$[sizeof(real) * k_campaign_difficulty_levels_count];
	byte $$$$$$$[sizeof(real) * k_campaign_difficulty_levels_count];
	byte $$$$$$$$[sizeof(real) * k_campaign_difficulty_levels_count];

	byte Q[0x54];
};
static_assert(sizeof(s_game_globals_difficulty_information) == 0x284);

struct s_game_globals_grenade
{
	short maximum_count;
	byte GQGKOFEHN[2]; // padding
	c_typed_tag_reference<EFFECT_TAG> throwing_effect;
	byte TF[16]; // padding
	c_typed_tag_reference<EQUIPMENT_TAG> equipment;
	c_typed_tag_reference<PROJECTILE_TAG> projectile;

	void update_reference_names();
};
static_assert(sizeof(s_game_globals_grenade) == 0x44);

struct s_game_globals_interface_tag_references
{
	struct gfx_ui_string_block;

	c_typed_tag_reference<BITMAP_TAG> obsolete1; // spinner bitmap
	c_typed_tag_reference<BITMAP_TAG> obsolete2;
	c_typed_tag_reference<COLOR_TABLE_TAG> screen_color_table;
	c_typed_tag_reference<COLOR_TABLE_TAG> hud_color_table;
	c_typed_tag_reference<COLOR_TABLE_TAG> editor_color_table;
	c_typed_tag_reference<COLOR_TABLE_TAG> dialog_color_table;
	c_typed_tag_reference<BITMAP_TAG> motion_sensor_sweep_bitmap;
	c_typed_tag_reference<BITMAP_TAG> motion_sensor_sweep_bitmap_mask;
	c_typed_tag_reference<BITMAP_TAG> multiplayer_hud_bitmap;
	s_tag_reference unused;
	c_typed_tag_reference<BITMAP_TAG> motion_sensor_blip_bitmap;
	c_typed_tag_reference<BITMAP_TAG> interface_goo_map1;
	c_typed_tag_reference<BITMAP_TAG> interface_goo_map2;
	c_typed_tag_reference<BITMAP_TAG> interface_goo_map3;
	c_typed_tag_reference<USER_INTERFACE_GLOBALS_DEFINITION_TAG> mainmenu_ui_globals;
	c_typed_tag_reference<USER_INTERFACE_GLOBALS_DEFINITION_TAG> singleplayer_ui_globals;
	c_typed_tag_reference<USER_INTERFACE_GLOBALS_DEFINITION_TAG> multiplayer_ui_globals;
	c_typed_tag_reference<CHUD_GLOBALS_DEFINITION_TAG> chud_globals;
	c_typed_tag_block<gfx_ui_string_block> gfx_ui_strings;

	struct gfx_ui_string_block
	{
		c_static_string<32> name;
		c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> strings;

		void update_reference_names();
	};
	static_assert(sizeof(gfx_ui_string_block) == 0x30);

	void update_reference_names();
};
static_assert(sizeof(s_game_globals_interface_tag_references) == 0x12C);

struct s_game_globals_player_information
{
	real walking_speed;
	real run_forward;
	real run_backward;
	real run_sideways;
	real run_acceleration;
	real sneak_forward;
	real sneak_backward;
	real sneak_sideways;
	real sneak_acceleration;
	real airborne_acceleration;
	real_point3d grenade_origin;
	real stun_movement_penalty;
	real stun_turning_penalty;
	real stun_jumping_penalty;
	real minimum_stun_time;
	real maximum_stun_time;
	real_bounds first_person_idle_time;
	real_fraction first_person_skip_fraction;
	real melee_inhibit_time;
	c_typed_tag_reference<SOUND_TAG> coop_countdown_sound;
	c_typed_tag_reference<SOUND_TAG> coop_respawn_sound;
	c_typed_tag_reference<EFFECT_TAG> coop_respawn_effect;
	long binoculars_zoom_count;
	real_bounds binoculars_zoom_range;
	real __unknown94;
	real __unknown98;
	c_typed_tag_reference<SOUND_TAG> flashlight_on;
	c_typed_tag_reference<SOUND_TAG> flashlight_off;
	c_typed_tag_reference<DAMAGE_RESPONSE_DEFINITION_TAG> default_damage_response;

	void update_reference_names();
};
static_assert(sizeof(s_game_globals_player_information) == 0xCC);

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

	void update_reference_names();
};
static_assert(sizeof(s_game_globals_player_representation) == 0x6C);

template<tag group_tag>
struct s_game_globals_tag_reference : s_tag_reference
{
	void update_reference_names();
};

