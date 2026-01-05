#include "saved_games/scenario_map_variant.hpp"

#include "cache/cache_files.hpp"
#include "memory/bitstream.hpp"
#include "memory/module.hpp"
#include "simulation/simulation_encoding.hpp"

static void* map_variant_get_unique_id = member_to_static_function(&c_map_variant::get_unique_id_);
HOOK_DECLARE_CALL(0x00AEB265, map_variant_get_unique_id);

uns64 c_map_variant::get_unique_id_()
{
	uns64 unique_id = c_map_variant::get_unique_id();
	if (unique_id == 0)
	{
		unique_id = s_saved_game_item_metadata::generate_unique_id();
	}
	return unique_id;
}

//.text:00580B00 ; 
//.text:00580B10 ; 
//.text:00580B20 ; 
//.text:00580B30 ; public: c_static_array<s_variant_object_datum, 640>::c_static_array<s_variant_object_datum, 640>()
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
//.text:00580D90 ; void __cdecl apply_map_variant_multiplayer_object_properties_to_scenario_object_properties(const s_variant_multiplayer_object_properties_definition*, s_scenario_multiplayer_object_properties*)
//.text:00580E30 ; public: void c_map_variant::apply_multiplayer_properties(int32, int32) const
//.text:00580E80 ; public: void c_map_variant::apply_multiplayer_properties(const s_variant_multiplayer_object_properties_definition*, int32) const
//.text:00580FE0 ; public: virtual bool c_simulation_map_variant_entity_definition::update_game_entity(int32, uns32, int32, const void*)
//.text:00581400 ; public: bool c_map_variant::build_simulation_baseline(int32, int32, void*)
//.text:005816C0 ; public: bool c_map_variant::build_simulation_update(int32, uns32*, int32, void*)
//.text:00581B90 ; public: bool c_map_variant::can_place_scenario_object(e_object_type, int32) const
//.text:00581C20 ; public: void c_map_variant::change_map_variant_index(int32, int16)
//.text:00581C90 ; public: static bool c_map_variant::collect_object_type(e_object_type)
//.text:00581CC0 ; public: void c_map_variant::copy_quotas_from(const c_map_variant*)
//.text:00581D60 ; public: void c_map_variant::create_additional_objects() const

void c_map_variant::create_default(e_map_id map_id)
{
	INVOKE_CLASS_MEMBER(0x00581F70, c_map_variant, create_default, map_id);
}

//.text:00582110 ; public: int32 c_map_variant::create_object(int32, int32, int32, const real_point3d*, const real_vector3d*, const real_vector3d*, int32, e_object_type, const s_variant_multiplayer_object_properties_definition*, uns16)

