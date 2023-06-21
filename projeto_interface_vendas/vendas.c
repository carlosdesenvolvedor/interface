#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>

typedef struct {
    GtkWidget *entry_nome;
    GtkWidget *entry_valor;
    GtkWidget *entry_quantidade;
    GtkWidget *combo_forma_pagamento;
    GtkWidget *label_valor_total;
     GtkWidget *label_data;
    GtkListStore *list_store;
    GtkWidget *treeview_produtos;
     char data[20];
    float valor;
    double valor_total;
} VendaData;
//alguns prototicpos
const char* get_selected_payment_option(VendaData *venda);
char* get_current_date();
void fechamento_do_dia(GtkWidget *widget, VendaData *venda);


char* get_current_date() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char *date = malloc(sizeof(char) * 11);
    sprintf(date, "%02d/%02d/%04d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
    return date;
}
const gchar* get_selected_payment_option(VendaData *venda) {
    gint active = gtk_combo_box_get_active(GTK_COMBO_BOX(venda->combo_forma_pagamento));
    if (active != -1) {
        GtkTreeIter iter;
        GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(venda->combo_forma_pagamento));
        if (gtk_tree_model_iter_nth_child(model, &iter, NULL, active)) {
            gchar *option;
            gtk_tree_model_get(model, &iter, 0, &option, -1);

            gchar *selected_option = g_strdup(option);  // Aloca uma cópia da string no heap
            g_free(option);  // Libera a string original alocada pela GtkTreeView

            return selected_option;
        }
    }
    return NULL;
}



void salvar_produto(GtkWidget *widget, VendaData *venda) {
    const char *nome = gtk_entry_get_text(GTK_ENTRY(venda->entry_nome));
    const char *valor_str = gtk_entry_get_text(GTK_ENTRY(venda->entry_valor));
    double valor = atof(valor_str);
    const char *quantidade_str = gtk_entry_get_text(GTK_ENTRY(venda->entry_quantidade));
    int quantidade = atoi(quantidade_str);
    double subtotal = valor * quantidade;

    // Adiciona o produto na TreeView
    GtkTreeIter iter;
    gtk_list_store_append(venda->list_store, &iter);
    gtk_list_store_set(venda->list_store, &iter, 0, nome, 1, valor, 2, quantidade, 3, subtotal, -1);

    // Atualiza o valor total da venda
    venda->valor_total += subtotal;

    // Atualiza o rótulo com o valor total da venda
    char valor_total_str[50];
    sprintf(valor_total_str, "Valor da Venda: R$ %.2f", venda->valor_total);
    gtk_label_set_text(GTK_LABEL(venda->label_valor_total), valor_total_str);

    // Limpa os campos de entrada
    gtk_entry_set_text(GTK_ENTRY(venda->entry_nome), "");
    gtk_entry_set_text(GTK_ENTRY(venda->entry_valor), "");
    gtk_entry_set_text(GTK_ENTRY(venda->entry_quantidade), "");
}

