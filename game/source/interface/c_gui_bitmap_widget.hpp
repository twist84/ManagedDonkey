#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"

enum e_bitmap_widget_definition_flags
{
	_bitmap_widget_definition_flag_scale_to_fit_bounds_bit = k_core_widget_definition_flag_count,
	_bitmap_widget_definition_flag_render_as_screen_blur_bit,
	_bitmap_widget_definition_flag_render_as_player_emblem_bit,
	_bitmap_widget_definition_flag_sprite_from_exported_integer_bit,
	_bitmap_widget_definition_flag_sequence_from_exported_integer_bit,
	_bitmap_widget_definition_flag_attach_shader_to_exported_integer_bit,
	_bitmap_widget_definition_flag_allow_list_item_to_override_animation_skin_bit,
	_bitmap_widget_definition_flag_can_recieve_focus_bit,
	_bitmap_widget_definition_flag_scale0_5_bit,
	_bitmap_widget_definition_flag_stretch_bit,

	k_bitmap_widget_definition_flag_count
};

struct s_bitmap_widget_definition : s_core_widget_definition
{
	s_tag_reference bitmap_tag;
	s_tag_reference custom_pixel_shader;
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
	void __cdecl set_sprite_sequence(long sprite_sequence);

protected:
	long __unknownDC;
	long __unknownE0;
	long m_sprite_frame;
	long m_sprite_sequence;
	s_runtime_bitmap_widget_definition m_core_definition;
};
static_assert(sizeof(c_gui_bitmap_widget) == 0x138);
