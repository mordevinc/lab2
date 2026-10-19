#ifndef CUCKOO_HASH_H
#define CUCKOO_HASH_H

#include <string>

const int CUCKOO_SIZE = 11;

// Хеширование кукушки: две таблицы, два хеша. Если места нет - выбиваем элемент.
struct CuckooHashTable {
    std::string t1[CUCKOO_SIZE];
    std::string t2[CUCKOO_SIZE];
    bool occ1[CUCKOO_SIZE] = {false};
    bool occ2[CUCKOO_SIZE] = {false};
};

void InitCuckoo(CuckooHashTable* ch);
bool CuckooInsert(CuckooHashTable* ch, const std::string& key);
bool CuckooExists(CuckooHashTable* ch, const std::string& key);
void CuckooDelete(CuckooHashTable* ch, const std::string& key);

#endif