#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "linear.h"
#include <string>

// Размер таблицы для метода цепочек
const int TABLE_SIZE = 100;

// Хеш-таблица на основе ForwardList (разрешение коллизий цепочками)
struct HashTable {
    ForwardList buckets[TABLE_SIZE];
};

void InitHashTable(HashTable* ht);
void HSET(HashTable* ht, const std::string& key, const std::string& value);
std::string HGET(HashTable* ht, const std::string& key);
void HDEL(HashTable* ht, const std::string& key);
bool HEXISTS(HashTable* ht, const std::string& key);

#endif