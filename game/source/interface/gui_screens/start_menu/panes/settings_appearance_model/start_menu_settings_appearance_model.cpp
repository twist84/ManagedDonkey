#include "interface/gui_screens/start_menu/panes/settings_appearance_model/start_menu_settings_appearance_model.hpp"

#include "memory/module.hpp"

HOOK_DECLARE(0x00AF94C0, start_menu_pane_parse_part_count_current);
HOOK_DECLARE(0x00AF9570, start_menu_pane_parse_part_count_total);
HOOK_DECLARE(0x00AF9600, start_menu_pane_parse_part_description);

bool __cdecl start_menu_pane_parse_part_count_current(void* this_ptr, wchar_t* buffer, long buffer_size)
{
    return false;
}

bool __cdecl start_menu_pane_parse_part_count_total(void* this_ptr, wchar_t* buffer, long buffer_size)
{
    return false;
}

bool __cdecl start_menu_pane_parse_part_description(void* this_ptr, wchar_t* buffer, long buffer_size)
{
    return false;
}

