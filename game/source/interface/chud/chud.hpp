#pragma once

#include "cseries/cseries.hpp"
#include "game/aim_target.hpp"
#include "interface/chud/chud_globals_definitions.hpp"
#include "interface/chud/chud_messaging.hpp"

struct s_aim_assist_targeting_result;

struct game_engine_interface_state
{
	enum e_variant_custom_flags
	{
		_variant_custom_a_available_bit = 0,
		_variant_custom_b_available_bit,
		_variant_custom_c_available_bit,

		k_number_of_variant_custom_flags,
	};

	int32 game_engine_type;
	wchar_t variant_name_string[32];
	bool in_sudden_death;
	bool timed_game;
	wchar_t time_left_string[20];
	bool round_starting;
	bool friendly_score_available;
	bool friendly_has_posession_marker;
	uns32 friendly_color;
	int32 friendly_score;
	bool enemy_score_available;
	bool enemy_has_posession_marker;
	uns32 enemy_color;
	int32 enemy_score;
	int32 enemy_team_or_player_index;
	bool friendly_over_enemy;
	int32 score_max;
	bool talking_player_available;
	wchar_t talking_player_name_string[32];
	bool arming_meter_available;
	real32 arming_meter_fraction;
	uns32 arming_meter_color;
	wchar_t arming_meter_string[64];
	int32 territory_count;
	uns32 territory_colors[8];
	real32 territory_thetas[8];
	int32 variant_custom_flags;

	struct
	{
		int32 crosshair_state;
		real32 budget_fraction;
		real32 budget_total;
		real32 budget_spent;
	} sandbox;
};
static_assert(sizeof(game_engine_interface_state) == 0x1B8);

struct chud_widget_datum :
	s_datum_header
{
	byte __data[0x16];
};
static_assert(sizeof(chud_widget_datum) == 0x18);

struct chud_player_hud_elements
{
	bool __unknown0;
	bool __unknown1;
	bool show_crosshair;
	bool show_shield;
	bool show_grenades;
	bool show_messages;
	bool show_motion_sensor;
	bool show_spike_grenades;
	bool show_fire_grenades;
	bool show_compass;
	bool show_stamina;
	bool show_energy_meters;
	bool show_consumables;
};
static_assert(sizeof(chud_player_hud_elements) == 0xD);

class c_chud_persistent_global_data
{
public:
	byte __data[0x14D];
	chud_player_hud_elements player_hud;
	byte __unknown181[0x316];
};
static_assert(sizeof(c_chud_persistent_global_data) == 0x470);

class c_chud_point_tracker
{
	bool m_active;
	real_point3d m_position;
	real_point2d m_position_screenspace;
	real_point2d m_position_virtual;
	real32 m_offscreen_amount;
	bool m_behind_camera;
	real32 m_behind_camera_yaw_sign;
};
static_assert(sizeof(c_chud_point_tracker) == 0x2C);

class c_chud_directional_damage
{
	struct s_instance
	{
		c_chud_point_tracker point_tracker;
		real_vector3d direction;
		int32 end_time;
		int32 creation_time;
		int16 fade_function;
		real32 size;
		real32 fade_alpha;
		real32 inner_scale;
		real32 outer_scale;
		real_argb_color color;
		real32 arrow_yaw;
		real32 arrow_behind_us_yaw_sign;
		real32 arrow_alpha;
	};
	static_assert(sizeof(s_instance) == 0x70);

	s_instance m_instances[4];
};
static_assert(sizeof(c_chud_directional_damage) == 0x1C0);

class c_chud_cinematic_fade
{
	real32 m_fade_current;
	real32 m_fade_desired;
	real32 m_fade_velocity;
};
static_assert(sizeof(c_chud_cinematic_fade) == 0xC);

class c_chud_damage_tracker
{
	real32 m_projectile_angles[4];
	int32 m_projectile_count;
};
static_assert(sizeof(c_chud_damage_tracker) == 0x14);

