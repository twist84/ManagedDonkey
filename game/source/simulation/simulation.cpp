#include "simulation/simulation.hpp"

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/game_engine.hpp"
#include "main/console.hpp"
#include "main/main.hpp"
#include "main/main_game.hpp"
#include "math/random_math.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/logic/network_life_cycle.hpp"
#include "networking/network_memory.hpp"
#include "profiler/profiler.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "simulation/game_interface/simulation_game_interface.hpp"
#include "simulation/simulation_encoding.hpp"
#include "simulation/simulation_gamestate_entities.hpp"
#include "simulation/simulation_type_collection.hpp"
#include "simulation/simulation_world.hpp"

REFERENCE_DECLARE(0x019A9FA0, s_simulation_globals, simulation_globals);

HOOK_DECLARE(0x004411E0, simulation_describe_status);
HOOK_DECLARE(0x00441230, simulation_dispose);
HOOK_DECLARE(0x00441270, simulation_dispose_from_old_map);
HOOK_DECLARE(0x00441320, simulation_film_record_update);
HOOK_DECLARE(0x00441330, simulation_film_retrieve_updates);
HOOK_DECLARE(0x004413B0, simulation_film_start_recording);
HOOK_DECLARE(0x00441410, simulation_film_stop_recording);
HOOK_DECLARE(0x004419C0, simulation_initialize);
HOOK_DECLARE(0x00441A40, simulation_initialize_for_new_map);
HOOK_DECLARE(0x00441CD0, simulation_must_close_saved_film);
HOOK_DECLARE(0x00441D30, simulation_notify_initial_core_load);
HOOK_DECLARE(0x00441D50, simulation_notify_players_created);
HOOK_DECLARE(0x00441DD0, simulation_notify_revert);
HOOK_DECLARE(0x00441DF0, simulation_notify_saved_film_ended);
HOOK_DECLARE(0x00441E10, simulation_notify_saved_film_revert);
HOOK_DECLARE(0x00441F90, simulation_record_update);
HOOK_DECLARE(0x00442080, simulation_saved_film_revert);
HOOK_DECLARE(0x004421B0, simulation_update);
HOOK_DECLARE(0x004426F0, simulation_update_pregame);

int32 g_simulation_bandwidth_eater = 0;
bool g_simulation_disable_replicated_aim_assist = false;

bool g_simulation_status_world = false;
s_status_line g_simulation_world_status_line;

bool g_simulation_status_views = false;
s_status_line g_simulation_view_status_lines[k_simulation_world_maximum_views];
s_status_line g_simulation_view_send_status_lines[k_simulation_world_maximum_views];

c_wait_for_render_thread::c_wait_for_render_thread(const char* file, int32 line) :
	m_flags(_internal_halt_render_thread_and_lock_resources(file, line))
{
}

c_wait_for_render_thread::~c_wait_for_render_thread()
{
	unlock_resources_and_resume_render_thread(m_flags);
}


//.text:00440A50 ; 
//.text:00440A60 ; 

int32 c_simulation_world::get_next_update_number() const
{
	return INVOKE_CLASS_MEMBER(0x00440A70, c_simulation_world, get_next_update_number);
}

//.text:00440A80 ; 
//.text:00440A90 ; 
//.text:00440AA0 ; 

e_simulation_world_state c_simulation_world::get_state() const
{
	return INVOKE_CLASS_MEMBER(0x00440AB0, c_simulation_world, get_state);
}

int32 c_simulation_world::get_time() const
{
	return INVOKE_CLASS_MEMBER(0x00440AC0, c_simulation_world, get_time);
}

//.text:00440AD0 ; 

e_simulation_world_type c_simulation_world::get_world_type() const
{
	return INVOKE_CLASS_MEMBER(0x00440AE0, c_simulation_world, get_world_type);
}

bool c_simulation_world::is_active() const
{
	return INVOKE_CLASS_MEMBER(0x00440AF0, c_simulation_world, is_out_of_sync);

	//ASSERT(exists());
	//
	//return m_world_state == _simulation_world_state_active;
}

bool c_simulation_world::is_authority() const
{
	return INVOKE_CLASS_MEMBER(0x00440B00, c_simulation_world, is_authority);

	//ASSERT(exists());
	//
	//bool is_client = m_world_type == _simulation_world_type_synchronous_game_client
	//	|| m_world_type == _simulation_world_type_synchronous_playback_client
	//	|| m_world_type == _simulation_world_type_distributed_client;
	//return !is_client;
}

bool c_simulation_world::is_distributed() const
{
	return INVOKE_CLASS_MEMBER(0x00440B20, c_simulation_world, is_distributed);

	//ASSERT(exists());
	//
	//return m_world_type == _simulation_world_type_distributed_authority || m_world_type == _simulation_world_type_distributed_client;
}

bool c_simulation_world::is_local() const
{
	return INVOKE_CLASS_MEMBER(0x00440B40, c_simulation_world, is_authority);

	//ASSERT(exists());
	//
	//bool is_local = m_world_type == _simulation_world_type_local || m_world_type == _simulation_world_type_local_playback;
	//ASSERT(!is_local || m_view_count == 0);
	//
	//return is_local;
}

bool c_simulation_world::is_out_of_sync() const
{
	return INVOKE_CLASS_MEMBER(0x00440B60, c_simulation_world, is_out_of_sync);
}

bool c_simulation_world::is_playback() const
{
	return INVOKE_CLASS_MEMBER(0x00440B90, c_simulation_world, is_playback);
}

bool c_simulation_world::runs_simulation() const
{
	return INVOKE_CLASS_MEMBER(0x00440C30, c_simulation_world, runs_simulation);
}

void __cdecl simulation_abort_immediate(e_simulation_abort_reason abort_reason)
{
	INVOKE(0x00440D70, simulation_abort_immediate, abort_reason);
}

bool __cdecl simulation_aborted()
{
	//return INVOKE(0x00440DD0, simulation_aborted);

	return simulation_globals.initialized && simulation_globals.simulation_aborted;
}

void __cdecl simulation_add_view_to_world(e_simulation_view_type view_type, const s_machine_identifier* remote_machine_identifier, int32 remote_machine_index, const char* remote_machine_name)
{
	INVOKE(0x00440DF0, simulation_add_view_to_world, view_type, remote_machine_identifier, remote_machine_index, remote_machine_name);
}

void __cdecl simulation_apply_after_game(const struct simulation_update* update)
{
	INVOKE(0x00440E40, simulation_apply_after_game, update);
}

