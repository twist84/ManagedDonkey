#pragma once

class c_hint_ref
{
public:
	int16 m_structure_index;
	int16 m_hint_index;
};
COMPILE_ASSERT(sizeof(c_hint_ref) == 0x4);

