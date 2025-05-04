#include "geometry/geometry_definitions.hpp"

#include "memory/module.hpp"
#include "visibility/visibility_collection.hpp"

//HOOK_DECLARE_CALL(0x009745F6, mesh_has_subpart_mopp);

// emulate having no mopps
bool const clear_mopps = true;

bool __cdecl mesh_has_subpart_mopp(s_render_geometry* render_geometry, int32 mesh_index)
{
	//if (clear_mopps && render_geometry->per_mesh_mopp.count)
	//		render_geometry->per_mesh_mopp.clear();
	//return INVOKE(0x0078C780, mesh_has_subpart_mopp, render_geometry, mesh_index);

	for (uns16 cluster_index = 0; cluster_index < c_visible_items::m_items.clusters.get_count(); cluster_index++)
	{
		c_visible_items::m_items.clusters[cluster_index].flags |= FLAG(3);
	}

	return false;
}

