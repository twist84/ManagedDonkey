#pragma once

#include "memory/module.hpp"

#include "game/game.hpp"
#include "game/game_engine_util.hpp"

// called from `game_engine_running`
c_hook_call<0x00552410> current_game_engine_hook({ .pointer = current_game_engine });

c_hook_call<0x005677AA> game_options_get_launch_settings_hook({ .pointer = game_options_get_launch_settings });
