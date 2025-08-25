#include "physics/collision_references.hpp"

#include "physics/bsp3d.hpp"
#include "structures/structure_bsp_definitions.hpp"

#include <climits>

c_collision_edge_reference::c_collision_edge_reference() :
	c_collision_bsp_reference(),
	m_edge_index(0)
{
}

c_collision_edge_reference::c_collision_edge_reference(c_collision_bsp_reference bsp_reference, int32 index) :
	c_collision_bsp_reference(bsp_reference),
	m_edge_index(index)
{
}

int32 c_collision_edge_reference::get_edge_index() const
{
	return m_edge_index;
}

int32 c_collision_edge_reference::get_vertex_index(int32 index) const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_edge* edge = &small_bsp->edges[m_edge_index];
		int32 vertex_index = edge->vertex_indices[index];
		return vertex_index == NONE ? vertex_index : vertex_index & 0xFFFF;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_edge* edge = &large_bsp->edges[m_edge_index];
	int32 vertex_index = edge->vertex_indices[index];
	return vertex_index;
}

int32 c_collision_edge_reference::get_edge_index(int32 index) const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_edge* edge = &small_bsp->edges[m_edge_index];
		int32 edge_index = edge->edge_indices[index];
		return edge_index == NONE ? edge_index : edge_index & 0xFFFF;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_edge* edge = &large_bsp->edges[m_edge_index];
	int32 edge_index = edge->edge_indices[index];
	return edge_index;
}

int32 c_collision_edge_reference::get_surface_index(int32 index) const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_edge* edge = &small_bsp->edges[m_edge_index];
		int32 surface_index = edge->surface_indices[index];
		return surface_index == NONE ? surface_index : surface_index & 0xFFFF;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_edge* edge = &large_bsp->edges[m_edge_index];
	int32 surface_index = edge->surface_indices[index];
	return surface_index;
}

c_collision_vertex_reference::c_collision_vertex_reference() :
	c_collision_bsp_reference(),
	m_vertex_index(0)
{
}

c_collision_vertex_reference::c_collision_vertex_reference(c_collision_bsp_reference bsp_reference, int32 index) :
	c_collision_bsp_reference(bsp_reference),
	m_vertex_index(index)
{
}

int32 c_collision_vertex_reference::get_vertex_index() const
{
	return m_vertex_index;
}

real_point3d* c_collision_vertex_reference::get_position() const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_vertex* vertex = &small_bsp->vertices[m_vertex_index];
		real_point3d* point = &vertex->point;
		return point;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_vertex* vertex = &large_bsp->vertices[m_vertex_index];
	real_point3d* point = &vertex->point;
	return point;
}

int32 c_collision_vertex_reference::get_first_edge_index() const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_vertex* vertex = &small_bsp->vertices[m_vertex_index];
		int32 first_edge = vertex->first_edge;
		return first_edge == NONE ? first_edge : first_edge & 0xFFFF;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_vertex* vertex = &large_bsp->vertices[m_vertex_index];
	int32 first_edge = vertex->first_edge;
	return first_edge;
}

int32 c_collision_vertex_reference::get_sink() const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_vertex* vertex = &small_bsp->vertices[m_vertex_index];
		int32 sink = vertex->sink;
		return sink == NONE ? sink : sink & 0xFFFF;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_vertex* vertex = &large_bsp->vertices[m_vertex_index];
	int32 sink = vertex->sink;
	return sink;
}

c_collision_surface_reference::c_collision_surface_reference() :
	c_collision_bsp_reference(),
	m_surface_index(0)
{
}

c_collision_surface_reference::c_collision_surface_reference(c_collision_bsp_reference bsp_reference, int32 index) :
	c_collision_bsp_reference(bsp_reference),
	m_surface_index(index)
{
}

