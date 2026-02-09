#pragma once

#include "cseries/async_buffer.hpp"
#include "networking/tools/network_blf.hpp"

struct s_saved_film_update;

enum e_saved_film_state
{
	_saved_film_open_for_read = 0,
	_saved_film_open_for_write,

	k_saved_film_state_count,

	k_saved_film_state_none = NONE,
};

enum
{
	k_saved_film_update_size_bits = 30,
	k_saved_film_update_size_max = FLAG(k_saved_film_update_size_bits) - 1
};

enum
{
	k_saved_film_maximum_size = 0x6400000,
	k_saved_film_async_io_buffer_size = sizeof(s_blf_saved_film),
	k_saved_film_maximum_map_signature_bytes = 0x3C,
};

enum e_saved_film_update_type
{
	_saved_film_update_type_simulation_update = 0,
	_saved_film_update_type_gamestate,

	k_saved_film_update_type_count,

	k_saved_film_update_type_none = -1,
};

struct s_saved_film_update
{
	e_saved_film_update_type update_type;
	int32 update_size;
};

class c_saved_film
{
public:
	c_saved_film();
	bool initialize(c_allocation_base* allocator);
	void dispose(c_allocation_base* allocator);
	bool open_for_read(const char* filename, e_controller_index controller_index, bool disable_version_check);
	bool read_update(s_saved_film_update* update_out);
	bool read_simulation_update(const s_saved_film_update* update, s_simulation_update* simulation_update_out);
	bool read_gamestate(const s_saved_film_update* update, void* compressed_game_state_buffer_out, int32 buffer_size, int32* compressed_game_state_size_out, int32* update_number_out);
	int32 get_position();
	bool set_position(int32 position);
	bool open_for_write(const char* filename, const game_options* options, e_controller_index controller_index);
	bool write_simulation_update(const s_simulation_update* update);
	bool write_gamestate();
	bool write_gamestate_from_buffer(int32 update_number, const void* gamestate, int32 gamestate_size);
	int32 get_ticks_remaining() const;
	int32 get_current_tick() const;
	int32 get_length_in_ticks() const;
	e_saved_film_state get_film_state() const;
	game_options* get_game_options();
	const s_blf_chunk_content_header* get_film_content_header() const;
	bool contains_gamestate() const;
	bool is_snippet() const;
	int32 get_snippet_start_tick() const;
	bool handle_revert(int32 file_position, int32 film_tick);
	bool close();
	bool header_valid(const s_blf_saved_film* header, bool disable_version_check) const;
	bool parse_update_header(uns32 header, s_saved_film_update* update_out) const;
	uns32 build_update_header(const s_saved_film_update* update) const;
	e_controller_index get_cached_controller_index() const;
	s_file_reference* get_file_reference();
	void mark_film_as_snippet(int32 start_tick);
	int32 get_map_signature_size() const;
	const byte* get_map_signature_bytes() const;
	void copy_film(const char* source_file, const char* destination_file);

private:
	void reset_internal_state();
	bool write_game_state_internal(int32 update_number, const void* compressed_game_state, int32 compressed_game_state_size);
	bool write_finish();
	bool read_data(void* data, int32 data_size);
	bool write_data(const void* data, int32 data_size);

public:
	~c_saved_film();
	c_saved_film& operator=(const c_saved_film&) = default;

//private:
	e_saved_film_state m_film_state;
	s_blf_saved_film m_film_header;
	int32 m_start_of_film_data_offset;
	int32 m_current_film_offset;
	int32 m_current_tick;
	c_async_stored_buffer_set<2> m_async_double_buffer;
	s_file_reference m_file_reference;
	e_controller_index m_content_controller_index;
	bool m_fatal_error_encountered_during_write;
	bool m_finalizing_film;
};

extern bool g_saved_films_show_timestamp;

