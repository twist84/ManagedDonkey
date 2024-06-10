#include "interface/c_player_profile.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE(0x0191D4FC, bool, g_controller_all_solo_levels_unlocked);

t_value_type<bool> const controller_all_solo_levels_unlocked = { .value = false };
DATA_PATCH_DECLARE(0x0191D4FC, controller_all_solo_levels_unlocked, controller_all_solo_levels_unlocked.bytes);

