/*
 * kixmail-initial-setup-widget.h
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


#ifndef _KIXMAIL_INITIAL_SETUP_WIDGET_H_
#define _KIXMAIL_INITIAL_SETUP_WIDGET_H_

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define KIXMAIL_TYPE_INITIAL_SETUP_WIDGET         (kixmail_initial_setup_widget_get_type ())
#define KIXMAIL_INITIAL_SETUP_WIDGET(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), KIXMAIL_TYPE_INITIAL_SETUP_WIDGET, KixmailInitialSetupWidget))
#define KIXMAIL_INITIAL_SETUP_WIDGET_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k), KIXMAIL_TYPE_INITIAL_SETUP_WIDGET, KixmailInitialSetupWidgetClass))
#define KIXMAIL_IS_INITIAL_SETUP_WIDGET(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), KIXMAIL_TYPE_INITIAL_SETUP_WIDGET))
#define KIXMAIL_IS_INITIAL_SETUP_WIDGET_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), KIXMAIL_TYPE_INITIAL_SETUP_WIDGET))
#define KIXMAIL_INITIAL_SETUP_WIDGET_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), KIXMAIL_TYPE_INITIAL_SETUP_WIDGET, KixmailInitialSetupWidgetClass))

typedef struct _KixmailInitialSetupWidget KixmailInitialSetupWidget;
typedef struct _KixmailInitialSetupWidgetClass KixmailInitialSetupWidgetClass;
typedef struct _KixmailInitialSetupWidgetPriv KixmailInitialSetupWidgetPriv;

struct _KixmailInitialSetupWidgetClass {
  GtkGridClass parent_class;
};

struct _KixmailInitialSetupWidget {
  GtkGrid parent;

  KixmailInitialSetupWidgetPriv *priv;
};

GType kixmail_initial_setup_widget_get_type (void) G_GNUC_CONST;

GtkWidget * kixmail_initial_setup_widget_new (void);

gboolean kixmail_initial_setup_widget_should_create_account (void);

G_END_DECLS

#endif /* _KIXMAIL_INITIAL_SETUP_WIDGET_H_ */
