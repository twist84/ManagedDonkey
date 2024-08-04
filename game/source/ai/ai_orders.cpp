#include "ai/ai_orders.hpp"

#include "scenario/scenario.hpp"

short orders_get_by_name(char const* name)
{
	struct scenario* scenario = global_scenario_get();
	if (scenario)
	{
		for (short orders_index = 0; orders_index < scenario->orders.count(); orders_index++)
		{
			orders_definition& orders = scenario->orders[orders_index];
			if (csstrnicmp(orders.name.get_string(), name, orders.name.length()) == 0)
				return orders_index;
		}
	}

	return NONE;
}

