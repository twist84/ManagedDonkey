#include "networking/tools/remote_command.hpp"

#include "donkey_networking/donkey_command_server.hpp"

#include "camera/observer.hpp"
#include "cseries/console.hpp"
#include "game/game.hpp"
#include "game/player_mapping.hpp"
#include "networking/transport/transport_endpoint_winsock.hpp"

#include <assert.h>

s_remote_command_globals remote_command_globals;

void __cdecl remote_command_initialize()
{
    command_server.start(11770);
}

void __cdecl remote_command_dispose()
{
    command_server.stop();
}

void __cdecl remote_command_disconnect()
{
    if (remote_command_globals.receive_endpoint)
    {
        if (remote_command_globals.send_endpoint == remote_command_globals.receive_endpoint)
            remote_command_globals.send_endpoint = NULL;

        transport_endpoint_delete(remote_command_globals.receive_endpoint);
        remote_command_globals.receive_endpoint = NULL;
    }

    if (remote_command_globals.send_endpoint)
    {
        transport_endpoint_delete(remote_command_globals.send_endpoint);
        remote_command_globals.send_endpoint = NULL;
    }
}

bool __cdecl remote_command_process_received_chunk(char const* buffer, long buffer_length)
{
    assert(buffer);
    assert(buffer_length > 0);

    // #TODO: while loop...

    return true;
}

bool __cdecl remote_camera_update(long user_index, struct s_observer_result const* camera)
{
    return false;
}

void __cdecl remote_command_process()
{
    // Check if there's a pending incoming connection request
    if (remote_command_globals.listen_endpoint && transport_endpoint_readable(remote_command_globals.listen_endpoint))
    {
        transport_endpoint* endpoint = transport_endpoint_accept(remote_command_globals.listen_endpoint);
        if (endpoint)
        {
            // If we're already connected to a remote host, disconnect from it first
            if (remote_command_globals.receive_endpoint)
            {
                c_console::write_line("### remote connection attempt causing us to drop existing connection to a host");
                remote_command_disconnect();
            }

            // Assign the new connection endpoint as the receive endpoint
            assert(remote_command_globals.receive_endpoint == nullptr);
            remote_command_globals.receive_endpoint = endpoint;

            // If the new endpoint is writeable, set it as the send endpoint too
            if (transport_endpoint_writeable(endpoint))
            {
                remote_command_globals.send_endpoint = remote_command_globals.receive_endpoint;
            }

            c_console::write_line("received a connection from a remote host!");
        }
    }

    // Check if we've lost connection to the remote host
    if (remote_command_globals.send_endpoint && (!transport_endpoint_connected(remote_command_globals.send_endpoint) || !transport_endpoint_writeable(remote_command_globals.send_endpoint)))
    {
        c_console::write_line("### lost connection to remote xbox");
        remote_command_disconnect();
    }

    // Process incoming data from the remote host
    if (remote_command_globals.receive_endpoint)
    {
        bool continue_processing = true;

        while (continue_processing)
        {
            // Check if the connection has been closed or there's no data to read
            if (!transport_endpoint_connected(remote_command_globals.receive_endpoint) || !transport_endpoint_readable(remote_command_globals.receive_endpoint))
            {
                continue_processing = false;
                break;
            }

            char buffer[32] = {};
            short buffer_length = transport_endpoint_read(remote_command_globals.receive_endpoint, buffer, sizeof(buffer));

            // If there's no data to read or an error occurred, stop processing
            if (buffer_length <= 0)
            {
                continue_processing = false;
                break;
            }

            // Process the received data
            if (!remote_command_process_received_chunk(buffer, buffer_length))
            {
                c_console::write_line("remote command client couldn't process received command");

                // Disconnect if there's an error
                remote_command_disconnect();
                continue_processing = false;
                break;
            }

            // If there's no more data to read, stop processing
            if (buffer_length < sizeof(buffer))
            {
                continue_processing = false;
                break;
            }
        }
    }

    // Update remote cameras if the game is in progress
    if (game_in_progress())
    {
        for (long user_index = 0; user_index < 4; ++user_index)
        {
            if (players_user_is_active(user_index))
            {
                s_observer_result const* camera = observer_try_and_get_camera(user_index);
                if (camera)
                {
                    remote_camera_update(user_index, camera);
                }
            }
        }
    }
}
