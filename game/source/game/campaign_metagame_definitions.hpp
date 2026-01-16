#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_campaign_metagame_style_type;
struct s_campaign_metagame_difficulty;
struct s_campaign_metagame_skull_data;
struct s_campaign_metagame_globals
{
	c_typed_tag_block<s_campaign_metagame_style_type> styles;
	c_typed_tag_block<s_campaign_metagame_difficulty> difficulty;
	c_typed_tag_block<s_campaign_metagame_skull_data> primary_skulls;
	c_typed_tag_block<s_campaign_metagame_skull_data> secondary_skulls;

	int32 friendly_death_point_count;
	int32 player_death_point_count;
	int32 player_betrayal_point_count;

	// how many kills for this to happen
	int32 multi_kill_count;

	// in what period of time does this have to happen (seconds)
	real32 multi_kill_window;

	// time after taking a guys shields down with emp damage you have to get the emp kill bonus (seconds)
	real32 emp_kill_window;

	// like EMP but for the jack-then-kill medal (seconds)
	real32 jack_kill_window_period;

	// like EMP but for the airborne jack-then-kill medal (seconds)
	real32 airborne_jack_kill_window_period;

	// window after which death from the grave medal can be awarded (seconds)
	real32 death_from_grave_required_period;

	// for sword/shotgun/snipes/etc medals
	int32 minor_spree_count;
	int32 major_spree_count;

	int32 killing_spree_threshold;
	int32 killing_frenzy_threshold;
	int32 running_riot_threshold;
	int32 rampage_threshold;
	int32 untouchable_threshold;
	int32 invincible_threshold;
	int32 double_kill_threshold;
	int32 triple_kill_threshold;
	int32 overkill_threshold;
	int32 killtacular_threshold;
	int32 killtrocity_threshold;
	int32 killamanjaro_threshold;
	int32 killtastrophe_threshold;
	int32 killpocalpyse_threshold;
	int32 killionaire_threshold;
};
COMPILE_ASSERT(sizeof(s_campaign_metagame_globals) == 0x98);

struct s_campaign_metagame_style_type
{
	real32 style_multiplier;
	int32 point_count;
	int32 display_ticks;
	c_string_id survival_mode_event_name;
};
COMPILE_ASSERT(sizeof(s_campaign_metagame_style_type) == 0x10);

struct s_campaign_metagame_difficulty
{
	real32 difficulty_multiplier;
};
COMPILE_ASSERT(sizeof(s_campaign_metagame_difficulty) == sizeof(real32));

struct s_campaign_metagame_skull_data
{
	real32 difficulty_multiplier;
};
COMPILE_ASSERT(sizeof(s_campaign_metagame_skull_data) == sizeof(real32));

enum e_campaign_metagame_bucket_flag
{
	_campaign_metagame_bucket_flag_only_counts_with_riders_bit = 0,

	k_campaign_metagame_bucket_flags
};

enum e_campaign_metagame_bucket_type
{
	_campaign_metagame_bucket_type_brute = 0,
	_campaign_metagame_bucket_type_grunt,
	_campaign_metagame_bucket_type_jackel,
	_campaign_metagame_bucket_type_marine,
	_campaign_metagame_bucket_type_bugger,
	_campaign_metagame_bucket_type_hunter,
	_campaign_metagame_bucket_type_flood_infection,
	_campaign_metagame_bucket_type_flood_carrier,
	_campaign_metagame_bucket_type_flood_combat,
	_campaign_metagame_bucket_type_flood_pure,
	_campaign_metagame_bucket_type_sentinel,
	_campaign_metagame_bucket_type_elite,
	_campaign_metagame_bucket_type_turret,
	_campaign_metagame_bucket_type_mongoose,
	_campaign_metagame_bucket_type_warthog,
	_campaign_metagame_bucket_type_scorpion,
	_campaign_metagame_bucket_type_hornet,
	_campaign_metagame_bucket_type_pelican,
	_campaign_metagame_bucket_type_shade,
	_campaign_metagame_bucket_type_watchtower,
	_campaign_metagame_bucket_type_ghost,
	_campaign_metagame_bucket_type_chopper,
	_campaign_metagame_bucket_type_mauler,
	_campaign_metagame_bucket_type_wraith,
	_campaign_metagame_bucket_type_banshee,
	_campaign_metagame_bucket_type_phantom,
	_campaign_metagame_bucket_type_scarab,
	_campaign_metagame_bucket_type_guntower,
	_campaign_metagame_bucket_type_engineer,
	_campaign_metagame_bucket_type_engineer_recharge_station,

	k_campaign_metagame_bucket_type_count
};

enum e_campaign_metagame_bucket_class
{
	_campaign_metagame_bucket_class_infantry = 0,
	_campaign_metagame_bucket_class_leader,
	_campaign_metagame_bucket_class_hero,
	_campaign_metagame_bucket_class_specialist,
	_campaign_metagame_bucket_class_light_vehicle,
	_campaign_metagame_bucket_class_heavy_vehicle,
	_campaign_metagame_bucket_class_giant_vehicle,
	_campaign_metagame_bucket_class_standard_vehicle,

	k_campaign_metagame_bucket_class_count
};

struct s_campaign_metagame_bucket
{
	c_flags<e_campaign_metagame_bucket_flag, int8, k_campaign_metagame_bucket_flags> flags;
	int8 type;
	int8 _class;
	byte pad0;
	int16 point_count;
	int16 pad1;
};
COMPILE_ASSERT(sizeof(s_campaign_metagame_bucket) == 0x8);

struct s_campaign_metagame_scenario;

