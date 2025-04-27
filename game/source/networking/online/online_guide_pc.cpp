#include "networking/online/online_guide_pc.hpp"

#include "cseries/async_xoverlapped_memory.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/c_controller.hpp"
#include "interface/user_interface_text.hpp"
#include "memory/module.hpp"
#include "shell/shell.hpp"
#include "xbox/xbox.hpp"

#include <windows.h>
#include <string.h>

//HOOK_DECLARE_CLASS(0x004E16A0, c_virtual_keyboard_task, constructor);
HOOK_DECLARE_CLASS(0x004E1840, c_virtual_keyboard_task, get_instance);
HOOK_DECLARE_CLASS_MEMBER(0x004E19A0, c_virtual_keyboard_task, set_default_text_);
HOOK_DECLARE_CLASS_MEMBER(0x004E19B0, c_virtual_keyboard_task, set_description_text_);
HOOK_DECLARE_CLASS_MEMBER(0x004E19D0, c_virtual_keyboard_task, set_title_text_);
HOOK_DECLARE_CLASS_MEMBER(0x004E1A00, c_virtual_keyboard_task, start_);
HOOK_DECLARE_CLASS_MEMBER(0x004E1A20, c_virtual_keyboard_task, success_);

HOOK_DECLARE(0x004E1860, online_guide_delay_toasts);
HOOK_DECLARE(0x004E1870, online_guide_dispose);
HOOK_DECLARE(0x004E1880, online_guide_handle_custom_action_pressed);
HOOK_DECLARE(0x004E1890, online_guide_handle_custom_gamercard);
HOOK_DECLARE(0x004E18A0, online_guide_initialize);
HOOK_DECLARE(0x004E18B0, online_guide_set_toast_position);
HOOK_DECLARE(0x004E18C0, online_guide_show_damaged_media_ui);
HOOK_DECLARE(0x004E18D0, online_guide_show_device_selector_ui);
HOOK_DECLARE(0x004E18E0, online_guide_show_file_share_recommendation);
HOOK_DECLARE(0x004E18F0, online_guide_show_friend_request_ui);
HOOK_DECLARE(0x004E1900, online_guide_show_friends_ui);
HOOK_DECLARE(0x004E1910, online_guide_show_gamer_card_ui);
HOOK_DECLARE(0x004E1920, online_guide_show_marketplace);
HOOK_DECLARE(0x004E1930, online_guide_show_marketplace_offer);
HOOK_DECLARE(0x004E1940, online_guide_show_message_ui);
HOOK_DECLARE(0x004E1950, online_guide_show_player_review_ui);
HOOK_DECLARE(0x004E1960, online_guide_show_sign_in_ui);
HOOK_DECLARE(0x004E1970, online_guide_show_virtual_keyboard_ui);
HOOK_DECLARE(0x004E1980, online_guide_update);

c_virtual_keyboard_task* __cdecl c_virtual_keyboard_task::constructor(
	char const* file,
	long line,
	e_controller_index controller_index,
	wchar_t const* default_text,
	wchar_t const* title_text,
	wchar_t const* description_text,
	uint32 maximum_input_characters,
	uint32 character_flags,
	bool sanitize_result
)
{
	c_overlapped_task::constructor(file, line);

	REFERENCE_DECLARE(this, uint32, vftable);
	vftable = 0x0164B38C;

	m_controller_index = controller_index;
	m_character_flags = character_flags;

	csmemset(m_result_text_buffer, 0, sizeof(m_result_text_buffer));
	csmemset(m_default_text_buffer, 0, sizeof(m_default_text_buffer));
	csmemset(m_title_text_buffer, 0, sizeof(m_title_text_buffer));
	csmemset(m_description_text_buffer, 0, sizeof(m_description_text_buffer));

	if (maximum_input_characters > 256)
		maximum_input_characters = 256;
	m_maximum_input_characters = maximum_input_characters;

	m_sanitize_result = sanitize_result;
	m_result_text_buffer[0] = 0;

	set_default_text(default_text);
	set_title_text(title_text);
	set_description_text(description_text);

	return this;
}

void __thiscall c_virtual_keyboard_task::set_default_text_(wchar_t const* default_text)
{
	set_default_text(default_text);
}

void __thiscall c_virtual_keyboard_task::set_description_text_(wchar_t const* description_text)
{
	set_description_text(description_text);
}

void __thiscall c_virtual_keyboard_task::set_title_text_(wchar_t const* title_text)
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
		ustrnzcpy(m_default_text_buffer, default_text, 256);
	else
		m_default_text_buffer[0] = 0;
}

void __cdecl c_virtual_keyboard_task::set_description_text(wchar_t const* description_text)
{
	if (description_text)
		ustrnzcpy(m_description_text_buffer, description_text, 64);
	else
		m_description_text_buffer[0] = 0;
}

