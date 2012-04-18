#include <gtk/gtk.h>

typedef struct {
    gchar *title;
    gchar *author;
    gboolean checked;
} Book;

static Book BOOKS[] = {
    {"The Human Condition", "Arendt, Hannah", TRUE},
    {"Les trois mousquetaires", "Dumas, Alexandre", FALSE},
    {"De la democratie en Amerique", "De Tocqueville, Alexis", TRUE},
    {"1984", "Orwell, George", FALSE},
    {"The Shock Doctrine", "Klein, Naomi", TRUE},
    {"No Logo", "Klein, Naomi", TRUE},
    {"Le comte de Monte Christo", "Dumas, Alexandre", FALSE},
    {"Ravages", "Barjavel, Rene", FALSE},
    {"Souvenirs", "De Tocqueville, Alexis", FALSE},
    NULL
};

enum
{
    TITLE_COLUMN,
    AUTHOR_COLUMN,
    CHECKED_COLUMN,
    N_COLUMNS
};

static GtkListStore*
create_book_list_store()
{
    GtkListStore *store;
    GtkTreeIter iter;
    int i;
    
    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING,
        G_TYPE_BOOLEAN);

    Book *book = &BOOKS[0];
    while (book->title != NULL) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            TITLE_COLUMN, book->title,
            AUTHOR_COLUMN, book->author,
            CHECKED_COLUMN, book->checked,
            -1);
        book++;
    }

    return store;
}

static GtkTreeViewColumn*
create_column(gint column_id, gchar *title, gboolean is_checkbox)
{
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    gchar *data_attrname;

    if (is_checkbox) {
        renderer = gtk_cell_renderer_toggle_new();
        data_attrname = "active";
    }
    else {
        renderer = gtk_cell_renderer_text_new();
        data_attrname = "text";
    }
    column = gtk_tree_view_column_new_with_attributes(title, renderer,
        data_attrname, column_id, NULL);
    gtk_tree_view_column_set_sort_indicator(column, TRUE);
    gtk_tree_view_column_set_sort_column_id(column, column_id);

    return column;
}

static GtkWidget*
create_book_list_view(GtkListStore *store)
{
    GtkWidget *list_view;
    GtkTreeViewColumn *column;

    list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    column = create_column(TITLE_COLUMN, "Title", FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);
    column = create_column(AUTHOR_COLUMN, "Author", FALSE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);
    column = create_column(CHECKED_COLUMN, "Checked", TRUE);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

    return list_view;
}

int
main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkListStore *store;
    GtkWidget *list_view;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "List Store");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    store = create_book_list_store();
    list_view = create_book_list_view(store);

    gtk_container_add(GTK_CONTAINER(window), list_view);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}