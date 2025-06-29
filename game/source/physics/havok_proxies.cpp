#include "physics/havok_proxies.hpp"

#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "memory/data.hpp"
#include "memory/thread_local.hpp"
#include "physics/physics_models.hpp"

#include <hkWorld.hpp>

//.text:00679AC0 ; void __cdecl havok_proxies_apply_hard_key_frame(const hkVector4&, const hkQuaternion&, real32, const hkVector4&, hkRigidBody*)

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

void __cdecl havok_proxies_render_debug()
{
	if (!restricted_region_locked_for_current_thread(k_game_state_update_region))
	{
		return;
	}

	c_data_iterator<s_havok_proxy_datum> iterator;
	iterator.begin(g_havok_proxy_data);
	while (iterator.next())
	{
		s_havok_proxy_datum* havok_proxy = iterator.get_datum();
		hkRigidBody* rigid_body = havok_proxy->rigid_body;
		if (!rigid_body)
		{
			continue;
		}

		real_matrix4x3 transform{};
		matrix4x3_from_hkTransform(&transform, &rigid_body->m_motion.m_motionState.m_transform);
		const real_argb_color* color = global_real_argb_orange;
		if (havok_proxy->rigid_body->isActive())
		{
			color = global_real_argb_pink;
		}

		render_debug_physics_shape(havok_proxy->rigid_body->m_collidable.m_shape, &transform, color);
	}
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

