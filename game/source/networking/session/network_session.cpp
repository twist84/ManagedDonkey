#include "networking/session/network_session.hpp"

#include "cseries/cseries_events.hpp"
#include "hf2p/hf2p.hpp"
#include "interface/c_controller.hpp"
#include "memory/module.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/messages/network_messages_session_membership.hpp"
#include "networking/session/network_managed_session.hpp"
#include "networking/session/network_observer.hpp"

bool __fastcall network_session_peer_request_player_desired_properties_update(c_network_session* _this, void* usused, long player_update_number, e_controller_index controller_index, s_player_configuration_from_client const* player_data_from_client, dword player_voice)
{
	return _this->peer_request_player_desired_properties_update(player_update_number, controller_index, player_data_from_client, player_voice);
}
HOOK_DECLARE_CALL(0x00437C8C, network_session_peer_request_player_desired_properties_update);

long c_network_session::current_local_state() const
{
	//return DECLFUNC(0x00434820, long, __thiscall, c_network_session const*)(this);

	return m_local_state;
}

bool c_network_session::disconnected() const
{
	//return DECLFUNC(0x00434830, bool, __thiscall, c_network_session const*)(this);

	return current_local_state() == _network_session_state_none;
}

bool c_network_session::established() const
{
	return current_local_state() >= _network_session_state_peer_established;
}

bool c_network_session::is_host() const
{
	//return DECLFUNC(0x00434D90, bool, __thiscall, c_network_session const*)(this);

	long current_state = current_local_state();
	if (current_state == _network_session_state_host_established
		|| current_state == _network_session_state_host_disband/*
		|| current_state == _network_session_state_host_handoff
		|| current_state == _network_session_state_host_reestablish*/)
	{
		ASSERT(m_session_membership.local_peer_index() == m_session_membership.host_peer_index());
		return true;
	}
	else if (established())
	{
		ASSERT(m_session_membership.local_peer_index() != m_session_membership.host_peer_index());
	}

	return false;
}

bool c_network_session::is_leader() const
{
	//return DECLFUNC(0x00434DB0, bool, __thiscall, c_network_session const*)(this);

	//return m_session_membership.is_leader();
	return m_session_membership.m_local_peer_index == m_session_membership.m_shared_network_membership.leader_peer_index;
}

bool c_network_session::leaving_session() const
{
	return DECLFUNC(0x00434E30, bool, __thiscall, c_network_session const*)(this);
}

bool c_network_session::peer_joining() const
{
	return DECLFUNC(0x0044FD10, bool, __thiscall, c_network_session const*)(this);
}

bool c_network_session::channel_is_authoritative(c_network_channel* channel)
{
	return DECLFUNC(0x0045A9E0, bool, __thiscall, c_network_session*, c_network_channel*)(this, channel);
}

void c_network_session::destroy_session()
{
	DECLFUNC(0x0045B960, void, __thiscall, c_network_session*)(this);
}

void c_network_session::force_disconnect()
{
	DECLFUNC(0x0045BE20, void, __thiscall, c_network_session*)(this);
}

bool c_network_session::force_disconnect_peer(s_transport_secure_address const* peer_secure_address)
{
	return DECLFUNC(0x0045BE80, bool, __thiscall, c_network_session*, s_transport_secure_address const*)(this, peer_secure_address);
}

long c_network_session::get_peer_observer_channel(long peer_index) const
{
	//return DECLFUNC(0x0045C100, long, __thiscall, c_network_session const*, long)(this, peer_index);

	ASSERT(established());

	if (peer_index != NONE && m_session_membership.is_peer_valid(peer_index))
		return m_session_membership.get_observer_channel_index(peer_index);

	return NONE;
}

c_network_session_membership const* c_network_session::get_session_membership() const
{
	return DECLFUNC(0x0045C250, c_network_session_membership const*, __thiscall, c_network_session const*)(this);

	//ASSERT(established());
	//ASSERT(m_session_membership.has_membership());
	//ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
	//ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));
	//
	//return &m_session_membership;
}

