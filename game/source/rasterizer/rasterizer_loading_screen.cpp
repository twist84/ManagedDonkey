#include "rasterizer/rasterizer_loading_screen.hpp"

bool __cdecl c_rasterizer_loading_screen::active()
{
	//return INVOKE(0x00A204E0, c_rasterizer_loading_screen::active);

	return false;
}

void __cdecl c_rasterizer_loading_screen::dispose()
{
	//INVOKE(0x00A204F0, c_rasterizer_loading_screen::dispose);
}

bool __cdecl c_rasterizer_loading_screen::fading_out()
{
	//return INVOKE(0x00A20500, c_rasterizer_loading_screen::fading_out);

	return false;
}

real __cdecl c_rasterizer_loading_screen::get_completion_remainder()
{
	//return INVOKE(0x00A20510, c_rasterizer_loading_screen::get_completion_remainder);

	return 0.0f;
}

void __cdecl c_rasterizer_loading_screen::initialize()
{
	//INVOKE(0x00A20520, c_rasterizer_loading_screen::initialize);
}

void __cdecl c_rasterizer_loading_screen::render()
{
	//INVOKE(0x00A20530, c_rasterizer_loading_screen::render);
}

void __cdecl c_rasterizer_loading_screen::set_completion_remainder(real completion_remainder)
{
	//INVOKE(0x00A20540, c_rasterizer_loading_screen::set_completion_remainder, completion_remainder);
}

void __cdecl c_rasterizer_loading_screen::start()
{
	//INVOKE(0x00A20550, c_rasterizer_loading_screen::start);
}

//.text:00A20560 ; bool __cdecl loading_basic_progress_keep_pumping()

void __cdecl c_rasterizer_loading_screen::stop()
{
	//INVOKE(0x00A20570, c_rasterizer_loading_screen::stop);
}

bool __cdecl c_rasterizer_loading_screen::suppress_render_scene()
{
	//return INVOKE(0x00A20580, c_rasterizer_loading_screen::suppress_render_scene);

	return false;
}

