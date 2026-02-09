#include "ai/ai_orders.hpp"

#include "scenario/scenario.hpp"

int16 orders_get_by_name(const char* name)
{
	s_scenario* scenario = global_scenario_get();
	if (scenario)
	{
		for (int16 orders_index = 0; orders_index < scenario->orders.count; orders_index++)
		{
			orders_definition& orders = scenario->orders[orders_index];
			if (csstrnicmp(orders.name, name, sizeof(orders.name)) == 0)
				return orders_index;
		}
	}

	return NONE;
}

