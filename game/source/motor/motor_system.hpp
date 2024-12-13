#pragma once

#include "cseries/cseries.hpp"
#include "motor/actions.hpp"

struct c_motor_task
{
public:
	void clear();
	bool conflicts(short a1) const;
	bool program_default();
	bool restricted_to(short a1) const;
	bool set(long motor_task, long action, long motor_program, short a4);
	bool valid() const;

//protected:
	long m_motor_task;
	long m_action;
	long m_motor_program;
	byte __unknownC;
	byte m_interrupted;
	short __unknownE;
	byte __data10[0x8];
	short m_resource_mask;
	bool __unknown1A;
	byte __data1B[0x1];
};
static_assert(sizeof(c_motor_task) == 0x1C);

struct c_motor_request
{
public:
	c_motor_request();
	void setup_force_airborne(e_action action);

//protected:
	bool m_initialized;
	bool m_force;
	long m_task;
	c_enum<e_action, long, _action_none, k_action_count> m_action;

	union
	{
		byte m_storage[0x2C];
	};
};
static_assert(sizeof(c_motor_request) == 0x38);

extern long __cdecl motor_system_submit(long motor_index, c_motor_request const* request);

