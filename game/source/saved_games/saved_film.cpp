#include "saved_games/saved_film.hpp"

#include "cache/cache_files.hpp"
#include "cache/restricted_memory.hpp"
#include "config/version.hpp"
#include "saved_games/content_catalogue.hpp"
#include "cseries/async.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "main/levels.hpp"
#include "memory/byte_swapping.hpp"
#include "networking/network_utilities.hpp"
#include "networking/tools/network_debug_dump.hpp"
#include "simulation/simulation.hpp"

bool g_saved_films_show_timestamp = false;

c_saved_film::c_saved_film() :
	m_film_header(),
	m_async_double_buffer()
{
	c_saved_film::reset_internal_state();
}

bool c_saved_film::initialize(c_allocation_base* allocator)
{
	c_saved_film::reset_internal_state();
	bool storage_allocated = m_async_double_buffer.allocate_storage(allocator, k_saved_film_async_io_buffer_size);
	if (!storage_allocated)
	{
		event(_event_warning, "networking:saved_film: failed to allocate storage for film");
	}
	return storage_allocated;
}

void c_saved_film::dispose(c_allocation_base* allocator)
{
	c_saved_film::close();
	m_async_double_buffer.release_storage(allocator);
}

// $REVIEW
bool c_saved_film::open_for_read(const char* filename, e_controller_index controller_index, bool disable_version_check)
{
	ASSERT(m_film_state == k_saved_film_state_none);
	ASSERT(m_content_controller_index == k_no_controller);

	if (VALID_CONTROLLER(controller_index))
	{
		//c_content_catalogue* content_catalogue = content_catalogue_get_interface(controller_index);
		//file_reference_create_from_path(&m_file_reference, filename, false);
		//int32 content_item_index = content_catalogue->content_item_try_and_get_from_file_reference(&m_file_reference);
		//if (content_item_index != NONE)
		//{
		//	if (!content_catalogue->content_item_mount(content_item_index, true))
		//	{
		//		event(_event_warning, "networking:saved_film: failed to open content item for saved film read (file= '%s')",
		//			filename);
		//
		//		event(_event_warning, "networking:saved_film: failed to open saved film '%s' for reading",
		//			filename);
		//		c_saved_film::close();
		//		return false;
		//	}
		//	m_content_controller_index = controller_index;
		//}

		m_content_controller_index = controller_index;
	}

	c_static_wchar_string<260> file;
	file.set_char(filename);

	if (!m_async_double_buffer.open_file(file.get_string(), _async_buffer_file_access_read, _async_buffer_disposition_open_existing))
	{
		event(_event_warning, "networking:saved_film: async_double_buffer.open_file() failed for '%s'",
			filename);

		event(_event_warning, "networking:saved_film: failed to open saved film '%s' for reading",
			filename);
		c_saved_film::close();
		return false;
	}

	m_film_state = _saved_film_open_for_read;
	if (!c_saved_film::read_data(&m_film_header, k_saved_film_async_io_buffer_size))
	{
		event(_event_warning, "networking:saved_film: failed to get saved film header for '%s'",
			filename);

		event(_event_warning, "networking:saved_film: failed to open saved film '%s' for reading",
			filename);
		c_saved_film::close();
		return false;
	}

	if (!c_saved_film::header_valid(&m_film_header, disable_version_check))
	{
		event(_event_warning, "networking:saved_film: film header not valid for '%s'",
			filename);

		event(_event_warning, "networking:saved_film: failed to open saved film '%s' for reading",
			filename);
		c_saved_film::close();
		return false;
	}

	m_current_film_offset = k_saved_film_async_io_buffer_size;
	return true;
}

bool c_saved_film::read_update(s_saved_film_update* update_out)
{
	ASSERT(update_out);
	ASSERT(m_film_state == _saved_film_open_for_read);

	uns32 header = 0;
	if (!c_saved_film::read_data(&header, sizeof(header)))
	{
		event(_event_warning, "networking:saved_film: failed to read header");
		return false;
	}

	if (!c_saved_film::parse_update_header(header, update_out))
	{
		event(_event_warning, "networking:saved_film: failed to parse header %d",
			header);
		return false;
	}

	return true;
}

