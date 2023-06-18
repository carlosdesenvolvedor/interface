#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <gdk/gdkkeysyms.h>

gboolean on_main_window_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data);
void on_botao_clicked(GtkButton *button, gpointer data);
void on_botao_reset_clicked(GtkWidget *widget, gpointer data);
void on_botao_raiz_quadrada_clicked(GtkWidget *widget, gpointer data);
void on_botao_porcentagem_clicked(GtkWidget *widget, gpointer data);
void on_botao_divisao_clicked(GtkWidget *widget, gpointer data);
void on_botao_multiplicacao_clicked(GtkWidget *widget, gpointer data);
void on_botao_subtracao_clicked(GtkWidget *widget, gpointer data);
void on_botao_soma_clicked(GtkWidget *widget, gpointer data);
void on_botao_igual_clicked(GtkWidget *widget, gpointer data);
void on_botao_ponto_clicked(GtkWidget *widget, gpointer data);
void on_main_window_destroy(GtkWidget *widget, gpointer data);

GtkEntry *display;
char operacao[100];
float primeiro_numero;
bool limpar_display = FALSE;

float ler_display() {
    gchar *string = g_strdup(gtk_entry_get_text(display));

    // Substituir vírgula por ponto
    gchar *ponto_decimal = strchr(string, ',');
    if (ponto_decimal != NULL)
        *ponto_decimal = '.';

    float numero = atof(string);
    g_free(string);

    return numero;
}

void usar_estilo() {
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "C:\\msys64\\home\\center music\\projeto3\\estilo_calculadora.css", FALSE);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

gboolean on_main_window_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_KP_Enter) {
        on_botao_igual_clicked(NULL, NULL);
        return TRUE;
    }

    char *tecla = NULL;

    switch (event->keyval) {
        case GDK_KEY_KP_0:
        case GDK_KEY_0:
            tecla = "0";
            break;
        case GDK_KEY_KP_1:
        case GDK_KEY_1:
            tecla = "1";
            break;
        case GDK_KEY_KP_2:
        case GDK_KEY_2:
            tecla = "2";
            break;
        case GDK_KEY_KP_3:
        case GDK_KEY_3:
            tecla = "3";
            break;
        case GDK_KEY_KP_4:
        case GDK_KEY_4:
            tecla = "4";
            break;
        case GDK_KEY_KP_5:
        case GDK_KEY_5:
            tecla = "5";
            break;
        case GDK_KEY_KP_6:
        case GDK_KEY_6:
            tecla = "6";
            break;
        case GDK_KEY_KP_7:
        case GDK_KEY_7:
            tecla = "7";
            break;
        case GDK_KEY_KP_8:
        case GDK_KEY_8:
            tecla = "8";
            break;
        case GDK_KEY_KP_9:
        case GDK_KEY_9:
            tecla = "9";
            break;
        case GDK_KEY_KP_Decimal:
        case GDK_KEY_period:
        case GDK_KEY_comma:
            on_botao_ponto_clicked(NULL, NULL);
            return TRUE;
        case GDK_KEY_KP_Add:
        case GDK_KEY_plus:
            on_botao_soma_clicked(NULL, NULL);
            return TRUE;
        case GDK_KEY_KP_Subtract:
        case GDK_KEY_minus:
            on_botao_subtracao_clicked(NULL, NULL);
            return TRUE;
        case GDK_KEY_KP_Multiply:
        case GDK_KEY_asterisk:
            on_botao_multiplicacao_clicked(NULL, NULL);
            return TRUE;
        case GDK_KEY_KP_Divide:
        case GDK_KEY_slash:
            on_botao_divisao_clicked(NULL, NULL);
            return TRUE;
        case GDK_KEY_percent:
            on_botao_porcentagem_clicked(NULL, NULL);
            return TRUE;
        case GDK_KEY_Start:
        case GDK_KEY_s:
            on_botao_raiz_quadrada_clicked(NULL, NULL);
            return TRUE;
    }

    if (tecla != NULL) {
        on_botao_clicked(NULL, tecla);
        return TRUE;
    }

    return FALSE;
}

void on_botao_clicked(GtkButton *button, gpointer data) {
    if (limpar_display) {
        gtk_entry_set_text(display, "0");
        limpar_display = FALSE;
    }       

    if (strcmp(gtk_entry_get_text(display), "0") == 0)
        gtk_entry_set_text(display, (char *)data);
    else {
        char texto[100];
        g_snprintf(texto, 100, "%s%s", gtk_entry_get_text(display), (char *)data);
        gtk_entry_set_text(display, texto);
    }
}

