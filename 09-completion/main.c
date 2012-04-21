#include <gtk/gtk.h>

static gchar *COMPLETIONS[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
    "ten",
    NULL
};

static GtkEntryCompletion *g_completion;

static GtkListStore*
create_completion_list_store()
{
    GtkListStore *store;
    GtkTreeIter iter;

    store = gtk_list_store_new(1, G_TYPE_STRING);

    gchar **completion = COMPLETIONS;
    while (*completion != NULL) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 0, *completion, -1);
        completion++;
    }

    return store;
}

static void
popup_radio_toggled(GtkToggleButton *button, gpointer data)
{
    gtk_entry_completion_set_inline_completion(g_completion, FALSE);
    gtk_entry_completion_set_popup_completion(g_completion, TRUE);
}

static void
inline_radio_toggled(GtkToggleButton *button, gpointer data)
{
    gtk_entry_completion_set_popup_completion(g_completion, FALSE);
    gtk_entry_completion_set_inline_completion(g_completion, TRUE);
}

int
main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *entry;
    GtkWidget *radio1;
    GtkWidget *radio2;
    GtkListStore *store;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Completion");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("There are completions for numbers from 'one' to 'ten'");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);

    radio1 = gtk_radio_button_new_with_label(NULL, "Popup");
    gtk_grid_attach(GTK_GRID(grid), radio1, 0, 1, 1, 1);
    radio2 = gtk_radio_button_new_with_label_from_widget(
        GTK_RADIO_BUTTON(radio1), "Inline");
    gtk_grid_attach(GTK_GRID(grid), radio2, 1, 1, 1, 1);

    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 2, 2, 1);
    
    store = create_completion_list_store();
    g_completion = gtk_entry_completion_new();
    gtk_entry_completion_set_model(g_completion, GTK_TREE_MODEL(store));
    gtk_entry_completion_set_text_column(g_completion, 0);
    gtk_entry_set_completion(GTK_ENTRY(entry), g_completion);

    g_signal_connect(radio1, "toggled", G_CALLBACK(popup_radio_toggled), NULL);
    g_signal_connect(radio2, "toggled", G_CALLBACK(inline_radio_toggled), NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}