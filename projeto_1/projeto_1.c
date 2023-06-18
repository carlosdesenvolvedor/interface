#include <gtk/gtk.h>


GtkLabel *label;
GtkEntry *entry;

void on_botao_clicked(GtkWidget *widget, gpointer data){
    const gchar *texto = gtk_entry_get_text(entry);
    gtk_label_set_text(label,texto);
}

void on_main_window_destroy(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}


int main(int argc, char const *argv[])
{
   
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new_from_file("ui.glade");
    gtk_builder_add_callback_symbols(builder,
                        "on_botao_clicked",G_CALLBACK(on_botao_clicked),
                        "on_main_window_destroy",G_CALLBACK(on_main_window_destroy),NULL);


    gtk_builder_connect_signals(builder, NULL);
    label = GTK_LABEL(gtk_builder_get_object(builder,"label"));
    entry = GTK_ENTRY(gtk_builder_get_object(builder,"entry"));
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    gtk_widget_show_all(window);
    gtk_main();


    return 0;
}
