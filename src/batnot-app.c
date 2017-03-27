#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "batnot-app.h"
#include "batnot-util.h"

G_DEFINE_TYPE(BatnotApp, batnot_app, GTK_TYPE_APPLICATION);

static void
batnot_app_init (BatnotApp* app)
{

}

void
send_notification(GApplication *app, gchar *message, GNotificationPriority priority) {
	GNotification *battery_notif = NULL;
	battery_notif = g_notification_new (message);
	g_notification_set_priority (battery_notif, priority);
	g_application_send_notification (app, "low-battery", battery_notif);
	g_object_unref (battery_notif);
	g_free (message);
}

void*
notify_low_battery(void *application) {
	GApplication *app = NULL;
	gchar *message = NULL;
	BatteryInfo *info = NULL;
	GNotificationPriority priority = G_NOTIFICATION_PRIORITY_NORMAL;
	int battery_level = 0;
	int charging = 0;
	app = (GApplication *) application;
	while (TRUE) {
		battery_level = batnot_battery_level();
		charging = batnot_charging ();
		if (charging && (info == NULL || !info->charging)) {
			message = g_strdup_printf ("[CHARGING]: %d%%", battery_level);
			priority = G_NOTIFICATION_PRIORITY_LOW;
			send_notification(app, message, priority);
		}
		if (!charging && battery_level < 20) {
			message = g_strdup_printf ("[LOW BATTERY]: %d%%", battery_level);
			priority = G_NOTIFICATION_PRIORITY_LOW;
			send_notification(app, message, priority);
		} else if (!charging && battery_level < 5) {
			message = g_strdup_printf ("[CRITICALLY LOW BATTERY]: %d%%", battery_level);
			priority = G_NOTIFICATION_PRIORITY_URGENT;
			send_notification(app, message, priority);
		}
		if (info != NULL) {
			free(info);
		}
		info = batnot_battery_info_new();
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
