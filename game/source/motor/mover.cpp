#include "motor/mover.hpp"

//e_motor_program __cdecl mover_get_motor_program(long motor_index)
long __cdecl mover_get_motor_program(long motor_index)
{
	return INVOKE(0x00BA66A0, mover_get_motor_program, motor_index);
}

