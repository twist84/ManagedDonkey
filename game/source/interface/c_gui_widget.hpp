#pragma once

#include "cseries/cseries.hpp"
#include "interface/gui_animation.hpp"
#include "tag_files/tag_groups.hpp"
#include "text/unicode.hpp"

struct s_core_widget_definition
{
	// e_core_widget_definition_flags
	// override: e_bitmap_widget_definition_flags
	// override: e_group_widget_definition_flags
	// override: e_list_widget_definition_flags
	// override: e_model_widget_definition_flags
	// override: e_screen_widget_definition_flags
	// override: e_text_widget_definition_flags
	uint32 flags;

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
	uint32 flags;

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

enum e_animation_state;

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
struct s_widget_animation_definition;
struct s_group_widget_definition;
struct s_button_key_definition;
struct s_list_widget_block;
struct s_list_item_widget_block;
struct s_model_widget_block;
struct s_animation_transform;
struct s_gui_widget_render_data;
struct c_controller_input_message;
struct c_gui_screen_widget;

struct c_gui_widget
{
protected:
	//virtual e_animation_state get_ambient_state();

public:
	//virtual ~c_gui_widget();
	//virtual void initialize();
	//virtual void post_initialize();
	//virtual void dispose();
	//virtual long get_name() const;
	//virtual long get_element_handle();
	//virtual long get_datasource_index();
	//virtual bool get_enabled();
	//virtual void set_enabled(bool);
	//virtual s_runtime_core_widget_definition* get_core_definition() = 0;
	//virtual real_rectangle2d* get_current_bounds(real_rectangle2d* unanimated_bounds);
	//virtual real_rectangle2d* get_container_current_bounds(real_rectangle2d* unanimated_bounds);
	//virtual bool should_render(bool* add_to_render_list);
	//virtual real_argb_color* get_cumulative_color_tint(real_argb_color* tint);
	//virtual bool can_receive_focus();
	//virtual bool within_focus_chain();
	//virtual c_gui_text_widget* create_text_widget(s_runtime_text_widget_definition const* definition);
	//virtual c_gui_bitmap_widget* create_bitmap_widget(s_runtime_bitmap_widget_definition const* definition);
	//virtual c_gui_group_widget* create_group_widget(s_group_widget_definition const* definition);
	//virtual c_gui_button_key_widget* create_button_key_widget(s_button_key_definition const* definition);
	//virtual c_gui_list_widget* create_list_widget(s_list_widget_block const* definition);
	//virtual c_gui_list_item_widget* create_list_item_widget(s_list_item_widget_block const* definition);
	//virtual c_gui_model_widget* create_model_widget(s_model_widget_block const* definition);
	//virtual void update(uint32 current_milliseconds);
	//virtual void update_render_state(uint32 current_milliseconds);
	//virtual void set_animated_state_baseline(s_animation_transform* transform);
	//virtual bool transitioning_in();
	//virtual bool transitioning_out();
	//virtual void assemble_render_data(s_gui_widget_render_data* render_data, rectangle2d const* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation);
	//virtual bool handle_widget_back_out();
	//virtual bool handle_widget_selected();
	//virtual bool handle_tab(c_controller_input_message const* message);
	//virtual bool handle_alt_stick(c_controller_input_message const* message);
	//virtual bool handle_alt_tab(c_controller_input_message const* message);
	//virtual bool handle_controller_input_message(c_controller_input_message const* message);
	//virtual bool get_string_by_string_id(long string_identifier, c_static_wchar_string<1024>* buffer);

	// HACK: so we don't have to manually construct the class
	union
	{
		struct
		{
			long(__thiscall* get_ambient_state)(c_gui_widget*);
			void* (__thiscall* destructor)(c_gui_widget*, uint32);
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
			void(__thiscall* update)(c_gui_widget*, uint32);
			void(__thiscall* update_render_state)(c_gui_widget*, uint32);
			void(__thiscall* set_animated_state_baseline)(c_gui_widget*, s_animation_transform*);
			bool(__thiscall* transitioning_in)(c_gui_widget*);
			bool(__thiscall* transitioning_out)(c_gui_widget*);
			void(__thiscall* assemble_render_data)(c_gui_widget*, s_gui_widget_render_data*, rectangle2d const*, e_controller_index, bool, bool, bool);
			bool(__thiscall* handle_widget_back_out)(c_gui_widget*);
			bool(__thiscall* handle_widget_selected)(c_gui_widget*);
			bool(__thiscall* handle_tab)(c_gui_widget*, c_controller_input_message const*);
			bool(__thiscall* handle_alt_stick)(c_gui_widget*, c_controller_input_message const*);
			bool(__thiscall* handle_alt_tab)(c_gui_widget*, c_controller_input_message const*);
			bool(__thiscall* handle_controller_input_message)(c_gui_widget*, c_controller_input_message const*);
			bool(__thiscall* get_string_by_string_id)(c_gui_widget*, long, c_static_wchar_string<1024>*);
		};

