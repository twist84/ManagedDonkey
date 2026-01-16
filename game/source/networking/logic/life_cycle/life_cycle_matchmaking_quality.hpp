#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "networking/logic/network_leaderboard.hpp"
#include "networking/session/network_managed_session.hpp"
#include "networking/transport/transport_qos.hpp"

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
COMPILE_ASSERT(sizeof(s_game_hopper_picked_game_collection) == 0x1B0);

struct s_matchmaking_quality_qos_sample
{
	byte __data0[0x20];
	s_transport_qos_result qos_result;
};
COMPILE_ASSERT(sizeof(s_matchmaking_quality_qos_sample) == 0x40);

struct s_matchmaking_session_search_status
{
	int32 session_count;
	uns32 start_timestamp;
	uns32 completed_timestamp;
	int32 failure_count;
	int32 retry_count;
	uns32 last_failure_time;
	int32 stage;
	int32 qos_desired_count;
	int32 probe_only_qos_completed_count;
	int32 full_qos_completed_count;
	int32 unsuitable_session_count;
	int32 unsuitable_reasons_count[12];
	int32 undesireable_session_count;
	int32 undesirable_reasons_count[2];
	int32 join_results[20];
	byte __data[0x8];
};
COMPILE_ASSERT(sizeof(s_matchmaking_session_search_status) == 0xC0);

enum e_matchmaking_search_stage;
enum e_language;
struct s_matchmaking_session_search_query
{
	e_matchmaking_search_stage stage;
	int32 stage_round;
	uns16 hopper_identifier;
	int32 min_skill_level;
	int32 max_skill_level;
	int32 party_size;
	bool mixed_skill_party;
	e_online_nat_type nat_type;
	int32 min_average_skill_level;
	int32 max_average_skill_level;
	real32 average_mu_min;
	real32 average_mu_max;
	int32 min_average_experience_rating;
	int32 max_average_experience_rating;
	int32 gamer_zone;
	int32 gamer_region;
	e_language language;
	int32 connection_threshold_ms;
	bool session_of_quitters;
	e_matchmaking_search_preference search_preference;
	int16 query_flags;
};
COMPILE_ASSERT(sizeof(s_matchmaking_session_search_query) == 0x54);

struct __declspec(align(8)) s_online_session_search_parameters
{
	int32 query;
	e_controller_index controller_index;
	int32 property_count;
	s_online_property properties[20];
	int32 context_count;
	s_online_context contexts[6];
};
COMPILE_ASSERT(sizeof(s_online_session_search_parameters) == 0x228);
COMPILE_ASSERT(0x000 == OFFSETOF(s_online_session_search_parameters, query));
COMPILE_ASSERT(0x004 == OFFSETOF(s_online_session_search_parameters, controller_index));
COMPILE_ASSERT(0x008 == OFFSETOF(s_online_session_search_parameters, property_count));
COMPILE_ASSERT(0x010 == OFFSETOF(s_online_session_search_parameters, properties));
COMPILE_ASSERT(0x1F0 == OFFSETOF(s_online_session_search_parameters, context_count));
COMPILE_ASSERT(0x1F4 == OFFSETOF(s_online_session_search_parameters, contexts));

struct s_matchmaking_session_search
{
	s_matchmaking_session_search_status status;
	s_matchmaking_session_search_query query;
	s_online_session_search_parameters online_query;
};
COMPILE_ASSERT(sizeof(s_matchmaking_session_search) == 0x340);
COMPILE_ASSERT(0x000 == OFFSETOF(s_matchmaking_session_search, status));
COMPILE_ASSERT(0x0C0 == OFFSETOF(s_matchmaking_session_search, query));
COMPILE_ASSERT(0x118 == OFFSETOF(s_matchmaking_session_search, online_query));

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
	int32 ping_msec;
	uns32 search_time;
	uns32 gather_time;
	uns32 arbitration_time;
	uns32 host_selection_time;
	uns32 prepare_map_time;
	uns32 in_match_time;
	uns8 local_address[32];
	int32 qos_sample_count;
	s_matchmaking_quality_qos_sample qos_samples[400];
	int32 session_search_count;
	s_matchmaking_session_search session_searches[16];
	bool live_service_qos_result_valid;
	s_transport_qos_result live_service_qos_result;
	bool nat_type_valid;
	int32 nat_type;
	bool primary_map_load_failure;
	bool secondary_map_load_failure;
};
COMPILE_ASSERT(sizeof(s_matchmaking_quality_data) == 0x98F0);

class c_matchmaking_quality
{
public:
	c_matchmaking_quality();

	bool m_needs_submission_to_webstats;
	s_matchmaking_quality_data m_data;
};
COMPILE_ASSERT(sizeof(c_matchmaking_quality) == 0x98F8);

