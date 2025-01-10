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

	k_core_widget_definition_flags
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
	// override: e_bitmap_widget_definition_flags
	// override: e_group_widget_definition_flags
	// override: e_list_widget_definition_flags
	// override: e_model_widget_definition_flags
	// override: e_screen_widget_definition_flags
	// override: e_text_widget_definition_flags
	dword_flags flags;

	// identification only
	c_string_id name;

	// scaled positioning & scaled anchoring
	c_enum<e_widget_positioning, short, _widget_positioning_unused, k_widget_positioning_count> scaled_positioning;
	short render_depth_bias;

	// bounds 720p & bounds 16x9 (1152x640)
	// bounds 480i & bounds 4x3 (640x480)
	rectangle2d ui_bounds[2];

	c_typed_tag_reference<GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG, INVALID_TAG> animation_collection;
};
static_assert(sizeof(s_core_widget_definition) == 0x2C);

struct s_runtime_core_widget_definition
{
	// e_core_widget_definition_flags
	// override: e_bitmap_widget_definition_flags
	// override: e_group_widget_definition_flags
	// override: e_list_widget_definition_flags
	// override: e_model_widget_definition_flags
	// override: e_screen_widget_definition_flags
	// override: e_text_widget_definition_flags
	dword_flags flags;

	// identification only
	c_string_id name;

	// scaled positioning & scaled anchoring
	c_enum<e_widget_positioning, long, _widget_positioning_unused, k_widget_positioning_count> scaled_positioning;
	long render_depth_bias;

	// bounds 720p & bounds 16x9 (1152x640)
	// bounds 480i & bounds 4x3 (640x480)
	real_rectangle2d ui_bounds[2];

	long animation_collection_index;
};
static_assert(sizeof(s_runtime_core_widget_definition) == 0x34);

enum e_gui_widget_type
{
	_gui_widget_type_text = 0,
	_gui_widget_type_bitmap,
	_gui_widget_type_model,
	_gui_widget_type_group,
	_gui_widget_type_button_key,
	_gui_widget_type_list_item,
	_gui_widget_type_slider,
	_gui_widget_type_list,
	_gui_widget_type_screen,

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
struct c_gui_screen_widget;

struct gui_real_rectangle2d
{
	real x0_0;
	real y0_0;
	real x0_1;
	real y0_1;
	real x1_0;
	real y0_2;
	real x1_1;
	real y1_0;
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
			bool(__thiscall* transitioning_in)(c_gui_widget*);
			bool(__thiscall* transitioning_out)(c_gui_widget*);
			void(__thiscall* assemble_render_data)(c_gui_widget*, s_gui_widget_render_data*, rectangle2d const*, e_controller_index, long, bool, bool, bool);
			bool(__thiscall* handle_widget_back_out)(c_gui_widget*);
			bool(__thiscall* handle_widget_selected)(c_gui_widget*);
			bool(__thiscall* handle_tab)(c_gui_widget*, c_controller_input_message const*);
			bool(__thiscall* handle_alt_tab)(c_gui_widget*, c_controller_input_message const*);
			void* __func34;
			bool(__thiscall* handle_controller_input_message)(c_gui_widget*, c_controller_input_message const*);
			bool(__thiscall* get_string_by_string_id)(c_gui_widget*, long, c_static_wchar_string<1024>*);
		};

		void* __funcs[64];
	}* __vftable;

	long get_ambient_state();

