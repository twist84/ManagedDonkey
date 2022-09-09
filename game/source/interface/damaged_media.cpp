#include "interface/damaged_media.hpp"

#include "cseries/cseries.hpp"

#include <windows.h>

char const fatal_error_message[] = "There once was a man from Bungie...... Nothing rhymes with Bungie.This is the worst possible way we can emulate a stupid !@#$%^& OS exception. And it's not our fault. And that makes it suck even more.I are sad.";

void damaged_media_exception()
{
    if (!IsDebuggerPresent())
        throw fatal_error_message;

    DebugBreak();
}
