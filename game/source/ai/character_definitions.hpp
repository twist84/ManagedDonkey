#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

enum e_character_flags
{
	_character_flag_flag1_bit = 0,

	k_character_flags
};

struct character_variant;
struct character_voice_properties;
struct character_general_properties;
struct character_vitality_properties;
struct character_placement_properties;
struct character_perception_properties;
struct character_look_properties;
struct character_movement_properties;
struct character_flocking_properties;
struct character_swarm_properties;
struct character_ready_properties;
struct character_engage_properties;
struct character_charge_properties;
struct character_evasion_properties;
struct character_cover_properties;
struct character_retreat_properties;
struct character_search_properties;
struct character_presearch_properties;
struct character_idle_properties;
struct character_vocalization_properties;
struct character_boarding_properties;
struct character_kungfu_properties;
struct character_guardian_properties;
struct character_combatform_properties;
struct character_engineer_properties;
struct character_inspect_properties;
struct character_scarab_properties;
struct character_weapons_properties;
struct character_firing_pattern_properties;
struct character_grenades_properties;
struct character_vehicle_properties;
struct character_morph_properties;
struct character_equipment_definition;
struct s_campaign_metagame_scenario;
struct character_activity_objects;

struct character_definition
{
	static tag const k_group_tag = CHARACTER_TAG;

	c_enum<e_character_flags, uns32, _character_flag_flag1_bit, k_character_flags> character_flags;
	c_typed_tag_reference<CHARACTER_TAG, INVALID_TAG> parent_character;
	c_typed_tag_reference<UNIT_TAG, INVALID_TAG> unit;
	c_typed_tag_reference<CREATURE_TAG, INVALID_TAG> creature;
	c_typed_tag_reference<STYLE_TAG, INVALID_TAG> style;
	c_typed_tag_reference<CHARACTER_TAG, INVALID_TAG> major_character;
	c_typed_tag_block<character_variant> variants;
	c_typed_tag_block<character_voice_properties> voice;
	c_typed_tag_block<character_general_properties> general_properties;
	c_typed_tag_block<character_vitality_properties> vitality_properties;
	c_typed_tag_block<character_placement_properties> placement_properties;
	c_typed_tag_block<character_perception_properties> perception_properties;
	c_typed_tag_block<character_look_properties> look_properties;
	c_typed_tag_block<character_movement_properties> movement_properties;
	c_typed_tag_block<character_flocking_properties> flocking_properties;
	c_typed_tag_block<character_swarm_properties> swarm_properties;
	c_typed_tag_block<character_ready_properties> ready_properties;
	c_typed_tag_block<character_engage_properties> engage_properties;
	c_typed_tag_block<character_charge_properties> charge_properties;

	// Danger Values
	// Danger values can be found in the ai-globals section of the globals tag.
	c_typed_tag_block<character_evasion_properties> evasion_properties;
	c_typed_tag_block<character_cover_properties> cover_properties;
	c_typed_tag_block<character_retreat_properties> retreat_properties;
	c_typed_tag_block<character_search_properties> search_properties;
	c_typed_tag_block<character_presearch_properties> presearch_properties;
	c_typed_tag_block<character_idle_properties> idle_properties;
	c_typed_tag_block<character_vocalization_properties> vocalization_properties;
	c_typed_tag_block<character_boarding_properties> boarding_properties;
	c_typed_tag_block<character_kungfu_properties> kungfu_properties;
	c_typed_tag_block<character_guardian_properties> guardian_properties;
	c_typed_tag_block<character_combatform_properties> combatform_properties;
	c_typed_tag_block<character_engineer_properties> engineer_properties;
	c_typed_tag_block<character_inspect_properties> inspect_properties;
	c_typed_tag_block<character_scarab_properties> scarab_properties;
	c_typed_tag_block<character_weapons_properties> weapons_properties;
	c_typed_tag_block<character_firing_pattern_properties> firing_pattern_properties;
	c_typed_tag_block<character_grenades_properties> grenades_properties;
	c_typed_tag_block<character_vehicle_properties> vehicle_properties;
	c_typed_tag_block<character_morph_properties> morph_properties;
	c_typed_tag_block<character_equipment_definition> equipment_definitions;
	c_typed_tag_block<s_campaign_metagame_scenario> campaign_metagame_bucket;
	c_typed_tag_block<character_activity_objects> activity_objects;
};
COMPILE_ASSERT(sizeof(character_definition) == 0x1F8);

