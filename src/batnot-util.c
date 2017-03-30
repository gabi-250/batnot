#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libupower-glib/upower.h>
#include "batnot-util.h"

BatteryInfo *
batnot_device_info (gpointer device)
{
	BatteryInfo *info;
	guint kind;

	info = malloc (sizeof (BatteryInfo));
	g_object_get (G_OBJECT (device), "percentage", &info->percentage,
		"state", &info->state,
		"kind", &kind,
		"time-to-empty", &info->time_to_empty,
		"time-to-full", &info->time_to_full,
		NULL);
	if (kind == UP_DEVICE_KIND_BATTERY) {
		return info;
	}
	free(info);
	return NULL;
}

BatteryInfo *
batnot_battery_info_new ()
{
	UpClient *client;
	BatteryInfo *info;
	GPtrArray *devices = NULL;
	int i;

	client = up_client_new ();
	devices = up_client_get_devices (client);
	for (i = 0; i < devices->len; ++i) {
		info = batnot_device_info (g_ptr_array_index (devices, i));
		if (info != NULL) {
			g_ptr_array_free (devices, TRUE);
			g_object_unref (client);
			return info;
		}
	}
	g_ptr_array_free (devices, TRUE);
	g_object_unref (client);
	return NULL;
}

