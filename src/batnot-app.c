#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <libupower-glib/upower.h>
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
notify_state_change(void *application) {
	GApplication *app = NULL;
	gchar *message = NULL;
	BatteryInfo *info = NULL;
	GNotificationPriority priority = G_NOTIFICATION_PRIORITY_NORMAL;
	int was_charging;

	info = batnot_battery_info_new();
	was_charging = (info->state == UP_DEVICE_STATE_CHARGING);
	app = (GApplication *) application;
	while (TRUE) {
		if (!was_charging && info->state == UP_DEVICE_STATE_CHARGING) {
			message = g_strdup_printf ("[CHARGING]: %d%%",
					           (int)info->percentage);
			send_notification (app, message, priority);
		} else if (was_charging && info->state == UP_DEVICE_STATE_DISCHARGING) {
			message = g_strdup_printf ("[DISCHARGING]: %d%%",
						   (int)info->percentage);
			send_notification (app, message, priority);
		}
		was_charging = (info->state == UP_DEVICE_STATE_CHARGING);
		if (info != NULL) {
			free(info);
		}
		info = batnot_battery_info_new ();
		sleep(0.2);
	}
	pthread_exit (NULL);
}

void*
notify_low_battery(void *application) {
	GApplication *app = NULL;
	gchar *message = NULL;
	BatteryInfo *info = NULL;
	GNotificationPriority priority = G_NOTIFICATION_PRIORITY_NORMAL;
	int battery_level;

	info = batnot_battery_info_new();
	battery_level = (int)info->percentage;
	app = (GApplication *) application;
	while (TRUE) {
		if (info->state == UP_DEVICE_STATE_DISCHARGING) {
			sleep(1);
			if (battery_level < 20) {
				message = g_strdup_printf ("[LOW BATTERY]: %d%%",
							   battery_level);
				priority = G_NOTIFICATION_PRIORITY_LOW;
				send_notification (app, message, priority);
				sleep (300);
			} else if (battery_level < 5) {
				message = g_strdup_printf ("[CRITICALLY LOW BATTERY]: %d%%",
							   battery_level);
				priority = G_NOTIFICATION_PRIORITY_URGENT;
				send_notification (app, message, priority);
				sleep (60);
			}
		} else if (info->state == UP_DEVICE_STATE_CHARGING) {
			sleep (300);
		}
		battery_level = (int)info->percentage;
		free (info);
		info = batnot_battery_info_new ();
	}
	pthread_exit (NULL);
}

static void
batnot_app_activate (GApplication* app)
{
	pthread_t warning_thread;
	pthread_t state_change_thread;
	int rc;

	rc = pthread_create (&warning_thread, NULL, notify_low_battery, (void*) app);
	rc = rc || pthread_create (&state_change_thread, NULL, notify_state_change, (void*) app);
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
