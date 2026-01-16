#pragma once

#include "cseries/cseries.hpp"
#include "rasterizer/rasterizer_text.hpp"

class c_simple_font_screen_display
{
public:
	c_simple_font_screen_display();
	~c_simple_font_screen_display();

	void close_session();
	void draw(int32 column, int32 row, uns32 color, const char* format, ...);
	bool open_session(real32 scale);

	//int32 get_max_column() const;
	//int32 get_max_row() const;
	//void get_text_row_column_from_screen_pos(point2d, int32*, int32*) const;
	//void get_text_row_column_from_screen_pos(const real_point2d*, int32*, int32*) const;
	//void get_screen_pos_from_text_column_row(int32, int32, point2d*) const;
	//int32 get_column_width(int32) const;
	//int32 get_char_width() const;

//protected:
	bool m_print_session_open;
	int32 m_base_x;
	int32 m_base_y;
	int32 m_total_width;
	int32 m_total_height;
	int32 m_char_width;
	int32 m_char_height;
	int32 m_max_column;
	int32 m_max_row;
	char buffer[1024];
};
COMPILE_ASSERT(sizeof(c_simple_font_screen_display) == 0x424);

struct bitmap_data;
struct rasterizer_vertex_screen;
namespace simple_font
{
	struct s_simple_font_globals
	{
		bool render_drop_shadow;
	};
	COMPILE_ASSERT(sizeof(s_simple_font_globals) == 0x1);

	struct s_font_data
	{
		void install();
		void remove();

		bool installed;

		int32 const char_width;
		int32 const char_height;
		int32 const char_pitch;
		int32 const char_start;
		int32 const char_end;
		real32 char_scale;

		int32 const texture_width;
		int32 const texture_height;
		bitmap_data* bitmap;
		c_rasterizer_texture_ref hardware_format;
		int32 pitch;

		int32 const source_width;
		int32 const source_height;
		const char* source_data;
	};
	COMPILE_ASSERT(sizeof(s_font_data) == 0x3C);

	extern bool __cdecl begin_rendering(real32 scale, bool drop_shadow);
	extern void __cdecl draw_quads(rasterizer_vertex_screen* triangle_vertices, int32 verts_used, bool apply_display_scalar_correction);
	extern void __cdecl end_rendering();
	extern int32 __cdecl get_height();
	extern int32 __cdecl get_width();
	extern void __cdecl install();
	extern int32 __cdecl make_quad(int32 x1, int32 y1, real32 u1, real32 v1, int32 x2, int32 y2, real32 u2, real32 v2, uns32 color, int32 shadow_offset, rasterizer_vertex_screen* triangle_vertices);
	extern void __cdecl print(int32 x, int32 y, uns32 color, const char* c, int32 count, bool apply_display_scalar_correction);
	extern void __cdecl print_block(int32 x, int32 y, int32 width, int32 height, uns32 color, const char* c);
	extern void __cdecl print_white(int32 x, int32 y, const char* c, int32 count);
	extern void __cdecl printf(int32 x, int32 y, uns32 color, const char* c, ...);
	extern void __cdecl printf(int32 x, int32 y, const char* c, ...);
	extern void __cdecl printf_down(int32 x, int32 y, uns32 color, const char* c, ...);
	extern void __cdecl printf_left(int32 x, int32 y, uns32 color, const char* c, ...);
	extern void __cdecl printf_up(int32 x, int32 y, uns32 color, const char* c, ...);
	extern void __cdecl remove();
	extern void __cdecl render_text(const s_font_data* font, int32 pixelX, int32 pixelY, int32 pixelPitch, int32 flags, uns32 color, const char* text, unsigned int charCount, bool apply_display_scalar_correction);
	extern void __cdecl vprintf(int32 x, int32 y, int32 flags, uns32 color, const char* c, char* arglist);

	extern s_font_data*(&g_fonts)[2];
	extern s_font_data& g_font_6x10;
	extern s_font_data& g_font_4x6;

	extern s_simple_font_globals& g_simple_font_globals;
	extern s_font_data*& g_activeFont;

	extern bool& x_initialized;
	extern real32& x_one_over_window_bounds_width;
	extern real32& x_one_over_window_bounds_height;
	extern real_vector2d& x_aspect_ratio_scale;
};

