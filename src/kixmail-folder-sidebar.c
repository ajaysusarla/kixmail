/*
 * kixmail-folder-sidebar.h
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

#include <config.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <gio/gio.h>

#define DEBUG_FLAG KIXMAIL_DEBUG_OTHER
#include <libkix/kixmail-debug.h>

#include "kixmail-folder-sidebar.h"


/* Internal methods */

/* Class implementation */


/* Public methods */
GtkWidget *
kixmail_folder_sidebar_new (KixmailWindow *window)
{
  KixmailFolderSidebar *sidebar = NULL;

  return GTK_WIDGET (sidebar);
}
