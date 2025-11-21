#pragma once

extern void __cdecl data_mine_dispose();
extern void __cdecl data_mine_enable(bool setting);
extern bool __cdecl data_mine_enabled();
extern void __cdecl data_mine_flush();
extern const char* __cdecl data_mine_get_tag_name(int32 tag_index);
extern void __cdecl data_mine_initialize();
extern void __cdecl data_mine_insert(const void* data, int32 data_length);
extern void __cdecl data_mine_insert_text_and_camera_position(const char* text);
extern void __cdecl data_mine_update();
extern void __cdecl data_mine_upload();
extern void __cdecl data_mine_upload_no_dialog();
extern bool __cdecl data_mine_uploading_files();

