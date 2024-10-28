#include "motor/motor_system.hpp"

void c_motor_task::clear()
{
	INVOKE_CLASS_MEMBER(0x00B744B0, c_motor_task, clear);
}

bool c_motor_task::conflicts(short a1) const
{
	return INVOKE_CLASS_MEMBER(0x00B744E0, c_motor_task, conflicts, a1);
}

//.text:00B74500 ; 
//.text:00B74510 ; 
//.text:00B74520 ; void __cdecl motor_system_animation_event(long, c_motor_task*, long)
//.text:00B74590 ; void __cdecl motor_system_event(long, c_motor_task*, e_motor_event)
//.text:00B745E0 ; bool __cdecl motor_system_finished(long, c_motor_task*, c_animation_channel*)
//.text:00B746B0 ; bool __cdecl motor_system_in_airborne_state(long)
//.text:00B74700 ; bool __cdecl motor_system_in_contact_with_ground(long)
//.text:00B74750 ; void __cdecl motor_system_interrupted(long, c_motor_task*, bool)
//.text:00B74810 ; bool __cdecl motor_system_is_super_collidable_ragdoll(long)
//.text:00B74860 ; bool __cdecl motor_system_object_post_update(long)
//.text:00B74900 ; bool __cdecl motor_system_program_handles_task(e_motor_program, e_motor_task)
//.text:00B74920 ; short __cdecl motor_system_relaxation_state(long)
//.text:00B74980 ; bool __cdecl motor_system_stunned(long)

//e_motor_result __cdecl motor_system_submit(long motor_index, c_motor_request const* request)
long __cdecl motor_system_submit(long motor_index, c_motor_request const* request)
{
	return INVOKE(0x00B749C0, motor_system_submit, motor_index, request);
}

//.text:00B74A20 ; bool __cdecl motor_system_update(long)
//.text:00B74AA0 ; bool __cdecl motor_task_update(long)

bool c_motor_task::program_default()
{
	return INVOKE_CLASS_MEMBER(0x00B74B40, c_motor_task, program_default);
}

bool c_motor_task::restricted_to(short a1) const
{
	return INVOKE_CLASS_MEMBER(0x00B74B60, c_motor_task, restricted_to, a1);
}

//bool c_motor_task::set(e_motor_task motor_task, e_action action, e_motor_program motor_program, short a4)
bool c_motor_task::set(long motor_task, long action, long motor_program, short a4)
{
	return INVOKE_CLASS_MEMBER(0x00B74B80, c_motor_task, set, motor_task, action, motor_program, a4);
}

bool c_motor_task::valid() const
{
	return INVOKE_CLASS_MEMBER(0x00B74BC0, c_motor_task, valid);
}

c_motor_request::c_motor_request() :
	__unknown0(0),
	__unknown1(0),
	m_task(),
	m_action(),
	m_storage()
{
}

void c_motor_request::setup_force_airborne(e_action action)
{
	m_task = 7;
	m_action = action;

	__unknown0 = 1;
}

