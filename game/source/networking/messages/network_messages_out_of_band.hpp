#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/logic/logic_matchmaking_desirability.hpp"
#include "networking/transport/transport_security.hpp"
#include "networking/network_utilities.hpp"
#include "text/unicode.hpp"

enum e_network_qos_response
{
	_network_qos_response_deny = 0,
	_network_qos_response_delay,
	_network_qos_response_accept,

	k_network_qos_response_count,
	k_network_qos_response_bits = 2,
};

struct s_network_message_ping
{
	uns16 unique_identifier;
	uns32 origin_timestamp;
	bool request_permission_to_qos;
};
COMPILE_ASSERT(sizeof(s_network_message_ping) == 0xC);

struct s_network_message_pong
{
	uns16 unique_identifier;
	uns32 origin_timestamp;
	e_network_qos_response qos_response;
};
COMPILE_ASSERT(sizeof(s_network_message_pong) == 0xC);

struct s_network_message_broadcast_search
{
	uns16 protocol_version;
	uns64 nonce;
};
COMPILE_ASSERT(sizeof(s_network_message_broadcast_search) == 0x10);

struct s_network_message_directed_search :
	s_network_message_broadcast_search
{
};
COMPILE_ASSERT(sizeof(s_network_message_directed_search) == sizeof(s_network_message_broadcast_search));

struct s_player_information
{
	s_player_identifier identifier;
	s_player_configuration configuration_data;
};
COMPILE_ASSERT(sizeof(s_player_information) == 0x1628);

struct s_network_squad_status_data_game_details
{
	e_network_session_class session_class;
	e_network_session_type session_type;
	int32 session_privacy_mode; // e_network_game_privacy
	wchar_t session_name[32];
	int16 life_cycle_state;
	s_transport_session_description description;
	int16 open_public_slot_count;
	int16 open_private_slot_count;
	int16 current_public_players;
	int16 current_private_players;
	int16 game_mode; // e_gui_game_mode
	int16 game_state; // e_session_game_mode
	int16 network_game_type; // e_network_game_type
	int16 connection_quality;
	char playlist_name[16];
	e_game_engine_type game_engine_type;
	wchar_t game_engine_variant_name[16];
	e_campaign_id campaign_id;
	e_campaign_difficulty_level campaign_difficulty_level;
	e_map_id map_id;
	int16 insertion_point;
	wchar_t map_variant_name[16];
	wchar_t saved_film_name[16];
	bool game_has_teams;
	int16 game_team_count;
	int16 target_score;
	int16 current_score;
	int16 player_count;
	int32 : 32;
	s_player_information players[16];
	int32 player_encoded_count;
	int32 game_timer_type;
	int32 game_timer_seconds;
	uns32 valid_team_mask;
	int32 team_scores[8];
};
COMPILE_ASSERT(sizeof(s_network_squad_status_data_game_details) == 0x163D0);
COMPILE_ASSERT(0x00000 == OFFSETOF(s_network_squad_status_data_game_details, session_class));
COMPILE_ASSERT(0x00004 == OFFSETOF(s_network_squad_status_data_game_details, session_type));
COMPILE_ASSERT(0x00008 == OFFSETOF(s_network_squad_status_data_game_details, session_privacy_mode));
COMPILE_ASSERT(0x0000C == OFFSETOF(s_network_squad_status_data_game_details, session_name));
COMPILE_ASSERT(0x0004C == OFFSETOF(s_network_squad_status_data_game_details, life_cycle_state));
COMPILE_ASSERT(0x0004E == OFFSETOF(s_network_squad_status_data_game_details, description));
COMPILE_ASSERT(0x0007E == OFFSETOF(s_network_squad_status_data_game_details, open_public_slot_count));
COMPILE_ASSERT(0x00080 == OFFSETOF(s_network_squad_status_data_game_details, open_private_slot_count));
COMPILE_ASSERT(0x00082 == OFFSETOF(s_network_squad_status_data_game_details, current_public_players));
COMPILE_ASSERT(0x00084 == OFFSETOF(s_network_squad_status_data_game_details, current_private_players));
COMPILE_ASSERT(0x00086 == OFFSETOF(s_network_squad_status_data_game_details, game_mode));
COMPILE_ASSERT(0x00088 == OFFSETOF(s_network_squad_status_data_game_details, game_state));
COMPILE_ASSERT(0x0008A == OFFSETOF(s_network_squad_status_data_game_details, network_game_type));
COMPILE_ASSERT(0x0008C == OFFSETOF(s_network_squad_status_data_game_details, connection_quality));
COMPILE_ASSERT(0x0008E == OFFSETOF(s_network_squad_status_data_game_details, playlist_name));
COMPILE_ASSERT(0x000A0 == OFFSETOF(s_network_squad_status_data_game_details, game_engine_type));
COMPILE_ASSERT(0x000A4 == OFFSETOF(s_network_squad_status_data_game_details, game_engine_variant_name));
COMPILE_ASSERT(0x000C4 == OFFSETOF(s_network_squad_status_data_game_details, campaign_id));
COMPILE_ASSERT(0x000C8 == OFFSETOF(s_network_squad_status_data_game_details, campaign_difficulty_level));
COMPILE_ASSERT(0x000CC == OFFSETOF(s_network_squad_status_data_game_details, map_id));
COMPILE_ASSERT(0x000D0 == OFFSETOF(s_network_squad_status_data_game_details, insertion_point));
COMPILE_ASSERT(0x000D2 == OFFSETOF(s_network_squad_status_data_game_details, map_variant_name));
COMPILE_ASSERT(0x000F2 == OFFSETOF(s_network_squad_status_data_game_details, saved_film_name));
COMPILE_ASSERT(0x00112 == OFFSETOF(s_network_squad_status_data_game_details, game_has_teams));
COMPILE_ASSERT(0x00114 == OFFSETOF(s_network_squad_status_data_game_details, game_team_count));
COMPILE_ASSERT(0x00116 == OFFSETOF(s_network_squad_status_data_game_details, target_score));
COMPILE_ASSERT(0x00118 == OFFSETOF(s_network_squad_status_data_game_details, current_score));
COMPILE_ASSERT(0x0011A == OFFSETOF(s_network_squad_status_data_game_details, player_count));
COMPILE_ASSERT(0x00120 == OFFSETOF(s_network_squad_status_data_game_details, players));
COMPILE_ASSERT(0x163A0 == OFFSETOF(s_network_squad_status_data_game_details, player_encoded_count));
COMPILE_ASSERT(0x163A4 == OFFSETOF(s_network_squad_status_data_game_details, game_timer_type));
COMPILE_ASSERT(0x163A8 == OFFSETOF(s_network_squad_status_data_game_details, game_timer_seconds));
COMPILE_ASSERT(0x163AC == OFFSETOF(s_network_squad_status_data_game_details, valid_team_mask));
COMPILE_ASSERT(0x163B0 == OFFSETOF(s_network_squad_status_data_game_details, team_scores));

