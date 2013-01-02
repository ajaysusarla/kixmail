/*
 * kixmail-filter-input-stream.h
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

#ifndef _KIXMAIL_FILTER_INPUT_STREAM_H_
#define _KIXMAIL_FILTER_INPUT_STREAM_H_

#include <gio/gio.h>

G_BEGIN_DECLS

#define KIXMAIL_TYPE_FILTER_INPUT_STREAM             (kixmail_filter_input_stream_get_type ())
#define KIXMAIL_FILTER_INPUT_STREAM(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), KIXMAIL_TYPE_FILTER_INPUT_STREAM, KixmailFilterInputStream))
#define KIXMAIL_FILTER_INPUT_STREAM_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), KIXMAIL_TYPE_FILTER_INPUT_STREAM, KixmailFilterInputStreamClass))
#define KIXMAIL_IS_FILTER_INPUT_STREAM(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KIXMAIL_TYPE_FILTER_INPUT_STREAM))
#define KIXMAIL_IS_FILTER_INPUT_STREAM_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), KIXMAIL_TYPE_FILTER_INPUT_STREAM))
#define KIXMAIL_FILTER_INPUT_STREAM_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), KIXMAIL_TYPE_FILTER_INPUT_STREAM, KixmailFilterInputStreamClass))

typedef struct _KixmailFilterInputStreamClass KixmailFilterInputStreamClass;
typedef struct _KixmailFilterInputStream KixmailFilterInputStream;
typedef struct _KixmailFilterInputStreamPriv KixmailFilterInputStreamPriv;

struct _KixmailFilterInputStreamClass {
  GFilterInputStreamClass parent_class;
};

struct _KixmailFilterInputStream {
  GFilterInputStream parent;

  KixmailFilterInputStreamPriv *priv;
};

GType kixmail_filter_input_stream_get_type (void);

GInputStream *kixmail_filter_input_stream_new (GInputStream  *base_stream);

gssize kixmail_filter_input_stream_read_line  (KixmailFilterInputStream  *fstream,
                                               void                      *buffer,
                                               gsize                     length,
                                               gboolean                  blocking,
                                               gboolean                  *got_line,
                                               GCancellable              *cancellable,
                                               GError                    **error);
gssize kixmail_filter_input_stream_read_until (KixmailFilterInputStream  *fstream,
                                               void                      *buffer,
                                               gsize                     length,
                                               const void                *boundary,
                                               gsize                     boundary_len,
                                               gboolean                  blocking,
                                               gboolean                  include_boundary,
                                               gboolean                  *got_boundary,
                                               GCancellable              *cancellable,
                                               GError                    **error);
G_END_DECLS
#endif /* _KIXMAIL_FILTER_INPUT_STREAM_H_ */

