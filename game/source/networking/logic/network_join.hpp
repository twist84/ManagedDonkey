#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/network_broadcast_search.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_security.hpp"

enum e_network_join_queue_mode
{
	_network_join_closed_to_all_joins = 0,
	_network_join_open_to_join_squad,
	_network_join_queue_joins_to_group,
	_network_join_process_queued_group_joins,
	_network_join_queue_closed_while_in_match,

	k_network_join_queue_mode_count
};

enum e_join_local_state
{
	_join_local_state_none = 0,
	_join_local_state_start_join_squad,
	_join_local_state_joining_squad,
	_join_local_state_leaving_old_squad,
	_join_local_state_processing_join_to_us,

	k_join_local_state_count
};

enum e_join_remote_state
{
	_join_remote_state_none = 0,
	_join_remote_state_party_start_join,
	_join_remote_state_party_join_host,
	_join_remote_state_party_join_clients,
	_join_remote_state_party_leave_old_squad,
	_join_remote_state_party_join_complete,
	_join_remote_state_leave_group,

	k_join_remote_state_count
};

enum e_join_type
{
	k_join_type_count = 2
};

enum e_networking_join_destination_squad
{
	k_join_destination_count = 1
};

enum e_transport_platform
{
	k_transport_platform_count = 2
};

enum e_life_cycle_join_result
{
	k_life_cycle_join_result_count = 22
};

enum e_network_session_class
{
	_network_session_class_none = 0,
	_network_session_class_offline,
	_network_session_class_system_link,
	_network_session_class_xbox_live,

	k_network_session_class_count
};

struct s_network_session_remote_session_join_data
{
	c_enum<e_join_remote_state, long, k_join_remote_state_count> join_state;
	c_enum<e_join_type, long, k_join_type_count> join_from;
	c_enum<e_join_type, long, k_join_type_count> join_to;
	c_enum<e_networking_join_destination_squad, long, k_join_destination_count> join_target;
	qword join_nonce;
	c_enum<e_transport_platform, long, k_transport_platform_count> platform;
	s_transport_secure_identifier session_id;
	s_transport_secure_key session_key;
	s_transport_secure_address host_secure_address;
	c_enum<e_network_session_class, long, k_network_session_class_count> session_class;
	c_enum<e_life_cycle_join_result, long, k_life_cycle_join_result_count> join_result;
	bool join_to_public_slots;
};
static_assert(sizeof(s_network_session_remote_session_join_data) == 0x58);

struct s_group_session_join_request_payload
{
	long payload_type;
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
	long joining_network_version_number;
	long user_player_index;
};
static_assert(sizeof(s_joining_peer) == 0x18);

struct s_joining_player
{
	s_player_identifier joining_peer_player_id;
};
static_assert(sizeof(s_joining_player) == sizeof(s_player_identifier));

struct s_network_session_join_request
{
	qword join_nonce;
	qword join_party_nonce;

	long joining_peer_count;
	s_joining_peer joining_peers[17];

	long joining_player_count;
	s_joining_player joining_players[16];

	bool join_to_public_slots;

	s_group_session_join_request_payload join_request_payload;
};
static_assert(sizeof(s_network_session_join_request) == 0x310);

struct s_networking_join_queue_entry
{
	transport_address address;

	dword __unknown14;

	s_network_session_join_request join_request;
	dword times[2];
	long session_desirability;

	dword __unknown334;
};
static_assert(sizeof(s_networking_join_queue_entry) == 0x338);

struct s_networking_join_data
{
	bool disable_outgoing_joins;
	c_enum<e_join_local_state, long, k_join_local_state_count> local_join_state;
	c_enum<e_life_cycle_join_result, long, k_life_cycle_join_result_count> local_join_result;
	dword time;

	// network_join_update
	bool __unknown10;

	s_network_session_remote_session_join_data join_data;
	bool request_join_squad_to_group;
	bool request_join_squad_to_target_group;
	bool request_join_group_to_group;
	s_network_session_remote_session_join_data new_join_data;
	long join_group_result;
	s_group_session_join_request_payload join_request_payload;

