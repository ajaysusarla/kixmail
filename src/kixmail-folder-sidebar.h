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

#ifndef _KIXMAIL_FOLDER_SIDEBAR_H_
#define _KIXMAIL_FOLDER_SIDEBAR_H_

#include "kixmail-window.h"

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KIXMAIL_TYPE_FOLDER_SIDEBAR             (kixmail_folder_sidebar_get_type ())
#define KIXMAIL_FOLDER_SIDEBAR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), KIXMAIL_TYPE_FOLDER_SIDEBAR, KixmailFolderSidebar))
#define KIXMAIL_FOLDER_SIDEBAR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), KIXMAIL_TYPE_FOLDER_SIDEBAR, KixmailFolderSidebarClass))
#define KIXMAIL_IS_FOLDER_SIDEBAR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KIXMAIL_TYPE_FOLDER_SIDEBAR))
#define KIXMAIL_IS_FOLDER_SIDEBAR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), KIXMAIL_TYPE_FOLDER_SIDEBAR))
#define KIXMAIL_FOLDER_SIDEBAR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), KIXMAIL_TYPE_FOLDER_SIDEBAR, KixmailFolderSidebarClass))

typedef struct _KixmailFolderSidebar KixmailFolderSidebar;
typedef struct _KixmailFolderSidebarClass KixmailFolderSidebarClass;
typedef struct _KixmailFolderSidebarPriv KixmailFolderSidebarPriv;


struct _KixmailFolderSidebar {
  GtkScrolledWindow parent;

  KixmailFolderSidebarPriv *priv;
};

struct _KixmailFolderSidebarClass {
  GtkScrolledWindowClass parent;
};

GType kixmail_folder_sidebar_get_type (void)  G_GNUC_CONST;

GtkWidget * kixmail_folder_sidebar_new (KixmailWindow *window);

G_END_DECLS

#endif /* _KIXMAIL_FOLDER_SIDEBAR_H_ */
