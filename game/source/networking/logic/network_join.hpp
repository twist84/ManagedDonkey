#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_broadcast_search.hpp"
#include "networking/session/network_session_parameters_session.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_security.hpp"

enum e_network_join_squad_join_source;
class c_network_session;
class c_network_session_membership;

struct s_group_session_join_request_payload
{
	s_matchmaking_search_party_properties search_party_properties;
};
COMPILE_ASSERT(sizeof(s_group_session_join_request_payload) == 0xD8);

struct s_squad_session_join_request_payload
{
	int32 dummy;
};
COMPILE_ASSERT(sizeof(s_squad_session_join_request_payload) == 0x4);

struct s_network_session_join_request_payload
{
	e_network_session_type payload_type;
	union
	{
		s_group_session_join_request_payload group_payload;
		s_squad_session_join_request_payload squad_payload;
	};
};
COMPILE_ASSERT(sizeof(s_network_session_join_request_payload) == 0xDC);
COMPILE_ASSERT(0x0 == OFFSETOF(s_network_session_join_request_payload, payload_type));
COMPILE_ASSERT(0x4 == OFFSETOF(s_network_session_join_request_payload, group_payload));
COMPILE_ASSERT(0x4 == OFFSETOF(s_network_session_join_request_payload, squad_payload));

struct s_network_session_join_request_peer
{
	s_transport_secure_address joining_peer_address;
	int32 joining_network_version_number;
	int32 user_player_index;
};
COMPILE_ASSERT(sizeof(s_network_session_join_request_peer) == 0x18);

struct s_network_session_join_request_player
{
	s_player_identifier joining_peer_player_id;
};
COMPILE_ASSERT(sizeof(s_network_session_join_request_player) == sizeof(s_player_identifier));

struct s_network_session_join_request
{
	uns64 join_nonce;
	uns64 party_nonce;
	int32 joining_peer_count;
	s_network_session_join_request_peer joining_peers[17];
	int32 joining_player_count;
	s_network_session_join_request_player joining_players[16];
	bool join_to_public_slots;
	s_network_session_join_request_payload payload;
};
COMPILE_ASSERT(sizeof(s_network_session_join_request) == 0x310);
COMPILE_ASSERT(0x000 == OFFSETOF(s_network_session_join_request, join_nonce));
COMPILE_ASSERT(0x008 == OFFSETOF(s_network_session_join_request, party_nonce));
COMPILE_ASSERT(0x010 == OFFSETOF(s_network_session_join_request, joining_peer_count));
COMPILE_ASSERT(0x014 == OFFSETOF(s_network_session_join_request, joining_peers));
COMPILE_ASSERT(0x1AC == OFFSETOF(s_network_session_join_request, joining_player_count));
COMPILE_ASSERT(0x1B0 == OFFSETOF(s_network_session_join_request, joining_players));
COMPILE_ASSERT(0x230 == OFFSETOF(s_network_session_join_request, join_to_public_slots));
COMPILE_ASSERT(0x234 == OFFSETOF(s_network_session_join_request, payload));

struct s_networking_join_queue_entry
{
	transport_address address;
	s_network_session_join_request join_request;
	uns32 times[2];
	int32 session_desirability;
};
COMPILE_ASSERT(sizeof(s_networking_join_queue_entry) == 0x338);

struct s_networking_join_data
{
	bool disable_joins;
	e_networking_join_local_state local_join_state;
	e_life_cycle_join_result local_join_result;
	uns32 timeout_start_time;
	bool client_already_joining;
	__declspec(align(8)) s_network_session_remote_session_join_data queued_squad_join;
	bool request_join_squad_to_group;
	bool request_join_squad_to_target_group;
	bool request_join_group_to_group;
	__declspec(align(8)) s_network_session_remote_session_join_data requested_group_join;
	e_life_cycle_join_result group_join_result;
	s_network_session_join_request_payload requested_payload;
	bool request_leave_group;
	e_networking_join_destination_squad requested_group_to_leave;
	e_network_join_queue_mode join_queue_mode;
	int32 join_queue_entry_count;
	int32 number_of_peers_expected_in_membership_at_last_desiribility_calculation;
	s_networking_join_queue_entry join_queue[32];
};
COMPILE_ASSERT(sizeof(s_networking_join_data) == 0x68C8);

extern s_networking_join_data& g_network_join_data;

//extern void __cdecl network_join_add_join_to_queue(c_network_session*, const transport_address*, const s_network_session_join_request*);
//extern void __cdecl network_join_calculate_full_queue_desirability(c_network_session_membership* membership);
//extern void __cdecl network_join_clear_group_join_result();
//extern void __cdecl network_join_clear_squad_join_result();
//extern void __cdecl network_join_disable_outgoing_joins(bool);
//extern void __cdecl network_join_flush_join_queue();
//extern void __cdecl network_join_flush_our_join_from_join_queue(const s_transport_secure_address*, uns64);
//extern e_life_cycle_join_result __cdecl network_join_get_group_join_result();
//extern e_network_join_queue_mode __cdecl network_join_get_join_queue_mode();
//extern e_life_cycle_join_result __cdecl network_join_get_squad_join_result();
//extern void __cdecl network_join_group_to_remote_group(const s_transport_secure_identifier*, const s_transport_secure_key*, const s_transport_secure_address*, const s_group_session_join_request_payload*);
//extern bool __cdecl network_join_is_any_join_in_progress();
//extern bool __cdecl network_join_is_any_join_in_progress_or_join_queue_active();
//extern bool __cdecl network_join_is_join_to_squad_in_progress();
//extern bool __cdecl network_join_is_leaving_group();
//extern void __cdecl network_join_leave_group_session_and_swap_if_necessary();
//extern void __cdecl network_join_local_squad_to_local_group();
//extern void __cdecl network_join_local_squad_to_remote_group(const s_transport_secure_identifier*, const s_transport_secure_key*, const s_transport_secure_address*, const s_group_session_join_request_payload*);
//extern void __cdecl network_join_local_squad_to_remote_target_group(const s_transport_secure_identifier*, const s_transport_secure_key*, const s_transport_secure_address*, const s_group_session_join_request_payload*);
//extern e_life_cycle_join_result __cdecl network_join_map_refuse_reason_to_join_result(e_network_join_refuse_reason, bool, const c_network_session*);
//extern int32 __cdecl network_join_number_of_joins_in_queue();
//extern bool __cdecl network_join_process_joins_from_queue()	;
//extern void __cdecl network_join_queue_update();
//extern void __cdecl network_join_remove_join_from_queue(uns64);
//extern void __cdecl network_join_remove_join_from_queue_internal(s_networking_join_queue_entry*);
extern void __cdecl network_join_set_join_queue_mode(e_network_join_queue_mode join_queue_mode);
//extern void __cdecl network_join_squad_join_abort();
//extern bool __cdecl network_join_to_remote_squad(e_network_join_squad_join_source, bool, bool, bool, e_network_session_class, const s_transport_secure_identifier*, const s_transport_secure_key*, const s_transport_secure_address*);
extern void __cdecl network_join_update();

