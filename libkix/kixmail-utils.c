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

#include "config.h"

#include "kixmail-utils.h"

#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>

#include <glib/gi18n-lib.h>

#include <libxml/uri.h>

#include <folks/folks.h>

#include <dbus/dbus-protocol.h>

#define DEBUG_FLAG KIXMAIL_DEBUG_OTHER
#include "kixmail-debug.h"


void
kixmail_init (void)
{
  static gboolean initialized = FALSE;

  if (initialized)
    return;

  g_type_init ();

  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

  kixmail_debug_set_flags (g_getenv ("KIXMAIL_DEBUG"));

  initialized = TRUE;
}

gchar *
kixmail_file_lookup (const gchar *filename, const gchar *subdir)
{
  gchar *path;

  if (subdir == NULL)
    subdir = ".";

  path = g_build_filename (g_getenv ("KIXMAIL_SRCDIR"), subdir, filename, NULL);
  if (!g_file_test (path, G_FILE_TEST_EXISTS))
    {
      g_free (path);
      path = g_build_filename (DATADIR, "kixmail", filename, NULL);
    }

  return path;
}
