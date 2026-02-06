#include "simulation/simulation_encoding.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "profiler/profiler.hpp"
#include "replication/replication_encoding.hpp"
#include "simulation/simulation.hpp"

bool __cdecl player_action_compare(const player_action* action_a, player_action* action_b)
{
	return INVOKE(0x0046DCA0, player_action_compare, action_a, action_b);
}

bool __cdecl player_action_decode(c_bitstream* packet, player_action* action)
{
	return INVOKE(0x0046DF80, player_action_decode, packet, action);
}

void __cdecl player_action_encode(c_bitstream* packet, const player_action* action)
{
	INVOKE(0x0046E240, player_action_encode, packet, action);
}

bool __cdecl simulation_camera_update_decode(c_bitstream* packet, s_simulation_camera_update* camera_update)
{
	return INVOKE(0x0046EA20, simulation_camera_update_decode, packet, camera_update);
}

void __cdecl simulation_camera_update_encode(c_bitstream* packet, const s_simulation_camera_update* camera_update)
{
	INVOKE(0x0046EAB0, simulation_camera_update_encode, packet, camera_update);
}

void __cdecl simulation_encoding_get_world_bounds(real_rectangle3d* world_bounds)
{
	INVOKE(0x0046EE60, simulation_encoding_get_world_bounds, world_bounds);
}

bool __cdecl simulation_encoding_point_within_world(const real_point3d* point)
{
	return INVOKE(0x0046EED0, simulation_encoding_point_within_world, point);
}

// is this part of `simulation_encoding`?
// checks if an objects type is biped
//.text:0046EF20 ;

bool __cdecl simulation_machine_update_decode(c_bitstream* packet, simulation_machine_update* machine_update)
{
	return INVOKE(0x0046EF60, simulation_machine_update_decode, packet, machine_update);
}

void __cdecl simulation_machine_update_encode(c_bitstream* packet, const simulation_machine_update* machine_update)
{
	INVOKE(0x0046EFC0, simulation_machine_update_encode, packet, machine_update);
}

bool __cdecl simulation_player_update_decode(c_bitstream* packet, simulation_player_update* player_update)
{
	return INVOKE(0x0046F050, simulation_player_update_decode, packet, player_update);
}

void __cdecl simulation_player_update_encode(c_bitstream* packet, const simulation_player_update* player_update)
{
	INVOKE(0x0046F170, simulation_player_update_encode, packet, player_update);
}

void __cdecl simulation_read_location(c_bitstream* packet, s_location* location)
{
	INVOKE(0x0046F330, simulation_read_location, packet, location);

	//location->cluster_reference.bsp_index = packet->read_integer("bsp_index", 2);
	//location->cluster_reference.cluster_index = packet->read_integer("cluster_index", 8);
}

//void __cdecl simulation_read_quantized_node_space_position(c_bitstream* packet, real_point3d* node_space_position, enum e_simulation_node_space_encoding_type encoding_type, int32 axis_encoding_size_in_bits)
void __cdecl simulation_read_quantized_node_space_position(c_bitstream* packet, real_point3d* node_space_position, int32 encoding_type, int32 axis_encoding_size_in_bits)
{
	INVOKE(0x0046F360, simulation_read_quantized_node_space_position, packet, node_space_position, encoding_type, axis_encoding_size_in_bits);
}

void __cdecl simulation_read_quantized_position(c_bitstream* packet, real_point3d* position, int32 axis_encoding_size_in_bits, const real_rectangle3d* world_bounds)
{
	INVOKE(0x0046F3F0, simulation_read_quantized_position, packet, position, axis_encoding_size_in_bits, world_bounds);
}

bool __cdecl simulation_update_compare(const struct simulation_update* update1, struct simulation_update* update2)
{
	return INVOKE(0x0046F4A0, simulation_update_compare, update1, update2);
}

