/*
 * kixmail-io.h
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
 *
 */

#ifndef _KIXMAIL_IO_H_
#define _KIXMAIL_IO_H_

#include <signal.h>
#include <glib.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define KIXMAIL_TYPE_IO             (kixmail_io_get_type ())
#define KIXMAIL_IO(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), KIXMAIL_TYPE_IO, KixmailIo))
#define KIXMAIL_IO_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), KIXMAIL_TYPE_IO, KixmailIoClass))
#define KIXMAIL_IS_IO(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KIXMAIL_TYPE_IO))
#define KIXMAIL_IS_IO_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), KIXMAIL_TYPE_IO))
#define KIXMAIL_IO_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), KIXMAIL_TYPE_IO, KixmailIoClass))

typedef struct _KixmailIoClass KixmailIoClass;
typedef struct _KixmailIo KixmailIo;
typedef struct _KixmailIoPriv KixmailIoPriv;

struct _KixmailIoClass {
  GObjectClass parent_class;
};

struct _KixmailIo {
  GObject parent;

  KixmailIoPriv *priv;
};


GType kixmail_io_get_type (void) G_GNUC_CONST;

KixmailIo * kixmail_io_new (void);

G_END_DECLS
#endif /* _KIXMAIL_IO_H_ */
