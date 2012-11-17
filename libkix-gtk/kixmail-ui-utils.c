/*
 * kixmail-ui-utils.c
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
 * Most parts of this file are copied from empathy (empathy-ui-utils.h).
 */

#include "config.h"

#include <string.h>
#include <X11/Xatom.h>
#include <gdk/gdkx.h>
#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <gio/gdesktopappinfo.h>

#include <folks/folks.h>

#include "kixmail-ui-utils.h"

#define DEBUG_FLAG KIXMAIL_DEBUG_OTHER
#include <libkix/kixmail-debug.h>
#include <libkix/kixmail-utils.h>

void
kixmail_gtk_init (void)
{
  static gboolean initialized = FALSE;

  if (initialized)
    return;

  kixmail_init ();

  gtk_icon_theme_append_search_path (gtk_icon_theme_get_default (),
      PKGDATADIR G_DIR_SEPARATOR_S "icons");

  /* Add icons from source dir if available */
  if (g_getenv ("KIXMAIL_SRCDIR") != NULL)
    {
      gchar *path;

      path = g_build_filename (g_getenv ("KIXMAIL_SRCDIR"), "data",
          "icons", "local-copy", NULL);

      if (g_file_test (path, G_FILE_TEST_EXISTS))
        gtk_icon_theme_append_search_path (gtk_icon_theme_get_default (), path);

      g_free (path);
    }

  initialized = TRUE;
}


static GtkBuilder *
builder_get_file_valist (const gchar *filename,
                         const gchar *first_object,
                         va_list args)
{
  GtkBuilder *gui;
  const gchar *name;
  GObject **object_ptr;
  GError *error = NULL;

  DEBUG ("Loading file %s", filename);

  gui = gtk_builder_new ();
  gtk_builder_set_translation_domain (gui, GETTEXT_PACKAGE);

  if (!gtk_builder_add_from_file (gui, filename, &error))
    {
      g_critical ("GtkBuilder Error (%s): %s",
          filename, error->message);

      g_clear_error (&error);
      g_object_unref (gui);

      /* we need to iterate and set all of the pointers to NULL */
      for (name = first_object; name; name = va_arg (args, const gchar *))
        {
          object_ptr = va_arg (args, GObject**);

          *object_ptr = NULL;
        }

      return NULL;
    }

  for (name = first_object; name; name = va_arg (args, const gchar *))
    {
      object_ptr = va_arg (args, GObject**);

      *object_ptr = gtk_builder_get_object (gui, name);

      if (!*object_ptr)
        {
          g_warning ("File is missing object '%s'.", name);
          continue;
        }
    }

  return gui;
}

GtkBuilder *
kixmail_builder_get_file (const gchar *filename,
                          const gchar *first_object,
                          ...)
{
  GtkBuilder *gui;
  va_list args;

  va_start (args, first_object);
  gui = builder_get_file_valist (filename, first_object, args);
  va_end (args);

  return gui;
}

void
kixmail_builder_connect (GtkBuilder *gui,
                         gpointer user_data,
                         const gchar *first_object,
                         ...)
{
  va_list args;
  const gchar *name;
  const gchar *sig;
  GObject *object;
  GCallback callback;

  va_start (args, first_object);
  for (name = first_object; name; name = va_arg (args, const gchar *))
    {
      sig = va_arg (args, const gchar *);
      callback = va_arg (args, GCallback);

      object = gtk_builder_get_object (gui, name);
      if (!object)
        {
          g_warning ("File is missing object '%s'.", name);
          continue;
        }

      g_signal_connect (object, sig, callback, user_data);
    }

  va_end (args);
}

GtkWidget *
kixmail_builder_unref_and_keep_widget (GtkBuilder *gui,
                                       GtkWidget *widget)
{
  /* On construction gui sinks the initial reference to widget. When gui
   * is finalized it will drop its ref to widget. We take our own ref to
   * prevent widget being finalised. The widget is forced to have a
   * floating reference, like when it was initially unowned so that it can
   * be used like any other GtkWidget. */

  g_object_ref (widget);
  g_object_force_floating (G_OBJECT (widget));
  g_object_unref (gui);

  return widget;
}

/* Takes care of moving the window to the current workspace. */
void
kixmail_window_present_with_time (GtkWindow *window,
    guint32 timestamp)
{
  GdkWindow *gdk_window;

  g_return_if_fail (GTK_IS_WINDOW (window));

  /* Move the window to the current workspace before trying to show it.
   * This is the behaviour people expect when clicking on the statusbar icon. */
  gdk_window = gtk_widget_get_window (GTK_WIDGET (window));

  if (gdk_window)
    {
      gint x, y;
      gint w, h;

      /* Has no effect if the WM has viewports, like compiz */
      gdk_x11_window_move_to_current_desktop (gdk_window);

      /* If window is still off-screen, hide it to force it to
       * reposition on the current workspace. */
      gtk_window_get_position (window, &x, &y);
      gtk_window_get_size (window, &w, &h);
      if (!KIXMAIL_RECT_IS_ON_SCREEN (x, y, w, h))
        gtk_widget_hide (GTK_WIDGET (window));
    }

  if (timestamp == GDK_CURRENT_TIME)
    gtk_window_present (window);
  else
    gtk_window_present_with_time (window, timestamp);
}

