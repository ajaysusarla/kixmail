/*
 * kixmail-window.c
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

#include <sys/stat.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gi18n.h>

#include <folks/folks.h>

#include "kixmail-window.h"
#include "kixmail-folder-sidebar.h"
#include "kixmail-actions.h"


#define DEBUG_FLAG KIXMAIL_DEBUG_WINDOW
#include <libkix/kixmail-debug.h>
#include <libkix/kixmail-utils.h>
#include <libkix-gtk/kixmail-ui-utils.h>


#define KIXMAIL_WINDOW_MIN_WIDTH	200
#define KIXMAIL_WINDOW_MIN_HEIGHT	200

#define KIXMAIL_WINDOW_DEFAULT_WIDTH		800
#define KIXMAIL_WINDOW_DEFAULT_HEIGHT		550

/* Accels (menu shortcuts) can be configured and saved */
#define ACCELS_FILENAME "accels.txt"

/* Name in the geometry file */
#define GEOMETRY_NAME "roster-window"

#define MENU_ITEM_MAX_WIDTH_CHARS 32

enum
{
  PROP_0,
  PROP_SHELL_RUNNING
};

#define KIXMAIL_WINDOW_GET_PRIVATE(object)                  \
  (G_TYPE_INSTANCE_GET_PRIVATE ((object),                   \
                                KIXMAIL_TYPE_WINDOW,        \
                                KixmailWindowPriv))

G_DEFINE_TYPE (KixmailWindow, kixmail_window, GTK_TYPE_APPLICATION_WINDOW)

struct _KixmailWindowPriv {

  guint flash_timeout_id;
  gboolean flash_on;

  GSettings *gsettings_ui;

  GtkWidget *preferences;
  GtkWidget *main_vbox;
  GtkWidget *throbber;
  GtkWidget *errors_vbox;
  GtkWidget *auth_vbox;
  GtkWidget *search_bar;
  GtkWidget *notebook;
  GtkWidget *button_account_settings;
  GtkWidget *button_online;
  GtkWidget *spinner_loading;
  GtkWidget *tooltip_widget;

  /*used*/
  GtkWidget *content_paned;

  /* Main View */
  GtkWidget *main_view;

  /* Side Pane */
  int side_pane_width;
  GtkWidget *sidebar;
  /*used*/

  GMenu *menumodel;

  GtkWidget *balance_vbox;

  guint size_timeout_id;

  gboolean shell_running;

  /*used*/
  guint sidebar_width_handler_id;
  /*used*/
};

/* Callbacks */


/* Internal functions */

/* Code should never force the window taller than this size.
 * (The user can still stretch the window taller if desired).
 */
static guint
get_max_forced_height (GdkScreen *screen)
{
	return (gdk_screen_get_height (screen) * 90) / 100;
}

/* Code should never force the window wider than this size.
 * (The user can still stretch the window wider if desired).
 */
static guint
get_max_forced_width (GdkScreen *screen)
{
	return (gdk_screen_get_width (screen) * 90) / 100;
}


static gboolean
save_sidebar_width_cb (gpointer user_data)
{
	KixmailWindow *window = user_data;
  KixmailWindowPriv *priv = KIXMAIL_WINDOW_GET_PRIVATE (window);

	priv->sidebar_width_handler_id = 0;

	DEBUG ("Saving sidebar width: %d", priv->side_pane_width);

  /*XXX : Save to gsettings*/

	return FALSE;
}

static void
side_pane_size_allocate_callback (GtkWidget *widget,
                                  GtkAllocation *allocation,
                                  gpointer user_data)
{
	KixmailWindow *window = user_data;
  KixmailWindowPriv *priv = KIXMAIL_WINDOW_GET_PRIVATE (window);

	if (priv->sidebar_width_handler_id != 0) {
		g_source_remove (priv->sidebar_width_handler_id);
		priv->sidebar_width_handler_id = 0;
	}

	if (allocation->width != priv->side_pane_width &&
	    allocation->width > 1) {
    priv->side_pane_width = allocation->width;

		priv->sidebar_width_handler_id =
			g_idle_add (save_sidebar_width_cb, window);
	}
}

static void
setup_side_pane_width (KixmailWindow *window)
{
  KixmailWindowPriv *priv = KIXMAIL_WINDOW_GET_PRIVATE (window);

  g_return_if_fail (priv->sidebar != NULL);

  /* XXX: Get width from gsettings*/

  priv->side_pane_width = 15; /* Temporary setting, remove with gsetting implenetation */

  gtk_paned_set_position (GTK_PANED (priv->content_paned),
                          priv->side_pane_width);
}

