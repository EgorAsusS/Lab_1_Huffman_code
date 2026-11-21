#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#define size 256
using namespace std;

struct Node { // Структура для построения дерева
    char symbol;
    struct Node* left;
    struct Node* right;
    Node(char symbol = 0, Node* left = nullptr, Node* right = nullptr) {
        this->symbol = symbol;
        this->left = left;
        this->right = right;
    }
};

void delete_tree(Node* root);
void Hoar_sort(size_t* arr, Node** arr_purpose, int l, int r);
void Hoar_sort(size_t* arr_sorting, Node** arr_purpose, int length);

int encode(const char* filepath, const char* filepath_new);
int decode(const char* filepath, const char* filepath_new);

int main() {
    int flag = 0;
    char mode = 0;
    char source[size] = { 0 };
    char destination[size] = { 0 };
    while (mode != 'e' && mode != 'd') {
        cout << "Change if you want to encode(e) or decode(d) your file. Input 'e' or 'd': " << endl;
        cin >> mode;
    }
    cin.ignore();
    cout << "Input source's filepath: " << endl;
    cin.getline(source, size);
    cout << "Input destination's filepath: " << endl;
    cin.getline(destination, size);
    if (mode == 'e') {
        flag = encode(source, destination);
    }
    else {
        flag = decode(source, destination);
    }
    if (flag == 1) {
        cout << "successful" << endl;
        return 0;
    }
    else if (flag == -1) {
        cerr << "Error occured while openning file" << endl;
    }
    else if (flag == -2) {
        cerr << "Error occured while closing file" << endl;
    }
    else if (flag == -3) {
        cerr << "Error occured while removing file" << endl;
    }
    else {
        cerr << "Error of reading, writing, moving onto file or other error";
    }
    return 1;
}

void delete_tree(Node* root){
    if (!root) {
        return;
    }
    delete_tree(root->left);
    delete_tree(root->right);
    root->left = nullptr;
    root->right = nullptr;
}

// Алгоритм сортировки Хоара для работы рекурсии
void Hoar_sort(size_t* arr, Node** arr_purpose, int l, int r) {
    if (arr == nullptr || arr_purpose == nullptr) {
        return;
    }
    if (l >= r) {
        return;
    }
    int i = l;
    int j = r;
    size_t x = arr[(l + r) / 2];
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
    Hoar_sort(arr, arr_purpose, l, j);
    Hoar_sort(arr, arr_purpose, i, r);
}

// Сортировка Хоара
void Hoar_sort(size_t* arr_sorting, Node** arr_purpose, int length) {
    if (length) {
        Hoar_sort(arr_sorting, arr_purpose, 0, length - 1);
    }
}


