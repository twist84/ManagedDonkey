#pragma once

#include "cseries/cseries.hpp"
#include "motor/actions.hpp"

class c_motor_task
{
public:
	void clear();
	bool conflicts(int16 a1) const;
	bool program_default();
	bool restricted_to(int16 a1) const;
	bool set(int32 motor_task, int32 action, int32 motor_program, int16 a4);
	bool valid() const;

//protected:
	int32 m_motor_task;
	int32 m_action;
	int32 m_motor_program;
	uns8 __unknownC;
	uns8 m_interrupted;
	int16 __unknownE;
	byte __data10[0x8];
	int16 m_resource_mask;
	bool __unknown1A;
	byte __data1B[0x1];
};
COMPILE_ASSERT(sizeof(c_motor_task) == 0x1C);

class c_motor_request
{
public:
	c_motor_request();
	void setup_force_airborne(e_action action);

//protected:
	bool m_initialized;
	bool m_force;
	int32 m_task;
	c_enum<e_action, int32, _action_none, k_action_count> m_action;

	union
	{
		byte m_storage[0x2C];
	};
};
COMPILE_ASSERT(sizeof(c_motor_request) == 0x38);

extern int32 __cdecl motor_system_submit(int32 motor_index, const c_motor_request* request);

