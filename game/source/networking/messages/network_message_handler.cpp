#include "networking/messages/network_message_handler.hpp"

#include "cseries/cseries_console.hpp"
#include "game/game_results.hpp"
#include "game/player_control.hpp"
#include "game/players.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/delivery/network_link.hpp"
#include "networking/logic/network_broadcast_search.hpp"
#include "networking/logic/network_join.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/session/network_session_membership.hpp"
#include "networking/session/network_session_parameters_base.hpp"
#include "networking/transport/transport_address.hpp"
#include "networking/transport/transport_security.hpp"
#include "simulation/simulation.hpp"
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

//struct s_network_message_broadcast_search
//{
//	word protocol_version;
//	qword nonce;
//};
//static_assert(sizeof(s_network_message_broadcast_search) == 0x10);

//struct s_network_message_broadcast_reply
//{
//	word protocol_version;
//	qword search_nonce;
//	s_network_session_status_data status_data;
//};
//static_assert(sizeof(s_network_message_broadcast_reply) == 0x164D0);

struct s_network_message_connect_request
{
	dword channel_identifier;
	dword_flags flags;
};
static_assert(sizeof(s_network_message_connect_request) == 0x8);

struct s_network_message_connect_refuse
{
	dword remote_identifier;
	long reason;
};
static_assert(sizeof(s_network_message_connect_refuse) == 0x8);

struct s_network_message_connect_establish
{
	dword identifier;
	dword remote_identifier;
};
static_assert(sizeof(s_network_message_connect_establish) == 0x8);

struct s_network_message_connect_closed
{
	dword remote_identifier;
	dword identifier;
	c_enum<e_network_connect_closed_reason, long, _network_connect_closed_reason_none, k_network_connect_closed_reason_count> reason;
};
static_assert(sizeof(s_network_message_connect_closed) == 0xC);

struct s_network_message_join_request
{
	word protocol;
	word executable_type;
	dword minimum_network_version;
	dword current_network_version;
	s_transport_secure_identifier session_id;
	s_network_session_join_request join_request;
};
static_assert(sizeof(s_network_message_join_request) == 0x330);

struct s_network_message_peer_connect
{
	word protocol;
	s_transport_secure_identifier session_id;
	qword join_nonce;
};
static_assert(sizeof(s_network_message_peer_connect) == 0x20);

struct s_network_message_join_abort
{
	s_transport_secure_identifier session_id;
	qword join_nonce;
};
static_assert(sizeof(s_network_message_join_abort) == 0x18);

struct s_network_message_join_refuse
{
	s_transport_secure_identifier session_id;
	c_enum<e_network_join_refuse_reason, long, _network_join_refuse_reason_none, k_network_join_refuse_reason_count> reason;
};
static_assert(sizeof(s_network_message_join_refuse) == 0x14);

struct s_network_message_leave_session
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_leave_session) == 0x10);

struct s_network_message_leave_acknowledge
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_leave_acknowledge) == 0x10);

struct s_network_message_session_disband
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_session_disband) == 0x10);

struct s_network_message_session_boot
{
	s_transport_secure_identifier session_id;
	c_enum<e_network_session_boot_reason, long, _network_session_boot_reason_player_booted_player, k_network_session_boot_reason_count> reason;
};
static_assert(sizeof(s_network_message_session_boot) == 0x14);

struct s_network_message_host_decline
{
	s_transport_secure_identifier session_id;
	bool session_exists;
	bool peer_exists;
	bool host_exists;
	s_transport_secure_address host_address;
};
static_assert(sizeof(s_network_message_host_decline) == 0x24);

struct s_network_message_peer_establish
{
	s_transport_secure_identifier session_id;
};
static_assert(sizeof(s_network_message_peer_establish) == 0x10);

struct s_network_message_time_synchronize
{
	s_transport_secure_identifier session_id;
	dword client_timestamp[2];
	dword authority_timestamp[2];
	dword synchronization_stage;
};
static_assert(sizeof(s_network_message_time_synchronize) == 0x24);

//struct s_network_session_peer_connectivity
//{
//	word_flags peer_connectivity_mask;
//	word_flags peer_probe_mask;
//	long peer_latency_min;
//	long peer_latency_est;
//	long peer_latency_max;
//};
//static_assert(sizeof(s_network_session_peer_connectivity) == 0x10);

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

