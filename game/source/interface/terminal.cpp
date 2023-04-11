#include "interface/terminal.hpp"

#include "text/draw_string.hpp"

s_terminal_globals terminal_globals;

void terminal_printf(real_argb_color const* color, char const* format, ...)
{
	//if (terminal_globals.initialized && !terminal_globals.suppress_output)
	//{
	//	c_font_cache_mt_safe font_cache;
	//	c_critical_section_scope section_scope;
	//	ASSERT(format);
	//}
}

