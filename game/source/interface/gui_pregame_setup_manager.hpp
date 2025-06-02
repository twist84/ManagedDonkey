#pragma once

#include "interface/user_interface_networking.hpp"
#include "main/global_preferences.hpp"

#pragma pack(push, 4)

struct c_gui_single_game_setup_storage
{
	e_gui_game_mode m_game_mode;

	union
	{
		s_gui_game_setup_storage::s_campaign_settings campaign_settings;
		s_gui_game_setup_storage::s_survival_settings survival_settings;
		s_gui_game_setup_storage::s_matchmaking_settings matchmaking_settings;
		s_gui_game_setup_storage::s_multiplayer_settings multiplayer_settings;
		s_gui_game_setup_storage::s_mapeditor_settings map_editor_settings;
		s_gui_game_setup_storage::s_theater_settings theater_settings;
	};
};
static_assert(sizeof(c_gui_single_game_setup_storage) == 0x24E84);

struct c_gui_pregame_setup_manager
{
public:
	static c_gui_pregame_setup_manager* __cdecl get();
	void restore_from_last(e_gui_game_mode lobby);
	void update();

protected:
	uns8 m_selected_item_category_ids[8];
	int32 m_advertisement_mode;
	c_gui_single_game_setup_storage m_storage;
	e_gui_game_mode m_game_mode;
	bool __unknown24E94;
};
static_assert(sizeof(c_gui_pregame_setup_manager) == 0x24E98);

#pragma pack(pop)

extern c_gui_pregame_setup_manager& g_gui_pregame_setup_manager;

