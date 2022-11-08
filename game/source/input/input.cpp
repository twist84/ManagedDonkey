#include "input/input.hpp"

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

#include <assert.h>

bool __cdecl input_has_gamepad(short gamepad_index)
{
	assert(gamepad_index >= 0 && gamepad_index < k_number_of_controllers);

	return INVOKE(0x00511A40, input_has_gamepad, gamepad_index);
}

