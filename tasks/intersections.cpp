#include "intersections.h"
#include "../structures/set.h"
#include <iostream>

void RunIntersections(Array* tokens) {
    std::cout << "=== Задание 3: Максимальное пересечение ===" << std::endl;
    
    // Первый токен (tokens[0]) - это сама команда "INTER". Множества идут начиная с 1.
    int num_sets = tokens->size - 1;
    if (num_sets < 2) {
        std::cout << "Нужно минимум 2 множества!" << std::endl;
        return;
    }

    // Создаем массив наших множеств
    Set* sets = new Set[num_sets];
    for (int i = 0; i < num_sets; i++) InitSet(&sets[i]);

    // Парсим строки типа "1,2,3" в структуру Set
    for (int i = 0; i < num_sets; i++) {
        std::string set_str = MGET(tokens, i + 1);
        std::string temp = "";
        for (int j = 0; j <= (int)set_str.size(); j++) {
            if (j == (int)set_str.size() || set_str[j] == ',') {
                if (!temp.empty()) {
                    SETADD(&sets[i], temp);
                    temp = "";
                }
            } else {
                temp += set_str[j];
            }
        }
    }

    int max_common = -1;
    int best_i = -1, best_j = -1;

    // Перебираем все возможные пары множеств
    for (int i = 0; i < num_sets; i++) {
        for (int j = i + 1; j < num_sets; j++) {
            int common = 0;
            
            // Идем по всем бакетам хеш-таблицы i-го множества
            for (int b = 0; b < TABLE_SIZE; b++) {
                FNode* curr = sets[i].ht.buckets[b].head;
                while (curr != nullptr) {
                    // Достаем ключ (отрезаем от "key:value")
                    size_t colon = curr->data.find(':');
                    std::string key = curr->data.substr(0, colon);
                    
                    // Если этот ключ есть во втором множестве - увеличиваем счетчик
                    if (SET_AT(&sets[j], key)) {
                        common++;
                    }
                    curr = curr->next;
                }
            }
            // Запоминаем лучшую пару
            if (common > max_common) {
                max_common = common;
                best_i = i;
                best_j = j;
            }
        }
    }

    std::cout << "Максимум общих элементов: " << max_common 
              << " (множества " << best_i + 1 << " и " << best_j + 1 << ")" << std::endl;
    
    delete[] sets;
}