void __cdecl c_virtual_keyboard_task::set_title_text(wchar_t const* title_text)
{
	if (title_text)
		ustrnzcpy(m_title_text_buffer, title_text, 64);
	else
		m_title_text_buffer[0] = 0;
}

void __cdecl c_virtual_keyboard_task::set_maximum_input_characters(uint32 maximum_input_characters)
{
	m_maximum_input_characters = maximum_input_characters;
}

void __cdecl c_virtual_keyboard_task::set_character_flags(uint32 character_flags)
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
	uint32 maximum_input_characters,
	uint32 character_flags,
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
			event(_event_warning, "ui: someone tried to get a duplicate instance of the virtual keyboard!");
	}

	return m_instance;
}

void overlapped_track_delete(c_overlapped_task* task)
{
	INVOKE(0x014E2120, overlapped_track_delete, task);
}

void __cdecl c_virtual_keyboard_task::dispose_instance()
{
	if (m_instance)
	{
		overlapped_track_delete(m_instance);
		m_instance = NULL;
	}
}

c_virtual_keyboard_task* c_virtual_keyboard_task::m_instance = nullptr;

void* c_virtual_keyboard_task::destructor(uint32 a1)
{
	INVOKE_CLASS_MEMBER(0x004E17D0, c_virtual_keyboard_task, destructor, a1);

	return this;
}

char const* c_virtual_keyboard_task::get_context_string()
{
	return "XShowKeyboardUI";
}

uint32 c_virtual_keyboard_task::start(void* overlapped)
{
	c_controller_interface* controller = controller_get(m_controller_index);
	if (!controller->is_signed_in_to_machine())
		return E_FAIL;

	if (m_maximum_input_characters > 256)
		return online_guide_show_virtual_keyboard_ui(m_controller_index, m_character_flags, m_default_text_buffer, m_title_text_buffer, m_description_text_buffer, m_result_text_buffer, 256, overlapped);

	return online_guide_show_virtual_keyboard_ui(m_controller_index, m_character_flags, m_default_text_buffer, m_title_text_buffer, m_description_text_buffer, m_result_text_buffer, m_maximum_input_characters, overlapped);
}

uint32 __thiscall c_virtual_keyboard_task::start_(void* overlapped)
{
	c_controller_interface* controller = controller_get(m_controller_index);
	if (!controller->is_signed_in_to_machine())
		return E_FAIL;

	if (m_maximum_input_characters > 256)
		return online_guide_show_virtual_keyboard_ui(m_controller_index, m_character_flags, m_default_text_buffer, m_title_text_buffer, m_description_text_buffer, m_result_text_buffer, 256, overlapped);

	return online_guide_show_virtual_keyboard_ui(m_controller_index, m_character_flags, m_default_text_buffer, m_title_text_buffer, m_description_text_buffer, m_result_text_buffer, m_maximum_input_characters, overlapped);
}

void __thiscall c_virtual_keyboard_task::success_(uint32 return_result)
{
	//INVOKE_CLASS_MEMBER(0x004E1A20, c_virtual_keyboard_task, success, return_result);

	//c_overlapped_task::success(return_result);

	if (!ustrncmp(m_result_text_buffer, L".fortune", 9))
		ustrnzcpy(m_result_text_buffer, L"My modem is on file", 256);

	wchar_string_sanitize_for_game(m_result_text_buffer, 256);
}

uint32 __cdecl online_guide_delay_toasts(long milliseconds)
{
	//return XNotifyDelayUI(milliseconds);

	return NONE;
}

void __cdecl online_guide_dispose()
{
	//overlapped_task_block_until_finished(&g_online_guide_globals);
}

void __cdecl online_guide_handle_custom_action_pressed()
{
}

