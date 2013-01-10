/*
 * kixmail-io-stream.h
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

#ifndef _KIXMAIL_IO_STREAM_H_
#define _KIXMAIL_IO_STREAM_H_

#include <gio/gio.h>

G_BEGIN_DECLS

#define KIXMAIL_TYPE_IO_STREAM             (kixmail_io_stream_get_type ())
#define KIXMAIL_IO_STREAM(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), KIXMAIL_TYPE_IO_STREAM, KixmailIOStream))
#define KIXMAIL_IO_STREAM_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), KIXMAIL_TYPE_IO_STREAM, KixmailIOStreamClass))
#define KIXMAIL_IS_IO_STREAM(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KIXMAIL_TYPE_IO_STREAM))
#define KIXMAIL_IS_IO_STREAM_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), KIXMAIL_TYPE_IO_STREAM))
#define KIXMAIL_IO_STREAM_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), KIXMAIL_TYPE_IO_STREAM, KixmailIOStreamClass))

typedef struct _KixmailIOStreamClass KixmailIOStreamClass;
typedef struct _KixmailIOStream KixmailIOStream;
typedef struct _KixmailIOStreamPriv KixmailIOStreamPriv;

struct _KixmailIOStreamClass {
  GIOStreamClass parent_class;

  /* signals */
  void (*readable) (KixmailIOStream *);
  void (*writable) (KixmailIOStream *);
  void (*disconnected) (KixmailIOStream *);

  void (*new_connection) (KixmailIOStream *, KixmailIOStream *);
};

struct _KixmailIOStream {
  GIOStream parent;

  KixmailIOStreamPriv *priv;
};

GType kixmail_io_stream_get_type (void);

GIOStream *kixmail_io_stream_new (GIOStream *base_iostream,
                                  gboolean close_on_dispose);

G_END_DECLS
#endif /* _KIXMAIL_IO_STREAM_H_ */

