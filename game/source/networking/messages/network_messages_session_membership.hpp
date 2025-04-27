#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/session/network_session_membership.hpp"
#include "text/unicode.hpp"

struct s_network_message_membership_update_peer_properties
{
	bool peer_name_updated;
	c_static_wchar_string<16> peer_name;
	c_static_wchar_string<32> peer_session_name;

	bool has_hard_drive;
	uint16 ready_hopper_id;
	uint32 game_start_error;

	bool peer_map_id_updated;
	uint32 peer_map_id;

	bool peer_map_updated;
	long peer_map_status;
	uint32 peer_map_progress_percentage;

	bool peer_game_instance_updated;
	uint64 peer_game_instance;

	bool available_multiplayer_map_mask_updated;
	uint32 available_multiplayer_map_mask;

	bool peer_connection_updated;
	uint32 estimated_downstream_bandwidth_bps;
	uint32 estimated_upstream_bandwidth_bps;

	bool estimated_upstream_is_reliable;
	uint32 nat_type;
	uint32 connectivity_badness_rating;
	uint32 host_badness_rating;
	uint32 client_badness_rating;
	uint32 language;
	s_network_session_peer_connectivity connectivity;

	bool versions_updated;
	uint32 determinism_version;
	uint32 determinism_compatible_version;

	bool flags_updated;
	uint32 flags;
};
static_assert(sizeof(s_network_message_membership_update_peer_properties) == 0xE0);

struct s_network_message_membership_update_peer
{
	uint32 peer_index;
	uint32 peer_update_type;
	uint32 peer_connection_state;

#pragma pack(push, 1)
	union
	{
		uint8 peer_info_bytes[0x20 - 0xC];
		struct
		{
			bool peer_info_updated;
			s_transport_secure_address peer_address;
		} peer_info;
	};
#pragma pack(pop)
	uint64 peer_party_nonce;
	uint64 peer_join_nonce;
	uint32 network_version_number;
	uint32 peer_creation_timestamp;

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
	uint32 player_voice;
};
static_assert(sizeof(s_network_message_membership_update_player) == 0x1650);

struct s_network_message_membership_update
{
	s_transport_secure_identifier session_id;
	long update_number;
	long incremental_update_number;
	uint32 baseline_checksum;

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

	uint32 checksum;
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

struct s_player_configuration_from_host_patial
{
	c_static_wchar_string<5> service_tag;
	c_flags<e_bungienet_user, uint8, k_bungienet_user_count> bungienet_user;
	c_static_array<rgb_color, k_color_type_count> colors;
	c_static_array<uint8, k_armor_type_count> armors;
	c_static_array<char, 4> consumables;
};
static_assert(sizeof(s_player_configuration_from_host_patial) == 0x30);

struct s_player_configuration_for_player_properties
{
	s_player_configuration_from_client client;
	s_player_configuration_from_host_patial host_partial;
};
static_assert(sizeof(s_player_configuration_for_player_properties) == 0x60);

struct s_network_message_player_properties
{
	s_transport_secure_identifier session_id;
	long player_update_number;
	long controller_index;
	s_player_configuration_for_player_properties player_data;
	long player_voice;
};
static_assert(sizeof(s_network_message_player_properties) == 0x4C + sizeof(s_player_configuration_from_host_patial));

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

