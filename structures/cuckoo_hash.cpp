#include "cuckoo_hash.h"
#include <iostream>

// Первая хеш-функция
int h1(const std::string& key) {
    int sum = 0; 
    for (char c : key) sum += c;
    return sum % CUCKOO_SIZE;
}

// Вторая хеш-функция (должна давать другой результат)
int h2(const std::string& key) {
    int sum = 0; 
    for (char c : key) sum += c * 3;
    return sum % CUCKOO_SIZE;
}

// Инициализация пустых таблиц
void InitCuckoo(CuckooHashTable* ch) {
    for (int i = 0; i < CUCKOO_SIZE; i++) {
        ch->occ1[i] = false; ch->occ2[i] = false;
        ch->t1[i] = ""; ch->t2[i] = "";
    }
}

// Вставка элемента
bool CuckooInsert(CuckooHashTable* ch, const std::string& key) {
    std::string current = key;
    
    // Ограничиваем количество шагов, чтобы не зациклиться
    for (int i = 0; i < 20; i++) { 
        int pos1 = h1(current);
        if (!ch->occ1[pos1]) {
            ch->t1[pos1] = current; ch->occ1[pos1] = true;
            return true;
        }
        // Место занято. Забираем чужой элемент и кладем свой.
        std::string evicted = ch->t1[pos1];
        ch->t1[pos1] = current;
        current = evicted; // Теперь нужно пристроить "выбитый" элемент

        int pos2 = h2(current);
        if (!ch->occ2[pos2]) {
            ch->t2[pos2] = current; ch->occ2[pos2] = true;
            return true;
        }
        // И тут занято. Снова меняемся.
        evicted = ch->t2[pos2];
        ch->t2[pos2] = current;
        current = evicted;
    }
    // Если за 20 шагов не нашли место - значит, цикл.
    std::cout << "Ошибка: цикл в кукушкином хешировании" << std::endl;
    return false;
}

// Проверка наличия
bool CuckooExists(CuckooHashTable* ch, const std::string& key) {
    int pos1 = h1(key); int pos2 = h2(key);
    if (ch->occ1[pos1] && ch->t1[pos1] == key) return true;
    if (ch->occ2[pos2] && ch->t2[pos2] == key) return true;
    return false;
}

// Удаление
void CuckooDelete(CuckooHashTable* ch, const std::string& key) {
    int pos1 = h1(key); int pos2 = h2(key);
    if (ch->occ1[pos1] && ch->t1[pos1] == key) ch->occ1[pos1] = false;
    if (ch->occ2[pos2] && ch->t2[pos2] == key) ch->occ2[pos2] = false;
}