#pragma once

#include "hs/hs.hpp"
#include "tag_files/tag_groups.hpp"

enum
{
	k_maximum_hs_scripts_per_scenario = 1024,
	k_maximum_hs_parameters_per_script = 5,
	k_maximum_hs_globals_per_scenario = 280,
	k_maximum_hs_references_per_scenario = 512,
	k_maximum_hs_syntax_nodes_per_scenario = 61440,
	k_maximum_hs_syntax_nodes_free_space = 512,
	//k_maximum_hs_string_data_per_scenario = 819200,
	//k_maximum_hs_source_files_per_scenario = 16,
	//k_maximum_hs_source_data_per_file = 1048576,
	k_maximum_hs_unit_seats = 64,
	k_maximum_hs_unit_seat_mappings = 65536,
	//k_custom_hs_string_field = 'hsbl',
};

struct hs_tag_reference
{
	c_typed_tag_reference<0xFFFFFFFF> reference;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(hs_tag_reference) == sizeof(s_tag_reference));

struct hs_script_parameter
{
	char name[32];
	int16 type; // e_hs_type
	int16 pad0;
};
COMPILE_ASSERT(sizeof(hs_script_parameter) == 0x24);

struct hs_global_internal
{
	char name[32];
	int16 type; // e_hs_type
	uns16 pad;
	int32 initialization_expression_index;
};
COMPILE_ASSERT(sizeof(hs_global_internal) == 0x28);

struct hs_script
{
	char name[32];
	int16 script_type; // e_hs_script_type
	int16 return_type; // e_hs_type
	int32 root_expression_index;
	c_typed_tag_block<hs_script_parameter> parameters;
};
COMPILE_ASSERT(sizeof(hs_script) == 0x34);

enum hs_source_file_flags
{
	_source_file_flag_generated_at_runtime = 0,
	_source_file_flag_ai_fragments,

	hs_source_file_flags
};

struct hs_string_data_definition
{
	__pragma(warning(disable : 4200)) byte data[];
};

enum e_hs_syntax_node_flags
{
	_hs_syntax_node_primitive_bit = 0,
	_hs_syntax_node_script_bit,
	_hs_syntax_node_variable_bit,
	_hs_syntax_node_permanent_bit,
	_hs_syntax_node_parameter_bit,
	_hs_syntax_node_line_number_bit,
	_hs_syntax_node_stripped_bit,

	NUMBER_OF_HS_SYNTAX_NODE_FLAGS
};

struct hs_syntax_node :
	s_datum_header
{
	union
	{
		int16 script_index;
		int16 function_index;
		int16 constant_type; // e_hs_type
	};

	int16 type; // e_hs_type

	uns16 flags; // e_hs_syntax_node_flags

	int32 next_node_index;
	int32 source_offset;

	union
	{
		uns32 data;

		bool bool_value;
		real32 real_value;
		int16 short_value;
		int32 long_value;
		const char* string_value;
		string_id string_id_value;

		byte storage[4];
	};

	int16 line_number;
	int16 pad0;
};
COMPILE_ASSERT(sizeof(hs_syntax_node) == 0x18);

struct hs_source_file
{
	char name[32];
	s_tag_data source;
	c_typed_tag_block<hs_tag_reference> external_references;
	uns32 flags;
};
COMPILE_ASSERT(sizeof(hs_source_file) == 0x44);

struct s_hs_unit_seat_mapping
{
	int32 unit_definition_tag_index;
	c_static_flags<k_maximum_hs_unit_seats> unit_seats;
};
COMPILE_ASSERT(sizeof(s_hs_unit_seat_mapping) == 0xC);

