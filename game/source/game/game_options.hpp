#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "game/game_engine_variant.hpp"
#include "game/game_engine_ctf.hpp"
#include "game/game_engine_slayer.hpp"
#include "game/game_engine_oddball.hpp"
#include "game/game_engine_king.hpp"
#include "game/game_engine_sandbox.hpp"
#include "game/game_engine_vip.hpp"
#include "game/game_engine_juggernaut.hpp"
#include "game/game_engine_territories.hpp"
#include "game/game_engine_assault.hpp"
#include "game/game_engine_infection.hpp"
#include "game/game_progression.hpp"
#include "game/players.hpp"
#include "networking/tools/network_webstats.hpp"
#include "saved_games/scenario_map_variant.hpp"
#include "shell/shell.hpp"

struct c_game_variant
{
public:
	c_game_variant();

	e_game_engine_type get_game_engine_index() const;
	void set_game_engine_index(e_game_engine_type game_engine_index);
	c_game_engine_base_variant const* get_active_variant() const;
	c_game_engine_base_variant* get_active_variant_writeable();
	c_game_engine_ctf_variant const* get_ctf_variant() const;
	c_game_engine_ctf_variant* get_ctf_variant_writeable();
	c_game_engine_slayer_variant const* get_slayer_variant() const;
	c_game_engine_slayer_variant* get_slayer_variant_writeable();
	c_game_engine_oddball_variant const* get_oddball_variant() const;
	c_game_engine_oddball_variant* get_oddball_variant_writeable();
	c_game_engine_king_variant const* get_king_variant() const;
	c_game_engine_king_variant* get_king_variant_writeable();
	c_game_engine_sandbox_variant const* get_sandbox_variant() const;
	c_game_engine_sandbox_variant* get_sandbox_variant_writeable();
	c_game_engine_vip_variant const* get_vip_variant() const;
	c_game_engine_vip_variant* get_vip_variant_writeable();
	c_game_engine_juggernaut_variant const* get_juggernaut_variant() const;
	c_game_engine_juggernaut_variant* get_juggernaut_variant_writeable();
	c_game_engine_territories_variant const* get_territories_variant() const;
	c_game_engine_territories_variant* get_territories_variant_writeable();
	c_game_engine_assault_variant const* get_assault_variant() const;
	c_game_engine_assault_variant* get_assault_variant_writeable();
	c_game_engine_infection_variant const* get_infection_variant() const;
	c_game_engine_infection_variant* get_infection_variant_writeable();

protected:

	c_enum<e_game_engine_type, long, _game_engine_base_variant, k_game_engine_type_count> m_game_engine_index;
	union
	{
		c_game_engine_base_variant m_base_variant;
		c_game_engine_ctf_variant m_ctf_variant;
		c_game_engine_slayer_variant m_slayer_variant;
		c_game_engine_oddball_variant m_oddball_variant;
		c_game_engine_king_variant m_king_variant;
		c_game_engine_sandbox_variant m_sandbox_variant;
		c_game_engine_vip_variant m_vip_variant;
		c_game_engine_juggernaut_variant m_juggernaut_variant;
		c_game_engine_territories_variant m_territories_variant;
		c_game_engine_assault_variant m_assault_variant;
		c_game_engine_infection_variant m_infection_variant;
		byte m_game_engine_variant[0x260];
	};
};
static_assert(sizeof(c_game_variant) == 0x264);

struct game_options
{
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
	char scenario_path[260];
	short initial_zone_set_index;
	bool load_level_only;
	bool dump_machine_index;
	bool dump_object_log;
	bool dump_random_seeds;
	bool playtest_mode;
	byte : 8;
	c_enum<e_game_playback_type, short, _game_playback_none, k_game_playback_count> game_playback;
	bool record_saved_film;
	byte : 8;
	long playback_start_ticks;
	long playback_length_in_ticks;
	c_enum<e_campaign_difficulty_level, short, _campaign_difficulty_level_easy, k_campaign_difficulty_levels_count> campaign_difficulty;
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
	c_flags<e_campaign_skulls_primary, long, k_campaign_skulls_primary_count> skulls_primary;
	c_flags<e_campaign_skulls_secondary, long, k_campaign_skulls_secondary_count> skulls_secondary;
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
