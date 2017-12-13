#include <string.h>
#include <gtk/gtk.h>
//include basic C libraries

static GtkWidget *window = NULL;


//brief Pixmap to scribble area, to store our scribbles

static cairo_surface_t *surface = NULL;

static gboolean scribble_configure_event (GtkWidget*, GdkEventConfigure*, gpointer);

// brief Redraw the screen from the surface
static gboolean scribble_expose_event (GtkWidget*, GdkEventExpose*, gpointer);


//brief Draw a rectangle on the screen
 
static gboolean scribble_configure_event (GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
        cairo_t *cr = NULL;

        if (surface)
        {
                cairo_surface_destroy (surface);
        }

        surface = gdk_window_create_similar_surface (widget->window,
                                                     CAIRO_CONTENT_COLOR,
                                                     widget->allocation.width,
                                                     widget->allocation.height);

        /* Initialize the surface to white */
        cr = cairo_create (surface);
        cairo_set_source_rgb (cr, 1, 1, 1);
        cairo_paint (cr);
        cairo_destroy (cr);

        /* We've handled the configure event, no need for further processing. */
        return TRUE;
}

/*!
 * \brief Redraw the screen from the surface
 */
static gboolean scribble_expose_event (GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
        cairo_t *cr = NULL;

        cr = gdk_cairo_create (widget->window);
        cairo_set_source_surface (cr, surface, 0, 0);
        gdk_cairo_rectangle (cr, &event->area);
        cairo_fill (cr);
        cairo_destroy (cr);

        return FALSE;
}
/*!
 * \brief Draw a rectangle on the screen
 */
static void draw_brush (GtkWidget *widget, gdouble x, gdouble y);

static gboolean scribble_button_press_event (GtkWidget*, GdkEventButton*, gpointer);
static gboolean scribble_motion_notify_event (GtkWidget*, GdkEventMotion*, gpointer);
static gboolean checkerboard_expose (GtkWidget*, GdkEventExpose*, gpointer);
static void close_window (void);
GtkWidget * do_drawingarea ();


/*!
 * \brief Draw a rectangle on the screen
 */
static void draw_brush (GtkWidget *widget, gdouble x, gdouble y)
{
        GdkRectangle update_rect;
        memset(&update_rect, 0, sizeof(GdkRectangle));

        cairo_t *cr = NULL;

        update_rect.x = x - 3;
        update_rect.y = y - 3;
        update_rect.width = 6;
        update_rect.height = 6;

        /* Paint to the surface, where we store our state */
        cr = cairo_create (surface);
        gdk_cairo_rectangle (cr, &update_rect);
        cairo_fill (cr);
        cairo_destroy (cr);

        /* Now invalidate the affected region of the drawing area. */
        gdk_window_invalidate_rect (widget->window,
                                    &update_rect,
                                    FALSE);
}

static gboolean scribble_button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer data)
{
        if (surface == NULL)
        {
                return FALSE; /* Paranoia check, in case we haven't gotten a configure event */
        }

        if (event->button == 1)
        {
                draw_brush (widget, event->x, event->y);
        }

        /* We've handled the event, stop processing */
        return TRUE;
}

static gboolean scribble_motion_notify_event (GtkWidget *widget, GdkEventMotion *event, gpointer data)
{
        int x = 0, y = 0;
        GdkModifierType state = 0;

        if (surface == NULL)
        {
                return FALSE; /* paranoia check, in case we haven't gotten a configure event */
        }

        /* This call is very important; it requests the next motion event.
         * If you don't call gdk_window_get_pointer(), you'll only get a single
         * motion event.The reason is that we specified GDK_POINTER_MOTION_HINT_MASK to gtk_widget_set_events()
         * If we hadn't specified that, we could juste use event->x, event->y as the pointer location.
         * But we'd also get deluged in events.
         * By requesting the next event as we handle the current one, we avoid getting a huge number of
         * events faster than we can cope.
         */
        gdk_window_get_pointer (event->window, &x, &y, &state);

        if (state & GDK_BUTTON1_MASK)
        {
                draw_brush (widget, x, y);
        }

        /* We've handled it, stop processing */
        return TRUE;
}