void fechar_pedido(GtkWidget *widget, VendaData *venda) {
    // Lógica para fechar o pedido e realizar outras operações necessárias

    // Exibe uma mensagem com o valor total da venda
    char mensagem[100];
    sprintf(mensagem, "Pedido fechado! Valor Total: R$ %.2f", venda->valor_total);
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(widget),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               mensagem);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Salva o pedido em um arquivo
    FILE *file = fopen("venda.txt", "a"); // Modo "a" para acrescentar ao arquivo
     // Obtem a data atual
        char *date = get_current_date();
            // Salva a data no arquivo
        fprintf(file, "Data: %s\n", date);

    if (file != NULL) {
        GtkTreeIter iter;
        gboolean valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(venda->list_store), &iter);
        while (valid) {
            gchar *nome;
            gfloat valor, subtotal;
            gint quantidade;
            gtk_tree_model_get(GTK_TREE_MODEL(venda->list_store), &iter, 0, &nome, 1, &valor, 2, &quantidade, 3, &subtotal, -1);
            fprintf(file, "Produto: %s | Valor: %.2f | Quantidade: %d | Subtotal: %.2f\n", nome, valor, quantidade, subtotal);
            g_free(nome);
            valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(venda->list_store), &iter);
        }

       
               // Verifica se é uma nova venda do dia ou uma venda do mesmo dia
        static char last_date[11] = ""; // Variável estática para armazenar a última data
        static double total_sales = 0.0; // Variável estática para armazenar o valor total das vendas do dia
        if (strcmp(date, last_date) != 0) {
            // Nova venda do dia
            strcpy(last_date, date); // Armazena a nova data
            total_sales = venda->valor_total; // Reinicia o valor total das vendas do dia
        } else {
            // Venda do mesmo dia
            total_sales += venda->valor_total; // Acumula o valor total das vendas do dia
        }



        // Salva o valor total no arquivo
        fprintf(file, "Valor Total: %.2f\n", venda->valor_total);
         // Salva o valor total das vendas do dia no arquivo
       

        g_free(date);
        // Obtenha a opção selecionada na forma de pagamento
        const char *forma_pagamento = get_selected_payment_option(venda);

        // Salve a forma de pagamento no arquivo
        fprintf(file, "Forma de Pagamento: %s\n", forma_pagamento);
        fprintf(file, "********* Valor Total do Dia: R$ %.2f ************\n", total_sales);
        fprintf(file,"----------------------------------------------------\n");

        // Libere a memória alocada para a opção selecionada
        g_free((gpointer)forma_pagamento);
        fclose(file);
    }

    // Reinicializa os valores da venda
    venda->valor_total = 0.0;
    gtk_label_set_text(GTK_LABEL(venda->label_valor_total), "Valor da Venda: R$ 0.00");

    // Limpa a TreeView
    gtk_list_store_clear(venda->list_store);
}


void fechamento_do_dia(GtkWidget *widget, VendaData *venda) {
  // Abre o arquivo "venda.txt" em modo de acrescentar ao final
    FILE *file = fopen("venda.txt", "a");
    if (file != NULL) {
    // Obtém a data atual
    char *date = get_current_date();


   // Verifica se é uma nova venda do dia ou uma venda do mesmo dia
        static char last_date[11] = ""; // Variável estática para armazenar a última data
        static double total_sales = 0.0; // Variável estática para armazenar o valor total das vendas do dia
        VendaData *venda = malloc(sizeof(VendaData));
        if (strcmp(date, last_date) != 0) {
            // Nova venda do dia
            strcpy(last_date, date); // Armazena a nova data
            total_sales = venda->valor_total; // Reinicia o valor total das vendas do dia
        } else {
            // Venda do mesmo dia
            total_sales += venda->valor_total; // Acumula o valor total das vendas do dia
        }

    // Salva o valor total das vendas do dia no arquivo
   
    fprintf(file, "Valor Total do Dia: R$ %.2f\n", total_sales);

    fclose(file);
    g_free(date);
}
}


 

