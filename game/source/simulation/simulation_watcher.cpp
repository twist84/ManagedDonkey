#include "simulation/simulation_watcher.hpp"

#include "main/loading.hpp"
#include "memory/module.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "simulation/simulation_world.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x0046CD70, c_simulation_watcher, in_online_networked_session);

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

bool c_simulation_watcher::need_to_generate_updates() const
{
	return INVOKE_CLASS_MEMBER(0x0046D090, c_simulation_watcher, need_to_generate_updates);
}

void c_simulation_watcher::setup_connection()
{
	INVOKE_CLASS_MEMBER(0x0046D380, c_simulation_watcher, setup_connection);
}

