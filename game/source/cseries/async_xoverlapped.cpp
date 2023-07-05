#include "cseries/async_xoverlapped.hpp"

#include "memory/module.hpp"


//HOOK_DECLARE(0x005A8F40, overlapped_task_start_internal);
HOOK_DECLARE_CALL(0x0074CA3C, overlapped_task_start_internal); // c_content_enumeration_overlapped_task::success
HOOK_DECLARE_CALL(0x00A82786, overlapped_task_start_internal); // user_interface_session_string_verify_update

//HOOK_DECLARE_CALL(0x00AA4306, overlapped_task_start_internal); // c_player_profile_interface::update_service_tag

HOOK_DECLARE_CALL(0x00AD3699, overlapped_task_start_internal); // c_player_marketplace::show_purchase_ui
HOOK_DECLARE_CALL(0x00AD36F6, overlapped_task_start_internal); // c_player_marketplace::start_asset_consume
HOOK_DECLARE_CALL(0x00AD3810, overlapped_task_start_internal); // c_player_marketplace::start_assets_enumerate
HOOK_DECLARE_CALL(0x00AE668B, overlapped_task_start_internal); // c_saved_film_take_screenshot::update
HOOK_DECLARE_CALL(0x00AE6868, overlapped_task_start_internal); // c_saved_film_take_screenshot::update
HOOK_DECLARE_CALL(0x00AEA33B, overlapped_task_start_internal); // ?
HOOK_DECLARE_CALL(0x00AF0D67, overlapped_task_start_internal); // c_gui_start_menu_hq_service_record_file_share_choose_item::__vftable50
HOOK_DECLARE_CALL(0x00AF0FBE, overlapped_task_start_internal); // c_gui_start_menu_hq_service_record_file_share_choose_item::__vftable49
HOOK_DECLARE_CALL(0x00AF26D6, overlapped_task_start_internal); // c_gui_start_menu_hq_service_record_file_share_item_selected::__vftable50
HOOK_DECLARE_CALL(0x00AF5088, overlapped_task_start_internal); // c_gui_start_menu_hq_screenshots_options::__vftable24
HOOK_DECLARE_CALL(0x00AF52D2, overlapped_task_start_internal); // c_gui_start_menu_hq_screenshots_options::__vftable24
HOOK_DECLARE_CALL(0x00AF8495, overlapped_task_start_internal); // c_start_menu_settings_appearance_emblem::handle_list_item_chosen
HOOK_DECLARE_CALL(0x00B0373C, overlapped_task_start_internal); // c_gui_screen_pregame_selection::begin_selected_item_text_edit
HOOK_DECLARE_CALL(0x00B0B8D6, overlapped_task_start_internal); // ?
HOOK_DECLARE_CALL(0x00B0BAA0, overlapped_task_start_internal); // ?
HOOK_DECLARE_CALL(0x00B0DA48, overlapped_task_start_internal); // c_gui_screen_game_options::update_save_as_new_operation

c_overlapped_task* c_overlapped_task::constructor(char const* file, long line)
{
	//DECLFUNC(0x005A8C80, c_overlapped_task*, __thiscall, c_overlapped_task const*, char const*, long)(this, file, line);

	REFERENCE_DECLARE(this, dword, vftable);
	vftable = 0x0165B220;
	
	set_file(file);
	set_line(line);

	//set_task_state_internal();
	m_task_flags = 0;
	m_task_state = _overlapped_task_state_none;

	return this;
}

bool c_overlapped_task::busy() const
{
	return m_task_state == _overlapped_task_state_starting
		|| m_task_state == _overlapped_task_state_pending
		|| m_task_state == _overlapped_task_state_completing;
}

void __cdecl overlapped_render()
{
	INVOKE(0x005A8E20, overlapped_render);
}

bool __stdcall overlapped_task_start_internal(c_overlapped_task* task, const char* file, int line)
{
	ASSERT(task);
	task->set_file(file);
	task->set_line(line);

	dword result = task->start(get_donkey_module());

	if (result == 0x80004005 || result == 0xFFFFFFFF)
		return false;

	task->m_task_state = _overlapped_task_state_succeeded;

	return true;
}

