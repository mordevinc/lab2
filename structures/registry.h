#ifndef REGISTRY_H
#define REGISTRY_H

#include <string>
#include <vector>
#include "linear.h"
#include "avl_tree.h"
#include "set.h"

// Перечисление всех типов структур
enum StructType {
    TYPE_NONE = 0,
    TYPE_ARRAY,
    TYPE_FLIST,
    TYPE_DLIST,
    TYPE_STACK,
    TYPE_QUEUE,
    TYPE_DEQUE,
    TYPE_TREE,
    TYPE_SET
};

// Обертка над структурой с её именем
struct StructEntry {
    std::string name;
    StructType type = TYPE_NONE;

    Array array;
    ForwardList flist;
    DoublyList dlist;
    Stack stack;
    Queue queue;
    Deque deque;
    AVLTree tree;
    Set set;
};

// Реестр всех структур
struct Registry {
    std::vector<StructEntry> entries;
};

void InitRegistry(Registry* reg);
void FreeRegistry(Registry* reg);
StructEntry* CreateStruct(Registry* reg, const std::string& name, StructType type);
StructEntry* FindStruct(Registry* reg, const std::string& name);
std::string TypeToString(StructType type);
StructType StringToType(const std::string& s);
void SaveRegistry(Registry* reg, const std::string& filename);
void LoadRegistry(Registry* reg, const std::string& filename);

#endif