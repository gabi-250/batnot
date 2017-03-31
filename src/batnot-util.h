#ifndef __BATNOT_UTIL_H__
#define __BATNOT_UTIL_H__

#define BUFF_SIZE 128

typedef struct _BatteryInfo {
	guint state;
	gdouble percentage;
	gint64 time_to_empty;
	gint64 time_to_full;
} BatteryInfo;

BatteryInfo* batnot_device_info      (gpointer device);
BatteryInfo* batnot_battery_info_new (void);

#endif
