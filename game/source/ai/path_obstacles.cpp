#include "ai/path_obstacles.hpp"

void __cdecl obstacles_new(struct obstacles* obstacles)
{
	INVOKE(0x014CB890, obstacles_new, obstacles);
}

void render_debug_obstacles(struct obstacles const* obstacles, real radius)
{
	// #TODO: implement this
}

