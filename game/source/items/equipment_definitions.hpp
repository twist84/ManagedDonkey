#pragma once

#include "cseries/cseries.hpp"
#include "game/materials.hpp"
#include "items/item_definitions.hpp"
#include "math/function_definitions.hpp"
#include "shell/shell.hpp"
#include "tag_files/tag_groups.hpp"

enum e_equipment_type
{
	_equipment_type_super_shield = 0,
	_equipment_type_multiplayer_powerup,
	_equipment_type_spawner,
	_equipment_type_proximity_mine,
	_equipment_type_motion_tracker_noise,
	_equipment_type_showme,
	_equipment_type_invisibility,
	_equipment_type_invincibility,
	_equipment_type_tree_of_life,
	_equipment_type_health_pack,
	_equipment_type_forced_reload,
	_equipment_type_concussive_blast,
	_equipment_type_tank_mode,
	_equipment_type_mag_pulse,
	_equipment_type_hologram,
	_equipment_type_reactive_armor,
	_equipment_type_bomb_run,
	_equipment_type_armor_lock,
	_equipment_type_adrenaline,
	_equipment_type_lightning_strike,
	_equipment_type_scrambler,
	_equipment_type_weapon_jammer,
	_equipment_type_ammo_pack,
	_equipment_type_vision,

	k_equipment_type_count,

	k_equipment_type_none = -1
};

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
struct s_equipment_type_invincibility_mode;
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
	static tag const k_group_tag = EQUIPMENT_TAG;

	_item_definition item;

	// $$$ EQUIPMENT $$$

	real duration;

	// time in seconds for the phantom volumes on this object to start accelerating things in them
	real phantom_volume_activation_time;

	// -1 means unlimited charges, 0 means fire on creation
	short charges;

	c_flags<e_equipment_flags, word, k_equipment_flags> flags;

	// AI
	// 
	// Settings which the AI use to know how they should react to this equipment

	// How large a danger zone we should create around this equipment (0 means no danger zone)
	real danger_radius;

	// How far does my target have to be for me to throw this at them?
	real min_deployment_distance; // wus

	// How long I should go unnoticed by nearby enemies
	real awareness_time; // seconds

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
			c_typed_tag_block<s_equipment_type_invincibility_mode> invincibility_mode;
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

		c_static_array<s_tag_block, k_equipment_type_count> equipment_types;
	};

	s_tag_reference chud_interface;
	s_tag_reference pickup_sound;
	s_tag_reference empty_sound;
	s_tag_reference activate_sound;
	s_tag_reference activate_effect;
	s_tag_reference deactivate_sound;

	c_string_id activation_animation;
	c_string_id active_animation;
	c_string_id deactivate_animation;

	void update_reference_names();
};
static_assert(sizeof(_equipment_definition) == 0x384);

struct s_equipment_type_super_shield
{
	real shield_recharge_delay_scale; // 0.0f defaults to 1.0f
	real shield_recharge_rate_scale; // 0.0f defaults to 1.0f
	real shield_ceiling_scale; // 0.0f defaults to 1.0f
	s_tag_reference shield_effect;
	s_tag_reference overcharge_effect;
	s_tag_reference overcharge_damage_effect;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_super_shield) == 0x3C);

struct s_equipment_type_multiplayer_powerup
{
	c_enum<e_multiplayer_powerup_flavor, long, _multiplayer_powerup_flavor_red, _multiplayer_powerup_flavor_yellow> flavor;
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

	s_tag_reference spawned_object;
	s_tag_reference spawned_effect;

	// distance from players eyeball on the z-plane that this effect spawns
	real spawn_radius;

	// z-offset of effect spawn
	real spawn_z_offset;

	// need a sphere of radius r's free space in order to spawn, otherwise we pick a new spawn location
	real spawn_area_radius;

	real spawn_velocity; // WU/sec

	c_enum<e_spawn_type, short, _spawn_type_along_aiming_vector, k_spawn_type_count> type;

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
	real arm_time;