bool c_saved_film::read_simulation_update(const s_saved_film_update* update, s_simulation_update* simulation_update_out)
{
	ASSERT(simulation_update_out);
	ASSERT(update->update_type == _saved_film_update_type_simulation_update);
	ASSERT(m_film_state == _saved_film_open_for_read);

	if (update->update_size <= 0 || update->update_size > sizeof(s_blf_saved_film))
	{
		event(_event_error, "networking:saved_film: read bad update size %d",
			update->update_size);
		return false;
	}

	byte data[k_maximum_game_state_header_size]{};
	if (!c_saved_film::read_data(data, update->update_size))
	{
		event(_event_warning, "networking:saved_film: failed to read update [size %d]",
			update->update_size);
		return false;
	}

	if (!simulation_update_read_from_buffer(simulation_update_out, update->update_size, data))
	{
		event(_event_warning, "networking:saved_film: read failed to decode update from buffer [size %d]",
			update->update_size);
		return false;
	}

	m_current_tick++;
	return true;
}

struct s_saved_film_gamestate_header
{
	int32 compressed_size;
	uns32 checksum;
	int32 update_number;
};

bool c_saved_film::read_gamestate(const s_saved_film_update* update, void* compressed_game_state_buffer_out, int32 buffer_size, int32* compressed_game_state_size_out, int32* update_number_out)
{
	ASSERT(update);
	ASSERT(compressed_game_state_buffer_out);
	ASSERT(buffer_size >= k_game_state_maximum_compressed_size);
	ASSERT(update->update_type == _saved_film_update_type_gamestate);
	ASSERT(update->update_size == sizeof(s_saved_film_gamestate_header));
	ASSERT(buffer_size >= k_game_state_maximum_compressed_size);
	ASSERT(m_film_state == _saved_film_open_for_read);

	s_saved_film_gamestate_header saved_film_gamestate_header{};
	if (!c_saved_film::read_data(&saved_film_gamestate_header, sizeof(s_saved_film_gamestate_header)))
	{
		event(_event_error, "networking:saved_film: failed to read game state header [header size %d]",
			sizeof(s_saved_film_gamestate_header));
		return false;
	}

	if (saved_film_gamestate_header.compressed_size > buffer_size)
	{
		event(_event_error, "networking:saved_film: read compressed game state size larger than buffer [%d > %d]",
			saved_film_gamestate_header.compressed_size,
			buffer_size);
		return false;
	}

	if (!c_saved_film::read_data(compressed_game_state_buffer_out, saved_film_gamestate_header.compressed_size))
	{
		event(_event_error, "networking:saved_film: read failed to read compressed game state [size %d]",
			saved_film_gamestate_header.compressed_size);
		return false;
	}

	//uns32 checksum = fast_checksum_buffer(NONE, compressed_game_state_buffer_out, saved_film_gamestate_header.compressed_size);
	//if (checksum != saved_film_gamestate_header.checksum)
	//{
	//	event(_event_error, "networking:saved_film: read compressed game state checksum mismatch [0x%08X != 0x%08X]",
	//		checksum,
	//		saved_film_gamestate_header.checksum);
	//	return false;
	//}

	*compressed_game_state_size_out = saved_film_gamestate_header.compressed_size;
	*update_number_out = saved_film_gamestate_header.update_number;
	return true;
}

int32 c_saved_film::get_position()
{
	ASSERT(m_film_state == _saved_film_open_for_read || m_film_state == _saved_film_open_for_write);
	ASSERT(m_async_double_buffer.ready_to_read() || m_async_double_buffer.ready_to_write());

	return m_async_double_buffer.get_position();
}

bool c_saved_film::set_position(int32 position)
{
	ASSERT(m_film_state == _saved_film_open_for_read || m_film_state == _saved_film_open_for_write);
	ASSERT(m_async_double_buffer.ready_to_read() || m_async_double_buffer.ready_to_write());

	if (!m_async_double_buffer.set_position(position))
	{
		event(_event_warning, "networking:saved_film: failed to set film position [%d]",
			position);
		return false;
	}

	return true;
}

