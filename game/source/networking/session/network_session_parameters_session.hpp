#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_parameters_generic.hpp"
#include "networking/transport/transport_security.hpp"

struct s_network_session_parameter_session_mode
{
	int32 session_mode_sequence;
	c_enum<e_network_session_mode, int32, _network_session_mode_none, k_network_session_mode_count> session_mode;
	uns32 session_mode_timestamp;
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_session_mode) == 0xC);

class c_network_session_parameter_session_mode :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_parameter_session_mode>
{
public:
	e_network_session_mode get() const;
	bool request_change(e_network_session_mode session_mode);
	bool set(e_network_session_mode session_mode);
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_session_mode) == 0x48);

struct s_network_session_parameter_session_size
{
	int32 maximum_peer_count;
	int32 maximum_player_count;
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_session_size) == 0x8);

class c_network_session_parameter_session_size :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_parameter_session_size>
{
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_session_size) == 0x40);

struct s_network_session_remote_session_join_data
{
	c_enum<e_join_remote_state, int32, _join_remote_state_none, k_join_remote_state_count> join_state;
	c_enum<e_join_type, int32, _join_type_squad, k_join_type_count> join_from;
	c_enum<e_join_type, int32, _join_type_squad, k_join_type_count> join_to;
	c_enum<e_networking_join_destination_squad, int32, _join_destination_target, k_join_destination_count> join_target;
	uns64 join_nonce;
	c_enum<e_transport_platform, int32, _transport_platform_xenon, k_transport_platform_count> platform;
	s_transport_secure_identifier session_id;
	s_transport_secure_key session_key;
	s_transport_secure_address host_secure_address;
	c_enum<e_network_session_class, int32, _network_session_class_offline, k_network_session_class_count> session_class;
	c_enum<e_life_cycle_join_result, int32, _life_cycle_join_result_none, k_life_cycle_join_result_count> join_result;
	bool join_to_public_slots;
};
COMPILE_ASSERT(sizeof(s_network_session_remote_session_join_data) == 0x58);

class c_network_session_parameter_requested_remote_join_data :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_remote_session_join_data>
{
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_requested_remote_join_data) == 0xE0);

class c_network_session_parameter_remote_join_data :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_remote_session_join_data>
{
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_remote_join_data) == 0xE0);

struct s_network_session_parameter_lobby_vote_set
{
	struct s_lobby_vote
	{
		uns8 __unknown0;
		uns8 __unknown1;
		uns8 __unknown2;
	};

	c_static_array<s_lobby_vote, 2> __unknown0;
	uns8 __unknown6;
	uns8 : 8;
};
COMPILE_ASSERT(sizeof(s_network_session_parameter_lobby_vote_set) == 0x8);

class c_network_session_parameter_lobby_vote_set :
	public c_network_session_parameter_base,
	public c_generic_network_session_parameter_data<s_network_session_parameter_lobby_vote_set>
{
};
COMPILE_ASSERT(sizeof(c_network_session_parameter_lobby_vote_set) == 0x40);

class c_bitstream;
extern bool __cdecl session_mode_decode_function(c_bitstream* packet, void* data, int32 data_size);
extern void __cdecl session_mode_encode_function(c_bitstream* packet, const void* data, int32 data_size);
extern bool __cdecl session_size_decode_function(c_bitstream* packet, void* data, int32 data_size);
extern void __cdecl session_size_encode_function(c_bitstream* packet, const void* data, int32 data_size);

