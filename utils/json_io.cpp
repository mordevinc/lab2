#include "json_io.h"
#include <fstream>
#include <sstream>
#include <iostream>

JsonObject JsonCreate() {
    JsonObject obj;
    obj.pairs.clear();
    return obj;
}

void JsonSet(JsonObject* obj, const std::string& key, const std::string& value) {
    for (int i = 0; i < (int)obj->pairs.size(); i++) {
        if (obj->pairs[i].key == key) {
            obj->pairs[i].value = value;
            return;
        }
    }
    JsonPair p;
    p.key = key;
    p.value = value;
    obj->pairs.push_back(p);
}

std::string JsonGet(JsonObject* obj, const std::string& key) {
    for (int i = 0; i < (int)obj->pairs.size(); i++) {
        if (obj->pairs[i].key == key) return obj->pairs[i].value;
    }
    return "";
}

void JsonRemove(JsonObject* obj, const std::string& key) {
    for (int i = 0; i < (int)obj->pairs.size(); i++) {
        if (obj->pairs[i].key == key) {
            obj->pairs.erase(obj->pairs.begin() + i);
            return;
        }
    }
}

std::string JsonToString(JsonObject* obj) {
    std::string result = "{\n";
    for (int i = 0; i < (int)obj->pairs.size(); i++) {
        result += "  \"" + obj->pairs[i].key + "\": \"" + obj->pairs[i].value + "\"";
        if (i != (int)obj->pairs.size() - 1) result += ",";
        result += "\n";
    }
    result += "}";
    return result;
}

// ИСПРАВЛЕННЫЙ ПАРСЕР
JsonObject JsonParse(const std::string& text) {
    JsonObject obj = JsonCreate();
    std::string key, value;
    bool inString = false;
    bool isKey = false;
    bool isValue = false;
    bool firstQuoteDone = false;

    for (int i = 0; i < (int)text.size(); i++) {
        char c = text[i];

        if (c == '"') {
            inString = !inString;
            if (inString) {
                if (!firstQuoteDone) {
                    isKey = true; isValue = false; firstQuoteDone = true;
                } else {
                    isKey = false; isValue = true;
                }
            } else {
                if (isValue) {
                    JsonSet(&obj, key, value);
                    key = ""; value = "";
                    isKey = false; isValue = false; firstQuoteDone = false;
                }
            }
            continue;
        }

        if (inString) {
            if (isKey) key += c;
            else if (isValue) value += c;
        }
    }

    return obj;
}

void JsonWriteToFile(JsonObject* obj, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Cannot open file: " << filename << std::endl;
        return;
    }
    file << JsonToString(obj);
    file.close();
}

JsonObject JsonReadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Cannot open file: " << filename << std::endl;
        return JsonCreate();
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return JsonParse(buffer.str());
}