class c_chud_motion_sensor
{
	struct s_motion_sensor_tracked_entity
	{
		int32 tracked_object_index;
		real32 squared_distance;
		int32 ping_timer;
	};
	static_assert(sizeof(s_motion_sensor_tracked_entity) == 0xC);

	struct s_motion_sensor_blip
	{
		real_point2d position;
		real32 unused;
		uns8 blip_type;
		uns8 blip_size;
		uns8 blip_shape;
		byte pad[1];
	};
	static_assert(sizeof(s_motion_sensor_blip) == 0x10);

	struct s_motion_sensor_history
	{
		int32 blip_count;
		s_motion_sensor_blip blips[16];
	};
	static_assert(sizeof(s_motion_sensor_history) == 0x104);

	s_motion_sensor_tracked_entity m_tracked_entities[20];
	s_motion_sensor_history m_histories[10];
	s_motion_sensor_blip m_custom_blips[20];
	real32 m_custom_blip_alphas[20];
	uns32 m_multiplayertracking_flags;
	int32 m_custom_blip_count;
	int32 m_user_index;
	int32 m_recent_history_index;
	real32 m_tracking_distance;
	real32 m_radar_distance;
	real32 m_oo_radar_distance;
};
static_assert(sizeof(c_chud_motion_sensor) == 0xCC4);

class c_chud_cortana_effect
{
	struct s_basis
	{
		real_point2d origin;
		real_vector2d vec0;
	};
	static_assert(sizeof(s_basis) == 0x10);

	int32 m_suck_object_index;
	int32 m_suck_marker_id;
	bool m_suck_enabled;
	real32 m_range_multiplier;
	s_basis m_noise_a_bases[2];
	s_basis m_noise_b_bases[2];
	real32 m_noise_a_theta;
	real32 m_noise_b_theta;
	real_point3d m_effect_origin;
	c_chud_point_tracker m_effect_tracker;
	real32 m_effect_blend;
	real32 m_cortana_distance_t;
	real32 m_desired_power;
	real32 m_current_power;
	int32 m_sound_handles_near[2];
	int32 m_sound_handles_far[2];
	int32 m_near_config_flags;
	int32 m_far_config_flags;
	s_cortana_effect_heading_config m_blended_config;
};
static_assert(sizeof(c_chud_cortana_effect) == 0x128);

struct s_chud_shared_persistent_user_data
{
	struct s_metagame
	{
		int32 player_count;
		real_argb_color player_colors[4];
	};
	static_assert(sizeof(s_metagame) == 0x44);

	bool player_training_available;
	wchar_t player_training_text[256];
	int32 campaign_objective_timer;
	wchar_t campaign_objective_text[256];

	byte __data204[0x800];
	int32 special_hud_version;

	real32 backpack_and_equipment_vertical_offset;
	real32 backpack_horizontal_offset;
	wchar_t scoreboard_time_left[20];
	wchar_t scoreboard_arming_meter_name[32];
	wchar_t scoreboard_talking_player_name[32];
	wchar_t scoreboard_variant_name[32];
	int32 scoreboard_scores_max_width;
	uns32 scoreboard_friendly_color;
	uns32 scoreboard_enemy_color;
	uns32 arming_meter_color;
	real32 scoreboard_player_leading_fraction;
	bool scoreboard_player_leading_fraction_initialized;
	real32 first_person_theta;
	bool firebomb_before_spike;
	bool lockable_target_available;
	real_point3d lockable_target_point;
	c_chud_point_tracker lockable_target_tracker;
	c_chud_cortana_effect cortana_effect;

	byte __dataE7C[0x1D8];

	int32 first_drawn_normal_widget_index;
	int32 first_drawn_saved_film_widget_index;
	s_metagame metagame;
	real32 invincible_theta;
	real_vector4d saved_film_data[4];
	int32 thread_safe_current_resolution;
};
static_assert(sizeof(s_chud_shared_persistent_user_data) == 0x10E8);

