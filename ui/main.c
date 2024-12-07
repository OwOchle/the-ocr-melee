#include <gtk/gtk.h>
#include <stdio.h>

GtkWidget *mainWindow = NULL;
// GtkWidget *mylabel = NULL;
GtkImage *mainImage = NULL;

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "main.glade", NULL);

    mainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));
    // mylabel = GTK_WIDGET(gtk_builder_get_object(builder, "label0"));
    mainImage = GTK_IMAGE(gtk_builder_get_object(builder, "mainImage"));


    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show_all(mainWindow);
    gtk_main();

    return 0;
}

void exit_app() // Quand on ferme la fenêtre
{
    printf("Exited app\n");
    gtk_widget_destroy(mainWindow);
    gtk_main_quit();
}

void button0_clicked() // Tests
{
    printf("Button clicked\n");
    // gtk_label_set_text(mylabel, "< insert cookie_image :3 >");
}

void lrotButton_clicked() // Bouton de rotation gauche
{
    printf("Rotated left by 3 degrees\n");
}

void rrotButton_clicked() // Bouton de rotation droit
{
    printf("Rotated right by 3 degrees\n");
}

void solveButton_clicked() // Bouton qui solve
{
    printf("Solving...\n");
}

void rotButton_clicked() // Bouton qui rotate AUTO
{
    printf("Rotating...\n");
}

void stepButton_clicked() // Bouton qui solve en montrant chaque étape
{
    printf("------------------------------------------\n");
    printf("Solving:\nStep 1 : ...\nStep 2 : ...\nStep 3 : ...\n");
    printf("------------------------------------------\n");
}

void on_fileImport_set(GtkFileChooserButton *file)
{
    char *str = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file));
    printf("imported file name = %s\n", str);
    gtk_image_set_from_file(mainImage,str);
}