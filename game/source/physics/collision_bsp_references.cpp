#include "physics/collision_bsp_references.hpp"

#include "structures/structure_bsp_definitions.hpp"

c_bsp3d_reference::c_bsp3d_reference() :
	m_bsp(nullptr),
	m_large_bsp(nullptr)
{
}

c_collision_bsp_reference::c_collision_bsp_reference() :
	c_bsp3d_reference()
{
}

c_collision_bsp_reference::c_collision_bsp_reference(structure_bsp const* bsp) :
	c_bsp3d_reference()
{
	initialize(bsp->resource_interface.get_resources());
}

void c_collision_bsp_reference::initialize(s_structure_bsp_resources const* bsp_resources)
{
	m_bsp = nullptr;
	m_large_bsp = nullptr;

	if (bsp_resources->bsp.count() == 1)
	{
		m_bsp = &bsp_resources->bsp[0];
	}
	else
	{
		if (bsp_resources->large_bsp.count() == 1)
		{
			m_large_bsp = &bsp_resources->large_bsp[0];
		}
	}
}

bool c_collision_bsp_reference::valid() const
{
	ASSERT(!(m_bsp != NULL && m_large_bsp != NULL));

	return !(m_bsp != NULL && m_large_bsp != NULL);
}

bool c_collision_bsp_reference::is_small() const
{
	ASSERT(valid());

	return m_bsp != nullptr;
}

collision_bsp const* c_collision_bsp_reference::get_small_bsp() const
{
	ASSERT(valid());

	return m_bsp;
}

large_collision_bsp const* c_collision_bsp_reference::get_large_bsp() const
{
	ASSERT(valid());

	return m_large_bsp;
}

long c_collision_bsp_reference::get_leaf_count() const
{
	ASSERT(valid());

	if (is_small())
		return m_bsp->leaves.count();

	return m_large_bsp->leaves.count();
}

long c_collision_bsp_reference::get_bsp2d_reference_count() const
{
	ASSERT(valid());

	if (is_small())
		return m_bsp->bsp2d_references.count();

	return m_large_bsp->bsp2d_references.count();
}

long c_collision_bsp_reference::get_bsp2d_node_count() const
{
	ASSERT(valid());

	if (is_small())
		return m_bsp->bsp2d_nodes.count();

	return m_large_bsp->bsp2d_nodes.count();
}

long c_collision_bsp_reference::get_surface_count() const
{
	ASSERT(valid());

	if (is_small())
		return m_bsp->surfaces.count();

	return m_large_bsp->surfaces.count();
}

long c_collision_bsp_reference::get_edge_count() const
{
	ASSERT(valid());

	if (is_small())
		return m_bsp->edges.count();

	return m_large_bsp->edges.count();
}

long c_collision_bsp_reference::get_vertex_count() const
{
	ASSERT(valid());

	if (is_small())
		return m_bsp->vertices.count();

	return m_large_bsp->vertices.count();
}