bool c_saved_film::open_for_write(const char* filename, const game_options* options, e_controller_index controller_index)
{
	ASSERT(filename);
	ASSERT(m_content_controller_index == k_no_controller);
	ASSERT(m_film_state == k_saved_film_state_none);
	
	assert_game_options_verify(options);
	
	c_static_wchar_string<256> file_name;
	file_name.set_char(filename);
	if (!m_async_double_buffer.open_file(file_name.get_string(), _async_buffer_file_access_write, _async_buffer_disposition_create_always))
	{
		event(_event_warning, "networking:saved_film: failed to open saved film %s for writing",
			filename);
	
		event(_event_warning, "networking:saved_film: failed to open saved film '%s' for writing",
			filename);
	
		c_saved_film::close();
		return false;
	}
	
	file_reference_create_from_path(&m_file_reference, filename, false);
	
	s_saved_game_item_metadata metadata{};
	metadata.initialize_from_current_game_settings(controller_index, _saved_game_film, L"", L"", 0);
	e_gui_game_mode game_mode = _gui_game_setup_mode_multiplayer;// metadata.get_gui_game_mode();
	
	const wchar_t* active_session_map_name = nullptr;

	switch (game_mode)
	{
	case _gui_game_setup_mode_none:
	{
		wchar_t level_name[64]{};
		active_session_map_name = levels_get_active_session_map_name(level_name, sizeof(level_name));
	}
	break;
	case _gui_game_setup_mode_campaign:
	case _gui_game_setup_mode_matchmaking:
	case _gui_game_setup_mode_multiplayer:
	case _gui_game_setup_mode_mapeditor:
	case _gui_game_setup_mode_theater:
	case _gui_game_setup_mode_survival:
	{
		active_session_map_name = options->map_variant.m_metadata.display_name;
	}
	break;
	default:
	{
		event(_event_warning, "networking:saved_film: unknown gui game mode [%d], using map variant name",
			game_mode);
	}
	break;
	}
	
	wchar_t display_name[16]{};
	saved_game_files_get_default_film_save_name(
		controller_index,
		game_mode,
		active_session_map_name,
		options->multiplayer_variant.get_active_variant()->get_metadata()->display_name,
		metadata.campaign_difficulty,
		false,
		display_name,
		sizeof(display_name));
	
	wchar_t description[128]{};
	saved_game_files_get_default_film_save_description(
		controller_index,
		game_mode,
		active_session_map_name,
		options->multiplayer_variant.get_active_variant()->get_metadata()->display_name,
		metadata.campaign_difficulty,
		description,
		sizeof(description));
	
	s_blf_saved_film saved_film{};
	s_blf_saved_film* header = &saved_film;

	csmemcpy(offset_pointer(&header->author.header, sizeof(s_blf_header)), offset_pointer(&m_film_header.author.header, sizeof(s_blf_header)), sizeof(s_blf_chunk_author) - sizeof(s_blf_header));
	csmemcpy(offset_pointer(&header->film_header.header, sizeof(s_blf_header)), offset_pointer(&m_film_header.film_header.header, sizeof(s_blf_header)), sizeof(s_blf_saved_film::s_blf_chunk_saved_film_header) - sizeof(s_blf_header));
	csmemcpy(offset_pointer(&header->film_data.header, sizeof(s_blf_header)), offset_pointer(&m_film_header.film_data.header, sizeof(s_blf_header)), sizeof(s_blf_saved_film::s_blf_chunk_saved_film_data) - sizeof(s_blf_header));
	zero_array(header->film_header.padding_to_align_for_utility_drive);
	
	header->content_header.initialize_from_current_game_settings(
		controller_index,
		_saved_game_film,
		display_name,
		description,
		0,
		0);
	
	csstrnzcpy(header->start_of_file.file_type, "halo 3 saved film", sizeof(header->start_of_file.file_type));
	
	csstrnzcpy(header->author.program_name, version_get_build_name(), sizeof(header->author.program_name));
	header->author.program_build_number = bswap_uns64(version_get_build_number_identifier());
	csstrnzcpy(header->author.program_build_string, version_get_build_string(), sizeof(header->author.program_build_string));
	csstrnzcpy(header->author.user, "", sizeof(header->author.user));
	
	csstrnzcpy(header->film_header.build_compatibility.build_number, version_get_build_string(), sizeof(header->film_header.build_compatibility.build_number));
	network_get_build_identifiers(
		&header->film_header.build_compatibility.executable_type,
		&header->film_header.build_compatibility.network_executable_version,
		&header->film_header.build_compatibility.network_compatible_version);
	
	header->film_header.build_compatibility.map_signature_size = 0;
	csmemset(header->film_header.build_compatibility.map_signature_bytes, 0, k_saved_film_maximum_map_signature_bytes);
	
	int32 signature_size = 0;
	const byte* signature_bytes = nullptr;
	if (cache_file_get_content_signature(&signature_size, &signature_bytes))
	{
		ASSERT(signature_size <= sizeof(header->film_header.build_compatibility.map_signature_bytes));
		if (signature_size > k_saved_film_maximum_map_signature_bytes)
		{
			signature_size = k_saved_film_maximum_map_signature_bytes;
		}
		header->film_header.build_compatibility.map_signature_size = signature_size;
		csmemcpy(header->film_header.build_compatibility.map_signature_bytes, signature_bytes, signature_size);
	}
	
	header->film_header.build_compatibility.map_language = _language_english;// get_map_language();
	header->film_header.build_compatibility.map_minor_version = get_map_minor_version();
	header->film_header.build_compatibility.map_minor_version_is_tracked = false;// get_map_minor_version_is_tracked();
	
	header->film_header.length_in_ticks = 0;
	header->film_header.recorded_time = system_seconds();
	header->film_header.is_host_film = game_options_valid() && game_is_authoritative();
	
	header->film_header.contains_gamestate = false;
	header->film_header.snippet_start_tick = NONE;
	
	if (const char* sessionid = netdebug_get_sessionid())
	{
		csstrnzcpy(header->film_header.session_id, sessionid, sizeof(header->film_header.session_id));
	}
	
	csmemcpy(&header->film_header.options, options, sizeof(game_options));
	m_film_state = _saved_film_open_for_write;

	if (!c_saved_film::write_data(header, sizeof(s_blf_saved_film)))
	{
		event(_event_warning, "networking:saved_film: film %s succesfully opened, but we failed to write the film header in",
			filename);
	
		event(_event_warning, "networking:saved_film: failed to open saved film '%s' for writing",
			filename);
	
		c_saved_film::close();
		return false;
	}
	
	m_current_film_offset = sizeof(s_blf_saved_film);
	m_start_of_film_data_offset = sizeof(s_blf_saved_film);
	
	ASSERT(m_start_of_film_data_offset == m_async_double_buffer.get_position());

	csmemcpy(&m_film_header, header, sizeof(s_blf_saved_film));

	return true;
}

