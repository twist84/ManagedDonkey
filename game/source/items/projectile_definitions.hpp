#pragma once

#include "cseries/cseries.hpp"
#include "objects/object_definitions.hpp"
#include "shell/shell.hpp"
#include "tag_files/tag_groups.hpp"

enum e_projectile_flags
{
	_projectile_flag_oriented_along_velocity_bit = 0,
	_projectile_flag_ai_must_use_ballistic_aiming_bit,
	_projectile_flag_detonation_max_time_if_attached_bit,
	_projectile_flag_has_super_combining_explosion_bit,
	_projectile_flag_damage_scales_based_on_distance_bit,
	_projectile_flag_travels_instantaneously_bit,
	_projectile_flag_steering_adjusts_orientation_bit,
	_projectile_flag_dont_noise_up_steering_bit,
	_projectile_flag_can_track_behind_itself_bit,
	_projectile_flag_robotron_steering_bit,
	_projectile_flag_affected_by_phantom_volumes_bit,
	_projectile_flag_expensive_chubby_test_bit,
	_projectile_flag_notifies_target_units_bit,
	_projectile_flag_use_ground_detonation_when_attached_bit,
	_projectile_flag_ai_minor_tracking_threat_bit,
	_projectile_flag_dangerous_when_inactive_bit,
	_projectile_flag_ai_stimulus_when_attached_bit,
	_projectile_flag_overpenedetonation_bit,
	_projectile_flag_no_impact_effects_on_bounce_bit,
	_projectile_flag_rc1_overpenetration_fixes_bit,

	// for equipment?
	_projectile_flag_unknown_bit20,

	k_projectile_flags
};

enum e_projectile_detonation_timer_mode
{
	_projectile_detonation_timer_mode_immediately = 0,
	_projectile_detonation_timer_mode_after_first_bounce,
	_projectile_detonation_timer_mode_when_at_rest,
	_projectile_detonation_timer_mode_after_first_bounce_off_any_surface,

	k_projectile_detonation_timer_mode_count
};

struct s_projectile_material_response_definition;
struct s_projectile_brute_grenade_definition;
struct s_projectile_fire_bomb_grenade_definition;
struct s_projectile_conical_projection_defintion;
struct _projectile_definition
{
	// $$$ PROJECTILE $$$

	c_flags<e_projectile_flags, dword, k_projectile_flags> flags;
	c_enum<e_projectile_detonation_timer_mode, short, _projectile_detonation_timer_mode_immediately, k_projectile_detonation_timer_mode_count> detonation_timer_starts;
	c_enum<e_ai_sound_volume, short, _ai_sound_volume_silent, k_ai_sound_volume_count> impact_noise;
	real collision_radius; // world units

	// won't detonate before this time elapses
	real arming_time; // seconds

	real danger_radius; // world units

	// detonation countdown (zero is untimed)
	real_bounds timer; // seconds

	// detonates when slowed below this velocity
	real minimum_velocity; // world units per second

	// detonates after travelling this distance
	real maximum_range; // world units

	// detonates after travelling this distance, but is reset after a bounce.
	// Combines with maximum range
	real bounce_maximum_range; // world units

	c_enum<e_ai_sound_volume, short, _ai_sound_volume_silent, k_ai_sound_volume_count> detonation_noise;
	short super_detonation_projectile_count;
	real super_detonation_time;
	c_typed_tag_reference<EFFECT_TAG> detonation_started;
	c_typed_tag_reference<EFFECT_TAG> detonation_effect_airborne;
	c_typed_tag_reference<EFFECT_TAG> detonation_effect_ground;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> detonation_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> attached_detonation_damage;
	c_typed_tag_reference<EFFECT_TAG> super_detonation;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> super_detonation_damage; // your momma
	c_typed_tag_reference<SOUND_TAG> detonation_sound;

	c_enum<e_damage_reporting_type, short, _damage_reporting_type_unknown, k_damage_reporting_type_count> damage_reporting_type;

	// pad
	byte UAQLONXGN[1];

	c_enum<e_object_type, char, _object_type_biped, k_object_type_count> super_detonation_object_types;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> super_attached_detonation_damage;

	// radius within we will generate material effects
	real material_effect_radius;

	// flyby/impact
	c_typed_tag_reference<SOUND_TAG> flyby_sound;
	c_typed_tag_reference<EFFECT_TAG> impact_effect;
	c_typed_tag_reference<EFFECT_TAG> object_impact_effect;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> impact_damage;

	// boarding fields
	real boarding_detonation_time;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> boarding_detonation_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG> boarding_attached_detonation_damage;

	// physics

