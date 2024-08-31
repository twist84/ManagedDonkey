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

extern void __cdecl havok_proxies_dispose();
extern void __cdecl havok_proxies_dispose_from_old_map();
extern void __cdecl havok_proxies_initialize();
extern void __cdecl havok_proxies_initialize_for_new_map();
extern void __cdecl havok_proxies_move();
extern void __cdecl havok_proxies_update();

