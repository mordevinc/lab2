#include "flags.h"
#include <iostream>

void InitFlags(Flags* flags) {
    flags->count = 0;
    flags->items.clear();
}

void AddFlag(Flags* flags, const std::string& key, const std::string& value) {
    Flag f;
    f.key = key;
    f.value = value;
    flags->items.push_back(f);
    flags->count++;
}

std::string GetFlag(Flags* flags, const std::string& key) {
    for (int i = 0; i < flags->count; i++) {
        if (flags->items[i].key == key) return flags->items[i].value;
    }
    return "";
}

void ParseFlags(int argc, char* argv[], Flags* flags) {
    InitFlags(flags);
    for (int i = 1; i < argc - 1; i++) {
        std::string arg = argv[i];
        if (arg.size() > 2 && arg[0] == '-' && arg[1] == '-') {
            AddFlag(flags, arg, argv[i + 1]);
            i++;
        }
    }
}

void PrintFlags(Flags* flags) {
    std::cout << "=== Flags ===" << std::endl;
    for (int i = 0; i < flags->count; i++) {
        std::cout << flags->items[i].key << " = " << flags->items[i].value << std::endl;
    }
}