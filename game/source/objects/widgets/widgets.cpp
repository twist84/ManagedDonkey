#include "objects/widgets/widgets.hpp"

//.text:00B9F0A0 ; 
//.text:00B9F0C0 ; 
//.text:00B9F0D0 ; void __cdecl __tls_set_g_widget_data_allocator(void*)
//.text:00B9F100 ; 
//.text:00B9F140 ; 
//.text:00B9F160 ; 
//.text:00B9F190 ; 
//.text:00B9F1C0 ; 
//.text:00B9F1D0 ; 
//.text:00B9F1E0 ; 
//.text:00B9F1F0 ; public: void __cdecl c_short_quaternion::get_real_matrix4x3(real_matrix4x3*) const
//.text:00B9F270 ; public: void __cdecl c_word_quantized_position::get_real_point3d(const real_rectangle3d*, real_point3d*) const
//.text:00B9F330 ; 
//.text:00B9F3A0 ; public: void __cdecl c_dword_unit_vector::get_real_vector3d(real_vector3d*) const
//.text:00B9F460 ; 
//.text:00B9F480 ; int32 __cdecl object_get_first_widget(int32)
//.text:00B9F4B0 ; 
//.text:00B9F4F0 ; public: void __cdecl c_short_quaternion::set_real_matrix4x3(const real_matrix4x3*)
//.text:00B9F570 ; public: void __cdecl c_word_quantized_position::set_real_point3d(const real_rectangle3d*, const real_point3d*)
//.text:00B9F670 ; public: void __cdecl c_short_quaternion::set_real_quaternion(real_quaternion*)
//.text:00B9F6E0 ; public: void __cdecl c_dword_unit_vector::set_real_vector3d(const real_vector3d*)
//.text:00B9F700 ; int16 __cdecl tag_group_to_widget_type(uns32)
//.text:00B9F730 ; void __cdecl widget_delete_type_datum(int32, int32, bool)
//.text:00B9F830 ; int32 __cdecl widget_get_next_widget(int32)
//.text:00B9F860 ; void __cdecl widget_get_type_data(int32, int32*, int32*)
//.text:00B9F8A0 ; void __cdecl widget_submit_transparent(int32)
//.text:00B9F950 ; void __cdecl widget_transfer_objects(int32, int32, int32)
//.text:00B9F9D0 ; widget_type_definition* __cdecl widget_type_definition_get(int16)
//.text:00B9F9F0 ; int32 __cdecl widget_type_flags(int16)
//.text:00B9FA10 ; void __cdecl widget_type_new_on_object(int32, int16, int32)
//.text:00B9FA10 ; void __cdecl widget_type_new_on_object(int32, int16, int32)
//.text:00B9FB20 ; void __cdecl widgets_delete(int32)
//.text:00B9FBC0 ; void __cdecl widgets_dispose()
//.text:00B9FBF0 ; void __cdecl widgets_dispose_from_old_map(void)
//.text:00B9FC30 ; void __cdecl widgets_dispose_from_old_structure_bsp(uns32)
//.text:00B9FCF0 ; void __cdecl widgets_initialize()
//.text:00B9FD50 ; void __cdecl widgets_initialize_for_new_map()
//.text:00B9FDA0 ; void __cdecl widgets_initialize_for_new_structure_bsp(uns32)
//.text:00B9FF30 ; void __cdecl widgets_new(int32)
//.text:00B9FFE0 ; void __cdecl widgets_object_submit_transparents(int32)
//.text:00BA00D0 ; void __cdecl widgets_render_debug(int32)
//.text:00BA0150 ; void __cdecl widgets_render_opaque(int32, int32)

void __cdecl widgets_render_unattached(int32 entry_point)
{
	INVOKE(0x00BA01F0, widgets_render_unattached, entry_point);
}

void __cdecl widgets_update(real32 game_seconds_elapsed)
{
	INVOKE(0x00BA0230, widgets_update, game_seconds_elapsed);
}

