/*
 * kixmail-application.h
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

#ifndef _KIXMAIL_APPLICATION_H_
#define _KIXMAIL_APPLICATION_H_

#include <gdk/gdk.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include "kixmail-window.h"

G_BEGIN_DECLS

#define KIXMAIL_TYPE_APPLICATION             (kixmail_application_get_type ())
#define KIXMAIL_APPLICATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), KIXMAIL_TYPE_APPLICATION, KixmailApplication))
#define KIXMAIL_APPLICATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), KIXMAIL_TYPE_APPLICATION, KixmailApplicationClass))
#define KIXMAIL_IS_APPLICATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KIXMAIL_TYPE_APPLICATION))
#define KIXMAIL_IS_APPLICATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), KIXMAIL_TYPE_APPLICATION))
#define KIXMAIL_APPLICATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), KIXMAIL_TYPE_APPLICATION, KixmailApplicationClass))

typedef struct _KixmailApplicationClass KixmailApplicationClass;
typedef struct _KixmailApplication KixmailApplication;
typedef struct _KixmailApplicationPriv KixmailApplicationPriv;

enum
{
    PROP_NO_CONNECT = 1,
    PROP_START_HIDDEN
};

struct _KixmailApplicationClass
{
	GtkApplicationClass parent_class;
};

struct _KixmailApplication
{
	GtkApplication parent_instance;


  KixmailApplicationPriv *priv;
};

GType kixmail_application_get_type (void) G_GNUC_CONST;


G_END_DECLS

#endif /* _KIXMAIL_APPLICATION_H_ */
