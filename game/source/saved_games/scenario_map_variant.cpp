#include "saved_games/scenario_map_variant.hpp"

#include "cache/cache_files.hpp"

//.text:00580B00 ; 
//.text:00580B10 ; 
//.text:00580B20 ; 
//.text:00580B30 ; 
//.text:00580B50 ; 

c_map_variant::c_map_variant()
{
	csmemset(this, 0, sizeof(*this));

	DECLFUNC(0x00580B80, void, __thiscall, c_map_variant*)(this);
}

//.text:00580C10 ; 
//.text:00580C20 ; 
//.text:00580C40 ; 

//c_map_variant::~c_map_variant()
//{
//	DECLFUNC(0x00580C60, void, __thiscall, c_map_variant*)(this);
//}

//.text:00580C80 ; 
//.text:00580CA0 ; 
//.text:00580CC0 ; 
//.text:00580CE0 ; 
//.text:00580CF0 ; 
//.text:00580D00 ; 
//.text:00580D10 ; 
//.text:00580D20 ; 
//.text:00580D30 ; 
//.text:00580D40 ; 
//.text:00580D50 ; 
//.text:00580D60 ; 
//.text:00580D70 ; 
//.text:00580D80 ; 
//.text:00580D90 ; 
//.text:00580E30 ; public: void c_map_variant::apply_multiplayer_properties(long, long) const
//.text:00580E80 ; public: void c_map_variant::apply_multiplayer_properties(s_variant_multiplayer_object_properties_definition const*, long) const
//.text:00580FE0 ; public: virtual bool __cdecl c_simulation_map_variant_entity_definition::update_game_entity(long, dword, long, void const*)
//.text:00581400 ; public: bool c_map_variant::build_simulation_baseline(long, long, void*)
//.text:005816C0 ; 
//.text:00581B90 ; public: bool c_map_variant::can_place_scenario_object(e_object_type, long) const
//.text:00581C20 ; 
//.text:00581C90 ; public: static bool c_map_variant::collect_object_type(enum e_object_type)
//.text:00581CC0 ; 
//.text:00581D60 ; 

//void c_map_variant::create_default(e_map_id map_id)
void c_map_variant::create_default(long map_id)
{
	DECLFUNC(0x00581F70, void, __thiscall, c_map_variant*, long)(this, map_id);
}

//.text:00582110 ; public: long c_map_variant::create_object(long, long, long, real_point3d const*, vector3d const*, vector3d const*, long, e_object_type, s_variant_multiplayer_object_properties_definition const*, word)

bool c_map_variant::decode(c_bitstream* packet)
{
	return DECLFUNC(0x00582550, bool, __thiscall, c_map_variant const*, c_bitstream*)(this, packet);
}

//.text:00582930 ; 
//.text:00582940 ; 

void c_map_variant::encode(c_bitstream* packet) const
{
	DECLFUNC(0x005829A0, void, __thiscall, c_map_variant const*, c_bitstream*)(this, packet);
}

//.text:005831C0 ; public: long c_map_variant::find_definition_index(long)
//.text:00583200 ; 
//.text:00583210 ; 
//.text:00583220 ; 
//.text:00583230 ; c_map_variant* __cdecl game_engine_get_runtime_map_variant()
//.text:00583250 ; public: real c_map_variant::get_budget_fraction(long*, long*) const
//.text:005832A0 ; 
//.text:005832E0 ; public: long c_map_variant::get_object_index(long) const
//.text:00583300 ; public: s_variant_quota const* c_map_variant::get_object_quota_datum(long)
//.text:00583320 ; 
//.text:005833F0 ; public: bool c_map_variant::get_object_scenario_properties(long, e_object_type*, long*)
//.text:005834E0 ; 
//.text:00583530 ; 
//.text:005835F0 ; 
//.text:00583600 ; public: s_variant_object_datum const* c_map_variant::get_variant_object_datum(long) const
//.text:00583630 ; public: void c_map_variant::get_variant_object_datum(long, s_variant_object_datum*) const
//.text:005836C0 ; public: void c_map_variant::get_variant_object_spawn_location(long, real_point3d*, vector3d*, vector3d*)
//.text:00583860 ; public: s_variant_quota* c_map_variant::get_writable_object_quota_datum(long)
//.text:00583880 ; public: void c_map_variant::handle_deleted_object(long)
//.text:00583920 ; 
//.text:00583B20 ; public: void c_map_variant::initialize_for_scenario()
//.text:00583F90 ; 
//.text:00583FA0 ; public: __cdecl s_variant_multiplayer_object_properties_definition::s_variant_multiplayer_object_properties_definition()
//.text:00583FD0 ; 
//.text:00583FF0 ; 
//.text:00584010 ; 
//.text:00584030 ; 
//.text:00584050 ; 
//.text:00584070 ; public: bool c_map_variant::is_equal_to(c_map_variant const*) const
//.text:005841C0 ; public: bool c_map_variant::is_showing_helpers()

