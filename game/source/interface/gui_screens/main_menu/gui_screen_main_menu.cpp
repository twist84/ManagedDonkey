#include "interface/gui_screens/main_menu/gui_screen_main_menu.hpp"

#include "memory/module.hpp"

// patch to disable call for `data->set_disabled_element(STRING_ID(global, name), STRING_ID(global, leave_game))`
byte const set_list_elements_patch_bytes[] = { 0x74, 0x25 };
DATA_PATCH_DECLARE(0x00AE8163, set_list_elements, set_list_elements_patch_bytes);

