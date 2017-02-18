#include "batnot-app.h"
#include "batnot-window.h"

G_DEFINE_TYPE(BatnotApp, batnot_app, GTK_TYPE_APPLICATION);

static void
batnot_app_init (BatnotApp* app)
{

}

static void
batnot_app_activate (GApplication* app)
{
	BatnotWindow *window;

	window = batnot_window_new (BATNOT_APP (app));
	gtk_window_present (GTK_WINDOW (window));
}

static void
batnot_app_open (GApplication *app,
	 	 GFile       **files,
	 	 gint           n_files,
	 	 const gchar   *hint)
{
	GList *windows;
	BatnotApp *window;
	int i;

	windows = gtk_application_get_windows (GTK_APPLICATION (app));
	if (windows) {
		window = BATNOT_WINDOW (windows->data);
	} else {
		window = batnot_window_new (BATNOT_APP (app));
	}

	for (i = 0; i < n_files; ++i) {
		batnot_window_open (window, files[i]);
	}

	gtk_window_present (GTK_WINDOW (window));
}

static void
batnot_app_class_init (BatnotAppClass *class)
{
	G_APPLICATION_CLASS (class)->activate = batnot_app_activate;
	G_APPLICATION_CLASS (class)->open = batnot_app_open;
}


BatnotApp*
batnot_app_new(void)
{
	return g_object_new (BATNOT_APP_TYPE, "application-id", "org.gtk.batnot",
			     "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