// $REVIEW
bool c_saved_film::write_simulation_update(const s_simulation_update* update)
{
	ASSERT(update);
	ASSERT(m_film_state == _saved_film_open_for_write);

	int32 out_update_length = 0;
	byte buffer[k_maximum_game_state_header_size]{};
	if (!simulation_update_write_to_buffer(update, sizeof(s_blf_saved_film), buffer, &out_update_length))
	{
		event(_event_error, "networking:saved_film: write failed to encode simulation update");

		event(_event_warning, "networking:saved_film: marking fatal error in saved film");
		m_fatal_error_encountered_during_write = true;
		return false;
	}

	if (out_update_length <= 0 || out_update_length > k_saved_film_update_size_max)
	{
		event(_event_error, "networking:saved_film: write bad encoded size %d",
			out_update_length);

		event(_event_warning, "networking:saved_film: marking fatal error in saved film");
		m_fatal_error_encountered_during_write = true;
		return false;
	}

	int32 current_film_offset = m_current_film_offset;
	int32 update_length = FLAG(k_saved_film_update_size_bits);
	if (out_update_length < FLAG(k_saved_film_update_size_bits))
	{
		update_length = out_update_length;
	}

	if (out_update_length + (k_saved_film_async_io_buffer_size + sizeof(update_length)) + current_film_offset >= k_saved_film_maximum_size)
	{
		event(_event_warning, "networking:saved_film: write would exceed max saved film size, closing",
			out_update_length);
		return false;
	}

	if (!c_saved_film::write_data(&update_length, sizeof(update_length)))
	{
		event(_event_warning, "networking:saved_film: write failed to write header for update [size %d]",
			out_update_length);

		event(_event_warning, "networking:saved_film: marking fatal error in saved film");
		m_fatal_error_encountered_during_write = true;
		return false;
	}

	if (!c_saved_film::write_data(buffer, out_update_length))
	{
		event(_event_warning, "networking:saved_film: write failed to write encoded update [size %d]",
			out_update_length);

		event(_event_warning, "networking:saved_film: marking fatal error in saved film");
		m_fatal_error_encountered_during_write = true;
		return false;
	}

	m_current_tick++;
	return true;
}

// $REVIEW
bool c_saved_film::write_gamestate()
{
	ASSERT(m_film_state == _saved_film_open_for_write);

	return false;

	//c_game_state_compressor* compressor = game_state_get_compressor();
	//byte* compressed_game_state = nullptr;
	//int32 compressed_size = 0;
	//
	//if (!compressor->lock())
	//{
	//	event(_event_error, "networking:saved_film: failed to lock game state compressor for write_gamestate");
	//
	//	event(_event_warning, "networking:saved_film: failed to write gamestate, marking as fatal error");
	//	m_fatal_error_encountered_during_write = true;
	//	return false;
	//}
	//
	//if (!c_saved_film::write_game_state_internal(0, compressed_game_state, compressed_size))
	//{
	//	event(_event_warning, "networking:saved_film: failed to write compressed gamestate [%d/%d]",
	//		(uns32)compressed_game_state,
	//		compressed_size);
	//
	//	compressor->unlock();
	//
	//	event(_event_warning, "networking:saved_film: failed to write gamestate, marking as fatal error");
	//	m_fatal_error_encountered_during_write = true;
	//	return false;
	//}
	//
	//compressor->unlock();
	//return true;
}

