#pragma once

#include "items/item_definitions.hpp"

enum e_weapon_definition_flags
{
	_weapon_vertical_heat_display_bit = 0,
	_weapon_mutually_exclusive_triggers_bit,
	_weapon_attacks_automatically_on_bump_bit,
	_weapon_must_be_readied_bit,
	_weapon_doesnt_count_toward_maximum_bit,
	_weapon_aim_assists_only_when_zoomed_bit,
	_weapon_prevents_grenade_throwing_bit,
	_weapon_must_be_picked_up_bit,
	_weapon_holds_triggers_when_dropped_bit,
	_weapon_prevents_melee_attack_bit,
	_weapon_detonates_when_dropped_bit,
	_weapon_cannot_fire_at_maximum_age_bit,
	_weapon_secondary_trigger_overrides_grenades_bit,
	_weapon_support_weapon_bit,
	_weapon_enables_integrated_night_vision_bit,
	_weapon_ais_use_weapon_melee_damage_bit,
	_weapon_forces_no_binoculars_bit,
	_weapon_loop_fp_firing_animation_bit,
	_weapon_prevents_crouching_bit,
	_weapon_cannot_fire_while_boosting_bit,
	_weapon_use_empty_melee_on_empty_bit,
	_weapon_uses_3rd_person_camera_bit,
	_weapon_can_be_dual_wielded_bit,
	_weapon_can_only_be_dual_wielded_bit,
	_weapon_melee_only_bit,
	_weapon_cannot_fire_if_parent_dead_bit,
	_weapon_weapon_ages_with_each_kill_bit,
	_weapon_weapon_uses_old_dual_fire_error_code_bit,
	_weapon_primary_trigger_melee_attacks_bit,
	_weapon_cannot_be_used_by_player_bit,
	_weapon_hold_fp_firing_animation_bit,

	// deviation angle is allowed to be less than primary autoaim angle - for the rocket launcher
	_weapon_strict_deviation_angle_bit,

	k_weapon_definition_flags
};

enum e_weapon_definition_secondary_flags
{
	_weapon_secondary_magnitizes_only_when_zoomed_bit = 0,
	_weapon_secondary_force_enable_equipment_tossing_bit,

	// melee-physics dash is disabled on melees that are not lunges
	_weapon_secondary_non_lunge_melee_dash_disabled_bit,

	_weapon_secondary_doesnt_use_player_control_crosshair_location_bit,
	_weapon_secondary_bypass_jump_aim_screen_hack_bit,
	_weapon_secondary_bit5,
	_weapon_secondary_bit6,
	_weapon_secondary_bit7,

	k_weapon_definition_secondary_flags
};

enum e_multiplayer_weapon_type
{
	_multiplayer_weapon_type_none = 0,
	_multiplayer_weapon_type_ctf_flag,
	_multiplayer_weapon_type_oddball_ball,
	_multiplayer_weapon_type_headhunter_head,
	_multiplayer_weapon_type_juggernaut_powerup,

	k_multiplayer_weapon_type_count
};

enum e_weapon_type
{
	_weapon_type_undefined = 0,
	_weapon_type_shotgun,
	_weapon_type_needler,
	_weapon_type_plasma_pistol,
	_weapon_type_plasma_rifle,
	_weapon_type_rocket_launcher,
	_weapon_type_energy_sword,
	_weapon_type_spartan_laser,
	_weapon_type_assault_rifle,
	_weapon_type_battle_rifle,
	_weapon_type_dmr,
	_weapon_type_magnum,
	_weapon_type_sniper_rifle,
	_weapon_type_smg,

	k_weapon_type_count
};

struct melee_damage_parameters_struct
{
	real_euler_angles2d damage_pyramid_angles;
	real32 damage_pyramid_depth;
	s_tag_reference first_hit_damage;
	s_tag_reference first_hit_response;
	s_tag_reference second_hit_damage;
	s_tag_reference second_hit_response;
	s_tag_reference third_hit_damage;
	s_tag_reference third_hit_response;
	s_tag_reference lunge_melee_damage;
	s_tag_reference lunge_melee_response;
	s_tag_reference empty_melee_damage;
	s_tag_reference empty_melee_response;
	s_tag_reference clang_melee_damage;
	s_tag_reference clang_melee_response;

	void update_reference_names();
};
static_assert(sizeof(melee_damage_parameters_struct) == 0xCC);

