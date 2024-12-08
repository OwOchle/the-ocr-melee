#include <gtk/gtk.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "fs_utils.h"
#include "proc_steps.h"
#include "solve_it.h"
#include "word_search.h"

#ifdef FROM_STRING
extern char thing[];
extern int ui_size;
#endif

GtkWidget *mainWindow = NULL;
GtkImage *mainImage = NULL;

GtkWidget *solverWindow = NULL;
GtkImage *solverImage = NULL;

GtkWidget *settingsWindow = NULL;
GtkImage *settingsImage= NULL;

GtkWidget *rotationWindow = NULL;
GtkImage *rotationImage= NULL;

char* imageName = NULL;

char *temp_dir = NULL;

char *network_path = NULL;

GdkPixbuf *pixbuf = NULL;

int imageSize = 500;
GtkWidget *gridWindow = NULL;
GtkImage *gridImage = NULL;
GtkGrid *grid = NULL;
int gridCreated = 0;

void update_all_pixbufs();

bool check_image_existence()
{
    if (imageName == NULL)
    {
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(mainWindow),
                                  flags,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  "No image selected");
        gtk_dialog_run(GTK_DIALOG (dialog));
        gtk_widget_destroy(dialog);

        return false;
    }

    struct stat _;

    if (stat(imageName, &_))
    {
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(mainWindow),
                                  flags,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  "Error while reading the file");
        gtk_dialog_run(GTK_DIALOG (dialog));
        gtk_widget_destroy(dialog);

        return false;
    }

    return true;
}

void on_close() 
{
    printf("Cleaning garbage\n");
    if (temp_dir != NULL)
        remove_dir_recursive(temp_dir);
}


void updateImage()
{
    gtk_image_set_from_pixbuf(mainImage,pixbuf);
    gtk_image_set_from_pixbuf(solverImage,pixbuf);
    gtk_image_set_from_pixbuf(settingsImage,pixbuf);
    gtk_image_set_from_pixbuf(rotationImage,pixbuf);
    gtk_image_set_from_pixbuf(gridImage,pixbuf);
}

int main(int argc, char *argv[])
{
    char *tmp = malloc(sizeof("/tmp/tmpdir.XXXXXX"));;
    strcpy(tmp, "/tmp/tmpdir.XXXXXX");
    temp_dir = mkdtemp(tmp);

    printf("temp directory is %s\n", temp_dir);

    atexit(on_close);

    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();

    #ifndef FROM_STRING
    gtk_builder_add_from_file(builder, "main.glade", NULL);
    #else
    gtk_builder_add_from_string(builder, thing, ui_size, NULL);
    #endif

    mainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
    mainImage = GTK_IMAGE(gtk_builder_get_object(builder, "mainImage"));

    solverWindow = GTK_WIDGET(gtk_builder_get_object(builder, "solverWindow"));
    solverImage = GTK_IMAGE(gtk_builder_get_object(builder, "solverImage"));

    settingsWindow = GTK_WIDGET(gtk_builder_get_object(builder, "settingsWindow"));
    settingsImage = GTK_IMAGE(gtk_builder_get_object(builder, "settingsImage"));

    rotationWindow = GTK_WIDGET(gtk_builder_get_object(builder, "rotationWindow"));
    rotationImage = GTK_IMAGE(gtk_builder_get_object(builder, "rotationImage"));

    gridWindow = GTK_WIDGET(gtk_builder_get_object(builder, "gridWindow"));
    gridImage = GTK_IMAGE(gtk_builder_get_object(builder, "gridImage"));
    grid = GTK_GRID(gtk_builder_get_object(builder, "letterGrid"));

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show(mainWindow);
    gtk_widget_show_all(mainWindow);
    gtk_window_set_position((GtkWindow*)mainWindow,GTK_WIN_POS_CENTER);
    gtk_main();

    return 0;
}

void exit_app() // Quand on ferme la fenêtre
{
    printf("Exited app\n");
    gtk_widget_destroy(mainWindow);
    gtk_widget_destroy(solverWindow);
    gtk_widget_destroy(settingsWindow);
    gtk_widget_destroy(rotationWindow);
    
    gtk_main_quit();
}

void open_main() // Bouton qui ouvre le main menu
{
    printf("Main menu opened\n");
    gtk_widget_hide(solverWindow);
    gtk_widget_hide(rotationWindow);
    gtk_widget_hide(settingsWindow);
    // gtk_window_set_position((GtkWindow*)mainWindow,GTK_WIN_POS_CENTER);
    gtk_widget_show_all(mainWindow);
}

void open_solver() // Bouton qui ouvre le solver
{
    printf("Solver opened\n");
    gtk_widget_hide(mainWindow);
    gtk_widget_hide(gridWindow);
    // gtk_window_set_position((GtkWindow*)solverWindow,GTK_WIN_POS_CENTER);
    gtk_widget_show_all(solverWindow);
}

void open_settings() // Bouton qui ouvre les settings
{
    printf("Settings opened\n");
    gtk_widget_hide(mainWindow);
    // gtk_window_set_position((GtkWindow*)settingsWindow,GTK_WIN_POS_CENTER);
    gtk_widget_show_all(settingsWindow);
}

void open_rotation() // Bouton qui ouvre la fenêtre de rotation
{
    printf("Rotation window opened\n");
    gtk_widget_hide(mainWindow);
    // gtk_window_set_position((GtkWindow*)rotationWindow,GTK_WIN_POS_CENTER);
    gtk_widget_show_all(rotationWindow);
}

void open_gridCorrection() // Bouton qui ouvre la fenêtre de correction des lettres
{
    printf("Grid correction opened\n");
    gtk_widget_hide(solverWindow);
    // gtk_window_set_position((GtkWindow*)rotationWindow, GTK_WIN_POS_CENTER);
    gtk_widget_show_all(gridWindow);
}

