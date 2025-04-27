#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_broadcast_search.hpp"
#include "networking/session/network_session_parameters_session.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_security.hpp"

enum e_network_join_squad_join_source;
struct c_network_session;
struct c_network_session_membership;

struct s_group_session_join_request_payload
{
	int32 payload_type;
	union
	{
		s_matchmaking_gather_party_properties gather_party_properties;
		s_matchmaking_search_party_properties search_party_properties;
	};
};
static_assert(sizeof(s_group_session_join_request_payload) == 0xDC);

struct s_joining_peer
{
	s_transport_secure_address joining_peer_address;
	int32 joining_network_version_number;
	int32 user_player_index;
};
static_assert(sizeof(s_joining_peer) == 0x18);

struct s_joining_player
{
	s_player_identifier joining_peer_player_id;
};
static_assert(sizeof(s_joining_player) == sizeof(s_player_identifier));

struct s_network_session_join_request
{
	uint64 join_nonce;
	uint64 join_party_nonce;
	int32 joining_peer_count;
	s_joining_peer joining_peers[17];
	int32 joining_player_count;
	s_joining_player joining_players[16];
	bool join_to_public_slots;
	s_group_session_join_request_payload join_request_payload;
};
static_assert(sizeof(s_network_session_join_request) == 0x310);

struct s_networking_join_queue_entry
{
	transport_address address;
	s_network_session_join_request join_request;
	uint32 times[2];
	int32 session_desirability;
};
static_assert(sizeof(s_networking_join_queue_entry) == 0x338);

struct s_networking_join_data
{
	bool disable_outgoing_joins;
	c_enum<e_join_local_state, int32, _join_local_state_none, k_join_local_state_count> local_join_state;
	c_enum<e_life_cycle_join_result, int32, _life_cycle_join_result_none, k_life_cycle_join_result_count> local_join_result;
	uint32 time;

	// network_join_update
	bool __unknown10;

	s_network_session_remote_session_join_data join_data;
	bool request_join_squad_to_group;
	bool request_join_squad_to_target_group;
	bool request_join_group_to_group;
	s_network_session_remote_session_join_data new_join_data;
	int32 join_group_result;
	s_group_session_join_request_payload join_request_payload;

	// network_join_update
	// network_join_leave_group_session_and_swap_if_necessary
	bool __unknown1B0;

	c_enum<e_networking_join_destination_squad, int32, _join_destination_target, k_join_destination_count> join_target;
	c_enum<e_network_join_queue_mode, int32, _network_join_closed_to_all_joins, k_network_join_queue_mode_count> join_queue_mode;
	int32 join_queue_entry_count;
	int32 join_peer_count;
	c_static_array<s_networking_join_queue_entry, 32> join_queue;
};
static_assert(sizeof(s_networking_join_data) == 0x68C8);

extern s_networking_join_data& g_network_join_data;

//extern void __cdecl network_join_add_join_to_queue(c_network_session*, transport_address const*, s_network_session_join_request const*);
//extern void __cdecl network_join_calculate_full_queue_desirability(c_network_session_membership* membership);
//extern void __cdecl network_join_clear_group_join_result();
//extern void __cdecl network_join_clear_squad_join_result();
//extern void __cdecl network_join_disable_outgoing_joins(bool);
//extern void __cdecl network_join_flush_join_queue();
//extern void __cdecl network_join_flush_our_join_from_join_queue(s_transport_secure_address const*, uint64);
//extern e_life_cycle_join_result __cdecl network_join_get_group_join_result();
//extern e_network_join_queue_mode __cdecl network_join_get_join_queue_mode();
//extern e_life_cycle_join_result __cdecl network_join_get_squad_join_result();
//extern void __cdecl network_join_group_to_remote_group(s_transport_secure_identifier const*, s_transport_secure_key const*, s_transport_secure_address const*, s_group_session_join_request_payload const*);
//extern bool __cdecl network_join_is_any_join_in_progress();
//extern bool __cdecl network_join_is_any_join_in_progress_or_join_queue_active();
//extern bool __cdecl network_join_is_join_to_squad_in_progress();
//extern bool __cdecl network_join_is_leaving_group();
//extern void __cdecl network_join_leave_group_session_and_swap_if_necessary();
//extern void __cdecl network_join_local_squad_to_local_group();
//extern void __cdecl network_join_local_squad_to_remote_group(s_transport_secure_identifier const*, s_transport_secure_key const*, s_transport_secure_address const*, s_group_session_join_request_payload const*);
//extern void __cdecl network_join_local_squad_to_remote_target_group(s_transport_secure_identifier const*, s_transport_secure_key const*, s_transport_secure_address const*, s_group_session_join_request_payload const*);
//extern e_life_cycle_join_result __cdecl network_join_map_refuse_reason_to_join_result(e_network_join_refuse_reason, bool, c_network_session const*);
//extern int32 __cdecl network_join_number_of_joins_in_queue();
//extern bool __cdecl network_join_process_joins_from_queue()	;
//extern void __cdecl network_join_queue_update();
//extern void __cdecl network_join_remove_join_from_queue(uint64);
//extern void __cdecl network_join_remove_join_from_queue_internal(s_networking_join_queue_entry*);
extern void __cdecl network_join_set_join_queue_mode(e_network_join_queue_mode join_queue_mode);
//extern void __cdecl network_join_squad_join_abort();
//extern bool __cdecl network_join_to_remote_squad(e_network_join_squad_join_source, bool, bool, bool, e_network_session_class, s_transport_secure_identifier const*, s_transport_secure_key const*, s_transport_secure_address const*);
extern void __cdecl network_join_update();

