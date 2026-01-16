#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_progression.hpp"
#include "game/players.hpp"
#include "networking/tools/network_webstats.hpp"
#include "saved_games/scenario_map_variant.hpp"

#pragma warning(push)
#pragma warning(disable : 26495)
struct game_options
{
	game_options()
	{
		DECLFUNC(0x00485420, void, __thiscall, game_options*)(this);
	}

	int32 game_mode;
	int8 game_simulation;
	int8 game_network_type;
	int16 game_tick_rate;
	uns64 game_instance;
	uns32 random_seed;
	e_language language;
	int32 determinism_version;
	e_campaign_id campaign_id;
	e_map_id map_id;
	c_static_string<260> scenario_path;
	int16 initial_zone_set_index;
	bool load_level_only;
	int8 dump_machine_index;
	bool dump_object_log;
	bool dump_random_seeds;
	bool playtest_mode;
	int16 game_playback;
	bool record_saved_film;
	int32 playback_start_tick;
	int32 playback_length_in_ticks;
	int16 campaign_difficulty;
	int16 campaign_insertion_point;
	int16 campaign_metagame_scoring;
	bool campaign_metagame_enabled;
	bool survival_enabled;
	uns8 : 8; // halo3_tag_test: campaign_allow_persistent_storage
	uns8 : 8; // halo3_tag_test: campaign_customization_enabled
	s_campaign_armaments campaign_armaments;
	uns8 : 8;
	uns8 : 8;
	s_campaign_game_progression campaign_game_progression;
	int32 campaign_active_primary_skulls;
	int32 campaign_active_secondary_skulls;
	s_hub_progression hub_progression;
	bool matchmade_game;
	byte pad1[7];
	s_game_matchmaking_options matchmaking_options;
	c_game_variant multiplayer_variant;
	c_map_variant map_variant;
	game_machine_options machines;
	game_player_options players[16];
};
COMPILE_ASSERT(sizeof(game_options) == 0x24B48);
#pragma warning(pop)