c_network_session_membership* c_network_session::get_session_membership_for_update()
{
	//return DECLFUNC(0x0045C260, c_network_session_membership*, __thiscall, c_network_session*)(this);

	ASSERT(established());
	ASSERT(is_host());
	ASSERT(m_session_membership.has_membership());
	ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
	ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));

	return &m_session_membership;
}

c_network_session_membership const* c_network_session::get_session_membership_unsafe() const
{
	//return DECLFUNC(0x0045C270, c_network_session_membership const*, __thiscall, c_network_session const*)(this);

	if (disconnected() || !m_session_membership.has_membership())
		return nullptr;

	ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
	ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));

	return &m_session_membership;
}

long c_network_session::get_session_membership_update_number() const
{
	return DECLFUNC(0x0045C290, long, __thiscall, c_network_session const*)(this);

	//ASSERT(established());
	//ASSERT(m_session_membership.has_membership());
	//
	//return m_session_membership.update_number();
}

c_network_session_parameters* c_network_session::get_session_parameters()
{
	//return DECLFUNC(0x0045C2A0, c_network_session_parameters*, __thiscall, c_network_session*)(this);

	return &m_session_parameters;
}

c_network_session_parameters const* c_network_session::get_session_parameters() const
{
	//return DECLFUNC(0x0045C2B0, c_network_session_parameters const*, __thiscall, c_network_session const*)(this);

	return &m_session_parameters;
}

char const* c_network_session::get_type_string(e_network_session_type type)
{
	static char const* const type_names[k_network_session_type_count]
	{
		"none",
		"squad",
		"group"
	};

	if (VALID_INDEX(type, k_network_session_type_count))
		return type_names[type];

	return "<unknown>";
}

char const* __cdecl c_network_session::get_class_string(e_network_session_class session_class)
{
	static char const* const session_class_names[k_network_session_class_count]
	{
		"offline",
		"system-link",
		"xbox-live"
	};

	if (VALID_INDEX(session_class, k_network_session_class_count))
		return session_class_names[session_class];

	return "<unknown>";
}

char const* c_network_session::get_state_string() const
{
	static char const* const state_names[k_network_session_state_count]
	{
		"none",
		"peer-creating",
		"peer-joining",
		"peer-join-abort",
		"peer-established",
		"peer-leaving",
		"host-established",
		"host-disband"

		//"host-handoff",
		//"host-reestablish",
		//"election"
	};

	long current_state = current_local_state();
	ASSERT(current_state >= 0 && current_state < k_network_session_state_count);

	return state_names[current_state];
}

char const* c_network_session::get_mode_string() const
{
	static char const* const mode_names[k_network_session_mode_count]
	{
		"none"
		"idle"
		"setup"
		"in-game"
		"end-game"
		"post-game"
		"matchmaking-start"
		"matchmaking-searching"
		"matchmaking-gathering"
		"matchmaking-slave"
		"matchmaking-disbanding"
		"matchmaking-arbitrating"
		"matchmaking-choosing-game"
	};

	long current_mode = m_session_parameters.session_mode.get();
	ASSERT(current_mode >= 0 && current_mode < k_network_session_mode_count);

	return mode_names[current_mode];
}

void c_network_session::handle_disconnection()
{
	DECLFUNC(0x0045C2C0, void, __thiscall, c_network_session*)(this);
}

