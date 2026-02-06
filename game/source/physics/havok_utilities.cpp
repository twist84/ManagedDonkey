#include "physics/havok_utilities.hpp"

#include "cseries/cseries.hpp"

hkMoppCode* __cdecl mopp_code_data_get_mopp_code(s_tag_data* data, int32 offset)
{
	//ASSERT(suppress_errors || offset + mopp_code->getCodeSize() <= data->size);
	//ASSERT(suppress_errors || mopp_code->m_data.m_data == reinterpret_cast<hkUint8*>(mopp_code + 1));

	return INVOKE(0x00717BF0, mopp_code_data_get_mopp_code, data, offset);
}

int32 __cdecl mopp_code_get_tag_data_size(const hkMoppCode* mopp_code)
{
	ASSERT(mopp_code != nullptr);

	return INVOKE(0x00717CA0, mopp_code_get_tag_data_size, mopp_code);
}

