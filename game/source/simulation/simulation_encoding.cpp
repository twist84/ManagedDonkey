#include "simulation/simulation_encoding.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "profiler/profiler.hpp"
#include "replication/replication_encoding.hpp"
#include "simulation/simulation.hpp"

bool __cdecl player_action_compare(s_player_action const* action_a, s_player_action* action_b)
{
	return INVOKE(0x0046DCA0, player_action_compare, action_a, action_b);
}

bool __cdecl player_action_decode(c_bitstream* packet, s_player_action* action)
{
	return INVOKE(0x0046DF80, player_action_decode, packet, action);
}

void __cdecl player_action_encode(c_bitstream* packet, s_player_action const* action)
{
	INVOKE(0x0046E240, player_action_encode, packet, action);
}

bool __cdecl simulation_camera_update_decode(c_bitstream* packet, s_simulation_camera_update* camera_update)
{
	return INVOKE(0x0046EA20, simulation_camera_update_decode, packet, camera_update);
}

void __cdecl simulation_camera_update_encode(c_bitstream* packet, s_simulation_camera_update const* camera_update)
{
	INVOKE(0x0046EAB0, simulation_camera_update_encode, packet, camera_update);
}

void __cdecl simulation_encoding_get_world_bounds(real_rectangle3d* world_bounds)
{
	INVOKE(0x0046EE60, simulation_encoding_get_world_bounds, world_bounds);
}

bool __cdecl simulation_encoding_point_within_world(real_point3d const* point)
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

void __cdecl simulation_machine_update_encode(c_bitstream* packet, simulation_machine_update const* machine_update)
{
	INVOKE(0x0046EFC0, simulation_machine_update_encode, packet, machine_update);
}

bool __cdecl simulation_player_update_decode(c_bitstream* packet, simulation_player_update* player_update)
{
	return INVOKE(0x0046F050, simulation_player_update_decode, packet, player_update);
}

void __cdecl simulation_player_update_encode(c_bitstream* packet, simulation_player_update const* player_update)
{
	INVOKE(0x0046F170, simulation_player_update_encode, packet, player_update);
}

void __cdecl simulation_read_location(c_bitstream* packet, s_location* location)
{
	INVOKE(0x0046F330, simulation_read_location, packet, location);

	//location->cluster_reference.bsp_index = packet->read_integer("bsp_index", 2);
	//location->cluster_reference.cluster_index = packet->read_integer("cluster_index", 8);
}

//void __cdecl simulation_read_quantized_node_space_position(c_bitstream* packet, real_point3d* node_space_position, enum e_simulation_node_space_encoding_type encoding_type, long axis_encoding_size_in_bits)
void __cdecl simulation_read_quantized_node_space_position(c_bitstream* packet, real_point3d* node_space_position, long encoding_type, long axis_encoding_size_in_bits)
{
	INVOKE(0x0046F360, simulation_read_quantized_node_space_position, packet, node_space_position, encoding_type, axis_encoding_size_in_bits);
}

void __cdecl simulation_read_quantized_position(c_bitstream* packet, real_point3d* position, long axis_encoding_size_in_bits, real_rectangle3d const* world_bounds)
{
	INVOKE(0x0046F3F0, simulation_read_quantized_position, packet, position, axis_encoding_size_in_bits, world_bounds);
}

bool __cdecl simulation_update_compare(struct simulation_update const* update1, struct simulation_update* update2)
{
	return INVOKE(0x0046F4A0, simulation_update_compare, update1, update2);
}

bool __cdecl simulation_update_decode(c_bitstream* packet, struct simulation_update* update)
{
	return INVOKE(0x0046F5A0, simulation_update_decode, packet, update);

	//bool result = true;
	//PROFILER(simulation_update_decode)
	//{
	//	ASSERT(packet);
	//	ASSERT(update);
	//
	//	update->update_number = packet->read_integer("update-number", 32);
	//	update->high_level_flags.set_unsafe(packet->read_integer("flags", 4));
	//	update->player_flags = packet->read_integer("player-flags", 16);
	//	update->action_test_flags = packet->read_integer("action-test-flags", 26);
	//
	//	for (long i = 0; i < 16; i++)
	//	{
	//		if (TEST_BIT(update->player_flags, i))
	//			result &= player_action_decode(packet, &update->player_actions[i]);
	//	}
	//
	//	update->valid_actor_mask = packet->read_integer("valid-actor-mask", 16);
	//	for (long i = 0; i < 16; i++)
	//	{
	//		if (TEST_BIT(update->valid_actor_mask, i))
	//		{
	//			update->actor_unit_indices[i] = packet->read_integer("actor-unit-index", 32);
	//			result &= unit_control_decode(packet, &update->actor_control[i]);
	//		}
	//	}
	//
	//	update->machine_update_exists = packet->read_bool("machine-update-exists");
	//	if (update->machine_update_exists)
	//	{
	//		result &= simulation_machine_update_decode(packet, &update->machine_update);
	//	}
	//
	//	update->valid_player_prediction_mask = packet->read_integer("valid-player-prediction-mask", 16);
	//	for (long i = 0; i < 16; i++)
	//	{
	//		if (TEST_BIT(update->valid_player_prediction_mask, i))
	//			result &= player_prediction_decode(packet, &update->player_prediction[i], false);
	//	}
	//
	//	update->valid_camera_mask = packet->read_integer("valid-camera-mask", 1);
	//	for (long i = 0; i < 1; i++)
	//	{
	//		if (TEST_BIT(update->valid_camera_mask, i))
	//			result &= simulation_camera_update_decode(packet, &update->camera_updates[i]);
	//	}
	//
	//	update->verify_game_time = packet->read_integer("verify-game-time", 32);
	//	update->verify_random = packet->read_integer("verify-random", 32);
	//	//result &= determinism_debug_manager_decode_game_state_checksum(packet, &update->determinism_verification);
	//	result &= update->bookkeeping_simulation_queue.decode(packet);
	//	result &= update->game_simulation_queue.decode(packet);
	//	result &= !packet->error_occurred();
	//	result &= update->verify_game_time >= 0;
	//	result &= update->update_number >= 0;
	//	result &= update->high_level_flags.valid();
	//	if (!result)
	//	{
	//		update->bookkeeping_simulation_queue.dispose();
	//		update->game_simulation_queue.dispose();
	//	}
	//}
	//return result;
}

