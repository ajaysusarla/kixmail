/*
 * kixmail-socket.h
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

#ifndef _KIXMAIL_SOCKET_H_
#define _KIXMAIL_SOCKET_H_

#include <glib.h>
#include <gio/gio.h>
#include <signal.h>

G_BEGIN_DECLS

#define KIXMAIL_TYPE_SOCKET             (kixmail_socket_get_type ())
#define KIXMAIL_SOCKET(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), KIXMAIL_TYPE_SOCKET, KixmailSocket))
#define KIXMAIL_SOCKET_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), KIXMAIL_TYPE_SOCKET, KixmailSocketClass))
#define KIXMAIL_IS_SOCKET(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KIXMAIL_TYPE_SOCKET))
#define KIXMAIL_IS_SOCKET_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), KIXMAIL_TYPE_SOCKET))
#define KIXMAIL_SOCKET_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), KIXMAIL_TYPE_SOCKET, KixmailSocketClass))

typedef struct _KixmailSocketClass KixmailSocketClass;
typedef struct _KixmailSocket KixmailSocket;
typedef struct _KixmailSocketPriv KixmailSocketPriv;

struct _KixmailSocketClass {
  GObjectClass parent_class;

  /* signals */
  void (*readable) (KixmailSocket *);
  void (*writable) (KixmailSocket *);
  void (*disconnected) (KixmailSocket *);

  void (*new_connection) (KixmailSocket *, KixmailSocket *);
};

struct _KixmailSocket {
  GObject parent;

  KixmailSocketPriv *priv;
};

#define KIXMAIL_SOCKET_LOCAL_ADDRESS       "local-address"
#define KIXMAIL_SOCKET_REMOTE_ADDRESS      "remote-address"
#define KIXMAIL_SOCKET_FLAG_NONBLOCKING    "non-blocking"
#define KIXMAIL_SOCKET_IS_SERVER           "is-server"
#define KIXMAIL_SOCKET_SSL_CREDENTIALS     "ssl-creds"
#define KIXMAIL_SOCKET_SSL_STRICT          "ssl-strict"
#define KIXMAIL_SOCKET_SSL_FALLBACK        "ssl-fallback"
#define KIXMAIL_SOCKET_TRUSTED_CERTIFICATE "trusted-certificate"
#define KIXMAIL_SOCKET_ASYNC_CONTEXT       "async-context"
#define KIXMAIL_SOCKET_USE_THREAD_CONTEXT  "use-thread-context"
#define KIXMAIL_SOCKET_TIMEOUT             "timeout"
#define KIXMAIL_SOCKET_TLS_CERTIFICATE     "tls-certificate"
#define KIXMAIL_SOCKET_TLS_ERRORS          "tls-errors"

typedef void (*KixmailSocketCallback) (KixmailSocket *sock,
                                         guint status,
                                         gpointer user_data);

GType kixmail_socket_get_type (void) G_GNUC_CONST;

KixmailSocket * kixmail_socket_new (const char *sockopt1, ...);

G_END_DECLS
#endif /* _KIXMAIL_SOCKET_H_ */
