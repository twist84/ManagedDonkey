#include "networking/session/network_session_manager.hpp"

#include "networking/session/network_session.hpp"

void c_network_session_manager::deregister_session(c_network_session* session)
{
	DECLFUNC(0x0045F0A0, void, __thiscall, c_network_session_manager*, c_network_session*)(this, session);
}

void c_network_session_manager::destroy_session_manager()
{
	DECLFUNC(0x0045F0C0, void, __thiscall, c_network_session_manager*)(this);
}

c_network_session* c_network_session_manager::get_session(long index)
{
	return DECLFUNC(0x0045F0D0, c_network_session*, __thiscall, c_network_session_manager*, long)(this, index);
}

c_network_session* c_network_session_manager::get_session(s_transport_secure_identifier const* session_id)
{
	return DECLFUNC(0x0045F0E0, c_network_session*, __thiscall, c_network_session_manager*, s_transport_secure_identifier const*)(this, session_id);
}

c_network_session* c_network_session_manager::get_session_from_managed_session_index(long index)
{
	return DECLFUNC(0x0045F120, c_network_session*, __thiscall, c_network_session_manager*, long)(this, index);
}

bool c_network_session_manager::initialize_session_manager()
{
	return DECLFUNC(0x0045F160, bool, __thiscall, c_network_session_manager*)(this);
}

void c_network_session_manager::register_session(c_network_session* session)
{
	DECLFUNC(0x0045F180, void, __thiscall, c_network_session_manager*)(this);
}

