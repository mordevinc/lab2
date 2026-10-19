#ifndef SET_H
#define SET_H

#include "hash_table.h"
#include <string>

// АТД Множество на основе хеш-таблицы
struct Set {
    HashTable ht;
};

void InitSet(Set* s);
void SETADD(Set* s, const std::string& key);
void SETDEL(Set* s, const std::string& key);
bool SET_AT(Set* s, const std::string& key);
void FreeSet(Set* s);

#endif