#pragma once

enum e_output_user_index;

class c_cubemap_debug
{
public:
	static void render();
	static void render_user_cubemap_samples(e_output_user_index user_index);
	static void render_object_cubemap_samples(long object_index);

	static bool g_render;
};