bool c_map_variant::decode(c_bitstream* packet)
{
	return INVOKE_CLASS_MEMBER(0x00582550, c_map_variant, decode, packet);

	//m_metadata.decode(packet);
	//m_map_variant_version = (int16)packet->read_integer("map-variant-version", 8);
	//m_original_map_signature_hash = packet->read_integer("map-variant-checksum", 32);
	//m_number_of_scenario_objects = (int16)packet->read_integer("number_of_scenario_objects", 10);
	//m_number_of_variant_objects = (int16)packet->read_integer("number_of_variant_objects", 10);
	//m_number_of_placeable_object_quotas = (int16)packet->read_integer("number_of_placeable_object_quotas", 9);
	//
	//bool variant_has_objects = VALID_INDEX(m_number_of_variant_objects, 640) && VALID_INDEX(m_number_of_placeable_object_quotas, 256);
	//
	//m_map_id = (e_map_id)packet->read_integer("map_id", 32);
	//m_built_in = packet->read_bool("built_in");
	//packet->read_bits_internal(&m_variant_scenario_bounds, SIZEOF_BITS(m_variant_scenario_bounds)); // world-bounds
	//m_game_engine_subtype = (int32)packet->read_integer("game_engine_subtype", 4);
	//packet->read_bits_internal(&m_maximum_budget, SIZEOF_BITS(m_maximum_budget)); // maximum_budget
	//packet->read_bits_internal(&m_spent_budget, SIZEOF_BITS(m_spent_budget)); // spent_budget
	//
	//bool result = variant_has_objects;
	//
	//if (!variant_has_objects)
	//	return false;
	//
	//for (int32 variant_object_index = 0; variant_object_index < 640; variant_object_index++)
	//{
	//	s_variant_object_datum& variant_object = m_variant_objects[variant_object_index];
	//
	//	if (variant_object_index >= m_number_of_variant_objects)
	//	{
	//		variant_object = s_variant_object_datum{};
	//		continue;
	//	}
	//
	//	if (!packet->read_bool("variant_object_exists"))
	//	{
	//		variant_object = s_variant_object_datum{};
	//		continue;
	//	}
	//
	//	variant_object.flags.set_unsafe((uns16)packet->read_integer("variant-object-flags", 16));
	//	variant_object.variant_quota_index = (int32)packet->read_integer("variant-object-definition-index", 32);
	//
	//	if (packet->read_bool("parent-object-exists"))
	//	{
	//		packet->read_bits_internal(&variant_object.spawn_attached_to, 64); // parent-object-identifier
	//	}
	//	else
	//	{
	//		variant_object.spawn_attached_to.clear();
	//	}
	//
	//	if (!packet->read_bool("variant_object_position_exists"))
	//		continue;
	//
	//	simulation_read_quantized_position(packet, &variant_object.position, 16, &m_variant_scenario_bounds);
	//	packet->read_axes<14, 20>("variant-object-axes", &variant_object.forward, &variant_object.up);
	//
	//	s_variant_multiplayer_object_properties_definition& variant_properties = variant_object.multiplayer_game_object_properties;
	//	variant_properties.cached_object_type = (uns8)packet->read_integer("variant-properties-cached-object-type", 8);
	//	variant_properties.symmetry_placement_flags.set_unsafe((uns16)packet->read_integer("variant-properties-flags", 8));
	//	variant_properties.game_engine_flags.set_unsafe((uns8)packet->read_integer("variant-properties-game-engine-flags", 8));
	//	variant_properties.shared_storage.value = (uns8)packet->read_integer("variant-properties-shared-storage", 8);
	//	variant_properties.spawn_rate = (char)packet->read_integer("variant-properties-spawn-time", 8);
	//	variant_properties.team_affiliation.set_raw_value((uns8)packet->read_integer("variant-properties-team-affiliation", 8));
	//
	//	e_multiplayer_object_boundary_shape shape_type = (e_multiplayer_object_boundary_shape)packet->read_integer("variant-properties-shape_type", 8);
	//	variant_properties.shape = shape_type;
	//
	//	switch (shape_type)
	//	{
	//	case _shape_sphere:
	//		variant_properties.shape_data.boundary_width_or_radius = packet->read_quantized_real("variant-properties-shape-radius-width", 0.0f, 60.0f, 16, false, false);
	//		break;
	//	case _shape_cylinder:
	//		variant_properties.shape_data.boundary_width_or_radius = packet->read_quantized_real("variant-properties-shape-radius-width", 0.0f, 60.0f, 16, false, false);
	//		variant_properties.shape_data.boundary_positive_height = packet->read_quantized_real("variant-properties-shape-positive_height", 0.0f, 60.0f, 16, false, false);
	//		variant_properties.shape_data.boundary_negative_height = packet->read_quantized_real("variant-properties-shape-positive_height", 0.0f, 60.0f, 16, false, false);
	//		break;
	//	case _shape_box:
	//		variant_properties.shape_data.boundary_width_or_radius = packet->read_quantized_real("variant-properties-shape-radius-width", 0.0f, 60.0f, 16, false, false);
	//		variant_properties.shape_data.boundary_box_length = packet->read_quantized_real("variant-properties-shape-length", 0.0f, 60.0f, 16, false, false);
	//		variant_properties.shape_data.boundary_positive_height = packet->read_quantized_real("variant-properties-shape-positive_height", 0.0f, 60.0f, 16, false, false);
	//		variant_properties.shape_data.boundary_negative_height = packet->read_quantized_real("variant-properties-shape-positive_height", 0.0f, 60.0f, 16, false, false);
	//		break;
	//	}
	//}
	//
	//for (int32 object_type_index = 0; object_type_index < k_object_type_count; object_type_index++)
	//{
	//	m_object_type_start_index[object_type_index] = (int16)packet->read_integer("map-variant-object-type-map", 9) - 1;
	//}
	//
	//int32 placable_object_quota_index = 0;
	//for (int32 quota_index = 0; quota_index < 256; quota_index++)
	//{
	//	s_variant_quota& variant_quota = m_quotas[quota_index];
	//
	//	if (placable_object_quota_index >= m_number_of_placeable_object_quotas)
	//	{
	//		variant_quota = s_variant_quota{};
	//		variant_quota.price_per_item = -1.0f;
	//		continue;
	//	}
	//
	//	variant_quota.object_definition_index = (int32)packet->read_integer("object_definition_index", 32);
	//	variant_quota.minimum_count = (uns8)packet->read_integer("minimum_count", 8);
	//	variant_quota.maximum_count = (uns8)packet->read_integer("maximum_count", 8);
	//	variant_quota.placed_on_map = (uns8)packet->read_integer("placed_on_map", 8);
	//	variant_quota.maximum_allowed = (uns8)packet->read_integer("maximum_allowed", 8);
	//	variant_quota.price_per_item = (real32)packet->read_integer("price-per-item", 32);
	//}
	//
	//return result;
}

