#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_broadcast_search.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_security.hpp"

struct s_network_session_remote_session_join_data
{
	long join_state;
	long join_from;
	long join_to;
	long join_target;
	qword join_nonce;
	long platform;
	s_transport_secure_identifier session_id;
	s_transport_secure_key session_key;
	s_transport_secure_address host_secure_address;
	long session_class;
	long join_result;

	bool join_to_public_slots;
	byte : 8;
	byte : 8;
	byte : 8;
};
static_assert(sizeof(s_network_session_remote_session_join_data) == 0x58);

struct s_network_session_join_request_payload
{
	long payload_type;
	union
	{
		s_matchmaking_gather_party_properties gather_party_properties;
		s_matchmaking_search_party_properties search_party_properties;
	};
};
static_assert(sizeof(s_network_session_join_request_payload) == 0xDC);

struct s_joining_peer
{
	s_transport_secure_address joining_peer_address;
	long joining_network_version_number;
	long user_player_indices[4];
};

struct s_queried_player_global_statistics
{
	bool valid;
	byte : 8;
	byte : 8;
	byte : 8;

	long experience_base;
	long experience_penalty;
	long highest_skill;
}; 

struct s_queried_player_hopper_statistics
{
	bool valid;
	byte : 8;
	word identifier;

	real mu;
	real sigma;
	long old_skill;
	long games_played;
	long games_completed;
	long games_won;
};

struct s_joining_player
{
	s_player_identifier joining_peer_player_id;
	qword joining_peer_player_xuid;
	bool joining_peer_is_silver_or_gold_live;
	bool joining_peer_is_online_enabled;
	bool joining_peer_is_user_created_content_allowed;
	bool joining_peer_is_friend_created_content_allowed;
	s_queried_player_global_statistics global_stats;
	s_queried_player_hopper_statistics hopper_stats;
};

struct s_network_session_join_request
{
	qword join_nonce;
	qword join_party_nonce;
	long joining_peer_count;
	s_joining_peer joining_peers[17];
	long joining_player_count;
	s_joining_player joining_players[16];

	bool join_to_public_slots;
	byte : 8;
	byte : 8;
	byte : 8;

	s_network_session_join_request_payload join_request_payload;
};

struct s_join_queue_entry
{
	transport_address address;
	byte __data[0x324];

	//qword join_nonce;
	//s_network_session_join_request join_request;
	//dword times[2];
	//long latency_maybe;
};
static_assert(sizeof(s_join_queue_entry) == 0x338);

#pragma pack(push, 4)
struct s_networking_join_data
{
	long local_join_state;
	long local_join_result;
	dword time;

	bool __unknownC;
	byte : 8;
	byte : 8;
	byte : 8;

	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;

	s_network_session_remote_session_join_data join_data;

	bool request_join_squad_to_group;
	bool request_join_squad_to_target_group;
	bool request_join_group_to_group;
	byte : 8;

	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;

	s_network_session_remote_session_join_data new_join_data;
	long join_group_result;
	s_network_session_join_request_payload join_request_payload;

	bool __unknown1AC;
	byte : 8;
	byte : 8;
	byte : 8;

	long join_target;
	long join_queue_mode;
	long join_queue_entry_count;
	long join_peer_count;

	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;

	s_join_queue_entry join_queue[32];
};
static_assert(sizeof(s_networking_join_data) == 0x68C4);
#pragma pack(pop)

extern s_networking_join_data& g_network_join_data;