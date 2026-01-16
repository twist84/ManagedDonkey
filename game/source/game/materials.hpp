#pragma once

#include "cseries/cseries.hpp"

class c_global_material_type
{
public:
	c_global_material_type() :
		m_index(NONE)
	{
	}

	bool operator!=(const c_global_material_type& other) const
	{
		return m_index != other.m_index;
	}

	uns16 m_index;
};
COMPILE_ASSERT(sizeof(c_global_material_type) == sizeof(uns16));

struct s_global_material_definition;

s_global_material_definition* global_get_material(c_global_material_type material_type);