bool c_network_session::handle_leave_internal(long peer_index)
{
	return DECLFUNC(0x0045C2C0, bool, __thiscall, c_network_session*, long)(this, peer_index);

	//ASSERT(is_host());
	//ASSERT(established());
	//ASSERT(peer_index != NONE);
	//ASSERT(peer_index != m_session_membership.local_peer_index());
	//
	//long observer_channel_index = m_session_membership.get_observer_channel_index(peer_index);
	//if (m_session_class == _network_session_class_xbox_live)
	//	m_observer->quality_statistics_notify_peer_left_gracefully(observer_owner(), observer_channel_index);
	//
	//if (membership_is_locked() || !m_session_membership.is_peer_valid(peer_index))
	//{
	//	// Missing in Halo Online
	//	//if (m_local_state == _network_session_state_host_handoff)
	//	//{
	//	//	handoff_remove_peer_from_consideration(peer_index);
	//	//	return true;
	//	//}
	//	//else if (m_local_state == _network_session_state_host_reestablish)
	//	//{
	//	//	reestablish_remove_peer_from_reestablishment(peer_index);
	//	//	return true;
	//	//}
	//	//else
	//	//{
	//	//	generate_event(_event_level_warning, "networking:session:membership: [%s] leave-request from peer [%s] denied, session membership is locked (state %s)",
	//	//		managed_session_get_id_string(m_managed_session_index),
	//	//		get_peer_description(peer_index),
	//	//		get_state_string());
	//	//}
	//
	//	generate_event(_event_level_warning, "networking:session:membership: [%s] leave-request from peer [%s] denied, session membership is locked (state %s)",
	//		managed_session_get_id_string(m_managed_session_index),
	//		get_peer_description(peer_index),
	//		get_state_string());
	//}
	//else
	//{
	//	generate_event(_event_level_message, "networking:session:membership: %s leave-request accepted for peer [%s]",
	//		managed_session_get_id_string(m_managed_session_index),
	//		get_peer_description(peer_index));
	//
	//	s_network_message_leave_acknowledge leave_acknowledge{};
	//	managed_session_get_id(m_managed_session_index, &leave_acknowledge.session_id);
	//	m_observer->observer_channel_send_message(
	//		observer_owner(),
	//		observer_channel_index,
	//		true,
	//		_network_message_leave_acknowledge,
	//		sizeof(s_network_message_leave_acknowledge),
	//		&leave_acknowledge);
	//
	//	return true;
	//}
	//
	//return false;
}

bool c_network_session::has_managed_session_connection() const
{
	return DECLFUNC(0x0045C3E0, bool, __thiscall, c_network_session const*)(this);
}

bool c_network_session::host_assume_leadership()
{
	return DECLFUNC(0x0045C410, bool, __thiscall, c_network_session*)(this);

	//if (established())
	//{
	//	if (is_host())
	//	{
	//		if (m_session_membership.leader_peer_index() != m_session_membership.local_peer_index())
	//		{
	//			generate_event(_event_level_status, "networking:session:membership: [%s] assuming leadership ([#%d] -> [#%d])",
	//				managed_session_get_id_string(m_managed_session_index),
	//				m_session_membership.leader_peer_index(),
	//				m_session_membership.local_peer_index());
	//
	//			return true;
	//		}
	//	}
	//	else
	//	{
	//		generate_event(_event_level_error, "networking:session:membership: [%s] failed assuming leadership, not host of session",
	//			managed_session_get_id_string(m_managed_session_index));
	//	}
	//}
	//else
	//{
	//	generate_event(_event_level_error, "networking:session:membership: [%s] failed assuming leadership, session not established",
	//		managed_session_get_id_string(m_managed_session_index));
	//}
	//
	//return false;
}

bool c_network_session::host_boot_machine(long peer_index, e_network_session_boot_reason boot_reason)
{
	return DECLFUNC(0x0045C4B0, bool, __thiscall, c_network_session*, long, e_network_session_boot_reason)(this, peer_index, boot_reason);

	//if (is_host())
	//{
	//	generate_event(_event_level_message, "networking:session:membership: [%s] booting machine [#%d] locally [reason %d]",
	//		managed_session_get_id_string(m_managed_session_index),
	//		peer_index,
	//		boot_reason);
	//
	//	if (m_session_membership.local_peer_index() == peer_index)
	//	{
	//		generate_event(_event_level_message, "networking:session:membership: we are the host and are being booted, leaving...");
	//
	//		user_interface_networking_notify_booted_from_session(session_type(), boot_reason);
	//		disband_and_reestablish_as_host(session_type() == _network_session_type_group);
	//	}
	//	else
	//	{
	//		boot_peer(peer_index, boot_reason);
	//	}
	//
	//	return true;
	//}
	//
	//return false;
}

