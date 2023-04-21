#include "geometry/geometry_definitions.hpp"

#include "memory/module.hpp"

HOOK_DECLARE_CALL(0x009745F6, mesh_has_subpart_mopp);

// emulate having no mopps
bool const clear_mopps = false;

bool __cdecl mesh_has_subpart_mopp(s_render_geometry* render_geometry, long mesh_index)
{
	if (clear_mopps && render_geometry->per_mesh_mopp.count())
			render_geometry->per_mesh_mopp.clear();

	return INVOKE(0x0078C780, mesh_has_subpart_mopp, render_geometry, mesh_index);
}

