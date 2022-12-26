#include "interface/user_interface_text.hpp"

#include "config/version.hpp"
#include "memory/module.hpp"

#include <assert.h>
#include <string.h>

HOOK_DECLARE(0x00ABC070, parse_build_number_string);

void wchar_string_sanitize_for_game(wchar_t* string, long maximum_character_count)
{
	assert(string != NULL);

	wchar_t v1[3][2]
	{
        { '<', '[' },
        { '>', ']' },
        { '|', '-' }
	};

    bool v2 = 0;
    do
    {
        for (long i = 0; i < maximum_character_count && string[i]; ++i)
        {
            for (long j = 0; j < 3; ++j)
            {
                if (string[i] == v1[j][0])
                {
                    string[i] = v1[j][1];
                    v2 = 1;
                    break;
                }
            }
        }
    } while (v2);
}

// this hook never gets hit
void __cdecl parse_build_number_string(e_controller_index controller_index, e_utf32 utf32, c_static_wchar_string<1024>* out_string)
{
    char const* build_name = version_get_build_name();
    char const* build_string = version_get_build_string();

    out_string->set(L"");

    if (strlen(build_name) > 1)
    {
        out_string->append_print(L"%hs (%hs)", build_name, build_string);
    }
    else
    {
        out_string->append_print(L"%hs", build_string);
    }
}

void magic_character_to_string(e_magic_character magic_character, wchar_t(&magic_string)[2])
{
    union
    {
        dword value;
        wchar_t str[2];
    } out = { .value = magic_character };

    ustrnzcpy(magic_string, out.str, 2);
}
