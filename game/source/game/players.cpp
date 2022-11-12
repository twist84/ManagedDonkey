#include "players.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries_windows.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00536680, player_get_weapon_loadout);

long __cdecl players_first_active_user()
{
	FUNCTION_BEGIN(true);

	return INVOKE(0x00589A30, players_first_active_user);
}

bool __cdecl player_teleport(long player_index, long object_index, real_point3d const* position)
{
	return INVOKE(0x0053F550, player_teleport, player_index, object_index, position);
}

s_s3d_player_weapon_configuration_loadout* __cdecl player_get_weapon_loadout(player_datum* player)
{
	s_s3d_player_weapon_configuration_loadout& loadout = player->configuration.host.weapon_configuration.loadouts[player->active_weapon_loadout];

	// allow player traits override
	loadout.primary_weapon_index = 0xFF;
	loadout.secondary_weapon_index = 0xFF;

	loadout.grenade_index = _grenade_type_firebomb;

	loadout.consumables[0] = 5; // #TODO: replace with enum
	loadout.consumables[1] = 6; // #TODO: replace with enum
	loadout.consumables[2] = 7; // #TODO: replace with enum
	loadout.consumables[3] = 8; // #TODO: replace with enum

	return &loadout;
}
