/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * kixmail.c
 * Copyright (C) Parthasarathi Susarla 2012 <ajaysusarla@gmail.com>
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
#include "kixmail.h"

#include <glib/gi18n.h>



/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/kixmail/ui/kixmail.ui" */
#define UI_FILE "src/kixmail.ui"
#define TOP_WINDOW "window"


G_DEFINE_TYPE (Kixmail, kixmail, GTK_TYPE_APPLICATION);

/* Create a new window loading a file */
static void
kixmail_new_window (GApplication *app,
                           GFile        *file)
{
	GtkWidget *window;

	GtkBuilder *builder;
	GError* error = NULL;

	/* Load UI from file */
	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, UI_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (builder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (builder, TOP_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				TOP_WINDOW,
				UI_FILE);
        }
	g_object_unref (builder);
	
	
	gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));
	if (file != NULL)
	{
		/* TODO: Add code here to open the file in the new window */
	}
	gtk_widget_show_all (GTK_WIDGET (window));
}


/* GApplication implementation */
static void
kixmail_activate (GApplication *application)
{
  kixmail_new_window (application, NULL);
}

static void
kixmail_open (GApplication  *application,
                     GFile        **files,
                     gint           n_files,
                     const gchar   *hint)
{
  gint i;

  for (i = 0; i < n_files; i++)
    kixmail_new_window (application, files[i]);
}

static void
kixmail_init (Kixmail *object)
{

}

static void
kixmail_finalize (GObject *object)
{

	G_OBJECT_CLASS (kixmail_parent_class)->finalize (object);
}

static void
kixmail_class_init (KixmailClass *klass)
{
	G_APPLICATION_CLASS (klass)->activate = kixmail_activate;
	G_APPLICATION_CLASS (klass)->open = kixmail_open;

	G_OBJECT_CLASS (klass)->finalize = kixmail_finalize;
}

Kixmail *
kixmail_new (void)
{
	g_type_init ();

	return g_object_new (kixmail_get_type (),
	                     "application-id", "org.gnome.kixmail",
	                     "flags", G_APPLICATION_HANDLES_OPEN,
	                     NULL);
}
