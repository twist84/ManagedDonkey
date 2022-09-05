#include "interface/damaged_media.hpp"

#include "cseries/cseries.hpp"

#include <windows.h>

bool test_damaged_media = false;
char const fatal_error_message[] = "There once was a man from Bungie...... Nothing rhymes with Bungie.This is the worst possible way we can emulate a stupid !@#$%^& OS exception. And it's not our fault. And that makes it suck even more.I are sad.";

void damaged_media_display()
{
    if (!IsDebuggerPresent())
        throw fatal_error_message;

    DebugBreak();
}

bool __cdecl sub_52F180(char* scenario_path, long map_load_type)
{
    if (test_damaged_media)
        return false;

    // sub_52F180 calls main_load_map_with_insertion_point(-1, scenario_path, map_load_type);
    return DECLTHUNK(0x0052F180, sub_52F180, scenario_path, map_load_type);
}
