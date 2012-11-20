/*
 * kixmail-window.h
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


#ifndef _KIXMAIL_WINDOW_H_
#define _KIXMAIL_WINDOW_H_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KIXMAIL_TYPE_WINDOW         (kixmail_window_get_type ())
#define KIXMAIL_WINDOW(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), KIXMAIL_TYPE_WINDOW, KixmailWindow))
#define KIXMAIL_WINDOW_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k), KIXMAIL_TYPE_WINDOW, KixmailWindowClass))
#define KIXMAIL_IS_WINDOW(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), KIXMAIL_TYPE_WINDOW))
#define KIXMAIL_IS_WINDOW_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), KIXMAIL_TYPE_WINDOW))
#define KIXMAIL_WINDOW_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), KIXMAIL_TYPE_WINDOW, KixmailWindowClass))

typedef struct _KixmailWindow KixmailWindow;
typedef struct _KixmailWindowClass KixmailWindowClass;
typedef struct _KixmailWindowPriv KixmailWindowPriv;

struct _KixmailWindowClass
{
  GtkApplicationWindowClass parent_class;

  void   (* close) (KixmailWindow *window);
};

struct _KixmailWindow
{
  GtkApplicationWindow parent;

  KixmailWindowPriv *priv;
};

GType kixmail_window_get_type (void) G_GNUC_CONST;

GtkWidget * kixmail_window_new (GtkApplication *app);

GMenuModel * kixmail_window_get_menu_model (KixmailWindow *window);

void kixmail_window_show_sidebar (KixmailWindow *window);
void kixmail_window_hide_sidebar (KixmailWindow *window);

G_END_DECLS
#endif /*_KIXMAIL_WINDOW_H_*/
