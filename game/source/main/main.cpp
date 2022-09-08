#include "main/main.hpp"

#include "camera/director.hpp"
#include "cseries/symbols_reader.hpp"
#include "game/player_control.hpp"
#include "main/global_preferences.hpp"
#include "main/main_game_launch.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"

#include <windows.h>
#include <assert.h>
#include <stdio.h>

void main_loop_body_begin()
{
    // right control for tests
    if (GetKeyState(VK_RCONTROL) & 0x8000)
    {
        dword address = symbols_reader.get_rva_blocking(L"?main_loop_body_main_part@@YAXXZ");
        assert(address == 0 || global_address_get(address) == 0x00505C10);

        printf("");
    }
}

void main_loop_body_end()
{
    // home cluster keys
    if (GetKeyState(VK_INSERT) & 0x8000)
    {
        global_preferences_set_shadow_quality(_quality_setting_high);
        global_preferences_set_texture_resolution_quality(_quality_setting_high);
        global_preferences_set_texture_filtering_quality(_quality_setting_high);
        global_preferences_set_lighting_quality(_quality_setting_high);
        global_preferences_set_effects_quality(_quality_setting_high);
        global_preferences_set_details_quality(_quality_setting_high);
        global_preferences_set_postprocessing_quality(_quality_setting_high);
        global_preferences_set_vsync(true);
        global_preferences_set_motion_blur(false);
        global_preferences_set_antialiasing(true);
        global_preferences_set_brightness(50);
        global_preferences_set_contrast(50);

        global_preferences_set_fullscreen(true);
        sub_79BA30(1920, 1080);
    }
    else if (GetKeyState(VK_DELETE) & 0x8000)
    {
    }
    else if (GetKeyState(VK_HOME) & 0x8000)
    {
        main_game_launch_set_multiplayer_splitscreen_count(2);
        //main_game_launch_set_coop_player_count(2);
        main_game_launch("maps\\riverworld");
    }
    else if (GetKeyState(VK_END) & 0x8000)
    {
        director_toggle(main_game_launch_get_last_player(), _director_mode_debug);
        Sleep(75);
    }
    else if (GetKeyState(VK_PRIOR) & 0x8000)
    {
        player_control_toggle_machinima_camera_enabled();
        player_control_toggle_machinima_camera_debug();
        Sleep(75);
    }
    else if (GetKeyState(VK_NEXT) & 0x8000)
    {
        player_control_toggle_machinima_camera_use_old_controls();
        Sleep(75);
    }
}