bool __cdecl simulation_update_decode(c_bitstream* packet, struct simulation_update* update)
{
	return INVOKE(0x0046F5A0, simulation_update_decode, packet, update);

#if 0
	bool success = true;
	PROFILER(simulation_update_decode)
	{
		ASSERT(packet);
		ASSERT(update);
	
		update->update_number = packet->read_integer("update-number", 32);
		update->flags.set_unsafe(packet->read_integer("flags", 4));
		update->player_flags = packet->read_integer("player-flags", 16);
		update->action_test_flags = packet->read_integer("action-test-flags", 26);
	
		for (int32 player_index = 0; player_index < 16; player_index++)
		{
			if (TEST_BIT(update->player_flags, player_index))
			{
				success = success && player_action_decode(packet, &update->player_actions[player_index]);
			}
		}
	
		update->valid_actor_mask = packet->read_integer("valid-actor-mask", 16);
		for (int32 actor_index = 0; actor_index < 16; actor_index++)
		{
			if (TEST_BIT(update->valid_actor_mask, actor_index))
			{
				update->actor_unit_indices[actor_index] = packet->read_integer("actor-unit-index", 32);
				success = success && unit_control_decode(packet, &update->actor_control[actor_index]);
			}
		}
	
		update->machine_update_exists = packet->read_bool("machine-update-exists");
		if (update->machine_update_exists)
		{
			success = success && simulation_machine_update_decode(packet, &update->machine_update);
		}
	
		update->valid_player_prediction_mask = packet->read_integer("valid-player-prediction-mask", 16);
		for (int32 player_index = 0; player_index < 16; player_index++)
		{
			if (TEST_BIT(update->valid_player_prediction_mask, player_index))
			{
				success = success && player_prediction_decode(packet, &update->player_prediction[player_index], false);
			}
		}
	
		update->valid_camera_mask = packet->read_integer("valid-camera-mask", 1);
		for (int32 camera_track_index = 0; camera_track_index < NUMBEROF(update->camera_updates); camera_track_index++)
		{
			if (TEST_BIT(update->valid_camera_mask, camera_track_index))
			{
				success = success && simulation_camera_update_decode(packet, &update->camera_updates[camera_track_index]);
			}
		}
	
		update->verify_game_time = packet->read_integer("verify-game-time", 32);
		update->verify_random = packet->read_integer("verify-random", 32);
		//success = success && determinism_debug_manager_decode_game_state_checksum(packet, &update->determinism_verification);
		success = success && update->bookkeeping_simulation_queue.decode(packet);
		success = success && update->game_simulation_queue.decode(packet);
		success = success && !packet->error_occurred();
		success = success && update->verify_game_time >= 0;
		success = success && update->update_number >= 0;
		success = success && update->flags.valid();
		if (!success)
		{
			update->bookkeeping_simulation_queue.dispose();
			update->game_simulation_queue.dispose();
		}
	}
	return success;
#endif
}

void __cdecl simulation_update_encode(c_bitstream* packet, const struct simulation_update* update)
{
	INVOKE(0x0046F830, simulation_update_encode, packet, update);

#if 0
	PROFILER(simulation_update_encode)
	{
		int32 start_bit_position = packet->get_current_stream_bit_position();
	
		ASSERT(packet);
		ASSERT(update);
	
		packet->write_integer("update-number", update->update_number, 32);
		packet->write_integer("flags", update->flags.get_unsafe(), 4);
		packet->write_integer("player-flags", update->player_flags, 16);
	
		packet->write_integer("action-test-flags", update->action_test_flags, 26);
		for (int32 player_index = 0; player_index < 16; player_index++)
		{
			if (TEST_BIT(update->player_flags, player_index))
			{
				player_action_encode(packet, &update->player_actions[player_index]);
			}
		}
	
		packet->write_integer("valid-actor-mask", update->valid_actor_mask, 16);
		for (int32 actor_index = 0; actor_index < 16; actor_index++)
		{
			if (TEST_BIT(update->valid_actor_mask, actor_index))
			{
				packet->write_integer("actor-unit-index", update->actor_unit_indices[actor_index], 32);
				unit_control_encode(packet, &update->actor_control[actor_index]);
			}
		}
	
		packet->write_bool("machine-update-exists", update->machine_update_exists);
		if (update->machine_update_exists)
		{
			simulation_machine_update_encode(packet, &update->machine_update);
		}
	
		packet->write_integer("valid-player-prediction-mask", update->valid_player_prediction_mask, 16);
		for (int32 player_index = 0; player_index < 16; player_index++)
		{
			if (TEST_BIT(update->valid_player_prediction_mask, player_index))
			{
				player_prediction_encode(packet, &update->player_prediction[player_index], false);
			}
		}
	
		packet->write_integer("valid-camera-mask", update->valid_camera_mask, 1);
		for (int32 camera_track_index = 0; camera_track_index < NUMBEROF(update->camera_updates); camera_track_index++)
		{
			if (TEST_BIT(update->valid_camera_mask, camera_track_index))
			{
				simulation_camera_update_encode(packet, &update->camera_updates[camera_track_index]);
			}
		}
	
		packet->write_integer("verify-game-time", update->verify_game_time, 32);
		packet->write_integer("verify-random", update->verify_random, 32);
	
		int32 pre_queues_encoded_size = (packet->get_current_stream_bit_position() - start_bit_position + 7) / 8;
		ASSERT(pre_queues_encoded_size > 0);
	
		if (pre_queues_encoded_size > 0x1800)
		{
			event(_event_critical, "simulation:encoding: encoded simulation update (no queues) exceeding estimate [%d > %d]",
				pre_queues_encoded_size,
				0x1800);
		}
	
		update->bookkeeping_simulation_queue.encode(packet);
		update->game_simulation_queue.encode(packet);
	}
#endif
}