void c_network_session::host_connection_refused(transport_address const* address, e_network_join_refuse_reason refuse_reason)
{
	DECLFUNC(0x0045C530, void, __thiscall, c_network_session*, transport_address const*, e_network_join_refuse_reason)(this, address, refuse_reason);

	//if (waiting_for_host_connection(address))
	//{
	//	if (refuse_reason == _network_join_refuse_reason_holding_in_queue && this->m_local_state == _network_session_state_peer_joining)
	//	{
	//		generate_event(_event_level_message, "networking:session:join: [%s] received join-queue ping from host, waiting",
	//			managed_session_get_id_string(m_managed_session_index));
	//
	//		peer_joining.__unknown34C = network_time_get();
	//	}
	//	else
	//	{
	//		generate_event(_event_level_message, "networking:session:join: [%s] host connection refused, aborting join",
	//			managed_session_get_id_string(m_managed_session_index));
	//
	//		m_join_refuse_reason = refuse_reason;
	//		ASSERT(current_local_state() == _network_session_state_peer_joining);
	//		disconnect();
	//	}
	//}
}

bool c_network_session::host_established() const
{
	return DECLFUNC(0x0045C5C0, bool, __thiscall, c_network_session const*)(this);

	//return current_local_state() == _network_session_state_host_established;
}

bool c_network_session::host_set_player_current_properties(long player_index, struct s_player_configuration const* player_data)
{
	return DECLFUNC(0x0045C5D0, bool, __thiscall, c_network_session*, long, s_player_configuration const*)(this, player_index, player_data);

	//ASSERT(player_index >= 0 && player_index < k_network_maximum_players_per_session);
	//ASSERT(player_data);
	//
	//if (established() && is_host() && !membership_is_locked() )
	//{
	//	if (m_session_membership.is_player_valid(player_index))
	//	{
	//		generate_event(_event_level_status, "networking:session:membership: [%s] local host updating current player properties for player [#%d]",
	//			managed_session_get_id_string(m_managed_session_index),
	//			player_index);
	//
	//		return true;
	//	}
	//	else
	//	{
	//		generate_event(_event_level_status, "networking:session:membership: [%s] local host updating player properties for invalid player [#%d]",
	//			managed_session_get_id_string(m_managed_session_index),
	//			player_index);
	//	}
	//}
	//
	//return false;
}

void c_network_session::idle()
{
	DECLFUNC(0x0045C670, void, __thiscall, c_network_session*)(this);
}

bool c_network_session::initialize_session(long session_index, e_network_session_type session_type, c_network_message_gateway* message_gateway, c_network_observer* observer, c_network_session_manager* session_manager)
{
	return DECLFUNC(0x0045CAB0, bool, __thiscall, c_network_session*, long, e_network_session_type, c_network_message_gateway*, c_network_observer*, c_network_session_manager*)(this, session_index, session_type, message_gateway, observer, session_manager);
}

void c_network_session::initiate_leave_protocol(bool leave_immediately)
{
	DECLFUNC(0x0045CB80, void, __thiscall, c_network_session*, bool)(this, leave_immediately);
}

bool c_network_session::is_peer_joining_this_session() const
{
	return DECLFUNC(0x0045CC20, bool, __thiscall, c_network_session const*)(this);

	//if (established())
	//{
	//	c_network_session_membership const* session_membership = get_session_membership();
	//	for (long peer_index = session_membership->get_first_peer(); peer_index != NONE; peer_index = session_membership->get_next_peer(peer_index))
	//	{
	//		if (peer_index != session_membership->local_peer_index() && session_membership->get_peer_connection_state(peer_index) != _network_session_peer_state_established)
	//			return true;
	//	}
	//}
	//
	//return false;
}

