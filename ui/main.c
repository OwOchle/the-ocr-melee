#include <gtk/gtk.h>
#include <stdio.h>

GtkWidget *window = NULL;
GtkLabel *mylabel = NULL;

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window0"));
    mylabel = GTK_LABEL(gtk_builder_get_object(builder, "label0"));

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

void exit_app()
{
    printf("Exited app\n");
    gtk_widget_destroy(window);
    gtk_main_quit();
}

void button0_clicked()
{
    printf("Button clicked\n");
    gtk_label_set_text(mylabel, "< insert cookie_image :3 >");
}