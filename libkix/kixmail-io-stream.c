/*
 * kixmail-io-stream.c
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "kixmail-io-stream.h"
#include "kixmail-filter-input-stream.h"

G_DEFINE_TYPE (KixmailIOStream, kixmail_io_stream, G_TYPE_IO_STREAM)

#define KIXMAIL_IO_STREAM_GET_PRIVATE(object)                  \
  (G_TYPE_INSTANCE_GET_PRIVATE ((object),                      \
                                KIXMAIL_TYPE_IO_STREAM,        \
                                KixmailIOStreamPriv))

struct _KixmailIOStreamPriv {
  GIOStream *base_iostream;
  gboolean close_on_dispose;

  GInputStream *istream;
  GOutputStream *ostream;
  gboolean disposing;
};

enum {
	PROP_0,

	PROP_BASE_IOSTREAM,
	PROP_CLOSE_ON_DISPOSE
};


/* Internal Functions */

/* Class Implementation */
static void
kixmail_io_stream_set_property (GObject *object,
                                guint prop_id,
                                const GValue *value,
                                GParamSpec *pspec)
{
  KixmailIOStream *iostream = KIXMAIL_IO_STREAM (object);
  GIOStream *io;

  switch (prop_id) {
	case PROP_BASE_IOSTREAM:
		io = iostream->priv->base_iostream = g_value_dup_object (value);
		if (io) {
			iostream->priv->istream =
				kixmail_filter_input_stream_new (g_io_stream_get_input_stream (io));
			iostream->priv->ostream =
				g_object_ref (g_io_stream_get_output_stream (io));
		} else {
			g_clear_object (&iostream->priv->istream);
			g_clear_object (&iostream->priv->ostream);
		}
		break;
	case PROP_CLOSE_ON_DISPOSE:
		iostream->priv->close_on_dispose = g_value_get_boolean (value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
  }
}

static void
kixmail_io_stream_get_property (GObject *object,
                                guint prop_id,
                                GValue *value,
                                GParamSpec *pspec)
{
	KixmailIOStream *iostream = KIXMAIL_IO_STREAM (object);

	switch (prop_id) {
	case PROP_BASE_IOSTREAM:
		g_value_set_object (value, iostream->priv->base_iostream);
		break;
	case PROP_CLOSE_ON_DISPOSE:
		g_value_set_boolean (value, iostream->priv->close_on_dispose);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
kixmail_io_stream_dispose (GObject *object)
{
	KixmailIOStream *iostream = KIXMAIL_IO_STREAM (object);

	iostream->priv->disposing = TRUE;

	G_OBJECT_CLASS (kixmail_io_stream_parent_class)->dispose (object);
}

static void
kixmail_io_stream_finalize (GObject *object)
{
	KixmailIOStream *iostream = KIXMAIL_IO_STREAM (object);

	g_clear_object (&iostream->priv->base_iostream);
	g_clear_object (&iostream->priv->istream);
	g_clear_object (&iostream->priv->ostream);

	G_OBJECT_CLASS (kixmail_io_stream_parent_class)->finalize (object);
}

static GInputStream *
kixmail_io_stream_get_input_stream (GIOStream *stream)
{
  return KIXMAIL_IO_STREAM (stream)->priv->istream;
}

static GOutputStream *
kixmail_io_stream_get_output_stream (GIOStream *stream)
{
  return KIXMAIL_IO_STREAM (stream)->priv->ostream;
}

static gboolean
kixmail_io_stream_close (GIOStream *stream,
                         GCancellable *cancellable,
                         GError **error)
{
  KixmailIOStream *iostream = KIXMAIL_IO_STREAM (stream);

  if (iostream->priv->disposing &&
      !iostream->priv->close_on_dispose)
    return TRUE;

  return g_io_stream_close (iostream->priv->base_iostream, cancellable, error);
}

static void
kixmail_io_stream_close_async (GIOStream *stream,
                               int io_priority,
                               GCancellable *cancellable,
                               GAsyncReadyCallback callback,
                               gpointer user_data)
{
  g_io_stream_close_async (KIXMAIL_IO_STREAM (stream)->priv->base_iostream,
                           io_priority, cancellable, callback, user_data);
}

static gboolean
kixmail_io_stream_close_finish (GIOStream *stream,
                                GAsyncResult *result,
                                GError **error)
{
  return g_io_stream_close_finish (KIXMAIL_IO_STREAM (stream)->priv->base_iostream,
                                   result, error);
}

static void
kixmail_io_stream_class_init (KixmailIOStreamClass *stream_class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (stream_class);
  GIOStreamClass *io_stream_class = G_IO_STREAM_CLASS (stream_class);

  object_class->set_property = kixmail_io_stream_set_property;
  object_class->get_property = kixmail_io_stream_get_property;
  object_class->dispose = kixmail_io_stream_dispose;
  object_class->finalize = kixmail_io_stream_finalize;

	io_stream_class->get_input_stream = kixmail_io_stream_get_input_stream;
	io_stream_class->get_output_stream = kixmail_io_stream_get_output_stream;
	io_stream_class->close_fn = kixmail_io_stream_close;
	io_stream_class->close_async = kixmail_io_stream_close_async;
	io_stream_class->close_finish = kixmail_io_stream_close_finish;

	g_object_class_install_property (
		object_class, PROP_BASE_IOSTREAM,
		g_param_spec_object ("base-iostream",
				     "Base IOStream",
				     "Base GIOStream",
				     G_TYPE_IO_STREAM,
				     G_PARAM_READWRITE |
				     G_PARAM_CONSTRUCT_ONLY));
	g_object_class_install_property (
		object_class, PROP_CLOSE_ON_DISPOSE,
		g_param_spec_boolean ("close-on-dispose",
				      "Close base stream",
				      "Close base GIOStream when closing",
				      TRUE,
				      G_PARAM_READWRITE |
				      G_PARAM_CONSTRUCT_ONLY));

  g_type_class_add_private (stream_class, sizeof (KixmailIOStreamPriv));
}

static void
kixmail_io_stream_init (KixmailIOStream *stream)
{
  stream->priv = KIXMAIL_IO_STREAM_GET_PRIVATE (stream);
}

/* Public Methods */

GIOStream *
kixmail_io_stream_new (GIOStream *base_iostream,
                       gboolean close_on_dispose)
{
  return g_object_new (KIXMAIL_TYPE_IO_STREAM,
                       "base-iostream", base_iostream,
                       "close-on-dispose", close_on_dispose,
                       NULL);
}

