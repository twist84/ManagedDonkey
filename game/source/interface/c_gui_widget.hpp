#pragma once

#include "cseries/cseries.hpp"
#include "cseries/ecc.hpp"
#include "interface/gui_animation.hpp"
#include "tag_files/tag_groups.hpp"
#include "text/unicode.hpp"

class c_controller_input_message;
class c_gui_bitmap_widget;
class c_gui_button_key_widget;
class c_gui_group_widget;
class c_gui_list_item_widget;
class c_gui_list_widget;
class c_gui_model_widget;
class c_gui_screen_widget;
class c_gui_text_widget;
struct s_animation_transform;
struct s_button_key_definition;
struct s_bitmap_widget_block;
struct s_group_widget_definition;
struct s_list_item_widget_block;
struct s_list_widget_block;
struct s_model_widget_block;
struct s_runtime_core_widget_definition;
struct s_runtime_text_widget_definition;
struct s_runtime_bitmap_widget_definition;
struct s_text_widget_block;
struct s_widget_animation_definition;

struct s_gui_skin_definition
{
	int32 unused_flags;
	c_typed_tag_block<s_text_widget_block> text_blocks;
	c_typed_tag_block<s_bitmap_widget_block> bitmap_blocks;
};
static_assert(sizeof(s_gui_skin_definition) == 0x1C);

struct s_core_widget_definition
{
	// e_core_widget_definition_flags
	// override: e_bitmap_widget_definition_flags
	// override: e_group_widget_definition_flags
	// override: e_list_widget_definition_flags
	// override: e_model_widget_definition_flags
	// override: e_screen_widget_definition_flags
	// override: e_text_widget_definition_flags
	int32 flags;

	// identification only
	string_id widget_identifier;

	// scaled positioning & scaled anchoring
	int16 scaled_anchoring;
	int16 render_depth_bias;

	// bounds 720p & bounds 16x9 (1152x640)
	// bounds 480i & bounds 4x3 (640x480)
	rectangle2d bounds[2];

	c_typed_tag_reference<GUI_WIDGET_ANIMATION_COLLECTION_DEFINITION_TAG, INVALID_TAG> animation_collection_reference;
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
	int32 flags;

	// identification only
	string_id widget_identifier;

	// scaled positioning & scaled anchoring
	e_widget_anchoring scaled_anchoring;
	int32 render_depth_bias;

	// bounds 720p & bounds 16x9 (1152x640)
	// bounds 480i & bounds 4x3 (640x480)
	real_rectangle2d runtime_bounds;
	real_rectangle2d authored_bounds;

	int32 animation_collection_reference_index;
};
static_assert(sizeof(s_runtime_core_widget_definition) == 0x34);

struct s_gui_widget_render_data
{
	enum e_widget_render_data_flags
	{
		_render_as_custom_storage_bitmap_bit = 0,
		_render_texture_camera_bit,
		_render_blurred_back_buffer_bit,
		_render_as_player_emblem_bit,
		_emblem_info_valid_bit,
		_render_in_screenshot_bit,
		_render_as_au2_playlist_rating,
		_render_as_au2_rank_tray,

		k_number_of_bitmap_render_data_flags
	};

	e_gui_widget_type type;
	c_flags<e_widget_render_data_flags, int32, k_number_of_bitmap_render_data_flags> flags;
	gui_real_rectangle2d projected_bounds;
	e_controller_index local_controller_index;

#pragma pack(push, 4)
	// >= profile builds
	string_id name;

	// >= play builds
	uns32 debug_color;
	uns64 animation_state_flags;
	real_point3d rotation_origin_with_depth;
	bool render_debug_name;
	bool render_debug_animation_state;
	bool render_debug_bounds;
	bool render_debug_rotation_origin;
#pragma pack(pop)
};
//static_assert(sizeof(s_gui_widget_render_data) == 0x2C); // == release
//static_assert(sizeof(s_gui_widget_render_data) == 0x30); // == profile
//static_assert(sizeof(s_gui_widget_render_data) == 0x50); // >= play

class c_gui_widget
{
public:
	enum e_gui_widget_flags
	{
		_responds_to_controller0_bit,
		_responds_to_controller1_bit,
		_responds_to_controller2_bit,
		_responds_to_controller3_bit,
		_initializing_bit,

		_debug_name_bit,
		_debug_animation_state_bit,
		_debug_bounds_bit,
		_debug_rotation_origin_bit,

		k_number_of_widget_flags,
		k_controller_mask = MASK(4),
	};

public:
	c_gui_bitmap_widget* __thiscall create_bitmap_widget_(const s_runtime_bitmap_widget_definition* definition);
	c_gui_button_key_widget* __thiscall create_button_key_widget_(const s_button_key_definition* definition);
	c_gui_group_widget* __thiscall create_group_widget_(const s_group_widget_definition* definition);
	c_gui_list_item_widget* __thiscall create_list_item_widget_(const s_list_item_widget_block* definition);
	c_gui_list_widget* __thiscall create_list_widget_(const s_list_widget_block* definition);
	c_gui_model_widget* __thiscall create_model_widget_(const s_model_widget_block* definition);
	c_gui_text_widget* __thiscall create_text_widget_(const s_runtime_text_widget_definition* definition);

