#pragma once

#include "game/game_engine_spawn_influencer.hpp"
#include "game/game_engine_event_definitions.hpp"
#include "sound/sound_definitions.hpp"
#include "tag_files/tag_groups.hpp"

struct s_multiplayer_universal_globals_definition;
struct s_multiplayer_runtime_globals_definition;
struct s_multiplayer_globals_definition
{
	static tag const k_group_tag = MULTIPLAYER_GLOBALS_TAG;

	c_typed_tag_block<s_multiplayer_universal_globals_definition> universal;
	c_typed_tag_block<s_multiplayer_runtime_globals_definition> runtime;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_globals_definition) == 0x18);

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
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> random_player_names;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> team_names;
	c_typed_tag_block<s_multiplayer_customized_model_character> customized_spartan_characters;
	c_typed_tag_block<s_multiplayer_customized_model_character> customized_elite_characters;
	c_typed_tag_block<s_multiplayer_equipment> equipment;
	c_typed_tag_block<s_multiplayer_energy_regeneration> energy_regeneration;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> multiplayer_text;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> sandbox_text;
	c_typed_tag_reference<SANDBOX_TEXT_VALUE_PAIR_DEFINITION_TAG, INVALID_TAG> sandbox_object_properties_values;

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
	c_typed_tag_reference<GAME_ENGINE_SETTINGS_DEFINITION_TAG, INVALID_TAG> halo3_game_engine_settings;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_universal_globals_definition) == 0xD8);

struct s_multiplayer_customized_model_selection;
struct s_multiplayer_customized_model_character
{
	c_string_id armor_region;
	c_string_id biped_region;
	c_typed_tag_block<s_multiplayer_customized_model_selection> customized_selection;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_customized_model_character) == 0x14);

struct s_multiplayer_customized_model_selection
{
	c_string_id selection_name;
	s_tag_reference third_person_armor_object;
	s_tag_reference first_person_armor_object;

	int16 __unknown24;
	byte __pad26[0x2];

	c_string_id parent_attach_marker;
	c_string_id child_attach_marker;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_customized_model_selection) == 0x30);

struct s_multiplayer_equipment
{
	c_string_id name;
	s_tag_reference object;
	int16 type;
	byte __pad[0x2];

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_equipment) == 0x18);

struct s_multiplayer_energy_regeneration
{
	int32 duration;
	int32 energy_level;
};
COMPILE_ASSERT(sizeof(s_multiplayer_energy_regeneration) == 0x8);

struct s_multiplayer_weapon_selection
{
	c_string_id name;

	// used only for random weapon set
	real32 random_weapon_set_weight;

	c_typed_tag_reference<WEAPON_TAG, INVALID_TAG> weapon_tag;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_weapon_selection) == 0x18);

struct s_multiplayer_vehicle_selection
{
	c_string_id name;
	c_typed_tag_reference<VEHICLE_TAG, INVALID_TAG> vehicle_tag;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_vehicle_selection) == 0x14);

struct s_multiplayer_grenade_selection
{
	c_string_id name;
	c_typed_tag_reference<EQUIPMENT_TAG, INVALID_TAG> grenade_tag;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_grenade_selection) == 0x14);

struct s_multiplayer_object_remap_entry;
struct s_multiplayer_weapon_set
{
	c_string_id name;
	c_typed_tag_block<s_multiplayer_object_remap_entry> remap_table;
};
COMPILE_ASSERT(sizeof(s_multiplayer_weapon_set) == 0x10);

struct s_multiplayer_vehicle_set
{
	c_string_id name;
	c_typed_tag_block<s_multiplayer_object_remap_entry> remap_table;
};
COMPILE_ASSERT(sizeof(s_multiplayer_vehicle_set) == 0x10);

struct s_multiplayer_object_remap_entry
{
	c_string_id placed_object_name;
	c_string_id remapped_object_name;
};
COMPILE_ASSERT(sizeof(s_multiplayer_object_remap_entry) == 0x8);