void on_botao_reset_clicked(GtkWidget *widget, gpointer data) {
    gtk_entry_set_text(display, "0");
}

void on_botao_raiz_quadrada_clicked(GtkWidget *widget, gpointer data) {
    strcpy(operacao, "raiz_quadrada");
    limpar_display = TRUE;
    on_botao_igual_clicked(widget, data);
}

void on_botao_porcentagem_clicked(GtkWidget *widget, gpointer data) {
    strcpy(operacao, "porcentagem");
    primeiro_numero = ler_display();
    limpar_display = TRUE;
}

void on_botao_divisao_clicked(GtkWidget *widget, gpointer data) {
    strcpy(operacao, "divisão");
    primeiro_numero = ler_display();
    limpar_display = TRUE;
}

void on_botao_multiplicacao_clicked(GtkWidget *widget, gpointer data) {
    strcpy(operacao, "multiplicação");
    primeiro_numero = ler_display();
    limpar_display = TRUE;
}

void on_botao_subtracao_clicked(GtkWidget *widget, gpointer data) {
    strcpy(operacao, "subtração");
    primeiro_numero = ler_display();
    limpar_display = TRUE;
}

void on_botao_soma_clicked(GtkWidget *widget, gpointer data) {
    strcpy(operacao, "soma");
    primeiro_numero = ler_display();
    limpar_display = TRUE;
}

void on_botao_igual_clicked(GtkWidget *widget, gpointer data) {
    float segundo_numero = ler_display();
    float resultado_f;

    if (strcmp(operacao, "soma") == 0)
        resultado_f = primeiro_numero + segundo_numero;
    else if (strcmp(operacao, "subtração") == 0)
        resultado_f = primeiro_numero - segundo_numero;
    else if (strcmp(operacao, "multiplicação") == 0)
        resultado_f = primeiro_numero * segundo_numero;
    else if (strcmp(operacao, "raiz_quadrada") == 0)
        resultado_f = sqrt(segundo_numero);
    else if (strcmp(operacao, "divisão") == 0)
        resultado_f = primeiro_numero / segundo_numero;
    else if (strcmp(operacao, "porcentagem") == 0)
        resultado_f = primeiro_numero * segundo_numero / 100;
    else
        resultado_f = 0.0; // Operação desconhecida

    char *resultado = g_strdup_printf("%.2f", resultado_f);
    gtk_entry_set_text(display, resultado);
    g_free(resultado);
}

void on_botao_ponto_clicked(GtkWidget *widget, gpointer data) {
    char texto[100];
    g_snprintf(texto, 100, "%s%s", gtk_entry_get_text(display), ",");
    gtk_entry_set_text(display, texto);
}

void on_main_window_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new_from_file("C:\\msys64\\home\\center music\\projeto3\\projeto3.glade");

    gtk_builder_add_callback_symbols(builder,
                                      "on_botao_clicked", G_CALLBACK(on_botao_clicked),
                                      "on_main_window_destroy", G_CALLBACK(on_main_window_destroy),
                                      "on_botao_reset_clicked", G_CALLBACK(on_botao_reset_clicked),
                                      "on_botao_raiz_quadrada_clicked", G_CALLBACK(on_botao_raiz_quadrada_clicked),
                                      "on_botao_porcentagem_clicked", G_CALLBACK(on_botao_porcentagem_clicked),
                                      "on_botao_divisao_clicked", G_CALLBACK(on_botao_divisao_clicked),
                                      "on_botao_multiplicacao_clicked", G_CALLBACK(on_botao_multiplicacao_clicked),
                                      "on_botao_subtracao_clicked", G_CALLBACK(on_botao_subtracao_clicked),
                                      "on_botao_soma_clicked", G_CALLBACK(on_botao_soma_clicked),
                                      "on_botao_igual_clicked", G_CALLBACK(on_botao_igual_clicked),
                                      "on_botao_ponto_clicked", G_CALLBACK(on_botao_ponto_clicked),
                                      NULL);

    gtk_builder_connect_signals(builder, NULL);

    display = GTK_ENTRY(gtk_builder_get_object(builder, "display"));
    gtk_entry_set_text(display, "0");

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    usar_estilo();
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_main_window_key_press_event), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
