/*
 * kixmail-application.c
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

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>
#include <gio/gio.h>
#include <libnotify/notify.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>


/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/kixmail/ui/kixmail.ui" */
#define UI_FILE "src/kixmail.ui"
#define TOP_WINDOW "window"

#define APPLICATION_WINDOW_MIN_WIDTH	300
#define APPLICATION_WINDOW_MIN_HEIGHT	100


#define KIXMAIL_APPLICATION_GET_PRIVATE(object)             \
  (G_TYPE_INSTANCE_GET_PRIVATE ((object),                   \
                                KIXMAIL_TYPE_APPLICATION,   \
                                KixmailApplicationPriv))

G_DEFINE_TYPE (KixmailApplication, kixmail_application, GTK_TYPE_APPLICATION);

struct _KixmailApplicationPriv {
  gboolean no_connect;
  gboolean start_hidden;
  gboolean show_preferences;

  gchar *geometry;
  gchar *preferences_tab;

};

/* callbacks */
static gboolean
preferences_cb (const char *option_name,
                const char *value,
                gpointer data,
                GError **error)
{
  KixmailApplication *self = data;
  KixmailApplicationPriv *priv = KIXMAIL_APPLICATION_GET_PRIVATE (self);

  priv->show_preferences = TRUE;

  g_free (priv->preferences_tab);
  priv->preferences_tab = g_strdup (value);

  return TRUE;
}

static gboolean
show_version_cb (const char *option_name,
                 const char *value,
                 gpointer data,
                 GError **error)
{
  g_print ("%s\n", PACKAGE_STRING);

  exit (EXIT_SUCCESS);
}

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
kixmail_application_quit_mainloop (GApplication *application)
{
  G_APPLICATION_CLASS(kixmail_application_parent_class)->quit_mainloop (application);
}

static int
kixmail_application_command_line (GApplication *application,
                                  GApplicationCommandLine *cmdline)
{

  kixmail_new_window (application, NULL);
  return 0;
}

static gboolean
kixmail_application_local_command_line (GApplication *application,
                                        gchar ***arguments,
                                        gint *exit_status)
{
  KixmailApplication *self = KIXMAIL_APPLICATION (application);
  KixmailApplicationPriv *priv = KIXMAIL_APPLICATION_GET_PRIVATE (self);
  gint i;
  gchar **argv;
  gint argc = 0;
  gboolean retval = FALSE;
  GError *error = NULL;
  gboolean no_connect = FALSE;
  gboolean start_hidden = FALSE;

  GOptionContext *optcontext;
  GOptionGroup *group;
  GOptionEntry options[] = {
      { "no-connect", 'n',
        0, G_OPTION_ARG_NONE, &no_connect,
        N_("Don't connect on startup"),
        NULL },
      { "start-hidden", 'h',
        0, G_OPTION_ARG_NONE, &start_hidden,
        N_("Don't display anything on startup"),
        NULL },
      { "show-preferences", 'p',
        G_OPTION_FLAG_OPTIONAL_ARG, G_OPTION_ARG_CALLBACK, &preferences_cb,
        NULL, NULL },
      { "version", 'v',
        G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, show_version_cb,
        NULL, NULL },
      { NULL }
  };

  group = g_option_group_new ("kixmail", NULL, NULL, application, NULL);
  g_option_group_set_translation_domain (group, GETTEXT_PACKAGE);
  g_option_group_add_entries (group, options);

  optcontext = g_option_context_new(N_("- Kixmail E-Mail Client"));
  g_option_context_add_group (optcontext, gtk_get_option_group (TRUE));
  g_option_context_set_main_group (optcontext, group);
  g_option_context_set_translation_domain (optcontext, GETTEXT_PACKAGE);

  argc = g_strv_length (*arguments);

  /* We dup the args because g_option_context_parse() sets things to NULL,
   * but we want to parse all the command line to the primary instance
   * if necessary. */
  argv = g_new (gchar*, argc + 1);
  for (i = 0; i <= argc; i++)
    argv[i] = (*arguments)[i];

  if (!g_option_context_parse (optcontext, &argc, &argv, &error))
    {
      g_print ("%s\nRun '%s --help' to see a full list of available command "
          "line options.\n",
          error->message, argv[0]);
      g_warning ("Error in empathy init: %s", error->message);

      *exit_status = EXIT_FAILURE;
      retval = TRUE;
    }

  g_free (argv);

  g_option_context_free (optcontext);

  priv->no_connect = no_connect;
  priv->start_hidden = start_hidden;

  return retval;
}

static void
kixmail_application_open (GApplication  *application,
                          GFile        **files,
                          gint           n_files,
                          const gchar   *hint)
{
  gint i;

  for (i = 0; i < n_files; i++)
    kixmail_new_window (application, files[i]);
}

static void
kixmail_application_init (KixmailApplication *application)
{
  application->priv = KIXMAIL_APPLICATION_GET_PRIVATE (application);
}

static void
kixmail_application_set_property (GObject *object,
                                  guint prop_id,
                                  const GValue *value,
                                  GParamSpec *pspec)
{
  KixmailApplication *self = KIXMAIL_APPLICATION (object);
  KixmailApplicationPriv *priv = KIXMAIL_APPLICATION_GET_PRIVATE (self);

  switch (prop_id)
    {
    case PROP_NO_CONNECT:
      priv->no_connect = g_value_get_boolean (value);
      break;
    case PROP_START_HIDDEN:
      priv->start_hidden = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
kixmail_application_constructed (GObject *object)
{
}

static void
kixmail_application_dispose (GObject *object)
{
}

static void
kixmail_application_finalize (GObject *object)
{

	G_OBJECT_CLASS (kixmail_application_parent_class)->finalize (object);
}

static void
kixmail_application_class_init (KixmailApplicationClass *class)
{
  GObjectClass *object_class;
  GApplicationClass *app_class;
  GParamSpec *spec;
  /*  GtkApplicationClass *gtkapp_class;*/

  object_class = G_OBJECT_CLASS (class);
  object_class->set_property = kixmail_application_set_property;
  object_class->constructed = kixmail_application_constructed;
  object_class->dispose = kixmail_application_dispose;
  object_class->finalize = kixmail_application_finalize;

  app_class = G_APPLICATION_CLASS (class);
  app_class->quit_mainloop  = kixmail_application_quit_mainloop;
	app_class->open = kixmail_application_open;
  app_class->command_line = kixmail_application_command_line;
  app_class->local_command_line = kixmail_application_local_command_line;

  spec = g_param_spec_boolean ("no-connect", "no connect",
                               "Don't connect on startup",
                               FALSE,
                               G_PARAM_STATIC_STRINGS | G_PARAM_WRITABLE |\
                               G_PARAM_CONSTRUCT_ONLY);
  g_object_class_install_property (object_class, PROP_NO_CONNECT, spec);

  spec = g_param_spec_boolean ("start-hidden", "start hidden",
                               "Don't display anything on startup",
                               FALSE,
                               G_PARAM_STATIC_STRINGS | G_PARAM_WRITABLE |\
                               G_PARAM_CONSTRUCT_ONLY);
  g_object_class_install_property (object_class, PROP_START_HIDDEN, spec);

  g_type_class_add_private (class, sizeof (KixmailApplicationPriv));

}

