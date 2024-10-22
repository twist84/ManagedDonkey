#pragma once

#include "cseries/cseries.hpp"
#include "rasterizer/rasterizer_text.hpp"

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

//protected:
	bool m_rendering;
	long m_x;
	long m_y;
	long m_width;
	long m_height;
	long m_column_width;
	long m_row_height;
	long m_max_column;
	long m_max_row;
	char m_string[1024];
};
static_assert(sizeof(c_simple_font_screen_display) == 0x424);

struct bitmap_data;
namespace simple_font
{
	struct s_simple_font_globals
	{
		bool __unknown0;
	};
	static_assert(sizeof(s_simple_font_globals) == 0x1);

	struct s_font_data
	{
		void install();

		bool installed;
		byte __data1[0x3];
		long width;
		long height;
		byte __dataC[0xC];
		real scale;
		long texture_width;
		long texture_height;
		bitmap_data* texture_bitmap;
		c_rasterizer_texture_ref texture_ref;
		long __unknown2C;
		long x1;
		long y1;
		char* buffer;
	};
	static_assert(sizeof(s_font_data) == 0x3C);
	
	extern bool __cdecl begin_rendering(real scale, bool a2);
	extern void __cdecl end_rendering();
	extern long __cdecl get_height();
	extern long __cdecl get_width();
	extern void __cdecl install();
	extern void __cdecl print(long a1, long a2, dword a3, char const* string, long string_length, bool a6);

	extern s_font_data*(&g_fonts)[2];
	extern s_font_data& g_font_6x10;
	extern s_font_data& g_font_4x6;

	extern s_simple_font_globals& g_simple_font_globals;
	extern s_font_data*& g_activeFont;
	extern bool& __unknown8;     // no_aspect_ratio_scaling?
	extern vector2d& __vectorC;  // screenspace_offset?
	extern vector2d& __vector14; // aspect_ratio_scaling?
};

