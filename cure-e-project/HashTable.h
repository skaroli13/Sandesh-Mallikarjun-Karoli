// HashTable.h
// Sandesh Mallikarjun Karoli
// 013114596
// Custom hash table implementation using separate chaining.

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "DelayData.h"
#include <string>
#include <iostream>

struct Node {
    std::string key;
    DelayData   value;
    Node*       next;

    Node(const std::string& k, const DelayData& v)
        : key(k), value(v), next(nullptr) {}
};

class HashTable {
private:
    static const int TABLE_SIZE = 1021;
    Node* table[TABLE_SIZE];
    int   numEntries;

    int hashFunction(const std::string& key) const {
        unsigned long hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
        }
        return static_cast<int>(hash % TABLE_SIZE);
    }

public:
    HashTable();
    ~HashTable();
    void insert(const std::string& key, const DelayData& data);
    DelayData* get(const std::string& key);
    bool contains(const std::string& key) const;
    void printAll() const;
    void printSummary() const;
    void printTopN(int n) const;
    int size() const { return numEntries; }
};

#endif