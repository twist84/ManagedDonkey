#include "saved_games/scenario_map_variant.hpp"

#include "cache/cache_files.hpp"

#include <string.h>

c_map_variant::c_map_variant()
{
	memset(this, 0, sizeof(*this));

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
	memset(tabs, ' ', tab_count * 4);

	PRINT_TABS; c_console::write_line("symmetry_placement_flags: 0x%04X", (word)symmetry_placement_flags);
	PRINT_TABS; c_console::write_line("game_engine_flags: 0x%02X", (byte)game_engine_flags);
	PRINT_TABS; c_console::write_line("owner_team: %d", (long)owner_team);
	PRINT_TABS; c_console::write_line("spare_clips/teleporter_channel/spawn_order: %d", shared_storage.spawn_order);
	PRINT_TABS; c_console::write_line("spawn_rate: %d", spawn_rate);
	PRINT_TABS; c_console::write_line("teleporter_channel: %d", (long)teleporter_channel);
	PRINT_TABS; c_console::write_line("boundary_shape: %d", (long)boundary_shape);
	PRINT_TABS; c_console::write_line("boundary_radius/boundary_width: %.2f", boundary_radius);
	PRINT_TABS; c_console::write_line("boundary_box_length: %.2f", boundary_box_length);
	PRINT_TABS; c_console::write_line("boundary_positive_height: %.2f", boundary_positive_height);
	PRINT_TABS; c_console::write_line("boundary_negative_height: %.2f", boundary_negative_height);
}

void s_variant_object_datum::print(c_map_variant* map_variant, long const tab_count)
{
	if (variant_quota_index == NONE)
		return;

	char tabs[128]{};
	memset(tabs, ' ', tab_count * 4);

	PRINT_TABS; c_console::write_line("flags: 0x%04X", (dword)flags);
	PRINT_TABS; c_console::write_line("object_datum_index: 0x%08X", object_datum_index);
	PRINT_TABS; c_console::write_line("editor_object_index: 0x%08X", editor_object_index);

	PRINT_TABS; c_console::write_line("variant_quota_index = %d", variant_quota_index);
	if (variant_quota_index != NONE)
	{
		PRINT_TABS; c_console::write_line("map_variant::m_quotas[%d]:", variant_quota_index);
		PRINT_TABS; c_console::write_line("{");
		map_variant->m_quotas[variant_quota_index].print(tab_count + 1);
		PRINT_TABS; c_console::write_line("}");

		printf("");
	}
	PRINT_TABS; c_console::write_line("position: { .i = %.2f, .j = %.2f, .k = %.2f }", position.x, position.y, position.z);
	PRINT_TABS; c_console::write_line("forward: { .i = %.2f, .j = %.2f, .k = %.2f }", forward.i, forward.j, forward.k);
	PRINT_TABS; c_console::write_line("up: { .i = %.2f, .j = %.2f, .k = %.2f }", up.i, up.j, up.k);
	PRINT_TABS; c_console::write_line("parent_object_identifier.m_unique_id: 0x%08X", parent_object_identifier.m_unique_id);
	PRINT_TABS; c_console::write_line("parent_object_identifier.m_origin_bsp_index: %d", parent_object_identifier.m_origin_bsp_index);
	PRINT_TABS; c_console::write_line("parent_object_identifier.m_type: %d", (long)parent_object_identifier.m_type);
	PRINT_TABS; c_console::write_line("parent_object_identifier.m_source: %d", (long)parent_object_identifier.m_source);

	PRINT_TABS; c_console::write_line("multiplayer_game_object_properties:");
	PRINT_TABS; c_console::write_line("{");
	multiplayer_game_object_properties.print(tab_count + 1);
	PRINT_TABS; c_console::write_line("}");
}

