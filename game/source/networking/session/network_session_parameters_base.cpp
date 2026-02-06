#include "networking/session/network_session_parameters_base.hpp"

#include "cseries/cseries_events.hpp"
#include "networking/session/network_managed_session.hpp"
#include "networking/session/network_session.hpp"
#include "networking/session/network_session_parameter_type_collection.hpp"

//c_network_session_parameter_base::c_network_session_parameter_base() :
//	m_parameter_type(k_network_session_parameter_type_none),
//	m_parameter_type_description(nullptr),
//	m_session(nullptr),
//	m_update_priority(_parameter_update_priority_default),
//	m_change_request_access(_parameter_change_request_access_default),
//	m_transmitted_peer_updates(),
//	m_flags(0)
//{
//	DECLFUNC(0x00450BB0, void, __thiscall, c_network_session_parameter_base*)(this);
//}

void c_network_session_parameter_base::build_change_request(int32 peer_index, void* payload, int32 payload_size) const
{
	//INVOKE_CLASS_MEMBER(0x00450C20, c_network_session_parameter_base, build_change_request, peer_index, payload, payload_size);

	ASSERT(payload);
	ASSERT(payload_size == get_change_request_payload_size());

	get_change_request_payload(peer_index, payload);
}

void c_network_session_parameter_base::build_update(int32 peer_index, s_network_session_parameter_update* update, int32 update_size) const
{
	//INVOKE_CLASS_MEMBER(0x00450C40, c_network_session_parameter_base, build_update, peer_index, update, update_size);

	ASSERT(update);
	ASSERT(update_size == get_update_size());

	get_update_payload(peer_index, update->payload);
}

bool c_network_session_parameter_base::can_clear_change_request_desired(void)
{
	//return INVOKE_CLASS_MEMBER(0x00450C60, c_network_session_parameter_base, can_clear_change_request_desired);

	return true;
}

bool c_network_session_parameter_base::can_set_peer_no_longer_requires_update(int32 peer_index)
{
	//return INVOKE_CLASS_MEMBER(0x00450C70, c_network_session_parameter_base, can_set_peer_no_longer_requires_update);

	return true;
}

bool c_network_session_parameter_base::change_request_allowed(void) const
{
	//return INVOKE_CLASS_MEMBER(0x00450C80, c_network_session_parameter_base, change_request_allowed);

	if (!m_session->established())
		return false;

	if (m_change_request_access == _parameter_change_request_access_peer)
		return true;

	if (m_change_request_access == _parameter_change_request_access_leader)
		return m_session->is_leader();

	return false;
}

void c_network_session_parameter_base::clear(void)
{
	//INVOKE_CLASS_MEMBER(0x00450CC0, c_network_session_parameter_base, clear);

	event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] clearing",
		get_session_description(),
		m_parameter_type,
		m_parameter_type_description);

	m_transmitted_peer_updates.set_all(false);
	m_flags = 0;
	notify_clear();
}

bool c_network_session_parameter_base::desires_change_request(void) const
{
	//return INVOKE_CLASS_MEMBER(0x00450CE0, c_network_session_parameter_base, desires_change_request);

	return TEST_BIT(m_flags, _network_session_parameter_desires_change_request_bit);
}

bool c_network_session_parameter_base::get_allowed(void) const
{
	//return INVOKE_CLASS_MEMBER(0x00450CF0, c_network_session_parameter_base, get_allowed);

	return TEST_BIT(m_flags, _network_session_parameter_valid_bit);
}

e_parameter_change_request_access c_network_session_parameter_base::get_change_request_access(void) const
{
	//return INVOKE_CLASS_MEMBER(0x00450D00, c_network_session_parameter_base, get_change_request_access);

	return m_change_request_access;
}

const char* c_network_session_parameter_base::get_change_request_denied_reason(void) const
{
	return INVOKE_CLASS_MEMBER(0x00450D10, c_network_session_parameter_base, get_change_request_denied_reason);
}

int32 c_network_session_parameter_base::get_change_request_size(void) const
{
	//return INVOKE_CLASS_MEMBER(0x00450D90, c_network_session_parameter_base, get_change_request_size);

	return get_change_request_payload_size();
}

const char* c_network_session_parameter_base::get_session_description(void) const
{
	return INVOKE_CLASS_MEMBER(0x00450DA0, c_network_session_parameter_base, get_session_description);

	//if (m_session && m_session->get_session_id(nullptr))
	//	return managed_session_get_id_string(m_session->managed_session_index());
	//
	//return "UNKNOWN";
}

