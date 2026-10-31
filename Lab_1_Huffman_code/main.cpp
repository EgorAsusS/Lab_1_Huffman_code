#include <iostream>

struct Node {
    char symbol;
    unsigned char* code;
    struct Node* left;
    struct Node* right;
};

Node* create_Node(Node*& root, char symbol, Node* left = nullptr, Node* right = nullptr) {
    Node* ptr_node = new Node;
    if (!ptr_node) {
        return nullptr;
    }
    ptr_node->symbol = symbol;
    ptr_node->left = left;
    ptr_node->right = right;
    ptr_node->code = nullptr;
    root = ptr_node;
    return ptr_node;
}

int encode(char* filepath, char* filepath_new);
int decode(char* filepath, char* filepath_new);

int main() {
    return 0;
}

int encode(char* filepath, char* filepath_new) {
    if (!filepath || !filepath_new) {
        return 0;
    }
    FILE* file = fopen(filepath, "r");
    FILE* file_new = NULL;
    if (!file) {
        return 0;
    }
    file_new = fopen(filepath_new, "w");
    char _symbol = 0;
    int flag = 1;
    int queue_length = 0;
    int j = 0;
    Node* ptr_node = nullptr;
    size_t ptr_node_frequency = 0;
    size_t Tab[256] = { 0 };
    if (!file_new) {
        return 0;
    }
    //if (fprintf(file_new, "%c", flag_tail) < 0) {
    //    flag = -2;
    //}
    while (flag > 0) {
        flag = fscanf(file, "%c", &_symbol);
        if (flag > 0) {
            Tab[_symbol]++;
        }
    }
    for (int i = 0; i < 256; i++) {
        if (Tab[i] != 0) {
            queue_length++;
        }
    }

    // Создание и заполнение очереди
    Node** queue = new Node*[queue_length];
    size_t* queue_frequency = new size_t[queue_length];
    for (int i = 0, j = 0; i < 256; i++) {
        if (Tab[i] != 0) {
            queue[j]->symbol = i;
            queue_frequency[j] = Tab[i];
        }
    }
    // Косвенная сортировка очереди по возрастанию частоты символов
    Hoar_sort(queue_frequency, queue, queue_length);

    //Построение дерева
    while (j != queue_length - 1) {
        create_Node(ptr_node, 0, queue[j], queue[j + 1]);
        if (!ptr_node) {
            //error
        }
        ptr_node_frequency = queue_frequency[j] + queue_frequency[j + 1];
        queue[j + 1] = ptr_node;
        queue_frequency[j + 1] = ptr_node_frequency;
        queue[j] = nullptr;
        j++;
    }

    ptr_node = queue[j]; // Корень дерева

    return 0; // Временно
}

int decode(char* filepath, char* filepath_new) {
}

// Алгоритм сортировки Хоара для работы рекурсии
void quick_sort(size_t* arr, Node** arr_purpose, int l, int r) {
    if (arr == nullptr || arr_purpose == nullptr) {
        return;
    }
    if (l >= r) {
        return;
    }
    int i = l;
    int j = r;
    int x = arr[(l + r) / 2];
    size_t temp = 0;
    Node* temp_purpose;
    while (i <= j) {
        for (; arr[i] < x; i++);
        for (; arr[j] > x; j--);
        if (i <= j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            temp_purpose = arr_purpose[i];
            arr_purpose[i] = arr_purpose[j];
            arr_purpose[i] = temp_purpose;
            i++;
            j--;
        }
    }
    temp_purpose = nullptr;
    quick_sort(arr, arr_purpose, l, j);
    quick_sort(arr, arr_purpose, i, r);
}

// Сортировка Хоара
void Hoar_sort(size_t* arr_sorting, Node** arr_purpose, size_t length) {
    quick_sort(arr_sorting, arr_purpose, 0, length - 1);
}