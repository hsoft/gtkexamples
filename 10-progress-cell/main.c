#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* When I was writing this on my Ubuntu machine, I had a very weird display corruption bug where,
at the beginning of the race, not all rows would display, but after a while everything would come
back in order. I had a lot of head scratching over this and I finally isolated the problem to:
Whenever a progress bar would have a value of 3 or 5, this row and all following rows would have
their display corrupted. Very weird. Really, you remove all the timer and random stuff and simply
assign a value of 3 or 5 to one of the progress values and you get the bug.

Now that I try to run this with GTK+ 3.4.1 on OS X, I don't get the same bug, so I don't know if the
cause for this is a bug in my code (for example, a subtle memory management problem) or if it's GTK+
or something else.
 */

#define NUM_HORSES 10

static GtkWidget *g_label;
static GtkListStore *g_store;
static GtkWidget *g_list_view;
static int g_progress[NUM_HORSES];
static gboolean g_running = FALSE;

enum
{
    NAME_COLUMN,
    PROGRESS_COLUMN,
    N_COLUMNS
};

static void
update_horse_progress()
{
    GtkTreeIter iter;
    gboolean can_iter;
    int i;

    i = 0;
    can_iter = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(g_store), &iter);
    while (can_iter) {
        gtk_list_store_set(g_store, &iter,
            PROGRESS_COLUMN, g_progress[i],
            -1);
        i++;
        can_iter = gtk_tree_model_iter_next(GTK_TREE_MODEL(g_store), &iter);
    }
}

static GtkListStore*
create_horse_list_store()
{
    GtkListStore *store;
    GtkTreeIter iter;
    int i;
    char horse_name[10];
    
    store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_INT);
    for (i=0; i<NUM_HORSES; i++) {
        sprintf(horse_name, "Horse %d", i+1);
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
            NAME_COLUMN, horse_name,
            PROGRESS_COLUMN, 0,
            -1);
    }
    return store;
}

static GtkWidget*
create_horse_list_view(GtkListStore *store)
{
    GtkWidget *list_view;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;

    list_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Name", renderer,
        "text", NAME_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);
    renderer = gtk_cell_renderer_progress_new();
    column = gtk_tree_view_column_new_with_attributes("Progress", renderer,
        "value", PROGRESS_COLUMN, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

    return list_view;
}

static gboolean
timer_update_race(gpointer data)
{
    int i;
    int winner = -1;

    for (i=0; i<NUM_HORSES; i++) {
        int progress = g_progress[i] + (rand() % 5);
        if (progress >= 100) {
            progress = 100;
            if (winner == -1) {
                winner = i;
            }
        }
        g_progress[i] = progress;
    }
    update_horse_progress();
    if (winner >= 0) {
        char msg[100];
        
        sprintf(msg, "The winner is horse #%d!", winner+1);
        gtk_label_set_text(GTK_LABEL(g_label), msg);
        g_running = FALSE;
        return FALSE;
    }
    else {
        return TRUE;
    }
}

static void
start_race_clicked(GtkWidget *widget, gpointer data)
{
    int i;
    
    if (g_running) {
        return;
    }
    g_running = TRUE;
    for (i=0; i<NUM_HORSES; i++) {
        g_progress[i] = 0;
    }
    update_horse_progress();
    gtk_label_set_text(GTK_LABEL(g_label), "The race is on!");
    g_timeout_add(1000, timer_update_race, NULL);
}

int
main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *button;

    srand((unsigned int)time(NULL));
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Horse Race!");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    button = gtk_button_new_with_label("Start Race");
    gtk_container_add(GTK_CONTAINER(box), button);
    g_signal_connect(button, "clicked", G_CALLBACK(start_race_clicked), NULL);

    g_label = gtk_label_new("Click Start Race to start.");
    gtk_container_add(GTK_CONTAINER(box), g_label);

    g_store = create_horse_list_store();
    update_horse_progress();
    g_list_view = create_horse_list_view(g_store);
    gtk_container_add(GTK_CONTAINER(box), g_list_view);

    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}