void
kixmail_window_present (GtkWindow *window)
{
  kixmail_window_present_with_time (window, gtk_get_current_event_time ());
}

GtkWindow *
kixmail_get_toplevel_window (GtkWidget *widget)
{
  GtkWidget *toplevel;

  g_return_val_if_fail (GTK_IS_WIDGET (widget), NULL);

  toplevel = gtk_widget_get_toplevel (widget);
  if (GTK_IS_WINDOW (toplevel) &&
      gtk_widget_is_toplevel (toplevel))
    return GTK_WINDOW (toplevel);

  return NULL;
}

static void
menu_deactivate_cb (GtkMenu *menu,
  gpointer user_data)
{
  /* FIXME: we shouldn't have to disconnect the signal (bgo #641327) */
  g_signal_handlers_disconnect_by_func (menu,
         menu_deactivate_cb, user_data);

  gtk_menu_detach (menu);
}

/* Convenient function to create a GtkMenu attached to @attach_to and detach
 * it when the menu is not displayed any more. This is useful when creating a
 * context menu that we want to get rid as soon as it as been displayed. */
GtkWidget *
kixmail_context_menu_new (GtkWidget *attach_to)
{
  GtkWidget *menu;

  menu = gtk_menu_new ();

  gtk_menu_attach_to_widget (GTK_MENU (menu), attach_to, NULL);

  /* menu is initially unowned but gtk_menu_attach_to_widget () taked its
   * floating ref. We can either wait that @attach_to releases its ref when
   * it will be destroyed (when leaving Kixmail most of the time) or explicitely
   * detach the menu when it's not displayed any more.
   * We go for the latter as we don't want to keep useless menus in memory
   * during the whole lifetime of Kixmail. */
  g_signal_connect (menu, "deactivate", G_CALLBACK (menu_deactivate_cb), NULL);

  return menu;
}

void
kixmail_launch_program (const gchar *dir,
    const gchar *name,
    const gchar *args)
{
  GdkDisplay *display;
  GError *error = NULL;
  gchar *path, *cmd;
  GAppInfo *app_info;
  GdkAppLaunchContext *context = NULL;

  /* Try to run from source directory if possible */
  path = g_build_filename (g_getenv ("KIXMAIL_SRCDIR"), "src",
      name, NULL);

  if (!g_file_test (path, G_FILE_TEST_EXISTS))
    {
      g_free (path);
      path = g_build_filename (dir, name, NULL);
    }

  if (args != NULL)
    cmd = g_strconcat (path, " ", args, NULL);
  else
    cmd = g_strdup (path);

  app_info = g_app_info_create_from_commandline (cmd, NULL, 0, &error);
  if (app_info == NULL)
    {
      DEBUG ("Failed to create app info: %s", error->message);
      g_error_free (error);
      goto out;
    }

  display = gdk_display_get_default ();
  context = gdk_display_get_app_launch_context (display);

  if (!g_app_info_launch (app_info, NULL, (GAppLaunchContext *) context,
      &error))
    {
      g_warning ("Failed to launch %s: %s", name, error->message);
      g_error_free (error);
      goto out;
    }

out:
  g_object_unref (&app_info);
  app_info = NULL;
  g_object_unref (&context);
  context = NULL;
  g_free (path);
  g_free (cmd);
}


/* Most of the workspace manipulation code has been copied from libwnck
 * Copyright (C) 2001 Havoc Pennington
 * Copyright (C) 2005-2007 Vincent Untz
 */
static void
_wnck_activate_workspace (Screen *screen,
                          int new_active_space,
                          Time timestamp)
{
  Display *display;
  Window root;
  XEvent xev;

  display = DisplayOfScreen (screen);
  root = RootWindowOfScreen (screen);

  xev.xclient.type = ClientMessage;
  xev.xclient.serial = 0;
  xev.xclient.send_event = True;
  xev.xclient.display = display;
  xev.xclient.window = root;
  xev.xclient.message_type = gdk_x11_get_xatom_by_name ("_NET_CURRENT_DESKTOP");
  xev.xclient.format = 32;
  xev.xclient.data.l[0] = new_active_space;
  xev.xclient.data.l[1] = timestamp;
  xev.xclient.data.l[2] = 0;
  xev.xclient.data.l[3] = 0;
  xev.xclient.data.l[4] = 0;

  gdk_error_trap_push ();
  XSendEvent (display, root, False,
      SubstructureRedirectMask | SubstructureNotifyMask,
      &xev);
  XSync (display, False);
  gdk_error_trap_pop_ignored ();
}