static void close_window (void)
{
        window = NULL;

        if (surface)
        {
                cairo_surface_write_to_png (surface, "test_pics/temp.png");

                g_object_unref (surface);
        }

        surface = NULL;
        gtk_main_quit();
}

GtkWidget * do_drawingarea ()
{
        GtkWidget *frame = NULL, *vbox = NULL, *da = NULL, *label = NULL;

        if (!window)
        {
                window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
                //gtk_window_set_screen (GTK_WINDOW (window), gtk_widget_get_screen (do_widget));
                gtk_window_set_title (GTK_WINDOW (window), "Drawing Area");
                gtk_window_set_default_size(GTK_WINDOW(window), 1000, 500);

                g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK (close_window), NULL);

                gtk_container_set_border_width (GTK_CONTAINER (window), 8);

                vbox = gtk_vbox_new (FALSE, 8);
                gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
                gtk_container_add (GTK_CONTAINER (window), vbox);

                /*
                 * Create the scribble area
                 */
                label = gtk_label_new (NULL);
                gtk_label_set_markup (GTK_LABEL (label), "<u>Scribble area</u>");
                gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);

                frame = gtk_frame_new (NULL);
                gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
                gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, 0);

                da = gtk_drawing_area_new();

                /* Set a minimum size */
                gtk_widget_set_size_request (da, 100, 100);

                gtk_container_add (GTK_CONTAINER (frame), da);

                /* Signals used to handle backing surface */
                g_signal_connect (da, "expose_event", G_CALLBACK (scribble_expose_event), NULL);
                g_signal_connect (da, "configure_event", G_CALLBACK (scribble_configure_event), NULL);

                /* Event signals */
                g_signal_connect (da, "motion-notify-event", G_CALLBACK (scribble_motion_notify_event), NULL);
                g_signal_connect (da, "button-press-event", G_CALLBACK (scribble_button_press_event), NULL);

                /* Ask to receive events the drawing area doesn't normally
                 * subscribe to
                 */
                gtk_widget_set_events (da, gtk_widget_get_events (da)
                                       | GDK_LEAVE_NOTIFY_MASK
                                       | GDK_BUTTON_PRESS_MASK
                                       | GDK_POINTER_MOTION_MASK
                                       | GDK_POINTER_MOTION_HINT_MASK);

        }

        if (!gtk_widget_get_visible (window))
        {
                gtk_widget_show_all (window);
        }
        else
        {
                gtk_widget_destroy (window);
        }

        return window;
}

void open_dialog(GtkWidget* button, gpointer window)
//open main dialog box

{
        GtkWidget *dialog;
        dialog = gtk_file_chooser_dialog_new("Chosse a file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
       //GtkFileChooserDialog — A file chooser dialog, suitable for "File/Open" or "File/Save" commands
        
        gtk_widget_show_all(dialog);
        //Recursively shows a widget, and any child widgets (if the widget is a container).

        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_home_dir());
        // Note that this is the folder that the file chooser is currently displaying

        gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
        // This function enters a recursive main loop and waits for the user to respond to the dialog, returning the response ID corresponding to the button the user clicked

        if(resp == GTK_RESPONSE_OK)
        {
                g_print("%s\n", gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
                loc = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        }
        else
                g_print("You pressed Cancel\n");
        gtk_widget_destroy(dialog);
        //cause the widget to be finalized if no additional references

}

void  opengui(int argc, char *argv[])
//open the file chooser menu

{
        threshold = 140;
        gtk_disable_setlocale();
        gtk_init (&argc, &argv);
        do_drawingarea();
        gtk_main();

}

void open_browse(int argc, char *argv[]){
        threshold = 70;
        gtk_disable_setlocale();
        gtk_init (&argc, &argv);
        GtkWidget *window, *button;
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
        button = gtk_button_new_with_label("Browse Image");
        g_signal_connect(button, "clicked", G_CALLBACK(open_dialog), window);
        //Connect handlers to signals and using a GObject instance as your signal handler user data

        gtk_container_set_border_width(GTK_CONTAINER(window), 100);
        //Sets the border width of the container.

        gtk_container_add(GTK_CONTAINER(window), button);
        // Adds widget to container , setting child properties at the same time. 

        gtk_widget_show_all(window);
        gtk_main();

}
