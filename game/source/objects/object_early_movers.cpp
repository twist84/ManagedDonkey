#include "objects/object_early_movers.hpp"

#include "memory/thread_local.hpp"
#include "render/render_debug.hpp"

bool debug_objects = true;
bool debug_objects_early_movers = true;

bool __cdecl object_early_mover_get_obb(long object_index_array_index, s_object_early_mover_obb* obb)
{
	return INVOKE(0x00B98190, object_early_mover_get_obb, object_index_array_index, obb);
}

void __cdecl object_early_mover_render_debug()
{
	if (!debug_objects)
		return;

	if (!debug_objects_early_movers)
		return;

	TLS_DATA_GET_VALUE_REFERENCE(g_object_early_movers_globals);
	for (long i = 0; i < g_object_early_movers_globals->object_index_count; i++)
	{
		s_object_early_mover_obb obb{};
		if (object_early_mover_get_obb(i, &obb))
			render_debug_box_outline_oriented(true, &obb.bounds, &obb.matrix, global_real_argb_blue);
	}
}