void __cdecl simulation_apply_before_game(const struct simulation_update* update)
{
	INVOKE(0x00440E50, simulation_apply_before_game, update);

	//PROFILER(simulation_apply_before_game)
	//{
	//	PROFILER(simulation)
	//	{
	//		ASSERT(update);
	//		ASSERT(simulation_globals.initialized);
	//		ASSERT(simulation_globals.world);
	//		ASSERT(game_in_progress());
	//
	//		for (int32 actor_index = 0; actor_index < k_maximum_players; actor_index++)
	//		{
	//			if (!TEST_BIT(update->valid_actor_mask, actor_index))
	//			{
	//				continue;
	//			}
	//
	//			if (!UNIT_GET(update->actor_unit_indices[actor_index]))
	//			{
	//				continue;
	//			}
	//
	//			unit_control(update->actor_unit_indices[actor_index], &update->actor_control[actor_index]);
	//		}
	//
	//		if (update->machine_update_exists)
	//		{
	//			players_set_machines(update->machine_update.machine_valid_mask, update->machine_update.machine_identifiers);
	//		}
	//
	//		c_data_iterator<player_datum> player_iterator;
	//		player_iterator.begin(*player_data);
	//		while (player_iterator.next())
	//		{
	//			if (player_iterator.get_datum()->unit_index != NONE)
	//			{
	//				int32 player_index = player_iterator.get_index();
	//				int32 player_absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index);
	//				if ((update->valid_player_prediction_mask & (1 << player_index)) != 0)
	//				{
	//					simulation_game_process_player_prediction(player_index, &update->player_prediction[player_absolute_index]);
	//				}
	//			}
	//		}
	//
	//		simulation_globals.world->apply_simulation_queue(&update->bookkeeping_simulation_queue);
	//
	//		int32 resource_lock = 0;
	//		if (update->game_simulation_queue.queued_count() <= 0)
	//		{
	//			ASSERT(update->game_simulation_queue.queued_size_in_bytes() == 0);
	//		}
	//		else
	//		{
	//			ASSERT(update->game_simulation_queue.queued_size_in_bytes() > 0);
	//			ASSERT(update->flags.test(_simulation_update_simulation_in_progress_bit) || update->flags.test(_simulation_update_game_simulation_queue_requires_application_bit));
	//
	//			if (!update->flags.test(_simulation_update_simulation_in_progress_bit) && !game_is_playback())
	//			{
	//				resource_lock = tag_resources_lock_game();
	//			}
	//
	//			simulation_globals.world->apply_simulation_queue(&update->game_simulation_queue);
	//
	//			if (!update->flags.test(_simulation_update_simulation_in_progress_bit))
	//			{
	//				objects_purge_deleted_objects();
	//				if (!game_is_playback())
	//				{
	//					tag_resources_unlock_game(resource_lock);
	//				}
	//			}
	//		}
	//
	//		if (update->flags.test(_simulation_update_flush_gamestate_bit) && !simulation_globals.world->is_authority())
	//		{
	//			ASSERT(!game_is_distributed());
	//			simulation_globals.world->gamestate_flush();
	//		}
	//	}
	//}
}

bool __cdecl simulation_boot_machine(const s_machine_identifier* machine, e_network_session_boot_reason boot_reason)
{
	return INVOKE(0x00440FE0, simulation_boot_machine, machine, boot_reason);
}

void __cdecl simulation_build_machine_update(bool* machine_update_valid, simulation_machine_update* machine_update)
{
	INVOKE(0x00441010, simulation_build_machine_update, machine_update_valid, machine_update);
}

void __cdecl simulation_build_player_updates()
{
	INVOKE(0x00441030, simulation_build_player_updates);
}

void __cdecl simulation_build_update(bool should_build, struct simulation_update* update, s_simulation_update_metadata* metadata)
{
	INVOKE(0x004410E0, simulation_build_update, should_build, update, metadata);

	//PROFILER(simulation_build_update)
	//{
	//	PROFILER(simulation)
	//	{
	//		ASSERT(simulation_globals.initialized);
	//		ASSERT(simulation_globals.world);
	//		VASSERT(simulation_globals.simulation_aborted, "simulation aborted inside game update!");
	//		ASSERT(simulation_globals.world->exists());
	//		ASSERT(game_in_progress());
	//		ASSERT(update);
	//		ASSERT(metadata);
	//
	//		csmemset(update, 0, sizeof(struct simulation_update));
	//		csmemset(metadata, 0, sizeof(struct s_simulation_update_metadata));
	//
	//		saved_film_manager_update_before_simulation_update();
	//		simulation_globals.world->build_update(should_build, update, metadata);
	//
	//		if ((!simulation_globals.world->is_authority() || simulation_globals.world->is_playback()) &&
	//			(!simulation_globals.world->is_distributed() || simulation_globals.world->is_playback()) &&
	//			!simulation_globals.world->is_out_of_sync())
	//		{
	//			bool should_go_out_of_sync = false;
	//
	//			if (update->flags.test(_simulation_update_gamestate_flushed_outside_game_tick_bit))
	//			{
	//				simulation_globals.world->gamestate_flush();
	//			}
	//
	//			if (update->update_number == simulation_globals.world->get_next_update_number())
	//			{
	//				if (update->verify_game_time == simulation_globals.world->get_time())
	//				{
	//					if (update->verify_random != get_random_seed())
	//					{
	//						event(_event_error, "networking:simulation:global: OUT OF SYNC, random seed differs, update [#%d] time [%d] seed [0x%08X] (local seed [0x%08X])",
	//							update->update_number,
	//							update->verify_game_time,
	//							update->verify_random,
	//							get_random_seed());
	//
	//						should_go_out_of_sync = true;
	//					}
	//				}
	//				else
	//				{
	//					event(_event_error, "networking:simulation:global: OUT OF SYNC, update time differs, update [#%d] time [%d] != local time %d",
	//						update->update_number,
	//						update->verify_game_time,
	//						simulation_globals.world->get_time());
	//
	//					should_go_out_of_sync = true;
	//				}
	//			}
	//			else
	//			{
	//				event(_event_error, "networking:simulation:global: OUT OF SYNC, update number differs, update [#%d] != next [#%d]",
	//					update->update_number,
	//					simulation_globals.world->get_next_update_number());
	//
	//				should_go_out_of_sync = true;
	//			}
	//
	//			saved_film_history_notify_determinism_manager_about_to_generate_checksum();
	//			determinism_debug_manager_generate_game_state_checksum(&determinism_verification);
	//
	//			if (!determinism_debug_manager_compare_game_state_checksum(&update->determinism_verification, &determinism_verification))
	//			{
	//				should_go_out_of_sync = true;
	//			}
	//
	//			if (net_allow_out_of_sync)
	//			{
	//				should_go_out_of_sync = true;
	//			}
	//
	//			if (should_go_out_of_sync)
	//			{
	//				determinism_debug_manager_notify_out_of_sync();
	//			}
	//
	//			if (should_go_out_of_sync)
	//			{
	//				simulation_globals.world->go_out_of_sync();
	//				saved_film_history_notify_out_of_sync();
	//			}
	//		}
	//		saved_film_manager_update_after_simulation_update(update, metadata);
	//	}
	//}
}

