#include "networking/session/network_session.hpp"

#include "cseries/cseries_events.hpp"
#include "hf2p/hf2p.hpp"
#include "interface/c_controller.hpp"
#include "memory/module.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/messages/network_messages_session_membership.hpp"
#include "networking/network_time.hpp"
#include "networking/session/network_managed_session.hpp"
#include "networking/session/network_observer.hpp"
#include "networking/session/network_session_manager.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x0045DD20, c_network_session, peer_request_player_desired_properties_update_);

bool c_network_session::peer_request_player_desired_properties_update_(int32 player_update_number, e_controller_index controller_index, const s_player_configuration_from_client* player_data_from_client, uns32 player_voice_settings)
{
	return c_network_session::peer_request_player_desired_properties_update(player_update_number, controller_index, player_data_from_client, player_voice_settings);
}

int32 c_network_session::current_local_state() const
{
	//return INVOKE_CLASS_MEMBER(0x00434820, c_network_session, current_local_state);

	return m_local_state;
}

bool c_network_session::disconnected() const
{
	//return INVOKE_CLASS_MEMBER(0x00434830, c_network_session, disconnected);

	return c_network_session::current_local_state() == _network_session_state_none;
}

bool c_network_session::established() const
{
	return c_network_session::current_local_state() >= _network_session_state_peer_established;
}

