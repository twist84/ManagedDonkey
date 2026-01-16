#pragma once

#include "ai/ai_globals.hpp"
#include "camera/camera_globals.hpp"
#include "cseries/cseries.hpp"
#include "cutscene/cinematics_definitions.hpp"
#include "game/campaign_metagame_definitions.hpp"
#include "game/game_options.hpp"
#include "game/game_progression.hpp"
#include "game/materials.hpp"
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
	uns32 active_structure_bsp_mask;
	uns32 active_designer_zone_mask;
	uns32 active_cinematic_zone_mask;
	game_options options;

	// ODST
	string_id active_game_progression_level;
	s_campaign_game_progression active_game_progression;

	bool game_in_progress;
	bool game_lost;
	bool game_revert;

	// ODST
	bool prepare_for_game_progression;

	int32 game_loss_timer;
	bool game_finished;
	int32 game_finished_timer;
	int32 active_primary_skulls;
	int32 active_secondary_skulls;
	bool game_is_playtest;
	bool game_had_an_update_tick_this_frame;
	s_game_cluster_bit_vectors cluster_pvs;
	s_game_cluster_bit_vectors cluster_pvs_local;
	s_game_cluster_bit_vectors cluster_activation;
	bool pvs_use_scripted_camera;
	uns8 unused_pad;
	int16 pvs_activation_type;
	struct
	{
		int32 object_index;
		//s_cluster_reference cluster_reference;
	} pvs_activation;

	int32 game_ragdoll_count;

	byte __data25204[0x4];
};
COMPILE_ASSERT(sizeof(game_globals_storage) == 0x25208);

extern e_language get_map_language();
extern int32 get_map_minor_version();

struct s_damage_globals_definition;
struct s_game_globals_havok_cleanup_resources;
struct s_game_globals_camera;
struct s_game_globals_difficulty_information;
struct s_game_globals_falling_damage;
struct s_game_globals_grenade;
struct s_game_globals_interface_tag_references;
struct s_game_globals_player_control;
struct s_game_globals_player_information;
struct s_game_globals_player_representation;
struct s_game_globals_shield_boost;
template<tag ...k_group_tags>
struct s_game_globals_tag_reference;
struct s_game_globals
{
	static tag const k_group_tag = GLOBALS_TAG;

	struct damage_reporting_type_block;

	int32 unused0[0x2B];

	e_language language;
	c_typed_tag_block<s_game_globals_havok_cleanup_resources> havok_cleanup_resources;
	c_typed_tag_block<s_sound_globals_definition> sound_globals;

	// AI globals
	// I have moved the ai globals out of this tag, and into its own tag which is referenced here.
	c_typed_tag_block<s_ai_globals_definition, 'slap'> ai_globals; // DEPRECATED
	c_typed_tag_reference<AI_GLOBALS_TAG, INVALID_TAG> ai_globals_ref;

	c_typed_tag_block<s_damage_globals_definition> damage_table;
	int8 unused1[0xC];
	c_typed_tag_block<s_game_globals_tag_reference<SOUND_TAG, INVALID_TAG>> sounds;
	c_typed_tag_block<s_game_globals_camera> camera;
	c_typed_tag_block<s_game_globals_player_control> player_control;
	c_typed_tag_block<s_game_globals_difficulty_information> difficulty_information;
	c_typed_tag_block<s_game_globals_grenade> grenades;
	int8 unused2[0xC];
	c_typed_tag_block<s_game_globals_interface_tag_references> interface_tag_references;

	// @weapon list (update _weapon_list enum in game_globals.h)
	c_typed_tag_block<s_game_globals_tag_reference<WEAPON_TAG, INVALID_TAG>> weapon_list;
	c_typed_tag_block<s_game_globals_tag_reference<EQUIPMENT_TAG, INVALID_TAG>> cheat_powerups;

	c_typed_tag_block<s_game_globals_player_information> player_information;
	c_typed_tag_block<s_game_globals_player_representation> player_representation;
	c_typed_tag_block<s_game_globals_player_representation> player_representation_debug;