struct s_character_voice
{
	c_typed_tag_reference<DIALOGUE_TAG, INVALID_TAG> dialogue;
	c_string_id designator;
	real32 weight;
};
COMPILE_ASSERT(sizeof(s_character_voice) == 0x18);

struct character_variant
{
	c_string_id variant_name;
	int16 variant_index;
	int16 pad0;
	c_typed_tag_block<s_character_voice> voices;
};
COMPILE_ASSERT(sizeof(character_variant) == 0x14);

struct character_voice_properties
{
	c_typed_tag_block<s_character_voice> voices;
};
COMPILE_ASSERT(sizeof(character_voice_properties) == 0xC);

struct character_general_properties
{
};
COMPILE_ASSERT(sizeof(character_general_properties) == 0x1);

struct character_vitality_properties
{
};
COMPILE_ASSERT(sizeof(character_vitality_properties) == 0x1);

struct character_placement_properties
{
	int32 flags;
	real32 few_upgrade_chance[sizeof(real32) * k_campaign_difficulty_levels_count];
	real32 normal_upgrade_chance[sizeof(real32) * k_campaign_difficulty_levels_count];
	real32 many_upgrade_chance[sizeof(real32) * k_campaign_difficulty_levels_count];
};
COMPILE_ASSERT(sizeof(character_placement_properties) == 0xC4);

struct character_perception_properties
{
	uns32 flags;
	real32 maximum_vision_distance;
	real32 central_vision_angle;
	real32 maximum_vision_angle;
	real32 peripheral_vision_angle;
	real32 peripheral_distance;
	real32 hearing_distance;
	real32 notice_projectile_chance;
	real32 notice_vehicle_chance;
	real32 awareness_time;
	real32 first_acknowledgement_surprise_distance;
};
COMPILE_ASSERT(sizeof(character_perception_properties) == 0x2C);

struct character_look_properties
{
	// how far we can turn our weapon
	real_euler_angles2d maximum_aiming_deviation; // degrees

	// how far we can turn our head
	real_euler_angles2d maximum_looking_deviation; // degrees

	real_euler_angles2d runtime_aiming_deviation_cosines;
	real_euler_angles2d runtime_looking_deviation_cosines;

	// how far we can turn our head left away from our aiming vector when not in combat
	real32 noncombat_look_delta_left; // degrees

	// how far we can turn our head right away from our aiming vector when not in combat
	real32 noncombat_look_delta_right; // degrees

	// how far we can turn our head left away from our aiming vector when in combat
	real32 combat_look_delta_left; // degrees

	// how far we can turn our head right away from our aiming vector when in combat
	real32 combat_look_delta_right; // degrees

	// rate at which we change look around randomly when not in combat
	real_bounds noncombat_idle_looking; // seconds

	// rate at which we change aiming directions when looking around randomly when not in combat
	real_bounds noncombat_idle_aiming; // seconds

	// rate at which we change look around randomly when searching or in combat
	real_bounds combat_idle_looking; // seconds

	// rate at which we change aiming directions when looking around randomly when searching or in combat
	real_bounds combat_idle_aiming; // seconds
};
COMPILE_ASSERT(sizeof(character_look_properties) == 0x50);

struct character_movement_properties
{
};
COMPILE_ASSERT(sizeof(character_movement_properties) == 0x1);

struct character_flocking_properties
{
	// Jaime, don't touch this
	// Ever.

	real32 deceleration_distance;
	real32 normalized_speed;
	real32 buffer_distance;
	real_bounds throttle_threshold_bounds;
	real32 deceleration_stop_time;
};
COMPILE_ASSERT(sizeof(character_flocking_properties) == 0x18);

struct character_swarm_properties
{
	// After the given number of deaths, the swarm scatters
	int16 scatter_killed_count;

	int16 pad1;

	// the distance from the target that the swarm scatters
	real32 scatter_radius;

	// amount of time to remain scattered
	real32 scatter_time;

	real32 hound_min_distance;
	real32 hound_max_distance;

	// how long the infection form and its victim will wrestle before the point of no return
	real_bounds infection_time; // secs

