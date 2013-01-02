/*
 * kixmail-socket.c
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
 *
 * Based on soup-socket.c from libsoup
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "kixmail-socket.h"

#define DEBUG_FLAG KIXMAIL_DEBUG_OTHER
#include <libkix/kixmail-debug.h>
#include <libkix/kixmail-sock-addr.h>


G_DEFINE_TYPE (KixmailSocket, kixmail_socket, G_TYPE_OBJECT)

#define KIXMAIL_SOCKET_GET_PRIVATE(object)                  \
  (G_TYPE_INSTANCE_GET_PRIVATE ((object),                   \
                                KIXMAIL_TYPE_SOCKET,        \
                                KixmailSocketPriv))

struct _KixmailSocketPriv {
  KixmailSockAddr *local_addr, *remote_addr;
  GIOStream *conn, *iostream;
  GSocket *gsock;
  GInputStream *istream;
  GOutputStream *ostream;
  GTlsCertificateFlags tls_errors;

	guint non_blocking:1;
	guint is_server:1;
	guint ssl:1;
	guint ssl_strict:1;
	guint ssl_fallback:1;
	guint clean_dispose:1;
	guint use_thread_context:1;
	guint use_proxy:1;
	gpointer ssl_creds;

	GMainContext   *async_context;
	GSource        *watch_src;
	GSource        *read_src, *write_src;

	GMutex iolock, addrlock;
	guint timeout;

	GCancellable *connect_cancel;
};



/* Internal Functions */

/* Class Implementation */
static void
kixmail_socket_finalize (GObject *object)
{

  G_OBJECT_CLASS (kixmail_socket_parent_class)->finalize (object);
}

static void
kixmail_socket_set_property (GObject *object,
                             guint prop_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
}

static void
kixmail_socket_get_property (GObject *object,
                             guint prop_id,
                             GValue *value,
                             GParamSpec *pspec)
{
}

static void
kixmail_socket_class_init (KixmailSocketClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = kixmail_socket_finalize;
  object_class->set_property = kixmail_socket_set_property;
  object_class->get_property = kixmail_socket_get_property;

  g_type_class_add_private (klass, sizeof (KixmailSocketPriv));
}

static void
kixmail_socket_init (KixmailSocket *socket)
{
  KixmailSocketPriv *priv = KIXMAIL_SOCKET_GET_PRIVATE (socket);

  priv->non_blocking = TRUE;
}

/* Public Methods */
KixmailSocket *
kixmail_socket_new (const char *sockopt1, ...)
{
  KixmailSocket *sock;
  va_list ap;

  va_start (ap, sockopt1);
  sock = (KixmailSocket *)g_object_new_valist (KIXMAIL_TYPE_SOCKET,
                                               sockopt1, ap);
  va_end (ap);

  return sock;
}