void __cdecl online_guide_handle_custom_gamercard(bool guide_is_open)
{
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

uint32 __cdecl online_guide_show_device_selector_ui(e_controller_index controller_index, uint32 requested_bytes, bool always_show, uint32* device_id, void* xenon_task_handle)
{
	//MessageBoxA(NULL, "Device Selector UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	ASSERT(device_id != NULL);
	ASSERT(xenon_task_handle != NULL);
	
	//if (controller_get(controller_index)->is_signed_in_to_machine())
	//{
	//	//return XShowDeviceSelectorUI(controller_index, 1, 0, requested_bytes, device_id, xenon_task_handle);
	//	return ERROR_IO_PENDING;
	//}
	//return E_FAIL;

	return NONE;
}

void __cdecl online_guide_show_file_share_recommendation(e_controller_index controller_index, uint64 file_share_xuid, long slot_index, char const* server_id, s_service_record_identity const* service_record_identity, s_custom_message_text* custom_message_text)
{
	MessageBoxA(NULL, "File Share Recommendation UI Placeholder", "networking:online:guide", MB_OK);

	//if (g_online_guide_globals->busy()) 
	//	event(_event_error, "networking:online:guide: online_guide_show_file_share_recommendation called when task is already in progress");
}

uint32 __cdecl online_guide_show_friend_request_ui(e_controller_index controller_index, uint64 user_xuid)
{
	MessageBoxA(NULL, "Friend Request UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	 
	//uint32 result = E_FAIL;
	//c_static_string<256> error_string;
	//
	//if (controller_get(controller_index)->is_signed_in_to_live())
	//	result = XShowFriendRequestUI(controller_index, user_xuid);
	//
	//if (FAILED(result))
	//	event(_event_warning, "networking:online:guide: XShowFriendRequestUI call failed with result %s", online_error_get_string(error_string, result));
	//
	//return result;

	return NONE;
}

uint32 __cdecl online_guide_show_friends_ui(e_controller_index controller_index)
{
	MessageBoxA(NULL, "Friends UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	
	//uint32 result = E_FAIL;
	//
	//if (controller_get(controller_index)->is_signed_in_to_live())
	//	result = XShowFriendsUI(controller_index);
	//
	//if (FAILED(result))
	//	event(_event_error, "networking:online:guide: XShowFriendsUI() returned #%ld for controller #%ld", result, controller_index);
	//
	//return result;

	return NONE;
}

uint32 __cdecl online_guide_show_gamer_card_ui(e_controller_index controller_index, uint64 user_xuid)
{
	MessageBoxA(NULL, "Gamer Card UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	
	//uint32 result = E_FAIL;
	//c_static_string<256> error_string;
	//
	//if (controller_get(controller_index)->is_signed_in_to_live())
	//	result = XShowGamerCardUI(controller_index, user_xuid);
	//
	//if (FAILED(result))
	//	event(_event_warning, "networking:online:guide: XShowGamerCardUI call failed with result %s", online_error_get_string(error_string, result));
	//
	//return result;

	return NONE;
}

void __cdecl online_guide_show_marketplace(e_controller_index controller_index)
{
}

void __cdecl online_guide_show_marketplace_offer(e_controller_index controller_index, uint64 marketplace_offer_id)
{
}

uint32 __cdecl online_guide_show_message_ui(e_controller_index controller_index, uint64 target_player_xuid)
{
	return 0;
}

uint32 __cdecl online_guide_show_player_review_ui(e_controller_index controller_index, uint64 target_user_xuid)
{
	MessageBoxA(NULL, "Player Review UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	ASSERT(target_user_xuid != NULL);
	
	//uint32 result = E_FAIL;
	//c_static_string<256> error_string;
	//
	//if (controller_get(controller_index)->is_signed_in_to_live())
	//	result = XShowPlayerReviewUI(controller_index, target_user_xuid);
	//
	//if (FAILED(result))
	//	event(_event_warning, "networking:online:guide: XShowPlayerReviewUI call failed with result %s", online_error_get_string(error_string, result));
	//
	//return result;

	return NONE;
}

bool __cdecl online_guide_show_sign_in_ui(long pane_count, uint32 flags)
{
	//MessageBoxA(NULL, "Signin UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(pane_count == 1 || pane_count == 2 || pane_count == 4);
	
	//uint32 result = 0;
	//uint32 signin_flags0 = (TEST_BIT(flags, 1) | TEST_BIT(flags, 2)) ? 2 : 0;
	//uint32 signin_flags1 = TEST_BIT(flags, 4) ? 0x400000 : 0;
	//
	//result = XShowSigninUI(pane_count, signin_flags0 | signin_flags1 | 0x80000);
	//if (result)
	//	event(_event_warning, "networking:online:guide: XShowSigninUI failed with result %08x.", result);
	//
	//return result == 0;

	return true;
}

//uint32 __cdecl online_guide_show_virtual_keyboard_ui(e_controller_index controller_index, uint32 character_flags, wchar_t const* default_text, wchar_t const* title_text, wchar_t const* description_text, wchar_t* result_text, uint32 maximum_character_count, struct _XOVERLAPPED* platform_handle)
uint32 __cdecl online_guide_show_virtual_keyboard_ui(e_controller_index controller_index, uint32 character_flags, wchar_t const* default_text, wchar_t const* title_text, wchar_t const* description_text, wchar_t* result_text, uint32 maximum_character_count, void* overlapped)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	ASSERT(result_text != NULL);
	ASSERT(maximum_character_count > 0);
	ASSERT(overlapped != NULL);

	c_controller_interface* controller = controller_get(controller_index);
	if (!controller->is_signed_in_to_machine())
		return E_FAIL;

	// $TODO: properly implement `XShowKeyboardUI`
	return ERROR_SUCCESS;

	return XShowKeyboardUI(controller_index, character_flags, default_text, title_text, description_text, result_text, maximum_character_count, overlapped);
}

void __cdecl online_guide_update()
{
	//online_guide_update_gamercard_dynamic_actions();
	//online_guide_update_get_game_details();
	//online_guide_update_get_service_record();
}