void __cdecl simulation_clear_errors()
{
	INVOKE(0x004411C0, simulation_clear_errors);
}

//.text:004411D0 ; 

void __cdecl simulation_describe_status(char* buffer, int32 buffer_size)
{
	//INVOKE(0x004411E0, simulation_describe_status, buffer, buffer_size);

	if (simulation_globals.initialized)
	{
		ASSERT(simulation_globals.watcher);
		simulation_globals.watcher->describe_status(buffer, buffer_size);
	}
	else
	{
		csstrnzcpy(buffer, "", buffer_size);
	}
}

void __cdecl simulation_destroy_update(struct simulation_update* update)
{
	INVOKE(0x00441220, simulation_destroy_update, update);

	//c_simulation_world::destroy_update(update);
}

void __cdecl simulation_dispose()
{
	//INVOKE(0x00441230, simulation_dispose);

	simulation_gamestate_entities_dispose();
	
	if (!simulation_globals.initialized)
	{
		return;
	}
	
	simulation_globals.world = NULL;
	simulation_globals.watcher = NULL;
	simulation_globals.type_collection = NULL;
	simulation_globals.initialized = false;
}

void __cdecl simulation_dispose_from_old_map()
{
	//INVOKE(0x00441270, simulation_dispose_from_old_map);

	if (main_game_reset_in_progress())
	{
		return;
	}
	
	simulation_globals.watcher->destroy_watcher();
	simulation_globals.world->destroy_world();
	game_results_dispose_from_old_map();
	simulation_gamestate_entities_dispose_from_old_map();
	simulation_film_stop_recording();
	simulation_globals.simulation_reset_in_progress = false;
}

void __cdecl simulation_end(e_simulation_abort_reason abort_reason)
{
	INVOKE(0x004412B0, simulation_end, abort_reason);
}

void __cdecl simulation_fatal_error()
{
	INVOKE(0x00441310, simulation_fatal_error);
}

bool __cdecl simulation_film_record_update(const struct simulation_update* update)
{
	//return INVOKE(0x00441320, simulation_film_record_update, update);

	ASSERT(update);
	
	if (!simulation_globals.recording_film)
	{
		return false;
	}
	
	if (!saved_film_manager_write_simulation_update(update))
	{
		event(_event_error, "networking:simulation:saved_film: failed to write update #%d to film", update->update_number);
		simulation_film_stop_recording();
		return false;
	}
	
	return true;
}

bool __cdecl simulation_film_retrieve_updates(int32 maximum_updates_to_read, int32* updates_read_out)
{
	//return INVOKE(0x00441330, simulation_film_retrieve_updates, maximum_updates_to_read, updates_read_out);

	ASSERT(updates_read_out);
	
	real32 speed = game_time_get_speed();
	int32 target_updates = game_seconds_to_ticks_round(speed * 0.25f);
	if (target_updates < 1)
	{
		target_updates = 1;
	}
	if (target_updates > maximum_updates_to_read)
	{
		target_updates = maximum_updates_to_read;
	}
	
	int32 updates_read = 0;
	
	for (int32 i = 0; i < 30; i++)
	{
		int32 updates_available = 0;
		simulation_globals.world->time_get_available(NULL, &updates_available);
	
		if (updates_available >= target_updates)
		{
			break;
		}
	
		int32 position = saved_film_manager_get_position();
	
		s_saved_film_update update_out{};
		if (!saved_film_manager_read_update(&update_out))
		{
			event(_event_warning,
				"networking:simulation:saved_film: failed to read update at time time %d",
				game_time_get());
			return false;
		}
	
		if (update_out.update_type != _saved_film_update_type_simulation_update)
		{
			event(_event_error,
				"networking:simulation:saved_film: failed to handle update type %d from film",
				update_out.update_type);
			return false;
		}
	
		struct simulation_update simulation_update_out{};
		s_simulation_update_metadata metadata{};
		metadata.flags.set(_simulation_update_from_saved_film_bit, true);
		metadata.saved_film_position = position;
		metadata.saved_film_tick = saved_film_manager_get_current_tick();
	
		if (!saved_film_manager_read_simulation_update(&update_out, &simulation_update_out))
		{
			event(_event_warning,
				"networking:simulation:saved_film: failed to read update at time time %d",
				game_time_get());
			continue;
		}
	
		if (!simulation_globals.world->handle_playback_update(&simulation_update_out, &metadata))
		{
			event(_event_error,
				"networking:simulation:saved_film: update #%d failed insert into world at time %d",
				simulation_update_out.update_number,
				game_time_get());
	
			c_simulation_world::destroy_update(&simulation_update_out);
			continue;
		}
	
		updates_read++;
	}
	
	*updates_read_out = updates_read;
	
	return true;
}

bool __cdecl simulation_film_start_recording()
{
	//return INVOKE(0x004413B0, simulation_film_start_recording);

	if (simulation_globals.recording_film)
	{
		event(_event_warning, "networking:simulation: attempting to start recording a film while we have one already open?");
		simulation_film_stop_recording();
	}
	
	const game_options* options = game_options_get();
	if (!options->record_saved_film)
	{
		return false;
	}
	
	c_static_string<260> game_description{};
	simulation_get_game_description(&game_description);
	
	ASSERT(!simulation_globals.recording_film);
	simulation_globals.recording_film = saved_film_manager_open_film_for_writing(game_description.get_string(), options);
	if (!simulation_globals.recording_film)
	{
		saved_film_manager_close();
		event(_event_warning, "saved_film: unable to open saved film for recording");
		return false;
	}
	
	return true;
}

void __cdecl simulation_film_stop_recording()
{
	//INVOKE(0x00441410, simulation_film_stop_recording);

	if (!simulation_globals.recording_film)
	{
		return;
	}
	
	saved_film_manager_close();
	simulation_globals.recording_film = false;
	saved_film_manager_copy_film_to_debug_path();
}

bool __cdecl simulation_format_player_netdebug_data(int32 player_index, const s_simulation_player_netdebug_data* netdebug_data, int32* filled_bar_count)
{
	return INVOKE(0x00441420, simulation_format_player_netdebug_data, player_index, netdebug_data, filled_bar_count);
}