struct aim_assist_parameters
{
	real32 autoaim_angle;
	real32 autoaim_range;
	real32 autoaim_falloff_range;
	real32 autoaim_near_falloff_range;
	real32 magnetism_angle;
	real32 magnetism_range;
	real32 magnetism_falloff_range;
	real32 magnetism_near_falloff_range;
	real32 deviation_angle;
	byte ZHV[0x4];
	byte CVYGPMLMX[0xC];
	byte UQXKLVAXI[0x4];
};
static_assert(sizeof(aim_assist_parameters) == 0x38);

struct weapon_tracking
{
	int16 tracking_type;
	byte F[0x2];
};
static_assert(sizeof(weapon_tracking) == 0x4);

struct weapon_interface_definition_new
{
	s_tag_block first_person;
	s_tag_reference chud_interface;

	void update_reference_names();
};
static_assert(sizeof(weapon_interface_definition_new) == 0x1C);

struct s_weapon_magazine
{
	uns32 flags;
	int16 rounds_recharged; // per second
	int16 rounds_total_initial;
	int16 rounds_total_maximum;
	int16 rounds_loaded_maximum;
	int16 runtime_rounds_inventory_maximum;

	// pad
	byte IIO[0x2];

	// the length of time it takes to load a single magazine into the weapon
	real32 reload_time; // seconds

	int16 rounds_reloaded;

	// pad
	byte VJGZW[0x2];

	// the length of time it takes to chamber the next round
	real32 chamber_time; // seconds

	// pad
	byte HPMIV[0x8];

	// pad
	byte P[0x10];

	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> reloading_effect;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> reloading_damage_effect;
	c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> chambering_effect;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> chambering_damage_effect;
	s_tag_block magazines;

	void update_reference_names();
};
static_assert(sizeof(s_weapon_magazine) == 0x80);

enum e_weapon_trigger_definition_flags
{
	_weapon_trigger_definition_autofire_single_action_only_bit = 0,

	k_weapon_trigger_definition_flags
};

enum e_weapon_trigger_input
{
	_weapon_trigger_input_right_trigger = 0,
	_weapon_trigger_input_left_trigger,
	_weapon_trigger_input_melee_attack,

	k_weapon_trigger_inputs
};

enum e_weapon_trigger_behavior
{
	_weapon_trigger_behavior_spew = 0,
	_weapon_trigger_behavior_latch,
	_weapon_trigger_behavior_latch_autofire,
	_weapon_trigger_behavior_charge,
	_weapon_trigger_behavior_latch_zoom,
	_weapon_trigger_behavior_latch_rocket_launcher,
	_weapon_trigger_behavior_spew_charge,
	_weapon_trigger_behavior_sword_charge,

	k_weapon_trigger_behaviors
};

enum e_trigger_prediction_type
{
	_trigger_prediction_type_none = 0,
	_trigger_prediction_type_spew,
	_trigger_prediction_type_charge,

	k_trigger_prediction_types
};

enum e_weapon_trigger_autofire_action
{
	_weapon_trigger_autofire_action_fire = 0,
	_weapon_trigger_autofire_action_charge,
	_weapon_trigger_autofire_action_track,
	_weapon_trigger_autofire_action_fire_other,

	k_weapon_trigger_autofire_actions
};

enum e_weapon_trigger_overcharged_action
{
	_weapon_trigger_overcharged_action_none = 0,
	_weapon_trigger_overcharged_action_explode,
	_weapon_trigger_overcharged_action_discharge,

	k_weapon_trigger_overcharged_actions
};

struct weapon_trigger_definition
{
	struct s_autofire_fields
	{
		// AUTOFIRE

		real32 autofire_time;
		real32 autofire_throw;
		c_enum<e_weapon_trigger_autofire_action, int16, _weapon_trigger_autofire_action_fire, k_weapon_trigger_autofire_actions> secondary_action;
		c_enum<e_weapon_trigger_autofire_action, int16, _weapon_trigger_autofire_action_fire, k_weapon_trigger_autofire_actions> primary_action;
	};
	static_assert(sizeof(s_autofire_fields) == 0xC);

	struct s_charging_fields
	{
		// CHARGING

		// the amount of time it takes for this trigger to become fully charged
		real32 charging_time; // seconds

		// the amount of time this trigger can be charged before becoming overcharged
		real32 charged_time; // seconds

		c_enum<e_weapon_trigger_overcharged_action, int16, _weapon_trigger_overcharged_action_none, k_weapon_trigger_overcharged_actions> overcharged_action;

