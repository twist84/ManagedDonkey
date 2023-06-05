#include "cache/cache_file_tag_resource_runtime.hpp"

#include "bitmaps/dds_file.hpp"
#include "cseries/cseries_console.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x0243F780, c_asynchronous_io_arena, g_cache_file_io_arena);

void patch_lz_cache_file_decompressor()
{
	patch_pointer({ .address = 0x01690134 }, lz_cache_file_decompressor_begin);
	patch_pointer({ .address = 0x01690138 }, lz_cache_file_decompressor_decompress_buffer);
	patch_pointer({ .address = 0x0169013C }, lz_cache_file_decompressor_finish);
}

bool __fastcall lz_cache_file_decompressor_begin(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void> a1)
{
	//return DECLFUNC(0x009E1430, bool, __thiscall, c_lz_cache_file_decompressor*, c_basic_buffer<void>)(_this, a1);

	_this->__buffer4.m_size = a1.m_size;
	_this->__bufferC.m_size = a1.m_size;

	_this->__unknown14 = true;

	_this->__buffer4.m_buffer = a1.m_buffer;
	_this->__bufferC.m_buffer = a1.m_buffer;

	return true;
}

bool __fastcall lz_cache_file_decompressor_decompress_buffer(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void> compressed_buffer, c_basic_buffer<void>* decompressed_buffer)
{
	return DECLFUNC(0x009E14F0, bool, __thiscall, c_lz_cache_file_decompressor*, c_basic_buffer<void>, c_basic_buffer<void>*)(_this, compressed_buffer, decompressed_buffer);
}

bool __fastcall lz_cache_file_decompressor_finish(c_lz_cache_file_decompressor* _this, void* unused, c_basic_buffer<void>* a1)
{
#ifndef ISEXPERIMENTAL
	for (long i = 0; i < NUMBEROF(bitmap_resources); i++)
	{
		s_bitmap_resource_info const& bitmap_resource = bitmap_resources[i];

		if (csmemcmp(_this->__buffer4.m_buffer, bitmap_resource.starting_data, sizeof(bitmap_resource.starting_data)) == 0)
		{
			c_dds_file _dds_file(bitmap_resource.filename);
			s_dds_file* dds_file = _dds_file.get();
			if (!dds_file)
				continue;

			byte* buffer4 = static_cast<byte*>(_this->__buffer4.m_buffer);
			byte* bufferC = static_cast<byte*>(_this->__bufferC.m_buffer);

			_this->__buffer4.m_size = dds_file->header.linear_size;
			bufferC = buffer4 + _this->__buffer4.m_size;

			csmemcpy(buffer4, dds_file->data, _this->__buffer4.m_size);
		}
	}
#endif // ISEXPERIMENTAL

	return DECLFUNC(0x009E1640, bool, __thiscall, c_lz_cache_file_decompressor*, c_basic_buffer<void>*)(_this, a1);
}

//HOOK_DECLARE(0x00563E10, tag_resource_get);
//
//void* __cdecl tag_resource_get(s_tag_resource const* resource)
//{
//    FUNCTION_BEGIN(false);
//
//    void* result = nullptr;
//    HOOK_INVOKE(result =, tag_resource_get, resource);
//    return result;
//}