//.text:00582930 ; 
//.text:00582940 ; public: void c_map_variant::distribute_suspended_objects()

void c_map_variant::encode(c_bitstream* packet) const
{
	INVOKE_CLASS_MEMBER(0x005829A0, c_map_variant, encode, packet);
}

//.text:005831C0 ; public: int32 c_map_variant::find_definition_index(int32)
//.text:00583200 ; 
//.text:00583210 ; 
//.text:00583220 ; 
//.text:00583230 ; c_map_variant* __cdecl game_engine_get_runtime_map_variant()
//.text:00583250 ; public: real32 c_map_variant::get_budget_fraction(int32*, int32*) const
//.text:005832A0 ; public: int32 c_map_variant::get_map_variant_index_for_definition_index(int32)
//.text:005832E0 ; public: int32 c_map_variant::get_object_index(int32) const
//.text:00583300 ; public: const s_variant_quota* c_map_variant::get_object_quota_datum(int32)
//.text:00583320 ; public: const s_scenario_multiplayer_object_properties* c_map_variant::get_object_scenario_multiplayer_properties(int32)
//.text:005833F0 ; public: bool c_map_variant::get_object_scenario_properties(int32, e_object_type*, int32*)
//.text:005834E0 ; public: int32 c_map_variant::get_scenario_object_reserved_map_variant_index(e_object_type, int32) const

uns64 c_map_variant::get_unique_id() const
{
	return INVOKE_CLASS_MEMBER(0x00583530, c_map_variant, get_unique_id);
}

//.text:005835F0 ; 
//.text:00583600 ; public: const s_variant_object_datum* c_map_variant::get_variant_object_datum(int32) const
//.text:00583630 ; public: void c_map_variant::get_variant_object_datum(int32, s_variant_object_datum*) const
//.text:005836C0 ; public: void c_map_variant::get_variant_object_spawn_location(int32, real_point3d*, real_vector3d*, real_vector3d*)
//.text:00583860 ; public: s_variant_quota* c_map_variant::get_writable_object_quota_datum(int32)
//.text:00583880 ; public: void c_map_variant::handle_deleted_object(int32)
//.text:00583920 ; public: void c_map_variant::handle_local_physics_attachment(int32)
//.text:00583B20 ; public: void c_map_variant::initialize_for_scenario()
//.text:00583F90 ; public: void c_map_variant::initialize_scenario_bounds()
//.text:00583FA0 ; public: s_variant_multiplayer_object_properties_definition::s_variant_multiplayer_object_properties_definition()
//.text:00583FD0 ; 
//.text:00583FF0 ; 
//.text:00584010 ; 
//.text:00584030 ; 
//.text:00584050 ; 
//.text:00584070 ; public: bool c_map_variant::is_equal_to(const c_map_variant*) const
//.text:005841C0 ; public: bool c_map_variant::is_showing_helpers()

