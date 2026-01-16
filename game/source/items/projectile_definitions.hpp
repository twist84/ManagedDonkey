#pragma once

#include "cseries/cseries.hpp"
#include "objects/object_definitions.hpp"
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

	c_flags<e_projectile_flags, uns32, k_projectile_flags> flags;
	c_enum<e_projectile_detonation_timer_mode, int16, _projectile_detonation_timer_mode_immediately, k_projectile_detonation_timer_mode_count> detonation_timer_starts;
	c_enum<e_ai_sound_volume, int16, _ai_sound_volume_silent, k_ai_sound_volume_count> impact_noise;
	real32 collision_radius; // world units

	// won't detonate before this time elapses
	real32 arming_time; // seconds

	real32 danger_radius; // world units

	// detonation countdown (zero is untimed)
	real_bounds timer; // seconds

	// detonates when slowed below this velocity
	real32 minimum_velocity; // world units per second

	// detonates after travelling this distance
	real32 maximum_range; // world units

	// detonates after travelling this distance, but is reset after a bounce.
	// Combines with maximum range
	real32 bounce_maximum_range; // world units

	c_enum<e_ai_sound_volume, int16, _ai_sound_volume_silent, k_ai_sound_volume_count> detonation_noise;
	int16 super_detonation_projectile_count;
	real32 super_detonation_time;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> detonation_started;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> detonation_effect_airborne;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> detonation_effect_ground;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> detonation_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> attached_detonation_damage;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> super_detonation;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> super_detonation_damage; // your momma
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> detonation_sound;

	c_enum<e_damage_reporting_type, int16, _damage_reporting_type_unknown, k_damage_reporting_type_count> damage_reporting_type;

	// pad
	byte UAQLONXGN[0x1];

	c_enum<e_object_type, int8, _object_type_biped, k_object_type_count> super_detonation_object_types;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> super_attached_detonation_damage;

	// radius within we will generate material effects
	real32 material_effect_radius;

	// flyby/impact
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> flyby_sound;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> impact_effect;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> object_impact_effect;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> impact_damage;

	// boarding fields
	real32 boarding_detonation_time;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> boarding_detonation_damage;
	c_typed_tag_reference<DAMAGE_EFFECT_TAG, INVALID_TAG> boarding_attached_detonation_damage;

	// physics

	// the proportion of normal gravity applied to the projectile when in air.
	real32 air_gravity_scale;

	// the range over which damage is scaled when the projectile is in air.
	real_bounds air_damage_range; // world units

	// the proportion of normal gravity applied to the projectile when in water.
	real32 water_gravity_scale;

	// the range over which damage is scaled when the projectile is in water.
	real_bounds water_damage_range; // world units

	// bullet's velocity when inflicting maximum damage
	real32 initial_velocity; // world units per second

	// bullet's velocity when inflicting minimum damage
	real32 final_velocity; // world units per second

	// base velocity used for ballistics calculations for indirect firing.
	real32 indirect_fire_velocity; // world units per second

	// scale on the initial velocity when fired by the ai on normal difficulty
	real32 ai_velocity_scale_normal; // [0-1]

	// scale on the initial velocity when fired by the ai on legendary difficulty (0 defaults to 1.0)
	real32 ai_velocity_scale_legendary; // [0-1]

	real32 guided_angular_velocity_lower; // degrees per second
	real32 guided_angular_velocity_upper; // degrees per second
	real32 guided_angular_velocity_at_rest; // degrees per second

	// what distance range the projectile goes from initial velocity to final velocity
	real_bounds acceleration_range; // world units

	real32 runtime_acceleration_bound_inverse;
	real32 targeted_leading_fraction;
	real32 guided_projectile_outer_range_error_radius;
	real32 autoaim_leading_max_lead_time;
	c_typed_tag_block<s_projectile_material_response_definition> material_responses;
	c_typed_tag_block<s_projectile_brute_grenade_definition> brute_grenade;
	c_typed_tag_block<s_projectile_fire_bomb_grenade_definition> fire_bomb_grenade;
	c_typed_tag_block<s_projectile_conical_projection_defintion> conical_spread;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(_projectile_definition) == 0x1AC);

struct projectile_definition
{
	static tag const k_group_tag = PROJECTILE_TAG;

	_object_definition object;
	_projectile_definition projectile;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(projectile_definition) == sizeof(_projectile_definition) + sizeof(_object_definition));

struct s_projectile_material_response_definition
{
	uns16 flags;

	// default result
	// (if the potential result, below, fails to happen)

	int16 default_response;
	c_string_id material_name;
	int16 runtime_material_index;

	// pad
	byte JJHT[0x2];

	// potential result
	int16 potential_response;
	uns16 response_flags;
	real32 chance_fraction; // [0,1]
	angle_bounds between; // degrees
	real_bounds _and; // world units per second

	// misc
	int16 scale_effects_by;

	// pad
	byte BDFI[0x2];

	// the angle of incidence is randomly perturbed by at most this amount to simulate irregularity.
	real32 angular_noise; // degrees

	// the velocity is randomly perturbed by at most this amount to simulate irregularity.
	real32 velocity_noise; // world units per second

	// penetration
	// the fraction of the projectile's velocity lost on penetration
	real32 initial_friction;

	// the maximum distance the projectile can travel through on object of this material
	real32 maximum_distance;

	// reflection
	// the fraction of the projectile's velocity parallel to the surface lost on impact
	real32 parallel_friction;

	// the fraction of the projectile's velocity perpendicular to the surface lost on impact
	real32 perpendicular_friction;
};
COMPILE_ASSERT(sizeof(s_projectile_material_response_definition) == 0x40);

struct s_projectile_brute_grenade_definition
{
	real32 minimum_angular_velocity; // degrees/sec
	real32 maximum_angular_velocity; // degrees/sec
	real32 spin_angular_velocity; // degrees/sec

	// 0==nothing 30==almost comlete damping
	real32 angular_damping;

	real32 drag_angle_k;
	real32 drag_speed_k;
	real32 drag_exponent;

	real32 attach_sample_radius;
	real32 attach_acceleration_k;
	real32 attach_acceleration_s;
	real32 attach_acceleration_e;
	real32 attach_acceleration_damping;
};
COMPILE_ASSERT(sizeof(s_projectile_brute_grenade_definition) == 0x30);

struct s_projectile_fire_bomb_grenade_definition
{
	real32 projection_offset;
};
COMPILE_ASSERT(sizeof(s_projectile_fire_bomb_grenade_definition) == sizeof(real32));

struct s_projectile_conical_projection_defintion
{
	// conical_projection
	// projectile_count = yaw_count * pitch_count
	int16 yaw_count;
	int16 pitch_count;

	// exp==.5 even distribution,
	// exp==1  halo2 distribution,
	// exp>1== weighted towards center
	real32 distribution_exponent;

	real32 spread; // degrees
};
COMPILE_ASSERT(sizeof(s_projectile_conical_projection_defintion) == 0xC);

