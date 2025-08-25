#include "rasterizer/rasterizer_hue_saturation.hpp"

#include "memory/module.hpp"
#include "memory/thread_local.hpp"

//REFERENCE_DECLARE(0x01914A7C, c_hue_saturation_control, x_default);
//REFERENCE_DECLARE(0x050FB650, c_hue_saturation_control, x_exposed_to_hs);

REFERENCE_DECLARE(0x01914A7C, c_hue_saturation_control, g_hue_saturation_control_default);
REFERENCE_DECLARE(0x050FB650, c_hue_saturation_control, g_hue_saturation_control_exposed_to_hs);

HOOK_DECLARE(0x00A41130, render_postprocess_reset_color_tweaking);

//c_hue_saturation_control c_hue_saturation_control::x_default
//{
//	.m_hue = 0.0f,
//	.m_saturation = 1.0f,
//	.m_filter_red = 1.0f,
//	.m_filter_green = 1.0f,
//	.m_filter_blue = 1.0f
//};

//c_hue_saturation_control c_hue_saturation_control::x_exposed_to_hs = c_hue_saturation_control::x_default;

void __cdecl c_hue_saturation_control::copy_from_gamestate()
{
	//INVOKE(0x00530470, copy_from_gamestate);

	//c_hue_saturation_control::x_exposed_to_hs = *c_hue_saturation_control::x_in_gamestate;
	g_hue_saturation_control_exposed_to_hs = *g_hue_saturation_control_in_gamestate;
}

void __cdecl c_hue_saturation_control::copy_to_gamestate()
{
	//INVOKE(0x005304B0, copy_from_gamestate);

	//*c_hue_saturation_control::x_in_gamestate = c_hue_saturation_control::x_exposed_to_hs;
	*g_hue_saturation_control_in_gamestate = g_hue_saturation_control_exposed_to_hs;
}

void __cdecl render_postprocess_reset_color_tweaking()
{
	//INVOKE(0x00A41130, render_postprocess_reset_color_tweaking);

	g_hue_saturation_control_exposed_to_hs.reset();
}

void c_hue_saturation_control::reset()
{
	//INVOKE_CLASS_MEMBER(0x00A41170, c_hue_saturation_control, reset);

	m_hue = 0.0f;
	m_saturation = 1.0f;
	m_filter_red = 1.0f;
	m_filter_green = 1.0f;
	m_filter_blue = 1.0f;
}

//.text:00A411A0 ; c_hue_saturation_control::set_hue_saturaton_and_color_filters, $TODO fix __usercall calling convention

