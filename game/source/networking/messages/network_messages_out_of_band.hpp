#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"
#include "networking/transport/transport_security.hpp"
#include "networking/network_utilities.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

struct s_network_message_ping
{
	word id;
	dword timestamp;
	bool request_qos;
};
static_assert(sizeof(s_network_message_ping) == 0xC);

struct s_network_message_pong
{
	word id;
	dword timestamp;
	dword qos_response;
};
static_assert(sizeof(s_network_message_pong) == 0xC);

struct s_network_message_broadcast_search
{
	word protocol_version;
	qword nonce;
};
static_assert(sizeof(s_network_message_broadcast_search) == 0x10);

struct s_network_message_directed_search : s_network_message_broadcast_search
{
};
static_assert(sizeof(s_network_message_directed_search) == sizeof(s_network_message_broadcast_search));

#pragma pack(push, 1)
struct s_network_session_status_data
{
	struct s_header
	{
		short protocol;
		short platform;
		long executable_type;
		long executable_version;
		long compatible_version;
	};
	static_assert(sizeof(s_header) == 0x10);

	s_header header;
	c_enum<e_network_session_mode, long, _network_session_mode_none, k_network_session_mode_count> session_mode;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;
	c_enum<e_network_session_class, long, _network_session_class_offline, k_network_session_class_count> session_class;
	c_enum<e_network_session_type, long, _network_session_type_none, k_network_session_type_count> session_type;
	long privacy_mode;
	c_static_wchar_string<32> session_name;
	c_enum<e_life_cycle_state, short, _life_cycle_state_none, k_life_cycle_state_count> life_cycle_state;
	s_transport_secure_identifier session_id;
	s_transport_secure_address host_address;
	s_transport_secure_key key;
	short public_slots;
	short private_slots;
	short public_players;
	short private_players;
	c_enum<e_session_game_mode, short, _session_game_mode_none, k_session_game_mode_count> game_mode;
	short game_state;
	c_enum<e_network_game_type, short, _network_game_type_none, k_network_game_type_count> network_game_type;
	short connection_quality;
	c_static_string<16> playlist_name;
	byte : 8;
	byte : 8;
	long game_engine_type;
	c_static_wchar_string<16> game_engine_variant_name;
	long campaign_id;
	long campaign_difficulty_level;
	long map_id;
	short insertion_point;
	c_static_wchar_string<16> map_variant_name;
	c_static_wchar_string<16> saved_film_name;
	bool game_has_teams;
	byte : 8;
	short maximum_team_count;
	short score_to_win_round;
	short best_player_score;
	short player_count;
	long __unknown134;
	c_static_array<s_network_session_status_data_player, 16> players;
	long player_encoded_count;
	long game_timer_type;
	long game_timer_seconds;
	dword team_mask;
	c_static_array<long, 8> team_scores;
	s_matchmaking_gather_party_properties gather_party_properties;
	byte : 8;
	byte : 8;
	byte : 8;
	byte : 8;

	void update_host_player_identifier(transport_address const* address)
	{
		players[0].identifier.ip_addr = address->ipv4_address;
		players[0].identifier.port = address->port;
		players[0].identifier.flags = address->address_length;
	}
};
static_assert(sizeof(s_network_session_status_data) == 0x164C0);
#pragma pack(pop)

struct s_network_message_broadcast_reply
{
	word protocol_version;
	qword search_nonce;
	s_network_session_status_data status_data;
};
static_assert(sizeof(s_network_message_broadcast_reply) == 0x164D0);

struct c_bitstream;

class c_network_message_broadcast_search
{
public:
	static void encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_directed_search
{
public:
	static void encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};


