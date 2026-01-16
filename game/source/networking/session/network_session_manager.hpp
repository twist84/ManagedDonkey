#pragma once

#include "cseries/cseries.hpp"

class c_network_session;
struct s_transport_secure_identifier;

class c_network_session_manager
{
public:
	void deregister_session(c_network_session* session);
	void destroy_session_manager();
	c_network_session* get_session(int32 index);
	c_network_session* get_session(const s_transport_secure_identifier* session_id);
	c_network_session* get_session_from_managed_session_index(int32 index);
	bool initialize_session_manager();
	void register_session(c_network_session* session);

protected:
	c_static_array<c_network_session*, 3> m_sessions;
};
COMPILE_ASSERT(sizeof(c_network_session_manager) == 0xC);

