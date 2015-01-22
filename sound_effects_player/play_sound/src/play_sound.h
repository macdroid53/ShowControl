/*
 * play_sound.h
 * Copyright © 2015 by John Sauter <John_Sauter@systemeyescomputerstore.com>
 * 
 * play_sound is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * play_sound is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _PLAY_SOUND_
#define _PLAY_SOUND_

#include <gtk/gtk.h>
#include <gst/gst.h>

G_BEGIN_DECLS
#define PLAY_SOUND_TYPE_APPLICATION             (play_sound_get_type ())
#define PLAY_SOUND_APPLICATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), PLAY_SOUND_TYPE_APPLICATION, Play_Sound))
#define PLAY_SOUND_APPLICATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), PLAY_SOUND_TYPE_APPLICATION, Play_SoundClass))
#define PLAY_SOUND_IS_APPLICATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PLAY_SOUND_TYPE_APPLICATION))
#define PLAY_SOUND_IS_APPLICATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), PLAY_SOUND_TYPE_APPLICATION))
#define PLAY_SOUND_APPLICATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), PLAY_SOUND_TYPE_APPLICATION, Play_SoundClass))
typedef struct _Play_SoundClass Play_SoundClass;
typedef struct _Play_Sound Play_Sound;
typedef struct _Play_SoundPrivate Play_SoundPrivate;

struct _Play_SoundClass
{
  GtkApplicationClass parent_class;
};

struct _Play_Sound
{
  GtkApplication parent_instance;

  Play_SoundPrivate *priv;

};

GType
play_sound_get_type (void) G_GNUC_CONST;
     Play_Sound *play_sound_new (void);

/* Information about a sound effect we might play */
 struct sound_effect_str
 {
   GtkWidget *cluster;    /* The cluster the sound is in, if any */
   gchar *file_name;      /* The name of the file holding the sound */
   GstBin *sound_control; /* The Gstreamer bin for this sound effect */
   gint cluster_number;   /* The number of the cluster the sound is in */
 };

/* Callbacks */

/* Given a widget, get its pipeline. */
 GstPipeline *play_sound_get_pipeline (GtkWidget * object);

/* Given a widget in a cluster, get its sound_effect structure. */
 struct sound_effect_str *play_sound_get_sound_effect (GtkWidget *
                                                           object);

/* Given the application, find the common area above the clusters. */
 GtkWidget *play_sound_find_common_area (GApplication * app);

/* Given the application, find the network buffer. */
gchar *play_sound_get_network_buffer (GApplication *app);

/* Given the application, find the parser information. */
void *play_sound_get_parse_info (GApplication *app);

/* Start playing the sound in a specified cluster. */
void play_sound_start_cluster (int cluster_no, GApplication *app);

/* Stop playing the sound in a specified cluster. */
void play_sound_stop_cluster (int cluster_no, GApplication *app);

G_END_DECLS
#endif /* _APPLICATION_H_ */