	c_typed_tag_block<s_game_globals_falling_damage> falling_damage;
	c_typed_tag_block<s_game_globals_shield_boost> shield_boost;
	c_typed_tag_block<s_global_material_definition, 'sort'> materials;

	static c_typed_tag_block<rgb_color> profile_colors;

	c_typed_tag_reference<MULTIPLAYER_GLOBALS_TAG, INVALID_TAG> multiplayer_globals;
	c_typed_tag_reference<SURVIVAL_MODE_GLOBALS_TAG, INVALID_TAG> survival_mode_globals;
	c_typed_tag_block<cinematics_globals_block> cinematics_globals;
	c_typed_tag_block<s_campaign_metagame_globals> campaign_metagame_globals;

	c_static_array<c_language_pack, k_language_count> language_packs;

	// Rasterizer globals
	c_typed_tag_reference<RASTERIZER_GLOBALS_TAG, INVALID_TAG> rasterizer_globals_ref;

	// Default camera fx settings
	c_typed_tag_reference<CAMERA_FX_SETTINGS_TAG, INVALID_TAG> default_camera_fx_settings;

	// halo online
	c_typed_tag_reference<PODIUM_SETTINGS_TAG, INVALID_TAG> podium_definition;

	// Default wind settings
	c_typed_tag_reference<WIND_TAG, INVALID_TAG> default_wind_settings;

	// Default collision damage
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> collision_damage_effect;
	c_typed_tag_reference<COLLISION_DAMAGE_TAG, INVALID_TAG> collision_damage;

	// global water material
	// the default value for what material type water is
	c_string_id global_water_material; // 'sted'
	c_global_material_type global_water_material_type;
	int16 pad;

	c_typed_tag_reference<EFFECT_GLOBALS_TAG, INVALID_TAG> effect_globals;
	c_typed_tag_reference<GAME_PROGRESSION_TAG, INVALID_TAG> game_progression;
	c_typed_tag_reference<ACHIEVEMENTS_TAG, INVALID_TAG> game_achievements;

	// halo online

	c_typed_tag_reference<INPUT_GLOBALS_TAG, INVALID_TAG> input_globals;

	real32 biped_camo_reference;
	real32 vehicle_camo_reference;
	real32 biped_speed_reference;
	real32 vehicle_speed_reference;
	c_function_definition speed_to_maximum_camo;

	// these 4 values are a struct passed to `sub_B56980` in `object_damage_update`
	// sub_B56980(
	//   &global_game_globals->__unknown5E4,
	//   TEST_BIT(object.damage_flags, 3),
	//   object.shield_vitality,
	//   &object.shield_impact_current_body_damage,
	//   &object.shield_impact_current_shield_damage,
	//   &object.body_damage_delay_ticks,
	// );
	// alongside these are the following
	// object.shield_vitality
	// TEST_BIT(object.damage_flags, 3)
	// object.shield_impact_current_body_damage
	// object.shield_impact_current_shield_damage
	// object.body_damage_delay_ticks
	real32 __unknown5E4;
	real32 __unknown5E8;
	real32 __unknown5EC;
	real32 __unknown5F0;

	real32 __unknown5F4;
	c_typed_tag_block<damage_reporting_type_block> damage_reporting_types;
	real32 __unknown604;

	struct damage_reporting_type_block
	{
		int16 index;
		int16 version;
		c_static_string<k_tag_string_length> name;
	};
	COMPILE_ASSERT(sizeof(damage_reporting_type_block) == 0x24);

	void update_static_runtime_data();
	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_game_globals) == 0x608);

struct s_damage_group_definition;
struct s_damage_globals_definition
{
	c_typed_tag_block<s_damage_group_definition> damage_groups;
};
COMPILE_ASSERT(sizeof(s_damage_globals_definition) == 0xC);

struct s_armor_modifier_definition;
struct s_damage_group_definition
{
	c_string_id name;
	c_typed_tag_block<s_armor_modifier_definition> armor_modifiers;
};
COMPILE_ASSERT(sizeof(s_damage_group_definition) == 0x10);

