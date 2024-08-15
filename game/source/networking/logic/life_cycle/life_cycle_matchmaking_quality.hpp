#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/transport/transport_qos.hpp"

struct s_game_hopper_picked_game
{
	long map_id;

	char game_variant_name[32];
	s_network_http_request_hash game_variant_hash;

	char map_variant_name[32];
	s_network_http_request_hash map_variant_hash;
};

struct s_game_hopper_picked_game_collection
{
	c_static_array<s_game_hopper_picked_game, 4> picked_games;
};
static_assert(sizeof(s_game_hopper_picked_game_collection) == 0x1B0);

struct s_matchmaking_quality_qos_sample
{
	byte __data0[0x20];
	s_transport_qos_result qos_result;
};
static_assert(sizeof(s_matchmaking_quality_qos_sample) == 0x40);

struct s_matchmaking_session_search
{
	byte __data[0x340];
};
static_assert(sizeof(s_matchmaking_session_search) == 0x340);

struct s_matchmaking_quality_data
{
	bool m_aborted;
	bool __unknown1_assemble;
	bool m_left_assemble;
	bool m_left_arbitration;
	bool m_not_enough_hosts;
	bool m_left_host_selection;
	bool m_left_prepare_map;
	bool m_vetoed;
	byte __unknown8_arbitration;
	byte __unknown9_arbitration;
	byte __unknownA_start;
	byte __unknownB_start;
	byte __unknownC_start;
	byte __padD[3];
	s_game_hopper_picked_game m_picked_game;
	dword m_ping;
	dword m_search_time;
	dword m_gather_time;
	dword m_arbitration_time;
	dword m_host_selection_time;
	dword m_prepare_map_time;
	dword m_in_match_time;
	byte __data98[0x20];
	long m_qos_sample_count;
	c_static_array<s_matchmaking_quality_qos_sample, 400> m_qos_samples;
	long m_session_search_count;
	c_static_array<s_matchmaking_session_search, 16> m_session_search_data;
	bool m_has_live_service_qos;
	s_transport_qos_result m_live_service_qos;
	bool m_has_nat_type;
	long m_nat_type;
	bool __unknown98EC_prepare_map;
};
static_assert(sizeof(s_matchmaking_quality_data) == 0x98F0);

struct c_matchmaking_quality
{
	bool m_needs_submission_to_webstats;
	dword __unknown4;
	s_matchmaking_quality_data m_data;
};
static_assert(sizeof(c_matchmaking_quality) == 0x98F8);

