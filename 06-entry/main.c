#include <gtk/gtk.h>

static GtkWidget *g_entry;
static GtkWidget *g_label;

static void
set_entry_text(GtkWidget *widget, gpointer data)
{
    gtk_label_set_text(GTK_LABEL(g_label), gtk_entry_get_text(GTK_ENTRY(g_entry)));
}

int
main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Entry");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    g_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), g_entry, 0, 0, 2, 1);
    
    g_label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), g_label, 0, 1, 2, 1);


    button = gtk_button_new_with_label("Set Text");
    g_signal_connect(button, "clicked", G_CALLBACK(set_entry_text), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);

    button = gtk_button_new_with_label("Quit");
    g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 1, 2, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}