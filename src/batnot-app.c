#include <pthread.h>
#include <stdlib.h>
#include "batnot-app.h"
#include "batnot-util.h"

G_DEFINE_TYPE(BatnotApp, batnot_app, GTK_TYPE_APPLICATION);

static void
batnot_app_init (BatnotApp* app)
{

}

void*
notify_low_battery(void *application) {
	GApplication *app = NULL;
	GNotification *battery_notif = NULL;
	gchar *message = NULL;
	GNotificationPriority priority = G_NOTIFICATION_PRIORITY_NORMAL;
	int battery_level = 0;
	app = (GApplication *) application;
	while (TRUE) {
		battery_level = batnot_battery_level();
		if (battery_level < 20) {
			message = g_strdup_printf ("[LOW BATTERY]: %d%%", battery_level);
			priority = G_NOTIFICATION_PRIORITY_LOW;

		} else if (battery_level < 5) {
			message = g_strdup_printf ("[CRITICALLY LOW BATTERY]: %d%%", battery_level);
			priority = G_NOTIFICATION_PRIORITY_URGENT;
		} else {
			pthread_exit(NULL);
		}
		battery_notif = g_notification_new (message);
		g_notification_set_priority (battery_notif, priority);
		g_application_send_notification (app, "low-battery", battery_notif);
		g_object_unref (battery_notif);
		g_free (message);
		sleep (300);
	}
	pthread_exit (NULL);
}


static void
batnot_app_activate (GApplication* app)
{
	pthread_t notif_thread;
	int rc;

	rc = pthread_create (&notif_thread, NULL, notify_low_battery, (void*) app);
	if (rc) {
		printf ("Error: could not start background thread.\n");
		exit (1);
	}
	gtk_main();

}

static void
batnot_app_class_init (BatnotAppClass *class)
{
	G_APPLICATION_CLASS (class)->activate = batnot_app_activate;
}


BatnotApp*
batnot_app_new(void)
{
	return g_object_new (BATNOT_APP_TYPE, "application-id", "org.gtk.batnot",
			     "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}
