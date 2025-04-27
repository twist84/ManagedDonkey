#include "game/campaign_metagame.hpp"

#include "game/game.hpp"
#include "render/simple_font.hpp"

REFERENCE_DECLARE(0x0244A898, bool, chud_debug_metagame);
REFERENCE_DECLARE(0x0244A8A0, s_campaign_metagame_runtime_globals_secondary, g_campaign_metagame_runtime_globals_secondary);
REFERENCE_DECLARE(0x0244C9B8, uint32, dword_244C9B8);

bool debug_skulls = false;

void __cdecl campaign_metagame_dispose()
{
	INVOKE(0x00606F90, campaign_metagame_dispose);
}

void __cdecl campaign_metagame_dispose_from_old_map()
{
	INVOKE(0x00606FB0, campaign_metagame_dispose_from_old_map);
}

void __cdecl campaign_metagame_initialize()
{
	INVOKE(0x00607C80, campaign_metagame_initialize);
}

void __cdecl campaign_metagame_initialize_for_new_map()
{
	INVOKE(0x00607CE0, campaign_metagame_initialize_for_new_map);
}

void __cdecl campaign_metagame_update()
{
	INVOKE(0x0060A4F0, campaign_metagame_update);
}

bool __cdecl sub_60B080()
{
	return INVOKE(0x0060B080, sub_60B080);
}

void render_debug_campaign_metagame()
{
	if (debug_skulls)
	{
		c_simple_font_screen_display screen_display{};
		if (screen_display.open_session(1.0f))
		{
			long text_y_offset = 0;

			screen_display.draw(0, text_y_offset++, 0xFFFFFFFF, "PRIMARY SKULLS");
			for (e_primary_skulls primary_skull = _campaign_skull_iron; primary_skull < k_number_of_primary_skulls; primary_skull = e_primary_skulls(primary_skull + 1))
			{
				bool active = game_skull_is_active_primary(primary_skull);
				screen_display.draw(4, text_y_offset++, active ? 0xFF00FF00 : 0xFFFF0000, "%s", global_primary_skull_names[primary_skull]);
			}

			screen_display.draw(0, text_y_offset++, 0xFFFFFFFF, "SECONDARY SKULLS");
			for (e_secondary_skulls secondary_skull = _campaign_skull_assassin; secondary_skull < k_number_of_secondary_skulls; secondary_skull = e_secondary_skulls(secondary_skull + 1))
			{
				bool active = game_skull_is_active_secondary(secondary_skull);
				screen_display.draw(4, text_y_offset++, active ? 0xFF00FF00 : 0xFFFF0000, "%s", global_secondary_skull_names[secondary_skull]);
			}
		}
		screen_display.close_session();
	}
}

