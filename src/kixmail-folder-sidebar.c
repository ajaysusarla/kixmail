/*
 * kixmail-folder-sidebar.h
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

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <gio/gio.h>

#define DEBUG_FLAG KIXMAIL_DEBUG_OTHER
#include <libkix/kixmail-debug.h>

#include "kixmail-folder-sidebar.h"
#include "kixmail-application.h"

enum {
  KIXMAIL_FOLDER_SIDEBAR_COLUMN_ROW_TYPE,
  KIXMAIL_FOLDER_SIDEBAR_COLUMN_NAME,
  KIXMAIL_FOLDER_SIDEBAR_COLUMN_URI,
  KIXMAIL_FOLDER_SIDEBAR_COLUMN_GICON,
  KIXMAIL_FOLDER_SIDEBAR_COLUMN_TOOLTIP,
  KIXMAIL_FOLDER_SIDEBAR_COLUMN_SECTION_TYPE,
  KIXMAIL_FOLDER_SIDEBAR_COLUMN_HEADING_TEXT,

  KIXMAIL_FOLDER_SIDEBAR_COLUMN_COUNT
};

enum {
  KIXMAIL_FOLDER_SIDEBAR_FOLDER_ALLOWS_CHILDREN = 1 << 0,
  KIXMAIL_FOLDER_SIDEBAR_FOLDER_CAN_DELETE = 1 << 1,
  KIXMAIL_FOLDER_SIDEBAR_FOLDER_IS_SPAM = 1 << 2,
  KIXMAIL_FOLDER_SIDEBAR_FOLDER_IS_OUTBOX = 1 << 3,
  KIXMAIL_FOLDER_SIDEBAR_FOLDER_IS_STORE = 1 << 4,
  KIXMAIL_FOLDER_SIDEBAR_FOLDER_IS_BUILTIN = 1 << 5,
  KIXMAIL_FOLDER_SIDEBAR_FOLDER_IS_SUBSCRIBABLE = 1 << 6,
};

/* Drag & Drop types  - From Evolution*/
enum DndDragType {
        DND_DRAG_TYPE_FOLDER,          /* drag an evo folder */
        DND_DRAG_TYPE_TEXT_URI_LIST,   /* drag to an mbox file */
        NUM_DRAG_TYPES
};

enum DndDropType {
        DND_DROP_TYPE_UID_LIST,        /* drop a list of message uids */
        DND_DROP_TYPE_FOLDER,          /* drop an evo folder */
        DND_DROP_TYPE_MESSAGE_RFC822,  /* drop a message/rfc822 stream */
        DND_DROP_TYPE_TEXT_URI_LIST,   /* drop an mbox file */
        NUM_DROP_TYPES
};

static const GtkTargetEntry drag_types[] = {
        { "x-folder",         0, DND_DRAG_TYPE_FOLDER         },
        { "text/uri-list",    0, DND_DRAG_TYPE_TEXT_URI_LIST  },
};

static const GtkTargetEntry drop_types[] = {
        { "x-uid-list" ,      0, DND_DROP_TYPE_UID_LIST       },
        { "x-folder",         0, DND_DROP_TYPE_FOLDER         },
        { "message/rfc822",   0, DND_DROP_TYPE_MESSAGE_RFC822 },
        { "text/uri-list",    0, DND_DROP_TYPE_TEXT_URI_LIST  },
};

/*
static GdkAtom drag_atoms[NUM_DRAG_TYPES];
static GdkAtom drop_atoms[NUM_DROP_TYPES];
*/

#define KIXMAIL_ICON_SIZE_SMALLER	24

#define KIXMAIL_FOLDER_SIDEBAR_GET_PRIVATE(object)                      \
  (G_TYPE_INSTANCE_GET_PRIVATE ((object),                               \
                                KIXMAIL_TYPE_FOLDER_SIDEBAR,            \
                                KixmailFolderSidebarPriv))

G_DEFINE_TYPE (KixmailFolderSidebar, kixmail_folder_sidebar, GTK_TYPE_SCROLLED_WINDOW);

