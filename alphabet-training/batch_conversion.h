#pragma once

#include <stdlib.h>

#include "read_image.h"
#include "network/network_utils/batch.h"


Batch *images_to_batch(size_t image_count, InputImage *images);