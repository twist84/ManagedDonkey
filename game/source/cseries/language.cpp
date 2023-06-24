#include "cseries/language.hpp"

#include "interface/user_interface_window_manager.hpp"
#include "main/main_game.hpp"
#include "text/font_loading.hpp"

#include <windows.h>

REFERENCE_DECLARE(0x0189DEE4, dword, g_game_language);

char const* k_language_names[k_language_count]
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

char const* k_language_suffix_names[k_language_count]
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

char const* k_language_iso_639_1_names[k_language_count]
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

    if (g_game_language != _language_invalid)
        g_game_language = get_e_language_from_windows_language(GetSystemDefaultUILanguage());

    return static_cast<e_language>(g_game_language);
}

char const* __cdecl get_current_language_suffix(bool a1)
{
    //return INVOKE(0x0052FD20, get_current_language_suffix, a1);

    return get_language_suffix(get_current_language(), a1);
}

char const* get_language_display_name(e_language language)
{
    //return INVOKE(0x0052FDC0, get_language_display_name, language);

    if (language > _language_invalid && language < k_language_count)
        return k_language_names[language];

    return "";
}

e_language get_language_from_display_name_slow(char const* display_name)
{
    //return INVOKE(0x0052FE60, get_language_from_display_name_slow, display_name);

    dword language = _language_english;

    while (true)
    {
        char const* temp = get_language_display_name(static_cast<e_language>(language));
        if (!csstricmp(display_name, temp))
            break;

        if (language++ > k_language_count)
            return _language_invalid;
    }

    return static_cast<e_language>(language);
}

char const* __cdecl get_language_iso_639_1_name(e_language language)
{
    //return INVOKE(0x0052FF30, get_language_display_name, language);

    if (language > _language_invalid && language < k_language_count)
        return k_language_iso_639_1_names[language];

    return "";
}

char const* __cdecl get_language_suffix(e_language language, bool a2)
{
    //return INVOKE(0x0052FFD0, get_language_suffix, language, a2);

    if (language > _language_invalid && language < k_language_count)
        return k_language_suffix_names[language];

    return "";
}

void __cdecl get_localized_data_directory_name(e_language language, char* buffer, long count)
{
    //INVOKE(0x00530070, get_localized_data_directory_name, language, buffer, count);

    ASSERT(buffer != NULL);
    ASSERT(count > 0);

    char const* language_suffix = get_language_suffix(language, 1);
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

    if (language != g_game_language)
    {
        g_game_language = language;
        font_reload();
        window_manager_reset_screens();
        main_game_notify_language_change(language);
    }
}

void __cdecl set_current_language_from_display_name_slow(char const* display_name)
{
    //INVOKE(0x00530130, set_current_language_from_display_name_slow, display_name);

    e_language language = get_language_from_display_name_slow(display_name);
    if (language != _language_invalid)
        set_current_language(language);
}