#include "networking/logic/network_bandwidth.hpp"

bool __cdecl network_bandwidth_initialize(c_network_observer* observer, s_bandwidth_configuration const* bandwidth_configuration)
{
    return INVOKE(0x004557A0, network_bandwidth_initialize, observer, bandwidth_configuration);
}

