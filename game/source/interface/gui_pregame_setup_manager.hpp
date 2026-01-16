#pragma once

#include "interface/user_interface_networking.hpp"
#include "main/global_preferences.hpp"

class c_gui_difficulty_selected_item;
class c_gui_game_variant_selected_item;
class c_gui_hopper_selected_item;
class c_gui_level_selected_item;
class c_gui_map_selected_item;
class c_gui_network_mode_selected_item;
class c_gui_saved_film_selected_item;
class c_gui_saved_screenshot_selected_item;
class c_gui_selected_item;

#pragma pack(push, 4)

class c_gui_pregame_setup_manager
{
public:
	void __thiscall set_selected_item_(e_controller_index controller_index, const c_gui_selected_item* selected_item, bool selected_from_recent_list, int32 source_screen_index_to_dispose);

private:
	void change_advertisement_mode(e_gui_network_session_advertisement_mode advertisement_mode, e_network_session_class current_session_class, e_network_session_class desired_session_class, int32 source_screen_index_to_dispose);

public:
	static c_gui_pregame_setup_manager* __cdecl get();
	static bool __cdecl get_selected_item_title(wchar_t* buffer, int32 buffer_count, e_gui_selected_item_type selection_type);
	void restore_from_last(e_gui_game_mode lobby);
	void restore_game_setup(const s_gui_single_game_setup_storage* storage);
	void set_selected_item(e_controller_index controller_index, const c_gui_selected_item* selected_item, bool selected_from_recent_list, int32 source_screen_index_to_dispose);

private:
	bool set_selected_item_type_difficulty(e_controller_index controller_index, const c_gui_difficulty_selected_item* difficulty_selected_item, bool selected_from_recent_list, int32& source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage);
	bool set_selected_item_type_film(e_controller_index controller_index, const c_gui_saved_film_selected_item* film_selected_item, bool selected_from_recent_list, int32& source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage);
	bool set_selected_item_type_game_variant(e_controller_index controller_index, const c_gui_game_variant_selected_item* game_variant_selected_item, bool selected_from_recent_list, int32& source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage);
	bool set_selected_item_type_hopper(e_controller_index controller_index, const c_gui_hopper_selected_item* hopper_selected_item, bool selected_from_recent_list, int32& source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage);
	bool set_selected_item_type_level(e_controller_index controller_index, const c_gui_level_selected_item* level_selected_item, bool selected_from_recent_list, int32& source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage);
	bool set_selected_item_type_map(e_controller_index controller_index, const c_gui_map_selected_item* map_selected_item, bool selected_from_recent_list, int32& source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage);
	bool set_selected_item_type_network_mode(e_controller_index controller_index, const c_gui_network_mode_selected_item* network_mode_selected_item, bool selected_from_recent_list, int32& source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage);
	bool set_selected_item_type_screenshot(e_controller_index controller_index, const c_gui_saved_screenshot_selected_item* screenshot_selected_item, bool selected_from_recent_list, int32& source_screen_index_to_dispose, s_gui_single_game_setup_storage& game_setup_storage);

public:
	void update();

//protected:
	bool m_selected_from_recent_list[8];
	int32 m_network_mode_change_request;
	s_gui_single_game_setup_storage m_storage_to_set_on_next_update;
	e_gui_game_mode m_game_mode;
	bool m_start_countdown_when_ready;
};
COMPILE_ASSERT(sizeof(c_gui_pregame_setup_manager) == 0x24E98);

#pragma pack(pop)

extern c_gui_pregame_setup_manager& g_gui_pregame_setup_manager;

