#include "networking/logic/storage/network_storage_files.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "tag_files/files.hpp"

bool __cdecl create_configuration_file(const char* filename, const void* file_contents, int32 file_size)
{
	//return INVOKE(0x004E7540, create_configuration_file, filename, file_contents, file_size);

	ASSERT(file_size > 0);
	ASSERT(file_contents);

	s_file_reference info;
	file_reference_create_from_path(&info, filename, false);

	if (!file_create_parent_directories_if_not_present(&info))
	{
		event(_event_warning, "create_configuration_file: unable to create parent directories: %s", filename);
		return false;
	}

	if (!file_create(&info))
	{
		event(_event_warning, "create_configuration_file: unable to create file: %s", filename);
		return false;
	}

	uns32 error = 0;
	if (!file_open(&info, FLAG(_permission_write_bit), &error))
	{
		event(_event_warning, "create_configuration_file: unable to open file: %s", filename);
		return false;
	}

	bool result = file_write(&info, file_size, file_contents);
	if (!file_close(&info))
	{
		event(_event_warning, "create_configuration_file: unable to close file: %s", filename);
	}

	return result;
}

