#include "hf2p/hf2p.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_windows.hpp"
#include "fmod/fmod.hpp"
#include "game/game_engine_util.hpp"
#include "game/players.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "units/units.hpp"

#include <stdlib.h>

HOOK_DECLARE(0x006006F0, hf2p_game_initialize);
HOOK_DECLARE(0x00600790, hf2p_game_dispose);
HOOK_DECLARE(0x00600850, hf2p_game_update);

REFERENCE_DECLARE(0x018B59D4, bool, g_hf2p_first_run);

void __cdecl game_statistics_reset()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00853FC0, game_statistics_reset);
}

void* hp2p_ui_proxy = reinterpret_cast<void*>(0x0244ED28);

void __cdecl hf2p_initialize()
{
	FUNCTION_BEGIN(true);

	//INVOKE(0x00600630, hf2p_initialize);

	//game_statistics_reset();
	//hp2p_ui_proxy = nullptr;
	//
	//// crashes up the call stack
	//// #TODO: investigate
	//fmod_initialize();
}

void __cdecl hf2p_game_initialize()
{
	FUNCTION_BEGIN(true);

	//HOOK_INVOKE(, hf2p_game_initialize);
}

void __cdecl hf2p_scenario_tags_load_finished()
{
	FUNCTION_BEGIN(true);

	//INVOKE(0x00600750, hf2p_scenario_tags_load_finished);

	g_hf2p_first_run = true;
	if (!g_hf2p_first_run)
	{
		hf2p_initialize();
		g_hf2p_first_run = true;
	}
}

void __cdecl hf2p_scenario_load()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00600770, hf2p_scenario_load);
}

void __cdecl hf2p_game_dispose()
{
	FUNCTION_BEGIN(true);

	//HOOK_INVOKE(, hf2p_game_dispose);
	//
	//fmod_terminate();
}

REFERENCE_DECLARE(0x04FE67A0, dword, mainmenu_unit_index);

void __cdecl hf2p_game_update()
{
	// this function runs in `main_loop_body_main_part`

	// update `mainmenu_unit_index`
	DECLFUNC(0x007B7940, void, __cdecl)();

	if (mainmenu_unit_index != 0xFFFFFFFF)
	{
		{
			s_s3d_player_armor_configuration_loadout loadout{};
			{
				for (long color_index = 0; color_index < 5; color_index++)
					loadout.colors[color_index].value = (system_milliseconds() * rand()) % 0x2F3F4F;

				for (long armor_index = 0; armor_index < 6; armor_index++)
					loadout.armors[armor_index] = (system_milliseconds() * rand()) % 70;
			}
			DECLFUNC(0x005A4430, void, __cdecl, s_s3d_player_armor_configuration_loadout*, dword)(&loadout, mainmenu_unit_index);

			for (long color_index = 0; color_index < 5; color_index++)
			{
				rgb_color& color = loadout.colors[color_index];
				real_rgb_color real_color;
				pixel32_to_real_rgb_color(color, &real_color);
				object_set_base_change_color_by_index(mainmenu_unit_index, color_index, &real_color);
			}
			DECLFUNC(0x005A2FA0, void, __cdecl, dword)(mainmenu_unit_index);
		}

		{
			long weapon_definition_index = game_engine_weapon_item_definition_index_from_absolute_weapons_selection_block_index(/* random */ short(0xFFFD), _weapon_set_primary);
			if (!unit_has_weapon_definition_index(mainmenu_unit_index, weapon_definition_index))
			{
				object_placement_data placement_data{};
				placement_data.__unknown15C = 0;
				placement_data.__unknown163 = 0;
				object_placement_data_new(&placement_data, weapon_definition_index, 0xFFFFFFFF, nullptr);
				placement_data.model_variant_index = 0;
				long object_index = object_new(&placement_data);
				if (object_index != -1 && !unit_add_weapon_to_inventory(mainmenu_unit_index, object_index, 8))
					object_delete(object_index);
			}
		}
	}

	printf("");
}

