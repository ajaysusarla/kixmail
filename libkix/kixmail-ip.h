/*
 * kixmail-ip.h
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

#ifndef _KIXMAIL_IP_H_
#define _KIXMAIL_IP_H_

#include <signal.h>

G_BEGIN_DECLS

#define KIXMAIL_TYPE_IP             (kixmail_ip_get_type ())
#define KIXMAIL_IP(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), KIXMAIL_TYPE_IP, KixmailIp))
#define KIXMAIL_IP_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), KIXMAIL_TYPE_IP, KixmailIpClass))
#define KIXMAIL_IS_IP(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), KIXMAIL_TYPE_IP))
#define KIXMAIL_IS_IP_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), KIXMAIL_TYPE_IP))
#define KIXMAIL_IP_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), KIXMAIL_TYPE_IP, KixmailIpClass))

typedef struct _KixmailIpClass KixmailIpClass;
typedef struct _KixmailIp KixmailIp;
typedef struct _KixmailIpPriv KixmailIpPriv;

struct _KixmailIpClass {
  GObjectClass parent_class;
};

struct _KixmailIp {
  GObject parent;

  KixmailIpPriv *priv;
};


GType kixmail_ip_get_type (void) G_GNUC_CONST;

G_END_DECLS
#endif /* _KIXMAIL_IP_H_ */