struct _KixmailFolderSidebarPriv {
  GtkTreeView *tree_view;
  GtkCellRenderer *icon_cell_renderer;
  char *uri;
  GtkTreeStore *store;
  KixmailWindow *window;

  /* DnD */
  GList     *drag_list;
	gboolean  drag_data_received;
	int       drag_data_info;
	gboolean  drop_occured;

  GtkWidget *popup_menu;
  GtkWidget *empty_trash_item;
  GtkWidget *popup_menu_properties_item;
};

/* Drag and drop interface declarations */
typedef struct {
  GtkTreeStore parent;

  KixmailFolderSidebar *sidebar;
} KixmailShortcutsModel;

typedef struct {
  GtkTreeStoreClass parent_class;
} KixmailShortcutsModelClass;

GType _kixmail_shortcuts_model_get_type (void);
static GtkTreeStore *kixmail_shortcuts_model_new (KixmailFolderSidebar *sidebar);
static void _kixmail_shortcuts_model_drag_source_init (GtkTreeDragSourceIface *iface);
G_DEFINE_TYPE_WITH_CODE (KixmailShortcutsModel, _kixmail_shortcuts_model, GTK_TYPE_TREE_STORE,
                         G_IMPLEMENT_INTERFACE (GTK_TYPE_TREE_DRAG_SOURCE,
                                                _kixmail_shortcuts_model_drag_source_init));

/* Internal methods */
static void
update_folder_list (KixmailFolderSidebar *sidebar)
{
  DEBUG ("update_folder_list");
}

static void
kixmail_folder_sidebar_set_parent_window (KixmailFolderSidebar *sidebar,
                                          KixmailWindow *window)
{
  KixmailFolderSidebarPriv *priv = KIXMAIL_FOLDER_SIDEBAR_GET_PRIVATE (sidebar);

  priv->window = window;
}

static void
heading_cell_renderer_func (GtkTreeViewColumn *column,
                            GtkCellRenderer *cell,
                            GtkTreeModel *model,
                            GtkTreeIter *iter,
                            gpointer user_data)
{
	gint type;

	gtk_tree_model_get (model, iter,
                      KIXMAIL_FOLDER_SIDEBAR_COLUMN_ROW_TYPE, &type,
                      -1);

	if (type == KIXMAIL_FOLDER_SIDEBAR_FOLDER_IS_STORE) {
		g_object_set (cell,
                  "visible", TRUE,
                  NULL);
	} else {
		g_object_set (cell,
                  "visible", FALSE,
                  NULL);
	}
}

static void
padding_cell_renderer_func (GtkTreeViewColumn *column,
                            GtkCellRenderer *cell,
                            GtkTreeModel *model,
                            GtkTreeIter *iter,
                            gpointer user_data)
{
	gint type;

	gtk_tree_model_get (model, iter,
                      KIXMAIL_FOLDER_SIDEBAR_COLUMN_ROW_TYPE, &type,
                      -1);

	if (type == KIXMAIL_FOLDER_SIDEBAR_FOLDER_IS_STORE) {
		g_object_set (cell,
                  "visible", FALSE,
                  "xpad", 0,
                  "ypad", 0,
                  NULL);
	} else {
		g_object_set (cell,
                  "visible", TRUE,
                  "xpad", 3,
                  "ypad", 3,
                  NULL);
	}
}

static void
icon_cell_renderer_func (GtkTreeViewColumn *column,
                         GtkCellRenderer *cell,
                         GtkTreeModel *model,
                         GtkTreeIter *iter,
                         gpointer user_data)
{
	gint type;

	gtk_tree_model_get (model, iter,
                      KIXMAIL_FOLDER_SIDEBAR_COLUMN_ROW_TYPE, &type,
                      -1);

	if (type == KIXMAIL_FOLDER_SIDEBAR_FOLDER_IS_STORE) {
		g_object_set (cell,
                  "visible", FALSE,
                  NULL);
	} else {
		g_object_set (cell,
                  "visible", TRUE,
                  NULL);
	}
}