bool c_map_variant::is_valid() const
{
	return INVOKE_CLASS_MEMBER(0x005841D0, c_map_variant, is_valid);
}

bool c_map_variant::lossily_compare_to(c_map_variant* map_variant)
{
	return INVOKE_CLASS_MEMBER(0x005842F0, c_map_variant, lossily_compare_to, map_variant);
}

//.text:005845C0 ; 
//.text:005845E0 ; 
//.text:00584600 ; 
//.text:00584610 ; 
//.text:00584620 ; void __cdecl map_variant_create_helper_object(int32)
//.text:00584790 ; void __cdecl map_variant_register_object(int32, int32, e_object_creation_origin)
//.text:005848D0 ; public: void c_map_variant::create_all_objects() const
//.text:005849D0 ; public: int32 c_map_variant::create_object(int32) const
//.text:00584A70 ; void __cdecl process_early_mover_attachment_for_scenario_attachments(int32, int32)
//.text:00584CF0 ; public: void c_map_variant::quantize(void)
//.text:00584D80 ; public: void c_map_variant::recalculate_budget()
//.text:00584E10 ; public: void c_map_variant::recalculate_quota(int32, bool)
//.text:00584FA0 ; public: void c_map_variant::recover_from_promotion_to_simulation_authority()
//.text:005855E0 ; public: void c_map_variant::register_object(int32, int32)
//.text:00585840 ; public: void c_map_variant::register_scenario_object(e_object_type, int32)
//.text:00585890 ; public: bool c_map_variant::register_scenario_object(int32, bool)
//.text:00585C00 ; public: void c_map_variant::remove_object(int32, e_object_removal_origin)
//.text:00585DE0 ; public: void c_map_variant::remove_scenario_object(e_object_type, int32, const s_variant_multiplayer_object_properties_definition*)
//.text:00585E50 ; public: void c_map_variant::reregister_all_objects()
//.text:00585EE0 ; public: void c_map_variant::reset_objects()
//.text:00586090 ; public: void c_map_variant::sanity_check(int32)

bool c_map_variant::save_to(const c_map_variant* source)
{
	return INVOKE_CLASS_MEMBER(0x005860F0, c_map_variant, save_to, source);
}

//.text:00586270 ; const s_scenario_multiplayer_object_properties* __cdecl scenario_multiplayer_object_properties_try_and_get(e_object_type, int32)
//.text:00586300 ; 
//.text:00586330 ; 
//.text:00586350 ; 
//.text:00586370 ; 
//.text:005863A0 ; 
//.text:005863C0 ; 
//.text:005863F0 ; 
//.text:00586420 ; public: void c_map_variant::set_client_number_of_variant_objects(int32)
//.text:00586440 ; public: void c_map_variant::set_client_spent_budget(int32)

void c_map_variant::set_description(const char* description)
{
	return INVOKE_CLASS_MEMBER(0x00586460, c_map_variant, set_description, description);
}

//.text:005864A0 ; public: void c_map_variant::set_metadata(const s_saved_game_item_metadata*)

void c_map_variant::set_name(const wchar_t* name)
{
	return INVOKE_CLASS_MEMBER(0x00586570, c_map_variant, set_name, name);
}

//.text:005865B0 ;
//.text:005865C0 ;
//.text:005865D0 ; public: void c_map_variant::set_variant_object_datum(int32, const s_variant_object_datum*, bool)
//.text:00586680 ; public: void c_map_variant::setup_object_properties(s_variant_object_datum*)
//.text:00586860 ; public: void c_map_variant::show_helpers(bool)
//.text:005868E0 ; public: void c_map_variant::spend(real32)
//.text:00586920 ; 
//.text:00586940 ; 
//.text:00586960 ; public: void c_map_variant::unregister_all_objects()
//.text:00586A00 ; public: void c_map_variant::update()
//.text:00586A90 ; void __cdecl update_helper_position(int32)
//.text:00586C20 ; public: bool c_map_variant::update_scenario_object_properties(int32) const

