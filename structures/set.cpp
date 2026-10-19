#include "set.h"

void InitSet(Set* s) {
    InitHashTable(&s->ht);
}

// Добавляем элемент (значение не важно, поэтому ставим "1")
void SETADD(Set* s, const std::string& key) {
    HSET(&s->ht, key, "1");
}

// Удаляем
void SETDEL(Set* s, const std::string& key) {
    HDEL(&s->ht, key);
}

// Проверяем наличие
bool SET_AT(Set* s, const std::string& key) {
    return HEXISTS(&s->ht, key);
}

// Очистка памяти
void FreeSet(Set* s) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        FreeForwardList(&s->ht.buckets[i]);
    }
}