bool c_network_session::join_abort(transport_address const* incoming_address, qword join_nonce)
{
	return DECLFUNC(0x0045CC80, bool, __thiscall, c_network_session*, transport_address const*, qword)(this, incoming_address, join_nonce);
}

bool c_network_session::join_abort_in_progress(transport_address const* address) const
{
	return DECLFUNC(0x0045CCD0, bool, __thiscall, c_network_session const*, transport_address const*)(this, address);
}

void c_network_session::join_abort_successful(transport_address const* address)
{
	DECLFUNC(0x0045CD00, void, __thiscall, c_network_session*, transport_address const*)(this, address);
}

void c_network_session::join_accept(s_network_session_join_request const* join_request, transport_address const* address)
{
	DECLFUNC(0x0045CDA0, void, __thiscall, c_network_session*, s_network_session_join_request const*, transport_address const*)(this, join_request, address);
}

bool c_network_session::join_allowed_by_privacy() const
{
	return DECLFUNC(0x0045D180, bool, __thiscall, c_network_session const*)(this);
}

//.text:0045D1E0 ; c_network_session::join_remote_session

bool c_network_session::leader_request_boot_machine(s_transport_secure_address const* boot_peer_address, e_network_session_boot_reason boot_reason)
{
	return DECLFUNC(0x0045D4A0, bool, __thiscall, c_network_session*, s_transport_secure_address const*, e_network_session_boot_reason)(this, boot_peer_address, boot_reason);
}

bool c_network_session::leader_request_delegate_leadership(s_transport_secure_address const* leader_address)
{
	return DECLFUNC(0x0045D600, bool, __thiscall, c_network_session*, s_transport_secure_address const*)(this, leader_address);
}

e_network_session_mode c_network_session::session_mode() const
{
	return m_session_parameters.session_mode.get();
}

s_network_session_player* c_network_session::get_player(long player_index)
{
	ASSERT(!disconnected());

	return &m_session_membership.m_shared_network_membership.players[player_index];
}

//.text:0045D760 ; void c_network_session::leave_session()
//.text:0045D780 ; bool c_network_session::membership_is_locked() const
//.text:0045D790 ; bool c_network_session::membership_is_stable() const
//.text:0045D8C0 ; virtual void c_network_session::notify_channel_connection(long, dword, bool)
//.text:0045D9E0 ; virtual void c_network_session::notify_channel_died(long)
//.text:0045D9F0 ; bool c_network_session::observing_channel(long) const
//.text:0045DA10 ; void c_network_session::peer_complete_player_add(long, s_player_identifier const*)
//.text:0045DA50 ; bool c_network_session::peer_is_member(s_transport_secure_address const*) const
//.text:0045DA70 ; void c_network_session::peer_request_player_add(long, s_player_identifier const*, long, e_controller_index, s_player_configuration_from_client const*, dword)
//.text:0045DCC0 ; e_network_join_refuse_reason c_network_session::peer_request_player_add_status(long, s_player_identifier const*)

// custom function for `c_network_session::peer_request_player_desired_properties_update`
void update_player_data(s_player_configuration_for_player_properties* player_data)
{
	// #TODO: save `c_player_profile_interface` from a config file
	c_player_profile_interface& player_profile = controller_get(_controller_index0)->m_player_profile;
	s_s3d_player_armor_configuration_loadout& armor_loadout = get_armor_loadout();
	s_s3d_player_weapon_configuration_loadout& weapon_loadout = get_weapon_loadout();

	player_data->host_partial.service_tag = player_profile.desired_service_tag;
	player_data->host_partial.bungienet_user = weapon_loadout.bungienet_user;
	player_data->host_partial.colors = armor_loadout.colors;
	player_data->host_partial.armors = armor_loadout.armors;
	player_data->host_partial.consumables = weapon_loadout.consumables;
}

