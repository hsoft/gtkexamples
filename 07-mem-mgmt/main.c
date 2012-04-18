#include <gtk/gtk.h>

/* GObjects use reference counting with g_object_ref() and g_object_unref()
   There's a good memory management guide at
   http://www.gtkforums.com/viewtopic.php?t=2412

   In this example we, as a matter of fact, do nothing special about memory
   management. We simply dynamically create a dialog on a button push. What
   we do, however, it to track when our child elements (both the dialog and
   the label) are being destroyed. Upon running the program, we see that they
   are both being destroyed when we close the child dialog. It's important to
   know why: The dialog is destroyed because it is default behavior, for a
   window to destroy itself on "delete-event". The label is destroyed because
   it was added to the container that is the child window. When a widget is
   added to a container, it takes responsibility for unref'fing it when the
   container itself is destroyed.

   A question you might ask is "But why don't I have to unref the label after
   having added it to its container?" If you read the linked article, you'll
   see that all GtkWidgets start with a "floating" ref, not a "real" ref. So
   that's why.
 */

static void
child_label_destroyed(GtkWidget *widget, gpointer data)
{
    g_print("A child label was destroyed.\n");
}

static void
child_dialog_destroyed(GtkWidget *widget, gpointer data)
{
    g_print("A child dialog was destroyed.\n");
}

static GtkWidget*
create_child_dialog()
{
    GtkWidget *window;
    GtkWidget *label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(child_dialog_destroyed), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "Sub-dialog");

    label = gtk_label_new("This is a sub dialog");
    g_signal_connect(window, "destroy", G_CALLBACK(child_label_destroyed), NULL);
    gtk_container_add(GTK_CONTAINER(window), label);
    gtk_widget_show(label);

    return window;
}

static void
new_dialog_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget *window;
    window = create_child_dialog();
    gtk_widget_show(window);
}

static GtkWidget*
create_main_window()
{
    GtkWidget *window;
    GtkWidget *button;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Memory Management");
    g_signal_connect(window,"destroy", G_CALLBACK(gtk_main_quit), NULL);

    button = gtk_button_new_with_label("New Dialog");
    g_signal_connect(button, "clicked", G_CALLBACK(new_dialog_clicked), NULL);
    gtk_container_add(GTK_CONTAINER(window), button);
    gtk_widget_show(button);

    return window;
}

int
main(int argc, char *argv[])
{
  GtkWidget *window;

  gtk_init (&argc, &argv);
  window = create_main_window();
  gtk_widget_show(window);
  gtk_main();

  return 0;
}