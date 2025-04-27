#include "interface/gui_selected_items.hpp"

#include "memory/module.hpp"

// replace call `c_load_pregame_players_selection_screen_message` with `c_load_pregame_selection_screen_message`
uint32 load_pregame_selection_screen_message_ctor = 0x00AC34D0;
HOOK_DECLARE_CALL_WITH_ADDRESS(0x00B21F33, load_pregame_selection_screen_message_ctor, load_pregame_players_selection_screen_message_ctor);