static gint
places_sidebar_sort_func (GtkTreeModel *model,
			  GtkTreeIter *iter_a,
			  GtkTreeIter *iter_b,
			  gpointer user_data)
{
  gint retval = 0;

  return retval;
}

static void
resize_event_cb (GtkContainer *container,
                  gpointer      user_data)
{
  g_print ("Resize event called\n");
}

/* Class implementation */
static void
kixmail_folder_sidebar_dispose (GObject *object)
{
  G_OBJECT_CLASS (kixmail_folder_sidebar_parent_class)->dispose (object);
}

static void
kixmail_folder_sidebar_style_set (GtkWidget *widget, GtkStyle *previous_style)
{
  KixmailFolderSidebar *sidebar;

  sidebar = KIXMAIL_FOLDER_SIDEBAR (widget);

  update_folder_list (KIXMAIL_FOLDER_SIDEBAR (sidebar));
}

static gboolean
kixmail_folder_sidebar_focus (GtkWidget *widget, GtkDirectionType direction)
{
  return GTK_WIDGET_CLASS (kixmail_folder_sidebar_parent_class)->focus (widget, direction);
}

static void
kixmail_folder_sidebar_class_init (KixmailFolderSidebarClass *class)
{
  G_OBJECT_CLASS (class)->dispose = kixmail_folder_sidebar_dispose;

  GTK_WIDGET_CLASS (class)->style_set = kixmail_folder_sidebar_style_set;
  GTK_WIDGET_CLASS (class)->focus = kixmail_folder_sidebar_focus;

  g_type_class_add_private (class, sizeof (KixmailFolderSidebarPriv));
}

