#include "render/depth_of_field.hpp"

//.text:00A3EA20 ; void __cdecl __tls_set_g_depth_of_field_game_globals_allocator(void*)
//.text:00A3EA40 ; 
//.text:00A3EA80 ; 
//.text:00A3EAB0 ; public: static void __cdecl s_depth_of_field::dispose()
//.text:00A3EAC0 ; public: static void __cdecl s_depth_of_field::dispose_from_old_map()
//.text:00A3EAD0 ; 
//.text:00A3EB00 ; public: static s_depth_of_field* __cdecl s_depth_of_field::get()
//.text:00A3EB20 ; public: bool __cdecl s_depth_of_field::get_current_depth_of_field_parameters(s_observer_depth_of_field const*, real*, real*, real*, real*)
//.text:00A3EC30 ; public: static real __cdecl s_depth_of_field::get_depth_of_field_blur_sigma(s_observer_depth_of_field const*)
//.text:00A3ECB0 ; public: static bool __cdecl s_depth_of_field::get_enabled()
//.text:00A3ECD0 ; public: static void __cdecl s_depth_of_field::initialize()
//.text:00A3ED30 ; public: static void __cdecl s_depth_of_field::initialize_for_new_map()
//.text:00A3ED50 ; void __cdecl render_animate_depth_of_field(real, real, real, real)
//.text:00A3EE30 ; void __cdecl render_animate_depth_of_field_blur(real, real)
//.text:00A3EEB0 ; void __cdecl render_enable_depth_of_field(bool)
//.text:00A3EEE0 ; void __cdecl render_set_depth_of_field(real)
//.text:00A3EF30 ; 
//.text:00A3EF70 ; public: void __cdecl s_depth_of_field::set_default_values()

void __cdecl s_depth_of_field::update()
{
	INVOKE(0x00A3EFE0, s_depth_of_field::update);
}

//.text:00A3F000 ; private: void __cdecl s_depth_of_field::update_internal()

