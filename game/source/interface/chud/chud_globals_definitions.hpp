#pragma once

#include "cseries/cseries.hpp"
#include "math/function_definitions.hpp"
#include "shell/shell.hpp"
#include "tag_files/tag_groups.hpp"

struct s_chud_skin_info;
struct s_chud_shader;
struct s_chud_suck_profile;
struct s_cortana_effect_config;
struct s_player_training_entry_data;
struct s_chud_globals_definition
{
	static tag const k_group_tag = CHUD_GLOBALS_DEFINITION_TAG;

	c_typed_tag_block<s_chud_skin_info> skins;

	// Shaders
	// 00: simple shader
	// 01: meter shader
	// 02: text simple shader
	// 03: meter shield shader
	// 04: meter gradient shader
	// 05: crosshair shader
	// 06: directional damage
	// 07: solid
	// 08: sensor
	// 09: meter single color
	// 10: navpoints
	// 11: medals
	// 12: texture cam
	// 13: cortana screen
	// 14: cortana camera
	// 15: cortana offscreen
	// 16: cortana screen final
	// 17: meter chapter
	// 18: meter double gradient
	// 19: meter radial gradient
	// 20: distortion and blur
	// 21: emblem
	// 22: cortana composite
	// 23: directional damage apply
	// 24: really simple shader
	// 25: unknown
	c_typed_tag_block<s_chud_shader> shaders;

	c_typed_tag_block<s_chud_suck_profile> suck_profiles;
	c_typed_tag_block<s_cortana_effect_config> cortana_configs;

	// Player Training
	c_typed_tag_block<s_player_training_entry_data> player_training_data;

	// Miscellany
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> metagame_emblems;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> metagame_medals;
	c_typed_tag_reference<CHUD_ANIMATION_DEFINITION_TAG, INVALID_TAG> metagame_medal_animation;
	short metagame_medal_basis;
	byte __pad6E[0x2];
	real metagame_medal_scale;
	real metagame_medal_width;
	real metagame_medal_horizontal_offset;
	real metagame_medal_vertical_offset;
	real metagame_scoreboard_primary_height;
	real metagame_scoreboard_other_height;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> directional_damage_microtexture;
	real directional_damage_micro_scale;

	// Motion Sensor
	real medium_blip_scale;
	real small_blip_scale;
	real large_blip_scale;
	real max_age_size;
	real size_power;
	real alpha_power;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> blip_texture_reference;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> grunt_birthday_effect;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> tentacle_porn;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> flood_goo;
	real motion_sensor_level_height_range;
	real shield_minor_threshold;
	real shield_major_threshold;
	real shield_critical_threshold;
	real health_minor_threshold;
	real health_major_threshold;
	real health_critical_threshold;

	// health effect intensity
	real_argb_color health_multiply_colors[2];
	real_argb_color health_additive_colors[2];
	c_function_definition health_effect_intensity;

	// shield effect intensity
	real_argb_color shield_multiply_colors[2];
	real_argb_color shield_additive_colors[2];
	c_function_definition shield_effect_intensity;

	real sprint_fov_multiplier;
	real sprint_fov_transition_in_time;
	real sprint_fov_transition_out_time;

	c_typed_tag_reference<CHUD_WIDGET_PARALLAX_DATA_TAG, INVALID_TAG> parallax_data;

	struct
	{
		real period;
		c_function_definition horizontal;
		c_function_definition vertical;
		c_function_definition yaw;
		c_function_definition pitch;
		c_function_definition roll;
	} camera_shake[2];

	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> survival_mode_multiplayer_intro;
	real achievement_toast_time; // seconds

	void update_reference_names();
};
static_assert(sizeof(s_chud_globals_definition) == 0x2B8);

enum e_chud_skin_type
{
	_chud_skin_type_default = 0,
	_chud_skin_type_dervish,
	_chud_skin_type_monitor,

	_chud_skin_type_mp_ffa,
	_chud_skin_type_mp_red_team,
	_chud_skin_type_mp_blue_team,

	k_chud_skin_type_count
};

