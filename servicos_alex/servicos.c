#include <gtk/gtk.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *produtoEntry;
GtkWidget *servicoEntry;
GtkWidget *descontoEntry;
GtkWidget *descricaoEntry;
GtkWidget *totalEntry;
GtkWidget *adicionarButton;
GtkWidget *finalizarButton;

//protótipos
void exibir_mensagem(const gchar *mensagem);


void abrir_servicos(GtkWidget *widget, gpointer data) {
    FILE *arquivo = fopen("servicos.txt", "r");
    if (arquivo == NULL) {
        exibir_mensagem("Erro ao abrir o arquivo de serviços.");
        return;
    }

    gchar buffer[200];
    gchar textoServicos[5000] = "";
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        // Verificar se o texto é uma linha válida em UTF-8
        if (!g_utf8_validate(buffer, -1, NULL)) {
            continue;  // Ignorar linhas inválidas
        }
        
        g_strlcat(textoServicos, buffer, sizeof(textoServicos));
    }

    fclose(arquivo);

    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "%s",
                                               textoServicos);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void exibir_mensagem(const gchar *mensagem) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "%s",
                                               mensagem);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void calcular_total(GtkWidget *widget, gpointer data) {
    // Obter os valores dos campos de entrada
    const gchar *produto = gtk_entry_get_text(GTK_ENTRY(produtoEntry));
    const gchar *servico = gtk_entry_get_text(GTK_ENTRY(servicoEntry));
    const gchar *desconto = gtk_entry_get_text(GTK_ENTRY(descontoEntry));
    const gchar *descricao = gtk_entry_get_text(GTK_ENTRY(descricaoEntry));

    // Converter os valores para números
    gdouble valorServico = g_ascii_strtod(servico, NULL);
    gdouble valorDesconto = g_ascii_strtod(desconto, NULL);

    // Calcular o valor total
    gdouble valorTotal = valorServico - valorDesconto;

    // Atualizar o campo de valor total
    gchar totalStr[20];
    g_snprintf(totalStr, sizeof(totalStr), "%.2f", valorTotal);
    gtk_entry_set_text(GTK_ENTRY(totalEntry), totalStr);
    gtk_editable_set_editable(GTK_EDITABLE(totalEntry), FALSE);

    gtk_widget_set_sensitive(adicionarButton, TRUE);
    gtk_widget_set_sensitive(finalizarButton, TRUE);
}

void adicionar_servico(GtkWidget *widget, gpointer data) {
    // Obter os valores dos campos de entrada
    const gchar *produto = gtk_entry_get_text(GTK_ENTRY(produtoEntry));
    const gchar *servico = gtk_entry_get_text(GTK_ENTRY(servicoEntry));
    const gchar *desconto = gtk_entry_get_text(GTK_ENTRY(descontoEntry));
    const gchar *descricao = gtk_entry_get_text(GTK_ENTRY(descricaoEntry));

    // Converter os valores para números
    gdouble valorServico = g_ascii_strtod(servico, NULL);
    gdouble valorDesconto = g_ascii_strtod(desconto, NULL);

    // Calcular o valor total
    gdouble valorTotal = valorServico - valorDesconto;

    // Obter a data atual do sistema
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    gchar dataStr[80];
    strftime(dataStr, sizeof(dataStr), "%d/%m/%Y", timeinfo);

    // Abrir o arquivo de serviços em modo de adição
    FILE *arquivo = fopen("servicos.txt", "a");
    if (arquivo == NULL) {
        exibir_mensagem("Erro ao abrir o arquivo de serviços.");
        return;
    }

    // Salvar os valores no arquivo
    fprintf(arquivo, "\n");
    fprintf(arquivo, "Data de Entrada: %s\n", dataStr);
    fprintf(arquivo, "Instrumento: %s\n", produto);
    fprintf(arquivo, "Valor do Serviço: %.2f\n", valorServico);
    fprintf(arquivo, "Desconto: %.2f\n", valorDesconto);
    fprintf(arquivo, "Descrição do serviço: %s\n", descricao);
    fprintf(arquivo, "Valor Total: %.2f\n", valorTotal);

    // Fechar o arquivo
    fclose(arquivo);

    // Atualizar o arquivo com o total geral
    FILE *arquivoTotal = fopen("servicos.txt", "r");
    if (arquivoTotal == NULL) {
        exibir_mensagem("Erro ao abrir o arquivo de serviços.");
        return;
    }

    gdouble totalGeral = 0.0;
    char linha[256];
    int qtdGeral;
    while (fgets(linha, sizeof(linha), arquivoTotal) != NULL) {
        if (strstr(linha, "Valor Total:") != NULL) {
            gdouble valor;
            sscanf(linha, "Valor Total: %lf", &valor);
            totalGeral += valor; 
            // preciso criar uma logica aqui para apagar está linha de total geral a cada novo produto
            // para sair somente o total geral no ultimo produto.
            qtdGeral++;
            int final = qtdGeral;
            while(qtdGeral<final){
                strstr(linha,"Total Geral:");
                sscanf(linha,"....");
            }
        }
    }

    fclose(arquivoTotal);

    // Abrir o arquivo de serviços em modo de escrita para adicionar o total geral
    FILE *arquivoAtualizado = fopen("servicos.txt", "a");
    if (arquivoAtualizado == NULL) {
        exibir_mensagem("Erro ao abrir o arquivo de serviços.");
        return;
    }
   
    
    fprintf(arquivoAtualizado,"\n código: %d\n",qtdGeral);
   
    fprintf(arquivoAtualizado, "\n\t\t\t\tTotal Geral: %.2f\n\n", totalGeral);
    fprintf(arquivoAtualizado, "==========================================================");
   
    fclose(arquivoAtualizado);

    exibir_mensagem("O serviço foi adicionado ao arquivo servicos.txt.");
}