struct s_chud_navpoint
{
	int32 navpoint_identifier;
	int32 arrow_type;
	int32 icon_type;
	int32 color_type;
	int32 flags;
	real32 alpha;
	wchar_t navpoint_text[6];
	real_point3d position_worldspace;
};
static_assert(sizeof(s_chud_navpoint) == 0x30);

class __declspec(align(4)) c_chud_navpoint_manager
{
public:
	struct s_managed_navpoint
	{
		s_chud_navpoint navpoint;
		bool valid;
		int32 last_valid_tick;
		real32 scale;
		c_chud_point_tracker point_tracker;
		int32 flash_type;
		int32 flash_timer;
		real32 fade_alpha;
	};
	static_assert(sizeof(s_managed_navpoint) == 0x74);

	s_managed_navpoint m_managed_navpoints[20];
	int32 m_named_player_index;
	wchar_t m_named_player_name[17];
	real32 m_name_theta;
	real32 m_named_player_name_width;
	int32 m_named_player_managed_navpoint_index;
	real32 m_distanced_theta;
	wchar_t m_distanced_text[10];
	real32 m_distanced_text_width;
	real32 m_distanced_managed_navpoint_index;
	int16 m_scripted_navpoints[8];
	real32 m_scripted_navpoint_vertical_offsets[8];
	int32 m_scripted_navpoint_count;
};
static_assert(sizeof(c_chud_navpoint_manager) == 0x998);

class c_chud_impulse_manager
{
	struct s_ammo_pickup
	{
		int32 definition_index;
		int32 count;
	};
	static_assert(sizeof(s_ammo_pickup) == 0x8);

	s_ammo_pickup m_ammo_pickups[3];
	int32 m_ammo_pickup_count;
	int32 m_grenade_pickups[4];
	byte __data2C[0xC];
};
static_assert(sizeof(c_chud_impulse_manager) == 0x38);

class c_chud_persistent_user_data
{
public:
	struct s_persistent_chud_definition_data
	{
		int32 chud_definition_index;
		int32 active_widget_count;
		int32 weapon_index;
	};
	static_assert(sizeof(s_persistent_chud_definition_data) == 0xC);

	struct s_shield_data
	{
		real32 last_shield_vitality;
		int32 fade_time;
		int32 last_shield_hit_time;
	};
	static_assert(sizeof(s_shield_data) == 0xC);

	struct s_body_data
	{
		real32 last_body_vitality;
		int32 fade_time;
		int32 last_body_hit_time;
	};
	static_assert(sizeof(s_body_data) == 0xC);

	struct s_flava_data
	{
		real32 target_distance;
		real32 target_elevation;
	};
	static_assert(sizeof(s_flava_data) == 0x8);

	struct s_stamina_data
	{
		real32 stamina;
		real32 last_stamina;
		real32 fov_multiplier;
	};
	static_assert(sizeof(s_stamina_data) == 0xC);

	struct s_consumable_data
	{
		real32 consumable;
		real32 last_consumable;
	};
	static_assert(sizeof(s_consumable_data) == 0x8);

public:
	s_persistent_chud_definition_data m_persistent_definitions[8];
	int32 m_updates_since_spawn;
	int32 m_user_index;
	int32 m_active_unit_index;
	bool m_texture_cam_active;
	bool m_previously_had_primary_and_backpack_weapons;
	bool m_force_initialization;
	uns64 m_alpha_watermark;
	c_chud_directional_damage m_directional_damage;
	c_chud_damage_tracker m_damage_tracker;
	c_chud_cinematic_fade m_cinematic_fade;
	c_chud_motion_sensor m_motion_sensor;
	s_chud_shared_persistent_user_data m_shared_data;
	c_chud_navpoint_manager m_navpoint_manager;
	c_chud_impulse_manager m_impulse_manager;
	c_chud_messaging_manager m_messaging_manager;
	s_shield_data m_shield_data;
	s_body_data m_body_data;
	s_flava_data m_flava_data;
	s_stamina_data m_stamina_data;
	s_consumable_data m_consumable_data;
	int32 m_last_sound_handles[32];
	uns16 m_sound_flags;
};
static_assert(sizeof(c_chud_persistent_user_data) == 0x3E90);

