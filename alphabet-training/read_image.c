#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "read_image.h"
// #include "../utils/array.h"
#include "../utils/verbose.h"

regex_t filename_regex;

uchar buf_avg(uchar buf[3])
{
    int sum = buf[0] + buf[1] + buf[2];
    return sum / 3;
}

int __load_image(FILE *file, uchar *out)
{
    if (fseek(file, 0x36, SEEK_SET))
    {
        verbose_printf("failed to seek with errno %d\n", errno);
        return 1;
    }

    uchar buf[3];

    for (int y = IMAGE_SIZE - 1; y >= 0; y--)
    {
        for (int x = 0; x < IMAGE_SIZE; x++)
        {
            if (!fread(buf, sizeof(uchar), 3, file))
            {
                verbose_printf("failed to read with errno %d\n", errno);
                return 1;
            }

            out[(y * IMAGE_SIZE) + x] = buf_avg(buf);
        }

        if (IMAGE_SIZE % 2 != 0)
        {
            fseek(file, 1, SEEK_CUR);
        }
    }

    return 0;
}

int load_image(char *path, InputImage *output)
{
    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        return 1;
    }

    int res = __load_image(file, output->image);

    size_t group_count = 5;
    regmatch_t groups[group_count];
    if (regexec(&filename_regex, output->name, group_count, groups, 0))
    {
        verbose_printf("invalid filename '%s'", output->name);
    }

    char type[255];

    strcpy(type, output->name + groups[2].rm_so);

    type[groups[2].rm_eo - groups[2].rm_so] = 0;

    verbose_printf("type of %s is %s\n", output->name, type);

    char character[255];
    strcpy(character, output->name + groups[4].rm_so);

    character[groups[4].rm_eo - groups[4].rm_so] = 0;

    verbose_printf("char of %s is %s\n", output->name, character);

    output->character = character[0];

    if (strcmp(type, "offcenter"))
    {
        output->category = IMG_OFFCENTER;
    }
    else if (strcmp(type, "rotated"))
    {
        output->category = IMG_ROTATED;
    }
    else if (strcmp(type, "plain"))
    {
        output->category = IMG_PLAIN;
    }

    fclose(file);

    return res;
}

InputImage *load_directory(char *path, size_t *count)
{
    struct stat stats;

    stat(path, &stats);

    if (!S_ISDIR(stats.st_mode))
    {
        errx(1, "path '%s' does not point to a directory", path);
    }

    size_t file_count = 0;
    DIR *dirp;
    struct dirent *entry;

    dirp = opendir(path);

    if (dirp == NULL)
    {
        errx(1, "could not open directory '%s'", path);
    }

    while ((entry = readdir(dirp)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            file_count++;
        }
    }

    verbose_printf("filecount: %d\n", file_count);

    InputImage *output = calloc(file_count, sizeof(InputImage));

    rewinddir(dirp);

    regcomp(
        &filename_regex, "([^_]*)_([^_]*)_([0-9]*)_([A-Za-z])\\.bmp",
        REG_EXTENDED
    );

    size_t i = 0;
    for (entry = readdir(dirp); entry != NULL && i < file_count;
         entry = readdir(dirp))
    {
        if (entry->d_type != DT_REG)
            continue;

        char f_path[255];

        strcpy(f_path, path);
        strcat(f_path, "/");
        strcat(f_path, entry->d_name);

        strcpy(output[i].name, entry->d_name);

        if (load_image(f_path, output + i))
        {
            verbose_printf("failed to load image %s\n", f_path);
            file_count--;
        }

        i++;
    }

    closedir(dirp);

    regfree(&filename_regex);

    *count = file_count;
    return output;
}