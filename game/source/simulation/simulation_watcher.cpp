#include "simulation/simulation_watcher.hpp"

#include "main/loading.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "simulation/simulation_world.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x0046CD70, c_simulation_watcher, in_online_networked_session);

//.text:0046BD80 ; 
//.text:0046BD90 ; private: void c_simulation_watcher::apply_machines_to_players()
//.text:0046BEE0 ; public: virtual bool c_simulation_watcher::attempt_channel_reconnection(int32 observer_channel_index, bool preexisting_connection) const
//.text:0046BEF0 ; public: bool c_simulation_watcher::boot_machine(const s_machine_identifier* machine_identifier, e_network_session_boot_reason boot_reason) const
//.text:0046BF30 ; public: void c_simulation_watcher::boot_machines(uns32 boot_machine_mask, e_network_session_boot_reason boot_reason) const
//.text:0046BF90 ; public: bool c_simulation_world::can_generate_updates() const
//.text:0046C030 ; public: virtual bool c_simulation_watcher::channel_is_load_bearing(int32 observer_index) const

void c_simulation_watcher::describe_status(char* buffer, int32 buffer_length) const
{
	//INVOKE_CLASS_MEMBER(0x0046C080, c_simulation_watcher, describe_status, buffer, buffer_length);

	csstrnzcpy(buffer, "", buffer_length);

	if (!m_world)
	{
		csstrnzcpy(buffer, "No gameworld exists", buffer_length);
		return;
	}

	if (m_world->is_local())
	{
		csstrnzcpy(buffer, "Local connection", buffer_length);
		return;
	}

	real32 load_progress = 0.0f;
	if (main_blocking_load_in_progress(&load_progress))
	{
		csnzprintf(buffer, buffer_length, "Loading map... %3.0f%%", load_progress * 100.0f);
		return;
	}

	if (!network_life_cycle_game_is_in_progress())
	{
		csstrnzcpy(buffer, "Life-cycle not In-Game", buffer_length);
		return;
	}

	if (!m_session)
	{
		csstrnzcpy(buffer, "No network session exists", buffer_length);
		return;
	}

	if (!m_session->established())
	{
		//m_session->describe_status(buffer, buffer_length);
		return;
	}

	//e_simulation_world_state state = m_world->get_state();
	e_simulation_world_state state = m_world->m_world_state;

	const c_network_session_membership* membership = m_session->get_session_membership();
	ASSERT(membership);

	char host_name[256]{};
	wchar_string_to_ascii_string(membership->get_host_peer()->properties.peer_name.get_string(), host_name, sizeof(host_name), NULL);
	int32 peer_observer_channel = m_session->get_peer_observer_channel(membership->host_peer_index());

	int32 join_time_elapsed = 0;
	int32 join_time_to_abort = 0;
	int32 join_attempt_count = 0;
	int32 join_attempt_maximum = 0;
	int32 join_client_establishing_count = 0;
	int32 join_client_waiting_count = 0;
	int32 join_client_joining_count = 0;
	int32 join_client_complete_count = 0;
	int32 join_client_total_count = 0;
	int32 join_time_to_failure = 0;

	m_world->get_join_status(
		&join_time_elapsed,
		&join_time_to_abort,
		&join_attempt_count,
		&join_attempt_maximum,
		&join_client_establishing_count,
		&join_client_waiting_count,
		&join_client_joining_count,
		&join_client_complete_count,
		&join_client_total_count,
		&join_time_to_failure);

	if (m_world->is_authority())
	{
		switch (state)
		{
		case _simulation_world_state_joining:
		{
			csnzprintf(buffer, buffer_length, "Waiting for clients to join...|n(est/wait/join/comp/total %d/%d/%d/%d/%d, give-up %4.1fsec)",
				join_client_establishing_count,
				join_client_waiting_count,
				join_client_joining_count,
				join_client_complete_count,
				join_client_total_count,
				(real32)join_time_to_abort / 1000.0f);
		}
		return;
		case _simulation_world_state_active:
		{
			csstrnzcpy(buffer, "Active gameworld authority", buffer_length);
		}
		return;
		case _simulation_world_state_handoff:
		{
			csnzprintf(buffer, buffer_length, "Handing off authority to new host '%s'", host_name);
		}
		return;
		case _simulation_world_state_leaving:
		{
			csstrnzcpy(buffer, "Gameworld authority leaving", buffer_length);
		}
		return;
		}

		csnzprintf(buffer, buffer_length, "Gameworld authority: state %s", c_simulation_world::get_state_string(state));
		return;
	}
	else
	{
		switch (state)
		{
		case _simulation_world_state_dead:
		{
			csstrnzcpy(buffer, "Gameworld client connection is dead", buffer_length);
		}
		return;
		case _simulation_world_state_joining:
		{
			csnzprintf(buffer, buffer_length, "Gameworld client joining '%s'...|n(attempt %d of %d, abort %4.1fsec, fail %4.1fsec)",
				host_name,
				join_client_joining_count,
				join_client_waiting_count,
				(real32)join_client_establishing_count / 1000.0f,
				(real32)join_time_to_abort / 1000.0f);
		}
		return;
		case _simulation_world_state_active:
		{
			csstrnzcpy(buffer, "Active gameworld client", buffer_length);
		}
		return;
		case _simulation_world_state_leaving:
		{
			csstrnzcpy(buffer, "Gameworld client leaving", buffer_length);
		}
		return;
		}

		csnzprintf(buffer, buffer_length, "Gameworld client: state %s", c_simulation_world::get_state_string(state));
		return;
	}

	char observer_status[256]{};
	//s_network_observer_status my_observer_status{};
	//if (peer_observer_channel == NONE && m_observer->get_observer_status(peer_observer_channel, &my_observer_status))
	//{
	//	csnzprintf(observer_status, sizeof(observer_status), "%s:", my_observer_status.channel_name);
	//}
	//else
	//{
		csstrnzcpy(observer_status, "unavailable", sizeof(observer_status));
	//}

	int32 disconnected_time_elapsed = 0;
	int32 disconnected_time_to_failure = 0;
	//m_world->get_disconnected_status(&disconnected_time_elapsed, &disconnected_time_to_failure);

	char session_status[1024]{};
	//m_session->describe_status(session_status, sizeof(session_status));

	csnzprintf(buffer, buffer_length, "Gameworld client connecting to '%s'...|n(channel %s, fail in %4.1fsec)|n%s",
		host_name,
		observer_status,
		(real32)disconnected_time_to_failure / 1000.0f,
		session_status);
}

