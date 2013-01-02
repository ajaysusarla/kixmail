/*
 * kixmail-io.c
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
 */

#include <config.h>

#include "kixmail-io.h"

#define DEBUG_FLAG KIXMAIL_DEBUG_OTHER
#include <libkix/kixmail-debug.h>

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


#define KIXMAIL_IO_GET_PRIVATE(object)                  \
  (G_TYPE_INSTANCE_GET_PRIVATE ((object),               \
                                KIXMAIL_TYPE_IO,        \
                                KixmailIoPriv))

G_DEFINE_TYPE (KixmailIo, kixmail_io, G_TYPE_OBJECT);

struct _KixmailIoPriv
{
  GSocket *sock;
  GSocketAddress *address;

  int fd;
  char *host;
  char *port;
};

/* Internal Functions */

/* Class Implementation */
static void
kixmail_io_get_property (GObject *object,
                         guint prop_id,
                         GValue *value,
                         GParamSpec *pspec)
{
}

static void
kixmail_io_set_property (GObject *object,
                         guint prop_id,
                         const GValue *value,
                         GParamSpec *pspec)
{
}

static void
kixmail_io_finalize (GObject *object)
{
  G_OBJECT_CLASS(kixmail_io_parent_class)->finalize (object);
}

static void
kixmail_io_class_init (KixmailIoClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = kixmail_io_finalize;
  object_class->get_property = kixmail_io_get_property;
  object_class->set_property = kixmail_io_set_property;

  g_type_class_add_private (object_class, sizeof (KixmailIoPriv));
}

static void
kixmail_io_init (KixmailIo *self)
{
  self->priv = KIXMAIL_IO_GET_PRIVATE (self);
}

/* Public Methods */

KixmailIo *
kixmail_io_new (void)
{
  return g_object_new (KIXMAIL_TYPE_IO, NULL);
}

