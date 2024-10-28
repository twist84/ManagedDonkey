#include "simulation/simulation_watcher.hpp"

#include "main/loading.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "simulation/simulation_world.hpp"

void c_simulation_watcher::describe_status(char* buffer, long buffer_size) const
{
	//INVOKE_CLASS_MEMBER(0x0046C080, c_simulation_watcher, describe_status, buffer, buffer_size);

	csstrnzcpy(buffer, "", buffer_size);

	if (!m_world)
	{
		csstrnzcpy(buffer, "No gameworld exists", buffer_size);
		return;
	}

	if (m_world->is_local())
	{
		csstrnzcpy(buffer, "Local connection", buffer_size);
		return;
	}

	real progress = 0.0f;
	if (main_blocking_load_in_progress(&progress))
	{
		csnzprintf(buffer, buffer_size, "Loading map... %3.0f%%", progress * 100.0f);
		return;
	}

	if (!network_life_cycle_game_is_in_progress())
	{
		csstrnzcpy(buffer, "Life-cycle not In-Game", buffer_size);
		return;
	}

	if (!m_session)
	{
		csstrnzcpy(buffer, "No network session exists", buffer_size);
		return;
	}

	if (!m_session->established())
	{
		//m_session->describe_status(buffer, buffer_size);
		return;
	}

	//e_simulation_world_state state = m_world->get_state();
	e_simulation_world_state state = m_world->m_world_state;

	c_network_session_membership const* membership = m_session->get_session_membership();
	ASSERT(membership);

	char host_peer_name[256]{};
	wchar_string_to_ascii_string(membership->get_host_peer()->properties.peer_name.get_string(), host_peer_name, sizeof(host_peer_name), NULL);
	long host_channel_index = m_session->get_peer_observer_channel(membership->host_peer_index());

	long join_time_elapsed = 0;
	long join_time_to_abort = 0;
	long join_attempt_count = 0;
	long join_attempt_maximum = 0;
	long join_client_establishing_count = 0;
	long join_client_waiting_count = 0;
	long join_client_joining_count = 0;
	long join_client_complete_count = 0;
	long join_client_total_count = 0;
	long join_time_to_failure = 0;

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
			csnzprintf(buffer, buffer_size, "Waiting for clients to join...|n(est/wait/join/comp/total %d/%d/%d/%d/%d, give-up %4.1fsec)",
				join_client_establishing_count,
				join_client_waiting_count,
				join_client_joining_count,
				join_client_complete_count,
				join_client_total_count,
				(real)join_time_to_abort / 1000.0f);
		}
		return;
		case _simulation_world_state_active:
			csstrnzcpy(buffer, "Active gameworld authority", buffer_size);
			return;
		case _simulation_world_state_handoff:
			csnzprintf(buffer, buffer_size, "Handing off authority to new host '%s'", host_peer_name);
			return;
		case _simulation_world_state_leaving:
			csstrnzcpy(buffer, "Gameworld authority leaving", buffer_size);
			return;
		}

		csnzprintf(buffer, buffer_size, "Gameworld authority: state %s", c_simulation_world::get_state_string(state));
		return;
	}
	else
	{
		switch (state)
		{
		case _simulation_world_state_dead:
			csstrnzcpy(buffer, "Gameworld client connection is dead", buffer_size);
			return;
		case _simulation_world_state_joining:
		{
			csnzprintf(buffer, buffer_size, "Gameworld client joining '%s'...|n(attempt %d of %d, abort %4.1fsec, fail %4.1fsec)",
				host_peer_name,
				join_client_joining_count,
				join_client_waiting_count,
				(real)join_client_establishing_count / 1000.0f,
				(real)join_time_to_abort / 1000.0f);
		}
		return;
		case _simulation_world_state_active:
			csstrnzcpy(buffer, "Active gameworld client", buffer_size);
			return;
		case _simulation_world_state_leaving:
			csstrnzcpy(buffer, "Gameworld client leaving", buffer_size);
			return;
		}

		csnzprintf(buffer, buffer_size, "Gameworld client: state %s", c_simulation_world::get_state_string(state));
		return;
	}

	char host_channel_name[256]{};
	//s_network_observer_status observer_status{};
	//if (host_channel_index == NONE && m_observer->get_observer_status(host_channel_index, &observer_status))
	//{
	//
	//}
	//else
	//{
	csstrnzcpy(host_channel_name, "unavailable", sizeof(host_channel_name));
	//}

	long disconnected_time_elapsed = 0;
	long disconnected_time_to_failure = 0;
	//m_world->get_disconnected_status(&disconnected_time_elapsed, &disconnected_time_to_failure);

	char session_status[1024]{};
	//m_session->describe_status(session_status, sizeof(session_status));

	csnzprintf(buffer, buffer_size, "Gameworld client connecting to '%s'...|n(channel %s, fail in %4.1fsec)|n%s",
		host_peer_name,
		host_channel_name,
		(real)disconnected_time_to_failure / 1000.0f,
		session_status);
}

//.text:0046C0A0 ; public: e_simulation_status __cdecl c_simulation_watcher::describe_status_simple() const

bool c_simulation_watcher::need_to_generate_updates() const
{
	return INVOKE_CLASS_MEMBER(0x0046D090, c_simulation_watcher, need_to_generate_updates);
}

