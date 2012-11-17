/*
 * kixmail-ui-utils.h
 * Copyright (C) 2012  Parthasarathi Susarla <ajaysusarla@gmail.com>
 *
 * kixmail is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * kixmail is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _KIXMAIL_UI_UTILS_H_
#define _KIXMAIL_UI_UTILS_H_

#include <gtk/gtk.h>

#include <folks/folks.h>

G_BEGIN_DECLS

#define KIXMAIL_RECT_IS_ON_SCREEN(x,y,w,h) ((x) + (w) > 0 && \
                                            (y) + (h) > 0 && \
                                            (x) < gdk_screen_width () && \
                                            (y) < gdk_screen_height ())

void kixmail_gtk_init (void);

/* Glade */
GtkBuilder * kixmail_builder_get_file (const gchar *filename,
                                       const gchar *first_object,
                                       ...);

void kixmail_builder_connect (GtkBuilder *gui,
                              gpointer user_data,
                              const gchar *first_object,
                              ...);

GtkWidget * kixmail_builder_unref_and_keep_widget (GtkBuilder *gui,
                                                   GtkWidget *root);

/* Window Management */
void kixmail_window_present_with_time (GtkWindow *window, guint32 timestamp);

void kixmail_window_present (GtkWindow *window);

GtkWindow * kixmail_get_toplevel_window (GtkWidget *widget);

void kixmail_move_to_window_desktop (GtkWindow *window, guint32 timestamp);

/* Misc */
GtkWidget * kixmail_context_menu_new (GtkWidget *attach_to);

void kixmail_launch_program (const gchar *dir,
                             const gchar *name,
                             const gchar *args);

void kixmail_set_css_provider (GtkWidget *widget);

gboolean kixmail_launch_external_app (const gchar *desktop_file,
                                      const gchar *args,
                                      GError **error);

G_END_DECLS
#endif /* _KIXMAIL_UI_UTILS_H_ */