	// seconds after it is created that it must self destruct.
	// 0 means never destroy
	real self_destruct_time;

	// seconds object moving at trigger velocity takes to trigger explosion.
	// This will smooth out sensitivity to velocity noise
	real trigger_time;

	// WU/sec at which we trigger explosion
	real trigger_velocity;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_proximity_mine) == 0x30);

struct s_equipment_type_motion_tracker_noise
{
	// time before it starts making noise
	real arm_time;

	// radius in WU that the noise extends to.
	real noise_radius;

	// number of noise points that are generated
	long noise_count;

	// radius in WU that the damage flash noise extends to.
	real flash_radius;
};
static_assert(sizeof(s_equipment_type_motion_tracker_noise) == 0x10);

struct s_equipment_type_showme
{
	// radius in WU that the showme extends to.
	real showme_radius;
};
static_assert(sizeof(s_equipment_type_showme) == 0x4);

struct s_equipment_type_invisibility_mode
{
	real invisible_time; // seconds
	real transition_time; // seconds
};
static_assert(sizeof(s_equipment_type_invisibility_mode) == 0x8);

struct s_equipment_type_invincibility_mode
{
	c_string_id invincibility_material;
	c_global_material_type invincibility_material_type;

	// pad
	byte invincibility_material_pad[0x2];

	real shield_recharge_time; // seconds

	c_typed_tag_reference<EFFECT_TAG> activation_effect;
	c_typed_tag_reference<EFFECT_TAG> attached_effect;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_invincibility_mode) == 0x2C);

struct s_equipment_type_treeoflife
{
	s_tag_reference regenerating_effect;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_treeoflife) == 0x10);

struct s_equipment_type_health_pack
{
	real heath_ammount;
	real shield_ammount;
};
static_assert(sizeof(s_equipment_type_health_pack) == 0x8);

struct s_equipment_type_forced_reload
{
	s_tag_reference effect;
	real ammo_penalty;

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
	long __unknown4;
	real duration;
	real damage_absorption_scale;
	real __unknown10;
	long __unknown14;
	s_tag_reference active_hud;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_tank_mode) == 0x28);

struct s_equipment_type_mag_pulse
{
	real __unknown0;
	real __unknown4;
	real __unknown8;
	real __unknownC;
};
static_assert(sizeof(s_equipment_type_mag_pulse) == 0x10);

struct s_equipment_type_hologram
{
	real hologram_duration; // s

	s_tag_reference creation_effect;
	s_tag_reference attached_effect;
	string_id attached_effect_marker;
	string_id attached_effect_primary_scale;
	string_id attached_effect_secondary_scale;
	s_tag_reference destruction_effect;

	// how fast shimmer decreases
	real shimmer_decrease_rate; // 1.0/s

	// how much to ping shimmer when hit by a bullet
	real shimmer_bullet_ping; // 0-1

	// this is a periodic function with a period of 1 second
	// the shimmer value is used as the range input (interpolates between green and red)
	c_function_definition shimmer_to_camo_function;

	s_tag_reference nav_point_hud;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_hologram) == 0x6C);

struct s_equipment_type_reactive_armor
{
	real duration;
	real damage_reflection_ratio;
	long __unknown8;
	s_tag_reference activation_effect;
	s_tag_reference __unknown1C;
	s_tag_reference melee_impact_effect;
	s_tag_reference __unknown3C;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_reactive_armor) == 0x4C);

struct s_equipment_type_bomb_run
{
	long grenade_count;
	real_bounds velocity_bounds;
	real horizontal_randomness;
	real vertical_randomness;
	s_tag_reference projectile;
	s_tag_reference throw_sound;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_bomb_run) == 0x34);

struct s_equipment_type_armor_lock
{
	c_typed_tag_reference<COLLISION_DAMAGE_TAG> collision_damage;
	c_typed_tag_reference<COLLISION_DAMAGE_TAG> friendly_collision_damage;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_armor_lock) == 0x20);

