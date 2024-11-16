#pragma once

#include <stdbool.h>

typedef void (*DataPrinter)(void *data, char *out); 

bool pb_init(unsigned long max, unsigned long refresh_rate);

bool pb_start();

void pb_set_data_print_function(DataPrinter dp);

void pb_update_current(unsigned long value, void *data);

bool pb_stop();

void pb_destroy();