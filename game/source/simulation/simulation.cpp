#include "simulation/simulation.hpp"

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/game_engine.hpp"
#include "main/main.hpp"
#include "math/random_math.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/network_memory.hpp"
#include "profiler/profiler.hpp"
#include "simulation/game_interface/simulation_game_interface.hpp"
#include "simulation/simulation_encoding.hpp"
#include "simulation/simulation_gamestate_entities.hpp"
#include "simulation/simulation_type_collection.hpp"
#include "simulation/simulation_world.hpp"

REFERENCE_DECLARE(0x019A9FA0, s_simulation_globals, simulation_globals);

HOOK_DECLARE(0x004411E0, simulation_describe_status);

void patch_simulation()
{
	patch_pointer({ .address = 0x01655EC0 }, simulation_initialize);
	patch_pointer({ .address = 0x01655EC4 }, simulation_dispose);
	patch_pointer({ .address = 0x01655EC8 }, simulation_initialize_for_new_map);
	patch_pointer({ .address = 0x01655ECC }, simulation_dispose_from_old_map);
}

c_wait_for_render_thread::c_wait_for_render_thread(char const* file, long line) :
	m_flags(_internal_halt_render_thread_and_lock_resources(file, line))
{
}

c_wait_for_render_thread::~c_wait_for_render_thread()
{
	unlock_resources_and_resume_render_thread(m_flags);
}

void __cdecl simulation_abort_immediate(e_simulation_abort_reason abort_reason)
{
	INVOKE(0x00440D70, simulation_abort_immediate, abort_reason);
}

bool __cdecl simulation_aborted()
{
	//return INVOKE(0x00440DD0, simulation_aborted);

	return simulation_globals.initialized && simulation_globals.aborted;
}

void __cdecl simulation_add_view_to_world(e_simulation_view_type view_type, s_machine_identifier const* remote_machine_identifier, long remote_machine_index, char const* remote_machine_name)
{
	INVOKE(0x00440DF0, simulation_add_view_to_world, view_type, remote_machine_identifier, remote_machine_index, remote_machine_name);
}

void __cdecl simulation_apply_after_game(struct simulation_update const* update)
{
	INVOKE(0x00440E40, simulation_apply_after_game, update);
}

void __cdecl simulation_apply_before_game(struct simulation_update const* update)
{
	INVOKE(0x00440E50, simulation_apply_before_game, update);

	//PROFILER(simulation_apply_before_game)
	//{
	//	PROFILER(simulation)
	//	{
	//		TLS_DATA_GET_VALUE_REFERENCE(player_data);
	//
	//		ASSERT(update);
	//		ASSERT(simulation_globals.initialized);
	//		ASSERT(simulation_globals.world);
	//		ASSERT(game_in_progress());
	//
	//		for (long actor_index = 0; actor_index < k_maximum_players; actor_index++)
	//		{
	//			if (TEST_BIT(update->valid_actor_mask, actor_index))
	//			{
	//				if (object_try_and_get_and_verify_type(update->actor_unit_indices[actor_index], _object_mask_unit))
	//					unit_control(update->actor_unit_indices[actor_index], &update->actor_control[actor_index]);
	//			}
	//		}
	//
	//		if (update->machine_update_exists)
	//			players_set_machines(update->machine_update.machine_valid_mask, update->machine_update.machine_identifiers);
	//
	//		c_data_iterator<player_datum> player_iterator;
	//		player_iterator.begin(*player_data);
	//		while (player_iterator.next())
	//		{
	//			if (player_iterator.get_datum()->unit_index != NONE)
	//			{
	//				long player_index = player_iterator.get_index();
	//				long player_absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index);
	//				if ((update->valid_player_prediction_mask & (1 << player_index)) != 0)
	//					simulation_game_process_player_prediction(player_index, &update->player_prediction[player_absolute_index]);
	//			}
	//		}
	//
	//		simulation_globals.world->apply_simulation_queue(&update->bookkeeping_simulation_queue);
	//
	//		long resource_lock = 0;
	//		if (update->game_simulation_queue.queued_count() <= 0)
	//		{
	//			ASSERT(update->game_simulation_queue.queued_size_in_bytes() == 0);
	//		}
	//		else
	//		{
	//			ASSERT(update->game_simulation_queue.queued_size_in_bytes() > 0);
	//			ASSERT(update->flags.test(_simulation_update_simulation_in_progress_bit) || update->flags.test(_simulation_update_game_simulation_queue_requires_application_bit));
	//
	//			if (!update->high_level_flags.test(_simulation_update_simulation_in_progress_bit) && !game_is_playback())
	//				resource_lock = tag_resources_lock_game();
	//
	//			simulation_globals.world->apply_simulation_queue(&update->game_simulation_queue);
	//
	//			if (!update->high_level_flags.test(_simulation_update_simulation_in_progress_bit))
	//			{
	//				objects_purge_deleted_objects();
	//				if (!game_is_playback())
	//					tag_resources_unlock_game(resource_lock);
	//			}
	//		}
	//
	//		if (update->high_level_flags.test(_simulation_update_high_level_unknown_bit1) && !simulation_globals.world->is_authority())
	//		{
	//			ASSERT(!game_is_distributed());
	//			simulation_globals.world->gamestate_flush();
	//		}
	//	}
	//}
}

