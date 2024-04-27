#pragma once

#include "cseries/cseries.hpp"

struct c_simple_font_screen_display
{
public:
	c_simple_font_screen_display();
	~c_simple_font_screen_display();

	void close_session();
	void draw(long, long, dword, char const* format, ...);
	bool open_session(real scale);

	//long get_max_column() const;
	//long get_max_row() const;
	//void get_text_row_column_from_screen_pos(int16_point2d, long*, long*) const;
	//void get_text_row_column_from_screen_pos(real_point2d const*, long*, long*) const;
	//void get_screen_pos_from_text_column_row(long, long, int16_point2d*) const;
	//long get_column_width(long) const;
	//long get_char_width() const;

protected:
	bool m_rendering;
	long m_x;
	long m_y;
	long m_width;
	long m_height;
	long m_column_width;
	long m_char_height;
	long m_max_column;
	long m_max_row;
	char m_string[1024];
};
static_assert(sizeof(c_simple_font_screen_display) == 0x424);