	// amount of randomness added to creature's throttle
	real32 perlin_offset_scale; // [0-1]

	// how fast the creature changes random offset to throttle
	real_bounds offset_period; // s

	// a random offset lower then given threshold is made 0. (threshold of 1 = no movement)
	real32 perlin_idle_movement_threshold; // [0-1]

	// a random offset lower then given threshold is made 0. (threshold of 1 = no movement)
	real32 perlin_combat_movement_threshold; // [0-1]

	// how long we have to move (stuck distance) before we get deleted
	real32 stuck_time;

	// how far we have to move in (stuck time) to not get deleted
	real32 stuck_distance;
};
COMPILE_ASSERT(sizeof(character_swarm_properties) == 0x38);

struct character_ready_properties
{
	// Character will pause for given time before engaging threat
	real_bounds ready_time_bounds;
};
COMPILE_ASSERT(sizeof(character_ready_properties) == sizeof(real_bounds));

struct character_engage_properties
{
};
COMPILE_ASSERT(sizeof(character_engage_properties) == 0x1);

struct character_charge_properties
{
	// Difficulty Limits
	// Per difficulty values to limit the number of guys in particular behaviours

	// How many guys in a single clump can be kamikazing at one time
	int16 max_kamikaze_count;

	// How many guys in a single clump can be berserking at one time
	int16 max_berserk_count;

	// We'd like at least this number of guys in a single clump can be berserking at one time (primarily combat forms)
	int16 min_berserk_count;
};
COMPILE_ASSERT(sizeof(character_charge_properties) == 0x6);

struct character_evasion_properties
{
	// Consider evading when immediate danger surpasses threshold
	real32 evasion_danger_threshold;

	// Wait at least this delay between evasions
	real32 evasion_delay_timer;

	// If danger is above threshold, the chance that we will evade.
	// Expressed as chance of evading within a 1 second time period
	real32 evasion_chance;

	// If target is within given proximity, possibly evade
	real32 evasion_proximity_threshold;

	// Chance of retreating (fleeing) after danger avoidance dive
	real32 dive_retreat_chance;
};
COMPILE_ASSERT(sizeof(character_evasion_properties) == 0x14);

struct character_cover_properties
{
};
COMPILE_ASSERT(sizeof(character_cover_properties) == 0x1);

struct character_retreat_properties
{
};
COMPILE_ASSERT(sizeof(character_retreat_properties) == 0x1);

struct character_search_properties
{
};
COMPILE_ASSERT(sizeof(character_search_properties) == 0x1);

struct character_presearch_properties
{
};
COMPILE_ASSERT(sizeof(character_presearch_properties) == 0x1);

struct character_idle_properties
{
	int32 flags;

	// time range for delays between idle poses
	real_bounds idle_pose_delay_time; // seconds

	// time to pause at a point while wandering
	real_bounds wander_delay_time; // seconds
};
COMPILE_ASSERT(sizeof(character_idle_properties) == 0x14);

struct character_vocalization_properties
{
	// Skip Fraction
	// For vocalization with priority < react, multiplies the skip fraction in the dialogue globals tag.
	// For priorities > react, multiplies the vocalization WEIGHT (i.e. the desire of that character to vocalize RELATIVE to other characters in the scene).
	// Thus if you hurt the arbiter, say, he's still going to yell at you, no matter what, and if you kill somebody and nobody else is there to witness it, he will yell at you.
	// As always, 0 means speak normally, 1 means skip everything (don't say anything)

	real32 character_skip_fraction; // [0,1]

	// How long does the player look at an AI before the AI responds?
	real32 look_comment_time; // s

	// How long does the player look at the AI before he responds with his 'long look' comment?
	real32 look_long_comment_time; // s
};
COMPILE_ASSERT(sizeof(character_vocalization_properties) == 0xC);

struct character_boarding_properties
{
};
COMPILE_ASSERT(sizeof(character_boarding_properties) == 0x1);

struct character_kungfu_properties
{
	// If the player is within this distance, open fire, even if your task is kungfu-fight disallowed"
	real32 kungfu_override_distance; // wus

	// If you are kungfu disallowed and your danger is above this level, take cover"
	real32 kungfu_cover_danger_threshold; // wus
};
COMPILE_ASSERT(sizeof(character_kungfu_properties) == 0x8);

