#pragma once

#include "cseries/cseries.hpp"

enum e_scenario_type
{
	_scenario_type_solo = 0,
	_scenario_type_multiplayer,
	_scenario_type_main_menu,
	_scenario_type_multiplayer_shared,
	_scenario_type_single_player_shared,

	k_scenario_type_count
};

enum e_scenario_campaign_type
{
	_scenario_campaign_type_unknown = 0,
	_scenario_campaign_type_hub,
	_scenario_campaign_type_linear,
	_scenario_campaign_type_scene,
	_scenario_campaign_type_cinematic,

	k_scenario_campaign_type_count
};

enum e_scenario_flags
{
	/* H2EK
	0	"cortana hack#sort cortana in front of other transparent geometry",
	1	"always draw sky#always draw sky 0, even if no +sky polygons are visible",
	2	"don't strip pathfinding#always leave pathfinding in, even for a multiplayer scenario",
	3	"symmetric multiplayer map",
	4	"quick loading (cinematic only) scenario",
	5	"characters use previous mission weapons",
	6	"lightmaps smooth palettes with neighbors",
	7	"snap to white at start"
	*/

	/* H3ODSTEK
	0	"don't strip pathfinding#always leave pathfinding in, even for a multiplayer scenario",
	1	"symmetric multiplayer map",
	2	"quick loading (cinematic only) scenario",
	3	"characters use previous mission weapons",
	4	"override globals!",
	5	"big vehicle use center point for light sampling",
	6	"don't use campaign sharing",
	7	"use eldo-like objects lighting sampling#sample from hi-poly poops",
	8	"survival",
	9	"force load objects in mode specific globals#forces attempt_to_load_missing_objects for MP and Survival"
	*/

	_scenario_flag_0_bit = 0,
	_scenario_flag_1_bit,
	_scenario_flag_2_bit,
	_scenario_flag_3_bit,
	_scenario_flag_4_bit,

	// TagTool has this here, making its index the same as H2EK
	_scenario_characters_use_previous_mission_weapons_bit,

	k_scenario_flag_count
};

// bare minimum definition just for `scenario_tags_match`
struct scenario
{
	c_enum<e_scenario_type, char, k_scenario_type_count> type;
	c_enum<e_scenario_campaign_type, char, k_scenario_campaign_type_count> campaign_type;
	c_flags<e_scenario_flags, word, k_scenario_flag_count> flags;
	long campaign_id;
	long map_id;

	//...
};