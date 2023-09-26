#include "hs/hs_compile.hpp"

#include "hs/hs.hpp"

#include <ctype.h>
#include <stdlib.h>

hs_type_primitive_parser_t hs_type_primitive_parsers[k_hs_type_count]
{
	// non-types
	nullptr,
	nullptr,
	nullptr,

	// void
	nullptr,

};