static void
kixmail_window_setup_sidebar (KixmailWindow *window)
{
  GtkWidget *sidebar;
  KixmailWindowPriv *priv = KIXMAIL_WINDOW_GET_PRIVATE (window);

	DEBUG ("kixmail_window_setup_sidebar");
  priv->sidebar = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);

	gtk_style_context_add_class (gtk_widget_get_style_context (priv->sidebar),
                               GTK_STYLE_CLASS_SIDEBAR);

	gtk_paned_pack1 (GTK_PANED (priv->content_paned),
                   GTK_WIDGET (priv->sidebar),
                   FALSE, FALSE);

  setup_side_pane_width (window);
	g_signal_connect (priv->sidebar,
                    "size_allocate",
                    G_CALLBACK (side_pane_size_allocate_callback),
                    window);

  sidebar = kixmail_folder_sidebar_new (window);
	gtk_box_pack_start (GTK_BOX (priv->sidebar), sidebar, TRUE, TRUE, 0);
	gtk_widget_show (sidebar);
	gtk_widget_show (GTK_WIDGET (priv->sidebar));
}

static void
kixmail_window_tear_down_sidebar (KixmailWindow *window)
{
  KixmailWindowPriv *priv = KIXMAIL_WINDOW_GET_PRIVATE (window);
	DEBUG ("kixmail_window_tear_down_sidebar");

	if (priv->sidebar != NULL) {
		gtk_widget_destroy (GTK_WIDGET (priv->sidebar));
		priv->sidebar = NULL;
	}
}

