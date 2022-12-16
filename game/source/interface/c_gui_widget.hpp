#pragma once

#include "cseries/cseries.hpp"
#include "interface/gui_animation.hpp"
#include "tag_files/tag_groups.hpp"
#include "text/unicode.hpp"

enum e_core_widget_definition_flags
{
	_core_widget_definition_flag_do_not_apply_old_content_upscaling_bit = 0,
	_core_widget_definition_flag_override_template_flags_bit,
	_core_widget_definition_flag_enable_animation_debugging_bit,

	k_core_widget_definition_flag_count
};

enum e_widget_positioning
{
	_widget_positioning_unused = 0,
	_widget_positioning_centered,
	_widget_positioning_top_edge,
	_widget_positioning_bottom_edge,
	_widget_positioning_left_edge,
	_widget_positioning_right_edge,
	_widget_positioning_top_left_corner,
	_widget_positioning_top_right_corner,
	_widget_positioning_bottom_right_corner,
	_widget_positioning_bottom_left_corner,

	k_widget_positioning_count
};

struct s_core_widget_definition
{
	// e_core_widget_definition_flags
	dword_flags flags;

	// identification only
	c_string_id name;

	// scaled positioning & scaled anchoring
	c_enum<e_widget_positioning, short, k_widget_positioning_count> scaled_positioning;
	short render_depth_bias;

	// bounds 720p & bounds 16x9 (1152x640)
	// bounds 480i & bounds 4x3 (640x480)
	short_rectangle2d ui_bounds[2];

	c_typed_tag_reference<'wacd'> animation_collection;
};
static_assert(sizeof(s_core_widget_definition) == 0x2C);

struct s_runtime_core_widget_definition
{
	// e_core_widget_definition_flags
	dword_flags flags;

	// identification only
	c_string_id name;

	// scaled positioning & scaled anchoring
	c_enum<e_widget_positioning, long, k_widget_positioning_count> scaled_positioning;
	long render_depth_bias;

	// bounds 720p & bounds 16x9 (1152x640)
	// bounds 480i & bounds 4x3 (640x480)
	real_rectangle2d ui_bounds[2];

	long animation_collection_index;
};
static_assert(sizeof(s_runtime_core_widget_definition) == 0x34);

enum e_gui_widget_type
{
	_gui_widget_type_text_widget = 0,
	_gui_widget_type_bitmap_widget,
	_gui_widget_type_model_widget,
	_gui_widget_type_group_widget,
	_gui_widget_type_button_key_widget,
	_gui_widget_type_list_item_widget,

	// unused?
	_gui_widget_type_unknown_widget,

	_gui_widget_type_list_widget,
	_gui_widget_type_screen_widget,

	k_gui_widget_type_count
};

enum e_controller_index;

struct s_runtime_core_widget_definition;
struct c_gui_text_widget;
struct c_gui_bitmap_widget;
struct c_gui_group_widget;
struct c_gui_button_key_widget;
struct c_gui_list_widget;
struct c_gui_list_item_widget;
struct c_gui_model_widget;
struct s_runtime_text_widget_definition;
struct s_runtime_bitmap_widget_definition;
struct s_group_widget_definition;
struct s_button_key_definition;
struct s_list_widget_block;
struct s_list_item_widget_block;
struct s_model_widget_block;
struct s_animation_transform;
struct s_gui_widget_render_data;
struct c_controller_input_message;

struct gui_real_rectangle2d
{
	real __unknown0;
	real __unknown1;
	real __unknown2;
	real __unknown3;
	real __unknown4;
	real __unknown5;
	real __unknown6;
	real __unknown7;
};
static_assert(sizeof(gui_real_rectangle2d) == 0x20);

struct c_gui_widget
{
protected:

	// HACK: so we don't have to manually construct the class
	union
	{
		struct
		{
			long(__thiscall* get_ambient_state)(c_gui_widget*);
			void* (__thiscall* destructor)(c_gui_widget*, dword);
			void(__thiscall* initialize)(c_gui_widget*);
			void(__thiscall* post_initialize)(c_gui_widget*);
			void(__thiscall* dispose)(c_gui_widget*);
			long(__thiscall* get_name)(c_gui_widget*);
			long(__thiscall* get_element_handle)(c_gui_widget*);
			long(__thiscall* get_datasource_index)(c_gui_widget*);
			bool(__thiscall* get_enabled)(c_gui_widget*);
			void(__thiscall* set_enabled)(c_gui_widget*, bool);
			s_runtime_core_widget_definition* (__thiscall* get_core_definition)(c_gui_widget*);
			real_rectangle2d* (__thiscall* get_current_bounds)(c_gui_widget*, real_rectangle2d*);
			real_rectangle2d* (__thiscall* get_container_current_bounds)(c_gui_widget*, real_rectangle2d*);
			bool(__thiscall* should_render)(c_gui_widget*, bool*);
			real_argb_color* (__thiscall* get_cumulative_color_tint)(c_gui_widget*, real_argb_color*);
			bool(__thiscall* can_receive_focus)(c_gui_widget*);
			bool(__thiscall* within_focus_chain)(c_gui_widget*);
			c_gui_text_widget* (__thiscall* create_text_widget)(c_gui_widget*, s_runtime_text_widget_definition const*);
			c_gui_bitmap_widget* (__thiscall* create_bitmap_widget)(c_gui_widget*, s_runtime_bitmap_widget_definition const*);
			c_gui_group_widget* (__thiscall* create_group_widget)(c_gui_widget*, s_group_widget_definition const*);
			c_gui_button_key_widget* (__thiscall* create_button_key_widget)(c_gui_widget*, s_button_key_definition const*);
			c_gui_list_widget* (__thiscall* create_list_widget)(c_gui_widget*, s_list_widget_block const*);
			c_gui_list_item_widget* (__thiscall* create_list_item_widget)(c_gui_widget*, s_list_item_widget_block const*);
			c_gui_model_widget* (__thiscall* create_model_widget)(c_gui_widget*, s_model_widget_block const*);
			void(__thiscall* update)(c_gui_widget*, dword);
			void(__thiscall* update_render_state)(c_gui_widget*, dword);
			void(__thiscall* set_animated_state_baseline)(c_gui_widget*, s_animation_transform*);
			void* __func27;
			void* __func28;
			void(__thiscall* assemble_render_data)(c_gui_widget*, s_gui_widget_render_data*, rectangle2d const*, e_controller_index, long, bool, bool, bool);
			bool(__thiscall* handle_widget_back_out)(c_gui_widget*);
			void* __func31;
			bool(__thiscall* handle_tab)(c_gui_widget*, c_controller_input_message const*);
			void* __func33;
			void* __func34;
			void* __func35;
			bool(__thiscall* get_string_by_string_id)(c_gui_widget*, string_id, c_static_wchar_string<1024>*);
		};

