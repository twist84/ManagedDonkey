#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "game/game_mode.hpp"
#include "game/game_progression.hpp"
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

// TODO: find a home
enum e_game_simulation_type
{
	_game_simulation_none = 0,
	_game_simulation_local,
	_game_simulation_synchronous_client,
	_game_simulation_synchronous_server,
	_game_simulation_distributed_client,
	_game_simulation_distributed_server,

	k_game_simulation_count
};

// TODO: find a home
enum e_game_playback_type
{
	_game_playback_none = 0,
	_game_playback_local,
	_game_playback_network_server,
	_game_playback_network_client,

	k_game_playback_count
};

// TODO: find a home
enum e_campaign_difficulty_level
{
	_campaign_difficulty_level_easy = 0,
	_campaign_difficulty_level_normal,
	_campaign_difficulty_level_heroic,
	_campaign_difficulty_level_legendary,

	k_campaign_difficulty_levels_count
};

// TODO: find a home
enum e_campaign_skulls_primary
{
	_campaign_skull_iron = 0,
	_campaign_skull_black_eye,
	_campaign_skull_tough_luck,
	_campaign_skull_catch,
	_campaign_skull_fog,
	_campaign_skull_famine,
	_campaign_skull_thunderstorm,
	_campaign_skull_tilt,
	_campaign_skull_mythic,

	k_campaign_skull_primary_count
};

// TODO: find a home
enum e_campaign_skulls_secondary
{
	_campaign_skull_assassin = 0,
	_campaign_skull_blind,
	_campaign_skull_superman,
	_campaign_skull_birthday_party,
	_campaign_skull_daddy,
	_campaign_skull_third_person,
	_campaign_skull_directors_cut,

	k_campaign_skull_secondary_count
};

// TODO: find a home
struct s_game_matchmaking_options
{
	word hopper_identifier;
	byte xlast_index;
	bool is_ranked;
	bool team_game;
	byte : 8;
	wchar_t hopper_name[32];
	byte : 8;
	byte : 8;
	long draw_probability;
	long beta;
	long tau;
	long experience_base_increment;
	long experience_penalty_decrement;
};
static_assert(sizeof(s_game_matchmaking_options) == 0x5C);

// TODO: find a home
struct c_game_variant
{
	c_enum<e_game_engine_variant, long, k_game_engine_variant_count> m_game_engine_index;

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
		byte variant_data[0x260];
	};
};
static_assert(sizeof(c_game_variant) == 0x264);

// TODO: find a home
struct c_map_variant
{
	byte __data[0xE090];
};
static_assert(sizeof(c_map_variant) == 0xE090);

// TODO: find a home
struct s_game_machine_options
{
	byte __data[0x128];
};
static_assert(sizeof(s_game_machine_options) == 0x128);

// TODO: find a home
struct game_player_options
{
	byte __data[0x1640];
};
static_assert(sizeof(game_player_options) == 0x1640);

struct game_options
{
	c_enum<e_game_mode, long, k_game_mode_count> game_mode;
	c_enum<e_game_simulation_type, char, k_game_mode_count> game_simulation;
	byte game_network_type;
	short game_tick_rate;
	qword game_instance;
	dword random_seed;
	c_enum<e_language, long, k_language_count> language;
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
	c_enum<e_game_playback_type, short, k_game_playback_count> game_playback;
	bool record_saved_film;
	byte : 8;
	long playback_start_ticks;
	long playback_length_in_ticks;
	c_enum<e_campaign_difficulty_level, short, k_campaign_difficulty_levels_count> campaign_difficulty;
	short campaign_insertion_index;
	short campaign_metagame_scoring;
	bool campaign_metagame_enabled;
	bool survival_enabled;
	byte : 8; // halo3_tag_test: campaign_allow_persistent_storage
	byte : 8; // halo3_tag_test: campaign_customization_enabled
	s_campaign_armaments campaign_armaments;
	byte : 8;
	byte : 8;
	s_campaign_game_progression campaign_game_progression;
	c_flags<e_campaign_skulls_primary, long, k_campaign_skull_primary_count> skulls_primary;
	c_flags<e_campaign_skulls_secondary, long, k_campaign_skull_secondary_count> skulls_secondary;
	s_hub_progression hub_progression;
	bool matchmade_game;
	byte __align2C9[7];
	s_game_matchmaking_options matchmaking_options;
	c_game_variant game_variant;
	c_map_variant map_variant;
	s_game_machine_options machine_options;
	c_static_array<game_player_options, 16> players;
};
static_assert(sizeof(game_options) == 0x24B48);

extern bool game_in_startup_phase();
extern bool game_in_progress();
//extern long game_create_lock_resources(e_game_create_mode);
//extern void game_create_unlock_resources(e_game_create_mode, long&);
//extern void game_start(enum e_game_create_mode);
extern bool game_options_valid();
extern bool game_options_verify(game_options const* options, char* error_string, long error_string_length);
extern void assert_game_options_verify(game_options const* options);
extern game_options* game_options_get();
extern void game_options_print_game_id();
//extern void game_options_setup_for_saved_film(e_game_playback_type playback_type);
extern void game_options_clear_game_playback();
//extern void game_options_game_engine_fixup();
extern e_campaign_difficulty_level game_difficulty_level_get();
extern e_campaign_difficulty_level game_difficulty_level_get_ignore_easy();
extern e_game_mode game_mode_get();
extern bool game_is_ui_shell();
extern bool game_is_multiplayer();
extern bool game_is_campaign();
extern bool game_is_survival();
extern bool game_is_campaign_or_survival();
extern void game_set_active_skulls(dword* active_primary_skulls, dword* active_secondary_skulls);
extern void game_set_difficulty(e_campaign_difficulty_level campaign_difficulty);
//extern bool game_is_cooperative();
//extern long game_coop_player_count();
extern bool game_is_playtest();
//extern bool game_had_an_update_tick_this_frame();
extern e_game_simulation_type game_simulation_get();
extern bool game_is_playback();
extern e_game_playback_type game_playback_get();
//extern void game_playback_set(e_game_playback_type playback_type);
extern void game_simulation_set(e_game_simulation_type game_simulation);
extern bool game_is_synchronous_networking();
extern bool game_is_networked();
//extern bool game_is_in_progress_on_live();
extern bool game_is_server();
extern bool game_is_authoritative();
extern bool game_is_predicted();
extern bool game_is_distributed();
extern long game_tick_rate_get();
extern bool game_skull_is_active_primary(e_campaign_skulls_primary primary_skull);
extern bool game_skull_is_active_secondary(e_campaign_skulls_secondary secondary_skull);
extern void game_skull_enable_primary(e_campaign_skulls_primary primary_skull, bool enable);
extern void game_skull_enable_secondary(e_campaign_skulls_secondary secondary_skull, bool enable);
extern bool game_coop_allow_respawn();
//extern bool game_survival_allow_respawn(long);
extern e_language game_get_master_language();
//extern bool game_is_language_neutral();
extern void game_options_setup_default_players(long player_count, game_options* options);