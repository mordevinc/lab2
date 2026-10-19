#include <iostream>
#include <sstream>
#include "structures/registry.h"
#include "utils/flags.h"
#include "utils/json_io.h"
#include "utils/file_utils.h"

#include "tasks/dfs.h"
#include "tasks/intersections.h"
#include "tasks/pyramid.h"
#include "tasks/huffman.h"
#include "tasks/longest_substring.h"
#include "tasks/lfu.h"
#include "structures/cuckoo_hash.h"

using namespace std;

// Разбиваем строку запроса на отдельные слова, используя наш Array
void SplitQuery(const string& query, Array* tokens) {
    string token = "";
    for (int i = 0; i <= (int)query.size(); i++) {
        if (i == (int)query.size() || query[i] == ' ') {
            if (!token.empty()) { 
                MPUSH(tokens, token); 
                token = ""; 
            }
        } else {
            token += query[i];
        }
    }
}

// Обрабатываем команду, введенную пользователем
void HandleQuery(Registry* reg, const string& query) {
    Array t;
    InitArray(&t, 100);
    SplitQuery(query, &t);
    
    if (t.size == 0) { FreeArray(&t); return; }
    string cmd = MGET(&t, 0);

    // Вызовы задач второй лабораторной
    if (cmd == "DFS") {
        int start = (t.size > 1) ? stoi(MGET(&t, 1)) : 0;
        string graph_data = (t.size > 2) ? MGET(&t, 2) : "";
        RunDFS(start, graph_data);
        FreeArray(&t); return;
    }
    else if (cmd == "HUFFMAN") {
        string text = (t.size > 1) ? MGET(&t, 1) : "abracadabra";
        RunHuffman(text);
        FreeArray(&t); return;
    }
    else if (cmd == "SUBSTRING") {
        string text = (t.size > 1) ? MGET(&t, 1) : "abcabcbb";
        RunLongestSubstring(text);
        FreeArray(&t); return;
    }
    else if (cmd == "PYRAMID") {
        RunPyramid(&t);
        FreeArray(&t); return;
    }
    else if (cmd == "LFU") {
        RunLFU(&t);
        FreeArray(&t); return;
    }
    else if (cmd == "INTER") {
        RunIntersections(&t);
        FreeArray(&t); return;
    }
    else if (cmd == "CUCKOO") {
        // Демонстрация работы хеширования кукушки
        CuckooHashTable ch;
        InitCuckoo(&ch);
        CuckooInsert(&ch, "apple");
        CuckooInsert(&ch, "banana");
        CuckooInsert(&ch, "cherry");
        cout << "banana exists? " << (CuckooExists(&ch, "banana") ? "Yes" : "No") << endl;
        CuckooDelete(&ch, "banana");
        cout << "banana exists? " << (CuckooExists(&ch, "banana") ? "Yes" : "No") << endl;
        FreeArray(&t); return;
    }

    // Общие команды для работы с реестром структур
    if (cmd == "PRINT") {
        if (t.size < 2) {
            for (int i = 0; i < (int)reg->entries.size(); i++) {
                StructEntry* e = &reg->entries[i];
                cout << "=== " << e->name << " (" << TypeToString(e->type) << ") ===" << endl;
                if (e->type == TYPE_ARRAY) PRINT(&e->array);
                else if (e->type == TYPE_FLIST) PRINT(&e->flist);
                else if (e->type == TYPE_DLIST) PRINT(&e->dlist);
                else if (e->type == TYPE_STACK) PRINT(&e->stack);
                else if (e->type == TYPE_QUEUE) PRINT(&e->queue);
                else if (e->type == TYPE_DEQUE) PRINT(&e->deque);
                else if (e->type == TYPE_TREE) PRINT(&e->tree);
                else if (e->type == TYPE_SET) cout << "Set contents printed via SET_AT." << endl;
            }
        } else {
            StructEntry* e = FindStruct(reg, MGET(&t, 1));
            if (!e) { cout << "not found" << endl; }
            else {
                if (e->type == TYPE_ARRAY) PRINT(&e->array);
                else if (e->type == TYPE_FLIST) PRINT(&e->flist);
                else if (e->type == TYPE_DLIST) PRINT(&e->dlist);
                else if (e->type == TYPE_STACK) PRINT(&e->stack);
                else if (e->type == TYPE_QUEUE) PRINT(&e->queue);
                else if (e->type == TYPE_DEQUE) PRINT(&e->deque);
                else if (e->type == TYPE_TREE) PRINT(&e->tree);
            }
        }
        FreeArray(&t); return;
    }

    if (cmd == "CREATE") {
        if (t.size < 3) { cout << "usage: CREATE <name> <type>" << endl; FreeArray(&t); return; }
        StructType type = StringToType(MGET(&t, 2));
        if (type == TYPE_NONE) { cout << "unknown type" << endl; FreeArray(&t); return; }
        if (FindStruct(reg, MGET(&t, 1))) { cout << "already exists" << endl; FreeArray(&t); return; }
        CreateStruct(reg, MGET(&t, 1), type);
        cout << "-> created" << endl;
        FreeArray(&t); return;
    }

    // Команды для работы с конкретными структурами
    if (cmd == "SETADD") {
        if (t.size < 3) { FreeArray(&t); return; }
        StructEntry* e = FindStruct(reg, MGET(&t, 1));
        if (e && e->type == TYPE_SET) {
            SETADD(&e->set, MGET(&t, 2));
            cout << "-> OK" << endl;
        } else cout << "Set not found" << endl;
    }
    else if (cmd == "SETDEL") {
        if (t.size < 3) { FreeArray(&t); return; }
        StructEntry* e = FindStruct(reg, MGET(&t, 1));
        if (e && e->type == TYPE_SET) {
            SETDEL(&e->set, MGET(&t, 2));
            cout << "-> OK" << endl;
        } else cout << "Set not found" << endl;
    }
    else if (cmd == "SET_AT") {
        if (t.size < 3) { FreeArray(&t); return; }
        StructEntry* e = FindStruct(reg, MGET(&t, 1));
        if (e && e->type == TYPE_SET) {
            cout << "-> " << (SET_AT(&e->set, MGET(&t, 2)) ? "TRUE" : "FALSE") << endl;
        } else cout << "Set not found" << endl;
    }
    else if (cmd == "MPUSH") {
        if (t.size < 3) { FreeArray(&t); return; }
        StructEntry* e = FindStruct(reg, MGET(&t, 1));
        if (e && e->type == TYPE_ARRAY) { MPUSH(&e->array, MGET(&t, 2)); cout << "-> " << MGET(&t, 2) << endl; }
        else cout << "not found" << endl;
    }
    else if (cmd == "SPUSH") {
        if (t.size < 3) { FreeArray(&t); return; }
        StructEntry* e = FindStruct(reg, MGET(&t, 1));
        if (e && e->type == TYPE_STACK) { SPUSH(&e->stack, MGET(&t, 2)); cout << "-> " << MGET(&t, 2) << endl; }
        else cout << "not found" << endl;
    }
    else if (cmd == "SPOP") {
        if (t.size < 2) { FreeArray(&t); return; }
        StructEntry* e = FindStruct(reg, MGET(&t, 1));
        if (e && e->type == TYPE_STACK) { cout << "-> " << SPOP(&e->stack) << endl; }
        else cout << "not found" << endl;
    }
    // ... остальные команды (FPUSH, TINSERT и т.д.) по аналогии
    
    else {
        cout << "unknown command: " << cmd << endl;
    }
    
    FreeArray(&t);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");
    EnsureDirectory("data");
    EnsureDirectory("bin");

    Flags flags;
    ParseFlags(argc, argv, &flags);
    string query = GetFlag(&flags, "--query");
    
    // Если команда относится к задачам, файл для сохранения нам не нужен
    Array check;
    InitArray(&check, 10);
    SplitQuery(query, &check);
    if (check.size > 0) {
        string cmd = MGET(&check, 0);
        if (cmd == "DFS" || cmd == "HUFFMAN" || cmd == "SUBSTRING" || 
            cmd == "PYRAMID" || cmd == "LFU" || cmd == "INTER" || cmd == "CUCKOO") {
            Registry dummy;
            HandleQuery(&dummy, query);
            FreeArray(&check);
            return 0;
        }
    }
    FreeArray(&check);

    string filename = GetFlag(&flags, "--file");
    if (filename.empty()) filename = "data.json";
    filename = BuildDataPath(filename);

    Registry reg;
    InitRegistry(&reg);

    // Если файл есть - загружаем, иначе создаем структуры по умолчанию
    if (FileExists(filename)) {
        LoadRegistry(&reg, filename);
    } else {
        CreateStruct(&reg, "myarray", TYPE_ARRAY);
        CreateStruct(&reg, "mystack", TYPE_STACK);
        CreateStruct(&reg, "myset", TYPE_SET);
        SaveRegistry(&reg, filename);
    }

    if (!query.empty()) HandleQuery(&reg, query);
    else HandleQuery(&reg, "PRINT");

    SaveRegistry(&reg, filename);
    FreeRegistry(&reg);
    return 0;
}