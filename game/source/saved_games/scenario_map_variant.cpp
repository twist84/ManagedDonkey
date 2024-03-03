#include "saved_games/scenario_map_variant.hpp"

#include "cache/cache_files.hpp"

c_map_variant::c_map_variant()
{
	csmemset(this, 0, sizeof(*this));

	DECLFUNC(0x00580B80, void, __thiscall, c_map_variant*)(this);
}

//void __cdecl c_map_variant::create_default(e_map_id map_id)
void __cdecl c_map_variant::create_default(long map_id)
{
	DECLFUNC(0x00581F70, void, __thiscall, c_map_variant*, long)(this, map_id);
}

bool __cdecl c_map_variant::is_valid() const
{
	return DECLFUNC(0x005841D0, bool, __thiscall, c_map_variant const*)(this);
}

bool __cdecl c_map_variant::read_from(c_map_variant const* source)
{
	return DECLFUNC(0x005860F0, bool, __thiscall, c_map_variant*, c_map_variant const*)(this, source);
}

void __cdecl c_map_variant::set_description(char const* description)
{
	return DECLFUNC(0x00586460, void, __thiscall, c_map_variant*, char const*)(this, description);
}

void __cdecl c_map_variant::set_name(wchar_t const* name)
{
	return DECLFUNC(0x00586570, void, __thiscall, c_map_variant*, wchar_t const*)(this, name);
}

bool __cdecl c_map_variant::validate()
{
	return DECLFUNC(0x00586D00, bool, __thiscall, c_map_variant*)(this);
}

//e_map_id c_map_variant::get_map_id() const
long __cdecl c_map_variant::get_map_id() const
{
	return m_map_id;
}

#define PRINT_TABS c_console::write(tabs)

void s_variant_multiplayer_object_properties_definition::print(long const tab_count)
{
	char tabs[128]{};
	csmemset(tabs, ' ', tab_count * 4);

	PRINT_TABS; c_console::write_line("<item name = \"Symmetry Placement Flags\" value = %d>", (word)symmetry_placement_flags);
	PRINT_TABS; c_console::write_line("<item name = \"Game Engine Flags\" value = %d>", (byte)game_engine_flags);
	PRINT_TABS; c_console::write_line("<item name = \"Owner Team\" value = %d>", (long)owner_team);
	PRINT_TABS; c_console::write_line("<item name = \"Shared Storage\" value = %d>", shared_storage);
	PRINT_TABS; c_console::write_line("<item name = \"Spawn Rate\" value = %d>", spawn_rate);
	PRINT_TABS; c_console::write_line("<item name = \"Teleporter Channel\" value = %d>", (long)teleporter_channel);
	PRINT_TABS; c_console::write_line("<item name = \"Boundary Shape\" value = %d>", (long)boundary_shape);
	PRINT_TABS; c_console::write_line("<item name = \"Boundary Radius\" value = %.2f>", boundary_radius);
	PRINT_TABS; c_console::write_line("<item name = \"Boundary Box Length\" value = %.2f>", boundary_box_length);
	PRINT_TABS; c_console::write_line("<item name = \"Boundary +Height\" value = %.2f>", boundary_positive_height);
	PRINT_TABS; c_console::write_line("<item name = \"Boundary -Height\" value = %.2f>", boundary_negative_height);
}

void s_variant_object_datum::print(c_map_variant* map_variant, long const tab_count)
{
	char tabs[128]{};
	csmemset(tabs, ' ', tab_count * 4);

	PRINT_TABS; c_console::write_line("<item name = \"Flags\" value = %d>", (dword)flags);
	PRINT_TABS; c_console::write_line("<item name = \"Object Datum Index\" value = 0x%08X>", object_datum_index);
	PRINT_TABS; c_console::write_line("<item name = \"Editor Object Index\" value = %d>", editor_object_index);

	if (variant_quota_index != NONE)
	{
		PRINT_TABS; c_console::write_line("<menu name = \"Variant Quota\" index = %d>", variant_quota_index);
		map_variant->m_quotas[variant_quota_index].print(tab_count + 1);
		PRINT_TABS; c_console::write_line("</menu>");
	}

	PRINT_TABS; c_console::write_line("<item name = \"Position\" value = \"%.2f %.2f %.2f\">", position.x, position.y, position.z);
	PRINT_TABS; c_console::write_line("<item name = \"Forward\" value = \"%.2f %.2f %.2f\">", forward.i, forward.j, forward.k);
	PRINT_TABS; c_console::write_line("<item name = \"Up\" value = \"%.2f %.2f %.2f\">", up.i, up.j, up.k);
	PRINT_TABS; c_console::write_line("<item name = \"Parent Object ID: Unique ID\" value = 0x%08X>", parent_object_identifier.m_unique_id);
	PRINT_TABS; c_console::write_line("<item name = \"Parent Object ID: Origin BSP Index\" value = %d>", parent_object_identifier.m_origin_bsp_index);
	PRINT_TABS; c_console::write_line("<item name = \"Parent Object ID: Type\" value = %d>", (long)parent_object_identifier.m_type);
	PRINT_TABS; c_console::write_line("<item name = \"Parent Object ID: Source\" value = %d>", (long)parent_object_identifier.m_source);

	PRINT_TABS; c_console::write_line("<menu name = \"Multiplayer Game Object Properties\">", variant_quota_index);
	multiplayer_game_object_properties.print(tab_count + 1);
	PRINT_TABS; c_console::write_line("</menu>");
}

