#include "memory/module.hpp"

#include "camera/director.hpp"
#include "game/player_control.hpp"
#include "main/global_preferences.hpp"
#include "main/main_game_launch.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "rasterizer/rasterizer.hpp"
#include "rasterizer/rasterizer_performance_throttles.hpp"

#include <windows.h>
#include <stdio.h>

static module_address global_module = { .pointer = GetModuleHandle(NULL) };

template<dword address>
inline c_hook_call<address>::c_hook_call(module_address const function, bool remove_base) :
    m_addr({ .address = global_module.address + (remove_base ? address - 0x00400000 : address) }),
    m_call({ .opcode = 0xE8, .offset = (function.address - m_addr.address - sizeof(call_instruction)) })
{
    memcpy(&m_call_original, m_addr.pointer, sizeof(call_instruction));
    apply();
}

template<dword address>
bool c_hook_call<address>::apply()
{
    dword protect;
    if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), PAGE_READWRITE, &protect))
        return false;

    memcpy(m_addr.pointer, &m_call, sizeof(call_instruction));

    if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), protect, &protect))
        return false;

    return true;
}

template<dword address>
bool c_hook_call<address>::revert()
{
    dword protect;
    if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), PAGE_READWRITE, &protect))
        return false;

    memcpy(m_addr.pointer, &m_call_original, sizeof(call_instruction));

    if (!VirtualProtect(m_addr.pointer, sizeof(call_instruction), protect, &protect))
        return false;

    return true;
}

template<dword address>
template<long k_patch_size>
c_data_patch<address>::c_data_patch(byte const(&bytes)[k_patch_size], bool remove_base) :
    m_addr({ .address = global_module.address + (remove_base ? address - 0x00400000 : address) }),
    m_byte_count(k_patch_size),
    m_bytes(bytes),
    m_bytes_original(new byte[m_byte_count]{})
{
    memcpy(m_bytes_original, m_addr.pointer, k_patch_size);
    apply();
}

template<dword address>
template<typename t_type, long k_patch_size>
c_data_patch<address>::c_data_patch(t_type const bytes, bool remove_base) :
    m_addr({ .address = global_module.address + (remove_base ? address - 0x00400000 : address) }),
    m_byte_count(k_patch_size),
    m_bytes(new byte[m_byte_count]{}),
    m_bytes_original(new byte[m_byte_count]{})
{
    memcpy(m_bytes, &bytes, k_patch_size);
    memcpy(m_bytes_original, m_addr.pointer, k_patch_size);
    apply();
}

template<dword address>
bool c_data_patch<address>::apply()
{
    dword protect;
    if (!VirtualProtect(m_addr.pointer, m_byte_count, PAGE_READWRITE, &protect))
        return false;

    memcpy(m_addr.pointer, m_bytes, m_byte_count);

    if (!VirtualProtect(m_addr.pointer, m_byte_count, protect, &protect))
        return false;

    return true;
}

template<dword address>
bool c_data_patch<address>::revert()
{
    dword protect;
    if (!VirtualProtect(m_addr.pointer, m_byte_count, PAGE_READWRITE, &protect))
        return false;

    memcpy(m_addr.pointer, m_bytes_original, m_byte_count);

    if (!VirtualProtect(m_addr.pointer, m_byte_count, protect, &protect))
        return false;

    return true;
}

void main_loop_body_begin();
void main_loop_body_end();

c_hook_call<0x00505C2B> main_loop_body_begin_call({ .pointer = main_loop_body_begin });
c_hook_call<0x0050605C> main_loop_body_end_call({ .pointer = main_loop_body_end });

void main_loop_body_begin()
{
    // right control for tests
    if (GetKeyState(VK_RCONTROL) & 0x8000)
    {
        life_cycle_globals;

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
        g_ignore_predefined_performance_throttles = true;
        memcpy(&g_current_performance_throttles, &g_default_performance_throttles.throttles[0], sizeof(s_performane_throttle));
    }
    else if (GetKeyState(VK_HOME) & 0x8000)
    {
        //main_game_launch_set_multiplayer_splitscreen_count(4);
        main_game_launch_set_coop_player_count(1);
        main_game_launch("maps\\riverworld");
    }
    else if (GetKeyState(VK_END) & 0x8000)
    {
        director_toggle(0, _director_mode_debug);
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
