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

c_virtual_keyboard_task* c_virtual_keyboard_task::m_instance = NULL;

c_virtual_keyboard_task* __cdecl c_virtual_keyboard_task::get_instance_(
	const char* file,
	int32 line,
	e_controller_index controller_index,
	const wchar_t* default_text,
	const wchar_t* title_text,
	const wchar_t* description_text,
	uns32 maximum_input_characters,
	uns32 character_flags,
	bool sanitize_result
)
{
	return c_virtual_keyboard_task::get_instance(
		file,
		line,
		controller_index,
		default_text,
		title_text,
		description_text,
		maximum_input_characters,
		character_flags,
		sanitize_result);
}

void __thiscall c_virtual_keyboard_task::set_default_text_(const wchar_t* default_text)
{
	set_default_text(default_text);
}

void __thiscall c_virtual_keyboard_task::set_description_text_(const wchar_t* description_text)
{
	set_description_text(description_text);
}

void __thiscall c_virtual_keyboard_task::set_title_text_(const wchar_t* title_text)
{
	set_title_text(title_text);
}

uns32 __thiscall c_virtual_keyboard_task::start_(void* overlapped)
{
	return c_virtual_keyboard_task::start(overlapped);
}

void __thiscall c_virtual_keyboard_task::success_(uns32 return_result)
{
	c_virtual_keyboard_task::success(return_result);
}

c_string_verify_task::c_string_verify_task(const char* file, int32 line) :
	c_overlapped_task(file, line),
	m_language(),
	m_strings_to_verify(),
	m_strings_to_verify_count(),
	m_results()
{
	//DECLFUNC(0x004E1660, void, __thiscall, c_string_verify_task*, const char*, int32)(this, file, line);
}

c_string_verify_task::c_string_verify_task() :
	c_overlapped_task("", 0),
	m_language(),
	m_strings_to_verify(),
	m_strings_to_verify_count(),
	m_results()
{
	//DECLFUNC(0x004E1680, void, __thiscall, c_string_verify_task*)(this);
}

c_virtual_keyboard_task::c_virtual_keyboard_task(
	const char* file,
	int32 line,
	e_controller_index controller_index,
	const wchar_t* default_text,
	const wchar_t* title_text,
	const wchar_t* description_text,
	uns32 maximum_input_characters,
	uns32 character_flags,
	bool sanitize_result) :
	c_overlapped_task(file, line),
	m_controller_index(controller_index),
	m_character_flags(character_flags),
	m_result_text_buffer(),
	m_default_text_buffer(),
	m_title_text_buffer(),
	m_description_text_buffer(),
	m_maximum_input_characters(MIN(maximum_input_characters, 256)),
	m_sanitize_result(sanitize_result)
{
	//004E16A0

	csmemset(m_result_text_buffer, 0, sizeof(m_result_text_buffer));
	csmemset(m_default_text_buffer, 0, sizeof(m_default_text_buffer));
	csmemset(m_title_text_buffer, 0, sizeof(m_title_text_buffer));
	csmemset(m_description_text_buffer, 0, sizeof(m_description_text_buffer));

	set_default_text(default_text);
	set_title_text(title_text);
	set_description_text(description_text);
}

//.text:004E17A0 ; public: virtual void* c_string_verify_task::`vector deleting destructor'(unsigned int)
c_string_verify_task::~c_string_verify_task()
{
}

//.text:004E17D0 ; public: virtual void * __cdecl c_virtual_keyboard_task::`vector deleting destructor'(unsigned int)
c_virtual_keyboard_task::~c_virtual_keyboard_task()
{
}

void overlapped_track_delete(c_overlapped_task* task)
{
	INVOKE(0x014E2120, overlapped_track_delete, task);
}

void __cdecl c_virtual_keyboard_task::dispose_instance()
{
	//INVOKE(0x004E1800, c_virtual_keyboard_task::dispose_instance);

	if (m_instance)
	{
		overlapped_track_delete(m_instance);
		m_instance = NULL;
	}
}

