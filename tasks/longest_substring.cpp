#include "longest_substring.h"
#include "../structures/hash_table.h"
#include <iostream>

void RunLongestSubstring(const std::string& s) {
    std::cout << "=== Задание 6.2: Самая длинная подстрока (с HashTable) ===" << std::endl;
    
    // Используем нашу хеш-таблицу (метод цепочек) из 6.1
    HashTable ht;
    InitHashTable(&ht);

    int max_len = 0;
    int left = 0; // Левая граница "скользящего окна"

    for (int right = 0; right < (int)s.length(); right++) {
        // Создаем ключ из одного символа
        std::string char_key = std::string(1, s[right]);
        
        // Если символ уже был в таблице
        if (HEXISTS(&ht, char_key)) {
            int last_pos = std::stoi(HGET(&ht, char_key));
            // И его позиция не вышла за пределы нашего окна - сдвигаем левую границу
            if (last_pos >= left) {
                left = last_pos + 1;
            }
        }
        
        // Обновляем позицию символа в таблице
        HSET(&ht, char_key, std::to_string(right));
        
        // Пересчитываем длину текущего окна
        int current_len = right - left + 1;
        if (current_len > max_len) max_len = current_len;
    }

    std::cout << "Строка: " << s << ", макс. длина: " << max_len << std::endl;
}