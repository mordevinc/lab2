#!/bin/bash

echo "=== Очистка и сборка ==="
rm -f data/data.json
make clean && make

echo -e "\n=== ТЕСТИРОВАНИЕ ЛАБЫ 1 ==="
./bin/dbms --file data.json --query 'MPUSH myarray apple'
./bin/dbms --file data.json --query 'MPUSH myarray banana'
./bin/dbms --file data.json --query 'FPUSH mylist one'
./bin/dbms --file data.json --query 'FPUSH_FRONT mylist zero'
./bin/dbms --file data.json --query 'LPUSH mydlist first'
./bin/dbms --file data.json --query 'SPUSH mystack top'
./bin/dbms --file data.json --query 'QPUSH myqueue first'
./bin/dbms --file data.json --query 'DPUSH_BACK mydeque back'
./bin/dbms --file data.json --query 'TINSERT mytree 50'
./bin/dbms --file data.json --query 'TINSERT mytree 30'
./bin/dbms --file data.json --query 'PRINT'

echo -e "\n=== ТЕСТИРОВАНИЕ ЛАБЫ 2 ==="
./bin/dbms --query 'DFS'
echo ""
./bin/dbms --query 'SET'
echo ""
./bin/dbms --query 'INTER'
echo ""
./bin/dbms --query 'PYRAMID'
echo ""
./bin/dbms --query 'HUFFMAN'
echo ""
./bin/dbms --query 'SUBSTRING'
echo ""
./bin/dbms --query 'LFU'

echo -e "\n=== ГОТОВО ==="