//e_simulation_status c_simulation_watcher::describe_status_simple() const
int32 c_simulation_watcher::describe_status_simple() const
{
	return INVOKE_CLASS_MEMBER(0x0046C0A0, c_simulation_watcher, describe_status_simple);
}

//.text:0046C1B0 ; public: virtual bool c_simulation_watcher::desire_channel_heartbeat(int32 observer_channel_index) const
//.text:0046C1C0 ; public: void c_simulation_watcher::destroy_watcher()
//.text:0046C1F0 ; public: void c_simulation_watcher::generate_machine_update(bool* machine_update_valid, simulation_machine_update* machine_update)
//.text:0046C240 ; public: void c_simulation_watcher::generate_player_updates(int32* player_update_count, int32 maximum_player_update_count, simulation_player_update* player_updates)
//.text:0046C540 ; public: uns32 c_simulation_watcher::get_machine_ban_flags(int32 machine_index) const
//.text:0046C5D0 ; public: bool c_simulation_watcher::get_machine_connectivity(const s_machine_identifier* machine_identifier) const
//.text:0046C640 ; public: int32 c_simulation_watcher::get_machine_index_by_identifier(const s_machine_identifier* remote_machine_identifier) const
//.text:0046C6B0 ; public: bool c_simulation_watcher::get_machine_is_host(const s_machine_identifier* machine_identifier) const
//.text:0046C730 ; public: uns32 c_simulation_watcher::get_machine_valid_mask() const
//.text:0046C740 ; 
//.text:0046C750 ; public: bool c_simulation_watcher::get_pending_acknowledgement(bool force_acknowledgement, uns32* machine_valid_mask, s_machine_identifier* machine_identifiers, uns32* player_valid_mask, uns32* player_in_game_mask, s_player_identifier* player_identifiers)
//.text:0046C840 ; public: uns32 c_simulation_watcher::get_player_in_game_mask() const
//.text:0046C850 ; public: bool c_simulation_watcher::get_player_is_in_game(int32 player_index, const s_player_identifier* player_identifier) const
//.text:0046C8C0 ; 
//.text:0046C8E0 ; public: void c_simulation_watcher::handle_new_gamestate(uns32 game_state_proc_flags)
//.text:0046CC50 ; public: void c_simulation_watcher::handle_player_creation()
//.text:0046CCB0 ; public: void c_simulation_watcher::handle_player_leave_game(int32 player_index)

