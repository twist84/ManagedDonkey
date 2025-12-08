#pragma once

#include "cseries/cseries.hpp"
#include "game/materials.hpp"
#include "items/equipment_types.hpp"
#include "items/item_definitions.hpp"
#include "math/function_definitions.hpp"
#include "tag_files/tag_groups.hpp"

enum e_equipment_flags
{
	_equipment_pathfinding_obstacle_bit = 0,
	_equipment_equipment_is_dangerous_to_ai_bit,

	// if an actor dies while carrying this, it gets deleted immediately
	// does not affect dropping by players
	_equipment_never_dropped_by_ai_bit,

	_equipment_protects_parent_from_aoe_bit,
	_equipment_third_person_camera_always_bit,
	_equipment_use_forced_primary_change_color_bit,
	_equipment_use_forced_secondary_change_color_bit,
	_equipment_can_not_be_picked_up_by_player_bit,

	// used with supercombine attach
	_equipment_is_removed_from_world_on_deactivation_bit,

	_equipment_is_dropped_by_player_bit,
	_equipment_is_dropped_by_ai_bit,

	k_equipment_flags
};

struct s_equipment_type_super_shield;
struct s_equipment_type_multiplayer_powerup;
struct s_equipment_type_spawner;
struct s_equipment_type_proximity_mine;
struct s_equipment_type_motion_tracker_noise;
struct s_equipment_type_showme;
struct s_equipment_type_invisibility_mode;
struct s_equipment_type_invincibility;
struct s_equipment_type_treeoflife;
struct s_equipment_type_health_pack;
struct s_equipment_type_forced_reload;
struct s_equipment_type_concussive_blast;
struct s_equipment_type_tank_mode;
struct s_equipment_type_mag_pulse;
struct s_equipment_type_hologram;
struct s_equipment_type_reactive_armor;
struct s_equipment_type_bomb_run;
struct s_equipment_type_armor_lock;
struct s_equipment_type_adrenaline;
struct s_equipment_type_lightning_strike;
struct s_equipment_type_scrambler;
struct s_equipment_type_weapon_jammer;
struct s_equipment_type_ammo_pack;
struct s_equipment_type_vision;
struct _equipment_definition
{
	// $$$ EQUIPMENT $$$

	real32 duration;

	// time in seconds for the phantom volumes on this object to start accelerating things in them
	real32 phantom_volume_activation_time;

	// -1 means unlimited charges, 0 means fire on creation
	int16 charges;

	c_flags<e_equipment_flags, uns16, k_equipment_flags> flags;

	// AI
	// 
	// Settings which the AI use to know how they should react to this equipment

	// How large a danger zone we should create around this equipment (0 means no danger zone)
	real32 danger_radius;

	// How far does my target have to be for me to throw this at them?
	real32 min_deployment_distance; // wus

	// How long I should go unnoticed by nearby enemies
	real32 awareness_time; // seconds

	s_tag_block override_camera;

	union
	{
		struct
		{
			c_typed_tag_block<s_equipment_type_super_shield> super_shield;
			c_typed_tag_block<s_equipment_type_multiplayer_powerup> multiplayer_powerup;
			c_typed_tag_block<s_equipment_type_spawner> spawner;
			c_typed_tag_block<s_equipment_type_proximity_mine> proximity_mine;
			c_typed_tag_block<s_equipment_type_motion_tracker_noise> motion_tracker_noise;
			c_typed_tag_block<s_equipment_type_showme> showme;
			c_typed_tag_block<s_equipment_type_invisibility_mode> invisibility_mode;
			c_typed_tag_block<s_equipment_type_invincibility> invincibility_mode;
			c_typed_tag_block<s_equipment_type_treeoflife> tree_of_life;
			c_typed_tag_block<s_equipment_type_health_pack> health_pack;
			c_typed_tag_block<s_equipment_type_forced_reload> forced_reload;
			c_typed_tag_block<s_equipment_type_concussive_blast> concussive_blast;
			c_typed_tag_block<s_equipment_type_tank_mode> tank_mode;
			c_typed_tag_block<s_equipment_type_mag_pulse> mag_pulse;
			c_typed_tag_block<s_equipment_type_hologram> hologram;
			c_typed_tag_block<s_equipment_type_reactive_armor> reactive_armor;
			c_typed_tag_block<s_equipment_type_bomb_run> bomb_run;
			c_typed_tag_block<s_equipment_type_armor_lock> armor_lock;
			c_typed_tag_block<s_equipment_type_adrenaline> adrenaline;
			c_typed_tag_block<s_equipment_type_lightning_strike> lightning_strike;
			c_typed_tag_block<s_equipment_type_scrambler> scrambler;
			c_typed_tag_block<s_equipment_type_weapon_jammer> weapon_jammer;
			c_typed_tag_block<s_equipment_type_ammo_pack> ammo_pack;
			c_typed_tag_block<s_equipment_type_vision> vision;
		};