		void* __funcs[64];
	}*__vftable;

protected:
	e_animation_state get_ambient_state();

public:
	~c_gui_widget();
	void initialize();
	void post_initialize();
	void dispose();
	long get_name() const;
	long get_element_handle();
	long get_datasource_index();
	bool get_enabled();
	void set_enabled(bool value);
	s_runtime_core_widget_definition* get_core_definition() { return __vftable->get_core_definition(this); };
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
	void update(uint32 current_milliseconds);
	void update_render_state(uint32 current_milliseconds);
	void set_animated_state_baseline(s_animation_transform* transform);
	bool transitioning_in();
	bool transitioning_out();
	void assemble_render_data(s_gui_widget_render_data* render_data, rectangle2d const* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation);
	bool handle_widget_back_out();
	bool handle_widget_selected();
	bool handle_tab(c_controller_input_message const* message);
	bool handle_alt_stick(c_controller_input_message const* message);
	bool handle_alt_tab(c_controller_input_message const* message);
	bool handle_controller_input_message(c_controller_input_message const* message);
	bool get_string_by_string_id(long string_identifier, c_static_wchar_string<1024>* buffer);

	void calculate_animation_transform(e_animation_state animation_state, long start_time_milliseconds, long current_time_milliseconds, s_widget_animation_definition const* animation, s_animation_transform* transform, real_vector2d const* aspect_ratio_scale, bool initialize, bool combinative, bool* finished);
	bool const can_all_children_be_disposed();
	bool const can_be_disposed();
	bool controller_can_drive(e_controller_index controller_index);
	void delete_all_children();
	e_controller_index get_arbitrary_responding_controller() const;
	real_rectangle2d* get_authored_bounds(real_rectangle2d* unanimated_bounds);
	c_gui_bitmap_widget* get_child_bitmap_widget(long name);
	c_gui_group_widget* get_child_group_widget(long name);
	c_gui_list_item_widget* get_child_list_item_widget(long name);
	c_gui_list_widget* get_child_list_widget(long name);
	c_gui_model_widget* get_child_model_widget(long name);
	c_gui_text_widget* get_child_text_widget(long name);
	c_gui_widget* get_child_widget(e_gui_widget_type type, long name);
	c_gui_widget* get_children();
	long get_controller_mask() const;
	real_argb_color const* get_debug_color();
	c_gui_widget* get_deepest_widget_that_can_receive_focus();
	long get_definition_index();
	e_controller_index get_driving_controller() const;
	long get_element_index();
	c_gui_widget* get_first_child_widget_by_type(e_gui_widget_type type);
	c_gui_widget* get_last_child_widget_by_type(e_gui_widget_type type);
	c_gui_widget* get_next();
	c_gui_bitmap_widget* get_next_bitmap_widget();
	c_gui_list_item_widget* get_next_list_item_widget(bool only_consider_valid_items);
	c_gui_list_widget* get_next_list_widget();
	c_gui_model_widget* get_next_model_widget();
	c_gui_text_widget* get_next_text_widget();
	c_gui_widget* get_next_widget_of_type(e_gui_widget_type type);
	c_gui_widget* get_parent();
	c_gui_group_widget* get_parent_group();
	c_gui_list_widget* get_parent_list();
	c_gui_list_item_widget* get_parent_list_item();
	c_gui_screen_widget* get_parent_screen();
	c_gui_widget* get_previous();
	c_gui_bitmap_widget* get_previous_bitmap_widget();
	c_gui_list_item_widget* get_previous_list_item_widget(bool only_consider_valid_items);
	c_gui_list_widget* get_previous_list_widget();
	c_gui_model_widget* get_previous_model_widget();
	c_gui_text_widget* get_previous_text_widget();
	c_gui_widget* get_previous_widget_of_type(e_gui_widget_type type);
	gui_real_rectangle2d* get_projected_bounds(rectangle2d const* window_bounds, gui_real_rectangle2d* projected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation);
	bool get_render_in_screenshot();
	e_controller_index get_single_responding_controller() const;
	void __thiscall get_unprojected_bounds(gui_real_rectangle2d* unprojected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation);
	bool is_animation_active(e_animation_state animation_state);
	bool leaf_node_of_widget(c_gui_widget* branch_widget);
	void modulate_tint_color(real_argb_color const* modulation);
	void remove_child_widget(c_gui_widget* child);
	static void render(long user_index, s_gui_widget_render_data const* render_data, rectangle2d const* window_bounds, bool is_screenshot);
	void set_child_bitmap_sprite_frame(long widget_name, long sprite_frame_index);
	void set_child_enabled(e_gui_widget_type widget_type, long widget_name, bool enabled);
	void set_child_use_alternate_ambient_state(e_gui_widget_type widget_type, long widget_name, bool value);
	void set_child_visible(e_gui_widget_type widget_type, long widget_name, bool visible);
	void set_children(c_gui_widget* children);
	void set_driving_controller(e_controller_index controller_index);
	void set_full_animation_state(s_animation_transform const* transform, bool recursive);
	void set_next(c_gui_widget* next);
	void set_parent(c_gui_widget* parent);
	void set_previous(c_gui_widget* previous);
	void set_tint_color_direct(real_argb_color const* color);
	void set_use_alternate_ambient_state(bool value);
	void set_visible(bool value);
	void start_animation(e_animation_state animation_state, bool recursive);
	void start_animation_at_time(e_animation_state animation_state, uint32 animation_start_time, bool recursive);
	void stomp_bounds(real_rectangle2d const* new_bounds);
	void update_animation(uint32 current_milliseconds);
	bool verify_animation_period(e_animation_state animation_state, long period);

//protected:
	long __unknown4;
	e_gui_widget_type m_type;

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
	real32 __unknown44;
	uint32 m_flags;
	long __unknown4C;
	s_animation_transform m_animated_state;
	long m_last_animated_milliseconds;
};
static_assert(sizeof(c_gui_widget) == 0xDC);

