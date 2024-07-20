#pragma once

#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"
#include "networking/online/online_service_record.hpp"
#include "shell/shell.hpp"

struct c_virtual_keyboard_task :
	public c_overlapped_task
{
public:
	c_virtual_keyboard_task* __cdecl constructor(
		char const* file,
		long line,
		e_controller_index controller_index,
		const wchar_t* default_text,
		const wchar_t* title_text,
		const wchar_t* description_text,
		dword maximum_input_characters,
		dword_flags character_flags,
		bool cancelled
	);

	void __thiscall _set_default_text(wchar_t const* default_text);
	void __thiscall _set_description_text(wchar_t const* description_text);
	void __thiscall _set_title_text(wchar_t const* title_text);

	void __cdecl set_controller_index(e_controller_index controller_index);
	void __cdecl set_default_text(wchar_t const* default_text);
	void __cdecl set_description_text(wchar_t const* description_text);
	void __cdecl set_title_text(wchar_t const* title_text);
	void __cdecl set_maximum_input_characters(dword maximum_input_characters);
	void __cdecl set_character_flags(dword_flags character_flags);
	void __cdecl set_sanitize_result(bool sanitize_result);

	static c_virtual_keyboard_task* __cdecl get_instance(
		char const* file,
		long line,
		e_controller_index controller_index,
		wchar_t const* default_text,
		wchar_t const* title_text,
		wchar_t const* description_text,
		dword maximum_input_characters,
		dword_flags character_flags,
		bool sanitize_result
	);

	virtual void* destructor(dword a1) override;
	virtual const char* get_context_string() override;
	virtual dword start(void* platform_handle) override;

	dword __thiscall _start(void* platform_handle);
	static void __fastcall _success(c_virtual_keyboard_task* _this, dword a1);

	static c_virtual_keyboard_task* m_instance;

protected:
	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> m_controller_index;
	dword_flags m_character_flags;
	wchar_t m_result_text[256];
	wchar_t m_default_text[256];
	wchar_t m_title_text[64];
	wchar_t m_description_text[256];
	long m_maximum_input_characters;
	bool m_sanitize_result;
};
static_assert(sizeof(c_virtual_keyboard_task) == 0x6A0);

struct c_string_verify_task :
	public c_overlapped_task
{
	byte __data10[0x80C];
};
static_assert(sizeof(c_string_verify_task) == sizeof(c_overlapped_task) + 0x80C);

enum e_online_guide_toast_position
{
	_toast_position0 = 0,
	_toast_position1,
	_toast_position2,
	_toast_position3,
	_toast_position4,
	_toast_position5,
	_toast_position6,
	_toast_position7,
	_toast_position8,

	k_toast_position_count
};

extern dword __cdecl online_guide_delay_toasts(long milliseconds);
extern void __cdecl online_guide_dispose();
extern void __cdecl online_guide_initialize();
extern void __cdecl online_guide_set_toast_position(e_online_guide_toast_position toast_position);
extern void __cdecl online_guide_show_damaged_media_ui();
extern void __cdecl online_guide_show_file_share_recommendation(e_controller_index controller_index, qword player_xuid, long a3, char const* a4, s_service_record_identity const* identity, wchar_t const* a6);
extern dword __cdecl online_guide_show_friend_request_ui(e_controller_index controller_index, qword player_xuid);
extern dword __cdecl online_guide_show_friends_ui(e_controller_index controller_index);
extern dword __cdecl online_guide_show_gamer_card_ui(e_controller_index controller_index, qword player_xuid);
extern dword __cdecl online_guide_show_player_review_ui(e_controller_index controller_index, qword target_user_xuid);
extern bool __cdecl online_guide_show_sign_in_ui(long pane_count, dword_flags flags);
extern void __cdecl online_guide_update();