void __cdecl simulation_write_location(c_bitstream* packet, const s_location* location)
{
	INVOKE(0x0046FC30, simulation_write_location, packet, location);

	//packet->write_integer("bsp_index", location->cluster_reference.bsp_index, 2);
	//packet->write_integer("cluster_index", location->cluster_reference.cluster_index, 8);
}

//void __cdecl simulation_write_quantized_node_space_position(c_bitstream* packet, const real_point3d* node_space_position, enum e_simulation_node_space_encoding_type encoding_type, int32 axis_encoding_size_in_bits)
void __cdecl simulation_write_quantized_node_space_position(c_bitstream* packet, const real_point3d* node_space_position, int32 encoding_type, int32 axis_encoding_size_in_bits)
{
	INVOKE(0x0046FCC0, simulation_write_quantized_node_space_position, packet, node_space_position, encoding_type, axis_encoding_size_in_bits);
}

void __cdecl simulation_write_quantized_position(c_bitstream* packet, const real_point3d* position, int32 a3, bool a4, const real_rectangle3d* world_bounds)
{
	INVOKE(0x0046FD80, simulation_write_quantized_position, packet, position, a3, a4, world_bounds);
}

bool __cdecl unit_control_decode(c_bitstream* bitstream, unit_control_data* unit_control)
{
	return INVOKE(0x0046FFE0, unit_control_decode, bitstream, unit_control);

	//ASSERT(bitstream);
	//ASSERT(unit_control);
	//
	//unit_control->desired_mode = bitstream->read_integer("animation-seat", 32);
	//unit_control->aiming_speed = bitstream->read_integer("animation-speed", 32);
	//
	//unit_control->weapon_set.set_identifier = bitstream->read_integer("weapon-set-id", 32);
	//bitstream->read_bits_internal(unit_control->weapon_set.weapon_indices.begin(), SIZEOF_BITS(k_unit_weapon_set_count));
	//
	//unit_control->grenade_index = bitstream->read_integer("grenade-index", 32);
	//unit_control->zoom_level = bitstream->read_integer("zoom-level", 32);
	//unit_control->control_flags = bitstream->read_integer("control-flags", 32);
	//
	//unit_control->primary_trigger = bitstream->read_quantized_real("primary-trigger", 0.0f, 1.0f, 20, false, true);
	//unit_control->secondary_trigger = bitstream->read_quantized_real("secondary-trigger", -1.0f, 1.0f, 20, false, true);
	//
	//unit_control->throttle.i = bitstream->read_quantized_real("throttle-i", -1.0f, 1.0f, 20, true, true);
	//unit_control->throttle.j = bitstream->read_quantized_real("throttle-j", -1.0f, 1.0f, 20, true, true);
	//unit_control->throttle.k = bitstream->read_quantized_real("throttle-k", -1.0f, 1.0f, 20, true, true);
	//
	//bitstream->read_unit_vector("facing", &unit_control->facing_vector, 19);
	//bitstream->read_unit_vector("aiming", &unit_control->aiming_vector, 19);
	//bitstream->read_unit_vector("looking", &unit_control->looking_vector, 19);
	//
	//// position
	//simulation_read_quantized_position(bitstream, &unit_control->gaze_position, 16, nullptr);
	//
	//// aim-assist-data
	//bitstream->read_bits_internal(align_pointer(&unit_control->aim_assist_data, 2), SIZEOF_BITS(s_aim_assist_targeting_result));
}

bool __cdecl unit_control_encode(c_bitstream* bitstream, const unit_control_data* unit_control)
{
	return INVOKE(0x00470180, unit_control_encode, bitstream, unit_control);
}

