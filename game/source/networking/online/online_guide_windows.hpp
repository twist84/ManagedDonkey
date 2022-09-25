#pragma once

#include "cseries/async_xoverlapped.hpp"
#include "cseries/cseries.hpp"

#include <string.h>

struct c_virtual_keyboard_task : public c_overlapped_task
{
	c_virtual_keyboard_task* constructor(
		const char* file,
		long line,
		long controller_index,
		const wchar_t* default_text,
		const wchar_t* title_text,
		const wchar_t* description_text,
		dword maximum_input_characters
	);

	void set_controller_index(long controller_index);
	void set_default_text(wchar_t const* default_text);
	void set_title_text(wchar_t const* title_text);
	void set_description_text(wchar_t const* description_text);
	void set_maximum_input_characters(dword maximum_input_characters);
	void set_character_flags(dword_flags character_flags);

	static c_virtual_keyboard_task* __cdecl get_instance(
		char const* file,
		long line,
		long controller_index,
		wchar_t const* default_text,
		wchar_t const* title_text,
		wchar_t const* description_text,
		dword maximum_input_characters,
		dword character_flags
	);

	long m_controller_index;
	dword_flags m_character_flags;
	wchar_t m_keyboard_results[256];
	wchar_t m_default_text[256];
	wchar_t m_title_text[64];
	wchar_t m_description_text[256];
	long m_maximum_input_characters;
	bool __unknown69C;

	static c_virtual_keyboard_task* m_instance;
};

void __fastcall virtual_keyboard_task_success(c_virtual_keyboard_task* task, dword a1);