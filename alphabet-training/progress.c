#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <err.h>
#include <math.h>
#include <string.h>

#include "progress.h"
#include "utils/verbose.h"

struct ProgressBarData
{
    bool running;

    unsigned short width;

    /**
     * Max bar value
     */
    unsigned long max;

    /**
     * Current value
     */
    unsigned long current;

    /**
     * Time to wait between each refresh
     * @note In miliseconds
     */
    unsigned long refresh_rate;

    pthread_t progress_thread;
};

struct ProgressBarData *gData = NULL;

unsigned short __get_terminal_width()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    return w.ws_col;
}

float __remap(float old_min, float old_max, float new_min, float new_max, float value)
{
    return new_min + (value - old_min) * (new_max - new_min) / (old_max - old_min);
}

bool pb_init(unsigned long max, unsigned long refresh_rate)
{
    gData = malloc(sizeof(struct ProgressBarData));

    if (gData == NULL)
    {
        return false;
    }

    gData->max = max;

    gData->refresh_rate = refresh_rate != 0 ? refresh_rate : 100;

    gData->width = __get_terminal_width();

    verbose_printf("terminal width: %hu", gData->width);

    gData->progress_thread = -1;

    gData->running = false;

    gData->current = 0;

    return true;
}

char *__get_closest_block(float value)
{
    if (value <= 1/8)
    {
        return " ";
    }
    if (value <= 2/8)
    {
        return "▏";
    }
    else if (value <= 3/8)
    {
        return "▎";
    }
    else if (value <= 4/8)
    {
        return "▍";
    }
    else if (value <= 5/8)
    {
        return "▌";
    }
    else if (value <= 6/8)
    {
        return "▋";
    }
    else if (value <= 7/8)
    {
        return "▊";
    }
    else
    {
        return "▉";
    }
}

void __get_bar(float width, unsigned short total_width, char *out)
{
    unsigned long full_block_width = floorf(width);
    unsigned long block_width = ceilf(width);
    unsigned long space_width = total_width - block_width;

    out[0] = 0;

    for (unsigned long i = 0; i < full_block_width; i++)
    {
        strcat(out, "█");
    }

    float res;    
    strcat(out, __get_closest_block(modff(width, &res)));

    for (unsigned long i = 0; i < space_width; i++)
    {
        strcat(out, " ");
    }
}

void *__update_bar(void *none)
{
    unsigned long sleep_time = gData->refresh_rate * 1000;
    unsigned short progress_width = gData->width - 40;
    char *out = calloc(1000, sizeof(char));

    verbose_printf("progress width: %hu", progress_width);

    printf("\e[?25l");

    while (gData->running)
    {
        float width = __remap(0, gData->max, 0, progress_width, gData->current);
        __get_bar(width, progress_width, out);
        printf("\r");
        printf("%6lu/%lu [%s]", gData->current, gData->max, out);

        fflush(stdout);
        usleep(sleep_time);
    }

    printf("\e[?25h");
    free(out);
    return NULL;
}

bool pb_start()
{
    if (gData == NULL)
    {
        errx(1, "progress bar not initialized");
    }

    if (gData->running == true)
    {
        return false;
    }

    gData->running = true;
    pthread_create(&(gData->progress_thread), NULL, __update_bar, NULL);
}

void pb_update_current(unsigned long value)
{
    if (gData == NULL)
    {
        return;
    }

    if (gData->running == false)
    {
        return;
    }

    gData->current = value;
}

bool pb_stop()
{
    if (gData == NULL)
    {
        errx(1, "progress bar not initialized");
    }

    if (gData->running == false)
    {
        return false;
    }

    gData->running = false;
    pthread_join(gData->progress_thread, NULL);

    return true;
}

void pb_destroy()
{
    if (gData == NULL)
    {
        return;
    }

    if (gData->running == true)
    {
        pb_stop();
    }

    free(gData);
}
