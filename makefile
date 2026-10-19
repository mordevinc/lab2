CXX = g++
CXXFLAGS = -std=c++17 -Wall -I.

# Список всех исходников проекта
SRC = main.cpp \
      structures/array.cpp \
      structures/forward_list.cpp \
      structures/doubly_list.cpp \
      structures/stack.cpp \
      structures/queue.cpp \
      structures/deque.cpp \
      structures/avl_tree.cpp \
      structures/hash_table.cpp \
      structures/cuckoo_hash.cpp \
      structures/set.cpp \
      structures/registry.cpp \
      utils/flags.cpp \
      utils/json_io.cpp \
      utils/file_utils.cpp \
      tasks/dfs.cpp \
      tasks/intersections.cpp \
      tasks/pyramid.cpp \
      tasks/huffman.cpp \
      tasks/longest_substring.cpp \
      tasks/lfu.cpp

TARGET = bin/dbms

all: $(TARGET)

# Сборка исполняемого файла в папку bin
$(TARGET): $(SRC)
	@mkdir -p bin
	@mkdir -p data
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -rf bin/* data/*.json

.PHONY: all clean