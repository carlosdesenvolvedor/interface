#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
    GtkWidget *treeview_produtos;
    GtkListStore *list_store;
    GtkWidget *entry_nome;
    GtkWidget *entry_valor;
    GtkWidget *entry_quantidade;
    GtkWidget *label_valor_total;
    double valor_total;
} Venda;
void salvar_produto(GtkWidget *widget, gpointer data) {
    Venda *venda_data = (Venda *)data;

    const gchar *nome = gtk_entry_get_text(GTK_ENTRY(venda_data->entry_nome));
    const gchar *valor_str = gtk_entry_get_text(GTK_ENTRY(venda_data->entry_valor));
    const gchar *quantidade_str = gtk_entry_get_text(GTK_ENTRY(venda_data->entry_quantidade));

    double valor = g_ascii_strtod(valor_str, NULL);
    int quantidade = g_ascii_strtoll(quantidade_str, NULL, 10);

    double subtotal = valor * quantidade;
    venda_data->valor_total += subtotal;

    GtkTreeIter iter;
    gtk_list_store_append(venda_data->list_store, &iter);
    gtk_list_store_set(venda_data->list_store, &iter, 0, nome, 1, valor, 2, quantidade, 3, subtotal, 4, "data", -1);

    gtk_label_set_text(GTK_LABEL(venda_data->label_valor_total), g_strdup_printf("Valor da Venda: R$ %.2f", venda_data->valor_total));

    gtk_entry_set_text(GTK_ENTRY(venda_data->entry_nome), "");
    gtk_entry_set_text(GTK_ENTRY(venda_data->entry_valor), "");
    gtk_entry_set_text(GTK_ENTRY(venda_data->entry_quantidade), "");
}



void nova_venda(GtkWidget *widget, gpointer data) {
    Venda *venda_data = (Venda *)data;

    gtk_list_store_clear(venda_data->list_store);
    gtk_label_set_text(GTK_LABEL(venda_data->label_valor_total), "Valor da Venda: R$ 0.00");
    venda_data->valor_total = 0.0;

    gtk_entry_set_text(GTK_ENTRY(venda_data->entry_nome), "");
    gtk_entry_set_text(GTK_ENTRY(venda_data->entry_valor), "");
    gtk_entry_set_text(GTK_ENTRY(venda_data->entry_quantidade), "");
}

