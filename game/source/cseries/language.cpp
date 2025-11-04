#include "cseries/language.hpp"

#include "cseries/cseries_events.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "main/main_game.hpp"
#include "text/font_loading.hpp"

#include <windows.h>

REFERENCE_DECLARE(0x0189DEE4, e_language, g_current_language);

const char* k_language_names[k_language_count]
{
	"english",
	"japanese",
	"german",
	"french",
	"spanish",
	"mexican",
	"italian",
	"korean",
	"chinese-traditional",
	"chinese-simplified",
	"portuguese",
	"russian"
};

const char* k_language_suffix_names[k_language_count]
{
	"en",
	"jpn",
	"de",
	"fr",
	"sp",
	"mx",
	"it",
	"kor",
	"cht",
	"chs",
	"pt",
	"ru"
};

const char* k_language_iso_639_1_names[k_language_count]
{
	"en-US",
	"jp-JP",
	"de-DE",
	"fr-FR",
	"es-ES",
	"es-MX",
	"it-IT",
	"ko-KR",
	"zh-TW",
	"zh-CN",
	"pt-PT",
	"ru-RU"
};

e_language get_e_language_from_windows_language(LANGID id)
{
	e_language result = _language_invalid;
	switch (id)
	{
	case LANG_CHINESE:
		result = _language_chinese_simplified;
		break;
	case LANG_GERMAN:
		result = _language_german;
		break;
	case LANG_SPANISH:
		result = _language_spanish;
		break;
	case LANG_FRENCH:
		result = _language_french;
		break;
	case LANG_ITALIAN:
		result = _language_italian;
		break;
	case LANG_JAPANESE:
		result = _language_japanese;
		break;
	case LANG_KOREAN:
		result = _language_korean;
		break;
	case LANG_PORTUGUESE:
		result = _language_portuguese;
		break;
	case LANG_RUSSIAN:
		result = _language_russian;
		break;
	default:
		result = k_language_default;
		break;
	}

	return result;
}

e_language get_current_language()
{
	return INVOKE(0x0052FC40, get_current_language);

	if (g_current_language == _language_invalid)
	{
		g_current_language = get_e_language_from_windows_language(GetSystemDefaultUILanguage());
		if (g_current_language == _language_invalid)
		{
			event(_event_warning, "Failed to find language, defaulting to english.");
			g_current_language = _language_english;
		}
	}
	
	return g_current_language;
}

const char* __cdecl get_current_language_suffix(bool english_is_empty)
{
	//return INVOKE(0x0052FD20, get_current_language_suffix, english_is_empty);

	return get_language_suffix(get_current_language(), english_is_empty);
}

const char* get_language_display_name(e_language language)
{
	//return INVOKE(0x0052FDC0, get_language_display_name, language);

	const char* result = "";
	if (IN_RANGE(language, _language_invalid, k_language_count))
	{
		result = k_language_names[language];
	}
	return result;
}

e_language get_language_from_display_name_slow(const char* display_name)
{
	//return INVOKE(0x0052FE60, get_language_from_display_name_slow, display_name);

	e_language language = k_language_default;
	while (true)
	{
		if (csstricmp(display_name, get_language_display_name(language)) == 0)
		{
			break;
		}

		if (++language >= k_language_count)
		{
			return _language_invalid;
		}
	}
	return language;
}

const char* __cdecl get_language_iso_639_1_name(e_language language)
{
	//return INVOKE(0x0052FF30, get_language_display_name, language);

	const char* result = "";
	if (IN_RANGE(language, _language_invalid, k_language_count))
	{
		result = k_language_iso_639_1_names[language];
	}
	return result;
}

const char* __cdecl get_language_suffix(e_language language, bool english_is_empty)
{
	//return INVOKE(0x0052FFD0, get_language_suffix, language, english_is_empty);

	const char* result = "";
	switch (language)
	{
	case _language_english:
	{
		if (!english_is_empty)
		{
			result = "en";
		}
	}
	break;
	case _language_japanese:
	{
		result = "jpn";
	}
	break;
	case _language_german:
	{
		result = "de";
	}
	break;
	case _language_french:
	{
		result = "fr";
	}
	break;
	case _language_spanish:
	{
		result = "sp";
	}
	break;
	case _language_mexican_spanish:
	{
		result = "mx";
	}
	break;
	case _language_italian:
	{
		result = "it";
	}
	break;
	case _language_korean:
	{
		result = "kor";
	}
	break;
	case _language_chinese_traditional:
	{
		result = "cht";
	}
	break;
	case _language_chinese_simplified:
	{
		result = "chs";
	}
	break;
	case _language_portuguese:
	{
		result = "pt";
	}
	break;
	case _language_russian:
	{
		result = "ru";
	}
	break;
	}
	return result;
}

void __cdecl get_localized_data_directory_name(e_language language, char* buffer, int32 count)
{
	//INVOKE(0x00530070, get_localized_data_directory_name, language, buffer, count);

	ASSERT(buffer != NULL);
	ASSERT(count > 0);

	const char* language_suffix = get_language_suffix(language, 1);
	*buffer = 0;
	csstrnzcat(buffer, "data", count);
	if (*language_suffix)
	{
		csstrnzcat(buffer, "_", count);
		csstrnzcat(buffer, language_suffix, count);
	}
}

void __cdecl set_current_language(e_language language)
{
	//INVOKE(0x00530100, set_current_language, language);

	if (language != g_current_language)
	{
		g_current_language = language;
		font_reload();
		window_manager_reset_screens();
		main_game_reload_map(NONE);
	}
}

void __cdecl set_current_language_from_display_name_slow(const char* display_name)
{
	//INVOKE(0x00530130, set_current_language_from_display_name_slow, display_name);

	e_language language = get_language_from_display_name_slow(display_name);
	if (language != _language_invalid)
	{
		set_current_language(language);
	}
}