		// 96 was the constant in code for the pp
		int16 cancelled_trigger_throw;

		// the amount of illumination given off when the weapon is fully charged
		real32 charged_illumination; // [0,1]

		// length of time the weapon will spew (fire continuously) while discharging
		real32 spew_time; // seconds

		// the charging effect is created once when the trigger begins to charge
		c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> charging_effect;

		// the charging effect is created once when the trigger begins to charge
		c_typed_tag_reference<DAMAGE_EFFECT_TAG, DAMAGE_RESPONSE_DEFINITION_TAG, INVALID_TAG> charging_damage_effect;

		// plays every tick you're charging or charged, scaled to charging fraction
		c_typed_tag_reference<DAMAGE_RESPONSE_DEFINITION_TAG, INVALID_TAG> charging_continuous_damage_response;

		// how much battery to drain per second when charged
		real32 charged_drain_rate;

		// the discharging effect is created once when the trigger releases its charge
		c_typed_tag_reference<SOUND_TAG, EFFECT_TAG, INVALID_TAG> discharge_effect;

		// the discharging effect is created once when the trigger releases its charge
		c_typed_tag_reference<DAMAGE_EFFECT_TAG, DAMAGE_RESPONSE_DEFINITION_TAG, INVALID_TAG> discharge_damage_effect;

		void update_reference_names();
	};
	static_assert(sizeof(s_charging_fields) == 0x68);

	c_flags<e_weapon_trigger_definition_flags, uns32, k_weapon_trigger_definition_flags> flags;
	c_enum<e_weapon_trigger_input, int16, _weapon_trigger_input_right_trigger, k_weapon_trigger_inputs> input;
	c_enum<e_weapon_trigger_behavior, int16, _weapon_trigger_behavior_spew, k_weapon_trigger_behaviors> behavior;
	int16 primary_barrel;
	int16 secondary_barrel;
	c_enum<e_trigger_prediction_type, int16, _trigger_prediction_type_none, k_trigger_prediction_types> prediction;

	// pad
	byte GNFR[0x2];

	s_autofire_fields autofire;
	s_charging_fields charging;

	real32 lock_on_hold_time;
	real32 lock_on_acquire_time;
	real32 lock_on_grace_time;

	void update_reference_names();
};
static_assert(sizeof(weapon_trigger_definition) == 0x90);

struct s_weapon_barrel_firing_parameters
{
	// the number of firing effects created per second
	real32 rounds_per_second;

	// the continuous firing time it takes for the weapon to achieve its final rounds per second
	real32 acceleration_time; // seconds

	// the continuous idle time it takes for the weapon to return from its final rounds per second to its initial
	real32 deceleration_time; // seconds

	// scale the barrel spin speed by this amount
	real32 barrel_spin_scale;

	// a percentage between 0 and 1 which controls how soon in its firing animation the weapon blurs
	real32 blurred_rate_of_fire;

	// allows designer caps to the shots you can fire from one firing action
	real32 shots_per_fire;

	// how long after a set of shots it takes before the barrel can fire again
	real32 fire_recovery_time; // seconds

	// how much of the recovery allows shots to be queued
	real32 soft_recovery_fraction;

	real32 __unknown;
};
static_assert(sizeof(s_weapon_barrel_firing_parameters) == sizeof(real32) * 9);

struct s_weapon_barrel_firing_error
{
	// the continuous firing time it takes for the weapon to achieve its final error
	real32 acceleration_time; // seconds

	// the continuous idle time it takes for the weapon to return to its initial error
	real32 deceleration_time; // seconds

	// the range of angles (in degrees) that a damaged weapon will skew fire
	real32 damage_error;

	real32 min_error_look_pitch_rate; // yaw rate is doubled
	real32 full_error_look_pitch_rate; // yaw rate is doubled

	// use to soften or sharpen the rate ding
	real32 look_pitch_error_power;

	real32 __unknown;
};
static_assert(sizeof(s_weapon_barrel_firing_error) == sizeof(real32) * 7);

struct s_weapon_barrel_dual_weapon_error
{
	// the continuous firing time it takes for the weapon to achieve its final error
	real32 acceleration_time; // seconds

	// the continuous idle time it takes for the weapon to return to its initial error
	real32 deceleration_time; // seconds