bool __cdecl simulation_boot_machine(s_machine_identifier const* machine, e_network_session_boot_reason boot_reason)
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
	//
	//		if (simulation_globals.aborted)
	//			ASSERT2("simulation aborted inside game update!");
	//
	//		ASSERT(simulation_globals.world->exists());
	//		ASSERT(game_in_progress());
	//		ASSERT(update);
	//		ASSERT(metadata);
	//
	//		csmemset(update, 0, sizeof(struct simulation_update));
	//		csmemset(metadata, 0, sizeof(struct s_simulation_update_metadata));
	//
	//		simulation_globals.world->build_update(should_build, update, metadata);
	//
	//		if ((!simulation_globals.world->is_authority() || simulation_globals.world->is_playback()) &&
	//			(!simulation_globals.world->is_distributed() || simulation_globals.world->is_playback()) &&
	//			!simulation_globals.world->is_out_of_sync())
	//		{
	//			bool should_go_out_of_sync = false;
	//
	//			if (update->high_level_flags.test(_simulation_update_high_level_unknown_bit2))
	//				simulation_globals.world->gamestate_flush();
	//
	//			if (update->update_number == simulation_globals.world->get_next_update_number())
	//			{
	//				if (update->verify_game_time == simulation_globals.world->get_time())
	//				{
	//					if (update->verify_random != get_random_seed())
	//					{
	//						generate_event(_event_level_error, "networking:simulation:global: OUT OF SYNC, random seed differs, update [#%d] time [%d] seed [0x%08X] (local seed [0x%08X])",
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
	//					generate_event(_event_level_error, "networking:simulation:global: OUT OF SYNC, update time differs, update [#%d] time [%d] != local time %d",
	//						update->update_number,
	//						update->verify_game_time,
	//						simulation_globals.world->get_time());
	//
	//					should_go_out_of_sync = true;
	//				}
	//			}
	//			else
	//			{
	//				generate_event(_event_level_error, "networking:simulation:global: OUT OF SYNC, update number differs, update [#%d] != next [#%d]",
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
	//				should_go_out_of_sync = true;
	//
	//			if (net_allow_out_of_sync)
	//				should_go_out_of_sync = true;
	//
	//			if (should_go_out_of_sync)
	//				determinism_debug_manager_notify_out_of_sync();
	//
	//			if (should_go_out_of_sync)
	//			{
	//				simulation_globals.world->go_out_of_sync();
	//				saved_film_history_notify_out_of_sync();
	//			}
	//		}
	//	}
	//}
}

void __cdecl simulation_clear_errors()
{
	INVOKE(0x004411C0, simulation_clear_errors);
}

// 004411D0