struct s_multiplayer_podium_stance_animation;
struct s_multiplayer_podium_move_animation;
struct s_multiplayer_podium_animation
{
	c_typed_tag_reference<MODEL_ANIMATION_GRAPH_TAG, INVALID_TAG> animation_tag;
	c_string_id default_unarmed;
	c_string_id default_armed;
	c_typed_tag_block<s_multiplayer_podium_stance_animation> stance_animations;
	c_typed_tag_block<s_multiplayer_podium_move_animation> move_animations;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_podium_animation) == 0x30);

struct s_multiplayer_podium_stance_animation
{
	c_static_string<k_tag_string_length> name;
	c_string_id base_animation;
	c_string_id loop_animation;
	c_string_id unarmed_transition;
	c_string_id armed_transition;
	real32 camera_distance_offset;
};
COMPILE_ASSERT(sizeof(s_multiplayer_podium_stance_animation) == 0x34);

struct s_multiplayer_podium_move_animation
{
	c_static_string<k_tag_string_length> name;
	c_string_id in_animation;
	c_string_id loop_animation;
	c_string_id out_transition;
	real32 offset;
	c_typed_tag_reference<WEAPON_TAG, INVALID_TAG> primary_weapon;
	c_typed_tag_reference<WEAPON_TAG, INVALID_TAG> secondary_weapon;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_podium_move_animation) == 0x50);

struct s_multiplayer_constants;
struct s_game_engine_status_response;
template<tag ...k_group_tags>
struct s_multiplayer_globals_tag_reference;
struct s_multiplayer_runtime_globals_definition
{
	c_typed_tag_reference<BIPED_TAG, INVALID_TAG> editor_biped;
	c_typed_tag_reference<SCENERY_TAG, INVALID_TAG> editor_helper;
	c_typed_tag_reference<WEAPON_TAG, INVALID_TAG> flag;
	c_typed_tag_reference<WEAPON_TAG, INVALID_TAG> ball;
	c_typed_tag_reference<WEAPON_TAG, INVALID_TAG> assault_bomb;
	c_typed_tag_reference<CRATE_TAG, INVALID_TAG> vip_influence_area;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG, INVALID_TAG> in_game_text;

	// gameplay modifier tags
	c_typed_tag_reference<PROJECTILE_TAG, INVALID_TAG> plant_plasma_on_death_projectile;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> vehicle_autoflip_effect;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> safety_booster_effect;

	// `player_spawn`
	// int32 sound_index = player_has_gameplay_modifier(_gameplay_modifier_respawn_modifier) ? runtime_data->modifier_respawn_sound.index : runtime_data->default_respawn_sound.index
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> default_respawn_sound;
	c_typed_tag_reference<SOUND_TAG, INVALID_TAG> modifier_respawn_sound;

	c_typed_tag_block<s_multiplayer_globals_tag_reference<SOUND_TAG, INVALID_TAG>> sounds;
	c_typed_tag_block<s_multiplayer_globals_tag_reference<SOUND_LOOPING_TAG, INVALID_TAG>> looping_sounds;

	c_typed_tag_block<s_multiplayer_event_response_definition> earn_wp_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> general_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> flavor_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> slayer_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> ctf_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> oddball_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> king_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> vip_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> juggernaut_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> territories_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> assault_events;
	c_typed_tag_block<s_multiplayer_event_response_definition> infection_events;

	int32 maximum_frag_count;
	int32 maximum_plasma_count;

	c_typed_tag_block<s_multiplayer_constants> multiplayer_constants;
	c_typed_tag_block<s_game_engine_status_response> state_responses;

	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> scoreboard_emblem_bitmap;
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> scoreboard_dead_emblem_bitmap;

	c_typed_tag_reference<SHADER_HALOGRAM_TAG, INVALID_TAG> hill_shader;

	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> unused_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> ctf_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> slayer_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> oddball_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> king_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> sandbox_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> vip_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> juggernaut_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> territories_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> assault_intro_message;
	c_typed_tag_reference<CHUD_DEFINITION_TAG, INVALID_TAG> infection_intro_message;

