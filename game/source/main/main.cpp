#include "main/main.hpp"

#include "cache/cache_files.hpp"
#include "cache/cache_files_windows.hpp"
#include "camera/director.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries_windows.hpp"
#include "cseries/symbols_reader.hpp"
#include "game/game.hpp"
#include "game/game_globals.hpp"
#include "game/player_control.hpp"
#include "interface/c_player_marketplace.hpp"
#include "interface/gui_custom_bitmap_storage.hpp"
#include "interface/user_interface_hs.hpp"
#include "interface/user_interface_networking.hpp"
#include "main/global_preferences.hpp"
#include "main/main_game_launch.hpp"
#include "memory/data.hpp"
#include "memory/module.hpp"
#include "networking/logic/logic_qos_reply_manager.hpp"
#include "networking/logic/network_arbitration.hpp"
#include "networking/logic/network_join.hpp"
#include "networking/logic/storage/network_http_request_queue.hpp"
#include "networking/network_memory.hpp"
#include "networking/tools/network_debug_dump.hpp"
#include "networking/transport/transport.hpp"
#include "networking/transport/transport_security.hpp"
#include "rasterizer/rasterizer.hpp"
#include "scenario/scenario.hpp"
#include "simulation/simulation.hpp"
#include "tag_files/string_ids.hpp"
#include "xbox/xnet.hpp"

#include <assert.h>

HOOK_DECLARE_CALL(0x00505C2B, main_loop_body_begin);
HOOK_DECLARE_CALL(0x0050605C, main_loop_body_end);

void __cdecl main_loop_body_begin()
{
    FUNCTION_BEGIN(false);

    // right control for tests
    if (GetKeyState(VK_RCONTROL) & 0x8000)
    {
        xnet_mapping;
        g_player_marketplace;
        g_broadcast_search_globals;
        network_arbitration_globals;
        g_netdebug_globals;
        g_network_http_request_queue;
        g_gui_custom_bitmap_storage_manager;
        network_base_memory_globals;
        transport_globals;
        transport_security_globals;
        logic_qos_reply_manager_globals;
        user_interface_networking_globals;
        g_network_join_data;
        cache_file_table_of_contents;
        cache_file_copy_globals;
        simulation_globals;
        g_cache_file_globals;
        global_game_globals;
        game_options* options = game_options_get();

        printf("");
    }

    if (GetKeyState(VK_PAUSE) & 0x8000)
    {
        static bool using_controller = 0;
        global_preferences_set_controls_method((using_controller = !using_controller) ? 1 : 0);
        *(bool*)0x0244DE98 = using_controller;
        Sleep(75);
    }
}

void __cdecl main_loop_body_end()
{
    FUNCTION_BEGIN(false);

    bool key_pressed = false;

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
        key_pressed = true;
    }
    else if (GetKeyState(VK_DELETE) & 0x8000)
    {
        static bool elite_hanger = false;

        if (game_is_ui_shell())
            user_interface_start_hs_script_by_name((elite_hanger = !elite_hanger) ? "elitehangar" : "humanhangar");

        key_pressed = true;
    }
    else if (GetKeyState(VK_HOME) & 0x8000)
    {
        //main_game_launch_set_multiplayer_splitscreen_count(1);
        main_game_launch_set_coop_player_count(1);
        main_game_launch("maps\\riverworld");
        key_pressed = true;
    }
    else if (GetKeyState(VK_END) & 0x8000)
    {
        director_toggle(main_game_launch_get_last_player(), _director_mode_debug);
        key_pressed = true;
    }
    else if (GetKeyState(VK_PRIOR) & 0x8000)
    {
        player_control_toggle_machinima_camera_enabled();
        player_control_toggle_machinima_camera_debug();
        key_pressed = true;
    }
    else if (GetKeyState(VK_NEXT) & 0x8000)
    {
        player_control_toggle_machinima_camera_use_old_controls();
        key_pressed = true;
    }

    if (key_pressed)
        Sleep(75);
}
