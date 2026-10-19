#include "set_task.h"
#include "../structures/set.h"
#include <iostream>

void RunSetTask() {
    std::cout << "=== Задание 2: АТД Множество ===" << std::endl;
    Set s;
    InitSet(&s);

    SETADD(&s, "apple");
    SETADD(&s, "banana");
    SETADD(&s, "apple"); // дубликат

    std::cout << "apple: " << (SET_AT(&s, "apple") ? "Да" : "Нет") << std::endl;
    std::cout << "cherry: " << (SET_AT(&s, "cherry") ? "Да" : "Нет") << std::endl;

    SETDEL(&s, "apple");
    std::cout << "apple после удаления: " << (SET_AT(&s, "apple") ? "Да" : "Нет") << std::endl;
}