struct s_network_squad_status_data_matchmaking
{
	s_matchmaking_gather_party_properties gather_party_properties;
};
COMPILE_ASSERT(sizeof(s_network_squad_status_data_matchmaking) == 0xD4);

struct s_network_squad_status_data
{
	struct s_header
	{
		int16 network_protocol_version;
		int16 network_platform;
		int32 executable_type;
		int32 executable_version;
		int32 compatible_version;
	};
	COMPILE_ASSERT(sizeof(s_header) == 0x10);

	s_header header;
	e_network_session_qos_status_data_type session_mode;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
	s_network_squad_status_data_game_details game_details;
	s_network_squad_status_data_matchmaking matchmaking;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;
	uns8 : 8;

	void update_host_player_identifier(const transport_address* address)
	{
		game_details.players[0].identifier = address;
	}
};
COMPILE_ASSERT(sizeof(s_network_squad_status_data) == 0x164C0);
COMPILE_ASSERT(0x00000 == OFFSETOF(s_network_squad_status_data, header));
COMPILE_ASSERT(0x00010 == OFFSETOF(s_network_squad_status_data, session_mode));
COMPILE_ASSERT(0x00018 == OFFSETOF(s_network_squad_status_data, game_details));
COMPILE_ASSERT(0x163E8 == OFFSETOF(s_network_squad_status_data, matchmaking));

struct s_network_message_broadcast_reply
{
	uns16 protocol_version;
	uns64 search_nonce;
	s_network_squad_status_data status_data;
};
COMPILE_ASSERT(sizeof(s_network_message_broadcast_reply) == 0x164D0);

class c_bitstream;

class c_network_message_ping
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_pong
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_broadcast_search
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_directed_search
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_broadcast_reply
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_type_collection;
extern void __cdecl network_message_types_register_out_of_band(c_network_message_type_collection* message_collection);

