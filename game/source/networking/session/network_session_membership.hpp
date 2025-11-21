#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "game/players.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/online/online.hpp"
#include "networking/transport/transport_security.hpp"
#include "text/unicode.hpp"

struct s_network_session_peer_connectivity
{
	uns16 peer_connectivity_mask;
	uns16 peer_probe_mask;
	int32 peer_latency_min;
	int32 peer_latency_est;
	int32 peer_latency_max;
};

struct s_network_session_peer_properties
{
	wchar_t peer_name[16];
	wchar_t peer_session_name[32];
	uns32 available_multiplayer_map_mask;
	e_map_id peer_map_id;
	e_network_session_map_status peer_map_status;
	int32 peer_map_progress_percentage;
	uns64 peer_game_instance;
	int32 ready_hopper_identifier;
	e_session_game_start_error game_start_error;
	bool has_hard_drive;
	int32 peer_estimated_downstream_bandwidth_bps;
	int32 peer_estimated_upstream_bandwidth_bps;
	bool peer_upstream_estimate_is_reliable;
	e_online_nat_type peer_nat_type;
	int32 connectivity_badness_rating;
	int32 host_badness_rating;
	int32 client_badness_rating;
	s_network_session_peer_connectivity peer_connectivity;
	e_language language;
	int32 determinism_version;
	int32 determinism_compatible_version;
	c_network_session_peer_properties_status_flags flags;
};
static_assert(sizeof(s_network_session_peer_properties) == 0xC0);

struct s_network_session_peer
{
	s_transport_secure_address secure_address;
	c_enum<e_network_session_peer_state, int32, _network_session_peer_state_none, k_network_session_peer_state_count> connection_state;
	int32 version;
	uns32 join_start_time;
	int32 __unknown1C;
	s_network_session_peer_properties properties;
	uns64 party_nonce;
	uns64 join_nonce;
	uns32 player_mask[1];
};
static_assert(sizeof(s_network_session_peer) == 0xF8);

struct s_network_session_player
{
	int32 player_update_number;
	s_player_identifier player_identifier;
	int32 peer_index;
	int32 peer_user_index;
	bool left_game;
	byte __pad15[3];
	e_controller_index controller_index;
	uns32 __unknown1C;
	s_player_configuration player_data;
	union
	{
		uns32 player_voice_settings;
		struct
		{
			int16 player_mute_mask;
			int16 player_voice_flags;
		};
	};

	uns32 __unknown1644;
};
static_assert(sizeof(s_network_session_player) == 0x1648);

struct s_network_session_shared_membership
{
	int32 update_number;
	int32 leader_peer_index;
	int32 host_peer_index;
	int32 private_slot_count;
	int32 public_slot_count;
	bool friends_only;
	bool are_slots_locked;
	uns8 __unknown16;
	uns8 __unknown17;
	int32 peer_count;
	c_static_flags<k_network_maximum_machines_per_session> peer_valid_mask;
	s_network_session_peer peers[k_network_maximum_machines_per_session];
	int32 player_count;
	c_static_flags<16> player_valid_mask;
	s_network_session_player players[16];
	int32 player_sequence_number;
	int32 __unknown17524;
};
constexpr size_t agt = OFFSETOF(s_network_session_shared_membership, __unknown17524);
static_assert(sizeof(s_network_session_shared_membership) == 0x17528);

struct s_local_session_peer
{
	uns32 flags;
	int32 channel_index;
	int32 expected_update_number;
};
static_assert(sizeof(s_local_session_peer) == 0xC);

struct s_player_add_queue_entry
{
	s_player_identifier player_identifier;
	int32 player_index;
	int32 user_index;
	e_controller_index controller_index;
	s_player_configuration_from_client player_data_from_client;
	uns32 player_voice_settings;
};
static_assert(sizeof(s_player_add_queue_entry) == 0x48);