	// network_join_update
	// network_join_leave_group_session_and_swap_if_necessary
	bool __unknown1B0;

	c_enum<e_networking_join_destination_squad, long, k_join_destination_count> join_target;
	c_enum<e_network_join_queue_mode, long, k_network_join_queue_mode_count> join_queue_mode;
	long join_queue_entry_count;
	long join_peer_count;
	c_static_array<s_networking_join_queue_entry, 32> join_queue;
};
static_assert(sizeof(s_networking_join_data) == 0x68C8);

extern s_networking_join_data& g_network_join_data;

struct c_network_session;
struct c_network_session_membership;
enum e_network_join_squad_join_source;
enum e_network_join_refuse_reason;

//0x0047DC70;	void __cdecl network_join_add_join_to_queue(c_network_session*, transport_address const*, s_network_session_join_request const*);
//0x0047DF10;	void __cdecl network_join_calculate_full_queue_desirability(c_network_session_membership* membership);
//0x0047E040;	void __cdecl network_join_clear_group_join_result();
//0x0047E050;	void __cdecl network_join_clear_squad_join_result();
//0x0047E0D0;	void __cdecl network_join_disable_outgoing_joins(bool);
//0x0047E0E0;	void __cdecl network_join_flush_join_queue();
//0x0047E160;	void __cdecl network_join_flush_our_join_from_join_queue(s_transport_secure_address const*, qword);
//0x0047E270;	e_life_cycle_join_result __cdecl network_join_get_group_join_result();
//0x0047E280;	e_network_join_queue_mode __cdecl network_join_get_join_queue_mode();
//0x0047E290;	e_life_cycle_join_result __cdecl network_join_get_squad_join_result();
//0x0047E340;	void __cdecl network_join_group_to_remote_group(s_transport_secure_identifier const*, s_transport_secure_key const*, s_transport_secure_address const*, s_group_session_join_request_payload const*);
//0x0047E400;	bool __cdecl network_join_is_any_join_in_progress();
//0x0047E460;	bool __cdecl network_join_is_any_join_in_progress_or_join_queue_active();
//0x0047E490;	bool __cdecl network_join_is_join_to_squad_in_progress();
//0x0047E4B0;	bool __cdecl network_join_is_leaving_group();
//0x0047E510;	void __cdecl network_join_leave_group_session_and_swap_if_necessary();
//0x0047E540;	void __cdecl network_join_local_squad_to_local_group();
//0x0047E580;	void __cdecl network_join_local_squad_to_remote_group(s_transport_secure_identifier const*, s_transport_secure_key const*, s_transport_secure_address const*, s_group_session_join_request_payload const*);
//0x0047E640;	void __cdecl network_join_local_squad_to_remote_target_group(s_transport_secure_identifier const*, s_transport_secure_key const*, s_transport_secure_address const*, s_group_session_join_request_payload const*);
//0x0047E700;	e_life_cycle_join_result __cdecl network_join_map_refuse_reason_to_join_result(e_network_join_refuse_reason, bool, c_network_session const*);
//0x0047E8A0;	long __cdecl network_join_number_of_joins_in_queue();
//0x0047E8B0;	bool __cdecl network_join_process_joins_from_queue()	;
//0x0047EA50;	void __cdecl network_join_queue_update();
//0x0047EBB0;	void __cdecl network_join_remove_join_from_queue(qword);
//0x0047EC40;	void __cdecl network_join_remove_join_from_queue_internal(s_networking_join_queue_entry*);
//0x0047EC70;	void __cdecl network_join_set_join_queue_mode(e_network_join_queue_mode);
//0x0047ECD0;	void __cdecl network_join_squad_join_abort();
//0x0047ECF0;	bool __cdecl network_join_to_remote_squad(e_network_join_squad_join_source, bool, bool, bool, e_network_session_class, s_transport_secure_identifier const*, s_transport_secure_key const*, s_transport_secure_address const*);
//0x0047EEA0;	void __cdecl network_join_update();

