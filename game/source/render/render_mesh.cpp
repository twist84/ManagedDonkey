#include "render/render_mesh.hpp"

#include "cseries/cseries.hpp"

void __cdecl begin_render_mesh_part_only()
{
	INVOKE(0x00A473F0, begin_render_mesh_part_only);

	//g_render_mesh_part_only = !g_disable_render_state_cache_optimization;
}

void __cdecl end_render_mesh_part_only()
{
	INVOKE(0x00A47400, end_render_mesh_part_only);

	//g_render_mesh_part_only = false;
	//if (g_render_state_cache_should_be_invalidated)
	//{
	//	c_rasterizer::invalidate_render_state_cache();
	//	render_method_submit_invalidate_cache();
	//	g_render_state_cache_should_be_invalidated = false;
	//}
}

//.text:00A47430 ; bool __cdecl part_is_renderable(s_part const*, geometry_material const*, int32)