struct s_armor_modifier_definition
{
	c_string_id name;
	real32 damage_multiplier;
};
COMPILE_ASSERT(sizeof(s_armor_modifier_definition) == 0x8);

struct s_game_globals_havok_cleanup_resources
{
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> object_cleanup_effect;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_game_globals_havok_cleanup_resources) == 0x10);

struct s_game_globals_camera
{
	c_typed_tag_reference<CAMERA_TRACK_TAG, INVALID_TAG> default_unit_camera_track;

	// CAMERA UNIVERSALS

	real32 field_of_view; // degrees
	real32 yaw_scale;
	real32 pitch_scale;
	real32 forward_scale;
	real32 side_scale;
	real32 up_scale;


	// DEAD CAMERA

	// time it takes for the camera to move from the initial distance to the final distance
	real32 transition_time; // seconds

	// time it takes for the camera to move to its final position during a falling death
	real32 falling_death_transition_time; // seconds

	// on the first frame after death, this is how far out of the body the camera will be
	real32 initial_distance; // wu

	// how far from the body the camera will settle
	real32 final_distance; // wu

	// how far above the body the camera focuses on
	real32 dead_cam_z_offset; // wu

	// the highest angle the camera can raise to(prevents it from flipping over the vertical axis)
	real32 dead_cam_maximum_elevation; // radians

	// delay in tracking the killer
	real32 dead_cam_movement_delay; // seconds

	// how long the death camera lasts before switching to orbiting camera
	real32 time; // seconds

	// minimum velocity to switch to fell to death behavior(when biped is not actually falling to death)
	real32 dead_camera_minimum_falling_velocity;


	// FLYING CAMERA

	// the scaling factor for the left stick when the left trigger is fully depressed
	real32 maximum_boost_speed;

	// seconds. while pegging boost, time to reach maximum speed
	real32 time_to_maximum_boost;

	c_enum<e_global_transition_function, int16, _global_transition_function_linear, k_global_transition_function_count> boost_function;
	uns8 hoist[2];

	// field of view when zoomed
	real32 zoomed_field_of_view; // degrees

	// scaling factor for look speed when zoomed
	real32 zoomed_look_speed;

	// radius of sphere for collision
	real32 bounding_sphere_radius; // wu

	// how quickly the camera responds to the user's input
	real32 flying_cam_movement_delay; // seconds

	// how long it takes to zoom in or out
	real32 zoom_transition_time; // seconds

	real32 vertical_movement_time_to;
	c_enum<e_global_transition_function, int16, _global_transition_function_linear, k_global_transition_function_count> vertical_movement_function;
	uns8 moist[2];

	// how long it takes in survival mode before switching to flying camera
	real32 survival_switch_time; // seconds


	// ORBITING CAMERA

	real32 minimum_distance; // wu
	real32 maximum_distance; // wu

	// how quickly the camera responds to the user's input
	real32 orbit_cam_movement_delay; // seconds

	// how far above the object's root node to position the camera's focus point
	real32 orbit_cam_z_offset; // wu

	// lowest angle the camera can be moved to
	real32 orbit_cam_minimum_elevation; // radians

	// highest angle the camera can be moved to
	real32 orbit_cam_maximum_elevation; // radians


	// SAVED FILMS

	// how fast the film plays when the trigger is fully depressed
	real32 max_playback_speed;

	// how long it takes for the screen to fade out when rewinding
	real32 fade_out_time; // seconds

	// see above
	real32 fade_in_time; // seconds


	// IN GAME

	// how long it takes the camera to move from first to third person when entering a vehicle
	real32 enter_vehicle_transition_time; // seconds

	// see above
	real32 exit_vehicle_transition_time; // seconds

	void update_reference_names();
};

struct s_look_function_block
{
	real32 scale;
};
COMPILE_ASSERT(sizeof(s_look_function_block) == sizeof(real32));

