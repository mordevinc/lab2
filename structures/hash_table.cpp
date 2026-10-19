#include "hash_table.h"

// Простая хеш-функция: складываем коды символов и берем остаток от деления
int hash_func(const std::string& key) {
    int sum = 0;
    for (char c : key) sum += c;
    return sum % TABLE_SIZE;
}

// Инициализируем пустые списки в каждом бакете
void InitHashTable(HashTable* ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        InitForwardList(&ht->buckets[i]);
    }
}

// Добавляем или обновляем значение. Храним в виде строки "key:value"
void HSET(HashTable* ht, const std::string& key, const std::string& value) {
    int idx = hash_func(key);
    std::string pair = key + ":" + value;
    
    // Ищем ключ в бакете
    FNode* curr = ht->buckets[idx].head;
    while (curr != nullptr) {
        size_t colon = curr->data.find(':');
        // Если нашли такой ключ, просто меняем значение
        if (colon != std::string::npos && curr->data.substr(0, colon) == key) {
            curr->data = pair; 
            return;
        }
        curr = curr->next;
    }
    // Если не нашли - добавляем новый узел в конец списка
    FPUSH_BACK(&ht->buckets[idx], pair);
}

// Получаем значение по ключу
std::string HGET(HashTable* ht, const std::string& key) {
    int idx = hash_func(key);
    FNode* curr = ht->buckets[idx].head;
    while (curr != nullptr) {
        size_t colon = curr->data.find(':');
        if (colon != std::string::npos && curr->data.substr(0, colon) == key) {
            return curr->data.substr(colon + 1); // Отдаем то, что после двоеточия
        }
        curr = curr->next;
    }
    return "";
}

// Удаляем ключ из таблицы
void HDEL(HashTable* ht, const std::string& key) {
    int idx = hash_func(key);
    FNode* curr = ht->buckets[idx].head;
    while (curr != nullptr) {
        size_t colon = curr->data.find(':');
        if (colon != std::string::npos && curr->data.substr(0, colon) == key) {
            FDEL_BY_VALUE(&ht->buckets[idx], curr->data); // Используем функцию из ЛР1
            return;
        }
        curr = curr->next;
    }
}

// Проверяем, есть ли ключ
bool HEXISTS(HashTable* ht, const std::string& key) {
    int idx = hash_func(key);
    FNode* curr = ht->buckets[idx].head;
    while (curr != nullptr) {
        size_t colon = curr->data.find(':');
        if (colon != std::string::npos && curr->data.substr(0, colon) == key) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}