bool c_map_variant::validate()
{
	return INVOKE_CLASS_MEMBER(0x00586D00, c_map_variant, validate);
}

//.text:00586D20 ; public: void c_map_variant::validate_for_all_engines()
//.text:00587220 ; public: void c_map_variant::validate_for_object_objective_games(e_scenario_game_engine)

e_map_id c_map_variant::get_map_id() const
{
	return m_map_id;
}

#define PRINT_TABS c_console::write(tabs)

void s_variant_multiplayer_object_properties_definition::print(int32 const tab_count)
{
	char tabs[128]{};
	csmemset(tabs, ' ', tab_count * 4);

	PRINT_TABS; c_console::write_line("<item name = \"Symmetry Placement Flags\" value = %d>", (uns16)symmetry_placement_flags);
	PRINT_TABS; c_console::write_line("<item name = \"Game Engine Flags\" value = %d>", (uns8)game_engine_flags);
	PRINT_TABS; c_console::write_line("<item name = \"Team Affiliation\" value = %d>", (int32)team_affiliation);
	PRINT_TABS; c_console::write_line("<item name = \"Shared Storage\" value = %d>", shared_storage);
	PRINT_TABS; c_console::write_line("<item name = \"Spawn Time In Seconds\" value = %d>", spawn_time_in_seconds);
	PRINT_TABS; c_console::write_line("<item name = \"Cached Object Type\" value = %d>", cached_object_type);
	PRINT_TABS; c_console::write_line("<item name = \"Shape\" value = %d>", (int32)shape);
	PRINT_TABS; c_console::write_line("<item name = \"Shape Data: Boundary Width Or Radius\" value = %.2f>", shape_data.boundary_width_or_radius);
	PRINT_TABS; c_console::write_line("<item name = \"Shape Data: Boundary Box Length\" value = %.2f>", shape_data.boundary_box_length);
	PRINT_TABS; c_console::write_line("<item name = \"Shape Data: Boundary +Height\" value = %.2f>", shape_data.boundary_positive_height);
	PRINT_TABS; c_console::write_line("<item name = \"Shape Data: Boundary -Height\" value = %.2f>", shape_data.boundary_negative_height);
}

void s_variant_object_datum::print(c_map_variant* map_variant, int32 const tab_count)
{
	char tabs[128]{};
	csmemset(tabs, ' ', tab_count * 4);

	PRINT_TABS; c_console::write_line("<item name = \"Flags\" value = %d>", (uns32)flags);
	PRINT_TABS; c_console::write_line("<item name = \"Object Index\" value = 0x%08X>", object_index);
	PRINT_TABS; c_console::write_line("<item name = \"Helper Object Index\" value = %d>", helper_object_index);

	if (variant_quota_index != NONE)
	{
		PRINT_TABS; c_console::write_line("<menu name = \"Variant Quota\" index = %d>", variant_quota_index);
		map_variant->m_quotas[variant_quota_index].print(tab_count + 1);
		PRINT_TABS; c_console::write_line("</menu>");
	}

	PRINT_TABS; c_console::write_line("<item name = \"Position\" value = \"%.2f %.2f %.2f\">", position.x, position.y, position.z);
	PRINT_TABS; c_console::write_line("<item name = \"Forward\" value = \"%.2f %.2f %.2f\">", forward.i, forward.j, forward.k);
	PRINT_TABS; c_console::write_line("<item name = \"Up\" value = \"%.2f %.2f %.2f\">", up.i, up.j, up.k);
	PRINT_TABS; c_console::write_line("<item name = \"Spawn Attached To: Unique ID\" value = 0x%08X>", spawn_attached_to.m_unique_id);
	PRINT_TABS; c_console::write_line("<item name = \"Spawn Attached To: Origin BSP Index\" value = %d>", spawn_attached_to.m_origin_bsp_index);
	PRINT_TABS; c_console::write_line("<item name = \"Spawn Attached To: Type\" value = %d>", (int32)spawn_attached_to.m_type);
	PRINT_TABS; c_console::write_line("<item name = \"Spawn Attached To: Source\" value = %d>", (int32)spawn_attached_to.m_source);

	PRINT_TABS; c_console::write_line("<menu name = \"Multiplayer Game Object Properties\">", variant_quota_index);
	multiplayer_game_object_properties.print(tab_count + 1);
	PRINT_TABS; c_console::write_line("</menu>");
}

