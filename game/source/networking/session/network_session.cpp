#include "networking/session/network_session.hpp"

#include "cseries/console.hpp"


c_network_session_membership const* c_network_session::get_session_membership() const
{
    ASSERT(established());
    ASSERT(m_session_membership.has_membership());
    ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
    ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));

    return &m_session_membership;
}

c_network_session_parameters const* c_network_session::get_session_parameters() const
{
    return &m_session_parameters;
}

c_network_session_parameters* c_network_session::get_session_parameters()
{
    //return DECLFUNC(0x0045C2A0, c_network_session_parameters*, __thiscall, c_network_session*)(this);

    return &m_session_parameters;
}

bool c_network_session::established() const
{
    return m_local_state > 3;
}

bool c_network_session::is_leader()
{
    return m_session_membership.m_local_peer_index == m_session_membership.m_shared_network_membership.leader_peer_index;
}

e_network_session_mode c_network_session::session_mode() const
{
    return m_session_parameters.m_parameters_internal.session_mode.get();
}

