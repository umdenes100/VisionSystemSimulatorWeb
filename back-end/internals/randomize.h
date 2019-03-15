#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../libraries/cJSON.h"

#define PI 3.14159f
#define OBSTACLE_WIDTH 0.2f
#define OBSTACLE_HEIGHT 0.5f
#define TARGET_DIAMETER 0.18f
#define OSV_WIDTH 0.35f

static int print_preallocated(cJSON *root);
void randomize(void);