void __cdecl simulation_describe_status(char* buffer, long buffer_size)
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
	if (simulation_globals.initialized)
	{
		simulation_globals.world = NULL;
		simulation_globals.watcher = NULL;
		simulation_globals.type_collection = NULL;
		simulation_globals.initialized = false;
	}
}

void __cdecl simulation_dispose_from_old_map()
{
	INVOKE(0x00441270, simulation_dispose_from_old_map);
}

void __cdecl simulation_end(e_simulation_abort_reason abort_reason)
{
	INVOKE(0x004412B0, simulation_end, abort_reason);
}

void __cdecl simulation_fatal_error()
{
	INVOKE(0x00441310, simulation_fatal_error);
}

// 00441320

bool __cdecl simulation_film_retrieve_updates(long ticks_remaining, long* updates_read_out)
{
	return INVOKE(0x00441330, simulation_film_retrieve_updates, ticks_remaining, updates_read_out);
}

bool __cdecl simulation_film_start_recording()
{
	return INVOKE(0x004413B0, simulation_film_start_recording);
}

void __cdecl simulation_film_stop_recording()
{
	INVOKE(0x00441410, simulation_film_stop_recording);
}

bool __cdecl simulation_format_player_netdebug_data(long a1, s_simulation_player_netdebug_data const* netdebug_data, long* a3)
{
	return INVOKE(0x00441420, simulation_format_player_netdebug_data, a1, netdebug_data, a3);
}

char const* __cdecl simulation_get_abort_reason_string()
{
	return INVOKE(0x00441580, simulation_get_abort_reason_string);
}

e_simulation_abort_reason __cdecl simulation_get_aborted_reason()
{
	return INVOKE(0x004415B0, simulation_get_aborted_reason);
}

void __cdecl simulation_get_game_description(c_static_string<260>* game_description)
{
	INVOKE(0x004415C0, simulation_get_game_description, game_description);
}

bool __cdecl simulation_get_machine_active_in_game(s_machine_identifier const* machine)
{
	return INVOKE(0x00441630, simulation_get_machine_active_in_game, machine);
}

bool __cdecl simulation_get_machine_bandwidth_events(s_machine_identifier const* machine, long bandwidth_event_type_count, long* bandwidth_event_counters)
{
	return INVOKE(0x004416B0, simulation_get_machine_bandwidth_events, machine, bandwidth_event_type_count, bandwidth_event_counters);
}

bool __cdecl simulation_get_machine_connectivity(s_machine_identifier const* machine)
{
	return INVOKE(0x00441700, simulation_get_machine_connectivity, machine);
}

bool __cdecl simulation_get_machine_is_host(s_machine_identifier const* machine)
{
	return INVOKE(0x00441720, simulation_get_machine_is_host, machine);
}

dword __cdecl simulation_get_network_time_since_abort()
{
	return INVOKE(0x00441740, simulation_get_network_time_since_abort);
}

// 00441780

c_simulation_view* __cdecl simulation_get_remote_view_by_channel(c_network_channel* channel)
{
	return INVOKE(0x004417C0, simulation_get_remote_view_by_channel, channel);
}

char const* simulation_get_starting_up_description()
{
	//return INVOKE(0x004417F0, simulation_get_starting_up_description);

	return simulation_starting_up() ? simulation_globals.status.get_string() : "";
}

//enum e_simulation_status __cdecl simulation_get_status(void)
long __cdecl simulation_get_status()
{
	return INVOKE(0x00441830, simulation_get_status);
}

c_simulation_type_collection* __cdecl simulation_get_type_collection()
{
	return INVOKE(0x00441850, simulation_get_type_collection);
}

bool __cdecl simulation_get_view_network_metrics(c_simulation_view const* view, long* rtt_msec, long* packet_rate, long* bandwidth_bps, long* packet_loss)
{
	return INVOKE(0x00441860, simulation_get_view_network_metrics, view, rtt_msec, packet_rate, bandwidth_bps, packet_loss);
}

c_simulation_world* __cdecl simulation_get_world()
{
	return INVOKE(0x00441940, simulation_get_world);
}