struct s_game_globals_player_control
{
	real32 magnetism_friction;           // how much the crosshair slows over enemies
	real32 magnetism_adhesion;           // how much the crosshair sticks to enemies
	real32 inconsequential_target_scale; // scales magnetism level for inconsequential targets like infection forms

	// crosshair
	real_point2d crosshair_location; // -1..1, 0 is middle of the screen

	// sprinting
	real32 seconds_to_start;             // how long you must be pegged before you start sprinting
	real32 seconds_to_full_speed;        // how long you must sprint before you reach top speed
	real32 decay_rate;                   // how fast being unpegged decays the timer (seconds per second)
	real32 full_speed_multiplier;        // how much faster we actually go when at full sprint
	real32 pegged_magnitude;             // how far the stick needs to be pressed before being considered pegged
	real32 pegged_angular_threshold;     // how far off straight up (in degrees) we consider pegged
	real32 stamina_deplete_restore_time; // time to restore stamina from empty or deplete from full (seconds)
	real32 cooldown_time;                // time between sprint end and next available use (seconds)

	// looking
	real32 look_default_pitch_rate; // degrees
	real32 look_default_yaw_rate;   // degrees

	real32 look_peg_threshold; // magnitude of yaw for pegged acceleration to kick in

	// time for a pegged look to reach maximum effect
	real32 look_yaw_acceleration_time; // seconds
	real32 look_yaw_acceleration_scale; // maximum effect of a pegged look (scales last value in the look function below)

	// time for a pegged look to reach maximum effect
	real32 look_pitch_acceleration_time; // seconds
	real32 look_pitch_acceleration_scale; // maximum effect of a pegged look (scales last value in the look function below)

	real32 look_autolevelling_scale;      // 1 is fast, 0 is none, >1 will probably be really fast
	real32 look_unused[2];
	real32 gravity_scale;
	int16 control_unused;
	int16 minimum_autolevelling_ticks; // amount of time player needs to move and not look up or down for autolevelling to kick in
	c_typed_tag_block<s_look_function_block> look_function;
};
COMPILE_ASSERT(sizeof(s_game_globals_player_control) == 0x70);

struct s_game_globals_difficulty_information
{
	// health
	// 
	// scale values for enemy health and damage settings
	real32 enemy_damage[k_campaign_difficulty_levels_count];    // enemy damage multiplier
	real32 enemy_vitality[k_campaign_difficulty_levels_count];  // enemy maximum body vitality scale
	real32 enemy_shield[k_campaign_difficulty_levels_count];    // enemy maximum shield vitality scale
	real32 enemy_recharge[k_campaign_difficulty_levels_count];  // enemy shield recharge scale
	real32 friend_damage[k_campaign_difficulty_levels_count];   // friend damage multiplier
	real32 friend_vitality[k_campaign_difficulty_levels_count]; // friend maximum body vitality scale
	real32 friend_shield[k_campaign_difficulty_levels_count];   // friend maximum shield vitality scale
	real32 friend_recharge[k_campaign_difficulty_levels_count]; // friend shield recharge scale
	real32 infection_forms[k_campaign_difficulty_levels_count]; // toughness of infection forms (may be negative)
	byte $[sizeof(real32) * k_campaign_difficulty_levels_count];

	// ranged fire
	// 
	// difficulty-affecting values for enemy ranged combat settings
	real32 rate_of_fire[k_campaign_difficulty_levels_count];       // enemy rate of fire scale
	real32 projectile_error[k_campaign_difficulty_levels_count];   // enemy projectile error scale, as a fraction of their base firing error.
	real32 burst_error[k_campaign_difficulty_levels_count];        // enemy burst error scale; reduces intra-burst shot distance.
	real32 new_target_delay[k_campaign_difficulty_levels_count];   // enemy new-target delay scale factor.
	real32 burst_separation[k_campaign_difficulty_levels_count];   // delay time between bursts scale factor for enemies.
	real32 target_tracking[k_campaign_difficulty_levels_count];    // additional target tracking fraction for enemies.
	real32 target_leading[k_campaign_difficulty_levels_count];     // additional target leading fraction for enemies.
	real32 overcharge_chance[k_campaign_difficulty_levels_count];  // overcharge chance scale factor for enemies.
	real32 special_fire_delay[k_campaign_difficulty_levels_count]; // delay between special-fire shots (overcharge, banshee bombs) scale factor for enemies.
	real32 guidance_vs_player[k_campaign_difficulty_levels_count]; // guidance velocity scale factor for all projectiles targeted on a player.
	real32 melee_delay_base[k_campaign_difficulty_levels_count];   // delay period added to all melee attacks, even when berserk.
	real32 melee_delay_scale[k_campaign_difficulty_levels_count];  // multiplier for all existing non-berserk melee delay times.
	byte $$[sizeof(real32) * k_campaign_difficulty_levels_count];

