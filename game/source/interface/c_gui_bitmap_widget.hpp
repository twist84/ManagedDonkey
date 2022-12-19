#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"

struct s_bitmap_widget_definition : s_core_widget_definition
{
	tag_reference bitmap_tag;
	tag_reference custom_pixel_shader;
	short_enum bitmap_blend_method;
	short initial_sprite_sequence;
	short initial_sprite_frame;
	byte : 8;
	byte : 8;
	string_id value_override_list;
	string_id value_identifier;
};
static_assert(sizeof(s_bitmap_widget_definition) == sizeof(s_core_widget_definition) + 0x30);

struct s_runtime_bitmap_widget_definition : s_runtime_core_widget_definition
{
	long bitmap_tag_reference_index;
	long __unknown38;
	long bitmap_blend_method;
	short initial_sprite_sequence;
	short initial_sprite_frame;
	string_id value_override_list;
	string_id value_identifier;
};
static_assert(sizeof(s_runtime_bitmap_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x18);

struct c_gui_bitmap_widget : public c_gui_widget
{
public:
	void __cdecl set_sprite_frame(long sprite_frame);

protected:
	long __unknownDC;
	long __unknownE0;
	long m_sprite_frame;
	long __unknownE8;
	s_runtime_bitmap_widget_definition m_core_definition;
};
static_assert(sizeof(c_gui_bitmap_widget) == 0x138);