		c_static_array<s_tag_block, k_equipment_type_count_halo_online> tag_blocks;
	};

	s_tag_reference chud_interface;
	s_tag_reference pickup_sound;
	s_tag_reference empty_sound;
	s_tag_reference activate_sound;
	s_tag_reference activate_effect;
	s_tag_reference deactivate_sound;

	string_id activation_animation;
	string_id active_animation;
	string_id deactivate_animation;

	void update_reference_names();
};
static_assert(sizeof(_equipment_definition) == 0x1B0);

struct equipment_definition
{
	static tag const k_group_tag = EQUIPMENT_TAG;

	_object_definition object;
	_item_definition item;
	_equipment_definition equipment;

	void update_reference_names();
};
static_assert(sizeof(equipment_definition) == sizeof(_object_definition) + sizeof(_item_definition) + sizeof(_equipment_definition));

struct s_equipment_type_super_shield
{
	real32 shield_recharge_delay_multiplier; // 0.0f defaults to 1.0f
	real32 shield_recharge_rate_multiplier; // 0.0f defaults to 1.0f
	real32 shield_ceiling_multiplier; // 0.0f defaults to 1.0f
	s_tag_reference shield_effect;
	s_tag_reference overcharge_effect;
	s_tag_reference overcharge_damage_effect;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_super_shield) == 0x3C);

struct s_equipment_type_multiplayer_powerup
{
	c_enum<e_multiplayer_powerup_flavor, int32, _powerup_flavor_red, _powerup_flavor_yellow> flavor;
};
static_assert(sizeof(s_equipment_type_multiplayer_powerup) == 0x4);

struct s_equipment_type_spawner
{
	enum e_spawn_type
	{
		_spawn_type_along_aiming_vector = 0,
		_spawn_type_camera_pos_z_plane,
		_spawn_type_foot_pos_z_plane,

		k_spawn_type_count
	};

	s_tag_reference spawned_object_definition;
	s_tag_reference spawned_effect_definition;

	// distance from players eyeball on the z-plane that this effect spawns
	real32 spawn_radius;

	// z-offset of effect spawn
	real32 spawn_z_offset;

	// need a sphere of radius r's free space in order to spawn, otherwise we pick a new spawn location
	real32 spawn_area_radius;

	real32 spawn_velocity; // WU/sec

	c_enum<e_spawn_type, int16, _spawn_type_along_aiming_vector, k_spawn_type_count> type;

	// pad
	byte LSJ[0x2];

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_spawner) == 0x34);

struct s_equipment_type_proximity_mine
{
	s_tag_reference explosion_effect;
	s_tag_reference explosion_damage_effect;

	// time before it becomes a proximity mine
	real32 arm_time;

	// seconds after it is created that it must self destruct.
	// 0 means never destroy
	real32 self_destruct_time;

	// seconds object moving at trigger velocity takes to trigger explosion.
	// This will smooth out sensitivity to velocity noise
	real32 trigger_time;

	// WU/sec at which we trigger explosion
	real32 minimum_velocity_to_trigger;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_proximity_mine) == 0x30);

struct s_equipment_type_motion_tracker_noise
{
	// time before it starts making noise
	real32 arm_time;

	// radius in WU that the noise extends to.
	real32 noise_radius;

	// number of noise points that are generated
	int32 noise_count;