struct s_chud_weapon_state_flags
{
	uns16 impulse_state;
	uns16 armed_state;
	uns16 crosshair_state;
	uns16 misc_state;
};
static_assert(sizeof(s_chud_weapon_state_flags) == 0x8);

struct s_chud_global_state_flags
{
	uns16 game_state_flags;
	uns16 game_skin_state_flags;
	uns16 game_team_flags;
	uns16 window_state_flags;
	uns16 game_engine_state_flags;
	uns32 misc_state_flags;
	uns16 sandbox_state_flags;
	uns16 hindsight_state_flags;
};
static_assert(sizeof(s_chud_global_state_flags) == 0x14);

struct s_chud_unit_state_flags
{
	uns16 impulse_state;
	uns16 zoom_state;
	uns16 armed_state;
	uns32 misc_state;
};
static_assert(sizeof(s_chud_unit_state_flags) == 0xC);

struct s_chud_misc_state_flags
{
	uns16 hidden_state_flags;
	uns16 pad;
	uns32 __flags4;
	uns32 __flags8;
};
static_assert(sizeof(s_chud_misc_state_flags) == 0xC);

struct s_campaign_metagame_interface_state
{
	int32 active_player_count;
	int32 active_player_subtotals[4];
	bool active_player_talking[4];
	int16 change_color[4];
	real32 game_timer;
	real32 score_multiplier;
	real32 time_multiplier;
	int32 transient_score;
	bool multikill_available;
	int32 negative_transient_score;
	bool netagive_transient_score_available;
	bool __unknown39;
	int32 survival_bonus_score;
};
static_assert(sizeof(s_campaign_metagame_interface_state) == 0x40);

struct s_saved_film_hud_interface_state
{
	real32 duration_in_seconds;
	real32 marker_position_in_seconds;
	int32 number_of_chapters_available;
	real32 buffered_theta;
	real32 current_position_theta;
	real32 recording_start_theta;
	bool recording;
	c_static_array<real32, 10> chapter_mark_theta;
};
static_assert(sizeof(s_saved_film_hud_interface_state) == 0x44);

struct s_network_game_quality
{
	int32 latency;
	e_network_rough_quality latency_quality;
	e_network_rough_quality host_framerate;
	e_network_rough_quality local_framerate;
};
static_assert(sizeof(s_network_game_quality) == 0x10);

class c_chud_update_user_data
{
public:
	void __thiscall compute_weapon_update_(int32 weapon_index, int32 chud_definition_type, s_aim_assist_targeting_result* aim_assist_targeting);

public:
	enum e_chud_definition_weapon_type
	{
		_chud_definition_type_weapon_primary = 0,
		_chud_definition_type_weapon_secondary,
		_chud_definition_type_weapon_backpack,
		_chud_definition_type_weapon_support,
		_chud_definition_type_4,
		_chud_definition_type_weapon_parent,
		_chud_definition_type_unit,
		_chud_definition_type_unit_parent,
		_chud_definition_type_hologram_nav_point,
		_chud_definition_type_equipment,
		_chud_definition_type_scoreboard,
		_chud_definition_type_sandbox,
		_chud_definition_type_saved_film,
		_chud_definition_type_multiplayer_intro,
		_chud_definition_type_tank_mode,
		k_chud_definition_type_count,
	};


