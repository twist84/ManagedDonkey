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
	uns16 ready_hopper_id;
	uns32 game_start_error;

	bool peer_map_id_updated;
	uns32 peer_map_id;

	bool peer_map_updated;
	int32 peer_map_status;
	uns32 peer_map_progress_percentage;

	bool peer_game_instance_updated;
	uns64 peer_game_instance;

	bool available_multiplayer_map_mask_updated;
	uns32 available_multiplayer_map_mask;

	bool peer_connection_updated;
	uns32 estimated_downstream_bandwidth_bps;
	uns32 estimated_upstream_bandwidth_bps;

	bool estimated_upstream_is_reliable;
	uns32 nat_type;
	uns32 connectivity_badness_rating;
	uns32 host_badness_rating;
	uns32 client_badness_rating;
	uns32 language;
	s_network_session_peer_connectivity connectivity;

	bool versions_updated;
	uns32 determinism_version;
	uns32 determinism_compatible_version;

	bool flags_updated;
	uns32 flags;
};
COMPILE_ASSERT(sizeof(s_network_message_membership_update_peer_properties) == 0xE0);

struct s_network_message_membership_update_peer
{
	uns32 peer_index;
	uns32 peer_update_type;
	uns32 peer_connection_state;

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
	uns64 peer_party_nonce;
	uns64 peer_join_nonce;
	uns32 network_version_number;
	uns32 peer_creation_timestamp;

	bool peer_properties_updated;
	s_network_message_membership_update_peer_properties peer_properties;
};
COMPILE_ASSERT(sizeof(s_network_message_membership_update_peer) == 0x120);

struct s_network_message_membership_update_player
{
	int32 player_index;
	int32 player_update_type;
	bool player_location_updated;
	s_player_identifier identifier;
	int16 : 16;
	int16 peer_index;
	int32 player_addition_number;
	bool player_occupies_a_public_slot;
	bool player_properties_updated;
	int16 peer_user_index;
	int32 player_update_number;
	int32 controller_index;
	int32 : 32;
	s_player_configuration player_data;
	uns32 player_voice;
};
COMPILE_ASSERT(sizeof(s_network_message_membership_update_player) == 0x1650);

struct s_network_message_membership_update
{
	s_transport_secure_identifier session_id;
	int32 update_number;
	int32 incremental_update_number;
	uns32 baseline_checksum;

	int16 peer_update_count;
	int16 player_update_count;
	c_static_array<s_network_message_membership_update_peer, 17> peer;
	c_static_array<s_network_message_membership_update_player, 16> players;

	bool player_addition_number_updated;
	int32 player_addition_number;

	bool leader_updated;
	int32 leader_peer_index;

	bool host_updated;
	int32 host_peer_index;

	bool slot_counts_updated;
	int32 private_slot_count;
	int32 public_slot_count;
	bool friends_only;
	bool are_slots_locked;

	uns32 checksum;
	int32 : 32;
};
COMPILE_ASSERT(sizeof(s_network_message_membership_update) == 0x17870);

struct s_network_message_peer_properties
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address secure_address;
	s_network_session_peer_properties peer_properties;
};
COMPILE_ASSERT(sizeof(s_network_message_peer_properties) == 0xE0);

struct s_network_message_delegate_leadership
{
	s_transport_secure_identifier session_id;
	s_transport_secure_address desired_leader_address;
};
COMPILE_ASSERT(sizeof(s_network_message_delegate_leadership) == 0x20);

struct s_network_message_boot_machine
{
	s_transport_secure_identifier session_id;
	int32 reason; // e_network_session_boot_reason
	s_transport_secure_address boot_peer_address;
};
COMPILE_ASSERT(sizeof(s_network_message_boot_machine) == 0x24);

struct s_network_message_player_add
{
	s_transport_secure_identifier session_id;
	s_player_identifier player_identifier;
	int32 user_index;
	int32 controller_index;
	s_player_configuration_from_client player_data;
	int32 player_voice;
};
COMPILE_ASSERT(sizeof(s_network_message_player_add) == 0x54);

struct s_network_message_player_refuse
{
	s_transport_secure_identifier session_id;
	s_player_identifier player_identifier;
	int32 join_refusal_reason;
};
COMPILE_ASSERT(sizeof(s_network_message_player_refuse) == 0x1C);

struct s_network_message_player_remove
{
	s_transport_secure_identifier session_id;
};
COMPILE_ASSERT(sizeof(s_network_message_player_remove) == 0x10);

struct s_player_configuration_from_host_partial
{
	c_static_wchar_string<5> service_tag;
	c_flags<e_bungienet_user, uns8, k_bungienet_user_count> bungienet_user;
	c_static_array<rgb_color, k_color_type_count> colors;
	c_static_array<uns8, k_armor_type_count> armors;
	c_static_array<int8, 4> consumables;
	s_emblem_info emblem;
};
COMPILE_ASSERT(sizeof(s_player_configuration_from_host_partial) == 0x38);

struct s_player_configuration_for_player_properties
{
	s_player_configuration_from_client client;
	s_player_configuration_from_host_partial host_partial;
};
COMPILE_ASSERT(sizeof(s_player_configuration_for_player_properties) == 0x68);

struct s_network_message_player_properties
{
	s_transport_secure_identifier session_id;
	int32 player_update_number;
	e_controller_index controller_index;
	s_player_configuration_for_player_properties player_data;
	int32 player_voice_settings;
};
COMPILE_ASSERT(sizeof(s_network_message_player_properties) == 0x4C + sizeof(s_player_configuration_from_host_partial));

class c_bitstream;

class c_network_message_membership_update
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_peer_properties
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_delegate_leadership
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_boot_machine
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_player_add
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_player_refuse
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_player_remove
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_player_properties
{
public:
	static void __cdecl encode(c_bitstream* packet, int32 message_storage_size, const void* message_storage);
	static bool __cdecl decode(c_bitstream* packet, int32 message_storage_size, void* message_storage);
};

class c_network_message_type_collection;
extern void __cdecl network_message_types_register_session_membership(c_network_message_type_collection* message_collection);

