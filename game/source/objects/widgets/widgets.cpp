#include "objects/widgets/widgets.hpp"

//.text:00B9F0A0 ; 
//.text:00B9F0C0 ; 
//.text:00B9F0D0 ; 
//.text:00B9F100 ; 
//.text:00B9F140 ; 
//.text:00B9F160 ; 
//.text:00B9F190 ; 
//.text:00B9F1C0 ; 
//.text:00B9F1D0 ; 
//.text:00B9F1E0 ; 
//.text:00B9F1F0 ; public: void __cdecl c_short_quaternion::get_real_matrix4x3(real_matrix4x3*) const
//.text:00B9F270 ; public: void __cdecl c_word_quantized_position::get_real_point3d(real_rectangle3d const*, real_point3d*) const
//.text:00B9F330 ; 
//.text:00B9F3A0 ; public: void __cdecl c_dword_unit_vector::get_real_vector3d(vector3d*) const
//.text:00B9F460 ; 
//.text:00B9F480 ; long __cdecl object_get_first_widget(long)
//.text:00B9F4B0 ; 
//.text:00B9F4F0 ; public: void __cdecl c_short_quaternion::set_real_matrix4x3(real_matrix4x3 const*)
//.text:00B9F570 ; 
//.text:00B9F670 ; public: void __cdecl c_short_quaternion::set_real_quaternion(real_quaternion*)
//.text:00B9F6E0 ; public: void __cdecl c_dword_unit_vector::set_real_vector3d(vector3d const*)
//.text:00B9F700 ; 
//.text:00B9F730 ; void __cdecl widget_delete_type_datum(long, long, bool)
//.text:00B9F830 ; long __cdecl widget_get_next_widget(long)
//.text:00B9F860 ; void __cdecl widget_get_type_data(long, long*, long*)
//.text:00B9F8A0 ; 
//.text:00B9F950 ; void __cdecl widget_transfer_objects(long, long, long)
//.text:00B9F9D0 ; 
//.text:00B9F9F0 ; 
//.text:00B9FA10 ; void __cdecl widget_type_new_on_object(long, short, long)
//.text:00B9FB20 ; void __cdecl widgets_delete(long)
//.text:00B9FBC0 ; 
//.text:00B9FBF0 ; 
//.text:00B9FC30 ; 
//.text:00B9FCF0 ; void __cdecl widgets_initialize()
//.text:00B9FD50 ; void __cdecl widgets_initialize_for_new_map()
//.text:00B9FDA0 ; void __cdecl widgets_initialize_for_new_structure_bsp(dword)
//.text:00B9FF30 ; void __cdecl widgets_new(long)
//.text:00B9FFE0 ; 
//.text:00BA00D0 ; 
//.text:00BA0150 ; void __cdecl widgets_render_opaque(long, long)
//.text:00BA01F0 ; void __cdecl widgets_render_unattached(long)

void __cdecl widgets_update(real game_seconds_elapsed)
{
	INVOKE(0x00BA0230, widgets_update, game_seconds_elapsed);
}

