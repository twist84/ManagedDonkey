#include "main/levels.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "networking/tools/network_blf.hpp"

HOOK_DECLARE(0x0054A2A0, levels_add_campaign);
HOOK_DECLARE(0x0054A4E0, levels_add_map_from_scripting);
HOOK_DECLARE(0x0054A6A0, levels_add_fake_map_from_scripting);
HOOK_DECLARE(0x0054A6C0, levels_add_level);

HOOK_DECLARE(0x0054AEC0, levels_dvd_enumeration_callback2);
HOOK_DECLARE(0x0054B250, levels_dvd_enumeration_callback);

HOOK_DECLARE(0x0054C320, levels_map_id_is_fake);

HOOK_DECLARE(0x0054C530, levels_process_level_configuration_file);


void __cdecl levels_add_campaign(s_blf_chunk_campaign const* campaign, bool byte_swap, char const* maps_path, bool is_dlc)
{
    FUNCTION_BEGIN(true);

    HOOK_INVOKE(, levels_add_campaign, campaign, byte_swap, maps_path, is_dlc);
}

void __cdecl levels_add_map_from_scripting(long map_id, char const* scenario_path)
{
    FUNCTION_BEGIN(true);

    HOOK_INVOKE(, levels_add_map_from_scripting, map_id, scenario_path);
}

void __cdecl levels_add_fake_map_from_scripting(char const* scenario_path)
{
    FUNCTION_BEGIN(true);

    //HOOK_INVOKE(, levels_add_fake_map_from_scripting, scenario_path);

    levels_add_map_from_scripting(-2, scenario_path);
}

s_level_datum const* __cdecl levels_add_level(s_blf_chunk_scenario const* scenario, bool byte_swap, char const* maps_path, bool is_dlc)
{
    FUNCTION_BEGIN(true);

    s_level_datum const* result = nullptr;
    HOOK_INVOKE(result =, levels_add_level, scenario, byte_swap, maps_path, is_dlc);

    return result;
}

// searches for `campaign`, `mapinfo`, `xex` and `preorder_unlock.txt`
long __cdecl levels_dvd_enumeration_callback2(void* userdata)
{
    FUNCTION_BEGIN(true);

    long result = 0;
    HOOK_INVOKE(result =, levels_dvd_enumeration_callback2, userdata);

    return result;
}

// searches for `map`
long __cdecl levels_dvd_enumeration_callback(s_levels_dvd_enumeration_callback_data* userdata)
{
    FUNCTION_BEGIN(true);

    long result = 0;
    HOOK_INVOKE(result =, levels_dvd_enumeration_callback, userdata);

    return result;
}

//bool __cdecl levels_map_id_is_fake(e_map_id map_id)
bool __cdecl levels_map_id_is_fake(long map_id)
{
    FUNCTION_BEGIN(true);

    //bool result = false;
    //HOOK_INVOKE(result =, levels_map_id_is_fake, map_id);
    //return result;

    return map_id == -2;
}

void __cdecl levels_process_level_configuration_file(s_file_reference* file, wchar_t const* maps_path, bool unused)
{
    FUNCTION_BEGIN(true);

    HOOK_INVOKE(, levels_process_level_configuration_file, file, maps_path, unused);
}