enum e_chud_curvature_res_flags
{
	_chud_curvature_res_720p_fullscreen_bit = 0,
	_chud_curvature_res_720p_halfscreen_bit,
	_chud_curvature_res_480p_fullscreen_bit,
	_chud_curvature_res_480i_fullscreen_bit,
	_chud_curvature_res_720p_quarterscreen_bit,
	_chud_curvature_res_480_halfscreen_bit,
	_chud_curvature_res_480p_quarterscreen_bit,
	_chud_curvature_res_480i_quarterscreen_bit,

	k_chud_curvature_res_flags
};

struct s_chud_curvature_info
{
	c_flags<e_chud_curvature_res_flags, dword, k_chud_curvature_res_flags> res_flags;
	angle source_fov_y;
	real souce_aspect;
	real destination_offset_z;
	long virtual_width;
	long virtual_height;
	real_point2d sensor_origin;
	real sensor_radius;
	real blip_radius;
	real global_safe_frame_horizontal;
	real global_safe_frame_vertical;
	real safe_frame_horizontal_ding;
	real safe_frame_vertical_ding;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> health_effect;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> third_person_health_effect;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> shield_effect;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> third_person_shield_effect;

	real state_message_scale;
	real_point2d state_message_offset;

	real __unknown1;
	real __unknown2;
	real __unknown3;
	real __unknown4;

	real message_anchor_vertical_offset;
	real state_message_vertical_offset;
	real medal_scale;
	real medal_width;

	real_point2d __unknown5;
	real __unknown6;
	real_point2d __unknown7;

	real message_scale;
	real message_width;
	long message_count_delta;

	real_point2d __unknown8;
	real __unknown9;
	real __unknown10;
	real __unknown11;
	real_point2d __unknown12;

	real pda_message_vertical_offset;
	real pda_message_horizontal_offset;

	void update_reference_names();
};
static_assert(sizeof(s_chud_curvature_info) == 0xE8);

enum e_chud_sound_cue_flags
{
	_chud_sound_cue_health_recharging_bit = 0,
	_chud_sound_cue_health_minor_damaged_bit,
	_chud_sound_cue_health_major_damaged_bit,
	_chud_sound_cue_health_critical_damaged_bit,
	_chud_sound_cue_health_minor_bit,
	_chud_sound_cue_health_major_bit,
	_chud_sound_cue_health_critical_bit,
	_chud_sound_cue_shield_recharging_bit,
	_chud_sound_cue_shield_minor_damaged_bit,
	_chud_sound_cue_shield_major_damaged_bit,
	_chud_sound_cue_shield_critical_damaged_bit,
	_chud_sound_cue_shield_minor_bit,
	_chud_sound_cue_shield_major_bit,
	_chud_sound_cue_shield_critical_bit,
	_chud_sound_cue_rocket_locking_bit,
	_chud_sound_cue_rocket_locked_bit,
	_chud_sound_cue_tracked_target_bit,
	_chud_sound_cue_locked_target_bit,
	_chud_sound_cue_vip_bit,
	_chud_sound_cue_juggernaut_bit,
	_chud_sound_cue_zombie_bit,
	_chud_sound_cue_last_man_standing_bit,
	_chud_sound_cue_stamina_full_bit,
	_chud_sound_cue_stamina_warning_bit,
	_chud_sound_cue_stamina_recharge_bit,
	_chud_sound_cue_bit25,
	_chud_sound_cue_bit26,
	_chud_sound_cue_bit27,
	_chud_sound_cue_tactical_package_error_bit,
	_chud_sound_cue_tactical_package_used_bit,
	_chud_sound_cue_gain_medal_bit,
	_chud_sound_cue_winning_points_bit,

	k_chud_sound_cue_flags
};

struct s_chud_sound_character_definition
{
	c_enum<e_odst_recon_variant, char, _odst_recon_variant_none, k_odst_recon_variant_count> character;

	// pad
	byte HKJNCIUW[0x3];

	c_typed_tag_reference<SOUND_TAG, SOUND_LOOPING_TAG, INVALID_TAG> sound;

	void update_reference_names();
};
static_assert(sizeof(s_chud_sound_character_definition) == 0x14);

struct chud_sound_element_definition
{
	c_flags<e_chud_sound_cue_flags, dword, k_chud_sound_cue_flags> latched_to;
	real scale;
	c_typed_tag_block<s_chud_sound_character_definition> characters;

