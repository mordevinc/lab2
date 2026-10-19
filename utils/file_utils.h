#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>

// Создать директорию, если её нет
void EnsureDirectory(const std::string& path);

// Проверить, существует ли файл
bool FileExists(const std::string& path);

// Построить путь: если относительный — добавить префикс "data/"
std::string BuildDataPath(const std::string& filename);

#endif