public:
	void* destructor(dword a1);
	void initialize();
	void post_initialize();
	void dispose();
	long get_name();
	long get_element_handle();
	long get_datasource_index();
	bool get_enabled();
	void set_enabled(bool a1);
	s_runtime_core_widget_definition* get_core_definition();
	real_rectangle2d* get_current_bounds(real_rectangle2d* unanimated_bounds);
	real_rectangle2d* get_container_current_bounds(real_rectangle2d* unanimated_bounds);
	bool should_render(bool* add_to_render_list);
	real_argb_color* get_cumulative_color_tint(real_argb_color* tint);
	bool can_receive_focus();
	bool within_focus_chain();
	c_gui_text_widget* create_text_widget(s_runtime_text_widget_definition const* definition);
	c_gui_bitmap_widget* create_bitmap_widget(s_runtime_bitmap_widget_definition const* definition);
	c_gui_group_widget* create_group_widget(s_group_widget_definition const* definition);
	c_gui_button_key_widget* create_button_key_widget(s_button_key_definition const* definition);
	c_gui_list_widget* create_list_widget(s_list_widget_block const* definition);
	c_gui_list_item_widget* create_list_item_widget(s_list_item_widget_block const* definition);
	c_gui_model_widget* create_model_widget(s_model_widget_block const* definition);
	void update(dword current_milliseconds);
	void update_render_state(dword current_milliseconds);
	void set_animated_state_baseline(s_animation_transform* transform);
	bool transitioning_in();
	bool transitioning_out();
	void assemble_render_data(s_gui_widget_render_data* render_data, rectangle2d const* window_bounds, e_controller_index controller_index, long projected_bounds, bool offset, bool scale_about_local_point, bool rotate_about_local_point);
	bool handle_widget_back_out();
	bool handle_widget_selected();
	bool handle_tab(c_controller_input_message const* message);
	bool handle_alt_tab(c_controller_input_message const* message);
	bool handle_controller_input_message(c_controller_input_message const* message);
	bool get_string_by_string_id(long string_identifier, c_static_wchar_string<1024>* buffer);
	c_gui_bitmap_widget* get_next_bitmap_widget();
	c_gui_list_item_widget* get_next_list_item_widget(bool only_consider_valid_items);
	c_gui_list_widget* get_next_list_widget();
	c_gui_model_widget* get_next_model_widget();
	c_gui_text_widget* get_next_text_widget();
	c_gui_widget* get_next_widget_of_type(e_gui_widget_type type);
	c_gui_group_widget* get_parent_group();
	c_gui_list_widget* get_parent_list();
	c_gui_list_item_widget* get_parent_list_item();
	c_gui_screen_widget* get_parent_screen();
	void set_visible(bool value);
	c_gui_widget* get_child_widget(e_gui_widget_type type, long name);
	c_gui_bitmap_widget* get_child_bitmap_widget(long name);
	c_gui_group_widget* get_child_group_widget(long name);
	c_gui_list_item_widget* get_child_list_item_widget(long name);
	c_gui_list_widget* get_child_list_widget(long name);
	c_gui_model_widget* get_child_model_widget(long name);
	c_gui_text_widget* get_child_text_widget(long name);
	c_gui_widget* get_first_child_widget_by_type(e_gui_widget_type type);

	e_controller_index get_any_responding_controller() const;
	void __thiscall get_unprojected_bounds(gui_real_rectangle2d* unprojected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation);

//protected:
	long __unknown4;
	c_enum<e_gui_widget_type, long, _gui_widget_type_text, k_gui_widget_type_count> m_type;

	bool m_visible;
	bool m_enabled;
	bool m_needs_disposal;
	bool m_use_alternate_ambient_state;

	c_robust_pointer<c_gui_widget> m_parent;
	c_robust_pointer<c_gui_widget> m_children;
	c_robust_pointer<c_gui_widget> m_next;
	c_robust_pointer<c_gui_widget> m_previous;

	//c_gui_widget* m_parent;
	//c_gui_widget* m_children;
	//c_gui_widget* m_next;
	//c_gui_widget* m_previous;

	c_string_id m_name;
	real __unknown44;
	dword_flags m_flags;
	long __unknown4C;
	s_animation_transform m_animated_state;
	long m_last_animated_milliseconds;
};
static_assert(sizeof(c_gui_widget) == 0xDC);