	// grenades
	// 
	// difficulty-affecting values for enemy grenade behavior
	real32 grenade_chance_scale[k_campaign_difficulty_levels_count]; // scale factor affecting the desicions to throw a grenade.
	real32 grenade_timer_scale[k_campaign_difficulty_levels_count];  // scale factor affecting the delay period between grenades thrown from the same encounter (lower is more often).
	byte $$$[sizeof(real32) * k_campaign_difficulty_levels_count];
	byte $$$$[sizeof(real32) * k_campaign_difficulty_levels_count];
	byte $$$$$[sizeof(real32) * k_campaign_difficulty_levels_count];

	// placement
	// 
	// difficulty-affecting values for enemy placement
	real32 major_upgrade_normal[k_campaign_difficulty_levels_count]; // fraction of actors upgraded to their major variant.
	real32 major_upgrade_few[k_campaign_difficulty_levels_count];    // fraction of actors upgraded to their major variant when mix = normal.
	real32 major_upgrade_many[k_campaign_difficulty_levels_count];   // fraction of actors upgraded to their major variant when mix = many.

	// vehicles
	// 
	// difficulty-affecting values for vehicle driving/combat
	real32 player_vehicle_ram_chance[k_campaign_difficulty_levels_count]; // Chance of deciding to ram the player in a vehicle
	byte $$$$$$[sizeof(real32) * k_campaign_difficulty_levels_count];
	byte $$$$$$$[sizeof(real32) * k_campaign_difficulty_levels_count];
	byte $$$$$$$$[sizeof(real32) * k_campaign_difficulty_levels_count];

	byte Q[0x54];
};
COMPILE_ASSERT(sizeof(s_game_globals_difficulty_information) == 0x284);

struct s_game_globals_falling_damage
{
	real_bounds harmful_falling_distance; // world units
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> falling_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> jumping_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> soft_landing_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> hard_landing_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> hs_damage;
	real32 maximum_falling_distance; // world units
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> distance_damage;
	real32 runtime_maximum_falling_velocity;
	real_bounds runtime_damage_velocity_bounds;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_game_globals_falling_damage) == 0x78);

struct s_game_globals_grenade
{
	int16 maximum_count;
	uns16 pad;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> throwing_effect;
	int32 unused[4];
	c_typed_tag_reference<ITEM_TAG, INVALID_TAG> item;
	c_typed_tag_reference<PROJECTILE_TAG, INVALID_TAG> projectile;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_game_globals_grenade) == 0x44);

struct s_game_globals_interface_tag_references
{
	struct gfx_ui_string_block;

	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> obsolete1; // spinner bitmap
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> obsolete2;
	c_typed_tag_reference<COLOR_TABLE_TAG, INVALID_TAG> screen_color_table;
	c_typed_tag_reference<COLOR_TABLE_TAG, INVALID_TAG> hud_color_table;
	c_typed_tag_reference<COLOR_TABLE_TAG, INVALID_TAG> editor_color_table;
	c_typed_tag_reference<COLOR_TABLE_TAG, INVALID_TAG> dialog_color_table;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> motion_sensor_sweep_bitmap;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> motion_sensor_sweep_bitmap_mask;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> multiplayer_hud_bitmap;
	s_tag_reference unused;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> motion_sensor_blip_bitmap;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> interface_goo_map1;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> interface_goo_map2;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> interface_goo_map3;
	c_typed_tag_reference<USER_INTERFACE_GLOBALS_DEFINITION_TAG, INVALID_TAG> mainmenu_ui_globals;
	c_typed_tag_reference<USER_INTERFACE_GLOBALS_DEFINITION_TAG, INVALID_TAG> singleplayer_ui_globals;
	c_typed_tag_reference<USER_INTERFACE_GLOBALS_DEFINITION_TAG, INVALID_TAG> multiplayer_ui_globals;
	c_typed_tag_reference<CHUD_GLOBALS_DEFINITION_TAG, INVALID_TAG> chud_globals;
	c_typed_tag_block<gfx_ui_string_block> gfx_ui_strings;

