#pragma once

#include "cseries/cseries.hpp"

struct c_network_session;
struct s_transport_secure_identifier;

struct c_network_session_manager
{
public:
	void deregister_session(c_network_session* session);
	void destroy_session_manager();
	c_network_session* get_session(long index);
	c_network_session* get_session(s_transport_secure_identifier const* session_id);
	c_network_session* get_session_from_managed_session_index(long index);
	bool initialize_session_manager();
	void register_session(c_network_session* session);

protected:
	c_static_array<c_network_session*, 3> m_sessions;
};
static_assert(sizeof(c_network_session_manager) == 0xC);