bool c_map_variant::is_valid() const
{
	return DECLFUNC(0x005841D0, bool, __thiscall, c_map_variant const*)(this);
}

bool c_map_variant::sub_5842F0(c_map_variant* map_variant)
{
	return DECLFUNC(0x005842F0, bool, __thiscall, c_map_variant*, c_map_variant*)(this, map_variant);
}

//.text:005845C0 ; 
//.text:005845E0 ; 
//.text:00584600 ; 
//.text:00584610 ; 
//.text:00584620 ; void __cdecl map_variant_create_helper_object(long)
//.text:00584790 ; why is `c_simulation_object_entity_definition::object_create_object` here?
//.text:005848D0 ; public: void c_map_variant::create_all_objects() const
//.text:005849D0 ; public: long c_map_variant::create_object(long) const
//.text:00584A70 ; 
//.text:00584CF0 ; public: void c_map_variant::quantize(void)
//.text:00584D80 ; public: void c_map_variant::recalculate_budget()
//.text:00584E10 ; public: void c_map_variant::recalculate_quota(long, bool)
//.text:00584FA0 ; public: void c_map_variant::recover_from_promotion_to_simulation_authority()
//.text:005855E0 ; public: void c_map_variant::register_object(long, long)
//.text:00585840 ; public: void c_map_variant::register_scenario_object(e_object_type, long)
//.text:00585890 ; public: bool c_map_variant::register_scenario_object(long, bool)
//.text:00585C00 ; public: void c_map_variant::remove_object(long, e_object_removal_origin)
//.text:00585DE0 ; 
//.text:00585E50 ; 
//.text:00585EE0 ; public: void c_map_variant::reset_objects()
//.text:00586090 ; public: void c_map_variant::sanity_check(long)

bool c_map_variant::read_from(c_map_variant const* source)
{
	return DECLFUNC(0x005860F0, bool, __thiscall, c_map_variant*, c_map_variant const*)(this, source);
}

//.text:00586270 ; s_scenario_multiplayer_object_properties const* __cdecl scenario_multiplayer_object_properties_try_and_get(e_object_type, long)
//.text:00586300 ; 
//.text:00586330 ; 
//.text:00586350 ; 
//.text:00586370 ; 
//.text:005863A0 ; 
//.text:005863C0 ; 
//.text:005863F0 ; 
//.text:00586420 ; 
//.text:00586440 ; public: void c_map_variant::set_client_spent_budget(long)

void c_map_variant::set_description(char const* description)
{
	return DECLFUNC(0x00586460, void, __thiscall, c_map_variant*, char const*)(this, description);
}

//.text:005864A0 ; public: void c_map_variant::set_metadata(s_content_item_metadata const*)

void c_map_variant::set_name(wchar_t const* name)
{
	return DECLFUNC(0x00586570, void, __thiscall, c_map_variant*, wchar_t const*)(this, name);
}

//.text:005865B0 ;
//.text:005865C0 ;
//.text:005865D0 ; public: void c_map_variant::set_variant_object_datum(long, s_variant_object_datum const*, bool)
//.text:00586680 ; public: void c_map_variant::setup_object_properties(s_variant_object_datum*)
//.text:00586860 ; public: void c_map_variant::show_helpers(bool)
//.text:005868E0 ; 
//.text:00586920 ; 
//.text:00586940 ; 
//.text:00586960 ; 
//.text:00586A00 ; public: void c_map_variant::update()
//.text:00586A90 ; void __cdecl update_helper_position(long)
//.text:00586C20 ; public: bool c_map_variant::update_scenario_object_properties(long) const

bool c_map_variant::validate()
{
	return DECLFUNC(0x00586D00, bool, __thiscall, c_map_variant*)(this);
}

//.text:00586D20 ; 
//.text:00587220 ; 

//e_map_id c_map_variant::get_map_id() const
long c_map_variant::get_map_id() const
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
	PRINT_TABS; c_console::write_line("<item name = \"Object Type\" value = %d>", object_type);
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

	for (long i = 0; i < m_simulation_entities.get_count(); i++)
	{
		PRINT_TABS; c_console::write_line("<item name = \"Simulation Entity Index\" index = %d value = %d>", i, m_simulation_entities[i]);
	}
	c_console::write_line("</menu>");
}

#undef PRINT_TABS

