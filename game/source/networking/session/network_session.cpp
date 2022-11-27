#include "networking/session/network_session.hpp"

#include "cseries/console.hpp"

c_network_session_parameters* c_network_session::get_session_parameters()
{
    //return DECLFUNC(0x0045C2A0, c_network_session_parameters*, __thiscall, c_network_session*)(this);

    return &m_session_parameters;
}

bool c_network_session::established()
{
    return m_local_state > 3;
}