static void
kixmail_folder_sidebar_init (KixmailFolderSidebar *sidebar)
{
  GtkTreeView *tree_view;
  GtkTreeViewColumn *col;
  GtkCellRenderer *cell;
  GtkTreeSelection *selection;
  //  GIcon *icon;
  KixmailFolderSidebarPriv *priv = KIXMAIL_FOLDER_SIDEBAR_GET_PRIVATE (sidebar);


	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sidebar),
                                  GTK_POLICY_NEVER,
                                  GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_hadjustment (GTK_SCROLLED_WINDOW (sidebar), NULL);
	gtk_scrolled_window_set_vadjustment (GTK_SCROLLED_WINDOW (sidebar), NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sidebar), GTK_SHADOW_IN);

	gtk_style_context_set_junction_sides (gtk_widget_get_style_context (GTK_WIDGET (sidebar)),
                                        GTK_JUNCTION_RIGHT | GTK_JUNCTION_LEFT);

  /* tree view */
	tree_view = GTK_TREE_VIEW (gtk_tree_view_new ());
	gtk_tree_view_set_headers_visible (tree_view, FALSE);

	col = GTK_TREE_VIEW_COLUMN (gtk_tree_view_column_new ());

	/* initial padding */
	cell = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, cell, FALSE);
	g_object_set (cell,
                "xpad", 6,
                NULL);

	/* headings */
	cell = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, cell, FALSE);
	gtk_tree_view_column_set_attributes (col, cell,
                                       "text",
                                       KIXMAIL_FOLDER_SIDEBAR_COLUMN_HEADING_TEXT,
                                       NULL);
	g_object_set (cell,
                "weight", PANGO_WEIGHT_BOLD,
                "weight-set", TRUE,
                "ypad", 6,
                "xpad", 0,
                NULL);

	gtk_tree_view_column_set_cell_data_func (col, cell,
                                           heading_cell_renderer_func,
                                           sidebar, NULL);

	/* icon padding */
	cell = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, cell, FALSE);
	gtk_tree_view_column_set_cell_data_func (col, cell,
                                           padding_cell_renderer_func,
                                           sidebar, NULL);

	/* icon renderer */
	cell = gtk_cell_renderer_pixbuf_new ();
	g_object_set (cell, "follow-state", TRUE, NULL);
	gtk_tree_view_column_pack_start (col, cell, FALSE);
	gtk_tree_view_column_set_attributes (col, cell,
                                       "gicon",
                                       KIXMAIL_FOLDER_SIDEBAR_COLUMN_GICON,
                                       NULL);
	gtk_tree_view_column_set_cell_data_func (col, cell,
                                           icon_cell_renderer_func,
                                           sidebar, NULL);

	/* normal text renderer */
	cell = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (col, cell, TRUE);
	g_object_set (G_OBJECT (cell), "editable", FALSE, NULL);
	gtk_tree_view_column_set_attributes (col, cell,
                                       "text",
                                       KIXMAIL_FOLDER_SIDEBAR_COLUMN_NAME,
                                       NULL);
	g_object_set (cell,
                "ellipsize", PANGO_ELLIPSIZE_END,
                "ellipsize-set", TRUE,
                NULL);


	gtk_tree_view_column_set_max_width (GTK_TREE_VIEW_COLUMN (col),
                                      KIXMAIL_ICON_SIZE_SMALLER);
	gtk_tree_view_append_column (tree_view, col);

	priv->store = kixmail_shortcuts_model_new (sidebar);
	gtk_tree_view_set_tooltip_column (tree_view,
                                    KIXMAIL_FOLDER_SIDEBAR_COLUMN_TOOLTIP);
	gtk_tree_sortable_set_sort_column_id (GTK_TREE_SORTABLE (priv->store),
                                        KIXMAIL_FOLDER_SIDEBAR_COLUMN_NAME,
                                        GTK_SORT_ASCENDING);
	gtk_tree_sortable_set_sort_func (GTK_TREE_SORTABLE (priv->store),
                                   KIXMAIL_FOLDER_SIDEBAR_COLUMN_NAME,
                                   places_sidebar_sort_func,
                                   sidebar, NULL);

	gtk_tree_view_set_model (tree_view, GTK_TREE_MODEL (priv->store));
	gtk_container_add (GTK_CONTAINER (sidebar), GTK_WIDGET (tree_view));
	gtk_widget_show (GTK_WIDGET (tree_view));
	gtk_tree_view_set_enable_search (tree_view, FALSE);

	gtk_widget_show (GTK_WIDGET (sidebar));
	priv->tree_view = tree_view;

	gtk_tree_view_set_search_column (tree_view, KIXMAIL_FOLDER_SIDEBAR_COLUMN_NAME);
	selection = gtk_tree_view_get_selection (tree_view);
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_BROWSE);

  g_signal_connect (tree_view, "size-allocate",
                    G_CALLBACK (resize_event_cb), sidebar);

  /*
	gtk_tree_selection_set_select_function (selection,
                                          tree_selection_func,
                                          sidebar,
                                          NULL);

	gtk_tree_view_enable_model_drag_source (GTK_TREE_VIEW (tree_view),
                                          GDK_BUTTON1_MASK,
                                          nautilus_shortcuts_source_targets,
                                          G_N_ELEMENTS (nautilus_shortcuts_source_targets),
                                          GDK_ACTION_MOVE);
	gtk_drag_dest_set (GTK_WIDGET (tree_view),
                     0,
                     nautilus_shortcuts_drop_targets, G_N_ELEMENTS (nautilus_shortcuts_drop_targets),
                     GDK_ACTION_MOVE | GDK_ACTION_COPY | GDK_ACTION_LINK);

	g_signal_connect (tree_view, "key-press-event",
                    G_CALLBACK (bookmarks_key_press_event_cb), sidebar);

	g_signal_connect (tree_view, "drag-motion",
                    G_CALLBACK (drag_motion_callback), sidebar);
	g_signal_connect (tree_view, "drag-leave",
                    G_CALLBACK (drag_leave_callback), sidebar);
	g_signal_connect (tree_view, "drag-data-received",
                    G_CALLBACK (drag_data_received_callback), sidebar);
	g_signal_connect (tree_view, "drag-drop",
                    G_CALLBACK (drag_drop_callback), sidebar);

	g_signal_connect (selection, "changed",
                    G_CALLBACK (bookmarks_selection_changed_cb), sidebar);
	g_signal_connect (tree_view, "popup-menu",
                    G_CALLBACK (bookmarks_popup_menu_cb), sidebar);
	g_signal_connect (tree_view, "button-release-event",
                    G_CALLBACK (bookmarks_button_release_event_cb), sidebar);

	eel_gtk_tree_view_set_activate_on_single_click (sidebar->tree_view,
                                                  TRUE);

	g_signal_connect_swapped (gnome_background_preferences, "changed::" NAUTILUS_PREFERENCES_SHOW_DESKTOP,
                            G_CALLBACK(desktop_setting_changed_callback),
                            sidebar);

	sidebar->hostname = g_strdup (_("Computer"));
	g_dbus_proxy_new_for_bus (G_BUS_TYPE_SYSTEM,
                            G_DBUS_PROXY_FLAGS_GET_INVALIDATED_PROPERTIES,
                            NULL,
                            "org.freedesktop.hostname1",
                            "/org/freedesktop/hostname1",
                            "org.freedesktop.hostname1",
                            NULL,
                            hostname_proxy_new_cb,
                            sidebar);

	g_signal_connect_object (nautilus_trash_monitor_get (),
                           "trash_state_changed",
                           G_CALLBACK (trash_state_changed_cb),
                           sidebar, 0);
  */

}

