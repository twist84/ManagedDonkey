#include "networking/online/online_guide_pc.hpp"

#include "cseries/async_xoverlapped_memory.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_controller.hpp"
#include "interface/user_interface_text.hpp"
#include "memory/module.hpp"
#include "xbox/xbox.hpp"

#include <windows.h>
#include <string.h>

//HOOK_DECLARE_CLASS(0x004E16A0, c_virtual_keyboard_task, constructor);
HOOK_DECLARE_CLASS(0x004E1840, c_virtual_keyboard_task, get_instance);
HOOK_DECLARE_CLASS_MEMBER(0x004E19A0, c_virtual_keyboard_task, _set_default_text);
HOOK_DECLARE_CLASS_MEMBER(0x004E19B0, c_virtual_keyboard_task, _set_description_text);
HOOK_DECLARE_CLASS_MEMBER(0x004E19D0, c_virtual_keyboard_task, _set_title_text);
HOOK_DECLARE_CLASS_MEMBER(0x004E1A00, c_virtual_keyboard_task, _start);
HOOK_DECLARE_CLASS(0x004E1A20, c_virtual_keyboard_task, _success);

HOOK_DECLARE(0x004E1860, online_guide_delay_toasts);
HOOK_DECLARE(0x004E1870, online_guide_dispose);
HOOK_DECLARE(0x004E18A0, online_guide_initialize);
HOOK_DECLARE(0x004E18B0, online_guide_set_toast_position);
HOOK_DECLARE(0x004E18C0, online_guide_show_damaged_media_ui);
HOOK_DECLARE(0x004E18E0, online_guide_show_file_share_recommendation);
HOOK_DECLARE(0x004E18F0, online_guide_show_friend_request_ui);
HOOK_DECLARE(0x004E1900, online_guide_show_friends_ui);
HOOK_DECLARE(0x004E1910, online_guide_show_gamer_card_ui);
HOOK_DECLARE(0x004E1950, online_guide_show_player_review_ui);
HOOK_DECLARE(0x004E1960, online_guide_show_sign_in_ui);
HOOK_DECLARE(0x004E1980, online_guide_update);

c_virtual_keyboard_task* __cdecl c_virtual_keyboard_task::constructor(
	char const* file,
	long line,
	e_controller_index controller_index,
	const wchar_t* default_text,
	const wchar_t* title_text,
	const wchar_t* description_text,
	dword maximum_input_characters,
	dword_flags character_flags,
	bool sanitize_result
)
{
	c_overlapped_task::constructor(file, line);

	REFERENCE_DECLARE(this, dword, vftable);
	vftable = 0x0164B38C;

	m_controller_index = controller_index;
	m_character_flags = character_flags;

	csmemset(m_result_text, 0, sizeof(m_result_text));
	csmemset(m_default_text, 0, sizeof(m_default_text));
	csmemset(m_title_text, 0, sizeof(m_title_text));
	csmemset(m_description_text, 0, sizeof(m_description_text));

	if (maximum_input_characters > 256)
		maximum_input_characters = 256;
	m_maximum_input_characters = maximum_input_characters;

	m_sanitize_result = sanitize_result;
	m_result_text[0] = 0;

	set_default_text(default_text);
	set_title_text(title_text);
	set_description_text(description_text);

	return this;
}

void c_overlapped_task::set_file(char const* file)
{
	m_file = file;
}

void c_overlapped_task::set_line(long line)
{
	m_line = line;
}

void __thiscall c_virtual_keyboard_task::_set_default_text(wchar_t const* default_text)
{
	set_default_text(default_text);
}

void __thiscall c_virtual_keyboard_task::_set_description_text(wchar_t const* description_text)
{
	set_description_text(description_text);
}

void __thiscall c_virtual_keyboard_task::_set_title_text(wchar_t const* title_text)
{
	set_title_text(title_text);
}

void __cdecl c_virtual_keyboard_task::set_controller_index(e_controller_index controller_index)
{
	m_controller_index = controller_index;
}

void __cdecl c_virtual_keyboard_task::set_default_text(wchar_t const* default_text)
{
	if (default_text)
		wcscpy_s(m_default_text, default_text);
	else
		m_default_text[0] = 0;
}

void __cdecl c_virtual_keyboard_task::set_description_text(wchar_t const* description_text)
{
	if (description_text)
		wcscpy_s(m_description_text, description_text);
	else
		m_description_text[0] = 0;
}

void __cdecl c_virtual_keyboard_task::set_title_text(wchar_t const* title_text)
{
	if (title_text)
		wcscpy_s(m_title_text, title_text);
	else
		m_title_text[0] = 0;
}

void __cdecl c_virtual_keyboard_task::set_maximum_input_characters(dword maximum_input_characters)
{
	m_maximum_input_characters = maximum_input_characters;
}