	gui_real_rectangle2d* __thiscall get_unprojected_bounds_(gui_real_rectangle2d* unprojected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation);

	bool __thiscall handle_alt_stick_(const c_controller_input_message* message);
	bool __thiscall handle_alt_tab_(const c_controller_input_message* message);
	bool __thiscall handle_controller_input_message_(const c_controller_input_message* message);
	bool __thiscall handle_tab_(const c_controller_input_message* message);

protected:
	virtual e_animation_state get_ambient_state();

public:
	virtual ~c_gui_widget();
	virtual void initialize();
	virtual void post_initialize();
	virtual void dispose();
	virtual int32 get_name() const;
	virtual int32 get_element_handle();
	virtual int32 get_datasource_index();
	virtual bool get_enabled();
	virtual void set_enabled(bool);
	virtual s_runtime_core_widget_definition* get_core_definition() = 0;
	virtual real_rectangle2d* get_current_bounds(real_rectangle2d* unanimated_bounds);
	virtual real_rectangle2d* get_container_current_bounds(real_rectangle2d* unanimated_bounds);
	virtual bool should_render(bool* add_to_render_list);
	virtual real_argb_color* get_cumulative_color_tint(real_argb_color* tint);
	virtual bool can_receive_focus();
	virtual bool within_focus_chain();
	virtual c_gui_text_widget* create_text_widget(const s_runtime_text_widget_definition* definition);
	virtual c_gui_bitmap_widget* create_bitmap_widget(const s_runtime_bitmap_widget_definition* definition);
	virtual c_gui_group_widget* create_group_widget(const s_group_widget_definition* definition);
	virtual c_gui_button_key_widget* create_button_key_widget(const s_button_key_definition* definition);
	virtual c_gui_list_widget* create_list_widget(const s_list_widget_block* definition);
	virtual c_gui_list_item_widget* create_list_item_widget(const s_list_item_widget_block* definition);
	virtual c_gui_model_widget* create_model_widget(const s_model_widget_block* definition);
	virtual void update(uns32 current_milliseconds);
	virtual void update_render_state(uns32 current_milliseconds);
	virtual void set_animated_state_baseline(s_animation_transform* transform);
	virtual bool transitioning_in();
	virtual bool transitioning_out();
	virtual void assemble_render_data(s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation);
	virtual bool handle_widget_back_out();
	virtual bool handle_widget_selected();
	virtual bool handle_tab(const c_controller_input_message* message);
	virtual bool handle_alt_stick(const c_controller_input_message* message);
	virtual bool handle_alt_tab(const c_controller_input_message* message);
	virtual bool handle_controller_input_message(const c_controller_input_message* message);
	virtual bool get_string_by_string_id(int32 string_identifier, c_static_wchar_string<1024>* buffer);

public:
	c_gui_widget(e_gui_widget_type type);
	static void __cdecl add_definition_fields(e_gui_widget_type type, const s_core_widget_definition* source_definition, s_runtime_core_widget_definition* dest_definition, const real_rectangle2d* unanimated_bounds, bool was_templated);

private:
	void animate(uns32 current_milliseconds);

protected:
	void animate_recursively(uns32 current_milliseconds);

public:
	void calculate_animation_transform(e_animation_state animation_state, int32 start_time_milliseconds, int32 current_time_milliseconds, const s_widget_animation_definition* animation, s_animation_transform* transform, const real_vector2d* aspect_ratio_scale, bool initialize, bool combinative, bool* finished);
	bool const can_all_children_be_disposed();
	bool const can_be_disposed();
	bool contains_point(const point2d* point);
	bool controller_can_drive(e_controller_index controller_index);
	c_gui_bitmap_widget* create_and_add_child_bitmap_widget(const s_bitmap_widget_block* bitmap_widget_block);
	void create_and_add_child_list_item_widgets(const s_tag_block* list_items_block, int32 gui_skin_tag_index);
	void delete_all_children();
	e_controller_index get_arbitrary_responding_controller() const;
	real_rectangle2d* get_authored_bounds(real_rectangle2d* unanimated_bounds);
	c_gui_bitmap_widget* get_child_bitmap_widget(int32 name);
	c_gui_group_widget* get_child_group_widget(int32 name);
	c_gui_list_item_widget* get_child_list_item_widget(int32 name);
	c_gui_list_widget* get_child_list_widget(int32 name);
	c_gui_model_widget* get_child_model_widget(int32 name);
	c_gui_text_widget* get_child_text_widget(int32 name);
	c_gui_widget* get_child_widget(e_gui_widget_type type, int32 name);
	c_gui_widget* get_children();
	int32 get_controller_mask() const;
	const real_argb_color* get_debug_color();
	c_gui_widget* get_deepest_widget_that_can_receive_focus();
	int32 get_definition_index();
	e_controller_index get_driving_controller() const;
	int32 get_element_index();
	c_gui_widget* get_first_child_widget_by_type(e_gui_widget_type type);
	c_gui_widget* get_last_child_widget_by_type(e_gui_widget_type type);
	c_gui_widget* get_next();
	void get_mouse_region(rectangle2d* mouse_region_out);
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
	gui_real_rectangle2d* get_projected_bounds(const rectangle2d* window_bounds, gui_real_rectangle2d* projected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation);
	int16 get_render_depth_bias();
	bool get_render_in_screenshot();
	real32 get_rendered_depth();
	e_controller_index get_single_responding_controller() const;
	e_gui_widget_type get_type() const;
	gui_real_rectangle2d* get_unprojected_bounds(gui_real_rectangle2d* unprojected_bounds, bool apply_translation, bool apply_scale, bool apply_rotation);
	bool get_visible() const;
	bool is_animation_active(e_animation_state animation_state);
	bool leaf_node_of_widget(c_gui_widget* branch_widget);
	void modulate_tint_color(const real_argb_color* modulation);
	void remove_child_widget(c_gui_widget* child);
	static void render(int32 user_index, const s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, bool is_screenshot);
	void set_child_bitmap_sprite_frame(int32 widget_name, int32 sprite_frame_index);
	void set_child_enabled(e_gui_widget_type widget_type, int32 widget_name, bool enabled);
	void set_child_use_alternate_ambient_state(e_gui_widget_type widget_type, int32 widget_name, bool value);
	void set_child_visible(e_gui_widget_type widget_type, int32 widget_name, bool visible);
	void set_children(c_gui_widget* children);
	void set_debug_animation_state(bool activate);
	void set_debug_bounds(bool activate);
	void set_debug_name(bool activate);
	void set_debug_rotation_origin(bool activate);
	void set_driving_controller(e_controller_index controller_index);
	void set_full_animation_state(const s_animation_transform* transform, bool recursive);
	void set_next(c_gui_widget* next);
	void set_parent(c_gui_widget* parent);
	void set_previous(c_gui_widget* previous);
	void set_tint_color_direct(const real_argb_color* color);
	void set_use_alternate_ambient_state(bool value);
	void set_visible(bool value);
	void start_animation(e_animation_state animation_state, bool recursive);
	void start_animation_at_time(e_animation_state animation_state, uns32 animation_start_time, bool recursive);
	void stomp_bounds(const real_rectangle2d* new_bounds);
	void update_animation(uns32 current_milliseconds);
	bool verify_animation_period(e_animation_state animation_state, int32 period);
	static const char* __cdecl widget_name_to_string(int32 name);

//protected:
	int32 __unknown4;
	e_gui_widget_type m_type;
	bool m_visible;
	bool m_enabled;
	bool m_needs_disposal;
	bool m_use_alternate_ambient_state;
	c_robust_pointer<c_gui_widget> m_parent;
	c_robust_pointer<c_gui_widget> m_children;
	c_robust_pointer<c_gui_widget> m_next;
	c_robust_pointer<c_gui_widget> m_previous;
	string_id m_name;
	real32 __unknown44;
	uns32 m_flags;
	int32 __unknown4C;
	s_animation_transform m_animated_state;
	int32 m_last_animated_milliseconds;
	int32 __unknownDC;
};
static_assert(sizeof(c_gui_widget) == 0xE0);
static_assert(0x04 == OFFSETOF(c_gui_widget, __unknown4));
static_assert(0x08 == OFFSETOF(c_gui_widget, m_type));
static_assert(0x0C == OFFSETOF(c_gui_widget, m_visible));
static_assert(0x0D == OFFSETOF(c_gui_widget, m_enabled));
static_assert(0x0E == OFFSETOF(c_gui_widget, m_needs_disposal));
static_assert(0x0F == OFFSETOF(c_gui_widget, m_use_alternate_ambient_state));
static_assert(0x10 == OFFSETOF(c_gui_widget, m_parent));
static_assert(0x1C == OFFSETOF(c_gui_widget, m_children));
static_assert(0x28 == OFFSETOF(c_gui_widget, m_next));
static_assert(0x34 == OFFSETOF(c_gui_widget, m_previous));
static_assert(0x40 == OFFSETOF(c_gui_widget, m_name));
static_assert(0x44 == OFFSETOF(c_gui_widget, __unknown44));
static_assert(0x48 == OFFSETOF(c_gui_widget, m_flags));
static_assert(0x4C == OFFSETOF(c_gui_widget, __unknown4C));
static_assert(0x50 == OFFSETOF(c_gui_widget, m_animated_state));
static_assert(0xD8 == OFFSETOF(c_gui_widget, m_last_animated_milliseconds));
static_assert(0xDC == OFFSETOF(c_gui_widget, __unknownDC));

extern bool gui_debug_text_bounds_global;
extern bool gui_debug_bitmap_bounds_global;
extern bool gui_debug_model_bounds_global;
extern bool gui_debug_list_item_bounds_global;
extern bool gui_debug_list_bounds_global;
extern bool gui_debug_group_bounds_global;
extern bool gui_debug_screen_bounds_global;

extern const char* __cdecl gui_widget_type_to_string(e_gui_widget_type type);
