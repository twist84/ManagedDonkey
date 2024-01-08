#include "networking/session/network_session_parameters_base.hpp"

#include "cseries/cseries_events.hpp"
#include "networking/session/network_session.hpp"

char const* c_network_session_parameter_base::get_session_description() const
{
	//if (m_session && m_session->get_session_id(nullptr))
	//	return managed_session_get_id_string(m_session->managed_session_index());

	return "UNKNOWN";
}

void c_network_session_parameter_base::set_desires_change_request()
{
	generate_event(_event_level_status, "networking:session_parameters: [%s] parameter %d [%s] desires change request", get_session_description(), m_type, m_name);
	m_state_flags |= FLAG(1);
	notify_set_change_request();
}

void c_network_session_parameter_base::reset_peer_state(long peer_index)
{
	generate_event(_event_level_status, "networking:session_parameters: [%s] parameter %d [%s] resetting peer %d state", get_session_description(), m_type, m_name, peer_index);
	m_transmitted_peer_updates[peer_index] = false;
	notify_reset_peer_state(peer_index);
}

void c_network_session_parameter_base::notify_update_sent_to_peer(long peer_index)
{
	generate_event(_event_level_status, "networking:session_parameters: [%s] notify parameter %d update sent to peer %d", get_session_description(), m_type, peer_index);
	notify_update_payload_sent_to_peer(peer_index);
	if (can_set_peer_no_longer_requires_update(peer_index))
	{
		set_peer_no_longer_requires_update(peer_index);
		generate_event(_event_level_status, "networking:session_parameters: [%s] parameter %d [%s] marking peer %d no longer requires update", get_session_description(), m_type, m_name, peer_index);
	}
}

void c_network_session_parameter_base::set_update_required()
{
	ASSERT(set_allowed());

	generate_event(_event_level_status, "networking:session_parameters: [%s] parameter %d [%s] marking dirty", get_session_description(), m_type, m_name);
	csmemset(m_transmitted_peer_updates, 0, sizeof(m_transmitted_peer_updates));
	notify_set_update_required();
}

void c_network_session_parameter_base::transition_state_to_become_host()
{
	generate_event(_event_level_status, "networking:session_parameters: [%s] parameter %d [%s] transitioning to host", get_session_description(), m_type, m_name);
	csmemset(m_transmitted_peer_updates, 0, sizeof(m_transmitted_peer_updates));
	m_state_flags &= FLAG(0);
	notify_transition_state_to_become_host();
}

void c_network_session_parameter_base::notify_change_request_sent()
{
	generate_event(_event_level_status, "networking:session_parameters: [%s] notify parameter %d change request sent to host", get_session_description(), m_type);
	notify_change_request_payload_sent();
	if (can_clear_change_request_desired())
	{
		set_change_request_no_longer_desired();
		generate_event(_event_level_status, "networking:session_parameters: [%s] parameter %d [%s] change request sent", get_session_description(), m_type, m_name);
	}
}

bool c_network_session_parameter_base::handle_change_request(void const* change_request, long change_request_size)
{
	ASSERT(change_request);
	ASSERT(change_request_size == get_change_request_size());

	bool result = handle_change_request_payload(change_request);
	if (result)
	{
		generate_event(_event_level_status, "networking:session_parameters: [%s] parameter %d [%s] change request handled", get_session_description(), m_type, m_name);
	}
	return result;
}

long c_network_session_parameter_base::get_change_request_size() const
{
	return get_change_request_payload_size();
}

bool c_network_session_parameter_base::handle_update(s_network_session_parameter_update const* update, long update_size)
{
	ASSERT(update);
	ASSERT(update->payload);
	ASSERT(update_size == get_update_size());

	bool result = handle_update_payload(update);
	if (result && parameter_data_ready_for_consumption())
	{
		generate_event(_event_level_status, "networking:session_parameters: [%s] parameter %d [%s] update handled and data now available", get_session_description(), m_type, m_name);
	}
	return result;
}

long c_network_session_parameter_base::get_update_size() const
{
	return get_update_payload_size();
}

void c_network_session_parameter_base::set_change_request_no_longer_desired()
{
	generate_event(_event_level_status, "networking:session_parameters: [%s] parameter %d [%s] change request no longer desired", get_session_description(), m_type, m_name);
	m_state_flags &= ~FLAG(1);
}

void c_network_session_parameter_base::set_peer_no_longer_requires_update(long peer_index)
{
	ASSERT(!m_transmitted_peer_updates[peer_index]);

	generate_event(_event_level_status, "networking:session_parameters: [%s] parameter %d [%s] peer %d no longer requires update", get_session_description(), m_type, m_name, peer_index);
	m_transmitted_peer_updates[peer_index] = true;
}

bool c_network_session_parameter_base::change_request_allowed() const
{
	//if (m_session->established())
	//{
	//	if (m_change_request_access == 2)
	//	{
	//		return true;
	//	}
	//	else if (m_change_request_access == 1)
	//	{
	//		return m_session->is_leader();
	//	}
	//}
	//return false;

	return DECLFUNC(0x00450C80, bool, __thiscall, c_network_session_parameter_base const*)(this);
}

bool c_network_session_parameter_base::get_allowed() const
{
	return TEST_BIT(m_state_flags, 0);
}

char const* c_network_session_parameter_base::get_change_request_denied_reason() const
{
	return DECLFUNC(0x00450D10, char const*, __thiscall, c_network_session_parameter_base const*)(this);
}

char const* c_network_session_parameter_base::get_set_denied_reason() const
{
	//if (set_allowed())
	//	return "NONE";
	//
	//if (!m_session->established()
	//	return "not established";
	//
	//if (!m_session->is_host())
	//	return "not host";

	return DECLFUNC(0x00450DD0, char const*, __thiscall, c_network_session_parameter_base const*)(this);
}

bool c_network_session_parameter_base::set_allowed() const
{
	//return m_session->established() && m_session()->is_host();

	return DECLFUNC(0x00451210, bool, __thiscall, c_network_session_parameter_base const*)(this);
}

