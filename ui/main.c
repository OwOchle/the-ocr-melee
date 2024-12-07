#include <gtk/gtk.h>
#include <stdio.h>

GtkWidget *mainWindow = NULL;
GtkImage *mainImage = NULL;
GtkWidget *solverWindow = NULL;
GtkWidget *settingsWindow = NULL;
GtkWidget *rotationWindow = NULL;

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "main.glade", NULL);

    mainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
    mainImage = GTK_IMAGE(gtk_builder_get_object(builder, "mainImage"));

    solverWindow = GTK_WIDGET(gtk_builder_get_object(builder, "solverWindow"));

    settingsWindow = GTK_WIDGET(gtk_builder_get_object(builder, "settingsWindow"));

    rotationWindow = GTK_WIDGET(gtk_builder_get_object(builder, "rotationWindow"));

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
    gtk_widget_show(mainWindow);
}

void open_solver() // Bouton qui ouvre le solver
{
    printf("Solver opened\n");
    gtk_widget_hide(mainWindow);
    gtk_widget_show(solverWindow);
}

void open_settings() // Bouton qui ouvre les settings
{
    printf("Settings opened\n");
    gtk_widget_hide(mainWindow);
    gtk_widget_show(settingsWindow);
}

void open_rotation() // Bouton qui ouvre la fenêtre de rotation
{
    printf("Rotation window opened\n");
    gtk_widget_hide(mainWindow);
    gtk_widget_show(rotationWindow);
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

void stepByStep_clicked() // Bouton qui solve en montrant chaque étape
{
    printf("------------------------------------------\n");
    printf("Solving:\nStep 1 : ...\nStep 2 : ...\nStep 3 : ...\n");
    printf("------------------------------------------\n");
}

void on_imageImport(GtkFileChooserButton *file)
{
    char *str = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file));
    printf("imported file name = %s\n", str);
    gtk_image_set_from_file(mainImage,str);
}

void on_networkImput(GtkFileChooserButton *file)
{

}