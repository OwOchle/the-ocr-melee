#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "proc_steps.h"

GtkWidget *mainWindow = NULL;
GtkImage *mainImage = NULL;

GtkWidget *solverWindow = NULL;
GtkImage *solverImage = NULL;

GtkWidget *settingsWindow = NULL;
GtkImage *settingsImage= NULL;

GtkWidget *rotationWindow = NULL;
GtkImage *rotationImage= NULL;

char* fileName = NULL; // Image de départ

GdkPixbuf *pixbuf = NULL;

bool check_image_existence()
{
    if (fileName == NULL)
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

    if (stat(fileName, &_))
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

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "main.glade", NULL);

    mainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
    mainImage = GTK_IMAGE(gtk_builder_get_object(builder, "mainImage"));

    solverWindow = GTK_WIDGET(gtk_builder_get_object(builder, "solverWindow"));
    solverImage = GTK_IMAGE(gtk_builder_get_object(builder, "solverImage"));

    settingsWindow = GTK_WIDGET(gtk_builder_get_object(builder, "settingsWindow"));
    settingsImage = GTK_IMAGE(gtk_builder_get_object(builder, "settingsImage"));

    rotationWindow = GTK_WIDGET(gtk_builder_get_object(builder, "rotationWindow"));
    rotationImage = GTK_IMAGE(gtk_builder_get_object(builder, "rotationImage"));

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show(mainWindow);
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
    gtk_widget_show_all(mainWindow);
}

void open_solver() // Bouton qui ouvre le solver
{
    printf("Solver opened\n");
    gtk_widget_hide(mainWindow);
    gtk_widget_show_all(solverWindow);
}

void open_settings() // Bouton qui ouvre les settings
{
    printf("Settings opened\n");
    gtk_widget_hide(mainWindow);
    gtk_widget_show_all(settingsWindow);
}

void open_rotation() // Bouton qui ouvre la fenêtre de rotation
{
    printf("Rotation window opened\n");
    gtk_widget_hide(mainWindow);
    gtk_widget_show_all(rotationWindow);
}

void leftRotation() // Bouton de rotation gauche
{
    printf("Rotated left by 3 degrees\n");
}

void rightRotation() // Bouton de rotation droit
{
    printf("Rotated right by 3 degrees\n");
}

void automaticRotation() // Bouton de rotation droit
{
    printf("Rotating...\n");
}

void allInOne_clicked() // Bouton qui solve d'un coup
{
    printf("Solving...\n");
}

void rotButton_clicked() // Bouton qui rotate AUTO
{
    printf("Rotating...\n");
}

void update_all_pixbufs()
{
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 800,450,GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf(mainImage,pixbuf);
    gtk_image_set_from_pixbuf(solverImage,pixbuf);
    gtk_image_set_from_pixbuf(settingsImage,pixbuf);
    gtk_image_set_from_pixbuf(rotationImage,pixbuf);
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
    
    g_object_unref(pixbuf);
    pixbuf = get_next_image_step(fileName);
    update_all_pixbufs();
}

void on_imageImport(GtkFileChooserButton *file)
{
    reset_steps();
    fileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file));
    printf("imported file name = %s\n", fileName);

    pixbuf = gdk_pixbuf_new_from_file(fileName, NULL);
    update_all_pixbufs();
}

void on_network_import(GtkFileChooserButton *file)
{
    
}