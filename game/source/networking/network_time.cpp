#include "networking/network_time.hpp"

#include "cseries/cseries_windows.hpp"
#include "networking/session/network_session.hpp"
#include "networking/session/network_session_manager.hpp"
#include "networking/transport/transport_security.hpp"

REFERENCE_DECLARE(0x019E8D4C, s_network_time_globals, network_time_globals);

int32 __cdecl network_time_difference_msec(uns32 timestamp1, uns32 timestamp2)
{
	return timestamp2 - timestamp1;
}

uns32 __cdecl network_time_get()
{
	//return INVOKE(0x00439E40, network_time_get);

	if (network_time_globals.time_locked)
	{
		return network_time_globals.time_locked_timestamp;
	}

	return network_time_get_exact();
}

uns32 __cdecl network_time_get_exact()
{
	//return INVOKE(0x00439E60, network_time_get_exact);

	return system_milliseconds();
}

int32 __cdecl network_time_since(uns32 timestamp)
{
	//return INVOKE(0x00439E70, network_time_since, timestamp);

	return network_time_difference_msec(timestamp, network_time_get());
}

void __cdecl network_session_time_deregister_session_manager()
{
	//INVOKE(0x0045F190, network_session_time_deregister_session_manager);

	ASSERT(network_time_globals.session_manager != nullptr);
	network_time_globals.session_manager = nullptr;
}

bool __cdecl network_session_time_exists(const s_transport_secure_identifier* session_id)
{
	return INVOKE(0x0045F1A0, network_session_time_get_time, session_id);

	//ASSERT(session_id);
	//c_network_session* session = network_time_globals.session_manager->get_session(session_id);
	//return session != nullptr && session->m_time_exists;
}

bool __cdecl network_session_time_get_id_and_time(int32 session_index, s_transport_secure_identifier* session_id, uns32* timestamp)
{
	return INVOKE(0x0045F1D0, network_session_time_get_id_and_time, session_index, session_id, timestamp);

	//if (!network_time_globals.session_manager != nullptr)
	//{
	//	return false;
	//}
	//
	//c_network_session* session = network_time_globals.session_manager->get_session(session_index);
	//if (session == nullptr || !session->m_time_exists)
	//{
	//	return false;
	//}
	//
	//session->get_secure_key(session_id, nullptr, nullptr);
	//*timestamp = session->time_get();
	//return true;
}

uns32 __cdecl network_session_time_get_time(const s_transport_secure_identifier* session_id)
{
	return INVOKE(0x0045F230, network_session_time_get_time, session_id);

	//ASSERT(session_id);
	//
	//if (!network_time_globals.session_manager != nullptr)
	//{
	//	return false;
	//}
	//
	//c_network_session* session = network_time_globals.session_manager->get_session(session_id);
	//if (session == nullptr || !session->m_time_exists)
	//{
	//	return false;
	//}
	//
	//return session->time_get();
}

void __cdecl network_session_time_register_session_manager(c_network_session_manager* session_manager)
{
	//INVOKE(0x0045F260, network_session_time_register_session_manager, session_manager);

	ASSERT(network_time_globals.session_manager == nullptr);
	network_time_globals.session_manager = session_manager;
}

void __cdecl network_time_lock(bool locked)
{
	//INVOKE(0x0045F270, network_time_lock, locked);

	network_time_globals.time_locked = locked;
	network_time_globals.time_locked_timestamp = network_time_get_exact();
}

