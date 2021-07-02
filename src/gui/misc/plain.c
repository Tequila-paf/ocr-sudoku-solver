#include <gtk/gtk.h>

// Signal handler for the "clicked" signal of the start button.
void on_start(GtkButton *button, gpointer user_data)
{
    g_print("on_start()\n");
}

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
	GdkRectangle *r =  user_data;

	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);

	cairo_set_source_rgb(cr, 1, 0, 0);
	cairo_rectangle(cr, r->x, r->y, r->width, r->height);
	cairo_fill(cr);

	return FALSE;
}


gboolean on_configure(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
	GdkRectangle *rect = user_data;

	int width = gtk_widget_get_allocated_width(widget);
	int height = gtk_widget_get_allocated_height(widget);

	rect -> x = width / 2;
	rect -> y = height / 2;

	return TRUE;
}

// Main function.
int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "plain.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    
    GdkRectangle rect = {100, 100, 10, 10};
	
	GtkDrawingArea* area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "area"));
	g_signal_connect(area, "draw", G_CALLBACK(on_draw), &rect);

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.duel"));
    GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "start_button"));

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start), NULL);
    g_signal_connect(area, "draw", G_CALLBACK(on_configure), &rect);
    

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
