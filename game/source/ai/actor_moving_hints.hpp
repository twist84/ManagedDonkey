#pragma once

struct c_hint_ref
{
	short m_structure_index;
	short m_hint_index;
};
static_assert(sizeof(c_hint_ref) == 0x4);

