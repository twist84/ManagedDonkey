#include "physics/havok_proxies.hpp"

//.text:00679AC0 ; void __cdecl havok_proxies_apply_hard_key_frame(hkVector4 const&, hkQuaternion const&, real32, hkVector4 const&, hkRigidBody*)

void __cdecl havok_proxies_dispose()
{
	INVOKE(0x0067A370, havok_proxies_dispose);
}

void __cdecl havok_proxies_dispose_from_old_map()
{
	INVOKE(0x0067A390, havok_proxies_dispose_from_old_map);
}

void __cdecl havok_proxies_initialize()
{
	INVOKE(0x0067A4E0, havok_proxies_initialize);
}

void __cdecl havok_proxies_initialize_for_new_map()
{
	INVOKE(0x0067A510, havok_proxies_initialize_for_new_map);
}

//.text:0067A530 ; void __cdecl havok_proxies_initialize_rigid_bodies()

void __cdecl havok_proxies_move()
{
	INVOKE(0x0067A5C0, havok_proxies_move);
}

void __cdecl havok_proxies_update()
{
	INVOKE(0x0067A630, havok_proxies_update);
}

//.text:0067AAF0 ; void __cdecl havok_proxy_aquire(int32)
//.text:0067AB40 ; bool __cdecl havok_proxy_aquire_internal(int32, bool)
//.text:0067ACA0 ; void __cdecl havok_proxy_rebuild(int32)
//.text:0067AD10 ; void __cdecl havok_proxy_release(int32)
//.text:0067AD30 ; void __cdecl havok_proxy_release_internal(int32, bool)
//.text:0067ADE0 ; int32 __cdecl havok_proxy_rigid_bodies_get(c_static_array<hkRigidBody*, 16>*)
//.text:0067AE50 ; 