	// radius in WU that the damage flash noise extends to.
	real32 flash_radius;
};
static_assert(sizeof(s_equipment_type_motion_tracker_noise) == 0x10);

struct s_equipment_type_showme
{
	// radius in WU that the showme extends to.
	real32 showme_radius;
};
static_assert(sizeof(s_equipment_type_showme) == 0x4);

struct s_equipment_type_invisibility_mode
{
	real32 invisible_time; // seconds
	real32 transition_time; // seconds
};
static_assert(sizeof(s_equipment_type_invisibility_mode) == 0x8);

struct s_equipment_type_invincibility
{
	c_string_id invincibility_material_name;
	c_global_material_type invincibility_material_type;
	int16 pad;
	real32 shield_recharge_time; // seconds

	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> activation_effect;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> ongoing_effect;
	//c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> ending_effect;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_invincibility) == 0x2C);

struct s_equipment_type_treeoflife
{
	s_tag_reference regenerating_effect;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_treeoflife) == 0x10);

struct s_equipment_type_health_pack
{
	real32 heath_ammount;
	real32 shield_ammount;
};
static_assert(sizeof(s_equipment_type_health_pack) == 0x8);

struct s_equipment_type_forced_reload
{
	s_tag_reference effect;
	real32 ammo_penalty;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_forced_reload) == 0x14);

struct s_equipment_type_concussive_blast
{
	s_tag_reference secondary_activate_effect;
	s_tag_reference secondary_damage_effect;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_concussive_blast) == 0x20);

struct s_equipment_type_tank_mode
{
	c_string_id new_player_material;
	int32 __unknown4;
	real32 duration;
	real32 damage_absorption_scale;
	real32 __unknown10;
	int32 __unknown14;
	s_tag_reference active_hud;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_tank_mode) == 0x28);

struct s_equipment_type_mag_pulse
{
	real32 __unknown0;
	real32 __unknown4;
	real32 __unknown8;
	real32 __unknownC;
};
static_assert(sizeof(s_equipment_type_mag_pulse) == 0x10);

struct s_equipment_type_hologram
{
	real32 hologram_duration; // s

	s_tag_reference creation_effect;
	s_tag_reference attached_effect;
	string_id attached_effect_marker;
	string_id attached_effect_primary_scale;
	string_id attached_effect_secondary_scale;
	s_tag_reference destruction_effect;

	// how fast shimmer decreases
	real32 shimmer_decrease_rate; // 1.0/s

	// how much to ping shimmer when hit by a bullet
	real32 shimmer_bullet_ping; // 0-1

	// this is a periodic function with a period of 1 second
	// the shimmer value is used as the range input (interpolates between green and red)
	c_function_definition shimmer_to_camo_function;

	s_tag_reference nav_point_hud;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_hologram) == 0x6C);

struct s_equipment_type_reactive_armor
{
	real32 duration;
	real32 damage_reflection_ratio;
	int32 __unknown8;
	s_tag_reference activation_effect;
	s_tag_reference __unknown1C;
	s_tag_reference melee_impact_effect;
	s_tag_reference __unknown3C;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_reactive_armor) == 0x4C);

struct s_equipment_type_bomb_run
{
	int32 grenade_count;
	real_bounds velocity_bounds;
	real32 horizontal_randomness;
	real32 vertical_randomness;
	s_tag_reference projectile;
	s_tag_reference throw_sound;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_bomb_run) == 0x34);

struct s_equipment_type_armor_lock
{
	c_typed_tag_reference<COLLISION_DAMAGE_TAG, INVALID_TAG> collision_damage;
	c_typed_tag_reference<COLLISION_DAMAGE_TAG, INVALID_TAG> friendly_collision_damage;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_armor_lock) == 0x20);

struct s_equipment_type_adrenaline
{
	real32 sprint_restore;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> activation_effect;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> active_effect;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_adrenaline) == 0x24);

struct s_equipment_type_lightning_strike
{
	real32 melee_time_reduction;
	s_tag_reference unknown_effect;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_lightning_strike) == 0x14);

