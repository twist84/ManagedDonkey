#pragma once

#include "tag_files/tag_groups.hpp"

struct s_multiplayer_universal_globals_definition;
struct s_multiplayer_runtime_globals_definition;
struct s_multiplayer_globals_definition
{
	c_typed_tag_block<s_multiplayer_universal_globals_definition> universal;
	c_typed_tag_block<s_multiplayer_runtime_globals_definition> runtime;
};
static_assert(sizeof(s_multiplayer_globals_definition) == 0x18);

struct s_multiplayer_customized_model_character;
struct s_multiplayer_equipment;
struct s_multiplayer_energy_regeneration;
struct s_multiplayer_weapon_selection;
struct s_multiplayer_vehicle_selection;
struct s_multiplayer_grenade_selection;
struct s_multiplayer_weapon_set;
struct s_multiplayer_vehicle_set;
struct s_multiplayer_podium_animation;
struct s_multiplayer_universal_globals_definition
{
	c_typed_tag_reference<'unic'> random_player_names;
	c_typed_tag_reference<'unic'> team_names;
	c_typed_tag_block<s_multiplayer_customized_model_character> customized_spartan_characters;
	c_typed_tag_block<s_multiplayer_customized_model_character> customized_elite_characters;
	c_typed_tag_block<s_multiplayer_equipment> equipment;
	c_typed_tag_block<s_multiplayer_energy_regeneration> energy_regeneration;
	c_typed_tag_reference<'unic'> multiplayer_text;
	c_typed_tag_reference<'unic'> sandbox_text;
	c_typed_tag_reference<'jmrq'> sandbox_object_properties_values;

	// Weapon Selections Halo3
	// Define the weapons you want to be available in multiplayer games here (does not include mp objects like ball or flag)
	c_typed_tag_block<s_multiplayer_weapon_selection> weapon_selections;

	// Vehicle Selections Halo3
	// Define the vehicles you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_vehicle_selection> vehicle_selections;

	// Grenade Selections Halo3
	// Define the grenades you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_grenade_selection> grenade_selections;

	// Weapon Sets Halo3
	// Define the weapon sets you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_weapon_set> weapon_sets;

	// Vehicle Sets Halo3
	// Define the vehicle sets you want to be available in multiplayer games here
	c_typed_tag_block<s_multiplayer_vehicle_set> vehicle_sets;

	c_typed_tag_block<s_multiplayer_podium_animation> podium_animations;

	// Game Engine Settings Halo3
	// This tag is only placeholder, until the Excel/XML setup system is up and running
	c_typed_tag_reference<'wezr'> halo3_game_engine_settings;
};
static_assert(sizeof(s_multiplayer_universal_globals_definition) == 0xD8);

struct s_multiplayer_customized_model_selection;
struct s_multiplayer_customized_model_character
{
	c_string_id armor_region;
	c_string_id biped_region;
	c_typed_tag_block<s_multiplayer_customized_model_selection> customized_areas;
};
static_assert(sizeof(s_multiplayer_customized_model_character) == 0x14);

struct s_multiplayer_customized_model_selection
{
	c_string_id selection_name;
	s_tag_reference third_person_armor_object;
	s_tag_reference first_person_armor_object;

	short __unknown24;
	byte __pad26[2];

	c_string_id parent_attach_marker;
	c_string_id child_attach_marker;
};
static_assert(sizeof(s_multiplayer_customized_model_selection) == 0x30);

struct s_multiplayer_equipment
{
	c_string_id name;
	s_tag_reference object;
	short type;
	byte __pad[2];
};
static_assert(sizeof(s_multiplayer_equipment) == 0x18);

struct s_multiplayer_energy_regeneration
{
	long duration;
	long energy_level;
};
static_assert(sizeof(s_multiplayer_energy_regeneration) == 0x8);

struct s_multiplayer_weapon_selection
{
	c_string_id name;

	// used only for random weapon set
	real random_weapon_set_weight;

	c_typed_tag_reference<'weap'> weapon_tag;
};
static_assert(sizeof(s_multiplayer_weapon_selection) == 0x18);

struct s_multiplayer_vehicle_selection
{
	c_string_id name;
	c_typed_tag_reference<'vehi'> vehicle_tag;
};
static_assert(sizeof(s_multiplayer_vehicle_selection) == 0x14);

struct s_multiplayer_grenade_selection
{
	c_string_id name;
	c_typed_tag_reference<'eqip'> grenade_tag;
};
static_assert(sizeof(s_multiplayer_grenade_selection) == 0x14);

