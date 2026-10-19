#ifndef FLAGS_H
#define FLAGS_H

#include <string>
#include <vector>

struct Flag {
    std::string key;
    std::string value;
};

struct Flags {
    int count = 0;
    std::vector<Flag> items;
};

void InitFlags(Flags* flags);
void AddFlag(Flags* flags, const std::string& key, const std::string& value);
std::string GetFlag(Flags* flags, const std::string& key);
void ParseFlags(int argc, char* argv[], Flags* flags);
void PrintFlags(Flags* flags);

#endif