#include "./randomize.h"

static int print_preallocated(cJSON *root) {
    char *out = NULL;
    char *buf = NULL;
    char *buf_fail = NULL;
    size_t len = 0;
    size_t len_fail = 0;

    out = cJSON_Print(root);

    len = strlen(out) + 5;
    buf = (char*)malloc(len);
    if (buf == NULL) {
        fprintf(stdout,"Failed to allocate memory.\n");
        exit(1);
    }

    len_fail = strlen(out);
    buf_fail = (char*)malloc(len_fail);
    if (buf_fail == NULL) {
        fprintf(stdout,"Failed to allocate memory.\n");
        exit(1);
    }

    if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
        fprintf(stdout,"cJSON_PrintPreallocated failed!\n");
        if (strcmp(out, buf) != 0) {
            fprintf(stdout,"cJSON_PrintPreallocated not the same as cJSON_Print!\n");
            fprintf(stdout,"cJSON_Print result:\n%s\n", out);
            fprintf(stdout,"cJSON_PrintPreallocated result:\n%s\n", buf);
        }
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    fprintf(stdout,"%s\n", buf);

    if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
        fprintf(stdout,"cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
        fprintf(stdout,"cJSON_Print result:\n%s\n", out);
        fprintf(stdout,"cJSON_PrintPreallocated result:\n%s\n", buf_fail);
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    free(out);
    free(buf_fail);
    free(buf);
    return 0;
}

void randomize(void) {
    cJSON *root = NULL;
    cJSON *osv = NULL;
    cJSON *point = NULL;
    cJSON *obstacles = NULL;
    cJSON *dest = NULL;
    int i, randomization = rand() % 6;
    float baseY, xMin, xMax, yMin, yMax, destX, destY;

    static const int presets[6][3] = {
        {0, 1, 2},
        {2, 1, 0},
        {0, 2, 1},
        {2, 0, 1},
        {1, 0, 2},
        {1, 2, 0}
    };


    //generate starting location
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "msg_type", cJSON_CreateString("randomization"));
    cJSON_AddItemToObject(root, "osv", osv = cJSON_CreateObject());
    cJSON_AddNumberToObject(osv, "x", 0.35);
    cJSON_AddNumberToObject(osv, "y", 0.4 + (rand() % 5)*0.3);
    cJSON_AddNumberToObject(osv, "theta", (rand() % 4) * PI/2 - PI);

    //generate obstacles
    obstacles = cJSON_CreateArray();
    for(int i = 0; i < 3; i++) {
        baseY = presets[randomization][i] * 0.65 + OBSTACLE_HEIGHT + 0.1;
        point = cJSON_CreateObject();
        cJSON_AddNumberToObject(point, "x", i * 0.55 + 1.5);
        cJSON_AddNumberToObject(point, "y", baseY);
        cJSON_AddItemToArray(obstacles, point);
    }
    cJSON_AddItemToObject(root, "obstacles", obstacles);

    //generate destination
    xMin = 2.8 + 0.4 + TARGET_DIAMETER / 2;
    xMax = 4 - OSV_WIDTH - 0.1 - TARGET_DIAMETER / 2;
    yMin = 0.4 + TARGET_DIAMETER / 2;
    yMax = 2 - 0.4 - TARGET_DIAMETER / 2;

    // we now have ranges
    destX = (rand() % 100) / 100.0 * (xMax - xMin) + xMin;
    destY = (rand() % 100) / 100.0 * (yMax - yMin) + yMin;
    dest = cJSON_CreateObject();
    cJSON_AddNumberToObject(dest, "x", destX);
    cJSON_AddNumberToObject(dest, "y", destY);
    cJSON_AddItemToObject(root, "destination", dest);

    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);


}
