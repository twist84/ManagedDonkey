#pragma once

#include "cseries/cseries.hpp"
#include "objects/objects.hpp"
#include "saved_games/content/content_item_metadata.hpp"

class c_map_variant;

using c_teleporter_channel = c_enum<enum e_teleporter_channel, uns8, k_channel_default, k_teleporter_channel_count>;

struct s_variant_multiplayer_object_properties_definition
{
public:
	struct tag_shape_data
	{
		real32 boundary_width_or_radius;
		real32 boundary_box_length;
		real32 boundary_positive_height;
		real32 boundary_negative_height;
	};
	static_assert(sizeof(tag_shape_data) == 0x10);

	s_variant_multiplayer_object_properties_definition() :
		symmetry_placement_flags(),
		game_engine_flags()
	{
		initialize_to_default();
	}

	void initialize_to_default()
	{
		symmetry_placement_flags = _game_engine_disregard_symmetry_for_placement;
		game_engine_flags = 12;
		team_affiliation = 9;
		shared_storage = { 0 };
		spawn_time_in_seconds = 0;
		cached_object_type = 0;
		shape = _shape_unused;
		shape_data.boundary_width_or_radius = 0.0f;
		shape_data.boundary_box_length = 0.0f;
		shape_data.boundary_positive_height = 0.0f;
		shape_data.boundary_negative_height = 0.0f;
	}

	void print(int32 const tab_count);

//protected:

	c_flags<e_game_engine_symmetric_placement, uns16, k_number_of_game_engine_symmetric_placement_settings> symmetry_placement_flags;
	c_flags<e_scenario_game_engine, uns8, k_scenario_game_engine_type_count> game_engine_flags;
	c_enum<e_multiplayer_team_designator, uns8, _multiplayer_team_designator_defender, k_multiplayer_team_designator_count> team_affiliation;

	union
	{
		uns8 spare_clips;
		uns8 teleporter_channel;
		uns8 spawn_order;

		uns8 value;
	} shared_storage;

	// seconds
	uns8 spawn_time_in_seconds;

	uns8 cached_object_type;

	c_enum<e_multiplayer_object_boundary_shape, int8, _shape_unused, k_multiplayer_object_boundary_count> shape;
	tag_shape_data shape_data;
};
static_assert(sizeof(s_variant_multiplayer_object_properties_definition) == 0x18);

enum e_variant_object_placement_flags
{
	_map_variant_occupied_slot = 0,
	_map_variant_scenario_object_bit = 3,
	_map_variant_scenario_object_removed = 5,

	k_variant_object_placement_flags
};

struct s_variant_object_datum
{
public:
	s_variant_object_datum() :
		flags(),
		reuse_timeout(),
		object_index(NONE),
		helper_object_index(NONE),
		variant_quota_index(NONE),
		position(),
		forward(),
		up(),
		spawn_attached_to(),
		multiplayer_game_object_properties()
	{
	}

	void print(c_map_variant* map_variant, int32 const tab_count);

//protected:
	c_flags<e_variant_object_placement_flags, uns16, k_variant_object_placement_flags> flags;
	int16 reuse_timeout;
	int32 object_index;
	int32 helper_object_index;
	int32 variant_quota_index;
	real_point3d position;
	real_vector3d forward;
	real_vector3d up;
	c_object_identifier spawn_attached_to;
	s_variant_multiplayer_object_properties_definition multiplayer_game_object_properties;
};
static_assert(sizeof(s_variant_object_datum) == 0x54);

struct s_variant_quota
{
public:
	s_variant_quota() :
		object_definition_index(NONE),
		minimum_count(0),
		maximum_count(0),
		placed_on_map(0),
		maximum_allowed(NONE),
		price_per_item(0.0f)
	{
	}

	void print(int32 const tab_count);

//protected:
	int32 object_definition_index;
	uns8 minimum_count;
	uns8 maximum_count;
	uns8 placed_on_map;
	uns8 maximum_allowed;
	real32 price_per_item;
};
static_assert(sizeof(s_variant_quota) == 0xC);

class c_bitstream;

class c_map_variant
{
public:
	c_map_variant();
	//~c_map_variant();
	void create_default(e_map_id map_id);
	bool decode(c_bitstream* packet);
	void encode(c_bitstream* packet) const;
	uns64 get_unique_id() const;
	bool is_valid() const;
	bool lossily_compare_to(c_map_variant* map_variant);
	void quantize();
	bool save_to(const c_map_variant* source) const;
	void set_description(const char* description);
	void set_metadata(const s_saved_game_item_metadata* metadata);
	void set_name(const wchar_t* name);
	bool validate();
	void validate_for_all_engines();
	e_map_id get_map_id() const;

	void print();

//protected:
	s_saved_game_item_metadata m_metadata;
	int16 m_map_variant_version;
	int16 m_number_of_scenario_objects;
	int16 m_number_of_variant_objects;
	int16 m_number_of_placeable_object_quotas;
	e_map_id m_map_id;
	real_rectangle3d m_variant_scenario_bounds;
	int32 m_game_engine_subtype;
	real32 m_maximum_budget;
	real32 m_spent_budget;
	bool m_showing_helpers;
	bool m_built_in;
	uns32 m_original_map_signature_hash;
	c_static_array<s_variant_object_datum, 640> m_variant_objects;
	c_static_array<int16, k_object_type_count> m_object_type_start_index;
	c_static_array<s_variant_quota, 256> m_quotas;
	c_static_array<int32, 80> m_gamestate_indices;
};
static_assert(sizeof(c_map_variant) == 0xE090);

extern c_map_variant* __cdecl game_engine_get_runtime_map_variant();

