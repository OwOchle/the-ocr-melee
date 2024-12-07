#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>

#define IMAGE_PROCESSING_EXECUTABLE "../image_processing/image_processing"

int current_step = 0;

void reset_steps()
{
    current_step = 0;
}

GdkPixbuf *get_next_image_step(char *filename) 
{
    char *step_name;
    switch (current_step)
    {
        case 0:
            step_name = "grayscale";
            break;
        
        case 1:
            step_name = "binary";
            break;
        
        case 2:
            step_name = "shapes";
            break;
        
        default:
            step_name = "sobel";
            break;
    }

    printf("Loading buf for type: %s", step_name);

    GdkPixbufLoader *loader = gdk_pixbuf_loader_new_with_type("png", NULL);

    size_t img_size = 0;
    char *image = malloc(sizeof(char) * 1048576);

    int image_pipe[2];

    if (pipe(image_pipe))
    {
        errx(1, "error opening pipe");
    }

    if (!fork())
    {
        close(image_pipe[0]);
        dup2(image_pipe[1], STDOUT_FILENO);

        char *arguments[5] = {IMAGE_PROCESSING_EXECUTABLE, filename, step_name, "-", NULL};

        execvp(IMAGE_PROCESSING_EXECUTABLE, arguments);
        exit(1);
    }
    else
    {
        close(image_pipe[1]);

        size_t r;
        while ((r = read(image_pipe[0], &image[img_size], 2048)) != 0)
        {
            img_size += r;
        }

        close(image_pipe[0]);
    }

    gdk_pixbuf_loader_write(loader, image, img_size, NULL);

    gdk_pixbuf_loader_close(loader, NULL);

    current_step++;
    free(image);

    return gdk_pixbuf_loader_get_pixbuf(loader);
}