const char* c_network_session_parameter_base::get_set_denied_reason(void) const
{
	//return INVOKE_CLASS_MEMBER(0x00450DD0, c_network_session_parameter_base, get_set_denied_reason);

	if (!set_allowed())
	{
		if (!m_session->established())
			return "not established";

		if (!m_session->is_host())
			return "not host";
	}
	return "NONE";
}

e_parameter_update_priority c_network_session_parameter_base::get_update_priority(void) const
{
	//return INVOKE_CLASS_MEMBER(0x00450E20, c_network_session_parameter_base, get_update_priority);

	return m_update_priority;
}

int32 c_network_session_parameter_base::get_update_size(void) const
{
	//return INVOKE_CLASS_MEMBER(0x00450E30, c_network_session_parameter_base, get_update_size);

	//return get_update_payload_size() + sizeof(s_network_session_parameter_update);
	return get_update_payload_size() + 4; // is the 4 `sizeof(s_network_session_parameter_update)`?
}

bool c_network_session_parameter_base::handle_change_request(const void* change_request, int32 change_request_size)
{
	//return INVOKE_CLASS_MEMBER(0x00450E40, c_network_session_parameter_base, handle_change_request, change_request, change_request_size);

	ASSERT(change_request);
	ASSERT(change_request_size == get_change_request_size());

	bool result = handle_change_request_payload(change_request);
	if (result)
	{
		event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] change request handled",
			get_session_description(),
			m_parameter_type,
			m_parameter_type_description);
	}
	return result;
}

bool c_network_session_parameter_base::handle_update(const s_network_session_parameter_update* update, int32 update_size)
{
	//return INVOKE_CLASS_MEMBER(0x00450EA0, c_network_session_parameter_base, handle_update, update, update_size);

	ASSERT(update);
	ASSERT(update->payload);
	ASSERT(update_size == get_update_size());

	bool result = handle_update_payload(update);
	if (result && parameter_data_ready_for_consumption())
	{
		event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] update handled and data now available",
			get_session_description(),
			m_parameter_type,
			m_parameter_type_description);
	}
	return result;
}

void c_network_session_parameter_base::initialize(e_network_session_parameter_type parameter_type, const char* parameter_type_description, e_parameter_update_priority update_priority, e_parameter_change_request_access change_request_access, c_network_session* session)
{
	//INVOKE_CLASS_MEMBER(0x00450F20, c_network_session_parameter_base, initialize, parameter_type, parameter_type_description, update_priority, change_request_access, session);

	ASSERT(session);
	ASSERT(parameter_type != k_network_session_parameter_type_none);
	ASSERT(parameter_type_description);
	ASSERT(update_priority != k_parameter_update_priority_none);
	ASSERT(change_request_access != k_network_session_parameter_type_none);

	m_parameter_type_description = parameter_type_description;
	m_update_priority = update_priority;
	m_parameter_type = parameter_type;
	m_change_request_access = change_request_access;
	m_session = session;
}

void c_network_session_parameter_base::notify_change_request_payload_sent(void)
{
	//INVOKE_CLASS_MEMBER(0x00450F50, c_network_session_parameter_base, notify_change_request_payload_sent);
}

void c_network_session_parameter_base::notify_change_request_sent(void)
{
	//INVOKE_CLASS_MEMBER(0x00450F60, c_network_session_parameter_base, notify_change_request_sent);

	event(_event_status, "networking:session_parameters: [%s] notify parameter %d change request sent to host",
		get_session_description(),
		m_parameter_type);

	notify_change_request_payload_sent();
	if (can_clear_change_request_desired())
	{
		set_change_request_no_longer_desired();

		event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] change request sent",
			get_session_description(),
			m_parameter_type,
			m_parameter_type_description);
	}
}

void c_network_session_parameter_base::notify_set_change_request(void)
{
	//INVOKE_CLASS_MEMBER(0x00451040, c_network_session_parameter_base, notify_set_change_request);
}

void c_network_session_parameter_base::notify_set_update_required(void)
{
	//INVOKE_CLASS_MEMBER(0x00451050, c_network_session_parameter_base, notify_set_update_required);
}

void c_network_session_parameter_base::notify_update_payload_sent_to_peer(int32 peer_index)
{
	//INVOKE_CLASS_MEMBER(0x00451060, c_network_session_parameter_base, notify_update_payload_sent_to_peer, peer_index);
}

void c_network_session_parameter_base::notify_update_sent_to_peer(int32 peer_index)
{
	//INVOKE_CLASS_MEMBER(0x00451070, c_network_session_parameter_base, notify_update_sent_to_peer, peer_index);

	event(_event_status, "networking:session_parameters: [%s] notify parameter %d update sent to peer %d",
		get_session_description(),
		m_parameter_type,
		peer_index);

	notify_update_payload_sent_to_peer(peer_index);
	if (can_set_peer_no_longer_requires_update(peer_index))
	{
		set_peer_no_longer_requires_update(peer_index);

		event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] marking peer %d no longer requires update",
			get_session_description(),
			m_parameter_type,
			m_parameter_type_description,
			peer_index);
	}
}

