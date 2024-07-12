#pragma once

#include <stdio.h>

class c_console
{
public:
	static void initialize(char const* window_title);
	static void dispose();

	static void write(char const* format, ...);
	static void write_line(char const* format, ...);
	static void write(wchar_t const* format, ...);
	static void write_line(wchar_t const* format, ...);

	static void write_va(char const* format, va_list list);
	static void write_line_va(char const* format, va_list list);
	static void write_va(wchar_t const* format, va_list list);
	static void write_line_va(wchar_t const* format, va_list list);

protected:
	static bool m_initialized;
	static FILE* m_file;
};

extern void get_error_message(unsigned long message_id, char(&message_buffer)[2048]);

