#pragma once

extern bool __cdecl get_clipboard_as_text(char* buf, long len);

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

extern long XShowConnectUI(
	wchar_t const* default_ip_text,
	wchar_t const* default_port_text,
	wchar_t const* default_id_text,
	wchar_t const* default_address_text,
	wchar_t* result_ip_text,
	wchar_t* result_port_text,
	wchar_t* result_id_text,
	wchar_t* result_address_text,
	void* platform_handle,
	bool* should_connect
);

