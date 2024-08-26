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

	c_enum<e_game_mode, long, _game_mode_none, k_game_mode_count> game_mode;
	c_enum<e_game_simulation_type, char, _game_simulation_none, k_game_simulation_count> game_simulation;
	byte game_network_type;
	short game_tick_rate;
	qword game_instance;
	dword random_seed;
	c_enum<e_language, dword, _language_invalid, k_language_count> language;
	long determinism_version;
	long campaign_id;
	long map_id;
	c_static_string<260> scenario_path;
	short initial_zone_set_index;
	bool load_level_only;
	byte dump_machine_index; // 5 bits, 32 max, is this be a char/int8?
	bool dump_object_log;
	bool dump_random_seeds;
	bool playtest_mode;
	byte : 8;
	c_enum<e_game_playback_type, short, _game_playback_none, k_game_playback_count> game_playback;
	bool record_saved_film;
	byte : 8;
	long playback_start_ticks;
	long playback_length_in_ticks;
	c_enum<e_campaign_difficulty_level, short, _campaign_difficulty_level_easy, k_number_of_campaign_difficulty_levels> campaign_difficulty;
	short campaign_insertion_point;
	short campaign_metagame_scoring;
	bool campaign_metagame_enabled;
	bool survival_enabled;
	byte : 8; // halo3_tag_test: campaign_allow_persistent_storage
	byte : 8; // halo3_tag_test: campaign_customization_enabled
	s_campaign_armaments campaign_armaments;
	byte : 8;
	byte : 8;
	s_campaign_game_progression campaign_game_progression;
	c_flags<e_primary_skulls, long, k_number_of_primary_skulls> initial_primary_skulls;
	c_flags<e_secondary_skulls, long, k_number_of_secondary_skulls> initial_secondary_skulls;
	s_hub_progression hub_progression;
	bool matchmade_game;
	byte __align2C9[7];
	s_game_matchmaking_options matchmaking_options;
	c_game_variant game_variant;
	c_map_variant map_variant;
	game_machine_options machine_options;
	c_static_array<game_player_options, 16> players;
};
static_assert(sizeof(game_options) == 0x24B48);
#pragma warning(pop)