void fechar_pedido(GtkWidget *widget, gpointer data) {
    Venda *venda = (Venda *)data;

    if (venda->valor_total == 0) {
        // Exibe uma mensagem de erro se o valor total da venda for zero
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(venda->treeview_produtos),
                                                   GTK_DIALOG_DESTROY_WITH_PARENT,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Nenhum produto adicionado à venda.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        // Abre o arquivo venda.txt para salvar o pedido
        FILE *file = fopen("venda.txt", "a");
        if (file != NULL) {
            // Obtém a data e hora atual
            time_t current_time = time(NULL);
            struct tm *local_time = localtime(&current_time);
            char datetime_str[20];
            strftime(datetime_str, sizeof(datetime_str), "%d/%m/%Y %H:%M:%S", local_time);

            // Escreve o cabeçalho do pedido
            fprintf(file, "========== Pedido ==========\n");
            fprintf(file, "Data e Hora: %s\n", datetime_str);
            fprintf(file, "============================\n");

            // Obtém a quantidade de produtos na lista
            GtkTreeModel *model = GTK_TREE_MODEL(venda->list_store);
            gint num_produto;
            gtk_tree_model_get_iter_first(model, &num_produto);
            gint count = 0;
            while (num_produto) {
                count++;
                gtk_tree_model_iter_next(model, &num_produto);
            }

            // Escreve os detalhes do pedido
            fprintf(file, "Quantidade de Produtos: %d\n", count);
            fprintf(file, "Valor Total: R$ %.2f\n", venda->valor_total);
            fprintf(file, "-------------------------\n");

            // Escreve os produtos do pedido
            GtkTreeIter iter;
            gboolean valid = gtk_tree_model_get_iter_first(model, &iter);
            while (valid) {
                gchar *nome;
                gchar *valor_str;
                gchar *quantidade_str;
                gdouble subtotal;
                gchar *date_str;
                gtk_tree_model_get(model, &iter,
                                   0, &nome,
                                   1, &valor_str,
                                   2, &quantidade_str,
                                   3, &subtotal,
                                   4, &date_str,
                                   -1);

                fprintf(file, "Produto: %s\n", nome);
                fprintf(file, "Valor: R$ %s\n", valor_str);
                fprintf(file, "Quantidade: %s\n", quantidade_str);
                fprintf(file, "Subtotal: R$ %.2f\n", subtotal);
                fprintf(file, "Data: %s\n", date_str);
                fprintf(file, "-------------------------\n");

                g_free(nome);
                g_free(valor_str);
                g_free(quantidade_str);
                g_free(date_str);

                valid = gtk_tree_model_iter_next(model, &iter);
            }

            fprintf(file, "\n");

            // Fecha o arquivo
            fclose(file);

            // Exibe uma mensagem de sucesso
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(venda->treeview_produtos),
                                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                                       GTK_MESSAGE_INFO,
                                                       GTK_BUTTONS_OK,
                                                       "Pedido fechado com sucesso!\nDetalhes salvos no arquivo venda.txt.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);

            // Limpa a lista de produtos e reinicia os valores
            gtk_list_store_clear(venda->list_store);
            venda->valor_total = 0;
            gtk_label_set_text(GTK_LABEL(venda->label_valor_total), "Valor da Venda: R$ 0.00");
        } else {
            // Exibe uma mensagem de erro se não foi possível abrir o arquivo
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(venda->treeview_produtos),
                                                       GTK_DIALOG_DESTROY_WITH_PARENT,
                                                       GTK_MESSAGE_ERROR,
                                                       GTK_BUTTONS_OK,
                                                       "Erro ao abrir o arquivo venda.txt.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    }
}






void salvar_produto(GtkWidget *widget, gpointer data) {
    Venda *venda = (Venda *)data;

    // Obtém os valores dos campos de entrada
    const gchar *nome = gtk_entry_get_text(GTK_ENTRY(venda->entry_nome));
    const gchar *valor_str = gtk_entry_get_text(GTK_ENTRY(venda->entry_valor));
    const gchar *quantidade_str = gtk_entry_get_text(GTK_ENTRY(venda->entry_quantidade));

    // Converte os valores para double
    gdouble valor = g_ascii_strtod(valor_str, NULL);
    gdouble quantidade = g_ascii_strtod(quantidade_str, NULL);

    // Calcula o subtotal do produto
    gdouble subtotal = valor * quantidade;

    // Atualiza o valor total da venda
    venda->valor_total += subtotal;

    // Converte o valor total para string formatada
    gchar *valor_total_str = g_strdup_printf("Valor da Venda: R$ %.2f", venda->valor_total);

    // Atualiza o rótulo com o valor total da venda
    gtk_label_set_text(GTK_LABEL(venda->label_valor_total), valor_total_str);

    // Insere os dados na lista de produtos
    GtkTreeIter iter;
    gtk_list_store_append(venda->list_store, &iter);
    gtk_list_store_set(venda->list_store, &iter, 0, nome, 1, valor_str, 2, quantidade_str, 3, subtotal, -1);

    // Limpa os campos de entrada
    gtk_entry_set_text(GTK_ENTRY(venda->entry_nome), "");
    gtk_entry_set_text(GTK_ENTRY(venda->entry_valor), "");
    gtk_entry_set_text(GTK_ENTRY(venda->entry_quantidade), "");

    // Libera a memória alocada
    g_free(valor_total_str);
}

void nova_venda(GtkWidget *widget, gpointer data) {
    Venda *venda = (Venda *)data;

    // Limpa a lista de produtos e reinicia os valores
    gtk_list_store_clear(venda->list_store);
    venda->valor_total = 0;
    gtk_label_set_text(GTK_LABEL(venda->label_valor_total), "Valor da Venda: R$ 0.00");
}

void fechar_pedido(GtkWidget *widget, gpointer data) {
    // Lógica para fechar o pedido
    // ...
}

int main(int argc, char *argv[]) {
    // Inicializa o GTK
    gtk_init(&argc, &argv);

    // Cria a janela principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Venda de Produtos");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Cria uma caixa vertical para organizar os elementos
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Cria a árvore de visualização dos produtos
    GtkWidget *treeview_produtos = gtk_tree_view_new();
    GtkListStore *list_store = gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_DOUBLE, G_TYPE_STRING);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_produtos), GTK_TREE_MODEL(list_store));
    g_object_unref(list_store);

    // Cria as colunas da árvore de visualização
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("Nome", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_produtos), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Valor", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_produtos), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Quantidade", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_produtos), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Subtotal", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_produtos), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Data", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_produtos), column);

    // Adiciona a árvore de visualização à caixa vertical
    gtk_box_pack_start(GTK_BOX(vbox), treeview_produtos, TRUE, TRUE, 0);

    // Cria uma caixa horizontal para os campos de entrada
    GtkWidget *hbox_campos = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_campos, FALSE, FALSE, 0);

    // Cria os campos de entrada
    GtkWidget *entry_nome = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_nome), "Nome do Produto");
    GtkWidget *entry_valor = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_valor), "Valor do Produto");
    GtkWidget *entry_quantidade = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_quantidade), "Quantidade");

    // Adiciona os campos de entrada à caixa horizontal
    gtk_box_pack_start(GTK_BOX(hbox_campos), entry_nome, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_campos), entry_valor, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_campos), entry_quantidade, TRUE, TRUE, 0);

    // Cria um botão "Adicionar Produto"
    GtkWidget *button_adicionar = gtk_button_new_with_label("Adicionar Produto");
    Venda venda_data;
    venda_data.treeview_produtos = treeview_produtos;
    venda_data.list_store = list_store;
    venda_data.entry_nome = entry_nome;
    venda_data.entry_valor = entry_valor;
    venda_data.entry_quantidade = entry_quantidade;
    venda_data.label_valor_total = gtk_label_new("Valor da Venda: R$ 0.00");
    venda_data.valor_total = 0;
    g_signal_connect(button_adicionar, "clicked", G_CALLBACK(salvar_produto), &venda_data);
    gtk_box_pack_start(GTK_BOX(vbox), button_adicionar, FALSE, FALSE, 0);

    // Cria o rótulo para exibir o valor total da venda
    gtk_box_pack_start(GTK_BOX(vbox), venda_data.label_valor_total, FALSE, FALSE, 0);

    // Cria uma caixa horizontal para os botões de ação
    GtkWidget *hbox_botoes = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_botoes, FALSE, FALSE, 0);

    // Cria um botão "Fechar Pedido"
    GtkWidget *button_fechar = gtk_button_new_with_label("Fechar Pedido");
    g_signal_connect(button_fechar, "clicked", G_CALLBACK(fechar_pedido), &venda_data);
    gtk_box_pack_start(GTK_BOX(hbox_botoes), button_fechar, TRUE, TRUE, 0);

    // Cria um botão "Nova Venda"
    GtkWidget *button_nova_venda = gtk_button_new_with_label("Nova Venda");
    g_signal_connect(button_nova_venda, "clicked", G_CALLBACK(nova_venda), &venda_data);
    gtk_box_pack_start(GTK_BOX(hbox_botoes), button_nova_venda, TRUE, TRUE, 0);

    // Exibe a janela principal e inicia o loop de eventos do GTK
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
//fim do código