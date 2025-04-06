#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/transport/transport_qos.hpp"

enum e_map_id;
struct s_game_hopper_picked_game
{
	e_map_id map_id;

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

struct __declspec(align(8)) s_matchmaking_quality_data
{
	bool aborted;
	bool assemble_timed_out;
	bool left_assemble;
	bool left_arbitration;
	bool not_enough_hosts;
	bool left_host_selection;
	bool left_prepare_map;
	bool vetoed;
	bool hit_arbitration_waiting_for_establishment_and_connectivity_give_up_time;
	bool hit_arbitration_waiting_for_completion_give_up_time;
	bool searching;
	bool gathering;
	bool gathering_by_force;
	s_game_hopper_picked_game vetoed_or_played_game;
	long ping_msec;
	dword search_time;
	dword gather_time;
	dword arbitration_time;
	dword host_selection_time;
	dword prepare_map_time;
	dword in_match_time;
	byte local_address[32];
	long qos_sample_count;
	s_matchmaking_quality_qos_sample qos_samples[400];
	long session_search_count;
	s_matchmaking_session_search session_searches[16];
	bool live_service_qos_result_valid;
	s_transport_qos_result live_service_qos_result;
	bool nat_type_valid;
	long nat_type;
	bool primary_map_load_failure;
	bool secondary_map_load_failure;
};
static_assert(sizeof(s_matchmaking_quality_data) == 0x98F0);

struct c_matchmaking_quality
{
	bool m_needs_submission_to_webstats;
	s_matchmaking_quality_data m_data;
};
static_assert(sizeof(c_matchmaking_quality) == 0x98F8);