struct s_network_message_parameters_update
{
	s_transport_secure_identifier session_id;
	bool initial_update;
	qword cleared_parameters;
	qword updated_parameters;
	s_network_session_parameter_update update;
};
static_assert(sizeof(s_network_message_parameters_update) == 0x20028);

struct s_network_message_parameters_request
{
	s_transport_secure_identifier session_id;
	qword change_request_parameters;
	s_network_session_parameter_update update;
};
static_assert(sizeof(s_network_message_parameters_request) == 0x20018);

struct s_network_message_view_establishment
{
	dword establishment_mode;
	dword establishment_identifier;

	bool signature_exists;
	dword signature_size;
	byte signature_data[0x3C];
};
static_assert(sizeof(s_network_message_view_establishment) == 0x4C);

struct s_network_message_player_acknowledge
{
	dword player_valid_mask;
	dword player_in_game_mask;
	c_static_array<s_player_identifier, 16> player_identifiers;
};
static_assert(sizeof(s_network_message_player_acknowledge) == 0x88);

struct s_network_message_synchronous_update
{
	simulation_update update;
	s_simulation_update_metadata metadata;
	byte __data[0x4];
};
static_assert(sizeof(s_network_message_synchronous_update) == 0x1668);

struct s_network_message_synchronous_playback_control
{
	long type;
	dword identifier;
	long update_number;
};
static_assert(sizeof(s_network_message_synchronous_playback_control) == 0xC);

struct s_network_message_synchronous_actions
{
	long action_number;
	long current_action_number;
	dword_flags user_flags;
	long : 32;
	c_static_array<player_action, 4> actions;
};
static_assert(sizeof(s_network_message_synchronous_actions) == 0x210);

struct s_network_message_synchronous_acknowledge
{
	long current_update_number;
};
static_assert(sizeof(s_network_message_synchronous_acknowledge) == 0x4);

struct s_network_message_synchronous_gamestate
{
	byte message_type;
	dword chunk_offset_next_update_compressed_checksum;
	dword chunk_size;
	dword decompressed_checksum;
#pragma warning(push)
#pragma warning(disable : 4200)
	byte additional_data[];
#pragma warning(pop)
};
static_assert(sizeof(s_network_message_synchronous_gamestate) == 0x10);

struct s_network_message_distributed_game_results
{
	dword establishment_identifier;
	long update_number;
	s_game_results_incremental_update update;
};
static_assert(sizeof(s_network_message_distributed_game_results) == 0x1B7A8);

