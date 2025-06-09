#include "game/campaign_metagame.hpp"

#include "game/game.hpp"
#include "interface/c_gui_screen_widget.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "render/simple_font.hpp"
#include "shell/shell.hpp"

REFERENCE_DECLARE(0x0244A898, bool, chud_debug_metagame);
REFERENCE_DECLARE(0x0244A8A0, s_campaign_metagame_runtime_globals_secondary, g_campaign_metagame_runtime_globals_secondary);
REFERENCE_DECLARE(0x0244C9B8, uns32, dword_244C9B8);

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

bool __cdecl campaign_skull_is_active(e_campaign_skulls_primary primary_skull)
{
	return INVOKE(0x0060AE20, campaign_skull_is_active, primary_skull);
}

void __cdecl metagame_postgame_dismiss_ui()
{
	//INVOKE(0x0060AFD0, metagame_postgame_dismiss_ui);

	if (c_gui_screen_widget* screen = window_manager_get()->get_screen_by_name(k_number_of_player_windows, STRING_ID(gui, carnage_report)))
	{
		screen->transition_out(_transition_out_normal);
	}
}

bool __cdecl metagame_postgame_in_progress()
{
	return INVOKE(0x0060B080, metagame_postgame_in_progress);
}

void render_debug_campaign_metagame()
{
	if (debug_skulls)
	{
		c_simple_font_screen_display screen_display{};
		if (screen_display.open_session(1.0f))
		{
			int32 text_y_offset = 0;

			screen_display.draw(0, text_y_offset++, 0xFFFFFFFF, "PRIMARY SKULLS");
			for (e_campaign_skulls_primary primary_skull = _campaign_skull_primary_iron; primary_skull < k_campaign_skull_primary_count; primary_skull = e_campaign_skulls_primary(primary_skull + 1))
			{
				bool active = game_skull_is_active_primary(primary_skull);
				screen_display.draw(4, text_y_offset++, active ? 0xFF00FF00 : 0xFFFF0000, "%s", global_primary_skull_names[primary_skull]);
			}

			screen_display.draw(0, text_y_offset++, 0xFFFFFFFF, "SECONDARY SKULLS");
			for (e_campaign_skulls_secondary secondary_skull = _campaign_skull_secondary_assassin; secondary_skull < k_campaign_skull_secondary_count; secondary_skull = e_campaign_skulls_secondary(secondary_skull + 1))
			{
				bool active = game_skull_is_active_secondary(secondary_skull);
				screen_display.draw(4, text_y_offset++, active ? 0xFF00FF00 : 0xFFFF0000, "%s", global_secondary_skull_names[secondary_skull]);
			}
		}
		screen_display.close_session();
	}
}

