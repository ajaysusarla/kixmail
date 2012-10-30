/*
 * kixmail-debug.h
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
 * Based on Empathy's empathy-debug.
 */

#ifndef _KIXMAIL_DEBUG_H_
#define _KIXMAIL_DEBUG_H_

#include "config.h"

#include <glib.h>

G_BEGIN_DECLS

/* Make sure to match this with the 'keys' in kixmail-debug.c */
typedef enum
{
  KIXMAIL_DEBUG_ACCOUNT = 1 << 1,
  KIXMAIL_DEBUG_OTHER = 1 << 2,
  KIXMAIL_DEBUG_CONNECTIVITY = 1 << 3,
  KIXMAIL_DEBUG_TLS = 1 << 4,
  KIXMAIL_DEBUG_TESTS = 1 << 5,
  KIXMAIL_DEBUG_MIME = 1 << 6,
  KIXMAIL_DEBUG_DBUS = 1 << 7,
  KIXMAIL_DEBUG_WINDOW = 1 << 8,
  KIXMAIL_DEBUG_SEARCH = 1 << 9,
  KIXMAIL_DEBUG_APPLICATION = 1 << 10,
} KixmailDebugFlags;

void kixmail_debug_set_flags (KixmailDebugFlags flags);
gblloean kixmail_debug_flag_is_set (KixmailDebugFlags flags);

void kixmail_debug (KixmailDebugFlags flag, const gchar *format, ...)
  G_GNUC_PRINTF (2, 3);

G_END_DECLS

#endif /* _KIXMAIL_DEBUG_H_ */

/*
 *
 */

#ifdef DEBUG_FLAG
#ifdef ENABLE_DEBUG

#undef DEBUG
#define DEBUG(format, ...) \
  kixmail_debug (DEBUG_FLAG, "%s: " format, G_STRFUNC, ##__VA_ARGS__)

#else /* !(ENABLE_DEBUG) */

#undef DEBUG
#define DEBUG(format, ...) do {} while(0)

#endif /* !(ENABLE_DEBUG)*/
#endif /* DEBUG_FLAG */