//.text:004E1810 ; public: virtual const char* c_invite_task::get_context_string() const

const char* c_string_verify_task::get_context_string() const
{
	//return INVOKE_CLASS_MEMBER(0x004E1820, c_string_verify_task, get_context_string);

	return "XStringVerify";
}

const char* c_virtual_keyboard_task::get_context_string() const
{
	//return INVOKE_CLASS_MEMBER(0x004E1830, c_virtual_keyboard_task, get_context_string);

	return "XShowKeyboardUI";
}

c_virtual_keyboard_task* __cdecl c_virtual_keyboard_task::get_instance(
	const char* file,
	int32 line,
	e_controller_index controller_index,
	const wchar_t* default_text,
	const wchar_t* title_text,
	const wchar_t* description_text,
	uns32 maximum_input_characters,
	uns32 character_flags,
	bool sanitize_result)
{
	if (!m_instance)
	{
		m_instance = OVERLAPPED_MALLOC(c_virtual_keyboard_task,
			file,
			line,
			controller_index,
			default_text,
			title_text,
			description_text,
			maximum_input_characters,
			character_flags,
			sanitize_result);
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
		{
			event(_event_warning, "ui: someone tried to get a duplicate instance of the virtual keyboard!");
		}
	}

	return m_instance;
}

//.text:004E1850 ; public: void c_string_verify_task::initialize(e_language, wchar_t const**, int32)

uns32 __cdecl online_guide_delay_toasts(int32 milliseconds)
{
	//return INVOKE(0x004E1860, online_guide_delay_toasts, milliseconds);

	//return XNotifyDelayUI(milliseconds);

	return NONE;
}

void __cdecl online_guide_dispose()
{
	//INVOKE(0x004E1870, online_guide_dispose);

	//overlapped_task_block_until_finished(&g_online_guide_globals);
}

void __cdecl online_guide_handle_custom_action_pressed()
{
	//INVOKE(0x004E1880, online_guide_handle_custom_action_pressed);
}

void __cdecl online_guide_handle_custom_gamercard(bool guide_is_open)
{
	//INVOKE(0x004E1890, online_guide_handle_custom_gamercard, guide_is_open);
}

void __cdecl online_guide_initialize()
{
	//INVOKE(0x004E18A0, online_guide_initialize);

	//g_online_guide_globals = 0;
	//XEnableGuestSignin(true);
}

void __cdecl online_guide_set_toast_position(e_online_guide_toast_position toast_position)
{
	//INVOKE(0x004E18B0, online_guide_set_toast_position, toast_position);

	//int32 toast_position_map[k_toast_position_count] { 5, 1, 9, 4, 0, 8, 6, 2, 10 };
	//XNotifyPositionUI(toast_position_map[toast_position]);
}

void __cdecl online_guide_show_damaged_media_ui()
{
	//INVOKE(0x004E18C0, online_guide_show_damaged_media_ui);

	MessageBoxA(NULL, "Damaged Media UI Placeholder", "networking:online:guide", MB_OK);

	//e_controller_index controller_index = controller_get_first_signed_in_controller();
	//if (controller_index == k_any_controller)
	//	controller_index = first_controller();
	//
	//XShowDirtyDiscErrorUI(controller_index);
}

