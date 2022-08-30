#include "physics/physics_constants.hpp"

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

		//return reinterpret_cast<decltype(character_ground_adhesion_forces_disabled)*>(0x006814B0)();
	}

	void __cdecl disable_character_ground_adhesion_forces(real disable_time)
	{
		if (disable_time <= k_physics_character_ground_adhesion_force_minimum_disable_time)
			disable_time = k_physics_character_ground_adhesion_force_minimum_disable_time;
		if (disable_time >= k_physics_character_ground_adhesion_force_maximum_disable_time)
			disable_time = k_physics_character_ground_adhesion_force_maximum_disable_time;
		global_physics_constants_get()->character_ground_adhesion_forces_enable_time = game_time_get() + game_seconds_to_ticks_round(disable_time);

		//reinterpret_cast<decltype(disable_character_ground_adhesion_forces)*>(0x00681510)(enable_time);
	}

	s_physics_constants* __cdecl global_physics_constants_get()
	{
		s_thread_local_storage* tls = get_tls();
		if (!tls || !tls->g_physics_constants)
			return nullptr;

		return tls->g_physics_constants;

		//return reinterpret_cast<decltype(global_physics_constants_get)*>(0x006815A0)();
	}

	real_vector3d* __cdecl global_physics_velocity_frame_get()
	{
		return &global_physics_constants_get()->velocity_frame;

		//return reinterpret_cast<decltype(global_physics_velocity_frame_get)*>(0x006815C0)();
	}

	void __cdecl global_physics_velocity_frame_set(real i, real j, real k)
	{
		global_physics_constants_get()->velocity_frame = { i, j, k };

		//reinterpret_cast<decltype(global_physics_velocity_frame_set)*>(0x006815E0)(i, j, k);
	}

	real __cdecl global_slip_surface_maximum_k_get()
	{
		return game_is_multiplayer() ? 1.0f : global_physics_constants_get()->lip_surface_maximum_k;

		//return reinterpret_cast<decltype(global_slip_surface_maximum_k_get)*>(0x00681620)();
	}

	real __cdecl global_standard_gravity_get()
	{
		return 4.1712594f;

		//return reinterpret_cast<decltype(global_standard_gravity_get)*>(0x00681670)();
	}

	void __cdecl physics_constants_dispose()
	{
		reinterpret_cast<decltype(physics_constants_dispose)*>(0x00681680)();
	}

	void __cdecl physics_constants_dispose_from_old_map()
	{
		reinterpret_cast<decltype(physics_constants_dispose_from_old_map)*>(0x006816A0)();
	}

	void __cdecl physics_constants_initialize()
	{
		reinterpret_cast<decltype(physics_constants_initialize)*>(0x006816B0)();
	}

	void __cdecl physics_constants_initialize_for_new_map()
	{
		reinterpret_cast<decltype(physics_constants_initialize_for_new_map)*>(0x00681770)();
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

		//reinterpret_cast<decltype(physics_constants_reset)*>(0x006817F0)();
	}

	void __cdecl physics_constants_set_gravity_absolute(real gravity)
	{
		global_physics_constants_get()->gravity = gravity;

		//reinterpret_cast<decltype(physics_constants_set_gravity_absolute)*>(0x00681870)(gravity);
	}

	void __cdecl physics_constants_set_gravity_halo_relative(real gravity)
	{
		physics_constants_set_gravity_absolute(gravity * global_standard_gravity_get());

		//reinterpret_cast<decltype(physics_constants_set_gravity_absolute)*>(0x006818A0)(gravity);
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