struct s_multiplayer_object_remap_entry;
struct s_multiplayer_weapon_set
{
	c_string_id name;
	c_typed_tag_block<s_multiplayer_object_remap_entry> remap_table;
};
static_assert(sizeof(s_multiplayer_weapon_set) == 0x10);

struct s_multiplayer_vehicle_set
{
	c_string_id name;
	c_typed_tag_block<s_multiplayer_object_remap_entry> remap_table;
};
static_assert(sizeof(s_multiplayer_vehicle_set) == 0x10);

struct s_multiplayer_object_remap_entry
{
	c_string_id placed_object_name;
	c_string_id remapped_object_name;
};
static_assert(sizeof(s_multiplayer_object_remap_entry) == 0x8);

struct s_multiplayer_podium_stance_animation;
struct s_multiplayer_podium_move_animation;
struct s_multiplayer_podium_animation
{
	c_typed_tag_reference<'jmad'> animation_tag;
	c_string_id default_unarmed;
	c_string_id default_armed;
	c_typed_tag_block<s_multiplayer_podium_stance_animation> stance_animations;
	c_typed_tag_block<s_multiplayer_podium_move_animation> move_animations;
};
static_assert(sizeof(s_multiplayer_podium_animation) == 0x30);

struct s_multiplayer_podium_stance_animation
{
	string name;
	c_string_id base_animation;
	c_string_id loop_animation;
	c_string_id unarmed_transition;
	c_string_id armed_transition;
	real camera_distance_offset;
};
static_assert(sizeof(s_multiplayer_podium_stance_animation) == 0x34);

struct s_multiplayer_podium_move_animation
{
	string name;
	c_string_id in_animation;
	c_string_id loop_animation;
	c_string_id out_transition;
	real offset;
	c_typed_tag_reference<'weap'> primary_weapon;
	c_typed_tag_reference<'weap'> secondary_weapon;
};
static_assert(sizeof(s_multiplayer_podium_move_animation) == 0x50);

struct s_multiplayer_constants;
struct s_multiplayer_runtime_globals_definition
{
	c_typed_tag_reference<'bipd'> editor_biped;
	c_typed_tag_reference<'scen'> editor_helper;
	c_typed_tag_reference<'weap'> flag;
	c_typed_tag_reference<'weap'> ball;
	c_typed_tag_reference<'weap'> assault_bomb;
	c_typed_tag_reference<'bloc'> vip_influence_area;
	c_typed_tag_reference<'unic'> in_game_text;

	c_typed_tag_reference<'proj'> __reference70_projectile;
	c_typed_tag_reference<'effe'> __reference80_effect;
	c_typed_tag_reference<'effe'> __reference90_effect;

	// `player_spawn`
	// long sound_index = player_has_gameplay_modifier(_gameplay_modifier_respawn_modifier) ? runtime_data->modifier_respawn_sound.index : runtime_data->default_respawn_sound.index
	c_typed_tag_reference<'snd!'> default_respawn_sound;
	c_typed_tag_reference<'snd!'> modifier_respawn_sound;

	s_tag_block sounds;
	s_tag_block looping_sounds;
	s_tag_block earn_wp_events;
	s_tag_block general_events;
	s_tag_block flavor_events;
	s_tag_block slayer_events;
	s_tag_block ctf_events;
	s_tag_block oddball_events;
	s_tag_block king_events;
	s_tag_block vip_events;
	s_tag_block juggernaut_events;
	s_tag_block territories_events;
	s_tag_block assault_events;
	s_tag_block infection_events;

	long maximum_frag_count;
	long maximum_plasma_count;

	c_typed_tag_block<s_multiplayer_constants> multiplayer_constants;
	s_tag_block state_responses;

	c_typed_tag_reference<'bitm'> scoreboard_emblem_bitmap;
	c_typed_tag_reference<'bitm'> scoreboard_dead_emblem_bitmap;

	c_typed_tag_reference<'rmhg'> hill_shader;

	c_typed_tag_reference<'chdt'> unused_intro_message;
	c_typed_tag_reference<'chdt'> ctf_intro_message;
	c_typed_tag_reference<'chdt'> slayer_intro_message;
	c_typed_tag_reference<'chdt'> oddball_intro_message;
	c_typed_tag_reference<'chdt'> king_intro_message;
	c_typed_tag_reference<'chdt'> sandbox_intro_message;
	c_typed_tag_reference<'chdt'> vip_intro_message;
	c_typed_tag_reference<'chdt'> juggernaut_intro_message;
	c_typed_tag_reference<'chdt'> territories_intro_message;
	c_typed_tag_reference<'chdt'> assault_intro_message;
	c_typed_tag_reference<'chdt'> infection_intro_message;