	real32 runtime_acceleration_time;
	real32 runtime_deceleration_time;
	real32 minimum_error; // degrees
	real32 error_angle; // degrees
	real32 dual_wield_damage_scale;
	real32 __unknown;
};
static_assert(sizeof(s_weapon_barrel_dual_weapon_error) == sizeof(real32) * 8);

struct s_weapon_barrel
{
	uns32 flags;

	// firing
	s_weapon_barrel_firing_parameters firing;

	// the magazine from which this trigger draws its ammunition
	int16 magazine;

	// the number of rounds expended to create a single firing effect
	int16 rounds_per_shot;

	// the minimum number of rounds necessary to fire the weapon
	int16 minimum_rounds_loaded;

	// the number of non-tracer rounds fired between tracers
	int16 rounds_between_tracers;

	c_string_id optional_barrel_marker_name;


	// prediction properties
	// what the behavior of this barrel is in a predicted network game

	int16 prediction_type;

	// how loud this weapon appears to the AI
	int16 firing_noise;


	// error
	// full error look pitch rate controlls how fast you can turn
	// with full error, yaw is implied from pitch. 0==130.
	// for reference, profile sensitivities are set to:
	// 1: 40
	// 3: 60
	// 9: 130

	s_weapon_barrel_firing_error firing_error;

	// dual weapon error
	s_weapon_barrel_dual_weapon_error dual_weapon_error;


	// projectile

	int16 distribution_function;
	int16 projectiles_per_shot;
	real32 distribution_angle; // degrees
	real32 minimum_error; // degrees
	angle_bounds error_angle; // degrees

	// $TODO map the rest of this struct
	byte __data88[0x1AC - 0x88];
};
static_assert(sizeof(s_weapon_barrel) == 0x1AC);

struct _weapon_definition
{
	// $$$ WEAPON $$$

	// All weapons should have 'primary trigger' and 'secondary trigger' markers as appropriate.
	// Blurred permutations are called '$primary-blur' and '$secondary-blur'.

	c_flags<e_weapon_definition_flags, uns32, k_weapon_definition_flags> flags;
	c_flags<e_weapon_definition_secondary_flags, uns32, k_weapon_definition_secondary_flags> secondary_flags;
	c_string_id unused_label;
	int16 secondary_trigger_mode;

	// if the second trigger loads alternate ammunition, this is the maximum number of shots that can be loaded at a time
	int16 maximum_alternate_shots_loaded;

	// how long after being readied it takes this weapon to switch its 'turned_on' attachment to 1.0
	real32 turn_on_time;


	// old obsolete export to functions

	real32 ready_time; // seconds
	s_tag_reference ready_effect;
	s_tag_reference ready_damage_effect;


	// heat

	// the heat value a weapon must return to before leaving the overheated state, once it has become overheated in the first place
	real32 heat_recovery_threshold; // [0,1]

	// the heat value over which a weapon first becomes overheated (should be greater than the heat recovery threshold)
	real32 overheated_threshold; // [0,1]

	// the heat value above which the weapon has a chance of exploding each time it is fired
	real32 heat_detonation_threshold; // [0,1]

	// the percent chance (between 0.0 and 1.0) the weapon will explode when fired over the heat detonation threshold
	real32 heat_detonation_fraction; // [0,1]

	// the amount of heat lost each second when the weapon is not being fired
	real32 heat_loss_per_second; // [0,1]

	// the amount of illumination given off when the weapon is overheated
	real32 heat_illusion; // [0,1]

	// the amount of heat lost each second when the weapon is not being fired
	real32 overheated_heat_loss_per_second; // [0,1]

	s_tag_reference overheated;
	s_tag_reference overheated_damage_effect;
	s_tag_reference detonation;
	s_tag_reference detonation_damage_effect;
	s_tag_reference player_melee_damage;
	s_tag_reference player_melee_response;


	// melee aim assist

	// magnetism angle: the maximum angle that magnetism works at full strength
	// magnetism range: the maximum distance that magnetism works at full strength
	// throttle magnitude: additional throttle to apply towards target when melee-ing w/ magnetism
	// throttle minimum distance: minimum distance to target that throttle magnetism kicks in
	// throttle maximum adjustment angle: maximum angle throttle magnetism will have an effect, relative to the players movement throttle

	melee_damage_parameters_struct melee_damage_parameters;

	s_tag_reference clash_effect;
	c_enum<e_damage_reporting_type, int8, _damage_reporting_type_unknown, k_damage_reporting_type_count> melee_damage_reporting_type;

