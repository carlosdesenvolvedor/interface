#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

// Estrutura de um nó da árvore AVL
typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

Node *root = NULL;

// Função para obter a altura de um nó
int getHeight(Node *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Função para calcular o fator de balanceamento de um nó
int getBalanceFactor(Node *node) {
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Função para atualizar a altura de um nó
void updateHeight(Node *node) {
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

// Função para criar um novo nó
Node *createNode(int key) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

// Função para fazer uma rotação simples à esquerda
Node *leftRotate(Node *node) {
    Node *newRoot = node->right;
    Node *temp = newRoot->left;

    newRoot->left = node;
    node->right = temp;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

// Função para fazer uma rotação simples à direita
Node *rightRotate(Node *node) {
    Node *newRoot = node->left;
    Node *temp = newRoot->right;

    newRoot->right = node;
    node->left = temp;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

// Função para fazer o balanceamento de um nó
Node *balanceNode(Node *node) {
    updateHeight(node);

    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1) {
        if (getBalanceFactor(node->left) < 0)
            node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balanceFactor < -1) {
        if (getBalanceFactor(node->right) > 0)
            node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Função para inserir um nó na árvore AVL
Node *insertNode(Node *node, int key) {
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insertNode(node->left, key);
    else if (key > node->key)
        node->right = insertNode(node->right, key);
    else
        return node;  // Não permitir chaves duplicadas

    return balanceNode(node);
}

// Função para encontrar o nó mínimo na árvore
Node *findMinNode(Node *node) {
    if (node == NULL || node->left == NULL)
        return node;
    return findMinNode(node->left);
}

// Função para remover um nó da árvore AVL
Node *removeNode(Node *node, int key) {
    if (node == NULL)
        return node;

    if (key < node->key)
        node->left = removeNode(node->left, key);
    else if (key > node->key)
        node->right = removeNode(node->right, key);
    else {
        if (node->left == NULL || node->right == NULL) {
            Node *temp = node->left ? node->left : node->right;
            if (temp == NULL) {
                temp = node;
                node = NULL;
            } else {
                *node = *temp;
            }
            free(temp);
        } else {
            Node *minNode = findMinNode(node->right);
            node->key = minNode->key;
            node->right = removeNode(node->right, minNode->key);
        }
    }

    if (node == NULL)
        return node;

    return balanceNode(node);
}

// Função para percorrer a árvore AVL em ordem (esquerda, raiz, direita) e armazenar os valores em um buffer
void inorderTraversal(Node *node, char *buffer, int *index, int depth, gboolean isRoot, int childPosition) {
    if (node != NULL) {
        inorderTraversal(node->left, buffer, index, depth + 1, FALSE, -1);

        int childOffset = 4;
        if (childPosition == 1) {
            childOffset = 8;
        } else if (childPosition == 2) {
            childOffset = 0;
        }

        if (isRoot) {
            *index += sprintf(&buffer[*index], "%*s[%d] (Pai: -, Filhos: %d, %d, Altura: %d)\n",
                              depth * 12 + childOffset, "", node->key,
                              node->left != NULL ? node->left->key : -1,
                              node->right != NULL ? node->right->key : -1, node->height);
        } else {
            if (node->left != NULL && node->right != NULL) {
                *index += sprintf(&buffer[*index], "%*s%d (Pai: %d, Filhos: %d, %d, Altura: %d)\n",
                                  depth * 12 + childOffset, "", node->key, node->key,
                                  node->left->key, node->right->key, node->height);
            } else if (node->left != NULL) {
                *index += sprintf(&buffer[*index], "%*s%d (Pai: %d, Filhos: %d, -, Altura: %d)\n",
                                  depth * 12 + childOffset, "", node->key, node->key, node->left->key, node->height);
            } else if (node->right != NULL) {
                *index += sprintf(&buffer[*index], "%*s%d (Pai: %d, Filhos: -, %d, Altura: %d)\n",
                                  depth * 12 + childOffset, "", node->key, node->key, node->right->key, node->height);
            } else {
                *index += sprintf(&buffer[*index], "%*s%d (Pai: -, Filhos: -, -, Altura: %d)\n",
                                  depth * 12 + childOffset, "", node->key, node->height);
            }
        }

        inorderTraversal(node->right, buffer, index, depth + 1, FALSE, 1);
    }
}
// Função para atualizar o texto da label com a árvore AVL
void updateTreeLabel(GtkLabel *label, Node *root) {
    char buffer[1000];
    int index = 0;
    sprintf(buffer, "Árvore AVL:\n");
    inorderTraversal(root, buffer, &index, 0, TRUE, -1);


    gtk_label_set_text(label, buffer);
}

// Função de callback para o botão "Inserir"
void on_insert_button_clicked(GtkButton *button, gpointer user_data) {
    int key = atoi(gtk_entry_get_text(GTK_ENTRY(user_data)));
    root = insertNode(root, key);
    gtk_entry_set_text(GTK_ENTRY(user_data), "");

    GtkWidget *label_tree = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "label_tree"));
    updateTreeLabel(GTK_LABEL(label_tree), root);
}

// Função de callback para o botão "Remover"
void on_remove_button_clicked(GtkButton *button, gpointer user_data) {
    int key = atoi(gtk_entry_get_text(GTK_ENTRY(user_data)));
    root = removeNode(root, key);
    gtk_entry_set_text(GTK_ENTRY(user_data), "");

    GtkWidget *label_tree = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "label_tree"));
    updateTreeLabel(GTK_LABEL(label_tree), root);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *entry_key;
    GtkWidget *button_insert;
    GtkWidget *button_remove;
    GtkWidget *label_tree;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Árvore AVL - Interface");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry_key = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_key), "Digite uma chave");
    gtk_grid_attach(GTK_GRID(grid), entry_key, 0, 0, 1, 1);

    button_insert = gtk_button_new_with_label("Inserir");
    g_signal_connect(button_insert, "clicked", G_CALLBACK(on_insert_button_clicked), entry_key);
    gtk_grid_attach(GTK_GRID(grid), button_insert, 1, 0, 1, 1);

    button_remove = gtk_button_new_with_label("Remover");
    g_signal_connect(button_remove, "clicked", G_CALLBACK(on_remove_button_clicked), entry_key);
    gtk_grid_attach(GTK_GRID(grid), button_remove, 0, 1, 1, 1);

    label_tree = gtk_label_new(NULL);
    updateTreeLabel(GTK_LABEL(label_tree), root);
    gtk_grid_attach(GTK_GRID(grid), label_tree, 0, 2, 2, 1);
    g_object_set_data(G_OBJECT(button_insert), "label_tree", label_tree);
    g_object_set_data(G_OBJECT(button_remove), "label_tree", label_tree);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