void s_variant_quota::print(long const tab_count)
{
	if (object_definition_index == NONE)
		return;

	char tabs[128]{};
	memset(tabs, ' ', tab_count * 4);

	PRINT_TABS; c_console::write("object: { definition_index = 0x%08X", object_definition_index);
	if (object_definition_index != NONE)
	{
		char const* tag_name = tag_get_name_safe(object_definition_index);
		c_console::write(", tag_name = '%s'", tag_name);

		printf("");
	}
	c_console::write_line(" }");

	PRINT_TABS; c_console::write_line("minimum_count: %d", minimum_count);
	PRINT_TABS; c_console::write_line("maximum_count: %d", maximum_count);
	PRINT_TABS; c_console::write_line("placed_on_map: %d", placed_on_map);
	PRINT_TABS; c_console::write_line("maximum_allowed: %d", maximum_allowed);
	PRINT_TABS; c_console::write_line("price_per_item: %.2f", price_per_item);
}

void c_map_variant::print()
{
	//m_metadata.print();

	long const tab_count = 1;

	char tabs[128]{};
	memset(tabs, ' ', tab_count * 4);

	PRINT_TABS; c_console::write_line("m_map_variant_version: %d", m_map_variant_version);
	PRINT_TABS; c_console::write_line("m_number_of_scenario_objects: %d", m_number_of_scenario_objects);
	PRINT_TABS; c_console::write_line("m_number_of_variant_objects: %d", m_number_of_variant_objects);
	PRINT_TABS; c_console::write_line("m_number_of_placeable_object_quotas: %d", m_number_of_placeable_object_quotas);
	PRINT_TABS; c_console::write_line("m_map_id: %d", m_map_id);

	PRINT_TABS; c_console::write_line("m_world_bounds: { .x = { .lower = %.2f, .upper = %.2f }, .y = { .lower = %.2f, .upper = %.2f }, .z = { .lower = %.2f, .upper = %.2f } }",
		m_world_bounds.x.lower, m_world_bounds.x.upper,
		m_world_bounds.y.lower, m_world_bounds.y.upper,
		m_world_bounds.z.lower, m_world_bounds.z.upper);

	PRINT_TABS; c_console::write_line("m_game_engine_subtype: %d", m_game_engine_subtype);
	PRINT_TABS; c_console::write_line("m_maximum_budget: %.2f", m_maximum_budget);
	PRINT_TABS; c_console::write_line("m_spent_budget: %.2f", m_spent_budget);
	PRINT_TABS; c_console::write_line("m_helpers_enabled: %s", m_helpers_enabled ? "true" : "false");
	PRINT_TABS; c_console::write_line("m_built_in: %s", m_built_in ? "true" : "false");
	PRINT_TABS; c_console::write_line("m_map_variant_checksum: 0x%08X", m_map_variant_checksum);

	for (long i = 0; i < m_variant_objects.get_count(); i++)
	{
		PRINT_TABS; c_console::write_line("m_variant_objects[%d]:", i);
		PRINT_TABS; c_console::write_line("{");
		m_variant_objects[i].print(this, tab_count + 1);
		PRINT_TABS; c_console::write_line("}");
	}

	for (long i = 0; i < m_object_type_start_index.get_count(); i++)
	{
		PRINT_TABS; c_console::write_line("m_object_type_start_index[%d]: %d", i, m_object_type_start_index[i]);
	}

	//for (long i = 0; i < m_quotas.count(); i++)
	//{
	//	PRINT_TABS; c_console::write_line("m_quotas[%d]:", i);
	//	PRINT_TABS; c_console::write_line("{");
	//	m_quotas[i].print(tab_count + 1);
	//	PRINT_TABS; c_console::write_line("}");
	//}

	for (long i = 0; i < m_chunk_simulation_object_glue_indices.get_count(); i++)
	{
		PRINT_TABS; c_console::write_line("m_chunk_simulation_object_glue_indices[%d]: 0x%08X", i, m_chunk_simulation_object_glue_indices[i]);
	}
}

#undef PRINT_TABS

