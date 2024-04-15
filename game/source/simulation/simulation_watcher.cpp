#include "simulation/simulation_watcher.hpp"

bool c_simulation_watcher::need_to_generate_updates() const
{
    return DECLFUNC(0x0046D090, bool, __thiscall, c_simulation_watcher const*)(this);
}

