#pragma once

#include "memory/module.hpp"

#include "main/main.hpp"
#include "main/loading.hpp"

c_hook_call<0x00505C2B> main_loop_body_begin_hook({ .pointer = main_loop_body_begin });
c_hook_call<0x0050605C> main_loop_body_end_hook({ .pointer = main_loop_body_end });

// test dirty disc error hook
c_hook_call<0x00567964> main_load_map_hook({ .pointer = main_load_map });
