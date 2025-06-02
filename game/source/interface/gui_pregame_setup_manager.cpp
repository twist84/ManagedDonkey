#include "interface/gui_pregame_setup_manager.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE(0x0191D750, c_gui_pregame_setup_manager, g_gui_pregame_setup_manager);

c_gui_pregame_setup_manager* __cdecl c_gui_pregame_setup_manager::get()
{
	//return INVOKE(0x00AA6C80, get);

	return &g_gui_pregame_setup_manager;
}

//.text:00AA6C90 ; 
//.text:00AA6CA0 ; 
//.text:00AA6CB0 ; 
//.text:00AA6CC0 ; 
//.text:00AA6CF0 ; 
//.text:00AA6D00 ; 
//.text:00AA6D10 ; 
//.text:00AA6D20 ; 
//.text:00AA6D30 ; 
//.text:00AA6D40 ; 
//.text:00AA6D50 ; 
//.text:00AA6D60 ; 
//.text:00AA6D70 ; 
//.text:00AA6D80 ; 
//.text:00AA6D90 ; 
//.text:00AA6DA0 ; public: int32 c_gui_pregame_setup_manager::get_selected_item_category_id(e_gui_selected_item_type)
//.text:00AA6EA0 ; public: bool c_gui_pregame_setup_manager::get_selected_item_title(wchar_t*, int32, e_gui_selected_item_type)
//.text:00AA70C0 ; 
//.text:00AA70D0 ; public: bool c_gui_pregame_setup_manager::handle_dialog_result(c_dialog_result_message const*)
//.text:00AA7130 ; public: bool c_gui_pregame_setup_manager::initialize_game_mode(e_gui_game_mode, bool)

void c_gui_pregame_setup_manager::restore_from_last(e_gui_game_mode lobby)
{
	INVOKE_CLASS_MEMBER(0x00AA7190, c_gui_pregame_setup_manager, restore_from_last, lobby);
}

//.text:00AA7270 ; public: void c_gui_pregame_setup_manager::restore_game_setup(e_gui_game_mode, s_gui_game_setup_storage*)
//.text:00AA72A0 ; private: void c_gui_pregame_setup_manager::restore_game_setup_internal(e_gui_game_mode, s_gui_game_setup_storage*)
//.text:00AA7B00 ; public: static void __cdecl c_gui_pregame_setup_manager::save_game_setup(s_gui_game_setup_storage const*, c_gui_single_game_setup_storage*)
//.text:00AA7D90 ; 
//.text:00AA7DB0 ; public: void c_gui_pregame_setup_manager::set_selected_item(e_controller_index, c_gui_selected_item const*, bool, int32)
//.text:00AA8760 ; 
//.text:00AA8840 ; 
//.text:00AA88E0 ; 
//.text:00AA89C0 ; public: bool c_gui_pregame_setup_manager::try_to_get_last_set_game_mode(e_gui_game_mode*) const

void c_gui_pregame_setup_manager::update()
{
	INVOKE_CLASS_MEMBER(0x00AA8A00, c_gui_pregame_setup_manager, update);
}

//.text:00AA8A40 ; 

