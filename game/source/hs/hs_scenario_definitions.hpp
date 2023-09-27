#pragma once

#include "hs/hs.hpp"
#include "memory/data.hpp"
#include "tag_files/tag_groups.hpp"

struct hs_tag_reference
{
	c_typed_tag_reference<0xFFFFFFFF> reference;

	void update_reference_names();
};
static_assert(sizeof(hs_tag_reference) == sizeof(s_tag_reference));

struct hs_script_parameter
{
	c_static_string<32> name;
	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> return_type;

	// pad
	byte JBG[2];
};
static_assert(sizeof(hs_script_parameter) == 0x24);

struct hs_global_internal
{
	c_static_string<32> name;
	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> type;

	// pad
	byte EB[2];

	long initialization_expression_index;
};
static_assert(sizeof(hs_global_internal) == 0x28);

struct hs_script
{
	c_static_string<32> name;
	c_enum<e_hs_script_type, short, _hs_script_startup, k_hs_script_type_count> script_type;
	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> return_type;
	long root_expression_index;
	c_typed_tag_block<hs_script_parameter> parameters;
};
static_assert(sizeof(hs_script) == 0x34);

enum hs_source_file_flags
{
	_source_file_flag_generated_at_runtime = 0,
	_source_file_flag_ai_fragments,

	hs_source_file_flags
};

struct hs_string_data_definition
{
#pragma warning(push)
#pragma warning(disable : 4200)
	byte data[];
#pragma warning(pop)
};

enum e_hs_syntax_node_flags
{
	_hs_syntax_node_primitive_bit = 0,
	_hs_syntax_node_script_bit,
	_hs_syntax_node_variable_bit,
	_hs_syntax_node_unknown_bit3,
	_hs_syntax_node_unknown_bit4,

	k_hs_syntax_node_flags
};

struct hs_syntax_node : s_datum_header
{
	union
	{
		short script_index;
		short function_index;
		c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> constant_type;
	};

	c_enum<e_hs_type, short, _hs_unparsed, k_hs_type_count> type;

	c_flags<e_hs_syntax_node_flags, word, k_hs_syntax_node_flags> flags;

	long next_node_index;
	long source_offset;
	byte data[4];
	short line_number;
	short HMM;
};
static_assert(sizeof(hs_syntax_node) == 0x18);

struct hs_source_file
{
	c_typed_tag_block<hs_tag_reference> external_references;
};
static_assert(sizeof(hs_source_file) == sizeof(s_tag_block));

struct s_hs_unit_seat_mapping
{
	long unit_definition_tag_index;
	long unit_seats;
	long unit_seats2;
};
static_assert(sizeof(s_hs_unit_seat_mapping) == 0xC);

