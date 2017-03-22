#include "batnot-app.h"
#include "batnot-util.h"

#include <pthread.h>

G_DEFINE_TYPE(BatnotApp, batnot_app, GTK_TYPE_APPLICATION);

static void
batnot_app_init (BatnotApp* app)
{

}

void*
notify_low_battery(void *application) {
	GApplication *app;
	GNotification *battery_notif = NULL;
	char notification[256];
	GNotificationPriority priority;
	int battery_level;
	app = (GApplication *) application;
	while (TRUE) {
		battery_level = batnot_battery_level();
		if (battery_level < 20) {
			g_stpcpy(notification,
				 "[LOW BATTERY]: You should probably plug in your charger.");
			priority = G_NOTIFICATION_PRIORITY_HIGH;

		} else if (battery_level < 5) {
			g_stpcpy(notification, "[CRITICALLY LOW BATTERY]: Hurry.");
			priority = G_NOTIFICATION_PRIORITY_URGENT;
		} else {
			pthread_exit(NULL);
		}

		battery_notif = g_notification_new (notification);
		g_notification_set_priority (battery_notif, priority);
		g_application_send_notification (app, "low-battery", battery_notif);
		g_application_withdraw_notification (app, "low-battery");
		//g_clear_object (battery_notif);
		sleep(300);
	}
	pthread_exit (NULL);
}


static void
batnot_app_activate (GApplication* app)
{
	pthread_t notif_thread;
	int rc;

	rc = pthread_create(&notif_thread, NULL, notify_low_battery, (void*) app);
	if (rc) {
		printf("Error: could not start background thread.\n");
		exit(1);
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
