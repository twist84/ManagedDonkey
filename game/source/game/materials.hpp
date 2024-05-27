#pragma once

#include "cseries/cseries.hpp"

struct c_global_material_type
{
public:
	c_global_material_type() :
		m_index(NONE)
	{
	}

	bool operator!=(c_global_material_type const& other) const
	{
		return m_index != other.m_index;
	}

	word m_index;
};
static_assert(sizeof(c_global_material_type) == sizeof(word));

struct s_global_material_definition;

s_global_material_definition* global_get_material(c_global_material_type material_type);

