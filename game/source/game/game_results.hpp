#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/tools/network_webstats.hpp"
#include "game/game_options.hpp"

struct s_game_results_game_description // s_game_results_multiplayer_game_description?
{
	bool team_game;
	uns64 game_instance;
	c_game_variant game_variant;
	c_static_wchar_string<32> map_variant_name;
	e_map_id map_id;
	c_static_string<260> scenario_path;
	bool started;
	uns32 start_time;
	bool finished;
	uns32 finish_time;

	uns8 __unknown3CC;
	uns8 __unknown3CD;
	bool simulation_aborted;

	byte __pad3CF[0x1];
};
COMPILE_ASSERT(sizeof(s_game_results_game_description) == 0x3D0);

struct s_integer_statistic_update
{
	uns16 statistic;
};

struct s_game_results_player_data
{
	bool player_exists;
	uns8 machine_index;

	union
	{
#pragma pack(push, 1)
		struct
		{
			s_player_identifier player_identifier;
			byte __padA[0x6];
		};
#pragma pack(pop)

		byte data[sizeof(s_player_identifier) + 0x6];
	};

	s_player_configuration configuration;

	uns8 player_standing;
	byte __pad1631[0xB];

	int32 player_score;
};
COMPILE_ASSERT(sizeof(s_game_results_player_data) == 0x1640);

struct s_game_results_player_data_update
{
	bool valid;
	bool data_valid;
	byte __pad2[0x6];
	s_game_results_player_data update;
};
COMPILE_ASSERT(sizeof(s_game_results_player_data_update) == 0x1648);

struct s_game_results_team_data
{
	bool team_exists;
	uns8 team_standing;
	uns16 team_score;
};
COMPILE_ASSERT(sizeof(s_game_results_team_data) == 0x4);

struct s_game_results_team_data_update
{
	bool valid;
	bool data_valid;

	s_game_results_team_data update;
};
COMPILE_ASSERT(sizeof(s_game_results_team_data_update) == 0x6);

struct s_game_results_player_medal_statistics_update
{
	bool valid;
	byte __pad[0x1]; // statistics_valid?
	s_integer_statistic_update statistics[k_game_results_medal_count];
};
COMPILE_ASSERT(sizeof(s_game_results_player_medal_statistics_update) == 0x76);

struct s_game_results_player_achievement_statistics_update
{
	bool valid;
	byte __pad[0x1]; // statistics_valid?
	s_integer_statistic_update statistics[k_achievement_count];
};
COMPILE_ASSERT(sizeof(s_game_results_player_achievement_statistics_update) == 0x60);

struct s_game_results_player_damage_statistics_update
{
	bool valid;
	byte __pad1[0x1]; // statistics_valid?
	s_integer_statistic_update statistics[k_game_results_damage_statistic_count];
};
COMPILE_ASSERT(sizeof(s_game_results_player_damage_statistics_update) == 0xC);

struct s_game_results_player_statistics
{
	s_integer_statistic_update statistics[k_game_results_statistic_count];

	s_integer_statistic_update medals[k_game_results_medal_count];
	s_integer_statistic_update achievements[k_achievement_count];

	s_game_results_player_damage_statistics_update damage[k_damage_reporting_type_count];
};
COMPILE_ASSERT(sizeof(s_game_results_player_statistics) == 0x438);

struct s_game_results_player_statistics_update
{
	bool valid;
	bool statistics_valid;
	s_integer_statistic_update statistics[k_game_results_statistic_count];

	s_game_results_player_medal_statistics_update medals;
	s_game_results_player_achievement_statistics_update achievements;
	s_game_results_player_damage_statistics_update damage[k_damage_reporting_type_count];
};
COMPILE_ASSERT(sizeof(s_game_results_player_statistics_update) == 0x43E);

enum e_game_results_player_vs_player_statistic
{
	_game_results_player_vs_player_statistic_player_vs_player_kills = 0,
	_game_results_player_vs_player_statistic_player_vs_player_deaths,

	k_game_results_player_vs_player_statistic_count
};

struct s_game_results_player_vs_player_statistics
{
	s_integer_statistic_update statistics[k_game_results_player_vs_player_statistic_count];
};
COMPILE_ASSERT(sizeof(s_game_results_player_vs_player_statistics) == 0x4);

