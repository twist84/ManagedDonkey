#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_progression.hpp"
#include "game/players.hpp"
#include "networking/tools/network_webstats.hpp"
#include "saved_games/scenario_map_variant.hpp"
#include "shell/shell.hpp"

#pragma warning(push)
#pragma warning(disable : 26495)
struct game_options
{
	game_options()
	{
		DECLFUNC(0x00485420, void, __thiscall, game_options*)(this);
	}

	long game_mode;
	char game_simulation;
	char game_network_type;
	short game_tick_rate;
	uint64 game_instance;
	uint32 random_seed;
	e_language language;
	long determinism_version;
	e_campaign_id campaign_id;
	e_map_id map_id;
	c_static_string<260> scenario_path;
	short initial_zone_set_index;
	bool load_level_only;
	uint8 dump_machine_index; // 5 bits, 32 max, is this be a char/int8?
	bool dump_object_log;
	bool dump_random_seeds;
	bool playtest_mode;
	short game_playback;
	bool record_saved_film;
	long playback_start_ticks;
	long playback_length_in_ticks;
	short campaign_difficulty;
	short campaign_insertion_point;
	short campaign_metagame_scoring;
	bool campaign_metagame_enabled;
	bool survival_enabled;
	uint8 : 8; // halo3_tag_test: campaign_allow_persistent_storage
	uint8 : 8; // halo3_tag_test: campaign_customization_enabled
	s_campaign_armaments campaign_armaments;
	uint8 : 8;
	uint8 : 8;
	s_campaign_game_progression campaign_game_progression;
	long campaign_active_primary_skulls;
	long campaign_active_secondary_skulls;
	s_hub_progression hub_progression;
	bool matchmade_game;
	uint8 pad1[7];
	s_game_matchmaking_options matchmaking_options;
	c_game_variant multiplayer_variant;
	c_map_variant map_variant;
	game_machine_options machines;
	game_player_options players[16];
};
static_assert(sizeof(game_options) == 0x24B48);
#pragma warning(pop)

