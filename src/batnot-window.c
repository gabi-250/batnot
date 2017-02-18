#include "batnot-window.h"

G_DEFINE_TYPE (BatnotWindow, batnot_window, GTK_TYPE_APPLICATION_WINDOW);

static void
batnot_window_init(BatnotWindow *window)
{
	//gtk_widget_init_template (GTK_WIDGET (window));
}

static void
batnot_window_class_init(BatnotWindowClass *class)
{

}

BatnotApp*
batnot_window_new (BatnotApp *app)
{
	return g_object_new (BATNOT_WINDOW_TYPE, "application",
			     app, NULL);
}

void
batnot_window_open (BatnotWindow *window, GFile *file)
{

}
