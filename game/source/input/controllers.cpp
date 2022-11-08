#include "input/controllers.hpp"

#include "shell/shell.hpp"

#include <assert.h>

long first_controller()
{
	return 0;
}

long next_controller(long controller_index)
{
	assert(controller_index == k_no_controller || (controller_index >= 0 && controller_index < k_number_of_controllers));

	long result = k_no_controller;
	if (controller_index >= 0 && controller_index < k_number_of_controllers - 1)
		result = controller_index + 1;

	return result;
}

