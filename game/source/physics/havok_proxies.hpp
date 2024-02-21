#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct hkRigidBody;
struct s_havok_proxy_datum :
	s_datum_header
{
	hkRigidBody* rigid_body;
	real_matrix4x3 matrix;
	long object_index;
	dword __time40;
};
static_assert(sizeof(s_havok_proxy_datum) == 0x44);

