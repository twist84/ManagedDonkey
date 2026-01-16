#pragma once

#include "cseries/cseries.hpp"

struct hkRigidBody;
struct s_havok_proxy_datum :
	s_datum_header
{
	hkRigidBody* rigid_body;
	real_matrix4x3 transform;
	int32 object_index;
	int32 creation_time;
};
COMPILE_ASSERT(sizeof(s_havok_proxy_datum) == 0x44);

extern void __cdecl havok_proxies_dispose();
extern void __cdecl havok_proxies_dispose_from_old_map();
extern void __cdecl havok_proxies_initialize();
extern void __cdecl havok_proxies_initialize_for_new_map();
extern void __cdecl havok_proxies_move();
extern void __cdecl havok_proxies_render_debug();
extern void __cdecl havok_proxies_update();