	// pad
	byte FEOROBJE[0x1];

	// the number of magnification levels this weapon allows
	int16 magnification_levels;
	real_bounds magnification_range;
	uns32 magnification_flags;
	real32 switch_ready_speed;


	// weapon aim assist

	aim_assist_parameters weapon_aim_assist;

	// Halo Online
	uns32 __unknown1DC;

	s_tag_block target_tracking;
	real32 ballistics0;
	real32 ballistics1;
	real32 ballistics2;
	real32 ballistics3;


	// movement

	int16 movement_penalized;

	// pad
	byte GTIXVRPA[0x2];

	real32 forward_movement_penalty;
	real32 sideways_movement_penalty;


	// AI targeting parameters
	real32 ai_scariness;


	// miscellaneous

	real32 weapon_power_on_time; // seconds
	real32 weapon_power_off_time; // seconds

	s_tag_reference weapon_power_on_effect;
	s_tag_reference weapon_power_off_effect;

	// how much the weapon's heat recovery is penalized as it ages
	real32 age_heat_recovery_penalty;

	// how much the weapon's rate of fire is penalized as it ages
	real32 age_rate_of_fire_penalty;

	// the age threshold when the weapon begins to misfire
	real32 age_misfire_start; // [0,1]

	// at age 1.0, the misfire chance per shot
	real32 age_misfire_chance; // [0,1]

	s_tag_reference pickup_sound;
	s_tag_reference zoom_in_sound;
	s_tag_reference zoom_out_sound;

	// how much to decrease active camo when a round is fired
	real32 active_camo_ding;

	// how fast to increase active camo (per tick) when a round is fired
	real32 active_camo_regrowth_rate;

	// the node that get's attached to the unit's hand
	c_string_id handle_node;

	// Halo Online
	real32 __unknown280; // STRING_ID(global, sprint_exit) related

	// weapon labels
	c_string_id weapon_class;
	c_string_id weapon_name;
	c_enum<e_multiplayer_weapon_type, int16, _multiplayer_weapon_type_none, k_multiplayer_weapon_type_count> multiplayer_weapon_type;


	// more miscellaneous

	c_enum<e_weapon_type, int16, _weapon_type_undefined, k_weapon_type_count> weapon_type;
	weapon_tracking tracking;
	int32 special_hud_version;
	int32 special_hud_icon;
	weapon_interface_definition_new player_interface;
	s_tag_block predicted_resources;
	c_typed_tag_block<s_weapon_magazine> magazines;
	c_typed_tag_block<weapon_trigger_definition> triggers;
	c_typed_tag_block<s_weapon_barrel> barrels;
	real32 runtime_weapon_power_on_velocity;
	real32 runtime_weapon_power_off_velocity;


	// first-person movement control

	real32 max_movement_acceleration;
	real32 max_movement_velocity;
	real32 max_turning_acceleration;
	real32 max_turning_velocity;
	s_tag_reference deployed_vehicle;
	s_tag_reference tossed_weapon;
	s_tag_reference age_effect;
	s_tag_reference age_weapon;
	s_tag_reference age_material_effects;
	real32 external_aging_amount;
	real32 campaign_external_aging_amount;
	real_vector3d first_person_weapon_offset;
	real_vector2d first_person_scope_size;

	// range in degrees. 0 is straight, -90 is down, 90 is up
	real_bounds support_third_person_camera_range; // degrees

	real32 weapon_zoom_time; // seconds
	real32 weapon_ready_for_use_time; // seconds
	real32 target_obstructed_max_distance; // wu
	c_string_id unit_stow_anchor_name;

	void update_reference_names();
};
static_assert(sizeof(_weapon_definition) == 0x384);
static_assert(0x191 == OFFSETOF(_weapon_definition, FEOROBJE));
static_assert(0x1DC == OFFSETOF(_weapon_definition, __unknown1DC));
static_assert(0x1FE == OFFSETOF(_weapon_definition, GTIXVRPA));
static_assert(0x280 == OFFSETOF(_weapon_definition, __unknown280));

struct weapon_definition
{
	static tag const k_group_tag = WEAPON_TAG;

	_object_definition object;
	_item_definition item;
	_weapon_definition weapon;

	void update_reference_names();
};
static_assert(sizeof(weapon_definition) == sizeof(_weapon_definition) + sizeof(_item_definition) + sizeof(_object_definition));