		void* __funcs[64];
	}*__vftable;

	long __cdecl get_ambient_state();

public:
	void* __cdecl destructor(dword a1);
	void __cdecl initialize();
	void __cdecl post_initialize();
	void __cdecl dispose();
	long __cdecl get_name();
	long __cdecl get_element_handle();
	long __cdecl get_datasource_index();
	bool __cdecl get_enabled();
	void __cdecl set_enabled(bool a1);
	s_runtime_core_widget_definition* __cdecl get_core_definition();
	real_rectangle2d* __cdecl get_current_bounds(real_rectangle2d* a1);
	real_rectangle2d* __cdecl get_container_current_bounds(real_rectangle2d* a1);
	bool __cdecl should_render(bool* a1);
	real_argb_color* __cdecl get_cumulative_color_tint(real_argb_color* a1);
	bool __cdecl can_receive_focus();
	bool __cdecl within_focus_chain();
	c_gui_text_widget* __cdecl create_text_widget(s_runtime_text_widget_definition const* a1);
	c_gui_bitmap_widget* __cdecl create_bitmap_widget(s_runtime_bitmap_widget_definition const* a1);
	c_gui_group_widget* __cdecl create_group_widget(s_group_widget_definition const* a1);
	c_gui_button_key_widget* __cdecl create_button_key_widget(s_button_key_definition const* a1);
	c_gui_list_widget* __cdecl create_list_widget(s_list_widget_block const* a1);
	c_gui_list_item_widget* __cdecl create_list_item_widget(s_list_item_widget_block const* a1);
	c_gui_model_widget* __cdecl create_model_widget(s_model_widget_block const* a1);
	void __cdecl update(dword a1);
	void __cdecl update_render_state(dword a1);
	void __cdecl set_animated_state_baseline(s_animation_transform* a1);
	void __cdecl assemble_render_data(s_gui_widget_render_data* a1, rectangle2d const* a2, e_controller_index controller_index, long a4, bool a5, bool a6, bool a7);
	bool __cdecl handle_widget_back_out();
	bool __cdecl handle_tab(c_controller_input_message const* input_message);
	bool __cdecl get_string_by_string_id(string_id name, c_static_wchar_string<1024>* out_string);
	
	c_gui_list_item_widget* __cdecl get_next_list_item_widget(bool a1);
	void __cdecl set_visible(bool is_visible);
	c_gui_widget* __cdecl get_child_widget(e_gui_widget_type widget_type, string_id name);
	c_gui_bitmap_widget* __cdecl get_child_bitmap_widget(string_id name);
	c_gui_group_widget* __cdecl get_child_group_widget(string_id name);
	c_gui_list_item_widget* __cdecl get_child_list_item_widget(string_id name);
	c_gui_list_widget* __cdecl get_child_list_widget(string_id name);
	c_gui_model_widget* __cdecl get_child_model_widget(string_id name);
	c_gui_text_widget* __cdecl get_child_text_widget(string_id name);
	c_gui_widget* __cdecl get_first_child_widget_by_type(e_gui_widget_type widget_type);

	static void __fastcall get_unprojected_bounds(c_gui_widget* _this, void* unused, gui_real_rectangle2d* unprojected_bounds, bool a3, bool a4, bool a5);

protected:
	long __unknown4;
	c_enum<e_gui_widget_type, long, k_gui_widget_type_count> m_type;

	bool m_visible;
	bool m_enabled;
	bool __unknownE;
	bool m_use_alternate_ambient_state;

	struct s_unknown
	{
		c_gui_widget* __unknown0;
		c_gui_widget* __unknown4;
		c_gui_widget* __unknown8;
	};

	s_unknown m_parent;
	s_unknown m_children;
	s_unknown m_next;
	s_unknown m_previous;

	//c_gui_widget* m_parent;
	//c_gui_widget* m_children;
	//c_gui_widget* m_next;
	//c_gui_widget* m_previous;

	c_string_id m_name;
	real __unknown44;
	dword_flags m_flags;
	long __unknown4C;
	s_animation_transform m_full_animation_state;
	long __unknownD8;
};
static_assert(sizeof(c_gui_widget) == 0xDC);

