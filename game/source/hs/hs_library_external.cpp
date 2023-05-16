#include "hs/hs_library_external.hpp"

#include "interface/terminal.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CALL(0x0072FE9A, hs_print);

void hs_print(char const* text)
{
	terminal_printf(global_real_argb_green, text);
}

