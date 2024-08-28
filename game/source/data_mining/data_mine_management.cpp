#include "data_mining/data_mine_management.hpp"

#include "cseries/cseries.hpp"

void __cdecl data_mine_dispose()
{
	INVOKE(0x005AC740, data_mine_dispose);
}

//.text:005AC750 ; void __cdecl data_mine_enable(bool)
//.text:005AC760 ; bool __cdecl data_mine_enabled()
//.text:005AC770 ; void __cdecl data_mine_flush()
//.text:005AC780 ; char const* __cdecl data_mine_get_tag_name(long)

void __cdecl data_mine_initialize()
{
	INVOKE(0x005AC790, data_mine_initialize);
}

//.text:005AC7A0 ; 
//.text:005AC7B0 ; void __cdecl data_mine_insert_text_and_camera_position(char const*)
//.text:005AC7C0 ; 

void __cdecl data_mine_update()
{
	INVOKE(0x005AC7D0, data_mine_update);
}

//.text:005AC7E0 ; void __cdecl data_mine_upload()
//.text:005AC7F0 ; void __cdecl data_mine_upload_no_dialog()
//.text:005AC800 ; bool __cdecl data_mine_uploading_files()