struct s_equipment_type_adrenaline
{
	real sprint_restore;
	c_typed_tag_reference<EFFECT_TAG> activation_effect;
	c_typed_tag_reference<EFFECT_TAG> active_effect;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_adrenaline) == 0x24);

struct s_equipment_type_lightning_strike
{
	real melee_time_reduction;
	s_tag_reference unknown_effect;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_lightning_strike) == 0x14);

struct s_equipment_type_scrambler
{
	long __unknown0;
	s_tag_reference __unknown4;
	long __unknown14;
	long __unknown18;
	long __unknown1C;
	long __unknown20;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_scrambler) == 0x24);

struct s_equipment_type_weapon_jammer
{
	long __unknown0;
	s_tag_reference __unknown4;
	long __unknown14;
	long __unknown18;
	long __unknown1C;
	long __unknown20;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_weapon_jammer) == 0x24);

struct s_equipment_type_ammo_pack_weapon
{
	c_string_id name;
	c_typed_tag_reference<WEAPON_TAG> weapon;
	long clip_size;

	void update_reference_names();
};
static_assert(sizeof(s_equipment_type_ammo_pack_weapon) == 0x18);

struct s_equipment_type_ammo_pack
{
	long radius;
	long clip_count;
	long max_use_time;
	long delay_between_clips;
	long deployment_delay;
	long value;
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

extern e_equipment_type equipment_definition_get_type(long definition_index, long expected_equipment_type_index);
extern bool equipment_definition_has_type(long definition_index, e_equipment_type equipment_type);
extern s_equipment_type_adrenaline const* equipment_get_adrenaline_definition(long definition_index);
extern s_equipment_type_ammo_pack const* equipment_get_ammo_pack_definition(long definition_index);
extern s_equipment_type_armor_lock const* equipment_get_armor_lock_definition(long definition_index);
extern s_equipment_type_bomb_run const* equipment_get_bomb_run_definition(long definition_index);
extern s_equipment_type_concussive_blast const* equipment_get_concussive_blast_definition(long definition_index);
extern s_equipment_type_forced_reload const* equipment_get_forced_reload_definition(long definition_index);
extern s_equipment_type_health_pack const* equipment_get_health_pack_definition(long definition_index);
extern s_equipment_type_hologram const* equipment_get_hologram_definition(long definition_index);
extern s_equipment_type_invincibility_mode const* __cdecl equipment_get_invincibility_mode_definition(long definition_index);
extern s_equipment_type_invisibility_mode const* __cdecl equipment_get_invisibility_mode_definition(long definition_index);
extern s_equipment_type_lightning_strike const* __cdecl equipment_get_lightning_strike_definition(long definition_index);
extern s_equipment_type_mag_pulse const* __cdecl equipment_get_mag_pulse_definition(long definition_index);
extern s_equipment_type_motion_tracker_noise const* __cdecl equipment_get_motion_tracker_noise_definition(long definition_index);
extern s_equipment_type_multiplayer_powerup const* __cdecl equipment_get_multiplayer_powerup_definition(long definition_index);
extern s_equipment_type_proximity_mine const* __cdecl equipment_get_proximity_mine_definition(long definition_index);
extern s_equipment_type_reactive_armor const* __cdecl equipment_get_reactive_armor_definition(long definition_index);
extern s_equipment_type_scrambler const* __cdecl equipment_get_scrambler_definition(long definition_index);
extern s_equipment_type_showme const* __cdecl equipment_get_showme_definition(long definition_index);
extern s_equipment_type_spawner const* __cdecl equipment_get_spawner_definition(long definition_index);
extern s_equipment_type_super_shield const* __cdecl equipment_get_super_shield_definition(long definition_index);
extern s_equipment_type_tank_mode const* __cdecl equipment_get_tank_mode_definition(long definition_index);
extern s_equipment_type_treeoflife const* __cdecl equipment_get_treeoflife_definition(long definition_index);
extern s_equipment_type_vision const* __cdecl equipment_get_vision_definition(long definition_index);
extern s_equipment_type_weapon_jammer const* __cdecl equipment_get_weapon_jammer_definition(long definition_index);