bool c_network_session::peer_request_player_desired_properties_update(long player_update_number, e_controller_index controller_index, s_player_configuration_from_client const* player_data_from_client, dword player_voice)
{
	//return DECLFUNC(0x0045DD20, bool, __thiscall, c_network_session*, long, e_controller_index, s_player_configuration_from_client const*, dword)(this, player_update_number, controller_index, player_data_from_client, player_voice);

	ASSERT(controller_index >= 0 && controller_index < k_number_of_controllers);
	ASSERT(player_data_from_client);

	if (!established())
		return false;

	s_player_configuration_for_player_properties player_data = { .client = *player_data_from_client };
	update_player_data(&player_data);

	if (is_host())
	{
		ASSERT(m_session_membership.local_peer_index() >= 0 && m_session_membership.local_peer_index() < k_network_maximum_machines_per_session);

		long player_index = m_session_membership.get_player_index_from_peer(m_session_membership.local_peer_index());
		if (player_index == NONE)
		{
			generate_event(_event_level_error, "networking:session:membership: [%s] local host requested player-properties does not exist",
				managed_session_get_id_string(m_managed_session_index));

			return false;
		}

		s_network_session_player* player = get_player(player_index);

		generate_event(_event_level_status, "networking:session:membership: [%s] local host applying player-properties for player [#%d]",
			managed_session_get_id_string(m_managed_session_index),
			player_index);

		ASSERT(player->peer_index == m_session_membership.local_peer_index());
		m_session_membership.set_player_properties(
			player_index,
			player_update_number,
			controller_index,
			&player_data,
			player_voice);
	}
	else
	{
		generate_event(_event_level_status, "networking:session:membership: [%s] sending player-properties request",
			managed_session_get_id_string(m_managed_session_index));

		s_network_message_player_properties message{};
		csmemset(&message, 0, sizeof(s_network_message_player_properties));

		managed_session_get_id(m_managed_session_index, &message.session_id);
		message.player_update_number = player_update_number;
		message.controller_index = controller_index;
		message.player_data = player_data;
		message.player_voice = player_voice;

		long observer_channel_index = m_session_membership.m_local_peers[m_session_membership.host_peer_index()].channel_index;
		m_observer->observer_channel_send_message(m_session_index, observer_channel_index, false, _network_message_player_properties, sizeof(message), &message);
	}

	return true;
}

//.text:0045DEB0 ; bool c_network_session::peer_request_player_remove(long)
//.text:0045DFE0 ; bool c_network_session::peer_request_properties_update(s_transport_secure_address const*, s_network_session_peer_properties const*)
//.text:0045E110 ; bool c_network_session::player_is_member(s_player_identifier const*) const
//.text:0045E130 ; 
//.text:0045E140 ; void c_network_session::process_pending_joins()

e_network_session_class c_network_session::session_class() const
{
	//return DECLFUNC(0x0045E910, e_network_session_class, __thiscall, c_network_session const*)(this);

	return m_session_class;
}

//.text:0045E920 ; bool c_network_session::ready_for_remote_peers_to_join() const
//.text:0045EA40 ; void c_network_session::reset_connection_state()
//.text:0045EA70 ; void c_network_session::reset_local_host_state(bool)
//.text:0045EAA0 ; void c_network_session::send_message_to_all_peers(c_network_session::e_session_message_transmission_type, e_network_message_type, long, void const*) const
//.text:0045EB50 ; void c_network_session::send_message_to_peer(c_network_session::e_session_message_transmission_type, long, e_network_message_type, long, void const*) const
//.text:0045EBD0 ; bool c_network_session::session_is_full(long, long) const
//.text:0045EC20 ; e_network_session_mode c_network_session::session_mode() const
//.text:0045ED30 ; void c_network_session::set_default_session_parameters()
//.text:0045EDC0 ; void c_network_session::set_disconnection_policy(e_network_session_disconnection_policy)
//.text:0045EDF0 ; 
//.text:0045EE40 ; bool c_network_session::set_privacy_details(e_network_session_privacy, e_network_session_closed_status, long, long)
//.text:0045EFD0 ; dword c_network_session::time_get() const
//.text:0045EFE0 ; void c_network_session::time_release()
//.text:0045EFF0 ; void c_network_session::time_set(long)

