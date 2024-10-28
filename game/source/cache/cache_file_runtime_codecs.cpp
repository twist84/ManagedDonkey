#include "cache_file_runtime_codecs.hpp"

c_cache_file_decompressor_registry::c_cache_file_decompressor_registry()
{
	//DECLFUNC(0x0055DB90, void, __thiscall, c_cache_file_decompressor_registry*)(this);
}

c_cache_file_runtime_decompressor_registry::c_cache_file_runtime_decompressor_registry()
{
	//DECLFUNC(0x0055DC80, void, __thiscall, c_cache_file_runtime_decompressor_registry*)(this);
}

c_cache_file_decompressor_service* c_cache_file_runtime_decompressor_registry::find_decompressor_service(s_tag_persistent_identifier* identifier)
{
	return INVOKE_CLASS_MEMBER(0x006EC990, c_cache_file_runtime_decompressor_registry, find_decompressor_service, identifier);
}

