#include "main/console.hpp"

#include "interface/terminal.hpp"
#include "main/debug_keys.hpp"

void __cdecl console_initialize()
{
	static bool initialize_console = true;
	if (initialize_console)
	{
		terminal_initialize();
		//console_status_render = true;
		//console_open_text.set("halo( ");
		debug_keys_initialize();

		initialize_console = false;
	}
}

void __cdecl console_dispose()
{
	static bool dispose_console = true;
	if (dispose_console)
	{
		debug_keys_dispose();
		terminal_dispose();

		dispose_console = false;
	}
}

