#include "interface/damaged_media.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"

#include <windows.h>

HOOK_DECLARE(0x00A9F6D0, damaged_media_exception);
HOOK_DECLARE(0x00A9F6D0, damaged_media_update);

char const fatal_error_message[] = "There once was a man from Bungie...... Nothing rhymes with Bungie.This is the worst possible way we can emulate a stupid !@#$%^& OS exception. And it's not our fault. And that makes it suck even more.I are sad.";

bool& g_show_damaged_media = *reinterpret_cast<bool*>(0x05260711);

void damaged_media_exception()
{
    FUNCTION_BEGIN(true);

    //HOOK_INVOKE(, damaged_media_exception);

    if (!IsDebuggerPresent())
        throw fatal_error_message;

    DebugBreak();
}

void __cdecl damaged_media_update()
{
    FUNCTION_BEGIN(false);

    //HOOK_INVOKE(, damaged_media_update);

    if (g_show_damaged_media)
        damaged_media_exception();
}