	void update_reference_names();
};
static_assert(sizeof(chud_sound_element_definition) == 0x14);

struct s_chud_medal_name
{
	c_string_id medal_name;
};
static_assert(sizeof(s_chud_medal_name) == sizeof(c_string_id));

struct s_chud_config_info
{
	real_point2d grenade_anchor_position;
	real grenade_anchor_offset;
	real backpack_equipment_verticle_offset_single;
	real backpack_equipment_verticle_offset_dual;
	real backpack_equipment_verticle_offset_none;
	real equipment_horizontal_size;
	real scoreboard_spacing_size;
};
static_assert(sizeof(s_chud_config_info) == 0x20);

struct s_chud_skin_info
{
	c_enum<e_chud_skin_type, long, _chud_skin_type_default, k_chud_skin_type_count> type;
	argb_color primary_background;
	argb_color secondary_background;
	argb_color highlight_background;
	argb_color flash_background;
	argb_color crosshair_normal;
	argb_color crosshair_enemy;
	argb_color crosshair_friendly;
	argb_color blip_base;
	argb_color blip_self;
	argb_color blip_enemy;
	argb_color blip_neutral;
	argb_color blip_friend;
	argb_color blip_ping;
	argb_color blip_custom_onscreen;
	argb_color blip_custom_offscreen;
	argb_color navpoint_normal;
	argb_color navpoint_neutral;
	argb_color navpoint_enemy;
	argb_color navpoint_dead;
	argb_color navpoint_text;
	argb_color message_flash_self;
	argb_color message_flash_friend;
	argb_color message_flash_enemy;
	argb_color message_flash_neutral;
	argb_color invincible_shield;
	argb_color player_navpoint_standing_by;
	argb_color player_navpoint_firing;
	argb_color player_navpoint_taking_damage;
	argb_color player_navpoint_speaking;
	argb_color neutral_territory;
	argb_color default_item_outline;
	argb_color magazine_item_outline;
	argb_color damage_item_outline;
	argb_color accuracy_item_outline;
	argb_color fastfire_item_outline;
	argb_color range_item_outline;
	argb_color power_item_outline;
	c_typed_tag_block<s_chud_curvature_info> curvature_infos;
	c_typed_tag_block<chud_sound_element_definition> hud_sounds;
	c_typed_tag_reference<SOUND_TAG, SOUND_LOOPING_TAG, INVALID_TAG> banned_vehicle_entrance_sound;
	c_typed_tag_reference<SOUND_TAG, SOUND_LOOPING_TAG, INVALID_TAG> grenade_switch_frag;
	c_typed_tag_reference<SOUND_TAG, SOUND_LOOPING_TAG, INVALID_TAG> grenade_switch_plasma;
	c_typed_tag_reference<SOUND_TAG, SOUND_LOOPING_TAG, INVALID_TAG> grenade_switch_claymore;
	c_typed_tag_reference<SOUND_TAG, SOUND_LOOPING_TAG, INVALID_TAG> grenade_switch_firebomb;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> microtexture_reference;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> noise_texture_reference;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> damage_arrow_reference;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> grenade_waypoint_reference;

	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> unknown_reference;

	real direction_damage_duration_modifier;
	real direction_damage_hide_angle;
	real direction_damage_size;

	real __unknown15C;
	real __unknown160;
	real __unknown164;

	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> navpoints_reference;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> player_navpoints_reference;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> mini_scoreboard_reference;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> meta_scoreboard_reference;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> survival_scoreboard_reference;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> null_scoreboard_reference;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> saved_film_reference;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> sandbox_reference;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> chud_text;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> medals_texture_reference;
	c_typed_tag_block<s_chud_medal_name> medal_names;
	c_typed_tag_reference<CHUD_ANIMATION_DEFINITION_TAG, INVALID_TAG> multiplayer_medal_animation;
	c_typed_tag_reference<CHUD_ANIMATION_DEFINITION_TAG, INVALID_TAG> medal_animation;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> test_bitmap[3];
	c_typed_tag_reference<DAMAGE_RESPONSE_DEFINITION_TAG, INVALID_TAG> jammer_damage;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> jammer_damage_sound;
	s_chud_config_info config_info;
	real waypoint_minimum_distance_scale;
	real waypoint_maximum_distance_scale;
	real waypoint_scale;