bool c_network_session::is_host() const
{
	//return INVOKE_CLASS_MEMBER(0x00434D90, c_network_session, is_host);

	int32 current_state = c_network_session::current_local_state();
	if (current_state == _network_session_state_host_established
		|| current_state == _network_session_state_host_disband
		/*|| current_state == _network_session_state_host_handoff
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
	//return INVOKE_CLASS_MEMBER(0x00434DB0, c_network_session, is_leader);

	return m_session_membership.is_leader();
}

bool c_network_session::leaving_session() const
{
	return INVOKE_CLASS_MEMBER(0x00434E30, c_network_session, leaving_session);
}

bool c_network_session::peer_joining() const
{
	return INVOKE_CLASS_MEMBER(0x0044FD10, c_network_session, peer_joining);
}

bool c_network_session_membership::peer_property_flag_test(e_peer_property_flag_test_type test_type, e_network_session_peer_properties_status_flags flag) const
{
	return INVOKE_CLASS_MEMBER(0x0044FD20, c_network_session_membership, peer_property_flag_test, test_type, flag);
}

bool c_network_session_membership::peer_property_flag_test_any_peer(e_network_session_peer_properties_status_flags flag) const
{
	return INVOKE_CLASS_MEMBER(0x0044FDA0, c_network_session_membership, peer_property_flag_test_any_peer, flag);
}

//.text:0045A0A0 ; private: void c_network_session::abort_pending_join(uns64)
//.text:0045A190 ; 
//.text:0045A230 ; public: void c_network_session::acknowledge_join_request(const transport_address*, e_network_join_refuse_reason)
//.text:0045A2B0 ; private: void c_network_session::add_pending_join_to_session(uns64)
//.text:0045A390 ; 
//.text:0045A3D0 ; public: bool c_network_session_membership::all_peers_established() const
//.text:0045A410 ; public: bool c_network_session::all_players_are_present() const
//.text:0045A430 ; public: virtual bool c_network_session::attempt_channel_reconnection(int32, bool) const
//.text:0045A4C0 ; private: void c_network_session::boot_peer(int32, e_network_session_boot_reason)
//.text:0045A5B0 ; public: e_network_join_refuse_reason c_network_session::can_accept_any_join_request() const
//.text:0045A650 ; public: e_network_join_refuse_reason c_network_session::can_accept_join_request(const s_network_session_join_request*) const
//.text:0045A7D0 ; public: e_network_join_refuse_reason c_network_session::can_accept_player_join_request(const s_player_identifier*, int32, int32, bool, bool) const
//.text:0045A9C0 ; public: virtual e_network_channel_owner_state c_network_session::channel_get_state(int32) const

bool c_network_session::channel_is_authoritative(c_network_channel* channel)
{
	return INVOKE_CLASS_MEMBER(0x0045A9E0, c_network_session, channel_is_authoritative, channel);
}

//.text:0045AA40 ; private: void c_network_session::check_to_send_membership_update()
//.text:0045AD50 ; private: void c_network_session::check_to_send_peer_join_abort()
//.text:0045AE10 ; private: void c_network_session::check_to_send_peer_join_request()
//.text:0045B000 ; private: void c_network_session::check_to_send_peer_leave_request()
//.text:0045B0F0 ; private: void c_network_session::check_to_send_time_synchronization()
//.text:0045B240 ; 
//.text:0045B250 ; public: bool c_network_session::compare_session_id(const s_transport_secure_identifier*) const
//.text:0045B270 ; public: bool c_network_session::connect_peer_to_remote_session(c_session_creation_flags, e_network_session_class, e_transport_platform, const s_transport_secure_identifier*, const s_transport_secure_key*, const s_transport_secure_address*, uns64, const s_online_context*, int32)
//.text:0045B310 ; public: bool c_network_session::create_host_session(c_session_creation_flags, e_network_session_class, e_transport_platform, const c_network_session_membership*, const s_online_context*, int32)
//.text:0045B6D0 ; private: bool c_network_session::create_session_for_remote_join(c_session_creation_flags, e_network_session_class, e_transport_platform, const s_transport_secure_identifier*, const s_transport_secure_key*, const s_transport_secure_address*, bool, uns64, s_network_session_join_request*, const s_online_context*, int32)
//.text:0045B910 ; public: virtual bool c_network_session::desire_channel_heartbeat(int32) const

void c_network_session::destroy_session()
{
	INVOKE_CLASS_MEMBER(0x0045B960, c_network_session, destroy_session);

	//if (m_session_manager)
	//{
	//	m_session_manager->deregister_session(this);
	//}
	//
	//if (m_local_state == _network_session_state_none)
	//{
	//	m_disconnection_policy = _network_session_disconnection_allow;
	//	c_network_session::disconnect();
	//	ASSERT(m_local_state == _network_session_state_none);
	//}
	//
	//m_local_state = k_network_session_state_none;
	//m_session_class = k_network_session_class_none;
	//m_session_type = _network_session_type_none;
	//m_message_gateway = nullptr;
	//m_observer = nullptr;
}

//.text:0045B9D0 ; public: void c_network_session::disband_and_reestablish_as_host(e_disband_and_reestablish_policy)
//.text:0045BAB0 ; private: void c_network_session::disband_peer(int32)

void c_network_session::disconnect()
{
	INVOKE_CLASS_MEMBER(0x0045BBB0, c_network_session, disconnect);
}

//.text:0045BC60 ; private: void c_network_session::finalize_single_player_add(e_network_join_refuse_reason)
//.text:0045BDF0 ; public: int32 c_network_session::find_peer_by_machine_identifier(const s_machine_identifier*) const
//.text:0045BE00 ; 
//.text:0045BE10 ; 

void c_network_session::force_disconnect()
{
	INVOKE_CLASS_MEMBER(0x0045BE20, c_network_session, force_disconnect);
}

bool c_network_session::force_disconnect_peer(const s_transport_secure_address* peer_secure_address)
{
	return INVOKE_CLASS_MEMBER(0x0045BE80, c_network_session, force_disconnect_peer, peer_secure_address);
}

//.text:0045BF30 ; 
//.text:0045BF40 ; 
//.text:0045BF50 ; 

const char* c_network_session::get_disconnection_policy_string() const
{
	ASSERT(VALID_INDEX(m_disconnection_policy, k_network_session_disconnection_policy_count));

	return g_session_disconnection_policy_strings[m_disconnection_policy];
}

//.text:0045BF60 ; public: int32 c_network_session_membership::get_host_observer_channel_index() const
//.text:0045BF70 ; public: const s_network_session_peer* c_network_session_membership::get_host_peer() const
//.text:0045BF80 ; public: const char* c_network_session::get_id_string() const
//.text:0045BF90 ; public: uns64 c_network_session_membership::get_join_nonce(int32) const
//.text:0045BFB0 ; public: const s_network_session_peer* c_network_session_membership::get_leader_peer() const
//.text:0045BFD0 ; public: int32 c_network_session::get_local_session_membership_update_number() const
//.text:0045BFE0 ; public: int32 c_network_session::get_maximum_player_count() const
//.text:0045C060 ; public: int32 c_network_session_membership::get_membership_update_number(int32) const
//.text:0045C080 ; 
//.text:0045C090 ; private: s_network_session_peer* c_network_session::get_peer(int32)
//.text:0045C0B0 ; private: const s_network_session_peer* c_network_session::get_peer(int32) const
//.text:0045C0D0 ; public: const char* c_network_session::get_peer_description(int23) const
//.text:0045C0E0 ; public: bool c_network_session_membership::get_peer_needs_reestablishment(int32) const

int32 c_network_session::get_peer_observer_channel(int32 peer_index) const
{
	//return INVOKE_CLASS_MEMBER(0x0045C100, c_network_session, get_peer_observer_channel, peer_index);

	ASSERT(established());

	if (peer_index == NONE || !m_session_membership.is_peer_valid(peer_index))
	{
		return NONE;
	}

	return m_session_membership.get_observer_channel_index(peer_index);
}

//.text:0045C140 ; private: s_network_session_player* c_network_session::get_player(int32)
//.text:0045C160 ; private: const s_network_session_player* c_network_session::get_player(int32) const
//.text:0045C180 ; public: int32 c_network_session::get_player_space_available() const
//.text:0045C190 ; public: const s_network_session_privacy_mode* c_generic_network_session_parameter<s_network_session_privacy_mode>::get_request() const
//.text:0045C1B0 ; public: bool c_network_session::get_secure_key(s_transport_secure_identifier*, s_transport_secure_key*, e_transport_platform*) const
//.text:0045C230 ; public: bool c_network_session::get_session_id(s_transport_secure_identifier*) const

const c_network_session_membership* c_network_session::get_session_membership() const
{
	return INVOKE_CLASS_MEMBER(0x0045C250, c_network_session, get_session_membership);

	//ASSERT(established());
	//ASSERT(m_session_membership.has_membership());
	//ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
	//ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));
	//
	//return &m_session_membership;
}

c_network_session_membership* c_network_session::get_session_membership_for_update()
{
	//return INVOKE_CLASS_MEMBER(0x0045C260, c_network_session, get_session_membership_for_update);

	ASSERT(established());
	ASSERT(is_host());
	ASSERT(m_session_membership.has_membership());
	ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
	ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));

	return &m_session_membership;
}

const c_network_session_membership* c_network_session::get_session_membership_unsafe() const
{
	//return INVOKE_CLASS_MEMBER(0x0045C270, c_network_session, get_session_membership_unsafe);

	if (c_network_session::disconnected() || !m_session_membership.has_membership())
	{
		return nullptr;
	}

	ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
	ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));

	return &m_session_membership;
}

int32 c_network_session::get_session_membership_update_number() const
{
	return INVOKE_CLASS_MEMBER(0x0045C290, c_network_session, get_session_membership_update_number);

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

const c_network_session_parameters* c_network_session::get_session_parameters() const
{
	//return DECLFUNC(0x0045C2B0, const c_network_session_parameters*, __thiscall, const c_network_session*)(this);

	return &m_session_parameters;
}

const char* c_network_session::get_type_string(e_network_session_type type)
{
	static const char* const type_names[k_network_session_type_count]
	{
		"none",
		"squad",
		"group"
	};

	if (VALID_INDEX(type, k_network_session_type_count))
		return type_names[type];

	return "<unknown>";
}

const char* __cdecl c_network_session::get_class_string(e_network_session_class session_class)
{
	static const char* const session_class_names[k_network_session_class_count]
	{
		"offline",
		"system-link",
		"xbox-live"
	};

	if (VALID_INDEX(session_class, k_network_session_class_count))
		return session_class_names[session_class];

	return "<unknown>";
}

const char* c_network_session::get_state_string() const
{
	static const char* const state_names[k_network_session_state_count]
	{
		"none",
		"peer-creating",
		"peer-joining",
		"peer-join-abort",
		"peer-established",
		"peer-leaving",
		"host-established",
		"host-disband",

		//"host-handoff",
		//"host-reestablish",
		//"election"
	};

	int32 current_state = c_network_session::current_local_state();
	ASSERT(current_state >= 0 && current_state < k_network_session_state_count);

	return state_names[current_state];
}

const char* c_network_session::get_mode_string() const
{
	static const char* const mode_names[k_network_session_mode_count]
	{
		"none",
		"idle",
		"setup",
		"in-game",
		"end-game",
		"post-game",
		"matchmaking-start",
		"matchmaking-searching",
		"matchmaking-gathering",
		"matchmaking-slave",
		"matchmaking-disbanding",
		"matchmaking-arbitrating",
		"matchmaking-choosing-game",
	};

	int32 current_mode = m_session_parameters.m_parameters.session_mode.get();
	ASSERT(current_mode >= 0 && current_mode < k_network_session_mode_count);

	return mode_names[current_mode];
}

void c_network_session::handle_disconnection()
{
	INVOKE_CLASS_MEMBER(0x0045C2C0, c_network_session, handle_disconnection);
}

bool c_network_session::handle_leave_internal(int32 peer_index)
{
	return INVOKE_CLASS_MEMBER(0x0045C2C0, c_network_session, handle_leave_internal, peer_index);

	//ASSERT(is_host());
	//ASSERT(established());
	//ASSERT(peer_index != NONE);
	//ASSERT(peer_index != m_session_membership.local_peer_index());
	//
	//int32 observer_channel_index = m_session_membership.get_observer_channel_index(peer_index);
	//if (m_session_class == _network_session_class_xbox_live)
	//{
	//	m_observer->quality_statistics_notify_peer_left_gracefully(observer_owner(), observer_channel_index);
	//}
	//
	//if (c_network_session::membership_is_locked() || !m_session_membership.is_peer_valid(peer_index))
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
	//	//	event(_event_warning, "networking:session:membership: [%s] leave-request from peer [%s] denied, session membership is locked (state %s)",
	//	//		managed_session_get_id_string(m_managed_session_index),
	//	//		get_peer_description(peer_index),
	//	//		get_state_string());
	//	//}
	//
	//	event(_event_warning, "networking:session:membership: [%s] leave-request from peer [%s] denied, session membership is locked (state %s)",
	//		managed_session_get_id_string(m_managed_session_index),
	//		get_peer_description(peer_index),
	//		c_network_session::get_state_string());
	//}
	//else
	//{
	//	event(_event_message, "networking:session:membership: %s leave-request accepted for peer [%s]",
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
	return INVOKE_CLASS_MEMBER(0x0045C3E0, c_network_session, has_managed_session_connection);

	//return managed_session_get_status(m_managed_session_index).test(_managed_session_creation_succeeded_bit);
}

bool c_network_session::host_assume_leadership()
{
	return INVOKE_CLASS_MEMBER(0x0045C410, c_network_session, host_assume_leadership);

	//if (c_network_session::established())
	//{
	//	if (c_network_session::is_host())
	//	{
	//		if (m_session_membership.leader_peer_index() != m_session_membership.local_peer_index())
	//		{
	//			event(_event_status, "networking:session:membership: [%s] assuming leadership ([#%d] -> [#%d])",
	//				managed_session_get_id_string(m_managed_session_index),
	//				m_session_membership.leader_peer_index(),
	//				m_session_membership.local_peer_index());
	//
	//			return true;
	//		}
	//	}
	//	else
	//	{
	//		event(_event_error, "networking:session:membership: [%s] failed assuming leadership, not host of session",
	//			managed_session_get_id_string(m_managed_session_index));
	//	}
	//}
	//else
	//{
	//	event(_event_error, "networking:session:membership: [%s] failed assuming leadership, session not established",
	//		managed_session_get_id_string(m_managed_session_index));
	//}
	//
	//return false;
}

bool c_network_session::host_boot_machine(int32 peer_index, e_network_session_boot_reason boot_reason)
{
	return INVOKE_CLASS_MEMBER(0x0045C4B0, c_network_session, host_boot_machine, peer_index, boot_reason);

	//if (c_network_session::is_host())
	//{
	//	event(_event_message, "networking:session:membership: [%s] booting machine [#%d] locally [reason %d]",
	//		managed_session_get_id_string(m_managed_session_index),
	//		peer_index,
	//		boot_reason);
	//
	//	if (m_session_membership.local_peer_index() == peer_index)
	//	{
	//		event(_event_message, "networking:session:membership: we are the host and are being booted, leaving...");
	//
	//		user_interface_networking_notify_booted_from_session(session_type(), boot_reason);
	//		c_network_session::initiate_leave_protocol(false);
	//	}
	//	else
	//	{
	//		c_network_session::boot_peer(peer_index, boot_reason);
	//	}
	//
	//	return true;
	//}
	//
	//return false;
}

void c_network_session::host_connection_refused(const transport_address* address, e_network_join_refuse_reason refuse_reason)
{
	INVOKE_CLASS_MEMBER(0x0045C530, c_network_session, host_connection_refused, address, refuse_reason);

	//if (!c_network_session::waiting_for_host_connection(address))
	//{
	//	return;
	//}
	//
	//if (refuse_reason == _network_join_refuse_reason_holding_in_queue && m_local_state == _network_session_state_peer_joining)
	//{
	//	event(_event_message, "networking:session:join: [%s] received join-queue ping from host, waiting",
	//		managed_session_get_id_string(m_managed_session_index));
	//
	//	m_local_state_data.peer_joining.join_ping_from_host_timestamp = network_time_get();
	//}
	//else
	//{
	//	event(_event_message, "networking:session:join: [%s] host connection refused, aborting join",
	//		managed_session_get_id_string(m_managed_session_index));
	//
	//	m_join_refuse_reason = refuse_reason;
	//	ASSERT(current_local_state() == _network_session_state_peer_joining);
	//	c_network_session::disconnect();
	//}
}

bool c_network_session::host_established() const
{
	return INVOKE_CLASS_MEMBER(0x0045C5C0, c_network_session, host_established);

	//return c_network_session::current_local_state() == _network_session_state_host_established;
}

bool c_network_session::host_set_player_current_properties(int32 player_index, const s_player_configuration* player_data)
{
	return INVOKE_CLASS_MEMBER(0x0045C5D0, c_network_session, host_set_player_current_properties, player_index, player_data);

	//ASSERT(player_index >= 0 && player_index < k_network_maximum_players_per_session);
	//ASSERT(player_data);
	//
	//if (!c_network_session::established() || !c_network_session::is_host() || c_network_session::membership_is_locked())
	//{
	//	return false;
	//}
	//
	//if (!m_session_membership.is_player_valid(player_index))
	//{
	//	event(_event_status, "networking:session:membership: [%s] local host updating player properties for invalid player [#%d]",
	//		managed_session_get_id_string(m_managed_session_index),
	//		player_index);
	//
	//	return false;
	//}
	//
	//event(_event_status, "networking:session:membership: [%s] local host updating current player properties for player [#%d]",
	//	managed_session_get_id_string(m_managed_session_index),
	//	player_index);
	//
	//return true;
}

void c_network_session::idle()
{
	INVOKE_CLASS_MEMBER(0x0045C670, c_network_session, idle);
}

bool c_network_session::initialize_session(int32 session_index, e_network_session_type session_type, c_network_message_gateway* message_gateway, c_network_observer* observer, c_network_session_manager* session_manager)
{
	return INVOKE_CLASS_MEMBER(0x0045CAB0, c_network_session, initialize_session, session_index, session_type, message_gateway, observer, session_manager);

	//ASSERT(m_session_type == _network_session_type_none);
	//ASSERT(m_local_state == NONE);
	//ASSERT(session_type >= 0 && session_type < k_network_session_type_count);
	//m_session_type = session_type;
	//m_session_index = session_index;
	//
	//ASSERT(message_gateway);
	//ASSERT(session_manager);
	//ASSERT(observer);
	//m_message_gateway = message_gateway;
	//m_observer = observer;
	//m_session_manager = session_manager;
	//session_manager->register_session(this);
	//
	//ASSERT(m_session_index >= 0 && m_session_index < k_network_maximum_sessions);
	//observer->register_owner((e_network_observer_owner)m_session_index, this);
	//
	//m_session_class = k_network_session_class_none;
	//m_session_membership.m_session = this;
	//
	//m_session_membership.reset_membership();
	////m_shadow_session_membership.reset_membership();
	//m_session_parameters.initialize(this, m_observer);
	//
	//m_local_last_time_synchronize_success_timestamp = NONE;
	//m_local_last_time_synchronize_sent_timestamp = NONE;
	//m_managed_session_index = NONE;
	//m_local_state = _network_session_state_none;
	//m_local_connection_identifier = 0;
	//m_time_exists = false;
	//m_only_use_squad_session_for_election = false;
	//m_disconnection_policy = _network_session_disconnection_allow_waiting_for_establishment;
	//
	//return true;
}

void c_network_session::initiate_leave_protocol(bool leave_immediately)
{
	return INVOKE_CLASS_MEMBER(0x0045CB80, c_network_session, initiate_leave_protocol, leave_immediately);
}

bool c_network_session::is_peer_joining_this_session() const
{
	return INVOKE_CLASS_MEMBER(0x0045CC20, c_network_session, is_peer_joining_this_session);

	//if (c_network_session::established())
	//{
	//	const c_network_session_membership* session_membership = c_network_session::get_session_membership();
	//	for (int32 peer_index = session_membership->get_first_peer(); peer_index != NONE; peer_index = session_membership->get_next_peer(peer_index))
	//	{
	//		if (peer_index != session_membership->local_peer_index() && session_membership->get_peer_connection_state(peer_index) != _network_session_peer_state_established)
	//		{
	//			return true;
	//		}
	//	}
	//}
	//
	//return false;
}

bool c_network_session::join_abort(const transport_address* incoming_address, uns64 join_nonce)
{
	return INVOKE_CLASS_MEMBER(0x0045CC80, c_network_session, join_abort, incoming_address, join_nonce);
}

bool c_network_session::join_abort_in_progress(const transport_address* address) const
{
	return INVOKE_CLASS_MEMBER(0x0045CCD0, c_network_session, join_abort_in_progress, address);
}

void c_network_session::join_abort_successful(const transport_address* address)
{
	return INVOKE_CLASS_MEMBER(0x0045CD00, c_network_session, join_abort_successful, address);
}

void c_network_session::join_accept(const s_network_session_join_request* join_request, const transport_address* address)
{
	return INVOKE_CLASS_MEMBER(0x0045CDA0, c_network_session, join_accept, join_request, address);
}

bool c_network_session::join_allowed_by_privacy() const
{
	return INVOKE_CLASS_MEMBER(0x0045D180, c_network_session, join_allowed_by_privacy);
}

//.text:0045D1E0 ; c_network_session::join_remote_session

bool c_network_session::leader_request_boot_machine(const s_transport_secure_address* boot_peer_address, e_network_session_boot_reason boot_reason)
{
	return INVOKE_CLASS_MEMBER(0x0045D4A0, c_network_session, leader_request_boot_machine, boot_peer_address, boot_reason);
}

bool c_network_session::leader_request_delegate_leadership(const s_transport_secure_address* leader_address)
{
	return INVOKE_CLASS_MEMBER(0x0045D600, c_network_session, leader_request_delegate_leadership, leader_address);
}

e_network_session_mode c_network_session::session_mode() const
{
	return m_session_parameters.m_parameters.session_mode.get();
}

e_network_session_type c_network_session::session_type() const
{
	ASSERT(m_session_type >= 0 && m_session_type < k_network_session_type_count);

	return m_session_type;
}

s_network_session_player* c_network_session::get_player(int32 player_index)
{
	ASSERT(!disconnected());

	return &m_session_membership.m_shared_network_membership.players[player_index];
}

void c_network_session::leave_session()
{
	//INVOKE_CLASS_MEMBER(0x0045D760, c_network_session, leave_session);

	c_network_session::initiate_leave_protocol(false);
}

bool c_network_session::membership_is_locked() const
{
	return INVOKE_CLASS_MEMBER(0x0045D780, c_network_session, membership_is_locked);
}

//.text:0045D790 ; bool c_network_session::membership_is_stable() const
//.text:0045D8C0 ; virtual void c_network_session::notify_channel_connection(int32, uns32, bool)
//.text:0045D9E0 ; virtual void c_network_session::notify_channel_died(int32)
//.text:0045D9F0 ; bool c_network_session::observing_channel(int32) const
//.text:0045DA10 ; void c_network_session::peer_complete_player_add(int32, const s_player_identifier*)
//.text:0045DA50 ; bool c_network_session::peer_is_member(const s_transport_secure_address*) const
//.text:0045DA70 ; void c_network_session::peer_request_player_add(int32, const s_player_identifier*, int32, e_controller_index, const s_player_configuration_from_client*, uns32)
//.text:0045DCC0 ; e_network_join_refuse_reason c_network_session::peer_request_player_add_status(int32, const s_player_identifier*)

// custom function for `c_network_session::peer_request_player_desired_properties_update`
void update_player_data(s_player_configuration_for_player_properties* player_data)
{
	// $TODO save `c_player_profile_interface` from a config file
	c_player_profile_interface& player_profile = controller_get(_controller0)->m_player_profile;
	s_s3d_player_armor_configuration_loadout& armor_loadout = get_armor_loadout();
	s_s3d_player_weapon_configuration_loadout& weapon_loadout = get_weapon_loadout();

	player_data->host_partial.service_tag = player_profile.m_appearance.desired_service_tag;
	player_data->host_partial.bungienet_user = weapon_loadout.bungienet_user;
	player_data->host_partial.colors = armor_loadout.colors;
	player_data->host_partial.armors = armor_loadout.armors;
	player_data->host_partial.consumables = weapon_loadout.consumables;
	player_data->host_partial.emblem = player_profile.get_emblem_info();
}

bool c_network_session::peer_request_player_desired_properties_update(int32 player_update_number, e_controller_index controller_index, const s_player_configuration_from_client* player_data_from_client, uns32 player_voice_settings)
{
	//return INVOKE_CLASS_MEMBER(0x0045DD20, c_network_session, peer_request_player_desired_properties_update, player_update_number, controller_index, player_data_from_client, player_voice_settings);

	ASSERT(controller_index >= 0 && controller_index < k_number_of_controllers);
	ASSERT(player_data_from_client);

	if (!c_network_session::established())
	{
		return false;
	}

	s_player_configuration_for_player_properties player_data = { .client = *player_data_from_client };
	update_player_data(&player_data);

	if (c_network_session::is_host())
	{
		ASSERT(VALID_INDEX(m_session_membership.local_peer_index(), k_network_maximum_machines_per_session));

		int32 player_index = m_session_membership.get_player_index_from_peer(m_session_membership.local_peer_index());
		if (player_index == NONE)
		{
			event(_event_error, "networking:session:membership: [%s] local host requested player-properties does not exist",
				managed_session_get_id_string(m_managed_session_index));

			return false;
		}

		s_network_session_player* player = get_player(player_index);

		event(_event_status, "networking:session:membership: [%s] local host applying player-properties for player [#%d]",
			managed_session_get_id_string(m_managed_session_index),
			player_index);

		ASSERT(player->peer_index == m_session_membership.local_peer_index());
		m_session_membership.set_player_properties(
			player_index,
			player_update_number,
			controller_index,
			&player_data,
			player_voice_settings);
	}
	else
	{
		event(_event_status, "networking:session:membership: [%s] sending player-properties request",
			managed_session_get_id_string(m_managed_session_index));

		s_network_message_player_properties message{};
		csmemset(&message, 0, sizeof(s_network_message_player_properties));

		managed_session_get_id(m_managed_session_index, &message.session_id);
		message.player_update_number = player_update_number;
		message.controller_index = controller_index;
		message.player_data = player_data;
		message.player_voice_settings = player_voice_settings;

		int32 observer_channel_index = m_session_membership.m_local_peer_state[m_session_membership.host_peer_index()].channel_index;
		m_observer->observer_channel_send_message(m_session_index, observer_channel_index, false, _network_message_player_properties, sizeof(message), &message);
	}

	return true;
}

//.text:0045DEB0 ; bool c_network_session::peer_request_player_remove(int32)
//.text:0045DFE0 ; bool c_network_session::peer_request_properties_update(const s_transport_secure_address*, const s_network_session_peer_properties*)
//.text:0045E110 ; bool c_network_session::player_is_member(const s_player_identifier*) const
//.text:0045E130 ; 
//.text:0045E140 ; void c_network_session::process_pending_joins()

e_network_session_class c_network_session::session_class() const
{
	//return INVOKE_CLASS_MEMBER(0x0045E910, c_network_session, session_class);

	return m_session_class;
}

//.text:0045E920 ; bool c_network_session::ready_for_remote_peers_to_join() const
//.text:0045EA40 ; void c_network_session::reset_connection_state()
//.text:0045EA70 ; void c_network_session::reset_local_host_state(bool)
//.text:0045EAA0 ; void c_network_session::send_message_to_all_peers(c_network_session::e_session_message_transmission_type, e_network_message_type, int32, const void*) const
//.text:0045EB50 ; void c_network_session::send_message_to_peer(c_network_session::e_session_message_transmission_type, int32, e_network_message_type, int32, const void*) const
//.text:0045EBD0 ; bool c_network_session::session_is_full(int32, int32) const
//.text:0045EC20 ; e_network_session_mode c_network_session::session_mode() const
//.text:0045ED30 ; void c_network_session::set_default_session_parameters()

void c_network_session::set_disconnection_policy(e_network_session_disconnection_policy disconnection_policy)
{
	//INVOKE_CLASS_MEMBER(0x0045EDC0, c_network_session, set_disconnection_policy, disconnection_policy);

	ASSERT(disconnection_policy != _network_session_disconnection_allow_waiting_for_establishment);
	m_disconnection_policy = disconnection_policy;
	event(_event_message, "networking:session: [%s] disconnection policy now %s",
		managed_session_get_id_string(m_managed_session_index),
		c_network_session::get_disconnection_policy_string());
}

//.text:0045EDF0 ; void c_network_session_membership::set_host_peer_index(int32)
//.text:0045EE40 ; bool c_network_session::set_privacy_details(e_network_session_privacy, e_network_session_closed_status, int32, int32)
//.text:0045EFD0 ; uns32 c_network_session::time_get() const
//.text:0045EFE0 ; void c_network_session::time_release()
//.text:0045EFF0 ; void c_network_session::time_set(int32)

bool c_network_session::waiting_for_host_connection(const transport_address* address) const
{
	return INVOKE_CLASS_MEMBER(0x0045F030, c_network_session, waiting_for_host_connection, address);
}

//.text:0045F060 ; bool c_network_session::waiting_for_initial_update() const
//.text:0045F080 ; void c_network_session::whack_session_to_offline_session_class()

void c_network_session::leave_session_and_disconnect()
{
	//INVOKE_CLASS_MEMBER(0x0047DC40, c_network_session, leave_session_and_disconnect);

	c_network_session::set_disconnection_policy(_network_session_disconnection_allow);
	c_network_session::initiate_leave_protocol(false);
}

bool c_network_session::handle_boot_machine(c_network_channel* channel, const s_network_message_boot_machine* message)
{
	return INVOKE_CLASS_MEMBER(0x004DA040, c_network_session, handle_boot_machine, channel, message);
}

bool c_network_session::handle_delegate_leadership(c_network_channel* channel, const s_network_message_delegate_leadership* message)
{
	return INVOKE_CLASS_MEMBER(0x004DA1B0, c_network_session, handle_delegate_leadership, channel, message);
}

bool c_network_session::handle_host_decline(c_network_channel* channel, const s_network_message_host_decline* message)
{
	return INVOKE_CLASS_MEMBER(0x004DA300, c_network_session, handle_host_decline, channel, message);
}

bool c_network_session::handle_join_request(const transport_address* address, const s_network_message_join_request* message)
{
	return INVOKE_CLASS_MEMBER(0x004DA410, c_network_session, handle_join_request, address, message);
}

void c_network_session::handle_leave_acknowledgement(const transport_address* address)
{
	return INVOKE_CLASS_MEMBER(0x004DA540, c_network_session, handle_leave_acknowledgement, address);
}

bool c_network_session::handle_leave_request(const transport_address* address)
{
	return INVOKE_CLASS_MEMBER(0x004DA5F0, c_network_session, handle_leave_request, address);
}

bool c_network_session::handle_membership_update(const s_network_message_membership_update* message)
{
	return INVOKE_CLASS_MEMBER(0x004DA680, c_network_session, handle_membership_update, message);
}

bool c_network_session::handle_parameters_request(c_network_channel* channel, const s_network_message_parameters_request* message)
{
	return INVOKE_CLASS_MEMBER(0x004DA690, c_network_session, handle_parameters_request, channel, message);
}

bool c_network_session::handle_parameters_update(const s_network_message_parameters_update* message)
{
	return INVOKE_CLASS_MEMBER(0x004DA770, c_network_session, handle_parameters_update, message);

	//ASSERT(message);
	//
	//if (c_network_session::is_host())
	//{
	//	event(_event_error, "networking:session:parameters: [%s] parameters-update received and we are the host",
	//		managed_session_get_id_string(m_managed_session_index));
	//
	//	return false;
	//}
	//
	//if ((c_network_session::established() || c_network_session::peer_joining()) && !c_network_session::leaving_session())
	//{
	//	if (!m_session_parameters.handle_update(message))
	//	{
	//		event(_event_error, "networking:session:parameters: [%s] failed to handle parameters-update, we must disconnect [%s]",
	//			managed_session_get_id_string(m_managed_session_index),
	//			c_network_session::get_state_string());
	//
	//		c_network_session::handle_disconnection();
	//		return false;
	//	}
	//
	//	return true;
	//}
	//
	//if (c_network_session::leaving_session())
	//{
	//	event(_event_message, "networking:session:parameters: [%s] parameters-update received but we're leaving [%s], ignoring the update",
	//		managed_session_get_id_string(m_managed_session_index),
	//		c_network_session::get_state_string());
	//
	//	return false;
	//}
	//
	//event(_event_message, "networking:session:parameters: [%s] parameters-update received and we are in a bad state to handle the update [%s]",
	//	managed_session_get_id_string(m_managed_session_index),
	//	c_network_session::get_state_string());
	//
	//return false;
}

void c_network_session::handle_peer_connect(const transport_address* address, const s_network_message_peer_connect* message)
{
	INVOKE_CLASS_MEMBER(0x004DA870, c_network_session, handle_peer_connect, address, message);
}

bool c_network_session::handle_peer_establish(c_network_channel* channel, const s_network_message_peer_establish* message)
{
	return INVOKE_CLASS_MEMBER(0x004DAAB0, c_network_session, handle_peer_establish, channel, message);
}

bool c_network_session::handle_peer_properties(c_network_channel* channel, const s_network_message_peer_properties* message)
{
	return INVOKE_CLASS_MEMBER(0x004DAC30, c_network_session, handle_peer_properties, channel, message);
}

bool c_network_session::handle_player_add(c_network_channel* channel, const s_network_message_player_add* message)
{
	return INVOKE_CLASS_MEMBER(0x004DAD20, c_network_session, handle_player_add, channel, message);
}

bool c_network_session::handle_player_properties(c_network_channel* channel, const s_network_message_player_properties* message)
{
	//return INVOKE_CLASS_MEMBER(0x004DAEC0, c_network_session, handle_player_properties, channel, message);

	if (!c_network_session::established() || !c_network_session::is_host())
	{
		event(_event_warning, "networking:session:membership: [%s] player-properties received but not host, can't update players",
			managed_session_get_id_string(m_managed_session_index));
		return false;
	}

	int32 observer_channel_index = m_observer->observer_channel_find_by_network_channel(m_session_index, channel);
	int32 peer_index = m_session_membership.get_peer_from_observer_channel(observer_channel_index);
	if (peer_index == NONE || peer_index == m_session_membership.local_peer_index())
	{
		event(_event_message, "networking:session:membership: [%s] player-properties received from invalid peer [#%d]",
			managed_session_get_id_string(m_managed_session_index),
			peer_index);
		return false;
	}

	//if (!VALID_INDEX(message->user_index, 4))
	//{
	//	event(_event_error, "networking:session:membership: [%s] player-properties received for invalid user_index [#%d]",
	//		managed_session_get_id_string(m_managed_session_index),
	//		message->user_index);
	//	return false;
	//}

	int32 player_index = m_session_membership.get_player_index_from_peer(peer_index);
	if (player_index == NONE)
	{
		event(_event_warning, "networking:session:membership: [%s] player-properties received but no player associated with peer [#%d]",
			managed_session_get_id_string(m_managed_session_index),
			peer_index);
	}
	else
	{
		m_session_membership.set_player_properties(player_index,
			message->player_update_number,
			message->controller_index,
			&message->player_data,
			message->player_voice_settings);

		event(_event_status, "networking:session:membership: [%s] player-properties accepted for peer/player [#%d]/[#%d]",
			managed_session_get_id_string(m_managed_session_index),
			peer_index,
			peer_index);
	}

	return true;
}

bool c_network_session::handle_player_refuse(c_network_channel* channel, const s_network_message_player_refuse* message)
{
	return INVOKE_CLASS_MEMBER(0x004DB000, c_network_session, handle_player_refuse, channel, message);
}

bool c_network_session::handle_player_remove(c_network_channel* channel, const s_network_message_player_remove* message)
{
	return INVOKE_CLASS_MEMBER(0x004DB0E0, c_network_session, handle_player_remove, channel, message);
}

bool c_network_session::handle_session_boot(const transport_address* address, const s_network_message_session_boot* message)
{
	return INVOKE_CLASS_MEMBER(0x004DB200, c_network_session, handle_session_boot, address, message);
}

bool c_network_session::handle_session_disband(const transport_address* address, const s_network_message_session_disband* message)
{
	return INVOKE_CLASS_MEMBER(0x004DB2B0, c_network_session, handle_session_disband, address, message);
}

bool c_network_session::handle_time_synchronize(const transport_address* address, const s_network_message_time_synchronize* message)
{
	return INVOKE_CLASS_MEMBER(0x004DB340, c_network_session, handle_time_synchronize, address, message);
}

const char* g_session_disconnection_policy_strings[k_network_session_disconnection_policy_count]
{
	"waiting-for-establishment",
	"allowed",
	"reestablish-as-host",
};