class c_network_session;
class c_network_session_membership
{
public:
	int32 get_first_peer() const;
	int32 get_first_player() const;
	const s_network_session_peer* get_host_peer() const;
	s_network_session_peer* get_host_peer();
	const s_network_session_peer* get_local_peer() const;
	s_network_session_peer* get_local_peer();
	int32 get_next_peer(int32 peer_index) const;
	int32 get_next_player(int32 peer_index) const;
	s_network_session_peer const* get_peer(int32 peer_index) const;
	s_network_session_peer* get_peer(int32 peer_index);
	e_network_session_peer_state get_peer_connection_state(int32 peer_index) const;
	s_network_session_player* get_player(int32 player_index);
	int32 get_player_index_from_peer(int32 peer_index);
	bool is_peer_valid(int32 peer_index) const;
	int32 get_observer_channel_index(int32 peer_index) const;
	int32 get_peer_from_observer_channel(int32 observer_channel_index) const;
	bool has_membership() const;
	int32 host_peer_index() const;
	void increment_update();
	bool is_leader() const;
	bool is_player_valid(int32 player_index) const;
	int32 local_peer_index() const;
	bool peer_property_flag_test(e_peer_property_flag_test_type test_type, e_network_session_peer_properties_status_flags flag) const;
	bool peer_property_flag_test_any_peer(e_network_session_peer_properties_status_flags flag) const;
	void set_player_properties(int32 player_index, int32 player_update_number, e_controller_index controller_index, const void* player_data_from_client, int32 player_voice_settings);

	c_network_session* m_session;
	s_network_session_shared_membership m_shared_network_membership;
	c_static_array<s_network_session_shared_membership, k_network_maximum_machines_per_session> m_transmitted_shared_network_membership;
	c_static_array<uns32, k_network_maximum_machines_per_session> m_baseline_checksum;
	bool __unknown1A3D1C; // m_has_received_a_membership_update?
	bool __unknown1A3D1D; // m_completed_first_player_update?
	bool __unknown1A3D1E; // m_needs_player_update?
	bool __unknown1A3D1F;
	int32 m_local_peer_index;
	int32 m_local_membership_update_number;
	s_local_session_peer m_local_peer_state[k_network_maximum_machines_per_session];
	s_player_add_queue_entry m_player_add_queue[4];
	int32 m_add_queue_first_used_slot;
	int32 m_add_queue_first_free_slot;
};
static_assert(sizeof(c_network_session_membership) == 0x1A3F20);
static_assert(0x00000000 == OFFSETOF(c_network_session_membership, m_session));
static_assert(0x00000008 == OFFSETOF(c_network_session_membership, m_shared_network_membership));
static_assert(0x00017530 == OFFSETOF(c_network_session_membership, m_transmitted_shared_network_membership));
static_assert(0x001A3CD8 == OFFSETOF(c_network_session_membership, m_baseline_checksum));
static_assert(0x001A3D1C == OFFSETOF(c_network_session_membership, __unknown1A3D1C));
static_assert(0x001A3D1D == OFFSETOF(c_network_session_membership, __unknown1A3D1D));
static_assert(0x001A3D1E == OFFSETOF(c_network_session_membership, __unknown1A3D1E));
static_assert(0x001A3D1F == OFFSETOF(c_network_session_membership, __unknown1A3D1F));
static_assert(0x001A3D20 == OFFSETOF(c_network_session_membership, m_local_peer_index));
static_assert(0x001A3D24 == OFFSETOF(c_network_session_membership, m_local_membership_update_number));
static_assert(0x001A3D28 == OFFSETOF(c_network_session_membership, m_local_peer_state));
static_assert(0x001A3DF4 == OFFSETOF(c_network_session_membership, m_player_add_queue));
static_assert(0x001A3F14 == OFFSETOF(c_network_session_membership, m_add_queue_first_used_slot));
static_assert(0x001A3F18 == OFFSETOF(c_network_session_membership, m_add_queue_first_free_slot));