void __cdecl simulation_update_encode(c_bitstream* packet, struct simulation_update const* update)
{
	INVOKE(0x0046F830, simulation_update_encode, packet, update);

	//PROFILER(simulation_update_encode)
	//{
	//	long start_bit_position = packet->get_current_stream_bit_position();
	//
	//	ASSERT(packet);
	//	ASSERT(update);
	//
	//	packet->write_integer("update-number", update->update_number, 32);
	//	packet->write_integer("flags", update->high_level_flags.get_unsafe(), 4);
	//	packet->write_integer("player-flags", update->player_flags, 16);
	//
	//	packet->write_integer("action-test-flags", update->action_test_flags, 26);
	//	for (long i = 0; i < 16; i++)
	//	{
	//		if (TEST_BIT(update->player_flags, i))
	//			player_action_encode(packet, &update->player_actions[i]);
	//	}
	//
	//	packet->write_integer("valid-actor-mask", update->valid_actor_mask, 16);
	//	for (long i = 0; i < 16; i++)
	//	{
	//		if (TEST_BIT(update->valid_actor_mask, i))
	//		{
	//			packet->write_integer("actor-unit-index", update->actor_unit_indices[i], 32);
	//			unit_control_encode(packet, &update->actor_control[i]);
	//		}
	//	}
	//
	//	packet->write_bool("machine-update-exists", update->machine_update_exists);
	//	if (update->machine_update_exists)
	//		simulation_machine_update_encode(packet, &update->machine_update);
	//
	//	packet->write_integer("valid-player-prediction-mask", update->valid_player_prediction_mask, 16);
	//	for (long i = 0; i < 16; i++)
	//	{
	//		if (TEST_BIT(update->valid_player_prediction_mask, i))
	//			player_prediction_encode(packet, &update->player_prediction[i], false);
	//	}
	//
	//	packet->write_integer("valid-camera-mask", update->valid_camera_mask, 1);
	//	for (long i = 0; i < 1; i++)
	//	{
	//		if (TEST_BIT(update->valid_camera_mask, i))
	//			simulation_camera_update_encode(packet, &update->camera_updates[i]);
	//	}
	//
	//	packet->write_integer("verify-game-time", update->verify_game_time, 32);
	//	packet->write_integer("verify-random", update->verify_random, 32);
	//
	//	long pre_queues_encoded_size = (packet->get_current_stream_bit_position() - start_bit_position + 7) / 8;
	//	ASSERT(pre_queues_encoded_size > 0);
	//
	//	if (pre_queues_encoded_size > 0x1800)
	//	{
	//		generate_event(_event_level_critical, "simulation:encoding: encoded simulation update (no queues) exceeding estimate [%d > %d]",
	//			pre_queues_encoded_size,
	//			0x1800);
	//	}
	//
	//	update->bookkeeping_simulation_queue.encode(packet);
	//	update->game_simulation_queue.encode(packet);
	//}
}

void __cdecl simulation_write_location(c_bitstream* packet, s_location const* location)
{
	INVOKE(0x0046FC30, simulation_write_location, packet, location);

	//packet->write_integer("bsp_index", location->cluster_reference.bsp_index, 2);
	//packet->write_integer("cluster_index", location->cluster_reference.cluster_index, 8);
}

//void __cdecl simulation_write_quantized_node_space_position(c_bitstream* packet, real_point3d const* node_space_position, enum e_simulation_node_space_encoding_type encoding_type, long axis_encoding_size_in_bits)
void __cdecl simulation_write_quantized_node_space_position(c_bitstream* packet, real_point3d const* node_space_position, long encoding_type, long axis_encoding_size_in_bits)
{
	INVOKE(0x0046FCC0, simulation_write_quantized_node_space_position, packet, node_space_position, encoding_type, axis_encoding_size_in_bits);
}

void __cdecl simulation_write_quantized_position(c_bitstream* packet, real_point3d const* position, long a3, bool a4, real_rectangle3d const* world_bounds)
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
	//bitstream->read_bits_internal(unit_control->weapon_set.weapon_indices.begin(), SIZEOF_BITS(k_weapon_set_count));
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
	//simulation_read_quantized_position(bitstream, &unit_control->gaze_position, 16, NULL);
	//
	//// aim-assist-data
	//bitstream->read_bits_internal(align_pointer(&unit_control->aim_assist_data, 2), SIZEOF_BITS(s_aim_assist_targeting_result));
}

bool __cdecl unit_control_encode(c_bitstream* bitstream, unit_control_data const* unit_control)
{
	return INVOKE(0x00470180, unit_control_encode, bitstream, unit_control);
}