const char* __cdecl simulation_get_abort_reason_string()
{
	return INVOKE(0x00441580, simulation_get_abort_reason_string);
}

e_simulation_abort_reason __cdecl simulation_get_aborted_reason()
{
	return INVOKE(0x004415B0, simulation_get_aborted_reason);

	//return simulation_globals.simulation_aborted_reason;
}

void __cdecl simulation_get_game_description(c_static_string<260>* game_description)
{
	INVOKE(0x004415C0, simulation_get_game_description, game_description);
}

bool __cdecl simulation_get_machine_active_in_game(const s_machine_identifier* machine)
{
	return INVOKE(0x00441630, simulation_get_machine_active_in_game, machine);
}

bool __cdecl simulation_get_machine_bandwidth_events(const s_machine_identifier* machine, int32 bandwidth_event_type_count, int32* bandwidth_event_counters)
{
	return INVOKE(0x004416B0, simulation_get_machine_bandwidth_events, machine, bandwidth_event_type_count, bandwidth_event_counters);
}

bool __cdecl simulation_get_machine_connectivity(const s_machine_identifier* machine)
{
	return INVOKE(0x00441700, simulation_get_machine_connectivity, machine);

	//return simulation_globals.watcher->get_machine_connectivity(machine);
}

bool __cdecl simulation_get_machine_is_host(const s_machine_identifier* machine)
{
	return INVOKE(0x00441720, simulation_get_machine_is_host, machine);

	//return simulation_globals.watcher->get_machine_is_host(machine);
}

uns32 __cdecl simulation_get_network_time_since_abort()
{
	return INVOKE(0x00441740, simulation_get_network_time_since_abort);
}

int32 __cdecl simulation_get_player_netdebug_filled_bar_count(int32 player_index)
{
	//return INVOKE(0x00441780, simulation_get_player_netdebug_filled_bar_count, player_index);

	int32 temp_result = NONE;
	const s_simulation_player_netdebug_data* netdebug_data = game_engine_get_player_netdebug_data(player_index);
	if (!simulation_format_player_netdebug_data(player_index, netdebug_data, &temp_result))
	{
		return NONE;
	}
	return temp_result;
}

c_simulation_view* __cdecl simulation_get_remote_view_by_channel(c_network_channel* channel)
{
	return INVOKE(0x004417C0, simulation_get_remote_view_by_channel, channel);
}

const char* simulation_get_starting_up_description()
{
	//return INVOKE(0x004417F0, simulation_get_starting_up_description);

	return simulation_starting_up() ? simulation_globals.status_buffer : "";
}

//enum e_simulation_status __cdecl simulation_get_status(void)
int32 __cdecl simulation_get_status()
{
	return INVOKE(0x00441830, simulation_get_status);

	//if (simulation_globals.initialized)
	//{
	//	return 0;
	//}
	//
	//ASSERT(simulation_globals.watcher);
	//return simulation_globals.watcher->describe_status_simple();
}

c_simulation_type_collection* __cdecl simulation_get_type_collection()
{
	return INVOKE(0x00441850, simulation_get_type_collection);

	//ASSERT(simulation_globals.initialized);
	//ASSERT(simulation_globals.type_collection);
	//return simulation_globals.type_collection;
}

bool __cdecl simulation_get_view_netdebug_data(const c_simulation_view* view, int32* rtt_msec, int32* packet_rate, int32* bandwidth_bps, int32* packet_loss)
{
	return INVOKE(0x00441860, simulation_get_view_netdebug_data, view, rtt_msec, packet_rate, bandwidth_bps, packet_loss);
}

c_simulation_world* __cdecl simulation_get_world()
{
	return INVOKE(0x00441940, simulation_get_world);

	//ASSERT(simulation_globals.initialized);
	//ASSERT(simulation_globals.world);
	//ASSERT(simulation_globals.world->exists());
	//return simulation_globals.world;
}

c_simulation_world* __cdecl simulation_get_world_if_exists()
{
	return INVOKE(0x00441950, simulation_get_world_if_exists);

	//if (!simulation_globals.initialized || !simulation_globals.world)
	//{
	//	return NULL;
	//}
	//
	//if (simulation_globals.world->m_world_type == _simulation_world_type_none)
	//{
	//	return NULL;
	//}
	//
	//return simulation_globals.world;
}

bool __cdecl simulation_in_progress()
{
	return INVOKE(0x00441970, simulation_in_progress);
}

void __cdecl simulation_initialize()
{
	//INVOKE(0x004419C0, simulation_initialize);

	ASSERT(!simulation_globals.initialized);

	simulation_gamestate_entities_initialize();
	if (network_memory_simulation_initialize(&simulation_globals.world, &simulation_globals.watcher, &simulation_globals.type_collection))
	{
		ASSERT(simulation_globals.world);
		ASSERT(simulation_globals.watcher);
		ASSERT(simulation_globals.type_collection);

		simulation_globals.type_collection->clear_types();

		int32 entity_type_count, event_type_count;
		simulation_game_register_types(simulation_globals.type_collection, &entity_type_count, &event_type_count);
		simulation_globals.type_collection->finish_types(entity_type_count, event_type_count);

		status_lines_initialize(&g_simulation_world_status_line, &g_simulation_status_world, 1);
		for (int32 view_index = 0; view_index < k_simulation_world_maximum_views; view_index++)
		{
			status_lines_initialize(&g_simulation_view_status_lines[view_index], &g_simulation_status_views, 1);
			status_lines_initialize(&g_simulation_view_send_status_lines[view_index], &g_simulation_status_views, 1);
		}

		simulation_globals.simulation_reset_in_progress = false;
		simulation_globals.recording_film = false;
		simulation_globals.initialized = true;
	}
}

void __cdecl simulation_initialize_for_new_map()
{
	//INVOKE(0x00441A40, simulation_initialize_for_new_map);

	ASSERT(simulation_globals.initialized);
	
	if (main_game_reset_in_progress())
	{
		return;
	}
	
	c_simulation_distributed_world* distributed_world = NULL;
	simulation_gamestate_entities_initialize_for_new_map();
	simulation_globals.simulation_fatal_error = false;
	simulation_globals.simulation_deferred = false;
	simulation_globals.simulation_aborted = false;
	simulation_globals.handled_determinism_failure = false;
	simulation_globals.performed_main_save_and_exit_campaign_immediately_this_map = false;
	simulation_film_start_recording();
	
	if (game_is_distributed() && !game_is_playback())
	{
		distributed_world = network_allocate_simulation_distributed_world();
	}
	
	ASSERT(simulation_globals.world);
	simulation_globals.world->initialize_world(
		game_simulation_get(),
		game_playback_get(),
		true,
		simulation_globals.type_collection,
		simulation_globals.watcher,
		distributed_world);
	
	ASSERT(simulation_globals.world->exists());
	ASSERT(simulation_globals.watcher);
	simulation_globals.watcher->initialize_watcher(simulation_globals.world);
	simulation_globals.watcher->setup_connection();
	game_results_initialize_for_new_map();
	simulation_globals.simulation_in_initial_state = true;
}

