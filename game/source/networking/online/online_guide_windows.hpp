#pragma once

#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"

#include <string.h>

struct c_virtual_keyboard_task : public c_overlapped_task
{
	static c_virtual_keyboard_task* __fastcall constructor(
		c_virtual_keyboard_task* _this,
		const char* file,
		long line,
		long controller_index,
		const wchar_t* default_text,
		const wchar_t* title_text,
		const wchar_t* description_text,
		dword maximum_input_characters
	);

	static void __fastcall set_controller_index(c_virtual_keyboard_task* _this, long controller_index);
	static void __fastcall set_default_text(c_virtual_keyboard_task* _this, wchar_t const* default_text);
	static void __fastcall set_title_text(c_virtual_keyboard_task* _this, wchar_t const* title_text);
	static void __fastcall set_description_text(c_virtual_keyboard_task* _this, wchar_t const* description_text);
	static void __fastcall set_maximum_input_characters(c_virtual_keyboard_task* _this, dword maximum_input_characters);
	static void __fastcall set_character_flags(c_virtual_keyboard_task* _this, dword_flags character_flags);

	static c_virtual_keyboard_task* __cdecl get_instance(
		char const* file,
		long line,
		long controller_index,
		wchar_t const* default_text,
		wchar_t const* title_text,
		wchar_t const* description_text,
		dword maximum_input_characters,
		dword_flags character_flags
	);

	static void __fastcall success(c_virtual_keyboard_task* _this, dword a1);

	static c_virtual_keyboard_task* m_instance;

	long m_controller_index;
	dword_flags m_character_flags;
	wchar_t m_keyboard_results[256];
	wchar_t m_default_text[256];
	wchar_t m_title_text[64];
	wchar_t m_description_text[256];
	long m_maximum_input_characters;
	bool __unknown69C;
};