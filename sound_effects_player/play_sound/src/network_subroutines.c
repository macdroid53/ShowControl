/*
 * network_subroutines.c
 *
 * Copyright © 2015 by John Sauter <John_Sauter@systemeyescomputerstore.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <gio/gio.h>
#include "play_sound.h"
#include "network_subroutines.h"

/* Subroutines to handle network messages */

/* Receive incoming data. */
static void
receive_data_callback (GObject * source_object, GAsyncResult * result,
                       gpointer user_data)
{
  gchar *network_buffer = NULL;
  GInputStream *istream;
  GError *error = NULL;
  gssize nread;
  gchar *message;

  istream = G_INPUT_STREAM (source_object);

  nread = g_input_stream_read_finish (istream, result, &error);
  if (error || nread <= 0)
    {
      if (error)
        g_error_free (error);
      g_input_stream_close (istream, NULL, NULL);
  }
    if (nread != 0)
  {
      network_buffer = play_sound_get_network_buffer (user_data);
      network_buffer[nread] = '\0';
      g_print (network_buffer);

      /* Continue reading. */
      message = play_sound_get_network_buffer (user_data);
      g_input_stream_read_async (istream, message, network_buffer_size,
                                 G_PRIORITY_DEFAULT, NULL,
                                 receive_data_callback, user_data);
    }

  return;
}

/* Receive an incoming connection. */
static gboolean
incoming_callback (GSocketService * service, GSocketConnection * connection,
                   GObject * source_object, gpointer user_data)
{
  GInputStream *istream;
  gchar *message;

  g_print ("Received connection.\n");

  istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
  message = play_sound_get_network_buffer (user_data);
  g_input_stream_read_async (istream, message, network_buffer_size,
                             G_PRIORITY_DEFAULT, NULL, receive_data_callback,
                             user_data);
  return FALSE;
}

/* Start up the network listener.  The return value is the network
 * message buffer. */
gchar *
network_init (GApplication * app)
{
  GError *error = NULL;
  GSocketService *service;
  gchar *network_buffer;

  network_buffer = g_malloc0 (network_buffer_size);

  /* Create a new socket service. */
  service = g_socket_service_new ();

  /* Connect to the port. */
  g_socket_listener_add_inet_port ((GSocketListener *) service, 1500,   /* port number */
                                   G_OBJECT (app), &error);
  /* Check for errors. */
  if (error != NULL)
    {
      g_error (error->message);
    }

  /* Listen to the "incoming" signal, which says that we have a connection. */
  g_signal_connect (service, "incoming", G_CALLBACK (incoming_callback), app);

  /* Start the socket service. */
  g_socket_service_start (service);

  return network_buffer;
}