void __cdecl c_virtual_keyboard_task::set_character_flags(dword_flags character_flags)
{
	m_character_flags = character_flags;
}

void __cdecl c_virtual_keyboard_task::set_sanitize_result(bool sanitize_result)
{
	m_sanitize_result = sanitize_result;
}

c_virtual_keyboard_task* __cdecl c_virtual_keyboard_task::get_instance(
	char const* file,
	long line,
	e_controller_index controller_index,
	wchar_t const* default_text,
	wchar_t const* title_text,
	wchar_t const* description_text,
	dword maximum_input_characters,
	dword_flags character_flags,
	bool sanitize_result
)
{
	if (!m_instance)
	{
		m_instance = (c_virtual_keyboard_task*)overlapped_malloc_tracked(sizeof(c_virtual_keyboard_task), __FILE__, __LINE__);
		m_instance->constructor(
			file,
			line,
			controller_index,
			default_text,
			title_text,
			description_text,
			maximum_input_characters,
			character_flags,
			sanitize_result
		);
	}
	else
	{
		if (!m_instance->busy())
		{
			m_instance->set_file(file);
			m_instance->set_line(line);
			m_instance->set_controller_index(controller_index);
			m_instance->set_default_text(default_text);
			m_instance->set_title_text(title_text);
			m_instance->set_description_text(description_text);
			m_instance->set_maximum_input_characters(maximum_input_characters);
			m_instance->set_character_flags(character_flags);
			m_instance->set_sanitize_result(sanitize_result);
		}
		else
			generate_event(_event_level_warning, "ui: someone tried to get a duplicate instance of the virtual keyboard!");
	}

	return m_instance;
}

c_virtual_keyboard_task* c_virtual_keyboard_task::m_instance = nullptr;

void* c_virtual_keyboard_task::destructor(dword a1)
{
	DECLFUNC(0x004E17D0, void, __thiscall, c_virtual_keyboard_task*, dword)(this, a1);

	return this;
}

const char* c_virtual_keyboard_task::get_context_string()
{
	return "XShowKeyboardUI";
}

dword __cdecl online_guide_show_virtual_keyboard_ui(e_controller_index controller_index, dword_flags character_flags, wchar_t const* default_text, wchar_t const* title_text, wchar_t const* description_text, wchar_t* result_text, dword maximum_character_count, void* platform_handle)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	ASSERT(result_text != NULL);
	ASSERT(maximum_character_count > 0);
	ASSERT(platform_handle != NULL);

	c_controller_interface* controller = controller_get(controller_index);
	if (!controller->is_signed_in_to_machine())
		return E_FAIL;

	return XShowKeyboardUI(controller_index, character_flags, default_text, title_text, description_text, result_text, maximum_character_count, platform_handle);
}

dword c_virtual_keyboard_task::start(void* platform_handle)
{
	c_controller_interface* controller = controller_get(m_controller_index);
	if (!controller->is_signed_in_to_machine())
		return E_FAIL;

	if (m_maximum_input_characters > 256)
		return online_guide_show_virtual_keyboard_ui(m_controller_index, m_character_flags, m_default_text, m_title_text, m_description_text, m_result_text, 256, platform_handle);

	return online_guide_show_virtual_keyboard_ui(m_controller_index, m_character_flags, m_default_text, m_title_text, m_description_text, m_result_text, m_maximum_input_characters, platform_handle);
}

dword __thiscall c_virtual_keyboard_task::_start(void* platform_handle)
{
	c_controller_interface* controller = controller_get(m_controller_index);
	if (!controller->is_signed_in_to_machine())
		return E_FAIL;

	if (m_maximum_input_characters > 256)
		return online_guide_show_virtual_keyboard_ui(m_controller_index, m_character_flags, m_default_text, m_title_text, m_description_text, m_result_text, 256, platform_handle);

	return online_guide_show_virtual_keyboard_ui(m_controller_index, m_character_flags, m_default_text, m_title_text, m_description_text, m_result_text, m_maximum_input_characters, platform_handle);
}

void __fastcall c_virtual_keyboard_task::_success(c_virtual_keyboard_task* _this, dword a1)
{
	DECLFUNC(0x004E1A20, void, __thiscall, c_virtual_keyboard_task*, dword)(_this, a1);

	if (!wcsncmp(_this->m_result_text, L".fortune", 9))
		wcsncpy_s(_this->m_result_text, L"My modem is on file", 256);

	wchar_string_sanitize_for_game(_this->m_result_text, 256);
}

dword __cdecl online_guide_delay_toasts(long milliseconds)
{
	//return XNotifyDelayUI(milliseconds);

	return -1;
}

void __cdecl online_guide_dispose()
{
	//overlapped_task_block_until_finished(&g_online_guide_globals);
}

