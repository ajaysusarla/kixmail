/*
 * kixmail-main.c
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

#include <config.h>

#include "kixmail-application.h"
#include "libkix/kixmail-utils.h"

#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <gio/gdesktopappinfo.h>
#include <libnotify/notify.h>

#include <libxml/parser.h>

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main (int argc, char *argv[])
{
	KixmailApplication *app;
	gint status;

  kixmail_init ();
  gtk_init (&argc, &argv);

	g_desktop_app_info_set_desktop_env ("GNOME");

	g_set_prgname ("kixmail");

	app = 	g_object_new (kixmail_application_get_type (),
						  "application-id", "org.gnome.kixmail",
						  "flags", G_APPLICATION_HANDLES_OPEN,
						  NULL);

	status = g_application_run (G_APPLICATION (app), argc, argv);

  notify_uninit (); /* initialised in kixmail-application.c */

	g_object_unref (app);

	return status;
}