c_simulation_world* __cdecl simulation_get_world_if_exists()
{
	return INVOKE(0x00441950, simulation_get_world_if_exists);
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
	
		long entity_type_count = NONE;
		long event_type_count = NONE;
	
		simulation_globals.type_collection->clear_types();
		simulation_game_register_types(simulation_globals.type_collection, &entity_type_count, &event_type_count);
		simulation_globals.type_collection->finish_types(entity_type_count, event_type_count);
	
		simulation_globals.reset_in_progress = false;
		simulation_globals.recording_film = false;
		simulation_globals.initialized = true;
	}
}

void __cdecl simulation_initialize_for_new_map()
{
	INVOKE(0x00441A40, simulation_initialize_for_new_map);
}

void __cdecl simulation_initialize_for_saved_game(long flags)
{
	INVOKE(0x00441B30, simulation_initialize_for_saved_game, flags);
}

void __cdecl simulation_invalidate()
{
	INVOKE(0x00441CC0, simulation_invalidate);
}

void __cdecl simulation_must_close_saved_film()
{
	INVOKE(0x00441CD0, simulation_must_close_saved_film);
}

void __cdecl simulation_notify_channel_closure(void* closure_callback)
{
	INVOKE(0x00441CE0, simulation_notify_channel_closure, closure_callback);
}

void __cdecl simulation_notify_core_save()
{
	INVOKE(0x00441CF0, simulation_notify_core_save);
}

void __cdecl simulation_notify_going_active()
{
	INVOKE(0x00441D10, simulation_notify_going_active);
}

void __cdecl simulation_notify_initial_core_load(long next_update_number)
{
	INVOKE(0x00441D30, simulation_notify_initial_core_load, next_update_number);
}

void __cdecl simulation_notify_players_created()
{
	INVOKE(0x00441D50, simulation_notify_players_created);
}

void __cdecl simulation_notify_reset_complete()
{
	INVOKE(0x00441D90, simulation_notify_reset_complete);
}

void __cdecl simulation_notify_reset_initiate()
{
	INVOKE(0x00441DC0, simulation_notify_reset_initiate);
}

void __cdecl simulation_notify_revert()
{
	INVOKE(0x00441DD0, simulation_notify_revert);
}

void __cdecl simulation_notify_saved_film_ended()
{
	INVOKE(0x00441DF0, simulation_notify_saved_film_ended);
}

void __cdecl simulation_notify_saved_film_revert(long a1, long a2)
{
	INVOKE(0x00441E10, simulation_notify_saved_film_revert, a1, a2);
}

bool __cdecl simulation_performed_main_save_and_exit_campaign_immediately_this_map()
{
	return INVOKE(0x00441E40, simulation_performed_main_save_and_exit_campaign_immediately_this_map);
}

void __cdecl simulation_player_joined_game(long player_index)
{
	INVOKE(0x00441E50, simulation_player_joined_game, player_index);
}

void __cdecl simulation_player_left_game(long player_index)
{
	INVOKE(0x00441EA0, simulation_player_left_game, player_index);
}

void __cdecl simulation_prepare_to_load_saved_game(long a1)
{
	INVOKE(0x00441EF0, simulation_prepare_to_load_saved_game, a1);
}

void __cdecl simulation_prepare_to_send()
{
	INVOKE(0x00441F00, simulation_prepare_to_send);
}

void __cdecl simulation_process_actor_control(long actor_index, unit_control_data const* control)
{
	INVOKE(0x00441F30, simulation_process_actor_control, actor_index, control);
}

void __cdecl simulation_process_input(dword player_mask, c_static_array<s_player_action, 4> const& actions)
{
	INVOKE(0x00441F60, simulation_process_input, player_mask, actions);
}

void __cdecl simulation_record_update(struct simulation_update const* update)
{
	INVOKE(0x00441F90, simulation_record_update, update);
}

void __cdecl simulation_remove_view_from_world(c_simulation_view* view)
{
	INVOKE(0x00441FA0, simulation_remove_view_from_world, view);
}

void __cdecl simulation_reset()
{
	INVOKE(0x00441FE0, simulation_reset);
}