void __cdecl simulation_initialize_for_saved_game(int32 game_state_proc_flags)
{
	INVOKE(0x00441B30, simulation_initialize_for_saved_game, game_state_proc_flags);
}

void __cdecl simulation_invalidate()
{
	INVOKE(0x00441CC0, simulation_invalidate);

	//simulation_globals.simulation_in_initial_state = false;
}

void __cdecl simulation_must_close_saved_film()
{
	//INVOKE(0x00441CD0, simulation_must_close_saved_film);

	simulation_globals.must_close_saved_film = true;
}

void __cdecl simulation_notify_channel_closure(void* simulation_context)
{
	INVOKE(0x00441CE0, simulation_notify_channel_closure, simulation_context);

	//c_simulation_view* view = (c_simulation_view*)simulation_context;
	//ASSERT(view != NULL);
	//view->notify_closed();
}

void __cdecl simulation_notify_core_save()
{
	//INVOKE(0x00441CF0, simulation_notify_core_save);

	if (!game_in_progress())
	{
		return;
	}
	
	ASSERT(simulation_globals.world);
	simulation_globals.world->notify_gamestate_flush_outside_game_tick();
}

void __cdecl simulation_notify_going_active()
{
	INVOKE(0x00441D10, simulation_notify_going_active);

	//if (!game_is_campaign() || !game_is_cooperative())
	//{
	//	return;
	//}
	//
	//players_update_for_checkpoint();
}

void __cdecl simulation_notify_initial_core_load(int32 update_number)
{
	//INVOKE(0x00441D30, simulation_notify_initial_core_load, update_number);

	if (!game_in_progress())
	{
		return;
	}
	
	ASSERT(simulation_globals.world);
	simulation_globals.world->notify_initial_gamestate_load(update_number);
}

void __cdecl simulation_notify_players_created()
{
	//INVOKE(0x00441D50, simulation_notify_players_created);

	if (!simulation_globals.initialized)
	{
		return;
	}
	
	ASSERT(simulation_globals.watcher);
	
	if (simulation_globals.gamestate_load_in_progress)
	{
		return;
	}
	
	if (simulation_globals.world->is_playback())
	{
		return;
	}
	
	simulation_globals.watcher->handle_player_creation();
}

void __cdecl simulation_notify_reset_complete()
{
	INVOKE(0x00441D90, simulation_notify_reset_complete);
}

void __cdecl simulation_notify_reset_initiate()
{
	INVOKE(0x00441DC0, simulation_notify_reset_initiate);

	//simulation_globals.simulation_reset_in_progress = true;
}

void __cdecl simulation_notify_revert()
{
	//INVOKE(0x00441DD0, simulation_notify_revert);

	if (!game_in_progress())
	{
		return;
	}
	
	ASSERT(simulation_globals.watcher);
	simulation_globals.watcher->notify_game_revert();
}

void __cdecl simulation_notify_saved_film_ended()
{
	//INVOKE(0x00441DF0, simulation_notify_saved_film_ended);

	if (!game_in_progress())
	{
		return;
	}
	
	ASSERT(simulation_globals.world);
	
	simulation_globals.world->notify_playback_control(_network_synchronous_playback_control_end_playback, NONE, NONE);
}

void __cdecl simulation_notify_saved_film_revert(int32 history_record_index, int32 next_update_number)
{
	//INVOKE(0x00441E10, simulation_notify_saved_film_revert, history_record_index, next_update_number);

	if (!game_in_progress())
	{
		return;
	}
	
	ASSERT(simulation_globals.world);
	
	simulation_globals.world->notify_playback_control(_network_synchronous_playback_control_revert, history_record_index, next_update_number);
}

bool __cdecl simulation_performed_main_save_and_exit_campaign_immediately_this_map()
{
	return INVOKE(0x00441E40, simulation_performed_main_save_and_exit_campaign_immediately_this_map);
}

void __cdecl simulation_player_joined_game(int32 player_index)
{
	INVOKE(0x00441E50, simulation_player_joined_game, player_index);

	//if (!simulation_globals.initialized)
	//{
	//	return;
	//}
	//
	//ASSERT(simulation_globals.world);
	//
	//if (simulation_globals.gamestate_load_in_progress)
	//{
	//	return;
	//}
	//
	//if (simulation_globals.world->is_playback())
	//{
	//	return;
	//}
	//
	//simulation_globals.world->create_player(player_index, game_simulation_get());
}

void __cdecl simulation_player_left_game(int32 player_index)
{
	INVOKE(0x00441EA0, simulation_player_left_game, player_index);
}

void __cdecl simulation_prepare_to_load_saved_game(int32 game_state_proc_flags)
{
	INVOKE(0x00441EF0, simulation_prepare_to_load_saved_game, game_state_proc_flags);

	//ASSERT(simulation_globals.initialized);
	//simulation_globals.gamestate_load_in_progress = true;
}

void __cdecl simulation_prepare_to_send()
{
	INVOKE(0x00441F00, simulation_prepare_to_send);

	//ASSERT(simulation_globals.initialized);
	//ASSERT(simulation_globals.world);
	//
	//if (simulation_globals.simulation_aborted || simulation_globals.world->m_world_type == _simulation_world_type_none || !game_in_progress())
	//{
	//	return;
	//}
	//
	//simulation_globals.world->process_pending_updates();
}

void __cdecl simulation_process_actor_control(int32 simulation_actor_index, const unit_control_data* actor_control)
{
	INVOKE(0x00441F30, simulation_process_actor_control, simulation_actor_index, actor_control);

	//ASSERT(simulation_globals.initialized);
	//ASSERT(simulation_globals.world);
	//ASSERT(game_in_progress());
	//
	//if (simulation_globals.simulation_aborted)
	//{
	//	return;
	//}
	//
	//if (simulation_globals.world->m_world_type == _simulation_world_type_none)
	//{
	//	return;
	//}
	//
	//simulation_globals.world->process_actor_control(simulation_actor_index, actor_control);
}