bool c_simulation_watcher::in_online_networked_session()
{
	//return INVOKE_CLASS_MEMBER(0x0046CD70, c_simulation_watcher, in_online_networked_session);

	return false;

	//if (!m_session)
	//{
	//	return false;
	//}
	//
	//return m_session->session_class() == _network_session_class_xbox_live
	//	&& m_session->get_session_membership_unsafe()->m_shared_network_membership.peer_count > 1;
}

void c_simulation_watcher::initialize_watcher(c_simulation_world* world)
{
	INVOKE_CLASS_MEMBER(0x0046CDA0, c_simulation_watcher, initialize_watcher, world);

	//ASSERT(world);
	//ASSERT(m_world == NULL);
	//
	//m_world = world;
	//m_session = NULL;
	//m_observer = NULL;
	//m_game_revert_occured = false;
	//c_simulation_watcher::reset_tracking_arrays();
}

//.text:0046CDD0 ; public: void c_simulation_watcher::log_acknowledgement_status(bool force_acknowledgement, uns32 machine_valid_mask, uns32 player_valid_mask, uns32 player_in_game_mask) const
//.text:0046CDE0 ; public: bool c_simulation_watcher::maintain_connection()
//.text:0046CEF0 ; private: bool c_simulation_watcher::maintain_simulation_authority(c_network_session* session, int32 local_peer_index, c_static_flags<16> peer_valid_flags, const s_network_session_peer* peers)
//.text:0046CFF0 ; private: bool c_simulation_watcher::maintain_simulation_client(int32 host_peer_index)

bool c_simulation_watcher::need_to_generate_updates() const
{
	return INVOKE_CLASS_MEMBER(0x0046D090, c_simulation_watcher, need_to_generate_updates);
}

//.text:0046D130 ; public: virtual void c_simulation_watcher::notify_channel_connection(int32 observer_channel_index, uns32 connection_identifier, bool connected)
//.text:0046D140 ; public: virtual void c_simulation_watcher::notify_channel_died(int32 observer_channel_index)
//.text:0046D150 ; public: void c_simulation_watcher::notify_game_revert()
//.text:0046D160 ; 
//.text:0046D170 ; private: static simulation_player_update* __cdecl c_simulation_watcher::player_update_allocate(int32* player_update_count, int32 maximum_player_update_count, simulation_player_update* player_updates)
//.text:0046D190 ; private: static void __cdecl c_simulation_watcher::player_update_find_swap_location(const s_player_collection* players, int32 add_player_index, const s_player_identifier* add_player_identifier, int32* swap_player_index, bool* remove_existing_player)
//.text:0046D280 ; private: static bool __cdecl c_simulation_watcher::player_update_must_leave_game(int32 player_index, const s_player_collection_player* player, c_static_flags<16> peer_valid_flags, const s_machine_identifier* network_machine_identifiers, c_static_flags<16> valid_network_player_flags, const s_network_session_player* network_players)
//.text:0046D2F0 ; public: void c_simulation_watcher::reset_tracking_arrays()
//.text:0046D370 ; 

void c_simulation_watcher::setup_connection()
{
	INVOKE_CLASS_MEMBER(0x0046D380, c_simulation_watcher, setup_connection);
}

//.text:0046D5B0 ; private: bool c_simulation_watcher::setup_view(int32 machine_index, const s_network_session_peer* session_peer, int32 observer_channel_index)
//.text:0046D6D0 ; private: bool c_simulation_watcher::update_machines(const s_transport_secure_address* session_host_address, int32 session_local_update_number, int32 session_update_number, int32 session_peer_index, c_static_flags<16> peer_valid_flags, const s_network_session_peer* session_peers)

