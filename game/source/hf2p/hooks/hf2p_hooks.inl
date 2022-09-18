#pragma once

#include "memory/module.hpp"

#include "hf2p/hf2p.hpp"

c_hook_call<0x00530CFF> hf2p_dispose_hook({ .pointer = hf2p_dispose });
c_hook_call<0x005315C0> hf2p_game_initialize_hook({ .pointer = hf2p_game_initialize });