/* Public methods */
GtkWidget *
kixmail_folder_sidebar_new (KixmailWindow *window)
{
  KixmailFolderSidebar *sidebar;

  sidebar = g_object_new (KIXMAIL_TYPE_FOLDER_SIDEBAR, NULL);

  kixmail_folder_sidebar_set_parent_window (sidebar, window);

  return GTK_WIDGET (sidebar);
}


/* Drag and Drop interfaces */
static gboolean
kixmail_shortcuts_model_row_draggable (GtkTreeDragSource *drag_source,
                                       GtkTreePath *path)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
  guint row_type;

	model = GTK_TREE_MODEL (drag_source);

	gtk_tree_model_get_iter (model, &iter, path);
	gtk_tree_model_get (model, &iter,
			    KIXMAIL_FOLDER_SIDEBAR_COLUMN_ROW_TYPE, &row_type,
			    -1);

	if (row_type != KIXMAIL_FOLDER_SIDEBAR_FOLDER_IS_STORE ||
      row_type != KIXMAIL_FOLDER_SIDEBAR_FOLDER_IS_BUILTIN)
		return TRUE;

	return FALSE;
}

static void
_kixmail_shortcuts_model_class_init (KixmailShortcutsModelClass *class)
{
}

static void
_kixmail_shortcuts_model_init (KixmailShortcutsModel *model)
{
  model->sidebar = NULL;
}

static void
_kixmail_shortcuts_model_drag_source_init (GtkTreeDragSourceIface *iface)
{
  iface->row_draggable = kixmail_shortcuts_model_row_draggable;
}

static GtkTreeStore *
kixmail_shortcuts_model_new (KixmailFolderSidebar *sidebar)
{
  KixmailShortcutsModel *model;
  GType model_types [KIXMAIL_FOLDER_SIDEBAR_COLUMN_COUNT] = {
    G_TYPE_INT,
    G_TYPE_STRING,
    G_TYPE_STRING,
    G_TYPE_ICON,
    G_TYPE_STRING,
    G_TYPE_INT,
    G_TYPE_STRING,
  };

  model = g_object_new (_kixmail_shortcuts_model_get_type (), NULL);
  model->sidebar = sidebar;

  gtk_tree_store_set_column_types (GTK_TREE_STORE (model),
                                   KIXMAIL_FOLDER_SIDEBAR_COLUMN_COUNT,
                                   model_types);

  return GTK_TREE_STORE (model);
}