void __cdecl c_network_message_handler::handle_out_of_band_message(transport_address const* incoming_address, e_network_message_type message_type, long message_storage_size, void const* message_storage)
{
	ASSERT(m_initialized);

	#define received_log c_console::write_line("networking:messages:handler: %d/%s received out-of-band from '%s'", \
		message_type, \
		m_message_types->get_message_type_name(message_type), \
		transport_address_get_string(incoming_address))
	
	network_message_converter_t converter = { .message_storage = message_storage };

	bool message_type_handled = false;
	switch (message_type)
	{
	//case _network_message_ping:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_ping));
	//	handle_ping(incoming_address, converter.message_ping);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_pong:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_pong));
	//	handle_pong(incoming_address, converter.message_pong);
	//	message_type_handled = true;
	//}
	//break;
	case _network_message_broadcast_search:
	{
		received_log;
		ASSERT(message_storage_size == sizeof(s_network_message_broadcast_search));
		handle_broadcast_search(incoming_address, converter.message_broadcast_search);
		message_type_handled = true;
	}
	break;
	case _network_message_broadcast_reply:
	{
		received_log;
		ASSERT(message_storage_size == sizeof(s_network_message_broadcast_reply));
		handle_broadcast_reply(incoming_address, converter.message_broadcast_reply);
		message_type_handled = true;
	}
	break;
	//case _network_message_connect_request:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_connect_request));
	//	handle_connect_request(incoming_address, converter.message_connect_request);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_connect_refuse:
	//{
	//	c_network_channel* associated_channel = m_link->get_associated_channel(incoming_address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_refuse));
	//		handle_connect_refuse(associated_channel, converter.message_connect_refuse);
	//		message_type_handled = true;
	//	}
	//}
	//break;
	//case _network_message_connect_establish:
	//{
	//	c_network_channel* associated_channel = m_link->get_associated_channel(incoming_address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_establish));
	//		handle_connect_establish(associated_channel, converter.message_connect_establish);
	//		message_type_handled = true;
	//	}
	//}
	//break;
	//case _network_message_connect_closed:
	//{
	//	c_network_channel* associated_channel = m_link->get_associated_channel(incoming_address);
	//	if (associated_channel)
	//	{
	//		ASSERT(message_storage_size == sizeof(s_network_message_connect_closed));
	//		handle_connect_closed(associated_channel, converter.message_connect_closed);
	//		message_type_handled = true;
	//	}
	//}
	//break;
	//case _network_message_join_request:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_join_request));
	//	handle_join_request(incoming_address, converter.message_join_request);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_peer_connect:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_peer_connect));
	//	handle_peer_connect(incoming_address, converter.message_peer_connect);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_join_abort:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_join_abort));
	//	handle_join_abort(incoming_address, converter.message_join_abort);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_join_refuse:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_join_refuse));
	//	handle_join_refuse(incoming_address, converter.message_join_refuse);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_leave_session:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_leave_session));
	//	handle_leave_session(incoming_address, converter.message_leave_session);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_leave_acknowledge:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_leave_acknowledge));
	//	handle_leave_acknowledge(incoming_address, converter.message_leave_acknowledge);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_session_disband:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_session_disband));
	//	handle_session_disband(incoming_address, converter.message_session_disband);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_session_boot:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_session_boot));
	//	handle_session_boot(incoming_address, converter.message_session_boot);
	//	message_type_handled = true;
	//}
	//break;
	//case _network_message_time_synchronize:
	//{
	//	ASSERT(message_storage_size == sizeof(s_network_message_time_synchronize));
	//	handle_time_synchronize(incoming_address, converter.message_time_synchronize);
	//	message_type_handled = true;
	//}
	//break;
	//default:
	//{
	//	c_console::write_line("networking:messages:handler: %d/%s from '%s' cannot be handled out-of-band, discarding",
	//		message_type,
	//		m_message_types->get_message_type_name(message_type),
	//		transport_address_get_string(incoming_address));
	//}
	//break;
	}

	if (!message_type_handled)
		DECLFUNC(0x0049D2C0, void, __thiscall, c_network_message_handler*, transport_address const*, e_network_message_type, long, void const*)(this, incoming_address, message_type, message_storage_size, message_storage);
}