uns32 __cdecl online_guide_show_device_selector_ui(e_controller_index controller_index, uns32 requested_bytes, bool always_show, uns32* device_id, void* xenon_task_handle)
{
	//return INVOKE(0x004E18D0, online_guide_show_device_selector_ui, controller_index, requested_bytes, always_show, device_id, xenon_task_handle);

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

void __cdecl online_guide_show_file_share_recommendation(e_controller_index controller_index, uns64 file_share_xuid, int32 slot_index, const char* server_id, const s_service_record_identity* service_record_identity, s_custom_message_text* custom_message_text)
{
	//INVOKE(0x004E18E0, online_guide_show_file_share_recommendation, controller_index, file_share_xuid, slot_index, server_id, service_record_identity, custom_message_text);

	MessageBoxA(NULL, "File Share Recommendation UI Placeholder", "networking:online:guide", MB_OK);

	//if (g_online_guide_globals->busy()) 
	//	event(_event_error, "networking:online:guide: online_guide_show_file_share_recommendation called when task is already in progress");
}

uns32 __cdecl online_guide_show_friend_request_ui(e_controller_index controller_index, uns64 user_xuid)
{
	//return INVOKE(0x004E18F0, online_guide_show_friend_request_ui, controller_index, user_xuid);

	MessageBoxA(NULL, "Friend Request UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));

	//uns32 result = E_FAIL;
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

uns32 __cdecl online_guide_show_friends_ui(e_controller_index controller_index)
{
	//return INVOKE(0x004E1900, online_guide_show_friends_ui, controller_index);

	MessageBoxA(NULL, "Friends UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));

	//uns32 result = E_FAIL;
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

uns32 __cdecl online_guide_show_gamer_card_ui(e_controller_index controller_index, uns64 user_xuid)
{
	//return INVOKE(0x004E1910, online_guide_show_gamer_card_ui, controller_index, user_xuid);

	MessageBoxA(NULL, "Gamer Card UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));

	//uns32 result = E_FAIL;
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
	//INVOKE(0x004E1920, online_guide_show_marketplace, controller_index);
}

void __cdecl online_guide_show_marketplace_offer(e_controller_index controller_index, uns64 marketplace_offer_id)
{
	//INVOKE(0x004E1930, online_guide_show_marketplace_offer, controller_index, marketplace_offer_id);

	ShellExecute(NULL, TEXT("open"), TEXT("https://github.com/twist84/ManagedDonkey"), NULL, NULL, SW_SHOWNORMAL);
}

uns32 __cdecl online_guide_show_message_ui(e_controller_index controller_index, uns64 target_player_xuid)
{
	//INVOKE(0x004E1940, online_guide_show_message_ui, controller_index, target_player_xuid);

	return 0;
}

uns32 __cdecl online_guide_show_player_review_ui(e_controller_index controller_index, uns64 target_user_xuid)
{
	//return INVOKE(0x004E1950, online_guide_show_player_review_ui, controller_index, target_user_xuid);

	MessageBoxA(NULL, "Player Review UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	ASSERT(target_user_xuid != NULL);

	//uns32 result = E_FAIL;
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

bool __cdecl online_guide_show_sign_in_ui(int32 pane_count, uns32 flags)
{
	//return INVOKE(0x004E1960, online_guide_show_sign_in_ui, pane_count, flags);

	//MessageBoxA(NULL, "Signin UI Placeholder", "networking:online:guide", MB_OK);

	ASSERT(pane_count == 1 || pane_count == 2 || pane_count == 4);

	//uns32 result = 0;
	//uns32 signin_flags0 = (TEST_BIT(flags, 1) | TEST_BIT(flags, 2)) ? 2 : 0;
	//uns32 signin_flags1 = TEST_BIT(flags, 4) ? 0x400000 : 0;
	//
	//result = XShowSigninUI(pane_count, signin_flags0 | signin_flags1 | 0x80000);
	//if (result)
	//	event(_event_warning, "networking:online:guide: XShowSigninUI failed with result %08x.", result);
	//
	//return result == 0;

	return true;
}

//uns32 __cdecl online_guide_show_virtual_keyboard_ui(e_controller_index controller_index, uns32 character_flags, const wchar_t* default_text, const wchar_t* title_text, const wchar_t* description_text, wchar_t* result_text, uns32 maximum_character_count, struct _XOVERLAPPED* platform_handle)
uns32 __cdecl online_guide_show_virtual_keyboard_ui(e_controller_index controller_index, uns32 character_flags, const wchar_t* default_text, const wchar_t* title_text, const wchar_t* description_text, wchar_t* result_text, uns32 maximum_character_count, void* overlapped)
{
	//return INVOKE(0x004E1970, online_guide_show_virtual_keyboard_ui, controller_index, character_flags, default_text, title_text, description_text, result_text, maximum_character_count, overlapped);

	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	ASSERT(result_text != NULL);
	ASSERT(maximum_character_count > 0);
	ASSERT(overlapped != NULL);

	c_controller_interface* controller = controller_get(controller_index);
	if (!controller->is_signed_in_to_machine())
	{
		return E_FAIL;
	}

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

void c_string_verify_task::reset()
{
	INVOKE_CLASS_MEMBER(0x004E1990, c_string_verify_task, reset);
}

void c_virtual_keyboard_task::set_character_flags(uns32 character_flags)
{
	m_character_flags = character_flags;
}

void c_virtual_keyboard_task::set_controller_index(e_controller_index controller_index)
{
	m_controller_index = controller_index;
}

void c_virtual_keyboard_task::set_default_text(const wchar_t* default_text)
{
	//INVOKE_CLASS_MEMBER(0x004E19A0, c_virtual_keyboard_task, set_default_text, default_text);

	if (default_text)
	{
		ustrnzcpy(m_default_text_buffer, default_text, 256);
	}
	else
	{
		m_default_text_buffer[0] = 0;
	}
}

void c_virtual_keyboard_task::set_description_text(const wchar_t* description_text)
{
	//INVOKE_CLASS_MEMBER(0x004E19B0, c_virtual_keyboard_task, set_description_text, description_text);

	if (description_text)
	{
		ustrnzcpy(m_description_text_buffer, description_text, 64);
	}
	else
	{
		m_description_text_buffer[0] = 0;
	}
}

//.text:004E19C0 ; 

void c_virtual_keyboard_task::set_maximum_input_characters(uns32 maximum_input_characters)
{
	m_maximum_input_characters = maximum_input_characters;
}

void c_virtual_keyboard_task::set_sanitize_result(bool sanitize_result)
{
	m_sanitize_result = sanitize_result;
}

void c_virtual_keyboard_task::set_title_text(const wchar_t* title_text)
{
	//INVOKE_CLASS_MEMBER(0x004E19D0, c_virtual_keyboard_task, set_title_text, title_text);

	if (title_text)
	{
		ustrnzcpy(m_title_text_buffer, title_text, 64);
	}
	else
	{
		m_title_text_buffer[0] = 0;
	}
}

//.text:004E19E0 ; public: virtual uns32 c_invite_task::start(void*)

uns32 c_string_verify_task::start(void* overlapped)
{
	return INVOKE_CLASS_MEMBER(0x004E19F0, c_string_verify_task, start, overlapped);
}

uns32 c_virtual_keyboard_task::start(void* overlapped)
{
	//return INVOKE_CLASS_MEMBER(0x004E1A00, c_virtual_keyboard_task, start, overlapped);

	c_controller_interface* controller = controller_get(m_controller_index);
	if (!controller->is_signed_in_to_machine())
	{
		return E_FAIL;
	}

	if (m_maximum_input_characters > 256)
	{
		return online_guide_show_virtual_keyboard_ui(m_controller_index, m_character_flags, m_default_text_buffer, m_title_text_buffer, m_description_text_buffer, m_result_text_buffer, 256, overlapped);
	}

	return online_guide_show_virtual_keyboard_ui(m_controller_index, m_character_flags, m_default_text_buffer, m_title_text_buffer, m_description_text_buffer, m_result_text_buffer, m_maximum_input_characters, overlapped);
}

void c_string_verify_task::success(uns32 return_result)
{
	INVOKE_CLASS_MEMBER(0x004E1A10, c_string_verify_task, success, return_result);
}

void c_virtual_keyboard_task::success(uns32 return_result)
{
	//INVOKE_CLASS_MEMBER(0x004E1A20, c_virtual_keyboard_task, success, return_result);

	//c_overlapped_task::success(return_result);

	if (!ustrncmp(m_result_text_buffer, L".fortune", 9))
	{
		ustrnzcpy(m_result_text_buffer, L"My modem is on file", 256);
	}

	wchar_string_sanitize_for_game(m_result_text_buffer, 256);
}

