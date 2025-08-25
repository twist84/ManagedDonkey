#pragma once

#include "interface/c_gui_widget.hpp"
#include "interface/user_interface_text.hpp"

enum e_text_widget_definition_flags
{
	_text_widget_definition_flag_left_justify_bit = k_core_widget_definition_flags,
	_text_widget_definition_flag_right_justify_bit,
	_text_widget_definition_flag_scrollable_bit,
	_text_widget_definition_flag_uppercase_bit,
	_text_widget_definition_flag_string_from_exported_text_bit,
	_text_widget_definition_flag_string_from_exported_string_id_bit,
	_text_widget_definition_flag_string_from_exported_global_string_id_bit,
	_text_widget_definition_flag_string_from_exported_integer_bit,
	_text_widget_definition_flag_string_from_list_item_label_bit,
	_text_widget_definition_flag_use_brackets_to_indicate_focus_bit,
	_text_widget_definition_flag_large_text_buffer_255_chars_bit,
	_text_widget_definition_flag_extra_large_text_buffer_bit,
	_text_widget_definition_flag_single_drop_shadow_bit,
	_text_widget_definition_flag_no_drop_shadow_bit,
	_text_widget_definition_flag_allow_list_item_to_override_animation_skin_bit,
	_text_widget_definition_flag_do_not_wrap_text_bit,

	k_text_widget_definition_flags
};

struct s_text_widget_definition :
	s_core_widget_definition
{
	string_id value_override_list;
	string_id value_identifier;
	string_id text_color_preset;
	int16 font;
	int16 pad0;
};
static_assert(sizeof(s_text_widget_definition) == 0x3C);
static_assert(sizeof(s_text_widget_definition) == sizeof(s_core_widget_definition) + 0x10);

struct s_text_widget_block
{
	s_tag_reference widget_template_reference;
	s_text_widget_definition override_definition;
};
static_assert(sizeof(s_text_widget_block) == 0x4C);

struct s_runtime_text_widget_definition :
	s_runtime_core_widget_definition
{
	string_id value_override_list;
	string_id value_identifier;
	real_argb_color text_color;
	e_font_id font;
};
static_assert(sizeof(s_runtime_text_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x1C);

class c_gui_screen_widget;
struct s_text_widget_block;
class c_gui_text_widget :
	public c_gui_widget
{
public:
	void __thiscall assemble_render_data_(s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation);
	bool __thiscall set_string_from_string_id_(c_static_wchar_string<1024>* buffer, c_gui_widget* string_list_owner, int32 value_name, bool forcibly_set_text);

	struct s_text_source_data
	{
		enum e_text_source
		{
			_source_invalid = -1,
			_source_raw,
			_source_string_id,
			_source_exported_data,
		};

		s_text_source_data();

		e_text_source type;
		int32 name;
		int32 element_handle;
	};
	static_assert(sizeof(s_text_source_data) == 0xC);

public:
	virtual ~c_gui_text_widget() = default;
	virtual s_runtime_core_widget_definition* get_core_definition() override;
	virtual real_argb_color* get_cumulative_color_tint(real_argb_color* tint) override;
	virtual bool within_focus_chain() override;
	virtual void update(uns32 current_milliseconds) override;
	virtual void update_render_state(uns32 current_milliseconds) override;
	virtual void set_animated_state_baseline(s_animation_transform* transform) override;
	virtual void assemble_render_data(s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation) override;
	virtual c_user_interface_text* get_text_internal() = 0;
	virtual unsigned int get_text_buffer_size() const = 0;
	virtual void initialize(const s_text_widget_block* template_and_override_block);
	virtual void set_text(const wchar_t* text);
	virtual void set_text_from_string_id(c_gui_screen_widget* screen, int32 id);
	virtual const wchar_t* get_text();

private:
	void compute_text_bounds(const rectangle2d* window_bounds, real_point2d rotation_origin, real32 position_x, real32 position_y, real32 depth, real32 scale_i, real32 scale_j, real_point2d* out_rotation_origin, real_rectangle2d* out_transformed_text_rect, real_rectangle2d* out_transformed_clip_rect, real32* out_glyph_scale);
	bool set_string_from_string_id(c_static_wchar_string<1024>* buffer, c_gui_widget* string_list_owner, int32 value_name, bool forcibly_set_text);
	void set_text_internal(const wchar_t* string, const s_text_source_data* source_data, bool forcibly_set);

protected:
	s_text_source_data m_text_source_data;
	s_runtime_text_widget_definition m_definition;
	int32 __unknown13C;
};
static_assert(sizeof(c_gui_text_widget) == 0x140);
static_assert(sizeof(c_gui_text_widget) == sizeof(c_gui_widget) + 0x60);

template<int32 k_maximum_count>
class c_sized_user_interface_text :
	public c_user_interface_text
{
public:
	//virtual void set_string(const wchar_t* new_string, bool parse_xml, int32 screen_index)
	//{
	//	// $IMPLEMENT
	//}
	//
	//virtual const wchar_t* get_string()
	//{
	//	return m_text_render_buffer.get_string();
	//}
	//
	//virtual void update(int32 user_index)
	//{
	//	// $IMPLEMENT
	//}

public:
	//c_sized_user_interface_text() :
	//	c_user_interface_text(),
	//	m_text_render_buffer(),
	//	m_text_cache_buffer()
	//{
	//}

protected:
	c_static_wchar_string<k_maximum_count> m_text_render_buffer;
	c_static_wchar_string<k_maximum_count> m_text_cache_buffer;
};
static_assert(sizeof(c_sized_user_interface_text<48>) == 0x11C);
static_assert(sizeof(c_sized_user_interface_text<256>) == 0x45C);
static_assert(sizeof(c_sized_user_interface_text<1024>) == 0x105C);

template<int32 k_text_buffer_size>
class c_gui_sized_text_widget :
	public c_gui_text_widget
{
public:
	//virtual ~c_gui_sized_text_widget() = default
	//
	//virtual c_user_interface_text* get_text_internal() override
	//{
	//	return &m_text;
	//}
	//
	//virtual unsigned int get_text_buffer_size() const override
	//{
	//	return k_text_buffer_size;
	//}

public:
	//c_gui_sized_text_widget() :
	//	c_gui_text_widget(),
	//	m_text()
	//{
	//}

protected:
	c_sized_user_interface_text<k_text_buffer_size> m_text;
};
static_assert(sizeof(c_gui_sized_text_widget<48>) == 0x25C);
static_assert(sizeof(c_gui_sized_text_widget<256>) == 0x59C);
static_assert(sizeof(c_gui_sized_text_widget<1024>) == 0x119C);