void __cdecl c_network_message_handler::handle_ping(transport_address const* incoming_address, s_network_message_ping const* message)
{
	DECLFUNC(0x0049D720, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_ping const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_pong(transport_address const* incoming_address, s_network_message_pong const* message)
{
	DECLFUNC(0x0049D9B0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_pong const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_broadcast_search(transport_address const* incoming_address, s_network_message_broadcast_search const* message)
{
	DECLFUNC(0x0049C310, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_broadcast_search const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_broadcast_reply(transport_address const* incoming_address, s_network_message_broadcast_reply const* message)
{
	DECLFUNC(0x0049C2B0, void, __cdecl, transport_address const*, s_network_message_broadcast_reply const*)(incoming_address, message);
}

void __cdecl c_network_message_handler::handle_connect_request(transport_address const* incoming_address, s_network_message_connect_request const* message)
{
	DECLFUNC(0x0049CDC0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_connect_request const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_connect_refuse(c_network_channel* channel, s_network_message_connect_refuse const* message)
{
	DECLFUNC(0x0049CD10, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_connect_refuse const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_connect_establish(c_network_channel* channel, s_network_message_connect_establish const* message)
{
	DECLFUNC(0x0049CC10, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_connect_establish const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_connect_closed(c_network_channel* channel, s_network_message_connect_closed const* message)
{
	DECLFUNC(0x0049CBA0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_connect_closed const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_join_request(transport_address const* incoming_address, s_network_message_join_request const* message)
{
	DECLFUNC(0x0049D0C0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_join_request const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_peer_connect(transport_address const* incoming_address, s_network_message_peer_connect const* message)
{
	DECLFUNC(0x0049D5C0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_peer_connect const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_join_abort(transport_address const* incoming_address, s_network_message_join_abort const* message)
{
	DECLFUNC(0x0049CF00, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_join_abort const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_join_refuse(transport_address const* incoming_address, s_network_message_join_refuse const* message)
{
	DECLFUNC(0x0049CFF0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_join_refuse const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_leave_session(transport_address const* incoming_address, s_network_message_leave_session const* message)
{
	DECLFUNC(0x0049D1C0, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_leave_session const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_leave_acknowledge(transport_address const* incoming_address, s_network_message_leave_acknowledge const* message)
{
	DECLFUNC(0x0049D150, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_leave_acknowledge const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_session_disband(transport_address const* incoming_address, s_network_message_session_disband const* message)
{
	DECLFUNC(0x0049DA80, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_session_disband const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_session_boot(transport_address const* incoming_address, s_network_message_session_boot const* message)
{
	DECLFUNC(0x0049DA00, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_session_boot const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message)
{
	DECLFUNC(0x0049CEA0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_host_decline const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message)
{
	DECLFUNC(0x0049D620, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_peer_establish const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_time_synchronize(transport_address const* incoming_address, s_network_message_time_synchronize const* message)
{
	DECLFUNC(0x0049DE40, void, __thiscall, c_network_message_handler*, transport_address const*, s_network_message_time_synchronize const*)(this, incoming_address, message);
}

void __cdecl c_network_message_handler::handle_membership_update(c_network_channel* channel, s_network_message_membership_update const* message)
{
	DECLFUNC(0x0049D250, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_membership_update const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message)
{
	DECLFUNC(0x0049D6B0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_peer_properties const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_delegate_leadership(c_network_channel* channel, s_network_message_delegate_leadership const* message)
{
	DECLFUNC(0x0049CDD0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_delegate_leadership const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_boot_machine(c_network_channel* channel, s_network_message_boot_machine const* message)
{
	DECLFUNC(0x0049C240, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_boot_machine const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_player_add(c_network_channel* channel, s_network_message_player_add const* message)
{
	DECLFUNC(0x0049D7F0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_player_add const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message)
{
	DECLFUNC(0x0049D8D0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_player_refuse const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_player_remove(c_network_channel* channel, s_network_message_player_remove const* message)
{
	DECLFUNC(0x0049D940, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_player_remove const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message)
{
	DECLFUNC(0x0049D860, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_player_properties const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_parameters_update(c_network_channel* channel, s_network_message_parameters_update const* message)
{
	DECLFUNC(0x0049D550, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_parameters_update const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message)
{
	DECLFUNC(0x0049D4E0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_parameters_request const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_view_establishment(c_network_channel* channel, dword a2, s_network_message_view_establishment const* message)
{
	DECLFUNC(0x0049DEC0, void, __thiscall, c_network_message_handler*, c_network_channel*, dword, s_network_message_view_establishment const*)(this, channel, a2, message);
}

void __cdecl c_network_message_handler::handle_player_acknowledge(c_network_channel* channel, s_network_message_player_acknowledge const* message)
{
	DECLFUNC(0x0049D780, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_player_acknowledge const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_synchronous_update(c_network_channel* channel, s_network_message_synchronous_update const* message)
{
	DECLFUNC(0x0049DDA0, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_synchronous_update const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_synchronous_playback_control(c_network_channel* channel, dword a2, s_network_message_synchronous_playback_control const* message)
{
	DECLFUNC(0x0049DD10, void, __thiscall, c_network_message_handler*, c_network_channel*, dword, s_network_message_synchronous_playback_control const*)(this, channel, a2, message);
}

void __cdecl c_network_message_handler::handle_synchronous_actions(c_network_channel* channel, s_network_message_synchronous_actions const* message)
{
	DECLFUNC(0x0049DB80, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_synchronous_actions const*)(this, channel, message);
}

void __cdecl c_network_message_handler::handle_synchronous_acknowledge(c_network_channel* channel, dword a2, s_network_message_synchronous_acknowledge const* message)
{
	DECLFUNC(0x0049DB00, void, __thiscall, c_network_message_handler*, c_network_channel*, dword, s_network_message_synchronous_acknowledge const*)(this, channel, a2, message);
}

void __cdecl c_network_message_handler::handle_synchronous_gamestate(c_network_channel* channel, s_network_message_synchronous_gamestate const* message, long a3, void const* a4)
{
	DECLFUNC(0x0049DC70, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_synchronous_gamestate const*, long, void const*)(this, channel, message, a3, a4);
}

void __cdecl c_network_message_handler::handle_distributed_game_results(c_network_channel* channel, s_network_message_distributed_game_results const* message)
{
	DECLFUNC(0x0049CE40, void, __thiscall, c_network_message_handler*, c_network_channel*, s_network_message_distributed_game_results const*)(this, channel, message);
}
