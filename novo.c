#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    gchar nome[50];
    gchar cpf[12];
    gdouble salario;
    gint dia_pagamento;
    gchar data_admissao[11];
} Funcionario;

void cadastrar_funcionario(GtkWidget *widget, gpointer data) {
    // Obtém os valores digitados nos campos de entrada
    GtkEntry *entry_nome = GTK_ENTRY(gtk_builder_get_object(data, "entry_nome"));
    GtkEntry *entry_cpf = GTK_ENTRY(gtk_builder_get_object(data, "entry_cpf"));
    GtkEntry *entry_salario = GTK_ENTRY(gtk_builder_get_object(data, "entry_salario"));
    GtkEntry *entry_dia_pagamento = GTK_ENTRY(gtk_builder_get_object(data, "entry_dia_pagamento"));
    GtkEntry *entry_data_admissao = GTK_ENTRY(gtk_builder_get_object(data, "entry_data_admissao"));

    Funcionario funcionario;

    // Copia os valores dos campos para a estrutura de funcionário
    strcpy(funcionario.nome, gtk_entry_get_text(entry_nome));
    strcpy(funcionario.cpf, gtk_entry_get_text(entry_cpf));
    funcionario.salario = atof(gtk_entry_get_text(entry_salario));
    funcionario.dia_pagamento = atoi(gtk_entry_get_text(entry_dia_pagamento));
    strcpy(funcionario.data_admissao, gtk_entry_get_text(entry_data_admissao));

    // Abre o arquivo para escrita no modo de adição
    FILE *arquivo = fopen("funcionarios.txt", "a");

    if (arquivo != NULL) {
        // Escreve os dados do funcionário no arquivo
        fprintf(arquivo, "Nome: %s\n", funcionario.nome);
        fprintf(arquivo, "CPF: %s\n", funcionario.cpf);
        fprintf(arquivo, "Salário: %.2f\n", funcionario.salario);
        fprintf(arquivo, "Dia de Pagamento: %d\n", funcionario.dia_pagamento);
        fprintf(arquivo, "Data de Admissão: %s\n", funcionario.data_admissao);
        fprintf(arquivo, "---------------------------------\n");

        // Fecha o arquivo
        fclose(arquivo);

        // Exibe uma mensagem de sucesso
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Funcionário cadastrado com sucesso!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // Aplica o estilo ao botão cadastrar
        GtkWidget *button_cadastrar = GTK_WIDGET(gtk_builder_get_object(data, "button_cadastrar"));

        GtkCssProvider *css_provider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(css_provider, "C:\\msys64\\home\\center music\\style.css", NULL);

        GtkStyleContext *style_context = gtk_widget_get_style_context(button_cadastrar);
        gtk_style_context_add_provider(style_context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

        gtk_widget_override_background_color(button_cadastrar, GTK_STATE_FLAG_NORMAL, NULL);
    } else {
        // Exibe uma mensagem de erro
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Erro ao abrir o arquivo!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "interface.glade", NULL);

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *button_cadastrar = GTK_WIDGET(gtk_builder_get_object(builder, "button_cadastrar"));
    g_signal_connect(button_cadastrar, "clicked", G_CALLBACK(cadastrar_funcionario), builder);

    gtk_widget_show_all(window);

    // Carregando o CSS externo
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "C:\\msys64\\home\\center music\\style.css", NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_main();

    return 0;
}