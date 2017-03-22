#include <gtk/gtk.h>
#include "batnot-app.h"

int
main (int argc, char **argv)
{
	BatnotApp *app;
	int status;

	app = batnot_app_new ();
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
