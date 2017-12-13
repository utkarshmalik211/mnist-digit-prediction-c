
void open_dialog(GtkWidget* button, gpointer window)
{
        GtkWidget *dialog;
        dialog = gtk_file_chooser_dialog_new("Chosse a file", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
        gtk_widget_show_all(dialog);
//      gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog),"/");
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), g_get_home_dir());
        gint resp = gtk_dialog_run(GTK_DIALOG(dialog));
        if(resp == GTK_RESPONSE_OK)
                a =  gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        else
                g_print("You pressed Cancel\n");
        gtk_widget_destroy(dialog);
}

void open_chooser_dialog(int argc, char* argv[])
{
        gtk_init(&argc, &argv);
        GtkWidget *window, *button;
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

        button = gtk_button_new_with_label("Browse Image");
        g_signal_connect(button, "clicked", G_CALLBACK(open_dialog), window);
        gtk_container_set_border_width(GTK_CONTAINER(window), 100);
        gtk_container_add(GTK_CONTAINER(window), button);


        gtk_widget_show_all(window);
        gtk_main();
}
