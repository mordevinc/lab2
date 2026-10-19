#include "lfu.h"
#include "../structures/hash_table.h"
#include <iostream>

void RunLFU(Array* tokens) {
    std::cout << "=== Задание 7: LFU-кэш ===" << std::endl;
    
    // Первый токен после команды - емкость кэша
    int capacity = std::stoi(MGET(tokens, 1));
    
    // Используем нашу хеш-таблицу для хранения пар "ключ:значение_частота"
    HashTable cache;
    InitHashTable(&cache);
    
    // Для каждой частоты держим свой двусвязный список (из ЛР1)
    DoublyList freq_lists[100];
    for (int i = 0; i < 100; i++) InitDoublyList(&freq_lists[i]);
    
    int min_freq = 0;
    int size = 0;

    // Начинаем разбор команд с 2-го индекса (SET/GET)
    for (int i = 2; i < tokens->size; ) {
        std::string cmd = MGET(tokens, i);
        
        if (cmd == "SET") {
            int key = std::stoi(MGET(tokens, i+1));
            int value = std::stoi(MGET(tokens, i+2));
            
            if (capacity > 0) {
                // Если ключа нет и кэш полон - вытесняем самый редкий
                if (!HEXISTS(&cache, std::to_string(key)) && size == capacity) {
                    // Берем первый элемент из списка минимальной частоты
                    std::string evict_key = freq_lists[min_freq].head->data;
                    LDEL_FRONT(&freq_lists[min_freq]); // Удаляем из списка
                    HDEL(&cache, evict_key);           // Удаляем из таблицы
                    size--;
                }
                
                // Если ключ уже существует - удаляем его из старого списка частот
                if (HEXISTS(&cache, std::to_string(key))) {
                    std::string val_freq = HGET(&cache, std::to_string(key));
                    int old_freq = std::stoi(val_freq.substr(val_freq.find(':') + 1));
                    LDEL_BY_VALUE(&freq_lists[old_freq], std::to_string(key));
                } else {
                    size++;
                }
                
                // Добавляем ключ с частотой 1
                HSET(&cache, std::to_string(key), std::to_string(value) + ":1");
                LPUSH_BACK(&freq_lists[1], std::to_string(key));
                min_freq = 1;
            }
            i += 3; // Пропускаем 3 токена (SET, key, value)
        } 
        else if (cmd == "GET") {
            int key = std::stoi(MGET(tokens, i+1));
            
            if (!HEXISTS(&cache, std::to_string(key))) {
                std::cout << "-1 ";
            } else {
                // Достаем значение и старую частоту
                std::string val_freq = HGET(&cache, std::to_string(key));
                int value = std::stoi(val_freq.substr(0, val_freq.find(':')));
                int old_freq = std::stoi(val_freq.substr(val_freq.find(':') + 1));
                
                // Удаляем из старого списка частот
                LDEL_BY_VALUE(&freq_lists[old_freq], std::to_string(key));
                
                // Перемещаем в список с частотой + 1
                int new_freq = old_freq + 1;
                if (freq_lists[min_freq].head == nullptr) min_freq++;
                
                LPUSH_BACK(&freq_lists[new_freq], std::to_string(key));
                HSET(&cache, std::to_string(key), std::to_string(value) + ":" + std::to_string(new_freq));
                
                std::cout << value << " ";
            }
            i += 2; // Пропускаем 2 токена (GET, key)
        } 
        else { 
            i++; 
        }
    }
    std::cout << std::endl;
}