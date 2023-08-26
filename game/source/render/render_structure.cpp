#include "render/render_structure.hpp"

#include "cseries/cseries.hpp"

s_structure_bsp_resources* c_structure_bsp_resource_interface::get_resources() const
{
    return DECLFUNC(0x004E9550, s_structure_bsp_resources*, __thiscall, c_structure_bsp_resource_interface const*)(this);

	//if (use_resource_items)
	//{
	//	if (tag_resource_not_empty(&tag_resources))
	//	{
	//		return static_cast<s_structure_bsp_resources*>(tag_resource_get(&tag_resources));
	//	}
	//}
	//
	//return &raw_resources.m_elements[0];
}
