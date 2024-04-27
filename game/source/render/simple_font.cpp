#include "render/simple_font.hpp"

#include <string.h>

namespace simple_font // probably struct
{
	//struct s_font_data;
	//REFERENCE_DECLARE(0x0524B6B8, s_font_data*, g_activeFont);

	//void __cdecl render_text(s_font_data* a1, long a2, long a3, long a4, long a5, dword a6, char const* string, unsigned int string_length, bool a9)
	//{
	//	INVOKE(0x00A77480, render_text, a1, a2, a3, a4, a5, a6, string, string_length, a9);
	//}

	void __cdecl print(long a1, long a2, dword a3, char const* string, long string_length, bool a6)
	{
		INVOKE(0x00A770E0, print, a1, a2, a3, string, string_length, a6);

		//render_text(g_activeFont, a1, a2, 1024, 0, a3, string, string_length, a6);
	}
}

c_simple_font_screen_display::c_simple_font_screen_display() :
	m_rendering(false),
	m_x(),            // not initialized
	m_y(),            // not initialized
	m_width(),        // not initialized
	m_height(),       // not initialized
	m_column_width(), // not initialized
	m_char_height(),  // not initialized
	m_max_column(),   // not initialized
	m_max_row(),      // not initialized
	m_string()        // not initialized
{
}

c_simple_font_screen_display::~c_simple_font_screen_display()
{
	close_session();
}

void c_simple_font_screen_display::close_session()
{
	DECLFUNC(0x00A767F0, void, __thiscall, c_simple_font_screen_display*)(this);
}

void c_simple_font_screen_display::draw(long a1, long a2, dword a3, char const* format, ...)
{
	//DECLFUNC(0x00A76960, void, __thiscall, c_simple_font_screen_display*, long, long, dword, char const*, ...)(this, a1, a2, a3, format);

	va_list list;
	va_start(list, format);

	cvsnzprintf(m_string, sizeof(m_string) - 1, format, list);
	simple_font::print(
		m_x + a1 * m_column_width,
		m_y + a2 * m_char_height,
		a3,
		m_string,
		strlen(m_string),
		false);
}

bool c_simple_font_screen_display::open_session(real scale)
{
	return DECLFUNC(0x00A76FA0, bool, __thiscall, c_simple_font_screen_display*, real)(this, scale);
}

