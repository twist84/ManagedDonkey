#include "ai/objectives.hpp"

#include "ai/ai_debug.hpp"
#include "ai/squads.hpp"
#include "memory/thread_local.hpp"
#include "render/render_debug.hpp"

char const* const inhibit_behavior_flag_names[k_inhibit_behavior_flags]
{
	"Cover",
	"Retreat",
	"Vehicles",
	"Grenades",
	"Berserk",
	"Equipment",
	"Pureform Ranged",
	"Pureform Tank",
	"Pureform Stalker"
};

s_task_record* __cdecl objective_get_task_record(short objective_index, short task_index)
{
	return INVOKE(0x01448840, objective_get_task_record, objective_index, task_index);
}

void ai_debug_render_objectives(long squad_index, real_point3d const* position)
{
	TLS_DATA_GET_VALUE_REFERENCE(squad_data);

	squad_datum* squad = (squad_datum*)datum_get_absolute(*squad_data, DATUM_INDEX_TO_ABSOLUTE_INDEX(squad_index));
	if (squad->objective_index != NONE)
	{
		s_objective& objective = global_scenario_get()->ai_objectives[squad->objective_index];

		s_task* task = NULL;
		if (squad->task_index != NONE)
			task = &objective.tasks[squad->task_index];

		if (squad->task_index != NONE && squad->objective_start_time != NONE)
		{
			if (game_time_get() - squad->objective_start_time < game_seconds_to_ticks_round(2.0))
			{
				short zone_area_count = 0;
				struct scenario* scenario = global_scenario_get();
				real_point3d point = *global_origin3d;

				ASSERT(task);

				if (task->areas.count() > 0)
				{
					for (short task_area_index = 0; task_area_index < task->areas.count(); task_area_index++)
					{
						s_area_reference& area_ref = task->areas[task_area_index];
						if (area_ref.zone >= 0 && area_ref.zone < scenario->zones.count())
						{
							//zone_definition* zone = zone_definition_get(area_ref.zone);
							zone_definition& zone = scenario->zones[area_ref.zone];

							if (area_ref.area >= 0 && area_ref.area < zone.areas.count())
							{
								area_definition& area_def = zone.areas[area_ref.area];
								add_vectors3d((vector3d*)&point, (vector3d*)&area_def.runtime_relative_mean_point, (vector3d*)&point);
								zone_area_count++;
							}
						}
					}

					if (zone_area_count > 0)
					{
						scale_vector3d((vector3d*)&point, 1.0f / zone_area_count, (vector3d*)&point);
						render_debug_line(true, position, &point, global_real_argb_yellow);
					}
				}
			}
		}

		char const* objective_name = objective.name.get_string();

		if (task)
		{
			s_task_record* record = objective_get_task_record(squad->objective_index, squad->task_index);
		
			real_argb_color const* group_color = global_real_argb_green;
			if (TEST_BIT(record->flags, 2))
				group_color = global_real_argb_red;

			char string[258]{};

			// Group State
		
			csnzprintf(string, sizeof(string), "[%s]", group_state_names[record->state.m_state]);
			render_debug_string_at_point(ai_debug_drawstack(), string, group_color);

			// Exhausting

			if (record->__time20 != NONE)
			{
				long ticks = record->__time20 - game_time_get();
				if (ticks > 0)
				{
					csnzprintf(string, sizeof(string), "Exhausting: %.2f", game_ticks_to_seconds(real(ticks)));
					render_debug_string_at_point(ai_debug_drawstack(), string, global_real_argb_red);
				}
			}

			// Deactivate

			if (task->maximum_duration > 0.0f)
			{
				long ticks = record->__time24 - game_time_get() + game_seconds_to_ticks_round(task->maximum_duration);
				if (ticks > 0)
				{
					csnzprintf(string, sizeof(string), "Deactivate in: %.2f", game_ticks_to_seconds(real(ticks)));
					render_debug_string_at_point(ai_debug_drawstack(), string, global_real_argb_red);
				}
			}

			// Engage

			real_argb_color const* engage_color = global_real_argb_red;
			if (TEST_BIT(record->flags, 12))
				engage_color = global_real_argb_green;
			else if (record->__time14 != NONE)
				engage_color = global_real_argb_yellow;
			render_debug_string_at_point(ai_debug_drawstack(), "engage", engage_color);

			// Search

			real_argb_color const* search_color = global_real_argb_red;
			if (TEST_BIT(record->flags, 10))
				search_color = global_real_argb_green;
			else if (TEST_BIT(record->flags, 11))
				search_color = global_real_argb_yellow;
			render_debug_string_at_point(ai_debug_drawstack(), "search", search_color);

			// Group Grenade

			real_argb_color const* group_grenade_color = global_real_argb_red;
			if (record->__time18 != NONE)
			{
				ASSERT(record->leader);

				if (game_time_get() - record->__time18 < game_seconds_to_ticks_round(2.0f))
					group_grenade_color = global_real_argb_green;
			}
			render_debug_string_at_point(ai_debug_drawstack(), "group grenade", group_grenade_color);

			// Group Name

			char const* task_name = task->name.get_string();
			csnzprintf(string, sizeof(string), "%s/%s", objective_name, task_name);
			render_debug_string_at_point(ai_debug_drawstack(), string, group_color);

			// Inhibit Groups
			for (short inhibit_group_bit = 0; inhibit_group_bit < k_inhibit_behavior_flags; inhibit_group_bit++)
			{
				if (task->inhibit_groups.test((e_inhibit_behavior_flags)inhibit_group_bit))
				{
					csnzprintf(string, sizeof(string), "- %s", inhibit_behavior_flag_names[inhibit_group_bit]);
					render_debug_string_at_point(ai_debug_drawstack(), string, global_real_argb_red);
				}
			}
		}
		else
		{
			render_debug_string_at_point(ai_debug_drawstack(), objective_name, global_real_argb_red);
		}
	}
}