static gboolean
_wnck_get_cardinal (Screen *screen,
                    Window xwindow,
                    Atom atom,
                    int *val)
{
  Display *display;
  Atom type;
  int format;
  gulong nitems;
  gulong bytes_after;
  gulong *num;
  int err, result;

  display = DisplayOfScreen (screen);

  *val = 0;

  gdk_error_trap_push ();
  type = None;
  result = XGetWindowProperty (display, xwindow, atom,
      0, G_MAXLONG, False, XA_CARDINAL, &type, &format, &nitems,
      &bytes_after, (void *) &num);
  err = gdk_error_trap_pop ();
  if (err != Success ||
      result != Success)
    return FALSE;

  if (type != XA_CARDINAL)
    {
      XFree (num);
      return FALSE;
    }

  *val = *num;

  XFree (num);

  return TRUE;
}

static int
window_get_workspace (Screen *xscreen,
                      Window win)
{
  int number;

  if (!_wnck_get_cardinal (xscreen, win,
        gdk_x11_get_xatom_by_name ("_NET_WM_DESKTOP"), &number))
    return -1;

  return number;
}

/* Ask X to move to the desktop on which @window currently is
 * and the present @window. */
void
kixmail_move_to_window_desktop (GtkWindow *window,
                                guint32 timestamp)
{
  GdkScreen *screen;
  Screen *xscreen;
  GdkWindow *gdk_window;
  int workspace;

  screen = gtk_window_get_screen (window);
  xscreen = gdk_x11_screen_get_xscreen (screen);
  gdk_window = gtk_widget_get_window (GTK_WIDGET (window));

  workspace = window_get_workspace (xscreen,
      gdk_x11_window_get_xid (gdk_window));
  if (workspace == -1)
    goto out;

  _wnck_activate_workspace (xscreen, workspace, timestamp);

out:
  gtk_window_present_with_time (window, timestamp);
}

void
kixmail_set_css_provider (GtkWidget *widget)
{
  GtkCssProvider *provider;
  gchar *filename;
  GError *error = NULL;
  GdkScreen *screen;

  filename = kixmail_file_lookup ("kixmail.css", "data");

  provider = gtk_css_provider_new ();

  if (!gtk_css_provider_load_from_path (provider, filename, &error))
    {
      g_warning ("Failed to load css file '%s': %s", filename, error->message);
      g_error_free (error);
      goto out;
    }

  if (widget != NULL)
    screen = gtk_widget_get_screen (widget);
  else
    screen = gdk_screen_get_default ();

  gtk_style_context_add_provider_for_screen (screen,
      GTK_STYLE_PROVIDER (provider),
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

out:
  g_free (filename);
  g_object_unref (provider);
}

static gboolean
launch_app_info (GAppInfo *app_info,
    GError **error)
{
  GdkAppLaunchContext *context = NULL;
  GdkDisplay *display;
  GError *err = NULL;

  display = gdk_display_get_default ();
  context = gdk_display_get_app_launch_context (display);

  if (!g_app_info_launch (app_info, NULL, (GAppLaunchContext *) context,
        &err))
    {
      DEBUG ("Failed to launch %s: %s",
          g_app_info_get_display_name (app_info), err->message);
      g_propagate_error (error, err);
      return FALSE;
    }

  g_object_unref (&context);
  context = NULL;
  return TRUE;
}

gboolean
kixmail_launch_external_app (const gchar *desktop_file,
    const gchar *args,
    GError **error)
{
  GDesktopAppInfo *desktop_info;
  gboolean result;
  GError *err = NULL;

  desktop_info = g_desktop_app_info_new (desktop_file);
  if (desktop_info == NULL)
    {
      DEBUG ("%s not found", desktop_file);

      g_set_error (error, G_IO_ERROR, G_IO_ERROR_NOT_FOUND,
          "%s not found", desktop_file);
      return FALSE;
    }

  if (args == NULL)
    {
      result = launch_app_info (G_APP_INFO (desktop_info), error);
    }
  else
    {
      gchar *cmd;
      GAppInfo *app_info;

      /* glib doesn't have API to start a desktop file with args... (#637875) */
      cmd = g_strdup_printf ("%s %s", g_app_info_get_commandline (
            (GAppInfo *) desktop_info), args);

      app_info = g_app_info_create_from_commandline (cmd, NULL, 0, &err);
      if (app_info == NULL)
        {
          DEBUG ("Failed to launch '%s': %s", cmd, err->message);
          g_free (cmd);
          g_object_unref (desktop_info);
          g_propagate_error (error, err);
          return FALSE;
        }

      result = launch_app_info (app_info, error);

      g_object_unref (app_info);
      g_free (cmd);
    }

  g_object_unref (desktop_info);
  return result;
}
