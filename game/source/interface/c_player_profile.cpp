#include "interface/c_player_profile.hpp"

REFERENCE_DECLARE(0x0191D4FC, bool, g_controller_all_solo_levels_unlocked);

bool g_controller_all_solo_levels_unlocked_unset = []() -> bool
{
	g_controller_all_solo_levels_unlocked = false;

	return g_controller_all_solo_levels_unlocked == false;
}();