bool c_network_session_parameter_base::parameter_data_ready_for_consumption(void) const
{
	//return INVOKE_CLASS_MEMBER(0x00451160, c_network_session_parameter_base, parameter_data_ready_for_consumption);

	return true;
}

bool c_network_session_parameter_base::peer_requires_update(int32 peer_index) const
{
	//return INVOKE_CLASS_MEMBER(0x00451170, c_network_session_parameter_base, peer_requires_update, peer_index);

	if (TEST_BIT(m_flags, _network_session_parameter_valid_bit))
		return !m_transmitted_peer_updates[peer_index];

	return false;
}

void c_network_session_parameter_base::reset_peer_state(int32 peer_index)
{
	//INVOKE_CLASS_MEMBER(0x00451190, c_network_session_parameter_base, reset_peer_state, peer_index);

	event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] resetting peer %d state",
		get_session_description(),
		m_parameter_type,
		m_parameter_type_description,
		peer_index);

	m_transmitted_peer_updates[peer_index] = false;
	notify_reset_peer_state(peer_index);
}

//.text:004511F0 ; void c_static_array<bool, 17>::set_all(const bool& value)

bool c_network_session_parameter_base::set_allowed(void) const
{
	//return INVOKE_CLASS_MEMBER(0x00451210, c_network_session_parameter_base, set_allowed);

	return m_session->established() && m_session->is_host();
}

void c_network_session_parameter_base::set_change_request_no_longer_desired(void)
{
	//INVOKE_CLASS_MEMBER(0x00451240, c_network_session_parameter_base, set_change_request_no_longer_desired);

	event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] change request no longer desired",
		get_session_description(),
		m_parameter_type,
		m_parameter_type_description);

	m_flags &= ~FLAG(_network_session_parameter_desires_change_request_bit);
}

void c_network_session_parameter_base::set_desires_change_request(void)
{
	//INVOKE_CLASS_MEMBER(0x00451290, c_network_session_parameter_base, set_desires_change_request);

	event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] desires change request",
		get_session_description(),
		m_parameter_type,
		m_parameter_type_description);

	m_flags |= FLAG(_network_session_parameter_desires_change_request_bit);
	notify_set_change_request();
}

void c_network_session_parameter_base::set_peer_no_longer_requires_update(int32 peer_index)
{
	//INVOKE_CLASS_MEMBER(0x004512E0, c_network_session_parameter_base, set_peer_no_longer_requires_update, peer_index);

	ASSERT(!m_transmitted_peer_updates[peer_index]);

	event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] peer %d no longer requires update",
		get_session_description(),
		m_parameter_type,
		m_parameter_type_description,
		peer_index);

	m_transmitted_peer_updates[peer_index] = true;
}

void c_network_session_parameter_base::set_update_required(void)
{
	//INVOKE_CLASS_MEMBER(0x00451340, c_network_session_parameter_base, set_update_required);

	ASSERT(set_allowed());

	event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] marking dirty",
		get_session_description(),
		m_parameter_type,
		m_parameter_type_description);

	m_transmitted_peer_updates.set_all(false);
	m_flags |= FLAG(_network_session_parameter_valid_bit);
	notify_set_update_required();
}

void c_network_session_parameter_base::transition_state_to_become_host(void)
{
	//INVOKE_CLASS_MEMBER(0x004513A0, c_network_session_parameter_base, transition_state_to_become_host);

	event(_event_status, "networking:session_parameters: [%s] parameter %d [%s] transitioning to host",
		get_session_description(),
		m_parameter_type,
		m_parameter_type_description);

	m_transmitted_peer_updates.set_all(false);
	m_flags &= FLAG(_network_session_parameter_valid_bit);
	notify_transition_state_to_become_host();
}

//void c_network_session_parameter_base::validate_buffer(int32 size, const void* original, void* temporary, void* buffer)
//{
//	int32 update_size = size;
//	c_bitstream encoded_message((byte*)buffer, size);
//	encoded_message.begin_writing(1);
//	g_network_session_parameter_type_collection->encode_update(m_parameter_type, &encoded_message, original, update_size);
//	encoded_message.finish_writing(nullptr);
//	csmemset(temporary, 0, update_size);
//	encoded_message.begin_reading();
//	g_network_session_parameter_type_collection->decode_update(m_parameter_type, &encoded_message, temporary, update_size);
//	encoded_message.finish_reading();
//	ASSERT(0 == csmemcmp(original, temporary, size));
//}