uns8 c_collision_surface_reference::get_best_plane_calculation_vertex_index() const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_surface* surface= &small_bsp->surfaces[m_surface_index];
		uns8 best_plane_calculation_vertex_index = surface->best_plane_calculation_vertex_index;
		return best_plane_calculation_vertex_index;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_surface* surface= &large_bsp->surfaces[m_surface_index];
	uns8 best_plane_calculation_vertex_index = surface->best_plane_calculation_vertex_index;
	return best_plane_calculation_vertex_index;
}

int32 c_collision_surface_reference::get_breakable_surface_set_index() const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_surface* surface = &small_bsp->surfaces[m_surface_index];
		int32 breakable_surface_set = surface->breakable_surface_set;
		return breakable_surface_set == NONE ? breakable_surface_set : breakable_surface_set & 0xFFFF;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_surface* surface = &large_bsp->surfaces[m_surface_index];
	int32 breakable_surface_set = surface->breakable_surface_set;
	return breakable_surface_set;
}

int32 c_collision_surface_reference::get_breakable_surface_index() const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_surface* surface = &small_bsp->surfaces[m_surface_index];
		int32 breakable_surface = surface->breakable_surface;
		return breakable_surface == NONE ? breakable_surface : breakable_surface & 0xFFFF;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_surface* surface = &large_bsp->surfaces[m_surface_index];
	int32 breakable_surface = surface->breakable_surface;
	return breakable_surface;
}

int32 c_collision_surface_reference::get_first_edge_index() const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_surface* surface = &small_bsp->surfaces[m_surface_index];
		int32 first_edge = surface->first_edge;
		return first_edge == NONE ? first_edge : first_edge & 0xFFFF;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_surface* surface = &large_bsp->surfaces[m_surface_index];
	int32 first_edge = surface->first_edge;
	return first_edge;
}

uns8 c_collision_surface_reference::get_flags() const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_surface* surface = &small_bsp->surfaces[m_surface_index];
		uns8 flags = surface->flags;
		return flags;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_surface* surface = &large_bsp->surfaces[m_surface_index];
	uns8 flags = surface->flags;
	return flags;
}

int32 c_collision_surface_reference::get_material_index() const
{
	if (is_small())
	{
		const collision_bsp* small_bsp = get_small_bsp();
		collision_surface* surface = &small_bsp->surfaces[m_surface_index];
		int32 material = surface->material;
		return material == NONE ? material : material & 0xFFFF;
	}

	const large_collision_bsp* large_bsp = get_large_bsp();
	large_collision_surface* surface = &large_bsp->surfaces[m_surface_index];
	int32 material = surface->material;
	return material;
}

const real_plane3d* c_collision_surface_reference::get_plane(real_plane3d* plane) const
{
	ASSERT(plane);

	if (is_small())
		return bsp3d_get_plane_from_designator_internal(get_small_bsp(), get_small_bsp()->surfaces[m_surface_index].plane, plane);

	return bsp3d_get_plane_from_designator_internal(get_large_bsp(), get_large_bsp()->surfaces[m_surface_index].plane, plane);
}

const real_plane3d* c_collision_surface_reference::get_plane(bool* is_negated) const
{
	//ASSERT(valid());

	if (is_negated)
		*is_negated = is_plane_negated();

	if (is_small())
		return &get_small_bsp()->planes[get_plane_index()];

	return &get_large_bsp()->planes[get_plane_index()];
}

int32 c_collision_surface_reference::get_plane_index() const
{
	// $TODO this assert
	//ASSERT((m_surface_index != NONE) == m_bsp.valid())

	if (is_small())
		return get_small_bsp()->surfaces[m_surface_index].plane & SHRT_MAX;

	return get_large_bsp()->surfaces[m_surface_index].plane & LONG_MAX;
}

bool c_collision_surface_reference::is_plane_negated() const
{
	// $TODO this assert
	//ASSERT((m_surface_index != NONE) == m_bsp.valid())

	if (is_small())
		return (get_small_bsp()->surfaces[m_surface_index].plane & 0x8000) != 0;

	return get_large_bsp()->surfaces[m_surface_index].plane < 0;
}

