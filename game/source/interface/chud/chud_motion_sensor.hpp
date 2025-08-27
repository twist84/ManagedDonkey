#pragma once

#include "cseries/cseries.hpp"

class c_chud_motion_sensor
{
public:
	enum
	{
		k_history_count = 10,
		k_max_tracked_entities_per_motion_sensor = 20,
		k_max_blips_per_history = 16,
		k_max_custom_blips_count = 20,
	};

	enum e_multiplayer_tracking_flags
	{
		_track_moving_friendlies_bit = 0,
		_track_moving_enemies_bit,
		_track_stationary_friendlies_bit,
		_track_stationary_enemies_bit,
		_track_moving_neutral_vehicles_bit,

		k_number_of_multiplayer_tracking_flags,
	};

	enum e_blip_type
	{
		_blip_type_none = 0,
		_blip_type_self,
		_blip_type_friend,
		_blip_type_enemy,
		_blip_type_neutral,
		_blip_type_custom_onscreen,
		_blip_type_custom_offscreen,
		_blip_type_ping,

		k_blip_type_count,
	};

	enum e_blip_size
	{
		_blip_size_normal = 0,
		_blip_size_small,
		_blip_size_large,

		k_blip_size_count,
	};

	enum e_blip_shape
	{
		_blip_shape_normal = 0,
		_blip_shape_waypoint_onscreen,
		_blip_shape_waypoint_offscreen,
		_blip_shape_ping,

		k_blip_shape_count,
	};

	struct s_motion_sensor_blip
	{
		real_point2d position;
		real32 unused;
		uns8 blip_type;
		uns8 blip_size;
		uns8 blip_shape;
		byte pad[1];
	};
	static_assert(sizeof(s_motion_sensor_blip) == 0x10);

	struct s_motion_sensor_history
	{
		int32 blip_count;
		s_motion_sensor_blip blips[k_max_blips_per_history];
	};
	static_assert(sizeof(s_motion_sensor_history) == 0x104);

	struct s_motion_sensor_tracked_entity
	{
		int32 tracked_object_index;
		real32 squared_distance;
		int32 ping_timer;
	};
	static_assert(sizeof(s_motion_sensor_tracked_entity) == 0xC);

//private:
	s_motion_sensor_tracked_entity m_tracked_entities[k_max_tracked_entities_per_motion_sensor];
	s_motion_sensor_history m_histories[k_history_count];
	s_motion_sensor_blip m_custom_blips[k_max_custom_blips_count];
	real32 m_custom_blip_alphas[k_max_custom_blips_count];
	c_flags<e_multiplayer_tracking_flags, int32, k_number_of_multiplayer_tracking_flags> m_multiplayertracking_flags;
	int32 m_custom_blip_count;
	int32 m_user_index;
	int32 m_recent_history_index;
	real32 m_tracking_distance;
	real32 m_radar_distance;
	real32 m_oo_radar_distance;
};
static_assert(sizeof(c_chud_motion_sensor) == 0xCC4);

