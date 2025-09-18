#include "math/random_math.hpp"

#include "cseries/cseries_events.hpp"
#include "editor/editor_stubs.hpp"
#include "game/game_globals.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"

#include <stdlib.h>

REFERENCE_DECLARE(0x0238ED14, uns32, random_seed_local);

//HOOK_DECLARE(0x0051F070, random_math_dispose);
//HOOK_DECLARE(0x0051F0B0, random_math_initialize);

int32 random_seed_allow_use_count = 0;
bool random_seed_debug_file_open = false;
s_file_reference random_seed_debug_file{};

//.text:0051E790 ; public: __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>()
//.text:0051E7B0 ; public: __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::~t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>()
//.text:0051E7C0 ; void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)
//.text:0051E7E0 ; real_vector3d* __cdecl _distriubuite_vector_in_cone3d(uns32*, const char*, const char*, uns32, const real_vector3d*, int32, int32, int32, int32, real32, real32, real32, real_vector3d*)
//.text:0051EAA0 ; real_vector3d* __cdecl _random_direction3d(uns32*, const char*, const char*, uns32, real_vector3d*)
//.text:0051ECA0 ; real_vector3d* __cdecl _random_vector_in_cone3d(uns32*, const char*, const char*, uns32, const real_vector3d*, real32, real32, real_vector3d*)
//.text:0051EE80 ; public: virtual void* __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::allocate(unsigned int, const char*)
//.text:0051EEC0 ; public: virtual void __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::deallocate(void*, int32)
//.text:0051EEF0 ; real64 __cdecl distribute_random_stratified(uns32*, const char*, const char*, uns32, int32, int32)
//.text:0051EF70 ; public: void __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::free_memory()

uns32 __cdecl generate_random_seed()
{
	//return INVOKE(0x0051EFA0, generate_random_seed);

	uns32 time = system_milliseconds();
	uns32 seed = system_seconds() ^ time;
	return seed ^ rand();
}

uns32 __cdecl get_local_random_seed()
{
	return INVOKE(0x0051EFC0, get_local_random_seed);

	return *get_local_random_seed_address();
}

uns32* __cdecl get_local_random_seed_address()
{
	//return INVOKE(0x0051EFD0, get_local_random_seed_address);

	return &random_seed_local;
}

uns32 __cdecl get_random_seed()
{
	//return INVOKE(0x0051EFE0, get_random_seed);

	return *get_random_seed_address();
}

uns32* __cdecl get_random_seed_address()
{
	//return INVOKE(0x0051F000, get_random_seed_address);

	if (game_in_editor())
		ASSERT(random_seed_allow_use_count > 0);

	return g_deterministic_random_seed_ptr;
}

uns32 __cdecl random_direction_table_count()
{
	return INVOKE(0x0051F020, random_direction_table_count);
}

bool __cdecl random_direction_table_sample(int32 sample_index, real_vector3d* out_sample)
{
	return INVOKE(0x0051F030, random_direction_table_sample, sample_index, out_sample);
}

void __cdecl random_math_dispose()
{
	//INVOKE(0x0051F070, random_math_dispose);

	if (random_seed_debug_file_open)
	{
		file_close(&random_seed_debug_file);
		random_seed_debug_file_open = false;
	}
}

void __cdecl random_math_dispose_tool()
{
	//INVOKE(0x0051F080, random_math_dispose_tool);

	if (g_deterministic_random_seed_ptr)
	{
		system_free(g_deterministic_random_seed_ptr);
		g_deterministic_random_seed_ptr = NULL;
	}
}

void __cdecl random_math_initialize()
{
	//INVOKE(0x0051F0B0, random_math_initialize);

	g_deterministic_random_seed_ptr = (uns32*)g_deterministic_random_seed_ptr_allocator.allocate(sizeof(uns32), "random math");
	random_math_initialize_internal();
}

void __cdecl random_math_initialize_internal()
{
	//INVOKE(0x0051F130, random_math_initialize_internal);
	
	*g_deterministic_random_seed_ptr = 0x78A8;
	random_seed_local = generate_random_seed();
	random_seed_allow_use_count = 0;
	//generate_random_direction_table();
}

void __cdecl random_math_initialize_tool()
{
	//INVOKE(0x0051F170, random_math_initialize_tool);

	g_deterministic_random_seed_ptr = (uns32*)system_malloc(sizeof(uns32));
	random_math_initialize_internal();
}

//.text:0051F1C0 ; public: void* __cdecl t_restricted_allocation_manager<1, 0, 0, &void __cdecl __tls_set_g_deterministic_random_seed_ptr_allocator(void*)>::reserve_memory(const char*, const char*, unsigned int, int32)

void __cdecl set_local_random_seed(uns32 local_random_seed)
{
	//INVOKE(0x0051F200, set_local_random_seed, local_random_seed);

	*get_local_random_seed_address() = local_random_seed;
}

void __cdecl set_random_seed(uns32 random_seed)
{
	//INVOKE(0x0051F210, set_random_seed, random_seed);

	*get_random_seed_address() = random_seed;
}

//.text:01596530 ; void __cdecl `dynamic initializer for 'g_deterministic_random_seed_ptr_allocator''()
//.text:015F99B0 ; void __cdecl `dynamic atexit destructor for 'g_deterministic_random_seed_ptr_allocator''()

bool random_seed_usable()
{
	return random_seed_allow_use_count > 0;
}

void random_seed_allow_use()
{
	ASSERT(get_current_thread_index() == k_thread_main);

	random_seed_allow_use_count++;
}

void random_seed_disallow_use()
{
	ASSERT(get_current_thread_index() == k_thread_main);

	if (!game_in_editor())
	{
		VASSERT(random_seed_allow_use_count > 0, "unmatched call to random_seed_disallow() somewhere");
	}

	random_seed_allow_use_count--;
}

void random_seed_debug_log_begin(const game_options* options)
{
	if (random_seed_debug_file_open)
	{
		file_close(&random_seed_debug_file);
		random_seed_debug_file_open = false;
	}
	ASSERT(!random_seed_debug_file_open);

	if (options->game_mode > _game_mode_multiplayer)
	{
		char date_and_time[256]{};
		system_get_date_and_time(date_and_time, sizeof(date_and_time), true);

		char scenario_name[8]{};
		csstrnzcpy(scenario_name, tag_name_strip_path(options->scenario_path.get_string()), 8);

		char rand_filename[256]{};
		csnzprintf(rand_filename, sizeof(rand_filename), "rand_%s_%s_m%d.txt", scenario_name, date_and_time, options->dump_machine_index);
		if (create_report_file_reference(&random_seed_debug_file, rand_filename, true) && file_create(&random_seed_debug_file))
		{
			uns32 file_open_flags = FLAG(_file_open_flag_desired_access_write);
			file_open_flags |= FLAG(_file_open_flag_set_file_end_and_close);
			file_open_flags |= FLAG(_file_open_flag_share_mode_read);

			uns32 error = 0;
			random_seed_debug_file_open = file_open(&random_seed_debug_file, file_open_flags, &error);
		}

		if (!random_seed_debug_file_open)
			event(_event_error, "couldn't create random seed debug file %s", rand_filename);

		if (random_seed_debug_file_open)
			file_printf(&random_seed_debug_file, "code reference function: %s at address: %x\n", "random_math_initialize", random_math_initialize);
	}
}

void random_seed_debug_log_end()
{
	if (random_seed_debug_file_open)
	{
		file_close(&random_seed_debug_file);
		random_seed_debug_file_open = false;
	}
}