	struct gfx_ui_string_block
	{
		c_static_string<k_tag_string_length> name;
		c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> strings;

		void update_reference_names();
	};
	COMPILE_ASSERT(sizeof(gfx_ui_string_block) == 0x30);

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_game_globals_interface_tag_references) == 0x12C);

struct s_game_globals_player_information
{
	// world units per second
	real32 walking_speed;         // world units per second
	real32 run_forward_speed;     // world units per second
	real32 run_backward_speed;    // world units per second
	real32 run_sideways_speed;    // world units per second
	real32 run_acceleration;      // world units per second squared
	real32 sneak_acceleration;    // world units per second
	real32 sneak_sideways_speed;  // world units per second
	real32 sneak_backward_speed;  // world units per second
	real32 sneak_forward_speed;   // world units per second
	real32 airborne_acceleration; // world units per second

	real_point3d grenade_origin;

	// 1.0 prevents moving while stunned
	real32 stun_movement_penalty; // [0,1]

	// 1.0 prevents moving while stunned
	real32 stun_turning_penalty; // [0,1]

	// 1.0 prevents moving while stunned
	real32 stun_jumping_penalty; // [0,1]

	// all stunning damage will last for at least this long
	real32 minimum_stun_time; // seconds

	// no stunning damage will last for longer than this
	real32 maximum_stun_time; // seconds

	real_bounds first_person_idle_time; // seconds
	real32 first_person_skip_fraction; // [0,1]

	// time to prevent player from melee attacking after being hit by damage that supports this (singleplayer only)
	real32 melee_inhibit_time; // seconds

	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> coop_countdown_sound;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> coop_respawn_sound;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> coop_respawn_effect;
	int32 binoculars_zoom_count;
	real_bounds binoculars_zoom_range;
	real32 __unknown94;
	real32 __unknown98;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> unit_sounds[2];
	c_typed_tag_reference<DAMAGE_RESPONSE_DEFINITION_TAG, INVALID_TAG> default_damage_response;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_game_globals_player_information) == 0xCC);

struct s_game_globals_player_representation
{
	c_string_id name;
	int8 model_choice;
	int8 _class;
	byte __pad6[0x2];
	s_tag_reference first_person_hands;
	s_tag_reference first_person_body;
	s_tag_reference first_person_unit;
	c_string_id third_person_variant;
	s_tag_reference binoculars_zoom_in_sounds;
	s_tag_reference binoculars_zoom_out_sounds;
	s_tag_reference voice;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_game_globals_player_representation) == 0x6C);

struct s_game_globals_shield_boost
{
	// shield boosting
	// 
	// This feature was implemented for the engineer character in atlas. Shield dynamics overrides for when shields are being 'boosted' by an external source
	real32 shield_boost_decay;         // amount of shield-boost to decay per second
	real32 shield_boost_recharge_time; // time to recharge full shields when getting boosted
	real32 shield_boost_stun_time;     // stun time when getting boosted
};
COMPILE_ASSERT(sizeof(s_game_globals_shield_boost) == 0xC);

template<tag ...k_group_tags>
struct s_game_globals_tag_reference :
	s_tag_reference
{
	void update_reference_names();
};

extern int32 __cdecl game_get_grenade_type_index_from_item_defintion(int32 item_definition_index);

