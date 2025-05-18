#include "interface/c_gui_text_widget.hpp"

#include "interface/c_gui_screen_widget.hpp"

//.text:00B18810 ; public: c_gui_text_widget::c_gui_text_widget()
//.text:00B18850 ; 

//.text:00B18870 ; public: virtual void * __cdecl c_gui_text_widget::`vector deleting destructor'(unsigned int)
c_gui_text_widget::~c_gui_text_widget()
{
	DECLFUNC(0x00B18870, void, __thiscall, c_gui_text_widget*)(this);
}

//.text:00B188A0 ; public: static void __cdecl c_gui_text_widget::add_definition_fields(s_text_widget_definition const*, s_runtime_text_widget_definition*, real_rectangle2d*, bool)
//.text:00B189C0 ; public: static void __cdecl c_gui_text_widget::assemble_definition(s_text_widget_block const*, s_runtime_text_widget_definition*, real_rectangle2d*)

void c_gui_text_widget::assemble_render_data(s_gui_widget_render_data* render_data, rectangle2d const* window_bounds, e_controller_index local_controller_index, bool apply_translation, bool apply_scale, bool apply_rotation)
{
	INVOKE_CLASS_MEMBER(0x00B18A10, c_gui_text_widget, assemble_render_data, render_data, window_bounds, local_controller_index, apply_translation, apply_scale, apply_rotation);
}

//.text:00B18C60 ; private: void c_gui_text_widget::compute_text_bounds(rectangle2d const*, real_point2d, real32, real32, real32, real32, real32, real_point2d*, real_rectangle2d*, real_rectangle2d*, real32*)

s_runtime_core_widget_definition* c_gui_text_widget::get_core_definition()
{
	return INVOKE_CLASS_MEMBER(0x00B18EB0, c_gui_text_widget, get_core_definition);
}

real_argb_color* c_gui_text_widget::get_cumulative_color_tint(real_argb_color* tint)
{
	return INVOKE_CLASS_MEMBER(0x00B18EC0, c_gui_text_widget, get_cumulative_color_tint, tint);
}

//.text:00B18F20 ; 
//.text:00B18F40 ; 
//.text:00B18F50 ; public: void c_gui_text_widget::get_tab_stops(int16*, int16*)
//.text:00B190C0 ; 

wchar_t const* c_gui_text_widget::get_text()
{
	return INVOKE_CLASS_MEMBER(0x00B18F90, c_gui_text_widget, get_text);
}

void c_gui_text_widget::initialize(s_text_widget_block const* template_and_override_block)
{
	INVOKE_CLASS_MEMBER(0x00B190D0, c_gui_text_widget, initialize, template_and_override_block);
}

//.text:00B19180 ; 

void c_gui_text_widget::set_animated_state_baseline(s_animation_transform* transform)
{
	INVOKE_CLASS_MEMBER(0x00B191B0, c_gui_text_widget, set_animated_state_baseline, transform);
}

//.text:00B191F0 ; 
//.text:00B19200 ; 
//.text:00B19220 ; private: bool c_gui_text_widget::set_string_from_string_id(c_static_wchar_string<1024>*, c_gui_widget*, int32, bool)
//.text:00B192A0 ; public: void c_gui_text_widget::set_tab_stops(int16 const*, int32)
//.text:00B192D0 ; 

void c_gui_text_widget::set_text(wchar_t const* text)
{
	INVOKE_CLASS_MEMBER(0x00B19300, c_gui_text_widget, set_text, text);
}

void c_gui_text_widget::set_text_from_string_id(c_gui_screen_widget* screen, int32 id)
{
	INVOKE_CLASS_MEMBER(0x00B19340, c_gui_text_widget, set_text_from_string_id, screen, id);
}

//.text:00B19390 ; private: void c_gui_text_widget::set_text_internal(wchar_t const*, c_gui_text_widget::s_text_source_data const*, bool)
//.text:00B194B0 ; 

void c_gui_text_widget::update(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B194D0, c_gui_text_widget, update, current_milliseconds);
}

void c_gui_text_widget::update_render_state(uns32 current_milliseconds)
{
	INVOKE_CLASS_MEMBER(0x00B19A90, c_gui_text_widget, update_render_state, current_milliseconds);
}

bool c_gui_text_widget::within_focus_chain()
{
	return INVOKE_CLASS_MEMBER(0x00B19AE0, c_gui_text_widget, within_focus_chain);
}

