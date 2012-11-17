/*
 * kixmail-utils.h
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

#ifndef _KIXMAIL_UTILS_H_
#define _KIXMAIL_UTILS_H_

#include <glib.h>
#include <glib-object.h>

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <folks/folks.h>

G_BEGIN_DECLS

void kixmail_init (void);

gchar * kixmail_file_lookup (const gchar *filename, const gchar *subdir);

G_END_DECLS

#endif /* _KIXMAIL_UTILS_H_ */
