/*
 * kixmail-debug.c
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

#include "config.h"

#include <stdarg.h>
#include <glib.h>
#include <glib/gstdio.h>

#include "kixmail-debug.h"

#ifdef ENABLE_DEBUG

static KixmailDebugFlags flags = 0;
static gboolean initialized = FALSE;

static GDebugKey keys[] = {
  { "Account", KIXMAIL_DEBUG_ACCOUNT },
  { "Other", KIXMAIL_DEBUG_OTHER },
  { "Connectivity", KIXMAIL_DEBUG_CONNECTIVITY},
  { "TLS", KIXMAIL_DEBUG_TLS },
  { "Tests", KIXMAIL_DEBUG_TESTS },
  { "Mime", KIXMAIL_DEBUG_MIME },
  { "DBus", KIXMAIL_DEBUG_DBUS },
  { "Window", KIXMAIL_DEBUG_WINDOW },
  { "Search", KIXMAIL_DEBUG_SEARCH },
  { "Application", KIXMAIL_DEBUG_APPLICATION },
  { 0, }
};

static void
kixmail_debug_set_flags_from_env (void)
{
  guint nkeys;
  const gchar *flags_string;

  for (nkeys = 0; keys[nkeys].value; nkeys++);

  flags_string = g_getenv ("KIXMAIL_DEBUG");

  if (flags_string)
    kixmail_debug_set_flags (g_parse_debug_string (flags_string, keys, nkeys));

  initialized = TRUE;
}

static void
kixmail_debug_valist (KixmailDebugFlags flag,
                      const gchar *format,
                      va_list args)
{
 if (G_UNLIKELY(!initialized))
    kixmail_debug_set_flags_from_env ();

  if (flag & flags)
    g_logv (G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, format, args);
}

void
kixmail_debug_set_flags (KixmailDebugFlags new_flags)
{
  flags |= new_flags;
  initialized = TRUE;
}

gboolean
kixmail_debug_flag_is_set (KixmailDebugFlags flag)
{
  return flag & flags;
}

void
kixmail_debug (KixmailDebugFlags flag,
               const gchar *format,
               ...)
{
  va_list args;

  va_start (args, format);
  kixmail_debug_valist (flag, format, args);
  va_end (args);
}

#else

void
kixmail_debug_set_flags (KixmailDebugFlags new_flags)
{
}

gboolean
kixmail_debug_flag_is_set (KixmailDebugFlags flag)
{
  return FALSE;
}

void
kixmail_debug (KixmailDebugFlags flag,
               const gchar *format,
               ...)
{
}

#endif /* ENABLE_DEBUG */