void leftRotation() // Bouton de rotation gauche
{
    printf("Rotated left\n");
    if (pixbuf != NULL)
    {
        pixbuf = gdk_pixbuf_rotate_simple(pixbuf, GDK_PIXBUF_ROTATE_CLOCKWISE);
        updateImage();
    }
}

void rightRotation() // Bouton de rotation droit
{
    printf("Rotated right\n");
    if (pixbuf != NULL)
    {
        pixbuf = gdk_pixbuf_rotate_simple(pixbuf, GDK_PIXBUF_ROTATE_COUNTERCLOCKWISE);
        updateImage();
    }
}

void automaticRotation() // Bouton de rotation droit
{
    printf("Rotating...\n");
}

void allInOne_clicked() // Bouton qui solve d'un coup
{
    printf("Solving...\n");
    get_output_from_proc(temp_dir, imageName);

    if (gridCreated == 0)
    {
        gridCreated = 1;
        for (int i = 0; i < 5-1; i++)
        {
            gtk_grid_insert_row(grid, 0);
            gtk_grid_insert_column(grid, 0);
        }

        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                GtkWidget *entry = gtk_entry_new();
                char *letter = "a";
                gtk_entry_set_width_chars(GTK_ENTRY(entry), 2);
                gtk_entry_set_alignment(GTK_ENTRY(entry), 0.5);
                gtk_entry_set_text(GTK_ENTRY(entry), letter);
                gtk_entry_set_max_length(GTK_ENTRY(entry), 1);
                gtk_grid_attach(grid, entry, x,y,1,1);
            }
        }
    }
}

void rotButton_clicked() // Bouton qui rotate AUTO
{
    printf("Rotating...\n");
}

void update_all_pixbufs()
{
    int imageWidth = gdk_pixbuf_get_width(pixbuf);
    int imageHeight = gdk_pixbuf_get_height(pixbuf);
    float imageRatio = (float)imageWidth/(float)imageHeight;

    printf("image is %ux%u pixels\nRatio : %f\n", imageWidth, imageHeight, imageRatio);

    if (imageRatio == 1.0f){
        pixbuf = gdk_pixbuf_scale_simple(pixbuf, imageSize,imageSize,GDK_INTERP_BILINEAR);
        printf("new image is %ux%u pixels\n", imageSize, imageSize);
    }
    else{
        pixbuf = gdk_pixbuf_scale_simple(pixbuf, imageSize * imageRatio, imageSize,GDK_INTERP_BILINEAR);
        printf("new image is %ux%u pixels\n", (int)(imageSize * imageRatio), imageSize);
    }

    gtk_image_set_from_pixbuf(mainImage,pixbuf);
    gtk_image_set_from_pixbuf(solverImage,pixbuf);
    gtk_image_set_from_pixbuf(settingsImage,pixbuf);
    gtk_image_set_from_pixbuf(rotationImage,pixbuf);
    gtk_image_set_from_pixbuf(gridImage, pixbuf);
}

void on_imageImport(GtkFileChooserButton *file)
{
    reset_steps();
    imageName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file));
    printf("imported file name = %s\n", imageName);

    pixbuf = gdk_pixbuf_new_from_file(imageName, NULL);
    update_all_pixbufs();
}

void stepByStep_clicked() // Bouton qui solve en montrant chaque étape
{
    if (!check_image_existence())
    {
        return;
    }

    printf("------------------------------------------\n");
    printf("Solving:\nStep 1 : ...\nStep 2 : ...\nStep 3 : ...\n");
    printf("------------------------------------------\n");

    GdkPixbuf *out = get_next_image_step(imageName);

    if (out != NULL)
    {
        g_object_unref(pixbuf);
        pixbuf = out;
        update_all_pixbufs();
        return;
    }

    struct output result = get_output_from_proc(temp_dir, imageName);

    mixed_words_t *res = ocr_letters(temp_dir, network_path, result);

    if (gridCreated == 0)
    {
        gridCreated = 1;
        for (int i = 0; i < (result.width - 1); i++)
        {
            gtk_grid_insert_row(grid, 0);   
        }

        for (int i = 0; i < (result.height - 1); i++)
        {
            gtk_grid_insert_column(grid, 0);
        }

        for (int x = 0; x < result.width; x++)
        {
            for (int y = 0; y < result.height; y++)
            {
                GtkWidget *entry = gtk_entry_new(); 
                char *letter = calloc(2, sizeof(char));
                letter[0] = res->grid[(y * res->width) + x];
                gtk_entry_set_width_chars(GTK_ENTRY(entry), 2);
                gtk_entry_set_alignment(GTK_ENTRY(entry), 0.5);
                gtk_entry_set_text(GTK_ENTRY(entry), letter);
                gtk_entry_set_max_length(GTK_ENTRY(entry), 1);
                gtk_grid_attach(grid, entry, x,y,1,1);
            }
        }
    }

    char *name;
    asprintf(&name, "%s/grid", temp_dir);

    FILE *f = fopen(name, "w+");
    

    for (size_t y = 0; y < res->height; y++)
    {
        for (size_t x = 0; x < res->width; x++)
        {
            GtkEntry *e = GTK_ENTRY(gtk_grid_get_child_at(grid, x, y));
            fprintf(f, "%c", gtk_entry_get_text(e)[0]);
        }

        if (y != res->height - 1)
            fprintf(f, "\n");
    }

    solver_action(name, res);

    free(name);
    fclose(f);
}

void on_network_import(GtkFileChooserButton *file)
{
    network_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file));
    printf("imported network = %s\n", network_path);
}