// $REVIEW
bool c_saved_film::write_gamestate_from_buffer(int32 update_number, const void* gamestate, int32 gamestate_size)
{
	ASSERT(m_film_state == _saved_film_open_for_write);

	return false;

	//c_game_state_compressor* compressor = game_state_get_compressor();
	//byte* compressed_game_state = nullptr;
	//int32 compressed_size = 0;
	//
	//if (!compressor->lock())
	//{
	//	event(_event_error, "networking:saved_film: write lock game state compressor for write_gamestate_from_buffer");
	//
	//	event(_event_warning, "networking:saved_film: failed to write gamestate from buffer, marking as fatal error");
	//	m_fatal_error_encountered_during_write = true;
	//	return false;
	//}
	//
	//if (!compressor->game_state_get_compressed_from_buffer(_game_state_compression_level_fast, gamestate, gamestate_size, &compressed_game_state, &compressed_size))
	//{
	//	event(_event_error, "networking:saved_film: write failed to compress game state");
	//
	//	compressor->unlock();
	//
	//	event(_event_warning, "networking:saved_film: failed to write gamestate from buffer, marking as fatal error");
	//	m_fatal_error_encountered_during_write = true;
	//	return false;
	//}
	//
	//if (!c_saved_film::write_game_state_internal(update_number, compressed_game_state, compressed_size))
	//{
	//	event(_event_warning, "networking:saved_film: failed to write compressed gamestate from buffer [%d/%d]",
	//		(uns32)compressed_game_state,
	//		compressed_size);
	//
	//	compressor->unlock();
	//
	//	event(_event_warning, "networking:saved_film: failed to write gamestate from buffer, marking as fatal error");
	//	m_fatal_error_encountered_during_write = true;
	//	return false;
	//}
	//
	//compressor->unlock();
	//return true;
}

int32 c_saved_film::get_ticks_remaining() const
{
	ASSERT(m_film_state == _saved_film_open_for_read);

	return m_film_header.film_header.length_in_ticks - m_current_tick;
}

int32 c_saved_film::get_current_tick() const
{
	ASSERT(m_film_state == _saved_film_open_for_read || m_film_state == _saved_film_open_for_write);

	return m_current_tick;
}

int32 c_saved_film::get_length_in_ticks() const
{
	ASSERT(m_film_state == _saved_film_open_for_read);

	return m_film_header.film_header.length_in_ticks;
}

e_saved_film_state c_saved_film::get_film_state() const
{
	return m_film_state;
}

game_options* c_saved_film::get_game_options()
{
	ASSERT(m_film_state != k_saved_film_state_none);

	return &m_film_header.film_header.options;
}

const s_blf_chunk_content_header* c_saved_film::get_film_content_header() const
{
	return &m_film_header.content_header;
}

bool c_saved_film::contains_gamestate() const
{
	return m_film_header.film_header.contains_gamestate;
}

bool c_saved_film::is_snippet() const
{
	return m_film_header.film_header.is_snippet;
}

int32 c_saved_film::get_snippet_start_tick() const
{
	if (!m_film_header.film_header.is_snippet)
	{
		return NONE;
	}

	return m_film_header.film_header.snippet_start_tick;
}

bool c_saved_film::handle_revert(int32 file_position, int32 film_tick)
{
	ASSERT(m_film_state == _saved_film_open_for_read);

	if (!m_async_double_buffer.set_position(file_position))
	{
		event(_event_warning, "networking:saved_film: failed to set file position for revert %d [tick %d]",
			file_position,
			film_tick);
		return false;
	}

	m_current_tick = film_tick;
	return true;
}

bool c_saved_film::close()
{
	bool result = false;
	bool open_for_write = false;

	switch (m_film_state)
	{
	case _saved_film_open_for_read:
	{
		event(_event_message, "networking:saved_film: closing film");
		result = true;
	}
	break;
	case _saved_film_open_for_write:
	{
		if (!c_saved_film::write_finish())
		{
			event(_event_warning, "networking:saved_film: failed to finish writing the film, film is invalid");
			break;
		}

		if (m_fatal_error_encountered_during_write)
		{
			break;
		}

		result = true;
		open_for_write = true;
	}
	break;
	}

	m_async_double_buffer.close_file();
	if (open_for_write && result)
	{
		char filename_buffer[256]{};
		file_reference_get_name(&m_file_reference, FLAG(_name_file_bit), filename_buffer, sizeof(filename_buffer));

		c_static_string<256> destination_filename{};
		destination_filename.set(filename_buffer);
		destination_filename.append(".film");
		file_rename(&m_file_reference, destination_filename.get_string());
	}

	if (VALID_INDEX(m_content_controller_index, k_number_of_controllers))
	{
		//c_content_catalogue* content_catalogue = content_catalogue_get_interface(m_content_controller_index);
		//if (content_catalogue->content_item_try_and_get_from_file_reference(&m_file_reference) == NONE)
		//{
		//	event(_event_warning, "networking:saved_film: on close failed to get content item for controller %d",
		//		m_content_controller_index);
		//}
	}

	c_saved_film::reset_internal_state();
	return result;
}