void __cdecl simulation_process_input(uns32 user_action_mask, const player_action* user_actions)
{
	INVOKE(0x00441F60, simulation_process_input, user_action_mask, user_actions);

	//ASSERT(simulation_globals.initialized);
	//ASSERT(simulation_globals.world);
	//ASSERT(game_in_progress());
	//
	//if (simulation_globals.simulation_aborted)
	//{
	//	return;
	//}
	//
	//if (simulation_globals.world->m_world_type == _simulation_world_type_none)
	//{
	//	return;
	//}
	//
	//simulation_globals.world->process_input(user_action_mask, user_actions);
}

void __cdecl simulation_record_update(const struct simulation_update* update)
{
	//INVOKE(0x00441F90, simulation_record_update, update);

	ASSERT(update);
	ASSERT(simulation_globals.initialized);
	ASSERT(simulation_globals.world);
	ASSERT(game_in_progress());
	
	if (simulation_globals.recording_film)
	{
		simulation_film_record_update(update);
	}
}

void __cdecl simulation_remove_view_from_world(c_simulation_view* view)
{
	INVOKE(0x00441FA0, simulation_remove_view_from_world, view);
}

void __cdecl simulation_reset()
{
	INVOKE(0x00441FE0, simulation_reset);

	//ASSERT(simulation_globals.world);
	//ASSERT(simulation_globals.world->is_authority());
	//if (simulation_globals.simulation_in_initial_state)
	//{
	//	simulation_globals.simulation_in_initial_state = false;
	//}
	//else
	//{
	//	simulation_reset_immediate();
	//}
}

bool __cdecl simulation_reset_in_progress()
{
	return INVOKE(0x00442070, simulation_reset_in_progress);

	//return simulation_globals.simulation_reset_in_progress;
}

void __cdecl simulation_saved_film_revert(int32 history_record_index, int32 next_update_number)
{
	//INVOKE(0x00442080, simulation_saved_film_revert, history_record_index, next_update_number);

	ASSERT(game_in_progress());
	ASSERT(game_is_playback());
	ASSERT(!game_is_authoritative_playback());
	saved_film_manager_request_revert_by_index(history_record_index);
	simulation_globals.simulation_deferred = true;
}

void __cdecl simulation_set_performed_main_save_and_exit_campaign_immediately_this_map(bool save_and_quit_performed)
{
	INVOKE(0x00442090, simulation_set_performed_main_save_and_exit_campaign_immediately_this_map, save_and_quit_performed);

	//simulation_globals.performed_main_save_and_exit_campaign_immediately_this_map = save_and_quit_performed;
}

bool __cdecl simulation_should_transmit_simulation_data()
{
	return INVOKE(0x004420A0, simulation_should_transmit_simulation_data);

	//return simulation_globals.initialized && game_in_progress();
}

void __cdecl simulation_start()
{
	INVOKE(0x004420C0, simulation_start);

	//ASSERT(game_in_progress());
	//
	//if (simulation_globals.initialized)
	//{
	//	ASSERT(simulation_globals.world);
	//
	//	if (!simulation_globals.world->attached_to_map())
	//	{
	//		simulation_globals.world->attach_to_map();
	//	}
	//}
}

bool __cdecl simulation_starting_up()
{
	//return INVOKE(0x004420E0, simulation_starting_up);

	bool starting_up = false;
	if (simulation_globals.initialized)
	{
		ASSERT(simulation_globals.world);

		if (!simulation_globals.simulation_aborted && simulation_globals.world->exists())
		{
			starting_up = !simulation_globals.world->is_active();
		}
	}
	return starting_up;
}

