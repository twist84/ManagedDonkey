#pragma once

extern bool __cdecl get_clipboard_as_text(char* buf, long len);
extern bool __cdecl set_clipboard_as_text(char* buf, long len);

extern long XShowKeyboardUI(
	long controller_index,
	unsigned long character_flags,
	wchar_t const* default_text,
	wchar_t const* title_text,
	wchar_t const* description_text,
	wchar_t* result_text,
	unsigned long maximum_character_count,
	void* platform_handle
);

