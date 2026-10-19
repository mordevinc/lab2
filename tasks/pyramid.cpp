#include "pyramid.h"
#include <iostream>

void RunPyramid(Array* tokens) {
    std::cout << "=== Задание 4: Пирамида ===" << std::endl;
    
    // Первый токен - количество блоков N
    int n = std::stoi(MGET(tokens, 1));
    
    // Используем наш Array для хранения блоков в виде строк "ширина,высота"
    Array blocks;
    InitArray(&blocks, n);
    
    // Заполняем массив из токенов (2 + i*2 - ширина, 3 + i*2 - высота)
    for (int i = 0; i < n; i++) {
        std::string w = MGET(tokens, 2 + i*2);
        std::string h = MGET(tokens, 3 + i*2);
        MPUSH(&blocks, w + "," + h);
    }

    // Сортировка пузырьком: по ширине возрастанию, при равенстве - по высоте убыванию
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            std::string s1 = MGET(&blocks, j);
            std::string s2 = MGET(&blocks, j+1);
            
            // Вытаскиваем ширину и высоту из строк
            int w1 = std::stoi(s1.substr(0, s1.find(',')));
            int h1 = std::stoi(s1.substr(s1.find(',')+1));
            int w2 = std::stoi(s2.substr(0, s2.find(',')));
            int h2 = std::stoi(s2.substr(s2.find(',')+1));
            
            bool swap_needed = false;
            if (w1 > w2) swap_needed = true;
            else if (w1 == w2 && h1 < h2) swap_needed = true;
            
            // Меняем местами
            if (swap_needed) { 
                MSET(&blocks, j, s2); 
                MSET(&blocks, j+1, s1); 
            }
        }
    }

    // Динамическое программирование для поиска макс. высоты
    Array dp;
    InitArray(&dp, n);
    int max_height = 0;
    
    for (int i = 0; i < n; i++) {
        std::string s_i = MGET(&blocks, i);
        int w_i = std::stoi(s_i.substr(0, s_i.find(',')));
        int h_i = std::stoi(s_i.substr(s_i.find(',')+1));
        
        int best_prev = 0;
        // Смотрим на все предыдущие блоки
        for (int j = 0; j < i; j++) {
            std::string s_j = MGET(&blocks, j);
            int w_j = std::stoi(s_j.substr(0, s_j.find(',')));
            
            // Если текущий блок можно поставить на предыдущий
            if (w_j < w_i) {
                int prev_h = std::stoi(MGET(&dp, j));
                if (prev_h > best_prev) best_prev = prev_h;
            }
        }
        // Текущая высота = лучшая предыдущая + высота текущего блока
        int current_h = best_prev + h_i;
        MPUSH(&dp, std::to_string(current_h));
        if (current_h > max_height) max_height = current_h;
    }
    
    std::cout << "Максимальная высота: " << max_height << std::endl;
    
    FreeArray(&blocks); 
    FreeArray(&dp);
}