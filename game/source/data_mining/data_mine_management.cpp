#include "data_mining/data_mine_management.hpp"

#include "cseries/cseries.hpp"

void __cdecl data_mine_dispose()
{
	INVOKE(0x005AC740, data_mine_dispose);
}

void __cdecl data_mine_enable(bool setting)
{
	INVOKE(0x005AC750, data_mine_enable, setting);
}

bool __cdecl data_mine_enabled()
{
	return INVOKE(0x005AC760, data_mine_enabled);
}

void __cdecl data_mine_flush()
{
	INVOKE(0x005AC770, data_mine_flush);
}

const char* __cdecl data_mine_get_tag_name(int32 tag_index)
{
	return INVOKE(0x005AC780, data_mine_get_tag_name, tag_index);
}

void __cdecl data_mine_initialize()
{
	INVOKE(0x005AC790, data_mine_initialize);
}

void __cdecl data_mine_insert(const void* data, int32 data_length)
{
	INVOKE(0x005AC7A0, data_mine_insert, data, data_length);
}

void __cdecl data_mine_insert_text_and_camera_position(const char* text)
{
	INVOKE(0x005AC7B0, data_mine_insert_text_and_camera_position, text);
}

//.text:005AC7C0 ; 

void __cdecl data_mine_update()
{
	INVOKE(0x005AC7D0, data_mine_update);
}

void __cdecl data_mine_upload()
{
	INVOKE(0x005AC7E0, data_mine_upload);
}

void __cdecl data_mine_upload_no_dialog()
{
	INVOKE(0x005AC7F0, data_mine_upload_no_dialog);
}

bool __cdecl data_mine_uploading_files()
{
	return INVOKE(0x005AC800, data_mine_uploading_files);
}