	c_typed_tag_reference<'lsnd'> menu_music_a;
	c_typed_tag_reference<'lsnd'> menu_music_b;
	c_typed_tag_reference<'lsnd'> menu_music_c;
	c_typed_tag_reference<'lsnd'> menu_music_d;
};
static_assert(sizeof(s_multiplayer_runtime_globals_definition) == 0x2A8);

struct s_multiplayer_constants
{
	// PLAYER SPAWN INFLUENCERS
	// These are things that influence player spawn points

	// ENEMY FORBID
	// This is a tight cylinder around the player with a strong negative weight. It serves to protect players from spawning close to an enemy, or having and enemy spawn close to them.
	real ef_full_weight_radius; // wu
	real ef_fall_off_radius; // wu
	real ef_upper_height; // wu
	real ef_lower_height; // wu
	real ef_weight;

	// ENEMY BIAS
	// This is a larger cylinder, with a size set specifically for the size of the map. It has a weak negative weight which falls to zero towards the outer radius. It serves to bias players away from spawning near enemies, or in enemy held areas.
	real eb_full_weight_radius; // wu
	real eb_fall_off_radius; // wu
	real eb_upper_height; // wu
	real eb_lower_height; // wu
	real eb_weight;

	// ALLY BIAS
	// This is a larger cylinder, with a size set specifically for the size of the map. It has a weak positive weight which falls to zero towards the outer radius. It serves to bias players towards spawning with allies, or in areas which are controlled by friendly players.
	real ab_full_weight_radius; // wu
	real ab_fall_off_radius; // wu
	real ab_upper_height; // wu
	real ab_lower_height; // wu
	real ab_weight;

	// SELECTED ALLY BIAS", "Same as ALLY BIAS, but used when the ally is selected in the dead-camera.
	real sab_full_weight_radius; // wu
	real sab_fall_off_radius; // wu
	real sab_upper_height; // wu
	real sab_lower_height; // wu
	real sab_weight;

	// DEAD TEAMMATE BIAS
	// Dead teammate influences are cylinders centered upon the body of a dead teammate (or the player's own dead body).
	real dt_full_weight_radius; // wu
	real dt_fall_off_radius; // wu
	real dt_upper_height; // wu
	real dt_lower_height; // wu
	real dt_weight;
	real dead_teammate_influence_duration; // seconds

	// WEAPON INFLUENCERS
	// These influencers are induced by weapons either weilded or carried in the player's backpack
	s_tag_block weapon_influencers;

	// VEHICLE INFLUENCERS
	// These influencers are induced by vehicles.
	s_tag_block vehicle_influencers;

	// PROJECTILE INFLUENCERS
	// These influencers are induced by projectiles.
	s_tag_block projectile_influencers;

	// EQUIPMENT INFLUENCERS
	// These influencers are induced by equipment.
	s_tag_block equipment_influencers;

	// KOTH HILL INFLUENCER
	// This influencer is induced by the King of the Hill hill goal area.
	real koth_full_weight_radius; // wu
	real koth_falloff_radius; // wu
	real koth_upper_cylinder_height; // wu
	real koth_lower_cylinder_height; // wu
	real koth_weight;

	// ODDBALL INFLUENCER
	// This influencer is induced by the oddball.
	real ob_full_weight_radius; // wu
	real ob_falloff_radius; // wu
	real ob_upper_cylinder_height; // wu
	real ob_lower_cylinder_height; // wu
	real ob_weight;

	// CTF FLAG AWAY INFLUENCER
	// This influencer is induced by the CTF flag stand when the flag is away.
	real ctf_full_weight_radius; // wu
	real ctf_falloff_radius; // wu
	real ctf_upper_cylinder_height; // wu
	real ctf_lower_cylinder_height; // wu
	real ctf_weight;

	// TERRITORY ALLY INFLUENCER
	// This influencer is induced by territories controlled by teammates.
	real tera_full_weight_radius; // wu
	real tera_falloff_radius; // wu
	real tera_upper_cylinder_height; // wu
	real tera_lower_cylinder_height; // wu
	real tera_weight;

	// TERRITORY ENEMY INFLUENCER
	// This influencer is induced by territories controlled by an enemy.
	real tere_full_weight_radius; // wu
	real tere_falloff_radius; // wu
	real tere_upper_cylinder_height; // wu
	real tere_lower_cylinder_height; // wu
	real tere_weight;

	// INFECTION SAFE ZONE HUMAN INFLUENCER
	// This influencer is induced by an infection safe zone upon humans.
	real infh_full_weight_radius; // wu
	real infh_falloff_radius; // wu
	real infh_upper_cylinder_height; // wu
	real infh_lower_cylinder_height; // wu
	real infh_weight;

