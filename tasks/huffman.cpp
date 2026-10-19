#include "huffman.h"
#include "../structures/linear.h"
#include <iostream>

// Узел дерева Хаффмана
struct HuffNode {
    char ch;
    int freq;
    HuffNode* left;
    HuffNode* right;
    
    HuffNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Рекурсивный обход для генерации кодов (0 - налево, 1 - направо)
void GenerateCodes(HuffNode* root, std::string code, Array* codes) {
    if (root == nullptr) return;
    
    // Если дошли до листа - сохраняем в наш Array строку вида "a:010"
    if (root->left == nullptr && root->right == nullptr) {
        std::string entry = std::string(1, root->ch) + ":" + code;
        MPUSH(codes, entry);
    }
    
    GenerateCodes(root->left, code + "0", codes);
    GenerateCodes(root->right, code + "1", codes);
}

// Восстановление исходной строки по бинарному коду
std::string Decode(HuffNode* root, const std::string& encoded) {
    std::string result = "";
    HuffNode* curr = root;
    
    for (char bit : encoded) {
        // Идем по дереву в зависимости от бита
        curr = (bit == '0') ? curr->left : curr->right;
        
        // Если дошли до листа - записываем символ и возвращаемся в корень
        if (curr->left == nullptr && curr->right == nullptr) {
            result += curr->ch;
            curr = root;
        }
    }
    return result;
}

void RunHuffman(const std::string& text) {
    std::cout << "=== Задание 5: Хаффман ===" << std::endl;
    std::cout << "Исходная строка: " << text << std::endl;

    // 1. Считаем частоты символов
    int freqs[256] = {0};
    for (char c : text) freqs[(unsigned char)c]++;

    // 2. Создаем узлы для каждого уникального символа
    HuffNode* nodes[256];
    int node_count = 0;
    for (int i = 0; i < 256; i++) {
        if (freqs[i] > 0) nodes[node_count++] = new HuffNode((char)i, freqs[i]);
    }

    // 3. Строим дерево: каждый раз объединяем два узла с наименьшей частотой
    while (node_count > 1) {
        int min1 = -1, min2 = -1;
        
        // Ищем два минимума вручную (без использования сортировок)
        for (int i = 0; i < node_count; i++) {
            if (min1 == -1 || nodes[i]->freq < nodes[min1]->freq) { 
                min2 = min1; min1 = i; 
            } else if (min2 == -1 || nodes[i]->freq < nodes[min2]->freq) { 
                min2 = i; 
            }
        }
        
        // Создаем нового родителя для них
        HuffNode* left = nodes[min1];
        HuffNode* right = nodes[min2];
        HuffNode* parent = new HuffNode('\0', left->freq + right->freq);
        parent->left = left; parent->right = right;
        
        // Удаляем два старых узла из массива, добавляем нового родителя
        if (min1 > min2) std::swap(min1, min2);
        nodes[min1] = parent;
        for (int i = min2; i < node_count - 1; i++) nodes[i] = nodes[i + 1];
        node_count--;
    }

    HuffNode* root = nodes[0];
    
    // 4. Генерируем таблицу кодов в наш Array
    Array codes;
    InitArray(&codes, 256);
    GenerateCodes(root, "", &codes);

    std::cout << "\nТаблица кодов:" << std::endl;
    for (int i = 0; i < codes.size; i++) std::cout << MGET(&codes, i) << std::endl;

    // 5. Кодируем текст
    std::string encoded = "";
    for (char c : text) {
        for (int i = 0; i < codes.size; i++) {
            std::string entry = MGET(&codes, i);
            // Если первый символ в строке "a:010" совпадает с нашим символом
            if (entry[0] == c) { 
                encoded += entry.substr(2); // Берем только код (отрезаем "a:")
                break; 
            }
        }
    }
    std::cout << "\nЗакодировано: " << encoded << std::endl;

    // 6. Декодируем обратно
    std::string decoded = Decode(root, encoded);
    std::cout << "Декодировано: " << decoded << std::endl;
    
    FreeArray(&codes);
}