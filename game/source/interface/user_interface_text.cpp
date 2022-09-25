#include "interface/user_interface_text.hpp"

#include <assert.h>

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