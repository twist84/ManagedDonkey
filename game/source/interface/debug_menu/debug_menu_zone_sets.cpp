#include "interface/debug_menu/debug_menu_zone_sets.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "interface/debug_menu/debug_menu_item_numbered.hpp"
#include "interface/debug_menu/debug_menu_main.hpp"
#include "main/main.hpp"
#include "scenario/scenario.hpp"

c_debug_menu_zone_sets::~c_debug_menu_zone_sets()
{
}

void c_debug_menu_zone_sets::notify_selected(int16 selected_value)
{
	if (VALID_INDEX(get_selection(), global_scenario_get()->zone_sets.count))
	{
		main_switch_zone_set(selected_value);
	}
	else
	{
		event(_event_critical, "this should be a valid zone set index WTF???");
	}
}

void c_debug_menu_zone_sets::open()
{
	c_debug_menu_scroll::open();
	update_caption();
}

void c_debug_menu_zone_sets::notify_up()
{
	c_debug_menu::notify_up();
	update_caption();
}

void c_debug_menu_zone_sets::notify_down()
{
	c_debug_menu::notify_down();
	update_caption();
}

c_debug_menu_zone_sets::c_debug_menu_zone_sets(c_debug_menu* parent, int16 num_visible, const char* name_ptr) :
	c_debug_menu_scroll(parent, num_visible, name_ptr)
{
	csstrnzcpy(m_caption, "", sizeof(m_caption));

	for (s_scenario_zone_set& zone_set : global_scenario_get()->zone_sets)
		add_item(DEBUG_MENU_MALLOC(c_debug_menu_item_numbered, this, zone_set.name.get_string(), NULL));
}

void c_debug_menu_zone_sets::update_caption()
{
	char caption[1024]{};

	if (VALID_INDEX(get_selection(), global_scenario_get()->zone_sets.count))
		scenario_get_structure_bsp_string_from_mask(global_scenario_get()->zone_sets[get_selection()].flags, caption, sizeof(caption));

	set_caption(caption);
}

