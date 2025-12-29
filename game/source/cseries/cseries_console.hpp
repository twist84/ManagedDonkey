#pragma once

#include <stdio.h>

class c_console
{
public:
	static void initialize(const char* window_title);
	static void dispose();
	static void toggle_window_visibility();
	static void clear();

	static void enable_ansi();

private:
	static bool console_allocated();

public:
	static void write(const char* format, ...);
	static void write_line(const char* format, ...);
	static void write(const wchar_t* format, ...);
	static void write_line(const wchar_t* format, ...);

	static void write_va(const char* format, va_list list);
	static void write_line_va(const char* format, va_list list);
	static void write_va(const wchar_t* format, va_list list);
	static void write_line_va(const wchar_t* format, va_list list);

protected:
	static bool m_initialized;
	static FILE* m_file;
};

extern void get_error_message(uns32 message_id, char(&message_buffer)[2048]);

