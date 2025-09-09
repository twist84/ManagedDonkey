#include "interface/c_gui_model_widget.hpp"

/*
#include "cache/cache_files.hpp"
#include "interface/c_controller.hpp"
#include "interface/c_gui_bitmap_widget.hpp"
#include "interface/gui_screens/start_menu/start_menu_pane_screen_widget.hpp"
#include "render/views/render_player_view.hpp"

c_gui_model_widget::c_gui_model_widget() :
	c_gui_widget(_gui_model),

	// H3
	m_definition(),
	m_player_appearance(),
	m_team_index(),

	// ODST/HO
	__point1C08(),
	__point1C14(),
	__point1C20(),
	__unknown1C2C(),
	__pad1C2D(),
	__point1C30(),
	__rotation1C3C(),
	__unknown1C44(),

	// H3
	m_yaw_degrees(),
	m_zoom_factor(),

	// ODST/HO
	m_object_index(),
	m_render_window(),
	m_texture_camera_slice_name(),
	m_texture_camera_near_clip_distance(),
	__time1C60(),
	m_texture_camera_unit_available(),
	m_texture_camera_unit_index(),
	__data1C6C()
{
	//DECLFUNC(0x00B19B00, void, __thiscall, c_gui_model_widget*)(this);
}

//.text:00B19EB0 ; public: virtual c_gui_model_widget::~c_gui_model_widget()
//.text:00B19EC0 ; public: virtual void* c_gui_model_widget::`vector deleting destructor'(unsigned int)

void c_gui_model_widget::add_definition_fields(const s_model_widget_definition* definition, bool was_templated)
{
	INVOKE_CLASS_MEMBER(0x00B19EF0, c_gui_model_widget, add_definition_fields, definition, was_templated);
}

void c_gui_model_widget::assemble_render_data(s_gui_widget_render_data* render_data, const rectangle2d* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	INVOKE_CLASS_MEMBER(0x00B1A010, c_gui_model_widget, assemble_render_data, render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);

	c_gui_widget::assemble_render_data(render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);

	//int32 player_appearance_biped = user_interface_get_player_appearance_biped(&m_player_appearance);
	//if (player_appearance_biped != NONE)
	//{
	//	// $IMPLEMENT
	//}
}

//.text:00B1A450 ; 
//.text:00B1A4D0 ; 
//.text:00B1A750 ; 
//.text:00B1AE40 ; 

//bool c_gui_model_widget::can_receive_focus()
//{
//	return INVOKE_CLASS_MEMBER(0x00B1AF20, c_gui_model_widget, can_receive_focus);
//}

//void c_gui_model_widget::dispose()
//{
//	INVOKE_CLASS_MEMBER(0x00B1AF60, c_gui_model_widget, dispose);
//}

//.text:00B1AFA0 ; 
//.text:00B1AFC0 ; 
//.text:00B1AFD0 ; 
//.text:00B1AFF0 ; 
//.text:00B1B010 ; 
//.text:00B1B030 ; 
//.text:00B1B060 ; 
//.text:00B1B080 ; 
//.text:00B1B0A0 ; 
//.text:00B1B0C0 ; 
//.text:00B1B0E0 ; 
//.text:00B1B0F0 ; 
//.text:00B1B100 ; 
//.text:00B1B130 ; public: const s_model_widget_camera_settings_block* c_gui_model_widget::get_camera_settings_block() const
//.text:00B1B1D0 ; 
//.text:00B1B200 ; 
//.text:00B1B290 ; 
//.text:00B1B3B0 ; 
//.text:00B1B3C0 ; 

s_runtime_core_widget_definition* c_gui_model_widget::get_core_definition()
{
	return INVOKE_CLASS_MEMBER(0x00B1B3D0, c_gui_model_widget, get_core_definition);

	//return &m_definition;
}

const s_player_appearance* c_gui_model_widget::get_player_appearance() const
{
	return INVOKE_CLASS_MEMBER(0x00B1B3E0, c_gui_model_widget, get_player_appearance);

	//return &m_player_appearance;
}

//.text:00B1B3F0 ; 
//.text:00B1B400 ; 

bool c_gui_model_widget::handle_camera_input(real32 a1, real32 a2, real32 a3, real32 a4, real32 yaw_delta, real32 zoom_delta)
{
	return INVOKE_CLASS_MEMBER(0x00B1B440, c_gui_model_widget, handle_camera_input, a1, a2, a3, a4, yaw_delta, zoom_delta);
}

//.text:00B1B9F0 ; 

bool c_gui_model_widget::handle_controller_input_message(const c_controller_input_message* message)
{
	//return INVOKE_CLASS_MEMBER(0x00B1BC30, c_gui_model_widget, handle_controller_input_message, message);

	return false;
}

void c_gui_model_widget::initialize(const s_model_widget_block* template_and_override_block)
{
	//INVOKE_CLASS_MEMBER(0x00B1BC50, c_gui_model_widget, initialize, template_and_override_block);

	if (template_and_override_block)
	{
		bool was_templated = false;
		s_model_widget_definition* widget_template = NULL;
		if (template_and_override_block->widget_template_reference.index != NONE)
		{
			widget_template = TAG_GET(NONE, s_model_widget_definition, template_and_override_block->widget_template_reference.index);
			c_gui_model_widget::add_definition_fields(widget_template, false);
			was_templated = true;
		}
		c_gui_model_widget::add_definition_fields(&template_and_override_block->override_definition, was_templated);
	}

	m_name = m_definition.widget_identifier;
	c_gui_widget::initialize();
	render_texture_camera_on();
}

//.text:00B1BCC0 ; 
//.text:00B1BCF0 ; 
//.text:00B1BD30 ; 
//.text:00B1BD50 ; 

void c_gui_model_widget::post_initialize()
{
	//INVOKE_CLASS_MEMBER(0x00B1BE30, c_gui_model_widget, post_initialize);

	c_gui_widget::post_initialize();
	c_gui_model_widget::reset_player_appearance();

	real32 yaw_degrees = 0.0f;
	real32 zoom_factor = 0.0f;

	c_gui_screen_widget* parent_screen = c_gui_widget::get_parent_screen();
	if (parent_screen && gui_screen_widget_is_start_menu_pane_screen(parent_screen))
	{
		c_start_menu_pane_screen_widget* start_menu_pane_screen = (c_start_menu_pane_screen_widget*)parent_screen;
		zoom_factor = start_menu_pane_screen->m_appearance_camera_zoom;
		yaw_degrees = start_menu_pane_screen->m_appearance_camera_yaw;
	}

	m_zoom_factor = zoom_factor;
	m_yaw_degrees = yaw_degrees;
	c_gui_model_widget::handle_camera_input(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

//.text:00B1BF70 ; 

void __cdecl c_gui_model_widget::render(const s_gui_model_widget_render_data* render_data, const rectangle2d* window_bounds)
{
	//INVOKE(0x00B1BFA0, c_gui_model_widget::render, render_data, window_bounds);

	ASSERT(render_data != NULL);
	s_gui_bitmap_widget_render_data bitmap_render_data{};
	csmemcpy(&bitmap_render_data, render_data, sizeof(s_gui_widget_render_data));
	bitmap_render_data.type = _gui_bitmap;
	bitmap_render_data.flags.set(s_gui_widget_render_data::_render_texture_camera_bit, true);
	bitmap_render_data.argb_tint = render_data->argb_tint;
	set_real_point2d(&bitmap_render_data.texture_uv_offset, 0.0f, 0.0f);
	bitmap_render_data.frame_buffer_blend_function = 3;
	bitmap_render_data.explicit_shader_index = NONE;
	render_bitmap(&bitmap_render_data, window_bounds);
}

void c_gui_model_widget::reset_player_appearance()
{
	//INVOKE_CLASS_MEMBER(0x00B1C260, c_gui_model_widget, reset_player_appearance);

	c_gui_screen_widget* parent_screen = c_gui_widget::get_parent_screen();
	if (!parent_screen)
	{
		return;
	}

	e_controller_index controller_index = parent_screen->get_arbitrary_responding_controller();
	if (!VALID_CONTROLLER(controller_index))
	{
		return;
	}

	c_controller_interface* controller = controller_get(controller_index);
	c_player_profile_interface* player_profile = controller->get_player_profile_interface();

	s_player_appearance appearance{};
	player_profile->get_player_appearance(&appearance);
	c_gui_model_widget::set_player_appearance(&appearance);
}

//.text:00B1C320 ; 
//.text:00B1C440 ; 
//.text:00B1C460 ; 
//.text:00B1C480 ; 
//.text:00B1C4A0 ; 
//.text:00B1C4E0 ; 

void c_gui_model_widget::set_player_appearance(const s_player_appearance* appearance)
{
	//INVOKE_CLASS_MEMBER(0x00B1C570, c_gui_model_widget, set_player_appearance, appearance);
	//DECLFUNC(0x00B1C570, void, __thiscall, c_gui_model_widget*, const s_player_appearance*)(this, appearance);

	c_gui_model_widget::set_player_appearance(appearance, _game_team_none);
}

void c_gui_model_widget::set_player_appearance(const s_player_appearance* appearance, e_game_team team_index)
{
	//INVOKE_CLASS_MEMBER(0x00B1C590, c_gui_model_widget, set_player_appearance, appearance, team_index);
	//DECLFUNC(0x00B1C590, void, __thiscall, c_gui_model_widget*, const s_player_appearance*, e_game_team)(this, appearance, team_index);

	if (csmemcmp(&m_player_appearance, appearance, sizeof(s_player_appearance)) != 0 || m_team_index != team_index)
	{
		m_player_appearance = *appearance;
		m_team_index = team_index;
		//user_interface_update_main_menu_character_model(appearance);
	}
}

//.text:00B1C610 ; 
//.text:00B1C7E0 ; 
//.text:00B1C890 ; 

//void c_gui_model_widget::update(uns32 current_milliseconds)
//{
//	INVOKE_CLASS_MEMBER(0x00B1C8E0, c_gui_model_widget, update, current_milliseconds);
//}

//.text:00B1CBA0 ; 
*/

 