bool c_network_session::waiting_for_host_connection(transport_address const* address) const
{
	return DECLFUNC(0x0045F030, bool, __thiscall, c_network_session const*, transport_address const*)(this, address);
}

//.text:0045F060 ; bool c_network_session::waiting_for_initial_update() const
//.text:0045F080 ; void c_network_session::whack_session_to_offline_session_class()

bool c_network_session::handle_boot_machine(c_network_channel* channel, s_network_message_boot_machine const* message)
{
	return DECLFUNC(0x004DA040, bool, __thiscall, c_network_session*, c_network_channel* channel, s_network_message_boot_machine const*)(this, channel, message);
}

bool c_network_session::handle_delegate_leadership(c_network_channel* channel, s_network_message_delegate_leadership const* message)
{
	return DECLFUNC(0x004DA1B0, bool, __thiscall, c_network_session*, c_network_channel* channel, s_network_message_delegate_leadership const*)(this, channel, message);
}

bool c_network_session::handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message)
{
	return DECLFUNC(0x004DA300, bool, __thiscall, c_network_session*, c_network_channel* channel, s_network_message_host_decline const*)(this, channel, message);
}

bool c_network_session::handle_join_request(transport_address const* address, s_network_message_join_request const* message)
{
	return DECLFUNC(0x004DA410, bool, __thiscall, c_network_session*, transport_address const* channel, s_network_message_join_request const*)(this, address, message);
}

void c_network_session::handle_leave_acknowledgement(transport_address const* address)
{
	DECLFUNC(0x004DA540, void, __thiscall, c_network_session*, transport_address const*)(this, address);
}

bool c_network_session::handle_leave_request(transport_address const* address)
{
	return DECLFUNC(0x004DA5F0, bool, __thiscall, c_network_session*, transport_address const*)(this, address);
}

bool c_network_session::handle_membership_update(s_network_message_membership_update const* message)
{
	return DECLFUNC(0x004DA680, bool, __thiscall, c_network_session*, s_network_message_membership_update const*)(this, message);
}

bool c_network_session::handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message)
{
	return DECLFUNC(0x004DA690, bool, __thiscall, c_network_session*, c_network_channel* channel, s_network_message_parameters_request const*)(this, channel, message);
}

bool c_network_session::handle_parameters_update(s_network_message_parameters_update const* message)
{
	return DECLFUNC(0x004DA770, bool, __thiscall, c_network_session*, s_network_message_parameters_update const*)(this, message);

	//ASSERT(message);
	//
	//if (is_host())
	//{
	//	generate_event(_event_level_error, "networking:session:parameters: [%s] parameters-update received and we are the host",
	//		managed_session_get_id_string(m_managed_session_index));
	//
	//	return false;
	//}
	//
	//if ((established() || peer_joining()) && !leaving_session())
	//{
	//	if (m_session_parameters.handle_update(message))
	//		return true;
	//
	//	generate_event(_event_level_error, "networking:session:parameters: [%s] failed to handle parameters-update, we must disconnect [%s]",
	//		managed_session_get_id_string(m_managed_session_index),
	//		get_state_string());
	//
	//	handle_disconnection();
	//	return false;
	//}
	//
	//if (leaving_session())
	//{
	//	generate_event(_event_level_message, "networking:session:parameters: [%s] parameters-update received but we're leaving [%s], ignoring the update",
	//		managed_session_get_id_string(m_managed_session_index),
	//		get_state_string());
	//
	//	return false;
	//}
	//
	//generate_event(_event_level_message, "networking:session:parameters: [%s] parameters-update received and we are in a bad state to handle the update [%s]",
	//	managed_session_get_id_string(m_managed_session_index),
	//	get_state_string());
	//
	//return false;
}

void c_network_session::handle_peer_connect(transport_address const* address, s_network_message_peer_connect const* message)
{
	DECLFUNC(0x004DA870, void, __thiscall, c_network_session*, transport_address const*, s_network_message_peer_connect const*)(this, address, message);
}

