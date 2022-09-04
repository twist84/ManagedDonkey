#pragma once

struct c_network_session;
struct c_network_session_manager
{
	c_network_session* m_sessions[3];
};
static_assert(sizeof(c_network_session_manager) == 0xC);