	// the proportion of normal gravity applied to the projectile when in air.
	real air_gravity_scale;

	// the range over which damage is scaled when the projectile is in air.
	real_bounds air_damage_range; // world units

	// the proportion of normal gravity applied to the projectile when in water.
	real water_gravity_scale;

	// the range over which damage is scaled when the projectile is in water.
	real_bounds water_damage_range; // world units

	// bullet's velocity when inflicting maximum damage
	real initial_velocity; // world units per second

	// bullet's velocity when inflicting minimum damage
	real final_velocity; // world units per second

	// base velocity used for ballistics calculations for indirect firing.
	real indirect_fire_velocity; // world units per second

	// scale on the initial velocity when fired by the ai on normal difficulty
	real ai_velocity_scale_normal; // [0-1]

	// scale on the initial velocity when fired by the ai on legendary difficulty (0 defaults to 1.0)
	real ai_velocity_scale_legendary; // [0-1]

	angle guided_angular_velocity_lower; // degrees per second
	angle guided_angular_velocity_upper; // degrees per second
	angle guided_angular_velocity_at_rest; // degrees per second

	// what distance range the projectile goes from initial velocity to final velocity
	real_bounds acceleration_range; // world units

	real runtime_acceleration_bound_inverse;
	real_fraction targeted_leading_fraction;
	real guided_projectile_outer_range_error_radius;
	real autoaim_leading_max_lead_time;
	c_typed_tag_block<s_projectile_material_response_definition> material_responses;
	c_typed_tag_block<s_projectile_brute_grenade_definition> brute_grenade;
	c_typed_tag_block<s_projectile_fire_bomb_grenade_definition> fire_bomb_grenade;
	c_typed_tag_block<s_projectile_conical_projection_defintion> conical_spread;

	void update_reference_names();
};
static_assert(sizeof(_projectile_definition) == 0x1AC);

struct projectile_definition
{
	static tag const k_group_tag = PROJECTILE_TAG;

	_object_definition object;
	_projectile_definition projectile;

	void update_reference_names();
};
static_assert(sizeof(projectile_definition) == sizeof(_projectile_definition) + sizeof(_object_definition));

struct s_projectile_material_response_definition
{
	word_flags flags;

	// default result
	// (if the potential result, below, fails to happen)

	short_enum default_response;
	c_string_id material_name;
	short runtime_material_index;

	// pad
	byte JJHT[2];

	// potential result
	short_enum potential_response;
	word_flags response_flags;
	real_fraction chance_fraction; // [0,1]
	angle_bounds between; // degrees
	real_bounds _and; // world units per second

	// misc
	short_enum scale_effects_by;

	// pad
	byte BDFI[2];

	// the angle of incidence is randomly perturbed by at most this amount to simulate irregularity.
	angle angular_noise; // degrees

	// the velocity is randomly perturbed by at most this amount to simulate irregularity.
	angle velocity_noise; // world units per second

	// penetration
	// the fraction of the projectile's velocity lost on penetration
	real initial_friction;

	// the maximum distance the projectile can travel through on object of this material
	real maximum_distance;

	// reflection
	// the fraction of the projectile's velocity parallel to the surface lost on impact
	real parallel_friction;

	// the fraction of the projectile's velocity perpendicular to the surface lost on impact
	real perpendicular_friction;
};
static_assert(sizeof(s_projectile_material_response_definition) == 0x40);

struct s_projectile_brute_grenade_definition
{
	angle minimum_angular_velocity; // degrees/sec
	angle maximum_angular_velocity; // degrees/sec
	angle spin_angular_velocity; // degrees/sec

	// 0==nothing 30==almost comlete damping
	real angular_damping;

	real drag_angle_k;
	real drag_speed_k;
	real drag_exponent;

	real attach_sample_radius;
	real attach_acceleration_k;
	real attach_acceleration_s;
	real attach_acceleration_e;
	real attach_acceleration_damping;
};
static_assert(sizeof(s_projectile_brute_grenade_definition) == 0x30);

struct s_projectile_fire_bomb_grenade_definition
{
	real projection_offset;
};
static_assert(sizeof(s_projectile_fire_bomb_grenade_definition) == sizeof(real));

struct s_projectile_conical_projection_defintion
{
	// conical_projection
	// projectile_count = yaw_count * pitch_count
	short yaw_count;
	short pitch_count;

	// exp==.5 even distribution,
	// exp==1  halo2 distribution,
	// exp>1== weighted towards center
	real distribution_exponent;

	angle spread; // degrees
};
static_assert(sizeof(s_projectile_conical_projection_defintion) == 0xC);