void nova_venda(GtkWidget *widget, GtkWidget *window) {
    GtkWidget *venda_window;
    GtkWidget *vbox;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *scrolled_window;
    VendaData *venda = g_new(VendaData, 1);

    // Inicializa os va n  lores da venda
    venda->valor_total = 0.0;

    // Cria uma nova janela para a venda
    venda_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(venda_window), "Nova Venda");
    
    gtk_container_set_border_width(GTK_CONTAINER(venda_window), 10);
    gtk_widget_set_size_request(venda_window, 400, 300);
    g_signal_connect(venda_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Cria uma caixa vertical para organizar os widgets
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(venda_window), vbox);

    // Cria um rótulo para mostrar o valor da venda
    venda->label_valor_total = gtk_label_new("Valor da Venda: R$ 0.00");
    gtk_box_pack_start(GTK_BOX(vbox), venda->label_valor_total, FALSE, FALSE, 0);

    // Cria uma TreeView para exibir os produtos da venda
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    // Cria a lista de armazenamento de dados para a TreeView
    venda->list_store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_INT, G_TYPE_FLOAT);

    // Cria a TreeView com quatro colunas (Nome, Valor, Quantidade, Subtotal)
    venda->treeview_produtos = gtk_tree_view_new_with_model(GTK_TREE_MODEL(venda->list_store));
    gtk_container_add(GTK_CONTAINER(scrolled_window), venda->treeview_produtos);

    // Cria as colunas da TreeView
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("Nome", renderer,
                                                                         "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(venda->treeview_produtos), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Valor", renderer,
                                                      "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(venda->treeview_produtos), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Quantidade", renderer,
                                                      "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(venda->treeview_produtos), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Subtotal", renderer,
                                                      "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(venda->treeview_produtos), column);

    // Cria uma caixa horizontal para os campos de entrada
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    // Cria um campo de entrada para o nome do produto
    venda->entry_nome = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(venda->entry_nome), "Nome do Produto");
    gtk_box_pack_start(GTK_BOX(hbox), venda->entry_nome, TRUE, TRUE, 0);

    // Cria um campo de entrada para o valor do produto
    venda->entry_valor = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(venda->entry_valor), "Valor");
    gtk_box_pack_start(GTK_BOX(hbox), venda->entry_valor, TRUE, TRUE, 0);

    // Cria um campo de entrada para a quantidade do produto
    venda->entry_quantidade = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(venda->entry_quantidade), "Quantidade");
    gtk_box_pack_start(GTK_BOX(hbox), venda->entry_quantidade, TRUE, TRUE, 0);

    // Cria um campo de entrada para a forma de pagamento do produto
    venda->combo_forma_pagamento = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(venda->combo_forma_pagamento), "Dinheiro");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(venda->combo_forma_pagamento), "Cartão de Crédito");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(venda->combo_forma_pagamento), "Cartão de Débito");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(venda->combo_forma_pagamento), "PIX");
    gtk_combo_box_set_active(GTK_COMBO_BOX(venda->combo_forma_pagamento), 0);
    gtk_box_pack_start(GTK_BOX(hbox), venda->combo_forma_pagamento, TRUE, TRUE, 0);

    // Cria um botão para adicionar o produto
    button = gtk_button_new_with_label("Adicionar");
    g_signal_connect(button, "clicked", G_CALLBACK(salvar_produto), venda);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    // Cria um botão para fechar o pedido
    button = gtk_button_new_with_label("Fechar Pedido");
    g_signal_connect(button, "clicked", G_CALLBACK(fechar_pedido), venda);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);

    gtk_widget_show_all(venda_window);
    VendaData *venda2 = g_new(VendaData, 1);

    // Inicializa os valores da venda
    venda->valor_total = 0.0;

    // Cria uma nova janela para a venda
    venda_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    venda->label_data = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), venda->label_data, FALSE, FALSE, 0);

    // Define a data atual
    char *date = get_current_date();
    gtk_label_set_text(GTK_LABEL(venda->label_data), date);
    g_free(date);
}



int main(int argc, char *argv[]) {
    // Inicializa o GTK
    gtk_init(&argc, &argv);

    // Cria a janela principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sistema de Vendas");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Cria um grid para organizar os elementos
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Cria um botão para iniciar uma nova venda
    GtkWidget *button_nova_venda = gtk_button_new_with_label("Nova Venda");
    g_signal_connect(button_nova_venda, "clicked", G_CALLBACK(nova_venda), NULL);
    gtk_grid_attach(GTK_GRID(grid), button_nova_venda, 0, 0, 2, 1);

    // Cria um botão para fechamento do dia
    GtkWidget *button_fechamento_dia = gtk_button_new_with_label("Fechar Dia");
    g_signal_connect(button_fechamento_dia, "clicked", G_CALLBACK(fechamento_do_dia), NULL);
    gtk_grid_attach(GTK_GRID(grid), button_fechamento_dia, 0, 1, 2, 1);

    gtk_widget_show_all(window);

    // Executa o loop principal do GTK
    gtk_main();

    return 0;
}