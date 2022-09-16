#pragma once

#include "cseries/cseries.hpp"

enum e_language
{
	_language_invalid = -1,

	_language_english,
	_language_japanese,
	_language_german,
	_language_french,
	_language_spanish,
	_language_mexican_spanish,
	_language_italian,
	_language_korean,
	_language_chinese_traditional,
	_language_chinese_simplified,
	_language_portuguese,
	_language_russian,

	k_language_count,
	k_language_default = _language_english
};

extern e_language get_current_language();
extern char const* __cdecl get_current_language_suffix(bool a1);
extern char const* get_language_display_name(e_language language);
extern e_language get_language_from_display_name_slow(char const* display_name);
extern char const* __cdecl get_language_iso_639_1_name(e_language language);
extern char const* __cdecl get_language_suffix(e_language language, bool a2);
extern void __cdecl get_localized_data_directory_name(e_language language, char* buffer, long count);
extern void __cdecl set_current_language(e_language language);
extern void __cdecl set_current_language_from_display_name_slow(char const* display_name);