struct s_equipment_type_scrambler
{
	int32 __unknown0;
	s_tag_reference __unknown4;
	int32 __unknown14;
	int32 __unknown18;
	int32 __unknown1C;
	int32 __unknown20;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_scrambler) == 0x24);

struct s_equipment_type_weapon_jammer
{
	int32 __unknown0;
	s_tag_reference __unknown4;
	int32 __unknown14;
	int32 __unknown18;
	int32 __unknown1C;
	int32 __unknown20;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_weapon_jammer) == 0x24);

struct s_equipment_type_ammo_pack_weapon
{
	c_string_id name;
	c_typed_tag_reference<WEAPON_TAG, INVALID_TAG> weapon;
	int32 clip_size;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_ammo_pack_weapon) == 0x18);

struct s_equipment_type_ammo_pack
{
	int32 radius;
	int32 clip_count;
	int32 max_use_time;
	int32 delay_between_clips;
	int32 deployment_delay;
	int32 value;
	c_typed_tag_block<s_equipment_type_ammo_pack_weapon> weapons;
	s_tag_reference ammo_delivery_sound;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_ammo_pack) == 0x34);

struct s_equipment_type_vision
{
	s_tag_reference screen_effect;
	s_tag_reference damage_response;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_vision) == 0x20);

extern e_equipment_type equipment_definition_get_type(int32 definition_index, int32 expected_equipment_type_index);
extern bool equipment_definition_has_type(int32 definition_index, e_equipment_type equipment_type);
extern const s_equipment_type_adrenaline* equipment_get_adrenaline_definition(int32 definition_index);
extern const s_equipment_type_ammo_pack* equipment_get_ammo_pack_definition(int32 definition_index);
extern const s_equipment_type_armor_lock* equipment_get_armor_lock_definition(int32 definition_index);
extern const s_equipment_type_bomb_run* equipment_get_bomb_run_definition(int32 definition_index);
extern const s_equipment_type_concussive_blast* equipment_get_concussive_blast_definition(int32 definition_index);
extern const s_equipment_type_forced_reload* equipment_get_forced_reload_definition(int32 definition_index);
extern const s_equipment_type_health_pack* equipment_get_health_pack_definition(int32 definition_index);
extern const s_equipment_type_hologram* equipment_get_hologram_definition(int32 definition_index);
extern const s_equipment_type_invincibility* __cdecl equipment_get_invincibility_mode_definition(int32 definition_index);
extern const s_equipment_type_invisibility_mode* __cdecl equipment_get_invisibility_mode_definition(int32 definition_index);
extern const s_equipment_type_lightning_strike* __cdecl equipment_get_lightning_strike_definition(int32 definition_index);
extern const s_equipment_type_mag_pulse* __cdecl equipment_get_mag_pulse_definition(int32 definition_index);
extern const s_equipment_type_motion_tracker_noise* __cdecl equipment_get_motion_tracker_noise_definition(int32 definition_index);
extern const s_equipment_type_multiplayer_powerup* __cdecl equipment_get_multiplayer_powerup_definition(int32 definition_index);
extern const s_equipment_type_proximity_mine* __cdecl equipment_get_proximity_mine_definition(int32 definition_index);
extern const s_equipment_type_reactive_armor* __cdecl equipment_get_reactive_armor_definition(int32 definition_index);
extern const s_equipment_type_scrambler* __cdecl equipment_get_scrambler_definition(int32 definition_index);
extern const s_equipment_type_showme* __cdecl equipment_get_showme_definition(int32 definition_index);
extern const s_equipment_type_spawner* __cdecl equipment_get_spawner_definition(int32 definition_index);
extern const s_equipment_type_super_shield* __cdecl equipment_get_super_shield_definition(int32 definition_index);
extern const s_equipment_type_tank_mode* __cdecl equipment_get_tank_mode_definition(int32 definition_index);
extern const s_equipment_type_treeoflife* __cdecl equipment_get_treeoflife_definition(int32 definition_index);
extern const s_equipment_type_vision* __cdecl equipment_get_vision_definition(int32 definition_index);
extern const s_equipment_type_weapon_jammer* __cdecl equipment_get_weapon_jammer_definition(int32 definition_index);