	c_typed_tag_reference<SOUND_LOOPING_TAG, INVALID_TAG> menu_music_a;
	c_typed_tag_reference<SOUND_LOOPING_TAG, INVALID_TAG> menu_music_b;
	c_typed_tag_reference<SOUND_LOOPING_TAG, INVALID_TAG> menu_music_c;
	c_typed_tag_reference<SOUND_LOOPING_TAG, INVALID_TAG> menu_music_d;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_runtime_globals_definition) == 0x2A8);

struct s_multiplayer_constants
{
	// PLAYER SPAWN INFLUENCERS
	// These are things that influence player spawn points

	// ENEMY FORBID
	// This is a tight cylinder around the player with a strong negative weight. It serves to protect players from spawning close to an enemy, or having and enemy spawn close to them.
	s_player_spawn_influence enemy_forbid_influencer;

	// ENEMY BIAS
	// This is a larger cylinder, with a size set specifically for the size of the map. It has a weak negative weight which falls to zero towards the outer radius. It serves to bias players away from spawning near enemies, or in enemy held areas.
	s_player_spawn_influence enemy_bias_influencer;

	// ALLY BIAS
	// This is a larger cylinder, with a size set specifically for the size of the map. It has a weak positive weight which falls to zero towards the outer radius. It serves to bias players towards spawning with allies, or in areas which are controlled by friendly players.
	s_player_spawn_influence ally_bias_influencer;

	// SELECTED ALLY BIAS", "Same as ALLY BIAS, but used when the ally is selected in the dead-camera.
	s_player_spawn_influence selected_ally_bias_influencer;

	// DEAD TEAMMATE BIAS
	// Dead teammate influences are cylinders centered upon the body of a dead teammate (or the player's own dead body).
	s_player_spawn_influence dead_teammate_influencer;
	real32 dead_teammate_influence_duration; // seconds

	// WEAPON INFLUENCERS
	// These influencers are induced by weapons either weilded or carried in the player's backpack
	c_typed_tag_block<s_weapon_spawn_influence> weapon_influencers;

	// VEHICLE INFLUENCERS
	// These influencers are induced by vehicles.
	c_typed_tag_block<s_vehicle_spawn_influence> vehicle_influencers;

	// PROJECTILE INFLUENCERS
	// These influencers are induced by projectiles.
	c_typed_tag_block<s_projectile_spawn_influence> projectile_influencers;

	// EQUIPMENT INFLUENCERS
	// These influencers are induced by equipment.
	c_typed_tag_block<s_equipment_spawn_influence> equipment_influencers;

	// KOTH HILL INFLUENCER
	// This influencer is induced by the King of the Hill hill goal area.
	s_netgame_goal_spawn_influence koth_hill_influencer;

	// ODDBALL INFLUENCER
	// This influencer is induced by the oddball.
	s_netgame_goal_spawn_influence oddball_influencer;

	// CTF FLAG AWAY INFLUENCER
	// This influencer is induced by the CTF flag stand when the flag is away.
	s_netgame_goal_spawn_influence ctf_flag_away_influencer;

	// TERRITORY ALLY INFLUENCER
	// This influencer is induced by territories controlled by teammates.
	s_netgame_goal_spawn_influence territories_ally_influencer;

	// TERRITORY ENEMY INFLUENCER
	// This influencer is induced by territories controlled by an enemy.
	s_netgame_goal_spawn_influence territories_enemy_influencer;

	// INFECTION SAFE ZONE HUMAN INFLUENCER
	// This influencer is induced by an infection safe zone upon humans.
	s_netgame_goal_spawn_influence infection_safe_zone_human_influencer;

