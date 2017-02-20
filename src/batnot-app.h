#ifndef __BATNOT_APP_H__
#define __BATNOT_APP_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define BATNOT_APP_TYPE (batnot_app_get_type ())
#define BATNOT_APP(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), BATNOT_APP_TYPE, BatnotApp))

typedef struct _BatnotApp BatnotApp;
typedef struct _BatnotAppClass BatnotAppClass;

struct _BatnotApp
{
	GtkApplication parent;
};

struct _BatnotAppClass
{
	GtkApplicationClass parent_class;
};


GType       batnot_app_get_type (void) G_GNUC_CONST;
BatnotApp*  batnot_app_new      (void);

G_END_DECLS

#endif
