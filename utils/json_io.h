#ifndef JSON_IO_H
#define JSON_IO_H

#include <string>
#include <vector>

struct JsonPair {
    std::string key;
    std::string value;
};

struct JsonObject {
    std::vector<JsonPair> pairs;
};

JsonObject JsonCreate();
void JsonSet(JsonObject* obj, const std::string& key, const std::string& value);
std::string JsonGet(JsonObject* obj, const std::string& key);
void JsonRemove(JsonObject* obj, const std::string& key);
std::string JsonToString(JsonObject* obj);
JsonObject JsonParse(const std::string& text);
void JsonWriteToFile(JsonObject* obj, const std::string& filename);
JsonObject JsonReadFromFile(const std::string& filename);

#endif