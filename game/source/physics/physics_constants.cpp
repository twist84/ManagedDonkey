#include "physics/physics_constants.hpp"

#include "cseries/cseries.hpp"
#include "game/game.hpp"
#include "game/game_time.hpp"
#include "memory/thread_local.hpp"

#include <cassert>
#include <math.h>

	bool __cdecl character_ground_adhesion_forces_disabled()
	{
		s_physics_constants* physics_constants = global_physics_constants_get();
		assert((physics_constants->character_ground_adhesion_forces_enable_time - game_time_get()) < game_seconds_to_ticks_round(k_physics_character_ground_adhesion_force_maximum_disable_time) + 1);

		return (physics_constants->character_ground_adhesion_forces_enable_time - game_time_get()) > 0;

		//DECLTHUNK(0x006814B0, character_ground_adhesion_forces_disabled)();
	}

	void __cdecl disable_character_ground_adhesion_forces(real disable_time)
	{
		if (disable_time <= k_physics_character_ground_adhesion_force_minimum_disable_time)
			disable_time = k_physics_character_ground_adhesion_force_minimum_disable_time;
		if (disable_time >= k_physics_character_ground_adhesion_force_maximum_disable_time)
			disable_time = k_physics_character_ground_adhesion_force_maximum_disable_time;
		global_physics_constants_get()->character_ground_adhesion_forces_enable_time = game_time_get() + game_seconds_to_ticks_round(disable_time);

		//DECLTHUNK(0x00681510, disable_character_ground_adhesion_forces)(enable_time);
	}

	s_physics_constants* __cdecl global_physics_constants_get()
	{
		s_thread_local_storage* tls = get_tls();
		if (!tls || !tls->g_physics_constants)
			return nullptr;

		return tls->g_physics_constants;

		//DECLTHUNK(0x006815A0, global_physics_constants_get)();
	}

	real_vector3d* __cdecl global_physics_velocity_frame_get()
	{
		return &global_physics_constants_get()->velocity_frame;

		//DECLTHUNK(0x006815C0, global_physics_velocity_frame_get)();
	}

	void __cdecl global_physics_velocity_frame_set(real i, real j, real k)
	{
		global_physics_constants_get()->velocity_frame = { i, j, k };

		//DECLTHUNK(0x006815E0, global_physics_velocity_frame_set)(i, j, k);
	}

	real __cdecl global_slip_surface_maximum_k_get()
	{
		return game_is_multiplayer() ? 1.0f : global_physics_constants_get()->lip_surface_maximum_k;

		//DECLTHUNK(0x00681620, global_slip_surface_maximum_k_get)();
	}

	real __cdecl global_standard_gravity_get()
	{
		return 4.1712594f;

		//DECLTHUNK(0x00681670, global_standard_gravity_get)();
	}

	void __cdecl physics_constants_dispose()
	{
		DECLTHUNK(0x00681680, physics_constants_dispose)();
	}

	void __cdecl physics_constants_dispose_from_old_map()
	{
		DECLTHUNK(0x006816A0, physics_constants_dispose_from_old_map)();
	}

	void __cdecl physics_constants_initialize()
	{
		DECLTHUNK(0x006816B0, physics_constants_initialize)();
	}

	void __cdecl physics_constants_initialize_for_new_map()
	{
		DECLTHUNK(0x00681770, physics_constants_initialize_for_new_map)();
	}

	void __cdecl physics_constants_reset()
	{
		s_physics_constants* physics_constants = global_physics_constants_get();

		physics_constants->gravity = global_standard_gravity_get();
		physics_constants->water_density = 1.0f;
		physics_constants->air_density = 0.0011f;
		physics_constants->character_ground_adhesion_forces_enable_time = 0;
		physics_constants->velocity_frame = { 0.0f, 0.0f, 0.0f }; // global_zero_vector3d
		physics_constants->lip_surface_maximum_k = static_cast<real>(cos(0.61086524f));

		//DECLTHUNK(0x006817F0, physics_constants_reset)();
	}

	void __cdecl physics_constants_set_gravity_absolute(real gravity)
	{
		global_physics_constants_get()->gravity = gravity;

		//DECLTHUNK(0x00681870, physics_constants_set_gravity_absolute)(gravity);
	}

	void __cdecl physics_constants_set_gravity_halo_relative(real gravity)
	{
		physics_constants_set_gravity_absolute(gravity * global_standard_gravity_get());

		//DECLTHUNK(0x006818A0, physics_constants_set_gravity_absolute)(gravity);
	}

	real __cdecl global_gravity_get()
	{
		return global_physics_constants_get()->gravity;
	}

	real __cdecl global_water_density_get()
	{
		return global_physics_constants_get()->water_density;
	}

	real __cdecl global_air_density_get()
	{
		return global_physics_constants_get()->air_density;
	}