/* Class implementation */
static void
kixmail_window_set_property (GObject *object,
                             guint property_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
  KixmailWindow *self = KIXMAIL_WINDOW (object);

  switch (property_id)
    {
    case PROP_SHELL_RUNNING:
      self->priv->shell_running = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
kixmail_window_get_property (GObject    *object,
                             guint property_id,
                             GValue *value,
                             GParamSpec *pspec)
{
  KixmailWindow *self = KIXMAIL_WINDOW (object);

  switch (property_id)
    {
    case PROP_SHELL_RUNNING:
      g_value_set_boolean (value, self->priv->shell_running);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
kixmail_window_finalize (GObject *window)
{
  G_OBJECT_CLASS (kixmail_window_parent_class)->finalize (window);
}

static GObject *
kixmail_window_constructor (GType type,
                            guint n_construct_params,
                            GObjectConstructParam *construct_params)
{
  static GObject *window = NULL;

  printf ("kixmail_window_constructor\n");

  if (window != NULL)
    return g_object_ref (window);

  window = G_OBJECT_CLASS (kixmail_window_parent_class)->constructor (type,
                                                                      n_construct_params,
                                                                      construct_params);

  g_object_add_weak_pointer (window, (gpointer) &window);

  return window;
}

static void
kixmail_window_constructed (GObject *self)
{
  KixmailWindow *window;
  KixmailWindowPriv *priv;
	GtkWidget *grid;

  printf ("kixmail_window_constructed\n");

  window = KIXMAIL_WINDOW (self);
  priv = KIXMAIL_WINDOW_GET_PRIVATE (self);

  G_OBJECT_CLASS (kixmail_window_parent_class)->constructed (self);

	gtk_application_window_set_show_menubar (GTK_APPLICATION_WINDOW (self), FALSE);

	grid = gtk_grid_new ();
	gtk_orientable_set_orientation (GTK_ORIENTABLE (grid), GTK_ORIENTATION_VERTICAL);
	gtk_widget_show (grid);
	gtk_container_add (GTK_CONTAINER (window), grid);

	priv->content_paned = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_widget_set_hexpand (priv->content_paned, TRUE);
	gtk_widget_set_vexpand (priv->content_paned, TRUE);

	gtk_container_add (GTK_CONTAINER (grid), priv->content_paned);
	gtk_widget_show (priv->content_paned);

	priv->main_view = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_paned_pack2 (GTK_PANED (priv->content_paned),
                   priv->main_view,
                   TRUE, FALSE);
	gtk_widget_show (priv->main_view);

}

static void
kixmail_window_destroy (GtkWidget *object)
{
  DEBUG ("kixmail_window_destroy");

  GTK_WIDGET_CLASS (kixmail_window_parent_class)->destroy (object);
}

static void
kixmail_window_show (GtkWidget *widget)
{
  DEBUG ("kixmail_window_show");

  GTK_WIDGET_CLASS (kixmail_window_parent_class)->show (widget);
}

static void
kixmail_window_get_preferred_width (GtkWidget *widget,
                                    gint *minimal_width,
                                    gint *natural_width)
{
	GdkScreen *screen;
	gint max_w, min_w, default_w;

	screen = gtk_window_get_screen (GTK_WINDOW (widget));

	max_w = get_max_forced_width (screen);
	min_w = KIXMAIL_WINDOW_MIN_WIDTH;

	default_w = KIXMAIL_WINDOW_DEFAULT_WIDTH;

	*minimal_width = MIN (min_w, max_w);
	*natural_width = MIN (default_w, max_w);
}

static void
kixmail_window_get_preferred_height (GtkWidget *widget,
                                      gint *minimal_height,
                                      gint *natural_height)
{
	GdkScreen *screen;
	gint max_h, min_h, default_h;

	screen = gtk_window_get_screen (GTK_WINDOW (widget));

	max_h = get_max_forced_height (screen);

	min_h = KIXMAIL_WINDOW_MIN_HEIGHT;

	default_h = KIXMAIL_WINDOW_DEFAULT_HEIGHT;

	*minimal_height = MIN (min_h, max_h);
	*natural_height = MIN (default_h, max_h);
}

static void
kixmail_window_realize (GtkWidget *widget)
{
  DEBUG ("kixmail_window_realize");
	GTK_WIDGET_CLASS (kixmail_window_parent_class)->realize (widget);
}

static void
real_window_close (KixmailWindow *window)
{
  g_return_if_fail (KIXMAIL_IS_WINDOW (window));

  gtk_widget_destroy (GTK_WIDGET (window));
}

static void
kixmail_window_class_init (KixmailWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  GParamSpec *pspec;

  printf ("kixmail_window_class_init\n");

  object_class->finalize = kixmail_window_finalize;
  object_class->constructor = kixmail_window_constructor;
  object_class->constructed = kixmail_window_constructed;

  object_class->set_property = kixmail_window_set_property;
  object_class->get_property = kixmail_window_get_property;

  widget_class->destroy = kixmail_window_destroy;
  widget_class->show = kixmail_window_show;
  widget_class->get_preferred_width = kixmail_window_get_preferred_width;
  widget_class->get_preferred_height = kixmail_window_get_preferred_height;
  widget_class->realize =kixmail_window_realize;

  klass->close = real_window_close;

  pspec = g_param_spec_boolean ("shell-running",
                                "Shell running",
                                "Whether the Shel is running or not",
                                FALSE,
                                G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
  g_object_class_install_property (object_class, PROP_SHELL_RUNNING, pspec);

  g_type_class_add_private (object_class, sizeof (KixmailWindowPriv));
}

static void
kixmail_window_init (KixmailWindow *self)
{
  printf ("kixmail_window_init\n");

  gtk_window_set_title (GTK_WINDOW (self), _("Kixmail"));
  gtk_window_set_role (GTK_WINDOW (self), "kixmail_main_window");
  gtk_window_set_icon_name (GTK_WINDOW (self), "email-client");
	gtk_window_set_hide_titlebar_when_maximized (GTK_WINDOW (self), TRUE);
}

/* Public Methods */

GtkWidget *
kixmail_window_new (GtkApplication *application)
{
  printf ("kixmail_window_new\n");

  return g_object_new (KIXMAIL_TYPE_WINDOW,
                         "application", application,
                         NULL);
}

void
kixmail_window_hide_sidebar (KixmailWindow *window)
{
  KixmailWindowPriv *priv = KIXMAIL_WINDOW_GET_PRIVATE (window);

  DEBUG ("kixmail_window_hide_sidebar");

  if (priv->sidebar == NULL)
    return;

  kixmail_window_tear_down_sidebar (window);
}

void
kixmail_window_show_sidebar (KixmailWindow *window)
{
  KixmailWindowPriv *priv = KIXMAIL_WINDOW_GET_PRIVATE (window);

  DEBUG ("kixmail_window_show_sidebar");

  if  (priv->sidebar != NULL)
    return;

  kixmail_window_setup_sidebar (window);
}

GMenuModel *
kixmail_window_get_menu_model (KixmailWindow *window)
{
  g_return_val_if_fail (KIXMAIL_IS_WINDOW (window), NULL);

  return G_MENU_MODEL (window->priv->menumodel);
}
