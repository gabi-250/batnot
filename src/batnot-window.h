#ifndef __BATNOT_WIN_H__
#define __BATNOT_WIN_H__

#include <gtk/gtk.h>
#include "batnot-app.h"

G_BEGIN_DECLS

#define BATNOT_WINDOW_TYPE (batnot_window_get_type ())
#define BATNOT_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), BATNOT_WINDOW_TYPE, BatnotWindow))

typedef struct _BatnotWindow BatnotWindow;
typedef struct _BatnotWindowClass BatnotWindowClass;

struct _BatnotWindow
{
	GtkApplicationWindow parent;
};

struct _BatnotWindowClass
{
	GtkApplicationWindowClass parent_class;
};

GType batnot_window_get_type (void) G_GNUC_CONST;

static void          batnot_window_init(BatnotWindow *window);
static void          batnot_window_class_init(BatnotWindowClass *class);
       BatnotApp* batnot_window_new (BatnotApp *app);
       void          batnot_window_open (BatnotWindow *window,
					 GFile        *file);

G_END_DECLS

#endif
