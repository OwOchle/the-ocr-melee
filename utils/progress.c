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
#include <signal.h>

#include "progress.h"
#include "utils/verbose.h"
#include "colors.h"

char *animation[] = { "⣷", "⣯", "⣟", "⡿", "⢿", "⣻", "⣽", "⣾" };

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

    time_t last_update;
    unsigned long value_last_update;
    unsigned long itps;

    char loading_state;

    pthread_t progress_thread;
};

struct ProgressBarData *gData = NULL;

unsigned short __get_terminal_width()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    return w.ws_col;
}

void reshow_pointer()
{
    fprintf(stderr, "\n\e[1;33m/!\\ Interrupt: network will not be saved. Stopping /!\\\e[0m\n");
    pb_stop();
    printf("\e[?25h");
    exit(0);
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

    gData->loading_state = 0;

    return true;
}

char *__get_closest_block(float value)
{
    if (value <= 0.125)
    {
        return " ";
    } 
    else if (value <= 0.25)
    {
        return "▏";
    }
    else if (value <= 0.375)
    {
        return "▎";
    }
    else if (value <= 0.5)
    {
        return "▍";
    }
    else if (value <= 0.625)
    {
        return "▌";
    }
    else if (value <= 0.75)
    {
        return "▋";
    }
    else if (value <= 0.875)
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

void *__update_bar()
{
    unsigned long sleep_time = gData->refresh_rate * 1000;
    unsigned short progress_width = gData->width - 40;
    char *bar = calloc(1000, sizeof(char));

    verbose_printf("progress width: %hu", progress_width);

    while (gData->running)
    {
        time_t cur = time(NULL);

        if (cur > gData->last_update)
        {
            gData->last_update = cur;
            gData->itps = gData->current > gData->value_last_update ? gData->current - gData->value_last_update : 0;

            gData->value_last_update = gData->current;
        }

        float width = __remap(0, gData->max, 0, progress_width, gData->current);
        __get_bar(width, progress_width, bar);
        printf("\33[2K\r");
        printf("%s%6lu/%lu%s [%s] %s%4lu it/s %s%s%s", BLU, gData->current, gData->max, BRED, bar, YEL, gData->itps, GRN, animation[gData->loading_state], reset);

        fflush(stdout);
        gData->loading_state = (gData->loading_state + 1) % 8;
        usleep(sleep_time);
    }

    free(bar);
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

    signal(SIGINT, reshow_pointer);
    printf("\e[?25l");
    gData->current = 0;
    gData->running = true;
    pthread_create(&(gData->progress_thread), NULL, __update_bar, NULL);

    return true;
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

    printf("\e[?25h");
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
