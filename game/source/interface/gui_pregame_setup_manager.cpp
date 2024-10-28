#include "interface/gui_pregame_setup_manager.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE(0x0191D750, c_gui_pregame_setup_manager, g_gui_pregame_setup_manager);

c_gui_pregame_setup_manager* __cdecl c_gui_pregame_setup_manager::get()
{
	//return INVOKE(0x00AA6C80, get);

	return &g_gui_pregame_setup_manager;
}

void c_gui_pregame_setup_manager::update()
{
	INVOKE_CLASS_MEMBER(0x00AA8A00, c_gui_pregame_setup_manager, update);
}