// $REVIEW
bool c_saved_film::header_valid(const s_blf_saved_film* header, bool disable_version_check) const
{
	ASSERT(header);

	if (header->film_data.header.chunk_size < 0 || header->film_data.header.chunk_size + k_maximum_game_state_header_size > k_saved_film_maximum_size)
	{
		event(_event_warning, "networking:saved_film: film payload %d bytes is outside valid range (max %d)",
			header->film_data.header.chunk_size,
			k_saved_film_maximum_size);
		return false;
	}

	//if (!disable_version_check)
	//{
	//	int32 local_executable_type = 0;
	//	int32 local_executable_version = 0;
	//	int32 local_compatible_version = 0;
	//	network_get_build_identifiers(&local_executable_type, &local_executable_version, &local_compatible_version);
	//
	//	int32 local_determinism_version = 0;
	//	int32 local_determinism_compatible_version = 0;
	//	game_get_determinism_versions(&local_determinism_version, &local_determinism_compatible_version);
	//
	//	if (header->film_header.build_compatibility.executable_type != local_executable_type)
	//	{
	//		event(_event_warning, "networking:saved_film: executable type mismatch [%d!=%d], film not valid",
	//			header->film_header.build_compatibility.executable_type,
	//			local_executable_type);
	//		return false;
	//	}
	//
	//	if (!game_determinism_version_compatible(header->film_header.options.determinism_version))
	//	{
	//		event(_event_warning, "networking:saved_film: determinism version mismatch [film %d local %d/%d], film not valid",
	//			header->film_header.options.determinism_version,
	//			local_determinism_version,
	//			local_determinism_compatible_version);
	//		return false;
	//	}
	//
	//	if (header->film_header.build_compatibility.network_executable_version != local_executable_version)
	//	{
	//		event(_event_warning, "networking:saved_film: film executable version is different than local [%d != %d], relying on deterministic version",
	//			header->film_header.build_compatibility.network_executable_version,
	//			local_executable_version);
	//	}
	//}

	char error_string[512]{};
	if (!game_options_verify(&header->film_header.options, error_string, sizeof(error_string)))
	{
		event(_event_warning, "networking:saved_film: film header has invalid game options (%s)",
			error_string);
		return false;
	}

	if (header->film_header.length_in_ticks <= 0)
	{
		event(_event_warning, "networking:saved_film: film header has invalid length in ticks [%d]",
			header->film_header.length_in_ticks);
		return false;
	}

	if (header->film_header.is_snippet)
	{
		if (!header->film_header.contains_gamestate)
		{
			event(_event_warning, "networking:saved_film: film is marked as snippet but does not contain gamestate?");
			return false;
		}

		if (header->film_header.snippet_start_tick < 0)
		{
			event(_event_warning, "networking:saved_film: film is marked as snippet but has bad snippet start tick %d",
				header->film_header.snippet_start_tick);
			return false;
		}
	}

	return true;
}

bool c_saved_film::parse_update_header(uns32 header, s_saved_film_update* update_out) const
{
	e_saved_film_update_type update_type = (e_saved_film_update_type)(header >> k_saved_film_update_size_bits);
	int32 update_size = (int32)(header & k_saved_film_update_size_max);

	if (update_type > _saved_film_update_type_gamestate)
	{
		event(_event_error, "networking:saved_film: header has bad update type %d",
			update_type);
		return false;
	}

	if (update_size <= 0 || update_size > k_saved_film_update_size_max)
	{
		event(_event_error, "networking:saved_film: header has bad update size %d",
			update_size);
		return false;
	}

	update_out->update_type = update_type;
	update_out->update_size = update_size;
	return true;
}

