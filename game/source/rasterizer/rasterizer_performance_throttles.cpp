#include "rasterizer/rasterizer_performance_throttles.hpp"

// c_performance_throttles::m_default_throttles
s_performance_throttles& g_default_performance_throttles = *reinterpret_cast<s_performance_throttles*>(0x01917BA0);

// c_performance_throttles::m_ignore_predefined_performance_throttles
bool& g_ignore_predefined_performance_throttles = *reinterpret_cast<bool*>(0x05115AB4);

// c_performance_throttles::m_current_throttles
s_performane_throttle& g_current_performance_throttles = *reinterpret_cast<s_performane_throttle*>(0x05115AB8);