#include "ai/cs_scenario_definitions.hpp"

#include "scenario/scenario.hpp"

short cs_point_set_index_by_name(char const* name)
{
	struct scenario* scenario = global_scenario_get();
	if (scenario->scripting_data.count() > 0)
	{
		for (short point_set_index = 0; point_set_index < scenario->scripting_data[0].point_sets.count(); point_set_index++)
		{
			cs_point_set& point_set = scenario->scripting_data[0].point_sets[point_set_index];
			if (point_set.name.is_equal(name))
				return point_set_index;
		}
	}

	return NONE;
}

short cs_point_index_by_name(cs_point_set* point_set, char const* name)
{
	for (short point_index = 0; point_index < point_set->points.count(); point_index++)
	{
		cs_point& point = point_set->points[point_index];
		if (point.name.is_equal(name))
			return point_index;
	}

	return NONE;
}

