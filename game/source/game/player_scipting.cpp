#include "game/player_scipting.hpp"

#include "game/player_mapping.hpp"
#include "units/bipeds.hpp"

void hs_player_ragdoll()
{
	int32 user_index = player_mapping_first_active_output_user();
	int32 unit_index = player_mapping_get_unit_by_output_user(user_index);
	if (unit_index != NONE)
	{
		biped_ragdoll(unit_index);
	}
}

