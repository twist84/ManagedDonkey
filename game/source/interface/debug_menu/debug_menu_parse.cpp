#include "interface/debug_menu/debug_menu_parse.hpp"

#include "interface/debug_menu/debug_menu.hpp"

char const* const g_token_names[k_token_count]
{
	"none",
	"min",
	"max",
	"inc",
	"menu",
	"zone_set_menu",
	"item",
	"caption",
	"name",
	"variable",
	"color",
	"type",
	"global",
	"command",
	"\r\n",
	"\n\r",
	"\r",
	"\n",
};

char const* debug_menu_build_recursive(FILE* menu_file, long& c, c_debug_menu* menu, long* line_count, char* error_buffer, long error_buffer_size)
{
	//return debug_menu_build_recursive(menu_file, c, 0, line_count, error_buffer, error_buffer_size);

	return "";
}

void debug_menu_parse(c_debug_menu* root_menu, char const* file_name)
{
	ASSERT(file_name != NULL);
	ASSERT(root_menu != NULL);

	FILE* menu_file;
	if (fopen_s(&menu_file, file_name, "rt") == 0 && menu_file)
	{
		char error_buffer[1024]{};

		long line_count = 1;
		long c = (long)fgetc(menu_file);
		debug_menu_build_recursive(menu_file, c, root_menu, &line_count, error_buffer, sizeof(error_buffer));
		fclose(menu_file);
	}
}

