#include "items/equipment_definitions.hpp"

#include "cseries/cseries.hpp"

e_equipment_type equipment_definition_get_type(long definition_index, long expected_equipment_type)
{
	return INVOKE(0x00BA0260, equipment_definition_get_type, definition_index, expected_equipment_type);
}

bool equipment_definition_has_type(long definition_index, e_equipment_type equipment_type)
{
	return INVOKE(0x00BA0360, equipment_definition_has_type, definition_index, equipment_type);
}