bool __cdecl simulation_reset_in_progress()
{
	return INVOKE(0x00442070, simulation_reset_in_progress);
}

void __cdecl simulation_saved_film_revert()
{
	INVOKE(0x00442080, simulation_saved_film_revert);
}

void __cdecl simulation_set_performed_main_save_and_exit_campaign_immediately_this_map(bool performed_main_save_and_exit_campaign_immediately_this_map)
{
	INVOKE(0x00442090, simulation_set_performed_main_save_and_exit_campaign_immediately_this_map, performed_main_save_and_exit_campaign_immediately_this_map);
}

bool __cdecl simulation_should_transmit_simulation_data()
{
	return INVOKE(0x004420A0, simulation_should_transmit_simulation_data);
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
	//		simulation_globals.world.attach_to_map();
	//}
}

bool __cdecl simulation_starting_up()
{
	//return INVOKE(0x004420E0, simulation_starting_up);

	if (simulation_globals.initialized)
	{
		ASSERT(simulation_globals.world);

		if (!simulation_globals.aborted && simulation_globals.world->exists())
			return !simulation_globals.world->is_active();
	}
	return false;
}

void __cdecl simulation_stop()
{
	INVOKE(0x00442110, simulation_stop);
}

long __cdecl simulation_time_get_maximum_available(bool* match_remote_time)
{
	return INVOKE(0x00442150, simulation_time_get_maximum_available, match_remote_time);
}

void __cdecl simulation_update()
{
	INVOKE(0x004421B0, simulation_update);
}

void __cdecl simulation_update_aftermath(struct simulation_update const* update, s_simulation_update_metadata* metadata)
{
	INVOKE(0x004423F0, simulation_update_aftermath, update, metadata);
}

void __cdecl simulation_update_out_of_sync()
{
	INVOKE(0x00442430, simulation_update_out_of_sync);
}

bool __cdecl simulation_update_player_netdebug_data(long player_index, s_simulation_player_netdebug_data* netdebug_data)
{
	return INVOKE(0x004424F0, simulation_update_player_netdebug_data, player_index, netdebug_data);
}

void __cdecl simulation_update_pregame()
{
	INVOKE(0x004426F0, simulation_update_pregame);

	//if (!simulation_globals.initialized)
	//	return;
	//
	//if (!game_in_progress())
	//	return;
	//
	//if (!simulation_globals.watcher->need_to_generate_updates())
	//	return;
	//
	//struct simulation_update update {};
	//s_simulation_update_metadata metadata{};
	//simulation_build_update(false, &update, &metadata);
	//ASSERT(!update.high_level_flags.test(_simulation_update_high_level_simulation_in_progress_bit));
	//
	////saved_film_history_after_update_built(&update, &metadata);
	////simulation_record_update(&update);
	//simulation_globals.recording_film = false;
	//
	////random_seed_allow_use();
	//damage_acceleration_queue_begin();
	//
	//simulation_apply_before_game(&update);
	//
	//damage_acceleration_queue_end();
	////random_seed_disallow_use();
	//
	//simulation_update_aftermath(&update, &metadata);
	//simulation_destroy_update(&update);
}

bool simulation_update_read_from_buffer(struct simulation_update* update, long buffer_size, byte const* buffer)
{
	return INVOKE(0x004427C0, simulation_update_read_from_buffer, update, buffer_size, buffer);

	//csmemset(update, 0, sizeof(struct simulation_update));
	//
	//c_bitstream packet((byte*)buffer, buffer_size);
	//packet.begin_reading();
	//
	//if (simulation_update_decode(&packet, update))
	//	return false;
	//
	//packet.finish_reading();
	//return true;
}

bool __cdecl simulation_update_write_to_buffer(struct simulation_update const* update, long buffer_size, byte* buffer, long* out_update_length)
{
	return INVOKE(0x00442840, simulation_update_write_to_buffer, update, buffer_size, buffer, out_update_length);
}

void simulation_debug_render()
{
	ASSERT(simulation_globals.world);

	simulation_globals.world->debug_render();
}