// -1 - Ошибка при выделении памяти через new
// 0 - успешное выпонение
int fill_Tab_code(Node* ptr, char* code, char** Tab_code) {
    char* s = code;
    int i = 0;
    if (!ptr) { // Доплнительная проверка входных данных
        return 0;
    }
    bool _is_not_symbol = ptr->left || ptr->right; // Символы - листья дерева, то есть ptr->left == ptr->right == nullptr
    if (_is_not_symbol) {
        if (ptr->left) {
            for (s = code; *s; s++); // Поиск конца кода
            *s = '0';
            s++;
            *s = '\0';
            if (fill_Tab_code(ptr->left, code, Tab_code)) {
                return -1;
            }
        }
        if (ptr->right) {
            for (s = code; *s; s++); // Поиск конца кода
            *s = '1';
            s++;
            *s = '\0';
            if (fill_Tab_code(ptr->right, code, Tab_code)) {
                return -1;
            }
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
        if (!Tab_code[(unsigned char)ptr->symbol]) {
            return -1;
        }
        for (i = 0; code[i]; i++, s++) {
            Tab_code[(unsigned char)ptr->symbol][i] = code[i];
        }
        Tab_code[(unsigned char)ptr->symbol][i] = '\0';
        if (s != code) {
            s--;
        }
        *s = '\0'; // Уменьшение кода вверх по дереву
    }
    return 0;
}

// 1 - успешное выполнение
// 0 - ошибка при чтении файла, записи файла или перемещении по файлу
// -1 - ошибка при открытии файла
// -2 - ошибка при закрытии файла
// -3 - ошибка при удалении файла
// Коды ошибок 0 - самая слабая, -3 - самая сильная (сильная подавляет слабую при возврате значений из функции)
int encode(const char* filepath, const char* filepath_new) {
    FILE* file = nullptr;
    FILE* file_new = nullptr;
    int flag = 0;
    int queue_length = 0;
    size_t ptr_node_frequency = 0;
    size_t Tab[256] = { 0 };
    char* Tab_code[256] = { 0 };
    char code[256] = { 0 };
    long ptr_file_new = 0;
    Node* ptr_node = nullptr;
    char* ptr_str = nullptr;
    size_t* queue_frequency = nullptr;
    Node** queue = nullptr;
    unsigned char byte = 0;
    unsigned char mask = 0;
    unsigned char tail_length = 0;
    try {
        if (!filepath || !filepath_new) {
            throw 0; // Error with code 0 for this moment
        }
        file = fopen(filepath, "rb");
        if (!file) {
            throw -1; // Error with code -1 for this moment
        }
        file_new = fopen(filepath_new, "wb+");
        if (!file_new) {
            throw -1; // Error with code -1 for this moment
        }
        while (flag >= 0) {
            flag = fgetc(file);
            if (flag >= 0) {
                Tab[(unsigned char)flag]++;
            }
        }
        if (ferror(file)) {
            throw 0; // Error with code 0 for this moment
        }
        if (fseek(file, 0, SEEK_SET)) {
            throw 0; // Error with code 0 for this moment
        }
        for (int i = 0; i < 256; i++) {
            if (Tab[i] != 0) {
                queue_length++;
            }
        }
        if (queue_length == 0){
            throw 1; // Пустой файл - возврат 1 на данный момент, т.к. это успешная ситуация
        }

        // Запись таблицы Tab в файл
        flag = fputc((unsigned char)queue_length - 1, file_new);
        for (int i = 0; i < 256; i++) {
            if (Tab[i]) {
                flag = fputc(i, file_new);
                if (flag < 0) {
                    throw 0; // Error with code 0 for this moment
                }
                flag = fwrite(&Tab[i], sizeof(Tab[i]), 1, file_new);
                if (flag == 0) {
                    throw 0; // Error with code 0 for this moment
                }
            }
        }
        ptr_file_new = ftell(file_new);
        if (ptr_file_new == -1L) {
            throw 0; // Error with code 0 for this moment
        }

        // Создание и заполнение очереди
        queue = new Node* [queue_length];
        if (!queue) {
            throw 0; // Error with code 0 for this moment
        }
        for (int i = 0; i < queue_length; i++) { // Зануление queue
            queue[i] = nullptr;
        }
        for (int i = 0; i < queue_length; i++) {
            queue[i] = new Node;
            if (!queue[i]) {
                throw 0; // Error with code 0 for this moment
            }
        }
        queue_frequency = new size_t[queue_length];
        if (!queue_frequency) {
            throw 0; // Error with code 0 for this moment
        }
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
                throw 0; // Error with code 0 for this moment
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
        if (fill_Tab_code(ptr_node, code, Tab_code)) {
            throw 0; // Error with code 0 for this moment
        }

        // Кодировние
        unsigned char byte = 0;
        unsigned char mask = 16; // 0001 0000 // Чтобы записать длину хвоста перед самим закодированным сообщением
        char* ptr_str = nullptr;
        unsigned char tail_length = 5; // Длина хвоста в конце файла
        flag = 1;
        while (flag >= 0) {
            flag = fgetc(file);
            if (flag >= 0) {
                for (ptr_str = Tab_code[(unsigned char)flag]; *ptr_str; ptr_str++) {
                    if (tail_length == 0) {
                        flag = fputc(byte, file_new);
                        if (flag < 0) { 
                            throw 0; // Error with code 0 for this moment
                        }
                        byte = 0;
                        mask = 128;
                        tail_length = 8;
                    }
                    if (*ptr_str == '0') {
                        byte = byte & ~mask;
                    }
                    else {
                        byte = byte | mask;
                    }
                    mask = mask >> 1;
                    tail_length--;
                }
            }
        }
        if (ferror(file)) { 
            throw 0; // Error with code 0 for this moment
        }
        if (tail_length != 0) {
            flag = fputc(byte, file_new);
            if (flag < 0) { 
                throw 0; // Error with code 0 for this moment
            }
            // Записываем сколько лишних символов в хвосте байта в начало файла
            if (fseek(file_new, ptr_file_new, SEEK_SET)) { // Сбрасываемся на начало после таблицы частот, но перед кодированным сообщением (3 бита)
                throw 0; // Error with code 0 for this moment
            }
            flag = fgetc(file_new);
            if (flag < 0) {
                throw 0; // Error with code 0 for this moment
            }
            byte = (unsigned char)flag;
            tail_length = tail_length << 5; // В первые 3 бита информация
            byte = byte | tail_length;
            if (fseek(file_new, ptr_file_new, SEEK_SET)) { // Сбрасываемся на начало после таблицы частот, но перед кодированным сообщением (3 бита)
                throw 0; // Error with code 0 for this moment
            }
            flag = fputc(byte, file_new);
            if (flag < 0) {
                throw 0; // Error with code 0 for this moment
            }
        }

        if (fclose(file_new) || fclose(file)) {
            throw -2; // Error with code -2 for this moment
        }

        delete_tree(ptr_node);
        delete ptr_node;
        ptr_node = nullptr;
        if (queue) {
            for (int i = 0; i < queue_length; i++) {
                delete queue[i];
            }
        }
        delete[] queue;
        delete[] queue_frequency;

        return 1; // Успешное завершение
    }
    catch (int return_value) {
        if (return_value != -2) {
            if (file) {
                if (fclose(file)) {
                    return_value = -2; // Error with code -2 for this moment
                }
            }
            if (file_new) {
                if (fclose(file_new)) {
                    return_value = -2; // Error with code -2 for this moment
                }
                if (return_value != 1) {
                    if (remove(filepath_new)) {
                        return_value = -3; // Error with code -3 for this moment
                    }
                }
            }
        }


        delete_tree(ptr_node);
        delete ptr_node;
        ptr_node = nullptr;
        if (queue) {
            for (int i = 0; i < queue_length; i++) {
                delete queue[i];
            }
        }
        delete[] queue;
        delete[] queue_frequency;
        
        return return_value; // Если 1 - успешное завершение, иначе код ошибки
    }
    catch (...) {
        int return_value = 0;
        if (file) {
            if (fclose(file)) {
                return_value = -2; // Error with code -2 for this moment
            }
        }
        if (file_new) {
            if (fclose(file_new)) {
                return_value = -2; // Error with code -2 for this moment
            }
            if (remove(filepath_new)) {
                return_value = -3; // Error with code -3 for this moment
            }
        }

        delete_tree(ptr_node);
        delete ptr_node;
        ptr_node = nullptr;
        if (queue) {
            for (int i = 0; i < queue_length; i++) {
                delete queue[i];
            }
        }
        delete[] queue;
        delete[] queue_frequency;

        return return_value; // Код ошибки
    }
}

// 1 - успешное выполнение
// 0 - ошибка при чтении файла, записи файла или перемещении по файлу
// -1 - ошибка при открытии файла
// -2 - ошибка при закрытии файла
// -3 - ошибка при удалении файла
// Коды ошибок 0 - самая слабая, -3 - самая сильная (сильная подавляет слабую при возврате значений из функции)
int decode(const char* filepath, const char* filepath_new) {
    FILE* file = nullptr;
    FILE* file_new = nullptr;
    int flag = 0;
    int flag_end = 0;
    int flag_file_new = 0;
    int shift_tail_length = 0;
    int queue_length = 0;
    size_t ptr_node_frequency = 0;
    size_t Tab[256] = { 0 };
    long ptr_file_new = 0;
    Node* ptr = nullptr;
    Node* ptr_node = nullptr;
    char* ptr_str = nullptr;
    size_t* queue_frequency = nullptr;
    Node** queue = nullptr;
    unsigned char byte = 0;
    unsigned char mask = 0;
    unsigned char tail_length = 0;
    try {
        if (!filepath || !filepath_new) {
            throw 0; // Error with code 0 for this moment
        }
        file = fopen(filepath, "rb");
        if (!file) {
            throw -1; // Error with code -1 for this moment
        }
        file_new = fopen(filepath_new, "wb+");
        if (!file_new) {
            throw -1; // Error with code -1 for this moment
        }

        // Считывание таблицы Tab из файла
        flag = fgetc(file);
        if (flag < 0) {
            throw 1; // Нет таблицы => файл пустой исходный был - возврат 1 на данный момент, т.к. это успешная ситуация
        }
        int Tab_length = flag + 1;
        for (int i = 0; i < Tab_length; i++) {
            flag = fgetc(file);
            if (flag >= 0) {
                flag = fread(&Tab[flag], sizeof(Tab[flag]), 1, file);
                if (flag == 0) { // Ошибка таблица неполная
                    throw 0; // Error with code 0 for this moment
                }
            }
            else { // Ошибка, т.к. только по таблице нельзя декодировать файл (либо таблица неполная)
                throw 0; // Error with code 0 for this moment
            }
        }
        for (int i = 0; i < 256; i++) {
            if (Tab[i] != 0) {
                queue_length++;
            }
        }
        if (queue_length == 0) {
            throw 1; // Пустая таблица предоставлена (исходный файл пустой) - возврат 1 на данный момент, т.к. это успешная ситуация
        }

        // Создание и заполнение очереди
        queue = new Node * [queue_length];
        if (!queue) {
            throw 0; // Error with code 0 for this moment
        }
        for (int i = 0; i < queue_length; i++) { // Зануление queue
            queue[i] = nullptr;
        }
        for (int i = 0; i < queue_length; i++) {
            queue[i] = new Node;
            if (!queue[i]) {
                throw 0; // Error with code 0 for this moment
            }
        }
        queue_frequency = new size_t[queue_length];
        if (!queue_frequency) {
            throw 0; // Error with code 0 for this moment
        }
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
                throw 0; // Error with code 0 for this moment
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

        // Декодирование
        byte = 0;
        mask = 224; // 1110 0000 // Чтобы считать длину хвоста (3 бит)
        tail_length = 5; // Длина хвоста в конце файла (Изначально 5)
        flag = fgetc(file);
        if (flag >= 0) {
            tail_length = flag & mask; // Реальная длина хвоста
            tail_length = tail_length >> 5; // Чтобы получилось это число не в старших битах
        }
        else {
            throw 0; // Error with code 0 for this moment
        }

        //ptr_node - корень
        ptr = ptr_node;
        mask = 16; // 0001 0000 // Чтобы считывать после длины хвоста (после 3 бит)
        flag_file_new = 0;
        flag_end = fgetc(file);
        shift_tail_length = 5;
        if (flag_end >= 0) {
            shift_tail_length = 8;
        }
        while (flag_end >= 0) {
            if (ptr->left == nullptr && ptr->right == nullptr) {
                flag_file_new = fputc(ptr->symbol, file_new);
                if (flag_file_new < 0) {
                    throw 0;
                }
                ptr = ptr_node;
            }
            if (flag & mask) {
                if (ptr->right) { // Корень может быть символом => left == right == nullptr
                    ptr = ptr->right;
                }
            }
            else {
                if (ptr->left) { // Корень может быть символом => left == right == nullptr
                    ptr = ptr->left;
                }
            }
            mask = mask >> 1;
            if (mask == 0) {
                mask = 128;
                flag = flag_end;
                flag_end = fgetc(file);
            }
        }
        if (ferror(file)) {
            throw 0; // Error with code 0 for this moment
        }
        if (tail_length) {
            for (int i = 0; i < shift_tail_length - tail_length; i++) {
                if (ptr->left == nullptr && ptr->right == nullptr) {
                    flag_file_new = fputc(ptr->symbol, file_new);
                    if (flag_file_new < 0) {
                        throw 0;
                    }
                    ptr = ptr_node;
                }
                if (flag & mask) {
                    if (ptr->right) { // Корень может быть символом => left == right == nullptr
                        ptr = ptr->right;
                    }
                }
                else {
                    if (ptr->left) { // Корень может быть символом => left == right == nullptr
                        ptr = ptr->left;
                    }
                }
                mask = mask >> 1;
            }
        }

        if (fclose(file_new) || fclose(file)) {
            throw -2; // Error with code -2 for this moment
        }

        delete_tree(ptr_node);
        delete ptr_node;
        ptr_node = nullptr;
        if (queue) {
            for (int i = 0; i < queue_length; i++) {
                delete queue[i];
            }
        }
        delete[] queue;
        delete[] queue_frequency;

        return 1; // Успешное завершение
    }
    catch (int return_value) {
        if (return_value != -2) {
            if (file) {
                if (fclose(file)) {
                    return_value = -2; // Error with code -2 for this moment
                }
            }
            if (file_new) {
                if (fclose(file_new)) {
                    return_value = -2; // Error with code -2 for this moment
                }
                if (return_value != 1) {
                    if (remove(filepath_new)) {
                        return_value = -3; // Error with code -3 for this moment
                    }
                }
            }
        }


        delete_tree(ptr_node);
        delete ptr_node;
        ptr_node = nullptr;
        for (int i = 0; i < queue_length; i++) {
            delete queue[i];
        }
        delete[] queue;
        delete[] queue_frequency;

        return return_value; // Если 1 - успешное завершение, иначе код ошибки
    }
    catch (...) {
        int return_value = 0;
        if (file) {
            if (fclose(file)) {
                return_value = -2; // Error with code -2 for this moment
            }
        }
        if (file_new) {
            if (fclose(file_new)) {
                return_value = -2; // Error with code -2 for this moment
            }
            if (remove(filepath_new)) {
                return_value = -3; // Error with code -3 for this moment
            }
        }

        delete_tree(ptr_node);
        delete ptr_node;
        ptr_node = nullptr;
        if (queue) {
            for (int i = 0; i < queue_length; i++) {
                delete queue[i];
            }
        }
        delete[] queue;
        delete[] queue_frequency;

        return return_value; // Код ошибки
    }
}