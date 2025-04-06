#pragma once

#include "cseries/cseries.hpp"
#include "objects/objects.hpp"
#include "saved_games/content/content_item_metadata.hpp"

struct c_map_variant;

enum e_game_engine_symmetric_placement
{
	_game_engine_disregard_symmetry_for_placement = 0,
	_game_engine_symmetric_placement,
	_game_engine_asymmetric_placement,

	k_number_of_game_engine_symmetric_placement_settings
};

enum e_scenario_game_engine
{
	_scenario_game_engine_type_ctf = 0,
	_scenario_game_engine_type_slayer,
	_scenario_game_engine_type_oddball,
	_scenario_game_engine_type_king,
	_scenario_game_engine_type_juggernaut,
	_scenario_game_engine_type_territories,
	_scenario_game_engine_type_assault,
	_scenario_game_engine_type_vip,
	_scenario_game_engine_type_infection,
	_scenario_game_engine_type_target_training,

	k_scenario_game_engine_type_count
};

enum e_multiplayer_team_designator
{
	_multiplayer_team_designator_defender = 0,
	_multiplayer_team_designator_attacker,
	_multiplayer_team_designator_third_party,
	_multiplayer_team_designator_fourth_party,
	_multiplayer_team_designator_fifth_party,
	_multiplayer_team_designator_sixth_party,
	_multiplayer_team_designator_seventh_party,
	_multiplayer_team_designator_eigth_party,
	_multiplayer_team_designator_neutral,

	k_multiplayer_team_designator_count
};

enum e_teleporter_channel
{
	_channel_first = 0,
	_channel_alpha = _channel_first,
	_channel_bravo,
	_channel_charlie,
	_channel_delta,
	_channel_echo,
	_channel_foxtrot,
	_channel_golf,
	_channel_hotel,
	_channel_india,
	_channel_juliet,
	_channel_kilo,
	_channel_lima,
	_channel_mike,
	_channel_november,
	_channel_oscar,
	_channel_papa,
	_channel_quebec,
	_channel_romeo,
	_channel_sierra,
	_channel_tango,
	_channel_uniform,
	_channel_victor,
	_channel_whiskey,
	_channel_xray,
	_channel_yankee,
	_channel_zulu,

	k_teleporter_channel_count,
	k_channel_default = _channel_first,
};
using c_teleporter_channel = c_enum<enum e_teleporter_channel, byte, k_channel_default, k_teleporter_channel_count>;

struct s_variant_multiplayer_object_properties_definition
{
public:
	struct tag_shape_data
	{
		real boundary_width_or_radius;
		real boundary_box_length;
		real boundary_positive_height;
		real boundary_negative_height;
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

	void print(long const tab_count);

//protected:

	c_flags<e_game_engine_symmetric_placement, word, k_number_of_game_engine_symmetric_placement_settings> symmetry_placement_flags;
	c_flags<e_scenario_game_engine, byte, k_scenario_game_engine_type_count> game_engine_flags;
	c_enum<e_multiplayer_team_designator, byte, _multiplayer_team_designator_defender, k_multiplayer_team_designator_count> team_affiliation;

	union
	{
		char spare_clips;
		char teleporter_channel;
		char spawn_order;

		byte value;
	} shared_storage;

	// seconds
	byte spawn_time_in_seconds;

	byte cached_object_type;

	c_enum<e_multiplayer_object_boundary_shape, char, _shape_unused, k_multiplayer_object_boundary_count> shape;
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

	void print(c_map_variant* map_variant, long const tab_count);

//protected:
	c_flags<e_variant_object_placement_flags, word, k_variant_object_placement_flags> flags;
	short reuse_timeout;
	long object_index;
	long helper_object_index;
	long variant_quota_index;
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

	void print(long const tab_count);

//protected:
	long object_definition_index;
	byte minimum_count;
	byte maximum_count;
	byte placed_on_map;
	byte maximum_allowed;
	real price_per_item;
};
static_assert(sizeof(s_variant_quota) == 0xC);

enum e_map_id;
struct c_bitstream;

struct c_map_variant
{
public:
	c_map_variant();
	//~c_map_variant();
	void create_default(e_map_id map_id);
	bool decode(c_bitstream* packet);
	void encode(c_bitstream* packet) const;
	bool is_valid() const;
	bool lossily_compare_to(c_map_variant* map_variant);
	bool read_from(c_map_variant const* source);
	void set_description(char const* description);
	void set_name(wchar_t const* name);
	bool validate();
	e_map_id get_map_id() const;

	void print();

//protected:
	s_saved_game_item_metadata m_metadata;
	short m_map_variant_version;
	short m_number_of_scenario_objects;
	short m_number_of_variant_objects;
	short m_number_of_placeable_object_quotas;
	e_map_id m_map_id;
	real_rectangle3d m_variant_scenario_bounds;
	long m_game_engine_subtype;
	real m_maximum_budget;
	real m_spent_budget;
	bool m_showing_helpers;
	bool m_built_in;
	dword m_original_map_signature_hash;
	c_static_array<s_variant_object_datum, 640> m_variant_objects;
	c_static_array<short, k_object_type_count> m_object_type_start_index;
	c_static_array<s_variant_quota, 256> m_quotas;
	c_static_array<long, 80> m_gamestate_indices;
};
static_assert(sizeof(c_map_variant) == 0xE090);