	void update_reference_names();
};
static_assert(sizeof(s_chud_skin_info) == 0x2B0);

struct s_chud_shader
{
	c_typed_tag_reference<VERTEX_SHADER_TAG, INVALID_TAG> vertex_shader;
	c_typed_tag_reference<PIXEL_SHADER_TAG, INVALID_TAG> pixel_shader;

	void update_reference_names();
};
static_assert(sizeof(s_chud_shader) == sizeof(s_tag_reference) * 2);

struct s_chud_suck_profile
{
	real effect_radius;

	real vertex_noise_min;
	real vertex_noise_max;
	real vertex_noise_power;

	real pixel_noise_min;
	real pixel_noise_max;
	real pixel_noise_power;

	real warp_radius_min;
	real warp_radius_max;
	real warp_radius_power;

	real warp_intensity_min;
	real warp_intensity_max;
	real warp_intensity_power;

	real noise_sharpness_min;
	real noise_sharpness_max;
	real noise_sharpness_power;
};
static_assert(sizeof(s_chud_suck_profile) == sizeof(real) * 16);

struct s_cortana_effect_heading_config
{
	real_bounds noise_a_velocity;
	real_bounds noise_a_scale_x;
	real_bounds noise_a_scale_y;

	real_bounds noise_b_velocity;
	real_bounds noise_b_scale_x;
	real_bounds noise_b_scale_y;

	real_bounds pixelation_threshold;
	real_bounds pixelation_persistence;
	real_bounds pixelation_velocity;
	real_bounds pixelation_turbulence;

	real_bounds translation_scale_x;
	real_bounds translation_scale_y;

	c_typed_tag_reference<SOUND_TAG, SOUND_LOOPING_TAG, INVALID_TAG> sound_reference;

	void update_reference_names();
};
static_assert(sizeof(s_cortana_effect_heading_config) == (sizeof(real_bounds) * 12) + sizeof(s_tag_reference));

struct s_cortana_effect_distance_config
{
	// Cortana Effect config

	// You can set multiple configs for seperate distances
	// Configs are blended by distance and heading/oblique based on facing
	// 
	// The sound reference gets blended and the amplitude
	// of the blended value is what blends between min/max.
	// 
	// noise_velocity: how fast the noise bases interpolate
	// noise scale x/y: how large or small the noise is
	// 
	//  - PIXELATION (controlled by noise A)
	// pixelation threshold: controls the rate at which pixelation occurs
	// pixelation persistence: controls how long pixelated bits stick around
	// pixelation velocity: speed scape bits move towards cortana
	// pixelation turbulence: scale random bit movement
	// 
	//  - TRANSLATION (controlled by noise B)
	// translation scale x/y: how much to accept noise contribution towards movement
	real distance;

	// Facing
	s_cortana_effect_heading_config facing;

	// Oblique
	s_cortana_effect_heading_config oblique;

	void update_reference_names();
};
static_assert(sizeof(s_cortana_effect_distance_config) == sizeof(real) + (sizeof(s_cortana_effect_heading_config) * 2));

struct s_cortana_effect_config
{
	c_string_id name;
	c_typed_tag_block<s_cortana_effect_distance_config> distance_configs;

	void update_reference_names();
};
static_assert(sizeof(s_cortana_effect_config) == sizeof(string_id) + sizeof(s_tag_block));

enum e_player_training_flags
{
	_player_training_not_in_multiplayer_bit = 0,

	k_player_training_flags
};

struct s_player_training_entry_data
{
	// comes out of the HUD text globals
	c_string_id display_string;

	// how long the message can be on screen before being hidden
	short max_display_time;

	// how many times a training message will get displayed (0-3 only!)
	short display_count;

	// how long a displayed but untriggered message stays up
	short dissapear_delay;

	// how long after display this message will stay hidden
	short redisplay_delay;

	// how long the event can be triggered before it's displayed
	real display_delay; // seconds

	c_flags<e_player_training_flags, word, k_player_training_flags> flags;

	// pad
	byte HURLUV[0x2];
};
static_assert(sizeof(s_player_training_entry_data) == 0x14);

