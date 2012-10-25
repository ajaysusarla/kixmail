/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * kixmail.h
 * Copyright (C) Parthasarathi Susarla 2012 <ajaysusarla@gmail.com>
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

#ifndef _KIXMAIL_
#define _KIXMAIL_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KIXMAIL_TYPE_APPLICATION             (kixmail_get_type ())
#define KIXMAIL_APPLICATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), KIXMAIL_TYPE_APPLICATION, Kixmail))
#define KIXMAIL_APPLICATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), KIXMAIL_TYPE_APPLICATION, KixmailClass))
#define KIXMAIL_IS_APPLICATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KIXMAIL_TYPE_APPLICATION))
#define KIXMAIL_IS_APPLICATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), KIXMAIL_TYPE_APPLICATION))
#define KIXMAIL_APPLICATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), KIXMAIL_TYPE_APPLICATION, KixmailClass))

typedef struct _KixmailClass KixmailClass;
typedef struct _Kixmail Kixmail;

struct _KixmailClass
{
	GtkApplicationClass parent_class;
};

struct _Kixmail
{
	GtkApplication parent_instance;
};

GType kixmail_get_type (void) G_GNUC_CONST;
Kixmail *kixmail_new (void);

/* Callbacks */

G_END_DECLS

#endif /* _APPLICATION_H_ */
