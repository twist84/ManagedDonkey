#pragma once

#include "interface/user_interface_networking.hpp"
#include "main/global_preferences.hpp"

class c_gui_selected_item;

#pragma pack(push, 4)

class c_gui_pregame_setup_manager
{
public:
	void __thiscall set_selected_item_(e_controller_index controller_index, const c_gui_selected_item* selected_item, bool selected_from_recent_list, int32 source_screen_index_to_dispose);

public:
	static c_gui_pregame_setup_manager* __cdecl get();
	static bool __cdecl get_selected_item_title(wchar_t* buffer, int32 buffer_count, e_gui_selected_item_type selection_type);
	void restore_from_last(e_gui_game_mode lobby);
	void set_selected_item(e_controller_index controller_index, const c_gui_selected_item* selected_item, bool selected_from_recent_list, int32 source_screen_index_to_dispose);
	void update();

//protected:
	bool m_selected_from_recent_list[8];
	int32 m_network_mode_change_request;
	s_gui_single_game_setup_storage m_storage_to_set_on_next_update;
	e_gui_game_mode m_game_mode;
	bool m_start_countdown_when_ready;
};
static_assert(sizeof(c_gui_pregame_setup_manager) == 0x24E98);

#pragma pack(pop)

extern c_gui_pregame_setup_manager& g_gui_pregame_setup_manager;

