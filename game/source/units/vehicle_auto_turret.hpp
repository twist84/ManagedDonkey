#pragma once

#include "cseries/cseries.hpp"
#include "objects/damage_owner.hpp"

struct c_vehicle_auto_turret
{
public:
	bool control(long vehicle_index);
	bool get_damage_owner(long vehicle_index, s_damage_owner* damage_owner);
	void handle_deleted_object(long vehicle_index, long deleted_object_index);
	bool init(long vehicle_index, long trigger_volume, real32 min_range, real32 alt_range, real32 alt_time, long creator_object_index);
	void reset();

private:
	real32 score_target(long vehicle_index, long target_unit_index);

public:
	void set_damage_owner(long damage_owner_object_index);

private:
	void track_auto_target(long vehicle_index);
	void update_auto_target(long vehicle_index);

protected:
	long m_target_index;
	long m_target_time;
	long m_range_time;
	long m_lost_time;
	s_damage_owner m_damage_owner;
	bool m_damage_owner_valid;
	long m_volume_index;
	real32 m_min_range;
	real32 m_alt_range;
	real32 m_alt_time;
};
static_assert(sizeof(c_vehicle_auto_turret) == 0x30);

