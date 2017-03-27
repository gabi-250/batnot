#ifndef __BATNOT_UTIL_H__
#define __BATNOT_UTIL_H__

#define BUFF_SIZE 128

typedef struct BatteryInfo {
	int charging;
	int battery_level;
} BatteryInfo;

char*        batnot_acpi_path(void);
char*        batnot_acpi_output(void);
int          batnot_battery_level(void);
int          batnot_charging(void);
BatteryInfo* batnot_battery_info_new(void);

#endif
