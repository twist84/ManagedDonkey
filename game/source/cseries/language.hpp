#pragma once

#include "cseries/cseries.hpp"

enum e_language
{
	k_first_language = 0,

	_language_english = k_first_language,
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
	_language_russian, // polish replaced with russian

	k_language_count,

	_language_invalid = -1,

	k_language_default = k_first_language
};

static inline e_language& operator++(e_language& value)
{
	return value = e_language(value + 1);
}

extern e_language get_current_language();
extern char const* __cdecl get_current_language_suffix(bool english_is_empty);
extern char const* get_language_display_name(e_language language);
extern e_language get_language_from_display_name_slow(char const* display_name);
extern char const* __cdecl get_language_iso_639_1_name(e_language language);
extern char const* __cdecl get_language_suffix(e_language language, bool english_is_empty);
extern void __cdecl get_localized_data_directory_name(e_language language, char* buffer, long count);
extern void __cdecl set_current_language(e_language language);
extern void __cdecl set_current_language_from_display_name_slow(char const* display_name);