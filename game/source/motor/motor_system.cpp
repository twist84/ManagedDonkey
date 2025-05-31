#include "motor/motor_system.hpp"

void c_motor_task::clear()
{
	INVOKE_CLASS_MEMBER(0x00B744B0, c_motor_task, clear);
}

bool c_motor_task::conflicts(int16 a1) const
{
	return INVOKE_CLASS_MEMBER(0x00B744E0, c_motor_task, conflicts, a1);
}

//.text:00B74500 ; public: void __cdecl c_motor_task::interrupt()
//.text:00B74510 ; public: bool __cdecl c_motor_task::interrupted() const
//.text:00B74520 ; void __cdecl motor_system_animation_event(int32, c_motor_task*, int32)
//.text:00B74590 ; void __cdecl motor_system_event(int32, c_motor_task*, e_motor_event)
//.text:00B745E0 ; bool __cdecl motor_system_finished(int32, c_motor_task*, c_animation_channel*)
//.text:00B746B0 ; bool __cdecl motor_system_in_airborne_state(int32)
//.text:00B74700 ; bool __cdecl motor_system_in_contact_with_ground(int32)
//.text:00B74750 ; void __cdecl motor_system_interrupted(int32, c_motor_task*, bool)
//.text:00B74810 ; bool __cdecl motor_system_is_super_collidable_ragdoll(int32)
//.text:00B74860 ; bool __cdecl motor_system_object_post_update(int32)
//.text:00B74900 ; bool __cdecl motor_system_program_handles_task(e_motor_program, e_motor_task)
//.text:00B74920 ; int16 __cdecl motor_system_relaxation_state(int32)
//.text:00B74980 ; bool __cdecl motor_system_stunned(int32)

//e_motor_result __cdecl motor_system_submit(int32 motor_index, const c_motor_request* request)
int32 __cdecl motor_system_submit(int32 motor_index, const c_motor_request* request)
{
	return INVOKE(0x00B749C0, motor_system_submit, motor_index, request);
}

//.text:00B74A20 ; bool __cdecl motor_system_update(int32)
//.text:00B74AA0 ; bool __cdecl motor_task_update(int32)

bool c_motor_task::program_default()
{
	return INVOKE_CLASS_MEMBER(0x00B74B40, c_motor_task, program_default);
}

bool c_motor_task::restricted_to(int16 a1) const
{
	return INVOKE_CLASS_MEMBER(0x00B74B60, c_motor_task, restricted_to, a1);
}

//bool c_motor_task::set(e_motor_task motor_task, e_action action, e_motor_program motor_program, int16 a4)
bool c_motor_task::set(int32 motor_task, int32 action, int32 motor_program, int16 a4)
{
	return INVOKE_CLASS_MEMBER(0x00B74B80, c_motor_task, set, motor_task, action, motor_program, a4);
}

bool c_motor_task::valid() const
{
	return INVOKE_CLASS_MEMBER(0x00B74BC0, c_motor_task, valid);
}

c_motor_request::c_motor_request() :
	m_initialized(false),
	m_force(false),
	m_task(),
	m_action(),
	m_storage()
{
}

void c_motor_request::setup_force_airborne(e_action action)
{
	m_task = 7;
	m_action = action;

	m_initialized = true;
}

