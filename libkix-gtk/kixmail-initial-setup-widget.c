/*
 * kixmail-initial-setup-widget.c
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

#include "kixmail-initial-setup-widget.h"

#include <glib/gi18n-lib.h>

#define DEBUG_FLAG KIXMAIL_DEBUG_ACCOUNT
#include <libkix/kixmail-debug.h>


enum {
  SIG_VALID = 1,
  LAST_SIGNAL
};

static gulong signals[LAST_SIGNAL] = { 0, };

#define KIXMAIL_INITIAL_SETUP_WIDGET_GET_PRIVATE(object)                \
  (G_TYPE_INSTANCE_GET_PRIVATE ((object),                               \
                                KIXMAIL_TYPE_INITIAL_SETUP_WIDGET,      \
                                KixmailInitialSetupWidgetPriv))

G_DEFINE_TYPE (KixmailInitialSetupWidget, kixmail_initial_setup_widget, GTK_TYPE_GRID)

struct _KixmailInitialSetupWidgetPriv {
  gchar *settings;
  /*account settings here*/
};


/* Internal functions */

/* Class implementation */
static void
kixmail_initial_setup_widget_constructed (GObject *object)
{
  KixmailInitialSetupWidget *self = KIXMAIL_INITIAL_SETUP_WIDGET (object);
  GtkWidget *widget;
//  GdkPixbuf *pix;
  gchar *markup;

  G_OBJECT_CLASS (kixmail_initial_setup_widget_parent_class)->constructed (object);

  gtk_container_set_border_width (GTK_CONTAINER (self), 12);

  widget = gtk_label_new ("Welcome to kixMail");

  gtk_misc_set_alignment (GTK_MISC (widget), 0, 0.5);
  gtk_label_set_line_wrap (GTK_LABEL (widget), TRUE);
  gtk_grid_attach (GTK_GRID (self), widget, 0, 0, 1, 1);
  gtk_widget_show (widget);

  widget = gtk_label_new (NULL);
  markup = g_strdup_printf ("<span size=\"small\">%s</span>",
                            _("You can modify or delete this account later"
                              "by choosing <span style=\"italic\">Edit → Accounts</span>."));
  gtk_label_set_markup (GTK_LABEL (widget), markup);
  g_free (markup);
  gtk_misc_set_alignment (GTK_MISC (widget), 0, 0.5);
  gtk_label_set_line_wrap (GTK_LABEL (widget), TRUE);
  gtk_grid_attach (GTK_GRID (self), widget, 0, 2, 2, 1);
  gtk_widget_show (widget);
}

static void
kixmail_initial_setup_widget_dispose (GObject *object)
{
  G_OBJECT_CLASS (kixmail_initial_setup_widget_parent_class)->dispose (object);
}

static void
kixmail_initial_setup_widget_class_init (KixmailInitialSetupWidgetClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->constructed = kixmail_initial_setup_widget_constructed;
  object_class->dispose = kixmail_initial_setup_widget_dispose;

  signals[SIG_VALID] =
      g_signal_new ("valid",
          G_TYPE_FROM_CLASS (klass),
          G_SIGNAL_RUN_LAST, 0, NULL, NULL,
          g_cclosure_marshal_generic,
          G_TYPE_NONE, 1, G_TYPE_BOOLEAN);

  g_type_class_add_private (object_class, sizeof(KixmailInitialSetupWidgetPriv));
}

static void
kixmail_initial_setup_widget_init (KixmailInitialSetupWidget *self)
{
  self->priv = KIXMAIL_INITIAL_SETUP_WIDGET_GET_PRIVATE (self);
}

/* Public methods */
GtkWidget *
kixmail_initial_setup_widget_new (void)
{
  return g_object_new (KIXMAIL_TYPE_INITIAL_SETUP_WIDGET,
                       "row-spacing", 12, NULL);
}

gboolean
kixmail_initial_setup_widget_should_create_account (void)
{
  /* Do a check here to see if there is no account created.
     Prompt initial setup dialog only if there is no account.
   */

  return TRUE;
}