void __cdecl simulation_status_lines_update()
{
	if (g_simulation_status_world && simulation_globals.initialized)
	{
		c_simulation_world* world = simulation_globals.world;
		ASSERT(world);

		s_status_line* line = &g_simulation_world_status_line;

		if (world->exists())
		{
			char textbuf[256]{};

			line->flags.set(_status_line_inhibit_drawing_bit, false);
			line->flags.set(_status_line_left_justify_bit, true);

			int32 world_type = world->get_world_type();
			csnzprintf(textbuf, NUMBEROF(textbuf), "%s",
				c_simulation_world::get_type_string(world_type));

			int32 world_state = world->get_state();
			switch (world_state)
			{
			case _simulation_world_state_joining:
			{

				int32 join_time_elapsed;
				int32 join_time_to_abort;
				int32 join_attempt_count;
				int32 join_attempt_maximum;
				int32 join_client_establishing_count;
				int32 join_client_waiting_count;
				int32 join_client_joining_count;
				int32 join_client_complete_count;
				int32 join_client_total_count;
				int32 join_time_to_failure;
				world->get_join_status(
					&join_time_elapsed,
					&join_time_to_abort,
					&join_attempt_count,
					&join_attempt_maximum,
					&join_client_establishing_count,
					&join_client_waiting_count,
					&join_client_joining_count,
					&join_client_complete_count,
					&join_client_total_count,
					&join_time_to_failure);

				csnzappendf(textbuf, NUMBEROF(textbuf), " %s@%d",
					c_simulation_world::get_state_string(_simulation_world_state_joining),
					join_time_elapsed);

				if (world->is_authority())
				{
					csnzappendf(textbuf, NUMBEROF(textbuf), " cli-%d/%d/%d/%d/%d",
						join_client_establishing_count,
						join_client_waiting_count,
						join_client_joining_count,
						join_client_complete_count,
						join_client_total_count);
				}
			}
			break;
			case _simulation_world_state_disconnected:
			{
				int32 time_elapsed;
				int32 time_to_failure;
				world->get_disconnected_status(&time_elapsed, &time_to_failure);
				csnzappendf(textbuf, NUMBEROF(textbuf), " %s@%d",
					c_simulation_world::get_state_string(_simulation_world_state_disconnected),
					time_elapsed);
			}
			break;
			default:
			{
				csnzappendf(textbuf, NUMBEROF(textbuf), " %s",
					c_simulation_world::get_state_string(world_state));
			}
			}

			{
				int32 failure_count, failure_timeout;
				if (world->get_failure_status(&failure_count, &failure_timeout))
				{
					csnzappendf(textbuf, NUMBEROF(textbuf), " fail-%d time-%d",
						failure_count,
						failure_timeout);
				}
			}

			{
				int32 view_count = world->get_view_count();
				csnzappendf(textbuf, NUMBEROF(textbuf), " %d view%s",
					view_count,
					view_count == 1 ? "" : "s");
			}

			if (!world->runs_simulation())
			{
				int32 next_update_dequeue, next_update_expected;
				world->get_update_queue_status(&next_update_dequeue, &next_update_expected);
				csnzappendf(textbuf, NUMBEROF(textbuf), " upd %d/%d",
					next_update_dequeue,
					next_update_expected);
			}
			
			if (world_type == _simulation_world_type_synchronous_game_client)
			{
				int32 gamestate_write_progress;
				world->get_synchronous_client_status(&gamestate_write_progress);
				if (gamestate_write_progress != NONE)
				{
					csnzappendf(textbuf, NUMBEROF(textbuf), " gs%.1fk", (real32)(gamestate_write_progress / 1024));
				}
			}

			g_simulation_world_status_line.text.print("world: %s", textbuf);
		}
		else
		{
			line->flags.set(_status_line_inhibit_drawing_bit, true);
		}
	}

	if (g_simulation_status_views && simulation_globals.initialized)
	{
		for (int32 view_index = 0; view_index < k_simulation_world_maximum_views; view_index++)
		{
			s_status_line* line = &g_simulation_view_status_lines[view_index];
			s_status_line* send_line = &g_simulation_view_send_status_lines[view_index];

			line->flags.set(_status_line_inhibit_drawing_bit, true);
			send_line->flags.set(_status_line_inhibit_drawing_bit, true);
		}

		if (simulation_globals.world->exists())
		{
			ASSERT(simulation_globals.world);

			s_simulation_world_view_iterator iterator{};
			c_simulation_world::iterator_begin(&iterator, NONE);

			c_simulation_view* view = NULL;
			while (simulation_globals.world->iterator_next(&iterator, &view))
			{
				int32 view_index = view->get_world_view_index();
				int32 view_type = view->view_type();

				ASSERT(VALID_INDEX(view_index, k_simulation_world_maximum_views));
				s_status_line* line = &g_simulation_view_status_lines[view_index];
				s_status_line* send_line = &g_simulation_view_send_status_lines[view_index];

				line->flags.set(_status_line_inhibit_drawing_bit, false);
				send_line->flags.set(_status_line_inhibit_drawing_bit, false);

				line->flags.set(_status_line_left_justify_bit, true);
				send_line->flags.set(_status_line_left_justify_bit, true);

				s_simulation_view_statistics statistics{};
				view->get_statistics(&statistics);

				char textbuf[256]{};
				csnzprintf(textbuf, NUMBEROF(textbuf), "events%d/%d/%d updates%d/%d creations%d/%d deletions%d/%d",
					c_simulation_view::get_type_string(view_type),
					view->get_view_establishment_mode(),
					view->get_view_establishment_identifier(),
					view->get_remote_establishment_mode(),
					view->get_remote_establishment_identifier());

				line->text.print("view %s: %s",
					view->get_view_description(),
					textbuf);

				send_line->text.print("events%d/%d/%d updates%d/%d creations%d/%d deletions%d/%d",
					statistics.event.events_pending,
					statistics.event.events_in_transit,
					statistics.event.events_sent,
					statistics.entity.updates_pending,
					statistics.entity.updates_sent,
					statistics.entity.creations_pending,
					statistics.entity.creations_sent,
					statistics.entity.deletions_pending,
					statistics.entity.deletions_sent);
			}
		}
	}
}

void __cdecl simulation_stop()
{
	INVOKE(0x00442110, simulation_stop);

	//ASSERT(game_in_progress());
	//
	//if (simulation_globals.initialized)
	//{
	//	ASSERT(simulation_globals.world);
	//	if (simulation_globals.world->attached_to_map() && !simulation_reset_in_progress() && !main_game_reset_in_progress())
	//	{
	//		simulation_globals.world->detach_from_map();
	//	}
	//}
}

void __cdecl simulation_test_update()
{
	for (int32 event_index = 0; event_index < g_simulation_bandwidth_eater; event_index++)
	{
		s_game_engine_event_data test_event{};
		game_engine_initialize_event(_game_engine_event_type_general, STRING_ID(game_engine, general_event_unused1), &test_event);
		simulation_action_multiplayer_event(&test_event);
	}
}

int32 __cdecl simulation_time_get_maximum_available(bool* match_remote_time)
{
	return INVOKE(0x00442150, simulation_time_get_maximum_available, match_remote_time);
}

