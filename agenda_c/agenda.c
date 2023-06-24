#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

// Estrutura de dados para o contato
typedef struct Contato {
    char nome[50];
    char telefone[20];
    struct Contato* esquerda;
    struct Contato* direita;
} Contato;

// Variável global para a raiz da árvore binária
Contato* raiz = NULL;

// Função para criar um novo contato
Contato* criarContato(const char* nome, const char* telefone) {
    Contato* novoContato = malloc(sizeof(Contato));
    strcpy(novoContato->nome, nome);
    strcpy(novoContato->telefone, telefone);
    novoContato->esquerda = NULL;
    novoContato->direita = NULL;
    return novoContato;
}

// Função para inserir um contato na árvore
void inserirContato(Contato* contato) {
    if (raiz == NULL) {
        raiz = contato;
        return;
    }

    Contato* atual = raiz;
    Contato* pai = NULL;

    while (1) {
        pai = atual;

        if (strcmp(contato->nome, atual->nome) < 0) {
            atual = atual->esquerda;
            if (atual == NULL) {
                pai->esquerda = contato;
                return;
            }
        } else {
            atual = atual->direita;
            if (atual == NULL) {
                pai->direita = contato;
                return;
            }
        }
    }
}

// Função para percorrer a árvore em ordem (exibição na interface gráfica)
void percorrerArvoreEmOrdem(Contato* contato, GtkTextBuffer* buffer) {
    if (contato != NULL) {
        percorrerArvoreEmOrdem(contato->esquerda, buffer);
        gchar* line = g_strdup_printf("Nome: %s, Telefone: %s\n", contato->nome, contato->telefone);
        gtk_text_buffer_insert_at_cursor(buffer, line, -1);
        g_free(line);
        percorrerArvoreEmOrdem(contato->direita, buffer);
    }
}

// Função de callback para o botão "Cadastrar"
void cadastrarContato(GtkWidget* widget, gpointer data) {
    GtkEntry* nomeEntry = GTK_ENTRY(data);
    GtkEntry* telefoneEntry = GTK_ENTRY(g_object_get_data(G_OBJECT(data), "telefoneEntry"));
    GtkTextBuffer* buffer = GTK_TEXT_BUFFER(g_object_get_data(G_OBJECT(data), "buffer"));

    const gchar* nome = gtk_entry_get_text(nomeEntry);
    const gchar* telefone = gtk_entry_get_text(telefoneEntry);

    Contato* novoContato = criarContato(nome, telefone);
    inserirContato(novoContato);

    gchar* line = g_strdup_printf("Contato cadastrado: Nome: %s, Telefone: %s\n", nome, telefone);
    gtk_text_buffer_insert_at_cursor(buffer, line, -1);
    g_free(line);
}

// Função de callback para o botão "Imprimir Árvore"
void imprimirArvore(GtkWidget* widget, gpointer data) {
    GtkTextBuffer* buffer = GTK_TEXT_BUFFER(data);
    gtk_text_buffer_set_text(buffer, "", -1);
    percorrerArvoreEmOrdem(raiz, buffer);
}

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    // Criação da janela principal e outros elementos da interface
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Agenda Telefônica");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget* vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget* nomeLabel = gtk_label_new("Nome:");
    gtk_box_pack_start(GTK_BOX(vbox), nomeLabel, FALSE, FALSE, 0);

    GtkEntry* nomeEntry = GTK_ENTRY(gtk_entry_new());
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(nomeEntry), FALSE, FALSE, 0);

    GtkWidget* telefoneLabel = gtk_label_new("Telefone:");
    gtk_box_pack_start(GTK_BOX(vbox), telefoneLabel, FALSE, FALSE, 0);

    GtkEntry* telefoneEntry = GTK_ENTRY(gtk_entry_new());
    gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(telefoneEntry), FALSE, FALSE, 0);

    GtkWidget* cadastrarButton = gtk_button_new_with_label("Cadastrar");
    gtk_box_pack_start(GTK_BOX(vbox), cadastrarButton, FALSE, FALSE, 0);
    g_signal_connect(cadastrarButton, "clicked", G_CALLBACK(cadastrarContato), nomeEntry);

    GtkWidget* imprimirButton = gtk_button_new_with_label("Imprimir Árvore");
    gtk_box_pack_start(GTK_BOX(vbox), imprimirButton, FALSE, FALSE, 0);

    GtkTextBuffer* buffer = gtk_text_buffer_new(NULL);
    GtkWidget* textView = gtk_text_view_new_with_buffer(buffer);
    gtk_box_pack_start(GTK_BOX(vbox), textView, TRUE, TRUE, 0);

    g_signal_connect(imprimirButton, "clicked", G_CALLBACK(imprimirArvore), buffer);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
