#include "main/levels.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "networking/tools/network_blf.hpp"

HOOK_DECLARE(0x0054A2A0, levels_add_campaign);
HOOK_DECLARE(0x0054A4E0, levels_add_map_from_scripting);
HOOK_DECLARE(0x0054A6A0, levels_add_fake_map_from_scripting);
HOOK_DECLARE(0x0054A6C0, levels_add_level);
HOOK_DECLARE(0x0054AD80, levels_begin_dvd_enumeration);
//HOOK_DECLARE(0x0054ADF0, levels_dispose);
//HOOK_DECLARE(0x0054AEA0, levels_dispose_from_old_map);
HOOK_DECLARE(0x0054AEC0, levels_dvd_enumeration_callback2);
HOOK_DECLARE(0x0054B250, levels_dvd_enumeration_callback);
//HOOK_DECLARE(0x0054B610, levels_get_available_map_mask);
//HOOK_DECLARE(0x0054B670, levels_get_campaign_count);
//HOOK_DECLARE(0x0054B6A0, levels_get_campaign_id_from_path);
//HOOK_DECLARE(0x0054B7B0, levels_get_campaign_level_count);
//HOOK_DECLARE(0x0054B8A0, levels_get_campaign_level_index);
//HOOK_DECLARE(0x0054B9D0, levels_get_campaign_map_by_display_name);
//HOOK_DECLARE(0x0054BAA0, levels_get_campaign_map_ids);
//HOOK_DECLARE(0x0054BB80, levels_get_campaign_next_map_id);
//HOOK_DECLARE(0x0054BCA0, levels_get_default_multiplayer_map_id);
//HOOK_DECLARE(0x0054BDB0, levels_get_checksum);
//HOOK_DECLARE(0x0054BE10, levels_get_multiplayer_map_by_display_name);
//HOOK_DECLARE(0x0054BEE0, levels_get_multiplayer_map_ids);
//HOOK_DECLARE(0x0054C040, levels_get_path);
//HOOK_DECLARE(0x0054C110, levels_initialize);
//HOOK_DECLARE(0x0054C2D0, levels_initialize_for_new_map);
HOOK_DECLARE(0x0054C320, levels_map_id_is_fake);
//HOOK_DECLARE(0x0054C330, levels_open_dlc);
//HOOK_DECLARE(0x0054C360, levels_path_is_dlc);
//HOOK_DECLARE(0x0054C3C0, levels_process_campaign_configuration_file);
HOOK_DECLARE(0x0054C530, levels_process_level_configuration_file);
//HOOK_DECLARE(0x0054C820, levels_delete);
//HOOK_DECLARE(0x0054C910, levels_try_and_get_by_map_id);
//HOOK_DECLARE(0x0054C9E0, levels_try_and_get_campaign_insertion);
//HOOK_DECLARE(0x0054CAB0, levels_try_and_get_campaign_map);
//HOOK_DECLARE(0x0054CAD0, levels_try_and_get_main_menu_map);
//HOOK_DECLARE(0x0054CB00, levels_try_and_get_multiplayer_map);
//HOOK_DECLARE(0x0054CB20, levels_update);

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

void __cdecl levels_add_level(s_blf_chunk_scenario const* scenario, bool byte_swap, wchar_t const* maps_path, bool is_dlc)
{
    FUNCTION_BEGIN(true);

    HOOK_INVOKE(, levels_add_level, scenario, byte_swap, maps_path, is_dlc);
}

bool __cdecl levels_begin_dvd_enumeration()
{
    FUNCTION_BEGIN(true);

    bool result = false;
    HOOK_INVOKE(result =, levels_begin_dvd_enumeration);
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
