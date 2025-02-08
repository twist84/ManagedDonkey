#pragma once

#include "cseries/cseries.hpp"

struct s_observer_depth_of_field;
struct s_depth_of_field
{
//public:
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static s_depth_of_field* __cdecl get();
	bool get_current_depth_of_field_parameters(s_observer_depth_of_field const* observer_depth_of_field, real* focus_distance, real* aperture, real* focus_half_width, real* depth_of_field_blur);
	static real __cdecl get_depth_of_field_blur_sigma(s_observer_depth_of_field const* observer_depth_of_field);
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
	real speed;
	real focus_distance_near;
	real focus_distance_far;
	real aperture;
	real blur_start;
	real blur_target;
	long blur_start_time;
	long blur_end_time;
	long animation_start_time;
	long animation_end_time;
	real animation_last_alpha;
	real actual_blur;
	real actual_focus_distance_near;
	real actual_focus_distance_far;
	real actual_aperture;
};
static_assert(sizeof(s_depth_of_field) == 0x40);

extern void __cdecl render_animate_depth_of_field(real near_distance, real far_distance, real depth, real ticks);
extern void __cdecl render_animate_depth_of_field_blur(real blur, real ticks);
extern void __cdecl render_enable_depth_of_field(bool enable);
extern void __cdecl render_set_depth_of_field(real depth);

