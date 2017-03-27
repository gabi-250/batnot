#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "batnot-util.h"

char*
batnot_acpi_path() {
	char* output;
	FILE *fp;

	fp = popen ("/bin/which acpi", "r");
	if (fp == NULL) {
		printf("Could not run \'which\'\n");
		exit(1);
	}

	output = (char *) malloc (BUFF_SIZE);
	while (fgets(output, BUFF_SIZE - 1, fp) != NULL) {
		// do nothing
	}
	return output;
}

char*
batnot_acpi_output() {
	char* output;
	FILE *fp;
	char* command;

	command = batnot_acpi_path();
	*(command + strlen(command) -1) = '\0';
	strcat (command, " -b");
	fp = popen (command, "r");
	if (fp == NULL) {
		printf("Could not run \'acpi\'\n");
		free (command);
		exit(1);
	}

	output = (char *) malloc (BUFF_SIZE);
	while (fgets(output, BUFF_SIZE - 1, fp) != NULL) {

	}
	return output;
}

int
batnot_charging() {
	char *acpi_output = NULL;
	int charging = 0;
	acpi_output = batnot_acpi_output ();
	charging = strstr (acpi_output, "Charging") != NULL;
	free (acpi_output);
	return charging;
}

BatteryInfo*
batnot_battery_info_new() {
	BatteryInfo *info = malloc(sizeof(BatteryInfo));
	info->charging = batnot_charging ();
	info->battery_level = batnot_battery_level ();
	return info;
}

int
batnot_battery_level() {
	char *acpi_output;
	char *percentage;
	int tens = 1;
	int bat_level = 0;

	acpi_output = batnot_acpi_output ();
	percentage = strchr (acpi_output, '%');

	if (percentage == NULL) {
		free (acpi_output);
		return -1;
	}
	--percentage;
	while (percentage - acpi_output >= 0 && isdigit ((unsigned char) *percentage)) {
		bat_level = bat_level + ((unsigned char) *percentage - '0') * tens;
		--percentage;
		tens *= 10;
	}

	free (acpi_output);
	return bat_level;
}
