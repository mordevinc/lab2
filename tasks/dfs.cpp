#include "dfs.h"
#include "../structures/linear.h"
#include <iostream>

void RunDFS(int start_node, const std::string& graph_data) {
    std::cout << "=== Задание 1: Обход графа в глубину (DFS) ===" << std::endl;
    
    // Используем наш Array для хранения списков смежности (соседей)
    Array adj;
    InitArray(&adj, 100);

    // Если граф передан строкой, парсим его. Вершины разделены ';'
    if (!graph_data.empty()) {
        std::string temp = "";
        for (int i = 0; i <= (int)graph_data.size(); i++) {
            if (i == (int)graph_data.size() || graph_data[i] == ';') {
                if (!temp.empty()) {
                    MPUSH(&adj, temp); // Сохраняем список соседей для одной вершины
                    temp = "";
                }
            } else {
                temp += graph_data[i];
            }
        }
        std::cout << "Загружен пользовательский граф (вершин: " << MLEN(&adj) << ")" << std::endl;
    } else {
        // Если аргументов нет, используем граф по умолчанию
        MPUSH(&adj, "1,2"); MPUSH(&adj, "0,3"); MPUSH(&adj, "0,4"); MPUSH(&adj, "1"); MPUSH(&adj, "2");
        std::cout << "Загружен граф по умолчанию (вершин: 5)" << std::endl;
    }

    int n = MLEN(&adj);
    
    // Создаем массив посещенных вершин. Изначально везде "0" (не посещена)
    Array visited;
    InitArray(&visited, n + 1);
    for (int i = 0; i < n; i++) MPUSH(&visited, "0");

    // Используем стек из первой лабы для хранения вершин
    Stack stack;
    InitStack(&stack);
    SPUSH(&stack, std::to_string(start_node));

    std::cout << "Порядок обхода: ";

    while (stack.size > 0) {
        // Достаем вершину из стека (она приходит как строка)
        std::string u_str = SPOP(&stack);
        int u = std::stoi(u_str); // Переводим строку обратно в число

        // Если вершина уже посещена или номер выходит за границы, пропускаем
        if (u < 0 || u >= n || MGET(&visited, u) == "1") continue;
        
        // Помечаем как посещенную и выводим
        MSET(&visited, u, "1");
        std::cout << u << " ";

        // Разбираем строку соседей (например, "1,2") посимвольно
        std::string neighbors = MGET(&adj, u);
        std::string temp = "";
        for (int i = 0; i <= (int)neighbors.size(); i++) {
            // Как только встретили запятую или конец строки - обрабатываем накопленное число
            if (i == (int)neighbors.size() || neighbors[i] == ',') {
                if (!temp.empty()) {
                    int v = std::stoi(temp);
                    // Если сосед еще не посещен - кидаем его в стек
                    if (v >= 0 && v < n && MGET(&visited, v) == "0") {
                        SPUSH(&stack, temp);
                    }
                    temp = "";
                }
            } else {
                temp += neighbors[i];
            }
        }
    }
    std::cout << std::endl;
    
    // Очищаем память
    FreeArray(&adj); 
    FreeArray(&visited); 
    FreeStack(&stack);
}