void __cdecl simulation_update()
{
	//INVOKE(0x004421B0, simulation_update);

	if (!simulation_globals.initialized)
	{
		simulation_globals.simulation_in_online_networked_session = false;
	}
	else
	{
		if (simulation_globals.simulation_aborted && game_in_progress() && game_is_ui_shell())
		{
			event( _event_critical, "networking:simulation: simulation aborted in main menu [%s], reloading the main menu!",
				simulation_get_abort_reason_string());

			main_menu_launch_force();
		}

		if (simulation_globals.simulation_deferred)
		{
			simulation_globals.simulation_deferred = saved_film_manager_has_pending_global_state_change();
		}

		if (!simulation_globals.simulation_aborted && game_in_progress())
		{
			ASSERT(simulation_globals.world != NULL);
			ASSERT(simulation_globals.watcher != NULL);

			if (!simulation_globals.simulation_aborted)
			{
				if (simulation_globals.simulation_fatal_error)
				{
					event(_event_error, "networking:simulation: fatal error raised, aborting simulation");
					simulation_abort_immediate(_simulation_abort_reason_fatal_error);
				}
			}

			if (!simulation_globals.simulation_aborted)
			{
				if (!simulation_globals.watcher->maintain_connection())
				{
					event(_event_message, "networking:simulation: watcher connection lost, aborting simulation");
					simulation_abort_immediate(_simulation_abort_reason_lost_connection);
				}
			}

			if (!simulation_globals.simulation_aborted)
			{
				if (simulation_globals.world->is_playback() && saved_film_manager_playback_aborted())
				{
					event(_event_message, "networking:simulation: saved film playback aborted, aborting simulation");
					simulation_abort_immediate(_simulation_abort_reason_film_playback_error);
				}
			}

			if (!simulation_globals.simulation_aborted && !simulation_globals.simulation_deferred)
			{
				simulation_globals.world->update();
			}

			simulation_update_out_of_sync();

			if (!simulation_globals.simulation_aborted && !simulation_globals.simulation_deferred)
			{
				if (!simulation_globals.world->runs_simulation()
					&& simulation_globals.world->time_running()
					&& simulation_globals.world->is_active()
					&& !simulation_globals.world->is_out_of_sync())
				{
					simulation_globals.world->process_playback_events();
				}

				if (!simulation_globals.simulation_aborted
					&& !simulation_globals.simulation_deferred
					&& simulation_globals.world->is_playback()
					&& simulation_globals.world->is_authority()
					&& simulation_globals.world->time_running()
					&& simulation_globals.world->is_active()
					&& !simulation_globals.world->is_out_of_sync())
				{
					bool exit_saved_film_due_to_error = false;
					e_game_playback_type playback = game_playback_get();
					if (playback == _game_playback_film || playback == _game_playback_network_server)
					{
						int32 saved_film_ticks_remaining = saved_film_manager_get_ticks_remaining();
						if (saved_film_ticks_remaining > 0)
						{
							int32 updates_read = 0;
							if (!simulation_film_retrieve_updates(saved_film_ticks_remaining, &updates_read) && updates_read != saved_film_ticks_remaining)
							{
								event(_event_error, "networking:simulation: failed to read updates from saved film (should have had %d to read?)", saved_film_ticks_remaining);
								exit_saved_film_due_to_error = true;
							}
						}

						{
							real32 seconds = game_time_get_speed() * 0.25f;
							int32 updates_required = game_seconds_to_ticks_round(seconds);
							ASSERT(updates_required >= 0);

							if (updates_required > 0)
							{
								int32 update_queue_length = 0;
								simulation_globals.world->time_get_available(NULL, &update_queue_length);
								ASSERT(update_queue_length >= 0);

								if (saved_film_ticks_remaining && !update_queue_length)
								{
									event(_event_error, "networking:simulation: update queue empty and it should not be (film ticks remaming %d)", saved_film_ticks_remaining);
									exit_saved_film_due_to_error = true;
								}
							}
						}
					}
					else
					{
						event(_event_error, "networking:simulation: world is playback authority but game playback type is not");
						exit_saved_film_due_to_error = true;
					}

					if (exit_saved_film_due_to_error)
					{
						saved_film_manager_abort_playback(_saved_film_playback_abort_simulation_failed_to_read);
						if (network_life_cycle_get_state() == _life_cycle_state_none)
						{
							main_menu_launch();
						}
					}
				}
			}
		}

		if (simulation_starting_up())
		{
			simulation_describe_status(simulation_globals.status_buffer, NUMBEROF(simulation_globals.status_buffer));
		}

		bool in_online_networked_session = false;
		if (simulation_globals.initialized)
		{
			if (simulation_globals.simulation_aborted && simulation_globals.recording_film)
			{
				saved_film_manager_close();
				simulation_globals.recording_film = false;
				saved_film_manager_copy_film_to_debug_path();
			}

			if (simulation_globals.simulation_aborted)
			{
				if (simulation_globals.world->exists() && simulation_globals.world->is_playback() && saved_film_manager_is_reading())
				{
					saved_film_manager_close();
				}
			}

			if (simulation_globals.must_close_saved_film)
			{
				saved_film_manager_close();
				simulation_globals.must_close_saved_film = false;
			}

			in_online_networked_session = simulation_globals.watcher->in_online_networked_session();
		}
		simulation_globals.simulation_in_online_networked_session = in_online_networked_session;

		simulation_status_lines_update();
		simulation_test_update();
	}
}

void __cdecl simulation_update_aftermath(const struct simulation_update* update, s_simulation_update_metadata* metadata)
{
	INVOKE(0x004423F0, simulation_update_aftermath, update, metadata);

	//ASSERT(update);
	//ASSERT(metadata);
	//ASSERT(simulation_globals.initialized);
	//ASSERT(simulation_globals.world);
	//ASSERT(game_in_progress());
	//if (simulation_globals.world->is_authority())
	//{
	//	simulation_globals.world->distribute_update(update, metadata);
	//}
	//simulation_globals.world->advance_update(update);
}

void __cdecl simulation_update_out_of_sync()
{
	INVOKE(0x00442430, simulation_update_out_of_sync);
}

bool __cdecl simulation_update_player_netdebug_data(int32 player_index, s_simulation_player_netdebug_data* netdebug_data)
{
	return INVOKE(0x004424F0, simulation_update_player_netdebug_data, player_index, netdebug_data);
}

void __cdecl simulation_update_pregame()
{
	//INVOKE(0x004426F0, simulation_update_pregame);

	if (!simulation_globals.initialized
		|| !game_in_progress()
		|| !simulation_globals.watcher->need_to_generate_updates())
	{
		return;
	}
	
	struct simulation_update update{};
	s_simulation_update_metadata metadata{};
	simulation_build_update(false, &update, &metadata);
	ASSERT(!update.flags.test(_simulation_update_simulation_in_progress_bit));
	simulation_record_update(&update);
	random_seed_allow_use();
	damage_acceleration_queue_begin();
	simulation_apply_before_game(&update);
	damage_acceleration_queue_end();
	random_seed_disallow_use();
	simulation_update_aftermath(&update, &metadata);
	simulation_destroy_update(&update);
}

bool simulation_update_read_from_buffer(struct simulation_update* update, int32 buffer_size, const uns8* buffer)
{
	return INVOKE(0x004427C0, simulation_update_read_from_buffer, update, buffer_size, buffer);

	//ASSERT(update);
	//ASSERT(buffer);
	//c_bitstream decoded_update((byte*)buffer, buffer_size);
	//csmemset(update, 0, sizeof(struct simulation_update));
	//decoded_update.begin_reading();
	//if (!simulation_update_decode(&decoded_update, update))
	//{
	//	event(_event_warning, "networking:simulation: failed to read simulation update, decode failed");
	//	return false;
	//}
	//decoded_update.finish_reading();
	//return true;
}

bool __cdecl simulation_update_write_to_buffer(const struct simulation_update* update, int32 buffer_size, uns8* buffer, int32* out_update_length)
{
	return INVOKE(0x00442840, simulation_update_write_to_buffer, update, buffer_size, buffer, out_update_length);

	//ASSERT(update);
	//ASSERT(buffer);
	//ASSERT(out_update_length);
	//c_bitstream encoded_update((byte*)buffer, buffer_size);
	//encoded_update.begin_writing(1);
	//simulation_update_encode(&encoded_update, update);
	//encoded_update.finish_writing(NULL);
	//int32 space_used_in_bytes = encoded_update.get_space_used_in_bytes();
	//if (encoded_update.overflowed())
	//{
	//	event(_event_error, "networking:simulation: simulation update encode overflowed [size %d]", space_used_in_bytes);
	//	return false;
	//}
	//*out_update_length = space_used_in_bytes;
	//return true;
}

void simulation_debug_render()
{
	if (!simulation_globals.initialized)
	{
		return;
	}

	ASSERT(simulation_globals.world);
	simulation_globals.world->debug_render();
}

