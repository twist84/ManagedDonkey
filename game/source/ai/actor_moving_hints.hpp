#pragma once

struct c_hint_ref
{
	int16 m_structure_index;
	int16 m_hint_index;
};
static_assert(sizeof(c_hint_ref) == 0x4);

