#include "render/render.hpp"

#include "memory/module.hpp"

// #TODO: reimplement `render_fullscreen_text_internal` to fix this in code
// 
// push    offset 0x0160D04C ; "\r\n"
// push    offset 0x0160C05C ; "blam cache release pc 1.106708 cert_ms23  Mar 20 2015 17:24:42"
byte const push_full_string_bytes[] = { 0x68, 0x5C, 0xC0, 0x60, 0x01 };
c_data_patch render_fullscreen_text_internal_patch0(0x00A298CF, NUMBEROF(push_full_string_bytes), push_full_string_bytes);

void render_fullscreen_text(s_render_fullscreen_text_context const* context, bool simple_font)
{
	INVOKE(0x00A297A0, render_fullscreen_text, context, simple_font);
}

