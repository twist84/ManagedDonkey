#pragma once

#include "cseries/cseries.hpp"
#include "objects/emblems.hpp"
#include "interface/c_gui_widget.hpp"

struct bitmap_data;
class c_tag_resource_demand_collector;
struct s_gui_bitmap_widget_render_data;

enum e_bitmap_widget_definition_flags
{
	_bitmap_widget_definition_flag_scale_to_fit_bounds_bit = k_core_widget_definition_flags,
	_bitmap_widget_definition_flag_render_as_screen_blur_bit,
	_bitmap_widget_definition_flag_render_as_player_emblem_bit,
	_bitmap_widget_definition_flag_sprite_from_exported_integer_bit,
	_bitmap_widget_definition_flag_sequence_from_exported_integer_bit,
	_bitmap_widget_definition_flag_attach_shader_to_exported_integer_bit,
	_bitmap_widget_definition_flag_allow_list_item_to_override_animation_skin_bit,
	_bitmap_widget_definition_flag_can_recieve_focus_bit,
	_bitmap_widget_definition_flag_scale0_5_bit,
	_bitmap_widget_definition_flag_stretch_bit,

	k_bitmap_widget_definition_flags
};

struct s_bitmap_widget_definition :
	s_core_widget_definition
{
	s_tag_reference bitmap_reference;
	s_tag_reference explicit_shader_reference;
	int16 render_blend_mode;
	int16 initial_sprite_sequence;
	int16 initial_sprite_frame;
	int16 pad0;
	int32 value_override_list;
	int32 value_identifier;
};
static_assert(sizeof(s_bitmap_widget_definition) == 0x5C);
static_assert(sizeof(s_bitmap_widget_definition) == sizeof(s_core_widget_definition) + 0x30);

struct s_bitmap_widget_block
{
	s_bitmap_widget_block()
	{
		DECLFUNC(0x00AB5C70, void, __thiscall, s_bitmap_widget_block*)(this);
	}

	s_tag_reference widget_template_reference;
	s_bitmap_widget_definition override_definition;
};
static_assert(sizeof(s_bitmap_widget_block) == 0x6C);

struct s_runtime_bitmap_widget_definition :
	s_runtime_core_widget_definition
{
	s_runtime_bitmap_widget_definition()
	{
		DECLFUNC(0x00AB5EA0, void, __thiscall, s_runtime_bitmap_widget_definition*)(this);
	}

	int32 bitmap_reference_index;
	int32 explicit_shader_reference_index;
	int32 render_blend_mode;
	int16 initial_sprite_sequence;
	int16 initial_sprite_frame;
	string_id value_override_list;
	string_id value_identifier;
};
static_assert(sizeof(s_runtime_bitmap_widget_definition) == sizeof(s_runtime_core_widget_definition) + 0x18);

class c_gui_bitmap_widget :
	public c_gui_widget
{
public:
	void __thiscall assemble_render_data_(s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation);

protected:
	virtual e_animation_state get_ambient_state() override;

public:
	virtual ~c_gui_bitmap_widget() = default;
	virtual s_runtime_core_widget_definition* get_core_definition() override;
	virtual real_rectangle2d* get_current_bounds(real_rectangle2d* unanimated_bounds) override;
	virtual bool can_receive_focus() override;
	virtual bool within_focus_chain() override;
	virtual void update_render_state(uns32 current_milliseconds) override;
	virtual void set_animated_state_baseline(s_animation_transform* transform) override;
	virtual void assemble_render_data(s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation) override;
	virtual void initialize(const s_bitmap_widget_block* template_and_override_block);
	virtual const bitmap_data* get_current_bitmap() const;
	virtual void __func39(c_tag_resource_demand_collector* demand_collector);
	virtual void __func40(c_tag_resource_demand_collector* demand_collector);

public:
	c_gui_bitmap_widget();
	static void __cdecl add_definition_fields(const s_bitmap_widget_definition* source_definition, s_runtime_bitmap_widget_definition* dest_definition, real_rectangle2d* positioning_bounds, bool was_templated);
	bool renders_as_player_emblem() const;
	void set_sprite_frame(int32 sprite_frame);
	void set_sprite_sequence(int32 sprite_sequence);

//protected:
	int32 m_override_sprite_bitmap_index;
	int32 m_override_sprite_frame;
	int32 m_override_sprite_sequence;
	s_runtime_bitmap_widget_definition m_definition;
};
static_assert(sizeof(c_gui_bitmap_widget) == 0x138);
static_assert(sizeof(c_gui_bitmap_widget) == sizeof(c_gui_widget) + 0x58);
static_assert(0xE0 == OFFSETOF(c_gui_bitmap_widget, m_override_sprite_bitmap_index));
static_assert(0xE4 == OFFSETOF(c_gui_bitmap_widget, m_override_sprite_frame));
static_assert(0xE8 == OFFSETOF(c_gui_bitmap_widget, m_override_sprite_sequence));
static_assert(0xEC == OFFSETOF(c_gui_bitmap_widget, m_definition));

struct s_gui_bitmap_widget_render_data :
	s_gui_widget_render_data
{
	union u_source_data
	{
		struct s_texture_camera_data
		{
			real32 __unknown0;
			real32 __unknown4;
			real32 __unknown8;
			real32 __unknownC;
		};
		static_assert(sizeof(s_texture_camera_data) == 0x10);

		struct s_sprite_data
		{
			int16 sequence;
			int16 frame;
		};
		static_assert(sizeof(s_sprite_data) == 0x4);

		struct s_custom_bitmap
		{
			int32 storage_index;
		};
		static_assert(sizeof(s_custom_bitmap) == 0x4);

		s_texture_camera_data texture_camera;
		s_sprite_data sprite;
		s_emblem_info emblem;
		s_custom_bitmap custom_bitmap;
	};
	static_assert(sizeof(u_source_data) == 0x10);

	int32 bitmap_definition_index;
	u_source_data source;
	uns32 argb_tint;
	real_point2d texture_uv_offset;
	int32 frame_buffer_blend_function;
	int32 explicit_shader_index;
};
//static_assert(sizeof(s_gui_bitmap_widget_render_data) == 0x54);
static_assert(sizeof(s_gui_bitmap_widget_render_data) == sizeof(s_gui_widget_render_data) + 0x28);

extern void __cdecl render_bitmap(const s_gui_bitmap_widget_render_data* render_data, const rectangle2d* window_bounds);

