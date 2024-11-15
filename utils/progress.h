#pragma once

#include <stdbool.h>

bool pb_init(unsigned long max, unsigned long refresh_rate);

bool pb_start();

void pb_update_current(unsigned long value);

bool pb_stop();

void pb_destroy();