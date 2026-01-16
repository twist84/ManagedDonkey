#pragma once

#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "networking/online/online_service_record.hpp"

class c_virtual_keyboard_task :
	public c_overlapped_task
{
public:
	static c_virtual_keyboard_task* __cdecl get_instance_(
		const char* file,
		int32 line,
		e_controller_index controller_index,
		const wchar_t* default_text,
		const wchar_t* title_text,
		const wchar_t* description_text,
		uns32 maximum_input_characters,
		uns32 character_flags,
		bool sanitize_result
	);
	void __thiscall set_default_text_(const wchar_t* default_text);
	void __thiscall set_description_text_(const wchar_t* description_text);
	void __thiscall set_title_text_(const wchar_t* title_text);
	uns32 __thiscall start_(void* overlapped);
	void __thiscall success_(uns32 return_result);

public:
	virtual ~c_virtual_keyboard_task() = default;
	virtual const char* get_context_string() const override;
	virtual uns32 start(void* overlapped) override;
	virtual void success(uns32 return_result) override;

public:
	c_virtual_keyboard_task(
		const char* file,
		int32 line,
		e_controller_index controller_index,
		const wchar_t* default_text,
		const wchar_t* title_text,
		const wchar_t* description_text,
		uns32 maximum_input_characters,
		uns32 character_flags,
		bool sanitize_result);

	static void __cdecl dispose_instance();
	static c_virtual_keyboard_task* __cdecl get_instance(
		const char* file,
		int32 line,
		e_controller_index controller_index,
		const wchar_t* default_text,
		const wchar_t* title_text,
		const wchar_t* description_text,
		uns32 maximum_input_characters,
		uns32 character_flags,
		bool sanitize_result
	);

	void set_character_flags(uns32 character_flags);
	void set_controller_index(e_controller_index controller_index);
	void set_default_text(const wchar_t* default_text);
	void set_description_text(const wchar_t* description_text);
	void set_maximum_input_characters(uns32 maximum_input_characters);
	void set_sanitize_result(bool sanitize_result);
	void set_title_text(const wchar_t* title_text);

	static c_virtual_keyboard_task* m_instance;

protected:
	e_controller_index m_controller_index;
	uns32 m_character_flags;
	wchar_t m_result_text_buffer[256];
	wchar_t m_default_text_buffer[256];
	wchar_t m_title_text_buffer[64];
	wchar_t m_description_text_buffer[256];
	int32 m_maximum_input_characters;
	bool m_sanitize_result;
};
COMPILE_ASSERT(sizeof(c_virtual_keyboard_task) == 0x6A0);
COMPILE_ASSERT(sizeof(c_virtual_keyboard_task) == sizeof(c_overlapped_task) + 0x690);

class c_string_verify_task :
	public c_overlapped_task
{
public:
	enum
	{
		k_maximum_simultaneous_strings = 4
	};

public:
	virtual ~c_string_verify_task() = default;
	virtual const char* get_context_string() const override;
	virtual uns32 start(void* overlapped) override;
	virtual void success(uns32 return_result) override;
	virtual void reset() override;

public:
	c_string_verify_task(const char* file, int32 line);
	c_string_verify_task();

protected:
	e_language m_language;
	wchar_t m_strings_to_verify[k_maximum_simultaneous_strings][256];
	int32 m_strings_to_verify_count;
	bool m_results[k_maximum_simultaneous_strings];
};
COMPILE_ASSERT(sizeof(c_string_verify_task) == 0x81C);
COMPILE_ASSERT(sizeof(c_string_verify_task) == sizeof(c_overlapped_task) + 0x80C);

enum e_online_guide_toast_position
{
	_toast_position_top_left = 0,
	_toast_position_top_center,
	_toast_position_top_right,
	_toast_position_center_left,
	_toast_position_center,
	_toast_position_center_right,
	_toast_position_bottom_left,
	_toast_position_bottom_center,
	_toast_position_bottom_right,

	k_toast_position_count,

	k_toast_position_default = _toast_position_bottom_center,
	k_toast_position_cinematic = _toast_position_top_right,
	k_toast_position_game_shell_ui = _toast_position_bottom_center,
	k_toast_position_in_game_fullscreen = _toast_position_bottom_center,
	k_toast_position_in_game_splitscreen = _toast_position_top_right
};

struct s_custom_message_text
{
	c_static_wchar_string<256> title;
	c_static_wchar_string<256> game_message;
	c_static_wchar_string<256> player_message;
	struct
	{
		e_language language;
		c_static_wchar_string<256> text;
	} view_file_button[k_language_count];
};

extern uns32 __cdecl online_guide_delay_toasts(int32 milliseconds);
extern void __cdecl online_guide_dispose();
extern void __cdecl online_guide_handle_custom_action_pressed();
extern void __cdecl online_guide_handle_custom_gamercard(bool guide_is_open);
extern void __cdecl online_guide_initialize();
extern void __cdecl online_guide_set_toast_position(e_online_guide_toast_position toast_position);
extern void __cdecl online_guide_show_damaged_media_ui();
extern uns32 __cdecl online_guide_show_device_selector_ui(e_controller_index controller_index, uns32 requested_bytes, bool always_show, uns32* device_id, void* xenon_task_handle);
extern void __cdecl online_guide_show_file_share_recommendation(e_controller_index controller_index, uns64 file_share_xuid, int32 slot_index, const char* server_id, const s_service_record_identity* service_record_identity, s_custom_message_text* custom_message_text);
extern uns32 __cdecl online_guide_show_friend_request_ui(e_controller_index controller_index, uns64 player_xuid);
extern uns32 __cdecl online_guide_show_friends_ui(e_controller_index controller_index);
extern uns32 __cdecl online_guide_show_gamer_card_ui(e_controller_index controller_index, uns64 player_xuid);
extern void __cdecl online_guide_show_marketplace(e_controller_index controller_index);
extern void __cdecl online_guide_show_marketplace_offer(e_controller_index controller_index, uns64 marketplace_offer_id);
extern uns32 __cdecl online_guide_show_message_ui(e_controller_index controller_index, uns64 target_player_xuid);
extern uns32 __cdecl online_guide_show_player_review_ui(e_controller_index controller_index, uns64 target_user_xuid);
extern bool __cdecl online_guide_show_sign_in_ui(int32 pane_count, uns32 flags);
extern uns32 __cdecl online_guide_show_virtual_keyboard_ui(e_controller_index controller_index, uns32 character_flags, const wchar_t* default_text, const wchar_t* title_text, const wchar_t* description_text, wchar_t* result_text, uns32 maximum_character_count, void* overlapped);
extern void __cdecl online_guide_update();