void finalizar_servico(GtkWidget *widget, gpointer data) {
    gtk_widget_set_sensitive(adicionarButton, FALSE);
    gtk_widget_set_sensitive(finalizarButton, FALSE);

    //exibir_mensagem("");

    // Limpar os campos de entrada
    gtk_entry_set_text(GTK_ENTRY(produtoEntry), "");
    gtk_entry_set_text(GTK_ENTRY(servicoEntry), "");
    gtk_entry_set_text(GTK_ENTRY(descontoEntry), "");
    gtk_entry_set_text(GTK_ENTRY(descricaoEntry), "");
    gtk_entry_set_text(GTK_ENTRY(totalEntry), "");
    gtk_editable_set_editable(GTK_EDITABLE(totalEntry), TRUE);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sistema de Serviços");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *produtoLabel = gtk_label_new("Instrumento:");
    gtk_grid_attach(GTK_GRID(grid), produtoLabel, 0, 0, 1, 1);

    produtoEntry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), produtoEntry, 1, 0, 1, 1);

    GtkWidget *servicoLabel = gtk_label_new("Valor do Serviço:");
    gtk_grid_attach(GTK_GRID(grid), servicoLabel, 0, 2, 1, 1 );

    servicoEntry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), servicoEntry,1, 2, 1, 1);

    GtkWidget *descontoLabel = gtk_label_new("Desconto:");
    gtk_grid_attach(GTK_GRID(grid), descontoLabel, 0, 3, 1, 1);

    descontoEntry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), descontoEntry, 1, 3, 1, 1);

    GtkWidget *descricaoLabel = gtk_label_new("Descrição do serviço:");
    gtk_grid_attach(GTK_GRID(grid), descricaoLabel, 0, 1, 1, 1);
    

    descricaoEntry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), descricaoEntry, 1, 1 ,1 , 1);

    GtkWidget *calcularButton = gtk_button_new_with_label("Calcular desconto");
    g_signal_connect(calcularButton, "clicked", G_CALLBACK(calcular_total), NULL);
    gtk_grid_attach(GTK_GRID(grid), calcularButton, 0, 4, 2, 1);

    GtkWidget *totalLabel = gtk_label_new("Valor Total c/ desconto:");
    gtk_grid_attach(GTK_GRID(grid), totalLabel, 0, 5, 1, 1);

    totalEntry = gtk_entry_new();
    gtk_entry_set_tabs(GTK_ENTRY(totalEntry), FALSE);
    gtk_grid_attach(GTK_GRID(grid), totalEntry, 1, 5, 1, 1);

    adicionarButton = gtk_button_new_with_label("Adicionar Serviço");
    g_signal_connect(adicionarButton, "clicked", G_CALLBACK(adicionar_servico), NULL);
    gtk_grid_attach(GTK_GRID(grid), adicionarButton, 0, 6, 2, 1);
    gtk_widget_set_sensitive(adicionarButton, FALSE);

    finalizarButton = gtk_button_new_with_label("Limpar");
    g_signal_connect(finalizarButton, "clicked", G_CALLBACK(finalizar_servico), NULL);
    gtk_grid_attach(GTK_GRID(grid), finalizarButton, 0, 7, 2, 1);
    gtk_widget_set_sensitive(finalizarButton, FALSE);
    
     GtkWidget *abrirServicosButton = gtk_button_new_with_label("Listar Serviços");
    g_signal_connect(abrirServicosButton, "clicked", G_CALLBACK(abrir_servicos), NULL);
    gtk_grid_attach(GTK_GRID(grid), abrirServicosButton, 0, 8, 2, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
