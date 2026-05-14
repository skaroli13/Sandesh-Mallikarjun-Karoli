// HashTable.cpp
// Sandesh Mallikarjun Karoli
// 013114596
// Implementation of the custom hash table using separate chaining.

#include "HashTable.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

HashTable::HashTable() : numEntries(0) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
        table[i] = nullptr;
    }
}

void HashTable::insert(const std::string& key, const DelayData& data) {
    int index = hashFunction(key);
    Node* current = table[index];

    while (current != nullptr) {
        if (current->key == key) {
            current->value.carrier_delay       += data.carrier_delay;
            current->value.weather_delay       += data.weather_delay;
            current->value.nas_delay           += data.nas_delay;
            current->value.security_delay      += data.security_delay;
            current->value.late_aircraft_delay += data.late_aircraft_delay;
            current->value.num_flights         += data.num_flights;
            return;
        }
        current = current->next;
    }

    Node* newNode = new Node(key, data);
    newNode->next = table[index];
    table[index] = newNode;
    numEntries++;
}

DelayData* HashTable::get(const std::string& key) {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            return &(current->value);
        }
        current = current->next;
    }
    return nullptr;
}

bool HashTable::contains(const std::string& key) const {
    int index = hashFunction(key);
    Node* current = table[index];
    while (current != nullptr) {
        if (current->key == key) return true;
        current = current->next;
    }
    return false;
}

void HashTable::printAll() const {
    std::cout << "\n===== ALL ENTRIES (" << numEntries << " unique airport+season combos) =====\n";
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            current->value.print();
            std::cout << "---------------------------------------------\n";
            current = current->next;
        }
    }
}

void HashTable::printSummary() const {
    std::vector<DelayData> entries;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            entries.push_back(current->value);
            current = current->next;
        }
    }

    std::sort(entries.begin(), entries.end(),
              [](const DelayData& a, const DelayData& b) {
                  return a.totalDelay() > b.totalDelay();
              });

    std::cout << "\n===== DELAY SUMMARY BY AIRPORT & SEASON (sorted by total delay) =====\n";
    std::cout << std::left
              << std::setw(8)  << "Airport"
              << std::setw(10) << "Season"
              << std::setw(12) << "TotalDelay"
              << std::setw(16) << "DominantCause"
              << std::setw(10) << "Flights"
              << "\n";
    std::cout << std::string(56, '-') << "\n";

    for (const auto& e : entries) {
        std::cout << std::left
                  << std::setw(8)  << e.airport
                  << std::setw(10) << e.season
                  << std::setw(12) << static_cast<int>(e.totalDelay())
                  << std::setw(16) << e.dominantCause()
                  << std::setw(10) << e.num_flights
                  << "\n";
    }
}

void HashTable::printTopN(int n) const {
    std::vector<DelayData> entries;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table[i];
        while (current != nullptr) {
            entries.push_back(current->value);
            current = current->next;
        }
    }

    std::sort(entries.begin(), entries.end(),
              [](const DelayData& a, const DelayData& b) {
                  return a.totalDelay() > b.totalDelay();
              });

    int limit = std::min(n, static_cast<int>(entries.size()));
    std::cout << "\n===== TOP " << limit << " AIRPORT+SEASON COMBOS BY TOTAL DELAY =====\n";
    for (int i = 0; i < limit; i++) {
        std::cout << (i + 1) << ". ";
        entries[i].print();
        std::cout << "---------------------------------------------\n";
    }
}