uns32 c_saved_film::build_update_header(const s_saved_film_update* update) const
{
	ASSERT(update->update_size > 0 && update->update_size <= k_saved_film_update_size_max);

	if (update->update_size > k_saved_film_update_size_max)
	{
		return (update->update_type << k_saved_film_update_size_bits) | FLAG(k_saved_film_update_size_bits);
	}

	int32 update_size = 0;
	if (update->update_size > 0)
	{
		update_size = update->update_size;
	}

	return update_size | (update->update_type << k_saved_film_update_size_bits);
}

e_controller_index c_saved_film::get_cached_controller_index() const
{
	return m_content_controller_index;
}

s_file_reference* c_saved_film::get_file_reference()
{
	return &m_file_reference;
}

void c_saved_film::mark_film_as_snippet(int32 start_tick)
{
	ASSERT(m_film_state == _saved_film_open_for_write);
	ASSERT(start_tick != NONE);
	ASSERT(start_tick >= 0);

	m_film_header.film_header.snippet_start_tick = start_tick;
	m_film_header.film_header.is_snippet = true;
}

int32 c_saved_film::get_map_signature_size() const
{
	return m_film_header.film_header.build_compatibility.map_signature_size;
}

const byte* c_saved_film::get_map_signature_bytes() const
{
	return m_film_header.film_header.build_compatibility.map_signature_bytes;
}

//void c_saved_film::copy_film(const char* source_file, const char* destination_file)

void c_saved_film::reset_internal_state()
{
	m_film_state = k_saved_film_state_none;
	csmemset(&m_film_header, 0, sizeof(s_blf_saved_film));
	m_start_of_film_data_offset = 0;
	m_current_film_offset = 0;
	m_current_tick = 0;
	m_async_double_buffer.initialize();
	csmemset(&m_file_reference, 0, sizeof(s_file_reference));
	m_content_controller_index = k_no_controller;
	m_fatal_error_encountered_during_write = false;
	m_finalizing_film = false;
}

bool c_saved_film::write_game_state_internal(int32 update_number, const void* compressed_game_state, int32 compressed_game_state_size)
{
	ASSERT(m_film_state == _saved_film_open_for_write);

	uns32 header = 0x4000000C;
	if (!c_saved_film::write_data(&header, sizeof(header)))
	{
		event(_event_warning, "networking:saved_film: write failed to write header for game state [compressed size %d]",
			compressed_game_state_size);

		m_fatal_error_encountered_during_write = true;
		return false;
	}

	s_saved_film_gamestate_header saved_film_gamestate_header{};
	saved_film_gamestate_header.compressed_size = compressed_game_state_size;
	saved_film_gamestate_header.checksum = NONE;// fast_checksum_buffer(NONE, compressed_game_state, compressed_game_state_size);
	saved_film_gamestate_header.update_number = update_number;
	if (!c_saved_film::write_data(&saved_film_gamestate_header, sizeof(s_saved_film_gamestate_header)))
	{
		event(_event_warning, "networking:saved_film: write failed to game state header [header size %d]",
			sizeof(s_saved_film_gamestate_header));

		m_fatal_error_encountered_during_write = true;
		return false;
	}

	if (!c_saved_film::write_data(compressed_game_state, compressed_game_state_size))
	{
		event(_event_warning, "networking:saved_film: write failed to write compressed game state [size %d]",
			compressed_game_state_size);

		m_fatal_error_encountered_during_write = true;
		return false;
	}

	m_film_header.film_header.contains_gamestate = true;
	return true;
}

int32 const k_main_time_default_vblank_rate = 60;
int32 const k_main_time_ideal_vblank_interval_per_game_update = 1;