	struct s_chud_definition_info
	{
		struct s_chud_definition_weapon_state
		{
			int32 special_hud_icon;
			s_chud_weapon_state_flags weapon_flags;
			uns16 flash_flags;
			real32 heat;
			real32 age;
			real32 battery;
			real32 barrel_spin;
			real32 overheating;
			real32 __unknown24;
			real32 charge;
			real32 primary_clip_remaining;
			real32 primary_clip_max;
			real32 primary_total_remaining;
			real32 primary_total_max;
			bool reloading;
			bool fired;
			bool lock_available;
			bool locking;
			bool tracking;
			bool locked;
			real32 locking_theta;
			real_point3d lock_point;
		};
		static_assert(sizeof(s_chud_definition_weapon_state) == 0x54);

		int32 weapon_index;
		int32 chud_definition_index;
		s_chud_definition_weapon_state weapon_state;
	};
	static_assert(sizeof(s_chud_definition_info) == 0x5C);

	struct s_unit_state
	{
		bool valid;
		real32 primary_autoaim;
		real32 secondary_autoaim;
		real32 body_vitality;
		real32 shield_vitality;
		real32 maximum_shield_vitality;
		int8 damaged_explosion_timer;
		int32 current_grenade_index;
		int32 grenade_counts[4];
		bool is_single_wielding;
		bool is_dual_wielding;
		int32 zoom_level;
		bool grenades_disabled;
		int32 frag_grenade_count;
		int32 plasma_grenade_count;
		int32 damage_flags;
		bool binoculars_enabled;
		bool motion_sensor_enabled;
		real32 motion_sensor_range;

		bool __unknown50;

		bool shield_enabled;
		uns8 autoaim_flags;

		bool __unknown53;
		bool __unknown54;

		bool has_hologram_target_point;
		real_point3d hologram_target_point;

		bool __unknown64;
		int32 tank_mode_user_index;

		c_aim_target_object aim_target;
		bool has_multiplayer_object;

		byte __data79[0x1];

		int16 consumable_icon[4];
		int16 consumable_cooldown_text;
		real32 consumable_cooldown_meter;
		int32 consumable_index;

		bool __unknown8C;
		real32 __unknown90;
		real32 __unknown94;
		real32 __unknown98[4][2];

		real32 consumable_cost[4];
		real32 energy_meter[16];
		real32 stamina_current;
	};
	static_assert(sizeof(s_unit_state) == 0x10C);

public:
	int32 m_user_index;
	int32 m_player_index;
	int32 m_unit_index;
	int32 m_unit_definition_index;
	bool m_initializing;
	s_chud_definition_info m_incoming_definition_infos[15];
	s_chud_global_state_flags m_global_flags;

	byte __data58C[0xC];

	s_chud_unit_state_flags m_unit_flags;
	int32 m_flash_flags;
	s_chud_misc_state_flags m_misc_flags;
	s_unit_state m_unit_state;
	bool m_suppress_weapon_swap_animations;
	bool m_fresh_unit;
	game_engine_interface_state m_game_engine_state;
	s_campaign_metagame_interface_state m_metagame_state;
	s_saved_film_hud_interface_state m_saved_film_state;
	s_network_game_quality m_network_quality;
};
static_assert(sizeof(c_chud_update_user_data) == 0x910);

extern s_chud_globals_definition*& chud_globals;


extern void __cdecl chud_dispose();
extern void __cdecl chud_dispose_from_old_map();
extern void __cdecl chud_draw_screen(int32 user_index);
extern void __cdecl chud_draw_screen_LDR(int32 user_index);
extern void __cdecl chud_draw_screen_saved_film(int32 user_index);
extern void __cdecl chud_draw_turbulence(int32 user_index);
extern void __cdecl chud_game_tick();
extern bool __cdecl chud_generate_damage_flash_texture(int32 user_index);
extern void __cdecl chud_initialize();
extern void __cdecl chud_initialize_for_new_map();
extern bool __cdecl chud_should_draw_screen_saved_film(int32 user_index);
extern void __cdecl chud_submit_navpoint(int32, s_chud_navpoint* navpoint);
extern void __cdecl chud_update(real32 world_seconds_elapsed);