struct s_game_results_player_vs_player_statistics_update
{
	bool valid;
	byte __pad1[0x1]; // statistics_valid?
	s_game_results_player_vs_player_statistics update;
};
COMPILE_ASSERT(sizeof(s_game_results_player_vs_player_statistics_update) == 0x6);

struct s_game_results_team_statistics
{
	s_integer_statistic_update statistics[k_game_results_statistic_count];
};
COMPILE_ASSERT(sizeof(s_game_results_team_statistics) == 0x66);

struct s_game_results_team_statistics_update
{
	bool team_valid;
	byte __pad1[0x1]; // statistics_valid?
	s_game_results_team_statistics update;
};
COMPILE_ASSERT(sizeof(s_game_results_team_statistics_update) == 0x68);

struct s_game_results_statistics
{
	s_game_results_player_statistics player[16];

	// player_vs_player[subject_player_absolute_index][reference_player_absolute_index].statistics
	c_static_array<c_static_array<s_game_results_player_vs_player_statistics, 16>, 16> player_vs_player;

	c_static_array<s_game_results_team_statistics, 16> team;
};
COMPILE_ASSERT(sizeof(s_game_results_statistics) == 0x4DE0);

struct s_game_results_statistics_update
{
	c_static_array<s_game_results_player_statistics_update, 16> player;

	// player_vs_player[subject_player_absolute_index][reference_player_absolute_index].statistics
	c_static_array<c_static_array<s_game_results_player_vs_player_statistics_update, 16>, 16> player_vs_player;

	c_static_array<s_game_results_team_statistics_update, 16> team;
};
COMPILE_ASSERT(sizeof(s_game_results_statistics_update) == 0x5060);

#pragma pack(push, 1)
struct s_game_results_machine_data
{
	s_machine_identifier machine_identifier;

	bool machine_exists;
	bool machine_connected;
	bool machine_host;
	bool machine_initial_host;
	bool machine_voluntary_quit;

	byte __pad17[0x1];

	struct
	{
		// TODO
		byte __data[0xA];
	} machine_bandwidth_estimate;
};
COMPILE_ASSERT(sizeof(s_game_results_machine_data) == 0x20);
#pragma pack(pop)

struct s_game_results_machine_data_update
{
	bool machine_valid;
	byte __pad1[0x1];

	s_game_results_machine_data update;
};
COMPILE_ASSERT(sizeof(s_game_results_machine_data_update) == 0x22);

struct s_game_results_incremental_update
{
	bool started;
	uns32 start_time;
	bool finished;
	uns32 finish_time;
	bool finalized;
	uns32 finish_reason;
	c_static_array<s_game_results_player_data_update, 16> player_updates;
	c_static_array<s_game_results_team_data_update, 16> team_updates;
	s_game_results_statistics_update statistics;
	c_static_array<s_game_results_machine_data_update, 17> machines;
	byte __pad1[0x6];
};
COMPILE_ASSERT(sizeof(s_game_results_incremental_update) == 0x1B7A0);

struct s_game_results_incremental
{
	bool finalized;
	bool started;
	uns32 start_time;
	bool finished;
	uns32 finish_time;
	c_static_array<s_game_results_player_data, 16> players;
	c_static_array<s_game_results_team_data, 16> teams;
	s_game_results_statistics statistics;
	c_static_array<s_game_results_machine_data, 17> machines;
};
COMPILE_ASSERT(sizeof(s_game_results_incremental) == 0x1B450);

struct s_game_results_event
{
	/* 0x00 */ c_enum<e_game_results_event_type, int8, _game_results_event_type_none, k_game_results_event_type_count> type;

	union
	{
#pragma pack(push, 1)
		struct
		{
			/* 0x01 */ uns8 killing_player_index;
			/* 0x02 */ uns8 dead_player_index;
			/* 0x03 */ byte __pad3;

			/* 0x04 */ real_point3d killing_player_position;
			/* 0x10 */ real_point3d dead_player_position;

			/* 0x1C */ c_enum<e_damage_reporting_type, int32, _damage_reporting_type_unknown, k_damage_reporting_type_count> tracked_damage_type; // from damage reporting info
		} kill;