void s_variant_quota::print(long const tab_count)
{
	char tabs[128]{};
	csmemset(tabs, ' ', tab_count * 4);

	PRINT_TABS; c_console::write("<item name = \"Object\" value = ");
	if (object_definition_index != NONE)
	{
		char const* tag_name = tag_get_name_safe(object_definition_index);
		c_console::write("\"%s\"", tag_name);
	}
	else
	{
		c_console::write("%d", object_definition_index);
	}
	c_console::write_line(">");

	PRINT_TABS; c_console::write_line("<item name = \"Minimum Count\" value = %d>", minimum_count);
	PRINT_TABS; c_console::write_line("<item name = \"Maximum Count\" value = %d>", maximum_count);
	PRINT_TABS; c_console::write_line("<item name = \"Placed On Map\" value = %d>", placed_on_map);
	PRINT_TABS; c_console::write_line("<item name = \"Maximum Allowed\" value = %d>", maximum_allowed);
	PRINT_TABS; c_console::write_line("<item name = \"Price Per Item\" value = %.2f>", price_per_item);
}

void c_map_variant::print()
{
	//m_metadata.print();

	long const tab_count = 1;

	char tabs[128]{};
	csmemset(tabs, ' ', tab_count * 4);

	c_console::write_line("<menu name = \"Map Variant\">");
	PRINT_TABS; c_console::write_line("<item name = \"Map Variant Version\" value = %d>", m_map_variant_version);
	PRINT_TABS; c_console::write_line("<item name = \"Number Of Scenario Objects\" value = %d>", m_number_of_scenario_objects);
	PRINT_TABS; c_console::write_line("<item name = \"Number Of Variant Objects\" value = %d>", m_number_of_variant_objects);
	PRINT_TABS; c_console::write_line("<item name = \"Number Of Placeable Object Quotas\" value = %d>", m_number_of_placeable_object_quotas);
	PRINT_TABS; c_console::write_line("<item name = \"Map ID\" value = %d>", m_map_id);

	PRINT_TABS; c_console::write_line("<item name = \"World Bounds: X\" value = \"%.2f %.2f\">", m_world_bounds.x.lower, m_world_bounds.x.upper);
	PRINT_TABS; c_console::write_line("<item name = \"World Bounds: Y\" value = \"%.2f %.2f\">", m_world_bounds.y.lower, m_world_bounds.y.upper);
	PRINT_TABS; c_console::write_line("<item name = \"World Bounds: Z\" value = \"%.2f %.2f\">", m_world_bounds.z.lower, m_world_bounds.z.upper);

	PRINT_TABS; c_console::write_line("<item name = \"Game Engine Subtype\" value = %d>", m_game_engine_subtype);
	PRINT_TABS; c_console::write_line("<item name = \"Maximum Budget\" value = %d>", m_maximum_budget);
	PRINT_TABS; c_console::write_line("<item name = \"Spent Budget\" value = %d>", m_spent_budget);
	PRINT_TABS; c_console::write_line("<item name = \"Helpers Enabled\" value = \"%s\">", m_helpers_enabled ? "true" : "false");
	PRINT_TABS; c_console::write_line("<item name = \"Built-in\" value = \"%s\">", m_built_in ? "true" : "false");
	PRINT_TABS; c_console::write_line("<item name = \"Map Variant Checksum\" value = 0x%08X>", m_map_variant_checksum);

	for (long i = 0; i < m_variant_objects.get_count(); i++)
	{
		if (m_variant_objects[i].object_datum_index == NONE)
			continue;

		PRINT_TABS; c_console::write_line("<menu name = \"Variant Object\" index = %d>", i);
		m_variant_objects[i].print(this, tab_count + 1);
		PRINT_TABS; c_console::write_line("</menu>");
	}

	for (long i = 0; i < m_object_type_start_index.get_count(); i++)
	{
		PRINT_TABS; c_console::write_line("<item name = \"Object Type Start Index\" index = %d value = %d>", i, m_object_type_start_index[i]);
	}

	for (long i = 0; i < m_quotas.get_count(); i++)
	{
		if (m_quotas[i].object_definition_index == NONE)
			continue;

		PRINT_TABS; c_console::write_line("<menu name = \"Variant Quota\" index = %d>", i);
		m_quotas[i].print(tab_count + 1);
		PRINT_TABS; c_console::write_line("</menu>");
	}

	for (long i = 0; i < m_chunk_simulation_object_glue_indices.get_count(); i++)
	{
		PRINT_TABS; c_console::write_line("<item name = \"Chunk Simulation Object Glue Index\" index = %d value = %d>", i, m_chunk_simulation_object_glue_indices[i]);
	}
	c_console::write_line("</menu>");
}

#undef PRINT_TABS

