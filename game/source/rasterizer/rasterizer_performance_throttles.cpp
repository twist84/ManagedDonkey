#include "rasterizer/rasterizer_performance_throttles.hpp"

// c_performance_throttles::m_default_throttles
REFERENCE_DECLARE(0x01917BA0, s_performance_throttles, g_default_performance_throttles);

// c_performance_throttles::m_ignore_predefined_performance_throttles
REFERENCE_DECLARE(0x05115AB4, bool, g_ignore_predefined_performance_throttles);

// c_performance_throttles::m_current_throttles
REFERENCE_DECLARE(0x05115AB8, s_performane_throttle, g_current_performance_throttles);

//.text:00A5F8E0 ; public: static s_performance_throttles* __cdecl c_performance_throttles::get_current_performance_throttles()
//.text:00A5F8F0 ; 
//.text:00A5F900 ; public: static real __cdecl c_performance_throttles::get_performance_throttle(char const*, long)
//.text:00A5FBB0 ; 
//.text:00A5FBE0 ; void __cdecl set_performance_throttle(char const*, long, real)
//.text:00A5FC00 ; public: static void __cdecl c_performance_throttles::set_performance_throttle(char const*, long, real)

void __cdecl c_performance_throttles::update_current_performance_throttles()
{
	INVOKE(0x00A5FF00, c_performance_throttles::update_current_performance_throttles);
}