void __cdecl online_guide_initialize()
{
	//g_online_guide_globals = 0;
	//XEnableGuestSignin(true);
}

void __cdecl online_guide_set_toast_position(e_online_guide_toast_position toast_position)
{
	//long toast_position_map[k_toast_position_count] { 5, 1, 9, 4, 0, 8, 6, 2, 10 };
	//XNotifyPositionUI(toast_position_map[toast_position]);
}

void __cdecl online_guide_show_damaged_media_ui()
{
	MessageBoxA(NULL, "Damaged Media UI Placeholder", "networking:online:guide", MB_OK);

	//e_controller_index controller_index = controller_get_first_signed_in_controller();
	//if (controller_index == k_any_controller)
	//	controller_index = first_controller();
	//
	//XShowDirtyDiscErrorUI(controller_index);
}

void __cdecl online_guide_show_file_share_recommendation(e_controller_index controller_index, qword user_xuid, long a3, char const* a4, s_service_record_identity const* identity, wchar_t const* a6)
{
	MessageBoxA(NULL, "File Share Recommendation UI Placeholder", "networking:online:guide", MB_OK);

	//if (g_online_guide_globals->busy()) 
	//	generate_event(_event_level_error, "networking:online:guide: online_guide_show_file_share_recommendation called when task is already in progress");
}

dword __cdecl online_guide_show_friend_request_ui(e_controller_index controller_index, qword user_xuid)
{
	MessageBoxA(NULL, "Friend Request UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	 
	//dword result = E_FAIL;
	//c_static_string<256> error_string;
	//
	//if (controller_get(controller_index)->is_signed_in_to_live())
	//	result = XShowFriendRequestUI(controller_index, user_xuid);
	//
	//if (FAILED(result))
	//	generate_event(_event_level_warning, "networking:online:guide: XShowFriendRequestUI call failed with result %s", online_error_get_string(error_string, result));
	//
	//return result;

	return -1;
}

dword __cdecl online_guide_show_friends_ui(e_controller_index controller_index)
{
	MessageBoxA(NULL, "Friends UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	
	//dword result = E_FAIL;
	//
	//if (controller_get(controller_index)->is_signed_in_to_live())
	//	result = XShowFriendsUI(controller_index);
	//
	//if (FAILED(result))
	//	generate_event(_event_level_error, "networking:online:guide: XShowFriendsUI() returned #%ld for controller #%ld", result, controller_index);
	//
	//return result;

	return -1;
}

dword __cdecl online_guide_show_gamer_card_ui(e_controller_index controller_index, qword user_xuid)
{
	MessageBoxA(NULL, "Gamer Card UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	
	//dword result = E_FAIL;
	//c_static_string<256> error_string;
	//
	//if (controller_get(controller_index)->is_signed_in_to_live())
	//	result = XShowGamerCardUI(controller_index, user_xuid);
	//
	//if (FAILED(result))
	//	generate_event(_event_level_warning, "networking:online:guide: XShowGamerCardUI call failed with result %s", online_error_get_string(error_string, result));
	//
	//return result;

	return -1;
}

dword __cdecl online_guide_show_player_review_ui(e_controller_index controller_index, qword target_user_xuid)
{
	MessageBoxA(NULL, "Player Review UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	ASSERT(target_user_xuid != NULL);
	
	//dword result = E_FAIL;
	//c_static_string<256> error_string;
	//
	//if (controller_get(controller_index)->is_signed_in_to_live())
	//	result = XShowPlayerReviewUI(controller_index, target_user_xuid);
	//
	//if (FAILED(result))
	//	generate_event(_event_level_warning, "networking:online:guide: XShowPlayerReviewUI call failed with result %s", online_error_get_string(error_string, result));
	//
	//return result;

	return -1;
}

bool __cdecl online_guide_show_sign_in_ui(long pane_count, dword_flags flags)
{
	//MessageBoxA(NULL, "Signin UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(pane_count == 1 || pane_count == 2 || pane_count == 4);
	
	//dword result = 0;
	//dword signin_flags0 = (TEST_BIT(flags, 1) | TEST_BIT(flags, 2)) ? 2 : 0;
	//dword signin_flags1 = TEST_BIT(flags, 4) ? 0x400000 : 0;
	//
	//result = XShowSigninUI(pane_count, signin_flags0 | signin_flags1 | 0x80000);
	//if (result)
	//	generate_event(_event_level_warning, "networking:online:guide: XShowSigninUI failed with result %08x.", result);
	//
	//return result == 0;

	return true;
}

void __cdecl online_guide_update()
{
	//online_guide_update_gamercard_dynamic_actions();
	//online_guide_update_get_game_details();
	//online_guide_update_get_service_record();
}

