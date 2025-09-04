#include "input/controllers.hpp"

#include "cseries/cseries.hpp"

e_controller_index first_controller()
{
	return _controller0;
}

e_controller_index next_controller(e_controller_index controller_index)
{
	ASSERT(controller_index == k_no_controller || (controller_index >= 0 && controller_index < k_number_of_controllers));

	e_controller_index result = k_no_controller;
	if (controller_index >= _controller0 && controller_index < k_number_of_controllers - 1)
	{
		result = controller_index + 1;
	}

	return result;
}

