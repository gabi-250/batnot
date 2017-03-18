#include "batnot-app.h"
#include "batnot-window.h"
#include "batnot-util.h"

#include <pthread.h>

G_DEFINE_TYPE(BatnotApp, batnot_app, GTK_TYPE_APPLICATION);

static void
batnot_app_init (BatnotApp* app)
{

}

static void
charge_laptop(GtkWidget *button, gpointer window) {
	GtkWidget *message_dialog;
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL;
	message_dialog = gtk_dialog_new_with_buttons ("Uh oh...", GTK_WINDOW (window),
						      flags, "Ok", GTK_RESPONSE_ACCEPT,
						      "Try again", GTK_RESPONSE_REJECT, NULL);
	GtkWidget *content_area = gtk_dialog_get_content_area (GTK_DIALOG (message_dialog));
	GtkWidget *message = gtk_label_new ("That is not how you charge a laptop!");
	gtk_container_set_border_width (GTK_CONTAINER (message_dialog), 20);
	gtk_box_set_spacing (GTK_BOX (content_area), 20);
	gtk_box_pack_start (GTK_BOX (content_area), message, TRUE, TRUE, 20);
	gtk_widget_show_all (message_dialog);
	gint result = gtk_dialog_run (GTK_DIALOG (message_dialog));
	if (result == GTK_RESPONSE_ACCEPT) {
		printf("OK\n");
	} else {
		printf("Try again\n");
	}
	gtk_widget_destroy (message_dialog);
}

static void
update_battery_level(GtkWidget *battery_level_bar, gpointer *data) {
	gdouble battery_level = (gdouble) batnot_battery_level();
	gtk_level_bar_set_value(GTK_LEVEL_BAR (battery_level_bar),
			        battery_level);
}

void*
notify_level_bar(void *battery_level_bar) {
	GtkWidget *level;

	level = GTK_WIDGET(battery_level_bar);
	while (TRUE) {
		g_signal_emit_by_name(level, "offset-changed");
		sleep(1);
	}
	pthread_exit(NULL);
}

static void
batnot_app_activate (GApplication* app)
{
	GtkWindow *window;
	GtkWidget *box;
	GtkWidget *battery_level;
	GtkWidget *charge_button;
	pthread_t thread;
	int rc;
	window = GTK_WINDOW (batnot_window_new (BATNOT_APP (GTK_APPLICATION (app))));
	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	battery_level = gtk_level_bar_new_for_interval (0.0, 100.0);
	charge_button = gtk_button_new_with_label ("Charge laptop");
	gtk_level_bar_set_value (GTK_LEVEL_BAR (battery_level),
				 (double) batnot_battery_level());
	gtk_container_set_border_width (GTK_CONTAINER(window), 30);
	gtk_container_add (GTK_CONTAINER (box), battery_level);
	gtk_container_add (GTK_CONTAINER (box), charge_button);
	gtk_container_add (GTK_CONTAINER (window), box);
	rc = pthread_create(&thread, NULL, notify_level_bar,
			    (void *)battery_level);
	if (rc) {
		printf("Error: could not start background thread.\n");
		exit(1);
	}
	g_signal_connect (charge_button, "clicked",
			  G_CALLBACK (charge_laptop), GTK_WINDOW (window));
	g_signal_connect(battery_level, "offset-changed",
			 G_CALLBACK (update_battery_level), NULL);
	gtk_widget_show_all (GTK_WIDGET (window));
	gtk_window_present (GTK_WINDOW (window));

}

static void
batnot_app_open (GApplication *app,
	 	 GFile       **files,
	 	 gint           n_files,
	 	 const gchar   *hint)
{
	GList *windows;
	BatnotWindow *window;
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
