#include "motor/motor_system.hpp"

//e_motor_result __cdecl motor_system_submit(long motor_index, c_motor_request const* request)
long __cdecl motor_system_submit(long motor_index, c_motor_request const* request)
{
	return INVOKE(0x00B749C0, motor_system_submit, motor_index, request);
}

