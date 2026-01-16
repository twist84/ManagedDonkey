#pragma once

#include "cseries/cseries.hpp"

struct s_observer_depth_of_field;
struct s_depth_of_field
{
//public:
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static s_depth_of_field* __cdecl get();
	bool get_current_depth_of_field_parameters(const s_observer_depth_of_field* observer_depth_of_field, real32* focus_distance, real32* aperture, real32* focus_half_width, real32* depth_of_field_blur);
	static real32 __cdecl get_depth_of_field_blur_sigma(const s_observer_depth_of_field* observer_depth_of_field);
	static bool __cdecl get_enabled();
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();
	void set_default_values();
	static void __cdecl update();

//private:
	void update_internal();

//protected:
	bool enabled;
	bool animate;
	real32 speed;
	real32 focus_distance_near;
	real32 focus_distance_far;
	real32 aperture;
	real32 blur_start;
	real32 blur_target;
	int32 blur_start_time;
	int32 blur_end_time;
	int32 animation_start_time;
	int32 animation_end_time;
	real32 animation_last_alpha;
	real32 actual_blur;
	real32 actual_focus_distance_near;
	real32 actual_focus_distance_far;
	real32 actual_aperture;
};
COMPILE_ASSERT(sizeof(s_depth_of_field) == 0x40);

extern void __cdecl render_animate_depth_of_field(real32 near_distance, real32 far_distance, real32 depth, real32 ticks);
extern void __cdecl render_animate_depth_of_field_blur(real32 blur, real32 ticks);
extern void __cdecl render_enable_depth_of_field(bool enable);
extern void __cdecl render_set_depth_of_field(real32 depth);