void s_variant_quota::print(int32 const tab_count)
{
	char tabs[128]{};
	csmemset(tabs, ' ', tab_count * 4);

	PRINT_TABS; c_console::write("<item name = \"Object\" value = ");
	if (object_definition_index != NONE)
	{
		const char* tag_name = tag_get_name_safe(object_definition_index);
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

	int32 const tab_count = 1;

	char tabs[128]{};
	csmemset(tabs, ' ', tab_count * 4);

	c_console::write_line("<menu name = \"Map Variant\">");
	PRINT_TABS; c_console::write_line("<item name = \"Map Variant Version\" value = %d>", m_map_variant_version);
	PRINT_TABS; c_console::write_line("<item name = \"Number Of Scenario Objects\" value = %d>", m_number_of_scenario_objects);
	PRINT_TABS; c_console::write_line("<item name = \"Number Of Variant Objects\" value = %d>", m_number_of_variant_objects);
	PRINT_TABS; c_console::write_line("<item name = \"Number Of Placeable Object Quotas\" value = %d>", m_number_of_placeable_object_quotas);
	PRINT_TABS; c_console::write_line("<item name = \"Map ID\" value = %d>", m_map_id);

	PRINT_TABS; c_console::write_line("<item name = \"World Bounds: X\" value = \"%.2f %.2f\">", m_variant_scenario_bounds.x0, m_variant_scenario_bounds.x1);
	PRINT_TABS; c_console::write_line("<item name = \"World Bounds: Y\" value = \"%.2f %.2f\">", m_variant_scenario_bounds.y0, m_variant_scenario_bounds.y1);
	PRINT_TABS; c_console::write_line("<item name = \"World Bounds: Z\" value = \"%.2f %.2f\">", m_variant_scenario_bounds.z0, m_variant_scenario_bounds.z1);

	PRINT_TABS; c_console::write_line("<item name = \"Game Engine Subtype\" value = %d>", m_game_engine_subtype);
	PRINT_TABS; c_console::write_line("<item name = \"Maximum Budget\" value = %d>", m_maximum_budget);
	PRINT_TABS; c_console::write_line("<item name = \"Spent Budget\" value = %d>", m_spent_budget);
	PRINT_TABS; c_console::write_line("<item name = \"Helpers Enabled\" value = \"%s\">", m_showing_helpers ? "true" : "false");
	PRINT_TABS; c_console::write_line("<item name = \"Built-in\" value = \"%s\">", m_built_in ? "true" : "false");
	PRINT_TABS; c_console::write_line("<item name = \"Map Variant Checksum\" value = 0x%08X>", m_original_map_signature_hash);

	for (int32 i = 0; i < m_variant_objects.get_count(); i++)
	{
		if (m_variant_objects[i].object_index == NONE)
			continue;

		PRINT_TABS; c_console::write_line("<menu name = \"Variant Object\" index = %d>", i);
		m_variant_objects[i].print(this, tab_count + 1);
		PRINT_TABS; c_console::write_line("</menu>");
	}

	for (int32 i = 0; i < m_object_type_start_index.get_count(); i++)
	{
		PRINT_TABS; c_console::write_line("<item name = \"Object Type Start Index\" index = %d value = %d>", i, m_object_type_start_index[i]);
	}

	for (int32 i = 0; i < m_quotas.get_count(); i++)
	{
		if (m_quotas[i].object_definition_index == NONE)
			continue;

		PRINT_TABS; c_console::write_line("<menu name = \"Variant Quota\" index = %d>", i);
		m_quotas[i].print(tab_count + 1);
		PRINT_TABS; c_console::write_line("</menu>");
	}

	for (int32 i = 0; i < m_gamestate_indices.get_count(); i++)
	{
		PRINT_TABS; c_console::write_line("<item name = \"Simulation Entity Index\" index = %d value = %d>", i, m_gamestate_indices[i]);
	}
	c_console::write_line("</menu>");
}

#undef PRINT_TABS