		struct
		{
			/* 0x01 */ uns8 player_index;
			/* 0x02 */ uns8 __unknown2; // -1
			/* 0x03 */ byte __pad3;

			/* 0x04 */ real_point3d player_position;

			/* 0x10 */ int32 carry;      // weapon_identifier?
			/* 0x14 */ int32 carry_type; // e_game_results_event_carry_type
		} carry;

		struct
		{
			/* 0x01 */ uns8 player_index;
			/* 0x02 */ uns8 __unknown2; // -1
			/* 0x03 */ byte __pad3;

			/* 0x04 */ real_point3d player_position;

			/* 0x10 */ int32 score;
			/* 0x14 */ int32 score_type; // e_game_results_event_score_type
		} score;
#pragma pack(pop)

		byte event_type_storage[0x1F];
	};

	/* 0x20 */ uns32 time;
};
COMPILE_ASSERT(sizeof(s_game_results_event) == 0x24);

class c_game_results
{
public:
	uns8 finish_reason;
	bool initialized;
	bool finalized;

	// is this game result specific version of `s_game_matchmaking_options`?
	s_game_matchmaking_options matchmaking_options;
	s_game_results_game_description game_description;
	c_static_array<s_game_results_player_data, 16> players;
	c_static_array<s_game_results_team_data, 16> teams;
	s_game_results_statistics statistics;
	c_static_array<s_game_results_event, 1000> events;
	c_static_array<s_game_results_machine_data, 17> machines;
};
COMPILE_ASSERT(sizeof(c_game_results) == 0x24510);

struct s_game_results_globals
{
	bool recording;
	bool recording_paused;
	bool updating;
	int32 event_index;
	uns32 __time8;
	c_game_results* results;
};
COMPILE_ASSERT(sizeof(s_game_results_globals) == 0x10);

class c_simulation_view;
class c_game_results_replicator
{
public:
	c_simulation_view* m_view;
	bool m_has_fatal_error;
	bool m_sending_updates;
	bool m_receiving_updates;
	int32 m_incremental_update_number;
	uns32 __unknownC;
	s_game_results_incremental m_incremental;
	uns32 __time1B460;
	byte __data1B464[0x4];
};
COMPILE_ASSERT(sizeof(c_game_results_replicator) == 0x1B468);

struct s_integer_statistic_definition
{
	const char* name;

	// type?
	uns32 __unknown4;

	uns16 minimum_value;
	uns16 maximum_value;
	uns32 encoding_bits;
};
COMPILE_ASSERT(sizeof(s_integer_statistic_definition) == 0x10);

extern s_integer_statistic_definition(&g_game_results_achievement_statistic_definitions)[k_achievement_count];
extern s_integer_statistic_definition(&g_game_results_statistic_definitions)[k_game_results_statistic_count];
extern s_integer_statistic_definition(&g_game_results_damage_statistic_definitions)[k_game_results_damage_statistic_count];
extern s_integer_statistic_definition(&g_game_results_player_vs_player_statistic_definitions)[k_game_results_player_vs_player_statistic_count];
extern s_integer_statistic_definition(&g_game_results_medal_statistic_definitions)[k_game_results_medal_count];

extern s_game_results_globals& g_game_results_globals;
extern c_game_results& g_current_game_results;

class c_bitstream;

extern void __cdecl game_results_dispose();
extern void __cdecl game_results_dispose_from_old_map();
extern void __cdecl game_results_initialize_for_new_map();
extern void __cdecl game_results_statistic_increment(int32 player_absolute_index, int32 team_index, e_game_results_statistic statistic, int32 a4);
extern void __cdecl game_results_statistic_set(int32 player_absolute_index, int32 team_index, e_game_results_statistic statistic, int32 a4);
extern bool __cdecl game_results_statistics_decode(c_bitstream* packet, s_integer_statistic_update* statistics, int32 statistics_count, const s_integer_statistic_definition* statistic_definitions, int32 statistics_definitions_count);
extern void __cdecl game_results_statistics_encode(c_bitstream* packet, const s_integer_statistic_update* statistics, int32 statistics_count, const s_integer_statistic_definition* statistic_definitions, int32 statistics_definitions_count);
extern void __cdecl game_results_update();