bool c_saved_film::write_finish()
{
	ASSERT(m_film_state == _saved_film_open_for_write);

	m_finalizing_film = true;

	if (!async_usable())
	{
		event(_event_warning, "networking:saved_film: async not usable, won't be able to finish the write!");
		m_finalizing_film = false;
		return false;
	}

	if (m_fatal_error_encountered_during_write)
	{
		event(_event_warning, "networking:saved_film: finishing saved film write with an encountered fatal error");
	}
	else
	{
		ASSERT(m_current_film_offset == m_async_double_buffer.get_position());
		ASSERT(m_current_film_offset >= sizeof(s_blf_saved_film) && m_current_film_offset <= k_saved_film_maximum_size);
	}

	s_blf_chunk_end_of_file end_of_file{};
	end_of_file.total_file_size = m_async_double_buffer.get_position();
	const int32 current_film_offset = m_current_film_offset;

	bool write_success = c_saved_film::write_data(&end_of_file, sizeof(end_of_file));
	if (!write_success)
	{
		event(_event_error, "networking:saved_film: saved_film_write_finish failed to write footer");
		m_finalizing_film = false;
		return false;
	}

	m_current_film_offset = current_film_offset;
	const int32 file_size_before_padding = m_async_double_buffer.get_position();

	const int32 padding_dwords = (((file_size_before_padding + 0xFFF) & ~0xFFFu) - file_size_before_padding) >> 2;
	uns32 pad = 0;
	for (int32 i = 0; i < padding_dwords; i++)
	{
		if (!c_saved_film::write_data(&pad, sizeof(pad)))
		{
			event(_event_warning, "networking:saved_film: failed to write dword padding data at end of film");
			write_success = false;
			break;
		}
	}

	byte pad_byte = 0;
	for (int32 i = 0, pad_bytes = (-(byte)file_size_before_padding & 3); write_success && i < pad_bytes; i++)
	{
		if (!c_saved_film::write_data(&pad_byte, sizeof(pad_byte)))
		{
			event(_event_warning, "networking:saved_film: failed to write byte padding data at end of film");
			write_success = false;
			break;
		}
	}

	if (!write_success || m_fatal_error_encountered_during_write)
	{
		m_finalizing_film = false;
		return false;
	}

	s_blf_saved_film* header = &m_film_header;

	header->film_header.length_in_ticks = m_current_tick;

	real32 length_seconds = 0.0f;
	if (game_time_initialized())
	{
		length_seconds = game_ticks_to_seconds((real32)m_current_tick);
	}
	else
	{
		length_seconds = (real32)(m_current_tick / (k_main_time_default_vblank_rate / k_main_time_ideal_vblank_interval_per_game_update));
	}

	header->content_header.metadata.size_in_bytes = file_size_before_padding;
	header->content_header.metadata.length_seconds = (int32)length_seconds;
	header->content_header.metadata.map_id = header->film_header.options.map_id;
	header->content_header.metadata.file_type = e_saved_game_file_type(_saved_game_film + header->film_header.is_snippet);
	header->content_header.metadata.game_engine_index = header->film_header.options.multiplayer_variant.get_game_engine_index();

	const int32 film_data_size = current_film_offset - m_start_of_film_data_offset;
	header->film_data.header.chunk_size = film_data_size + sizeof(s_blf_header);

	ASSERT(header->film_data.header.chunk_size >= 0);
	ASSERT(header->film_data.header.chunk_size == (int32)(file_size_before_padding - sizeof(s_blf_saved_film) - sizeof(s_blf_chunk_end_of_file) + sizeof(s_blf_header)));

	if (!c_saved_film::header_valid(header, false))
	{
		event(_event_warning, "networking:saved_film: header built but is invalid");
		m_finalizing_film = false;
		return false;
	}

	if (!m_async_double_buffer.set_position(0))
	{
		event(_event_error, "networking:saved_film: failed to seek to beginning to write header");
		m_finalizing_film = false;
		return false;
	}

	if (!c_saved_film::write_data(header, sizeof(s_blf_saved_film)))
	{
		event(_event_error, "networking:saved_film: failed to write header");
		m_finalizing_film = false;
		return false;
	}

	m_finalizing_film = false;
	return true;
}

bool c_saved_film::read_data(void* data, int32 data_size)
{
	ASSERT(data);
	ASSERT(data_size > 0);
	ASSERT(m_film_state == _saved_film_open_for_read);
	ASSERT(m_async_double_buffer.ready_to_read());

	int32 bytes_read = 0;
	m_async_double_buffer.read(data, data_size, &bytes_read);
	if (bytes_read != data_size)
	{
		event(_event_warning, "networking:saved_film: film async operation returned unexpected results. Wanted to read %d bytes, but async buffer read only %d.",
			data_size,
			bytes_read);
		return false;
	}

	return true;
}

bool c_saved_film::write_data(const void* data, int32 data_size)
{
	ASSERT(m_film_state == _saved_film_open_for_write);

	if (!m_finalizing_film && data_size + m_current_film_offset > k_saved_film_maximum_size)
	{
		event(_event_warning, "networking:saved_film: the film exceeds the maximum size allowable. Trying to write %d bytes at offset %d (maximum film size %d)",
			data_size,
			m_current_film_offset,
			k_saved_film_maximum_size);
		return false;
	}

	int32 bytes_written = 0;
	m_async_double_buffer.write(data, data_size, &bytes_written);
	m_current_film_offset += bytes_written;

	if (bytes_written != data_size)
	{
		event(_event_warning, "networking:saved_film: film async operation returned unexpected results. Wanted to write %d bytes, but async buffer wrote only %d.",
			data_size,
			bytes_written);
		return false;
	}

	return true;
}

c_saved_film::~c_saved_film()
{
}