	// INFECTION SAFE ZONE ZOMBIE INFLUENCER
	// This influencer is induced by an infection safe zone upon zombies.
	real infz_full_weight_radius; // wu
	real infz_falloff_radius; // wu
	real infz_upper_cylinder_height; // wu
	real infz_lower_cylinder_height; // wu
	real infz_weight;

	// VIP INFLUENCER
	// This influencer is induced by a VIP upon his teammates.
	real vip_full_weight_radius; // wu
	real vip_falloff_radius; // wu
	real vip_upper_cylinder_height; // wu
	real vip_lower_cylinder_height; // wu
	real vip_weight;

	// MORE MP CONSTANTS
	// More old Halo2 stuff follows...
	real maximum_random_spawn_bias;
	real teleporter_recharge_time; // seconds
	real grenade_danger_weight;
	real grenade_danger_inner_radius;
	real grenade_danger_outer_radius;
	real grenade_danger_lead_time; // seconds
	real vehicle_danger_min_speed; // wu/sec
	real vehicle_danger_weight;
	real vehicle_danger_radius;
	real vehicle_danger_lead_time; // seconds
	real vehicle_nearby_player_dist; // how nearby a player is to count a vehicle as 'occupied'
	c_typed_tag_reference<'bitm'> hill_bitmap;
	real flag_return_distance;
	real flag_contest_inner_radius;
	real flag_contest_outer_radius;
	real territories_waypoint_vertical_offset;
	c_typed_tag_reference<'effe'> bomb_explode_effect;
	c_typed_tag_reference<'effe'> bomb_explode_secondary_effect;
	c_typed_tag_reference<'effe'> bomb_explode_dmg_effect;
	c_typed_tag_reference<'effe'> bomb_defuse_effect;
	c_typed_tag_reference<'effe'> sandbox_effect;
	c_string_id bomb_defusal_string;
	c_string_id blocked_teleporter_string;
	long __unknown1DC;

	// RESPAWN STRINGS
	// These are used for respawn status message displays
	// The text comes from the in-game-text multiplayer message strings list tag
	// in the multiplayer runtime globals block above
	c_string_id voluntary_respawn_control_instructions;
	c_string_id spawn_allowed_default_respawn;
	c_string_id spawn_at_player_allowed_looking_at_self;
	c_string_id spawn_at_player_allowed_looking_at_target;
	c_string_id spawn_at_player_allowed_looking_at_potential_target;
	c_string_id spawn_at_territory_allowed_looking_at_target;
	c_string_id spawn_at_territory_allowed_looking_at_potential_target;
	c_string_id you_are_out_of_lives;
	c_string_id invalid_spawn_target_selected;
	c_string_id targetted_player_enemies_nearby;
	c_string_id targetted_player_unfriendly_team;
	c_string_id targetted_player_dead;
	c_string_id targetted_player_in_combat;
	c_string_id targetted_player_too_far_from_owned_flag;
	c_string_id no_available_netpoints;
	c_string_id targetted_netpoint_contested;
};
static_assert(sizeof(s_multiplayer_constants) == 0x220);

extern long __cdecl multiplayer_universal_data_get_random_weapon_definition_index();
extern long __cdecl multiplayer_universal_data_get_remapped_vehicle_definition_index(long vehicle_tag_index, s_multiplayer_vehicle_set const* vehicle_set);
extern long __cdecl multiplayer_universal_data_get_remapped_weapon_definition_index(long weapon_tag_index, s_multiplayer_weapon_set const* weapon_set);
extern long __cdecl multiplayer_universal_data_get_vehicle_selection_definition_index(long name);
extern short __cdecl multiplayer_universal_data_get_vehicle_set_absolute_index_from_string_id(long name);
extern long __cdecl multiplayer_universal_data_get_vehicle_set_name_from_absolute_index(short absolute_index);
extern long __cdecl multiplayer_universal_data_get_weapon_selection_definition_index(long name);
extern short __cdecl multiplayer_universal_data_get_weapon_set_absolute_index_from_string_id(long name);
extern long __cdecl multiplayer_universal_data_get_weapon_set_name_from_absolute_index(short absolute_index);
extern s_multiplayer_vehicle_set const* __cdecl multiplayer_universal_data_vehicle_set_try_and_get(short vehicle_set_index);
extern s_multiplayer_weapon_set const* __cdecl multiplayer_universal_data_weapon_set_try_and_get(short weapon_set_index);
extern s_multiplayer_runtime_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_runtime_data();
extern s_multiplayer_universal_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_universal_data();

extern s_multiplayer_universal_globals_definition* multiplayer_globals_try_and_get_universal_data(s_multiplayer_globals_definition* multiplayer_globals);