	// INFECTION SAFE ZONE ZOMBIE INFLUENCER
	// This influencer is induced by an infection safe zone upon zombies.
	s_netgame_goal_spawn_influence infection_safe_zone_zombie_influencer;

	// VIP INFLUENCER
	// This influencer is induced by a VIP upon his teammates.
	s_netgame_goal_spawn_influence vip_influencer;

	// MORE MP CONSTANTS
	// More old Halo2 stuff follows...
	real32 maximum_random_spawn_bias;
	real32 teleporter_recharge_time; // seconds
	real32 grenade_danger_weight;
	real32 grenade_danger_inner_radius;
	real32 grenade_danger_outer_radius;
	real32 grenade_danger_lead_time; // seconds
	real32 vehicle_danger_min_speed; // wu/sec
	real32 vehicle_danger_weight;
	real32 vehicle_danger_radius;
	real32 vehicle_danger_lead_time; // seconds
	real32 vehicle_nearby_player_dist; // how nearby a player is to count a vehicle as 'occupied'
	c_typed_tag_reference<BITMAP_TAG, INVALID_TAG> hill_bitmap;
	real32 flag_return_distance;
	real32 flag_contest_inner_radius;
	real32 flag_contest_outer_radius;
	real32 territories_waypoint_vertical_offset;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> bomb_explode_effect;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> bomb_explode_secondary_effect;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> bomb_explode_dmg_effect;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> bomb_defuse_effect;
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> sandbox_effect;
	c_string_id bomb_defusal_string;
	c_string_id blocked_teleporter_string;
	int32 __unknown1DC;

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

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_multiplayer_constants) == 0x220);

struct s_game_engine_status_response
{
	c_flags<e_game_engine_status_flags, int16, k_game_engine_status_flags> flags;
	byte FAW[0x2]; // pad
	c_enum<e_game_engine_status, int16, _game_engine_status_waiting_for_space_to_clear, k_game_engine_status_count> state;
	byte BNYFIDDGX[0x2]; // pad
	c_string_id ffa_message;
	c_string_id team_message;
	s_tag_reference unused;
	byte GTL[0x4]; // pad

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_game_engine_status_response) == 0x24);

template<tag ...k_group_tags>
struct s_multiplayer_globals_tag_reference :
	s_tag_reference
{
	void update_reference_names();
};

extern int32 __cdecl multiplayer_universal_data_get_random_weapon_definition_index();
extern int32 __cdecl multiplayer_universal_data_get_remapped_vehicle_definition_index(int32 vehicle_tag_index, const s_multiplayer_vehicle_set* vehicle_set);
extern int32 __cdecl multiplayer_universal_data_get_remapped_weapon_definition_index(int32 weapon_tag_index, const s_multiplayer_weapon_set* weapon_set);
extern int32 __cdecl multiplayer_universal_data_get_vehicle_selection_definition_index(int32 name);
extern int16 __cdecl multiplayer_universal_data_get_vehicle_set_absolute_index_from_string_id(int32 name);
extern int32 __cdecl multiplayer_universal_data_get_vehicle_set_name_from_absolute_index(int16 absolute_index);
extern int32 __cdecl multiplayer_universal_data_get_weapon_selection_definition_index(int32 name);
extern int16 __cdecl multiplayer_universal_data_get_weapon_set_absolute_index_from_string_id(int32 name);
extern int32 __cdecl multiplayer_universal_data_get_weapon_set_name_from_absolute_index(int16 absolute_index);
extern const s_multiplayer_vehicle_set* __cdecl multiplayer_universal_data_vehicle_set_try_and_get(int16 vehicle_set_index);
extern const s_multiplayer_weapon_set* __cdecl multiplayer_universal_data_weapon_set_try_and_get(int16 weapon_set_index);
extern s_multiplayer_runtime_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_runtime_data();
extern s_multiplayer_universal_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_universal_data();

extern s_multiplayer_universal_globals_definition* multiplayer_globals_try_and_get_universal_data(s_multiplayer_globals_definition* multiplayer_globals);

