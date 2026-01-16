#pragma once

#include "cseries/cseries.hpp"
#include "objects/damage_owner.hpp"

class c_vehicle_auto_turret
{
public:
	bool control(int32 vehicle_index);
	bool get_damage_owner(int32 vehicle_index, s_damage_owner* damage_owner);
	void handle_deleted_object(int32 vehicle_index, int32 deleted_object_index);
	bool init(int32 vehicle_index, int32 trigger_volume, real32 min_range, real32 alt_range, real32 alt_time, int32 creator_object_index);
	void reset();

private:
	real32 score_target(int32 vehicle_index, int32 target_unit_index);

public:
	void set_damage_owner(int32 damage_owner_object_index);

private:
	void track_auto_target(int32 vehicle_index);
	void update_auto_target(int32 vehicle_index);

protected:
	int32 m_target_index;
	int32 m_target_time;
	int32 m_range_time;
	int32 m_lost_time;
	s_damage_owner m_damage_owner;
	bool m_damage_owner_valid;
	int32 m_volume_index;
	real32 m_min_range;
	real32 m_alt_range;
	real32 m_alt_time;
};
COMPILE_ASSERT(sizeof(c_vehicle_auto_turret) == 0x30);

