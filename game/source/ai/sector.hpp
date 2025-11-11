#pragma once

#include "cseries/cseries.hpp"

struct pathfinding_data;
struct sector;

class c_sector_ref
{
public:
	void clear();
	int32 get_index() const;
	const pathfinding_data* pathfinding_data();
	void set(int16 bsp_index, uns16 sector_index);
	bool valid() const;
	
	//bool operator==(int32 index) const;
	//bool operator==(const c_sector_ref& other_ref) const;
	//bool operator!=(int32 index) const;
	//bool operator!=(const c_sector_ref& other_ref) const;

private:
	int16 m_bsp_index;
	uns16 m_sector_index;
};
static_assert(sizeof(c_sector_ref) == 0x4);

extern const pathfinding_data* __cdecl pathfinding_data_get(int16 structure_bsp_index);
extern sector* __cdecl pathfinding_get_sector(c_sector_ref ref);

void sector_link_render_debug(int32 link_index, const pathfinding_data* pf_data, const real_argb_color* color, bool a4);

