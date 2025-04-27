#include "interface/c_gui_bitmap_widget.hpp"

//.text:00B16670 ; public: __cdecl c_gui_bitmap_widget::c_gui_bitmap_widget()

//c_gui_bitmap_widget::~c_gui_bitmap_widget()
//{
//	DECLFUNC(0x00B166B0, void, __thiscall, c_gui_bitmap_widget*)(this);
//}

//.text:00B166E0 ; public: static void __cdecl c_gui_bitmap_widget::add_definition_fields(s_bitmap_widget_definition const*, s_runtime_bitmap_widget_definition*, real_rectangle2d*, bool)
//.text:00B16760 ; public: static void __cdecl c_gui_bitmap_widget::assemble_definition(s_bitmap_widget_block const*, s_runtime_bitmap_widget_definition*, real_rectangle2d*)
//.text:00B167B0 ; public: virtual void __cdecl c_gui_bitmap_widget::assemble_render_data(s_gui_widget_render_data*, rectangle2d const*, e_controller_index, int32, bool, bool, bool)
//.text:00B168F0 ; public: virtual bool __cdecl c_gui_bitmap_widget::can_receive_focus()

//e_animation_state c_gui_bitmap_widget::get_ambient_state()
//{
//	return INVOKE_CLASS_MEMBER(0x00B16900, c_gui_bitmap_widget, get_ambient_state);
//}

//.text:00B16970 ; 
//.text:00B16A20 ; 
//.text:00B16AB0 ; public: virtual s_runtime_core_widget_definition* __cdecl c_gui_bitmap_widget::get_core_definition()

bitmap_data const* c_gui_bitmap_widget::get_current_bitmap() const
{
	//return INVOKE_CLASS_MEMBER((uint32)__vftable->__funcs[38], c_gui_bitmap_widget, get_current_bitmap);
	return INVOKE_CLASS_MEMBER(0x00B16AC0, c_gui_bitmap_widget, get_current_bitmap);
}

//.text:00B16B70 ; public: bitmap_data const* __cdecl c_gui_bitmap_widget::get_current_bitmap_hardware_format(c_rasterizer_texture_ref*) const
//.text:00B16C40 ; public: virtual real_rectangle2d* __cdecl c_gui_bitmap_widget::get_current_bounds(real_rectangle2d*)
//.text:00B16E10 ; 
//.text:00B16E40 ; 
//.text:00B16F00 ; 
//.text:00B16F20 ; 
//.text:00B16FC0 ; 
//.text:00B16FE0 ; public: virtual void __cdecl c_gui_bitmap_widget::initialize(s_bitmap_widget_block const*)
//.text:00B170A0 ; 
//.text:00B170D0 ; 
//.text:00B170F0 ; void __cdecl render_bitmap(s_gui_bitmap_widget_render_data const*)
//.text:00B17640 ; public: bool __cdecl c_gui_bitmap_widget::renders_as_player_emblem() const
//.text:00B17650 ; public: virtual void __cdecl c_gui_bitmap_widget::set_animated_state_baseline(s_animation_transform*)

void c_gui_bitmap_widget::set_sprite_frame(int32 sprite_frame)
{
	m_sprite_frame = sprite_frame;
}

void c_gui_bitmap_widget::set_sprite_sequence(int32 sprite_sequence)
{
	m_sprite_sequence = sprite_sequence;
}

//.text:00B17690 ; public: virtual void __cdecl c_gui_bitmap_widget::update_render_state(uint32)
//.text:00B17810 ; public: virtual bool __cdecl c_gui_bitmap_widget::within_focus_chain()

