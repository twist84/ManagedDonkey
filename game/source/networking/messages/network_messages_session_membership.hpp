#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_membership.hpp"
#include "text/unicode.hpp"

struct s_network_message_membership_update_peer_properties
{
	bool peer_name_updated;
	c_static_wchar_string<16> peer_name;
	c_static_wchar_string<32> peer_session_name;

	bool has_hard_drive;
	word ready_hopper_id;
	dword game_start_error;

	bool peer_map_id_updated;
	dword peer_map_id;

	bool peer_map_updated;
	long peer_map_status;
	dword peer_map_progress_percentage;

	bool peer_game_instance_updated;
	qword peer_game_instance;

	bool available_multiplayer_map_mask_updated;
	dword available_multiplayer_map_mask;

	bool peer_connection_updated;
	dword estimated_downstream_bandwidth_bps;
	dword estimated_upstream_bandwidth_bps;

	bool estimated_upstream_is_reliable;
	dword nat_type;
	dword connectivity_badness_rating;
	dword host_badness_rating;
	dword client_badness_rating;
	dword language;
	s_network_session_peer_connectivity connectivity;

	bool versions_updated;
	dword determinism_version;
	dword determinism_compatible_version;

	bool flags_updated;
	dword_flags flags;
};
static_assert(sizeof(s_network_message_membership_update_peer_properties) == 0xE0);

struct s_network_message_membership_update_peer
{
	dword peer_index;
	dword peer_update_type;
	dword peer_connection_state;

#pragma pack(push, 1)
	union
	{
		byte peer_info_bytes[0x20 - 0xC];
		struct
		{
			bool peer_info_updated;
			s_transport_secure_address peer_address;
		} peer_info;
	};
#pragma pack(pop)
	qword peer_party_nonce;
	qword peer_join_nonce;
	dword network_version_number;
	dword peer_creation_timestamp;

	bool peer_properties_updated;
	s_network_message_membership_update_peer_properties peer_properties;
};
static_assert(sizeof(s_network_message_membership_update_peer) == 0x120);

struct s_network_message_membership_update_player
{
	long player_index;
	long player_update_type;
	bool player_location_updated;
	s_player_identifier identifier;
	short : 16;
	short peer_index;
	long player_addition_number;
	bool player_occupies_a_public_slot;
	bool player_properties_updated;
	short peer_user_index;
	long player_update_number;
	long controller_index;
	long : 32;
	s_player_configuration player_data;
	dword player_voice;
};
static_assert(sizeof(s_network_message_membership_update_player) == 0x1650);

struct s_network_message_membership_update
{
	s_transport_secure_identifier session_id;
	long update_number;
	long incremental_update_number;
	dword baseline_checksum;

	short peer_update_count;
	short player_update_count;
	c_static_array<s_network_message_membership_update_peer, 17> peer;
	c_static_array<s_network_message_membership_update_player, 16> players;

	bool player_addition_number_updated;
	long player_addition_number;

	bool leader_updated;
	long leader_peer_index;

	bool host_updated;
	long host_peer_index;

	bool slot_counts_updated;
	long private_slot_count;
	long public_slot_count;
	bool friends_only;
	bool are_slots_locked;

	dword checksum;
	long : 32;
};
static_assert(sizeof(s_network_message_membership_update) == 0x17870);

struct s_network_message_peer_properties
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address secure_address;
	s_network_session_peer_properties peer_properties;
};
static_assert(sizeof(s_network_message_peer_properties) == 0xE0);

struct s_network_message_delegate_leadership
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address desired_leader_address;
};
static_assert(sizeof(s_network_message_delegate_leadership) == 0x20);

struct s_network_message_boot_machine
{
	s_transport_secure_identifier session_id;
	long reason; // e_network_session_boot_reason
	s_transport_secure_address boot_peer_address;
};
static_assert(sizeof(s_network_message_boot_machine) == 0x24);

struct s_network_message_player_add
{
	s_transport_secure_identifier session_id;
	s_player_identifier player_identifier;
	long user_index;
	long controller_index;
	s_player_configuration_from_client player_data;
	long player_voice;
};
static_assert(sizeof(s_network_message_player_add) == 0x54);

struct s_network_message_player_refuse
{
	s_transport_secure_identifier session_id;
	s_player_identifier player_identifier;
	long join_refusal_reason;
};
static_assert(sizeof(s_network_message_player_refuse) == 0x1C);

struct s_network_message_player_remove
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_player_remove) == 0x10);

struct s_network_message_player_properties
{
	s_transport_secure_identifier session_id;
	long player_update_number;
	long controller_index;
	s_player_configuration_from_client player_from_client;
	long player_voice;
};
static_assert(sizeof(s_network_message_player_properties) == 0x4C);

struct c_bitstream;

class c_network_message_membership_update
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_peer_properties
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_delegate_leadership
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_boot_machine
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_player_add
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_player_refuse
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_player_remove
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

class c_network_message_player_properties
{
public:
	static void __cdecl encode(c_bitstream* packet, long message_storage_size, void const* message_storage);
	static bool __cdecl decode(c_bitstream* packet, long message_storage_size, void* message_storage);
};

struct c_network_message_type_collection;
extern void __cdecl network_message_types_register_session_membership(c_network_message_type_collection* message_collection);

