#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

struct Node {
    char symbol;
    struct Node* left;
    struct Node* right;
    Node(char symbol = 0, Node* left = nullptr, Node* right = nullptr) {
        this->symbol = symbol;
        this->left = left;
        this->right = right;
    }
    ~Node() {
        delete left;
        delete right;
        left = nullptr;
        right = nullptr;
    }
};



int encode(const char* filepath, const char* filepath_new);
int decode(const char* filepath, const char* filepath_new);

int main() {
    encode("source.txt", "encode.txt");
    return 0;
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
            arr_purpose[j] = temp_purpose;
            i++;
            j--;
        }
    }
    temp_purpose = nullptr;
    quick_sort(arr, arr_purpose, l, j);
    quick_sort(arr, arr_purpose, i, r);
}

// Сортировка Хоара
void Hoar_sort(size_t* arr_sorting, Node** arr_purpose, int length) {
    if (length) {
        quick_sort(arr_sorting, arr_purpose, 0, length - 1);
    }
}

void fill_Tab_code(Node* ptr, char* code, char** Tab_code) {
    bool _is_not_symbol = ptr->left || ptr->right; // Символы - листья дерева, то есть ptr->left == ptr->right == nullptr
    char* s = code;
    int i = 0;
    if (_is_not_symbol) {
        if (ptr->left) {
            for (s = code; *s; s++); // Поиск конца кода
            *s = '0';
            s++;
            *s = '\0';
            fill_Tab_code(ptr->left, code, Tab_code);
        }
        if (ptr->right) {
            for (s = code; *s; s++); // Поиск конца кода
            *s = '1';
            s++;
            *s = '\0';
            fill_Tab_code(ptr->right, code, Tab_code);
        }
        for (s = code; *s; s++); // Поиск конца кода
        if (s != code) {
            s--;
        }
        *s = '\0'; // Уменьшение кода вверх по дереву
    }
    else {
        if (!*code) { // Если корень и есть символ
            *s = '0';
            s++;
            *s = '\0';
        }
        Tab_code[(unsigned char)ptr->symbol] = new char[strlen(code) + 1];
        for (i = 0; code[i]; i++, s++) {
            Tab_code[(unsigned char)ptr->symbol][i] = code[i];
        }
        Tab_code[(unsigned char)ptr->symbol][i] = '\0';
        if (s != code) {
            s--;
        }
        *s = '\0'; // Уменьшение кода вверх по дереву
    }
    return;
}

int encode(const char* filepath, const char* filepath_new) {
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
            Tab[(unsigned char)_symbol]++;
        }
    }
    if (fseek(file, 0, SEEK_SET)) {
        flag = -2;
    }

    for (int i = 0; i < 256; i++) {
        if (Tab[i] != 0) {
            queue_length++;
        }
    }
    // if (queue_length == 0){
    //      return 0; // Выход
    // }

    // Создание и заполнение очереди
    Node** queue = new Node*[queue_length];
    for (int i = 0; i < queue_length; i++) {
        queue[i] = new Node;
    }
    size_t* queue_frequency = new size_t[queue_length];
    for (int i = 0, j = 0; i < 256 && j < queue_length; i++) {
        if (Tab[i] != 0) {
            queue[j]->symbol = i;
            queue_frequency[j] = Tab[i];
            j++;
        }
    }
    Hoar_sort(queue_frequency, queue, queue_length);
    
    //Построение дерева
    int j = 0;
    int k = 0;
    while (j != queue_length - 1) {
        ptr_node = new Node(0, queue[j], queue[j + 1]);
        if (!ptr_node) {
            //error
        }
        ptr_node_frequency = queue_frequency[j] + queue_frequency[j + 1];
        queue[j + 1] = ptr_node;
        queue_frequency[j + 1] = ptr_node_frequency;
        queue[j] = nullptr;
        // Обновление очереди с учетом частоты нового элемента (условного символа)
        for (int k = j + 1; k < queue_length && queue_frequency[k] > queue_frequency[k + 1]; k++) {
            ptr_node_frequency = queue_frequency[k];
            queue_frequency[k] = queue_frequency[k + 1];
            queue_frequency[k + 1] = ptr_node_frequency;
            ptr_node = queue[k];
            queue[k] = queue[k + 1];
            queue[k + 1] = ptr_node;
        }
        j++;
    }

    ptr_node = queue[j]; // Корень дерева
    queue[j] = nullptr;


    // Заполнение таблицы кодов Tab_code
    char* Tab_code[256] = { 0 };
    char code[256] = { 0 };
    fill_Tab_code(ptr_node, code, Tab_code);


    // Запись таблицы Tab в файл
    if (fprintf(file_new, "%c", (unsigned char)queue_length - 1) < 0) { // Записываем длину таблицы - 1, т.к. если длина таблицы = 0, то будет пустой файл и выйдем по коду выше
        flag = -2;
    }
    for (int i = 0; i < 256; i++) {
        if (Tab[i]) {
            if (fprintf(file_new, "%c%zu", (char)i, Tab[i]) < 0) {
                flag = -2;
            }
        }
    }
    //if (fprintf(file_new, "%c", '\n') < 0) {
    //    flag = -2;
    //}
    long ptr_file_new = ftell(file_new);


    // Кодировние
    unsigned char byte = 0;
    unsigned char mask = 16; // 0001 0000 // Чтобы записать длину хвоста перед самим закодированным сообщением
    char* ptr_str = nullptr;
    unsigned char tail_length = 0; // Длина хвоста в конце файла
    flag = 1; // Временно
    while (flag > 0) {
        flag = fscanf(file, "%c", &_symbol);
        if (flag > 0) {
            for (ptr_str = Tab_code[(unsigned char) _symbol]; *ptr_str; ptr_str++) {
                if (tail_length == 8) {
                    if (fprintf(file_new, "%c", byte) < 0) {
                        flag = -2;
                    }
                    byte = 0;
                    mask = -1;
                    tail_length = 0;
                }
                if (*ptr_str == '0') {
                    byte = byte & ~mask;
                }
                else {
                    byte = byte | mask;
                }
                mask = mask >> 1;
                tail_length++;
            }
        }
    }
    if (tail_length != 0 && tail_length != 8) {
        if (fprintf(file_new, "%c", byte) < 0) { // Записываем округленный до целого байт
            flag = -2;
        }
        // Записываем сколько лишних символов в хвосте байта в начало файла
        if (fseek(file_new, ptr_file_new, SEEK_SET)) { // Сбрасываемся на начало после таблицы частот, но перед кодированным сообщением (3 бита)
            flag = -2;
        }
        flag = fscanf(file_new, "%c", &byte);
        tail_length = tail_length << 5; // В первые 3 бита информация
        byte = byte | tail_length;
        if (fprintf(file_new, "%c", byte) < 0) { // Записываем длину хвоста
            flag = -2;
        }
    }

    delete ptr_node;
    for (int i = 0; i < queue_length; i++) {
        delete queue[i];
    }
    delete[] queue;
    delete[] queue_frequency;


    fclose(file_new);
    fclose(file);
    return 0; // Временно
}

int decode(const char* filepath, const char* filepath_new) {
    return 0;
}