struct character_guardian_properties
{
	// length of time for which the guardian surges
	real32 surge_time; // seconds

	// minimum enforced delay between surges
	real32 surge_delay_time; // seconds

	// surge when our target gets closer than this to me (0 value defaults to 2wu
	real32 proximity_surge_distance; // "wu

	// length of time it takes the guardian to get to its phase destination
	real32 phase_time; // seconds

	// Minimum distance that I will consider phasing
	real32 current_position_distance; // wu

	// Minimum distance from my target that I will phase to
	real32 target_position_distance; // wu
};
COMPILE_ASSERT(sizeof(character_guardian_properties) == 0x18);

struct character_combatform_properties
{
	// distance at which combatform will be forced into berserk
	real32 berserk_distance; // wu

	// chance of berserking this second
	real32 berserk_chance;
};
COMPILE_ASSERT(sizeof(character_combatform_properties) == 0x8);

struct character_engineer_properties
{
	// try and rise this amount before dying
	real32 death_height; // wu

	// spend this time rising
	real32 death_rise_time; // seconds

	// spend this time detonating
	real32 death_detonation_time; // seconds

	// Boost the shields of allies within this radius during combat" },
	real32 shield_boost_radius_max;

	// Allies within this radius get maximum shield boost" },
	real32 shield_boost_radius_min;

	// Boost allies' shields by this amount during combat" },
	real32 shield_boost_vitality;

	// Detonation Thresholds
	// If shield vitality AND body vitality fall below the thresholds below, detonate
	real32 detonation_shield_threshold;
	real32 detonation_body_vitality;

	// if target enters within this radius, either detonate or deploy equipment
	real32 proximity_radius; // wus

	// chance of detonating if target enters the drain radius radius" },
	real32 proximity_detonation_chance;

	// if target enters radius and detonation is not chosen, deploy this equipment.
	c_typed_tag_reference<EQUIPMENT_TAG, INVALID_TAG> proximity_equipment;
};
COMPILE_ASSERT(sizeof(character_engineer_properties) == 0x38);

struct character_inspect_properties
{
	// distance from object at which to stop and turn on the inspection light
	real32 stop_distance; // wu"

	// time which we should inspect each object for
	real_bounds inspect_time; // seconds"

	// range in which we should search for objects to inspect
	real_bounds search_range; // wu"
};
COMPILE_ASSERT(sizeof(character_inspect_properties) == 0x14);

struct character_scarab_properties
{
	// When target within this distance, the scarab will back up
	real32 fighting_min_distance; // wus

	// When target outside this distance, the scarab will chase
	real32 fighting_max_distance; // wus

	// When within these bounds distance from the target, we blend in our anticipated facing vector
	real_bounds anticipated_aim_radius; // wus

	// When moving forward within this dot of our desired facing, just move forward", nullptr
	real32 snap_forward_angle; // [0-1]

	// When moving forward within this dot of our desired facing, just move forward", nullptr
	real32 snap_forward_angle_max; // [0-1]
};
COMPILE_ASSERT(sizeof(character_scarab_properties) == 0x18);

struct character_weapons_properties
{
};
COMPILE_ASSERT(sizeof(character_weapons_properties) == 0x1);

struct character_firing_pattern_properties
{
};
COMPILE_ASSERT(sizeof(character_firing_pattern_properties) == 0x1);

struct character_grenades_properties
{
};
COMPILE_ASSERT(sizeof(character_grenades_properties) == 0x1);

struct character_vehicle_properties
{
};
COMPILE_ASSERT(sizeof(character_vehicle_properties) == 0x1);

struct character_morph_properties
{
};
COMPILE_ASSERT(sizeof(character_morph_properties) == 0x1);

struct character_equipment_definition
{
};
COMPILE_ASSERT(sizeof(character_equipment_definition) == 0x1);

struct character_activity_objects
{
	c_string_id activity_name;
	c_typed_tag_reference<CRATE_TAG, INVALID_TAG> crate;
	c_string_id crate_marker_name;
	c_string_id unit_marker_name;
};
COMPILE_ASSERT(sizeof(character_activity_objects) == 0x1C);

extern character_perception_properties* __cdecl actor_perception_properties_get(int32 actor_index);