bool c_network_session::handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message)
{
	return DECLFUNC(0x004DAAB0, bool, __thiscall, c_network_session*, c_network_channel* channel, s_network_message_peer_establish const*)(this, channel, message);
}

bool c_network_session::handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message)
{
	return DECLFUNC(0x004DAC30, bool, __thiscall, c_network_session*, c_network_channel* channel, s_network_message_peer_properties const*)(this, channel, message);
}

bool c_network_session::handle_player_add(c_network_channel* channel, s_network_message_player_add const* message)
{
	return DECLFUNC(0x004DAD20, bool, __thiscall, c_network_session*, c_network_channel* channel, s_network_message_player_add const*)(this, channel, message);
}

bool c_network_session::handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message)
{
	//return DECLFUNC(0x004DAEC0, bool, __thiscall, c_network_session*, c_network_channel*, s_network_message_player_properties const*)(this, channel, message);

	if (established() && is_host())
	{
		long observer_channel_index = m_observer->observer_channel_find_by_network_channel(m_session_index, channel);
		long peer_index = m_session_membership.get_peer_from_observer_channel(observer_channel_index);
		if (peer_index == NONE || peer_index == m_session_membership.local_peer_index())
		{
			generate_event(_event_level_message, "networking:session:membership: [%s] player-properties received from invalid peer [#%d]",
				managed_session_get_id_string(m_managed_session_index),
				peer_index);
		}
		//else if (message->user_index < 0 || message->user_index >= 4)
		//{
		//	generate_event(_event_level_error, "networking:session:membership: [%s] player-properties received for invalid user_index [#%d]",
		//		managed_session_get_id_string(m_managed_session_index),
		//		message->user_index);
		//}
		else
		{
			long player_index = m_session_membership.get_player_index_from_peer(peer_index);
			if (player_index == NONE)
			{
				generate_event(_event_level_warning, "networking:session:membership: [%s] player-properties received but no player associated with peer [#%d]",
					managed_session_get_id_string(m_managed_session_index),
					peer_index);
			}
			else
			{
				m_session_membership.set_player_properties(player_index,
					message->player_update_number,
					message->controller_index,
					&message->player_data,
					message->player_voice);

				generate_event(_event_level_status, "networking:session:membership: [%s] player-properties accepted for peer/player [#%d]/[#%d]",
					managed_session_get_id_string(m_managed_session_index),
					peer_index,
					peer_index);
			}

			return true;
		}
	}
	else
	{
		generate_event(_event_level_warning, "networking:session:membership: [%s] player-properties received but not host, can't update players",
			managed_session_get_id_string(m_managed_session_index));
	}

	return false;
}

bool c_network_session::handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message)
{
	return DECLFUNC(0x004DB000, bool, __thiscall, c_network_session*, c_network_channel*, s_network_message_player_refuse const*)(this, channel, message);
}

bool c_network_session::handle_player_remove(c_network_channel* channel, s_network_message_player_remove const* message)
{
	return DECLFUNC(0x004DB0E0, bool, __thiscall, c_network_session*, c_network_channel*, s_network_message_player_remove const*)(this, channel, message);
}

bool c_network_session::handle_session_boot(transport_address const* address, s_network_message_session_boot const* message)
{
	return DECLFUNC(0x004DB200, bool, __thiscall, c_network_session*, transport_address const*, s_network_message_session_boot const*)(this, address, message);
}

bool c_network_session::handle_session_disband(transport_address const* address, s_network_message_session_disband const* message)
{
	return DECLFUNC(0x004DB2B0, bool, __thiscall, c_network_session*, transport_address const*, s_network_message_session_disband const*)(this, address, message);
}

bool c_network_session::handle_time_synchronize(transport_address const* address, s_network_message_time_synchronize const* message)
{
	return DECLFUNC(0x004DB340, bool, __thiscall, c_network_session*, transport_address const*, s_network_message_time_synchronize const*)(this, address, message);
}

