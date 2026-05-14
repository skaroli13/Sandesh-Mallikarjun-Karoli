// main.cpp
// Sandesh Mallikarjun Karoli
// 013114596
// CURE-E Final Project: Flight Delay Analysis

#include "HashTable.h"
#include "DelayData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iomanip>

std::string monthToSeason(int month) {
    if (month == 12 || month == 1 || month == 2) return "Winter";
    if (month >= 3  && month <= 5)               return "Spring";
    if (month >= 6  && month <= 8)               return "Summer";
    return "Fall";
}

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n\"");
    size_t end   = s.find_last_not_of(" \t\r\n\"");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

double safeDouble(const std::string& s) {
    std::string t = trim(s);
    if (t.empty()) return 0.0;
    try { return std::stod(t); } catch (...) { return 0.0; }
}

int safeInt(const std::string& s) {
    std::string t = trim(s);
    if (t.empty()) return 0;
    try { return std::stoi(t); } catch (...) { return 0; }
}

std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(trim(field));
    return fields;
}

int loadCSV(const std::string& filename, HashTable& ht) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file: " << filename << "\n";
        return 0;
    }
    std::string headerLine;
    std::getline(file, headerLine);
    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> fields = parseCSVLine(line);
        if (fields.size() < 21) continue;
        int    month           = safeInt(fields[1]);
        std::string airport    = trim(fields[4]);
        double carrier_delay   = safeDouble(fields[16]);
        double weather_delay   = safeDouble(fields[17]);
        double nas_delay       = safeDouble(fields[18]);
        double security_delay  = safeDouble(fields[19]);
        double late_delay      = safeDouble(fields[20]);
        if (airport.empty() || month < 1 || month > 12) continue;
        std::string season = monthToSeason(month);
        std::string key    = airport + "_" + season;
        DelayData data(airport, season);
        data.carrier_delay       = carrier_delay;
        data.weather_delay       = weather_delay;
        data.nas_delay           = nas_delay;
        data.security_delay      = security_delay;
        data.late_aircraft_delay = late_delay;
        data.num_flights         = 1;
        ht.insert(key, data);
        count++;
    }
    file.close();
    return count;
}

void analyzeSeasons(const std::vector<DelayData>& allEntries) {
    std::map<std::string, DelayData> seasonTotals;
    std::vector<std::string> seasons = {"Winter", "Spring", "Summer", "Fall"};
    for (const auto& s : seasons) seasonTotals[s] = DelayData("ALL", s);

    for (const auto& e : allEntries) {
        auto& agg = seasonTotals[e.season];
        agg.carrier_delay       += e.carrier_delay;
        agg.weather_delay       += e.weather_delay;
        agg.nas_delay           += e.nas_delay;
        agg.security_delay      += e.security_delay;
        agg.late_aircraft_delay += e.late_aircraft_delay;
        agg.num_flights         += e.num_flights;
    }

    std::cout << "\n===== DELAY CAUSE BREAKDOWN BY SEASON =====\n";
    std::cout << std::left
              << std::setw(10) << "Season"
              << std::setw(14) << "Carrier"
              << std::setw(14) << "Weather"
              << std::setw(14) << "NAS"
              << std::setw(14) << "Security"
              << std::setw(16) << "LateAircraft"
              << std::setw(14) << "Total"
              << std::setw(16) << "DominantCause"
              << "\n";
    std::cout << std::string(112, '-') << "\n";

    for (const auto& s : seasons) {
        const auto& d = seasonTotals[s];
        std::cout << std::left
                  << std::setw(10) << s
                  << std::setw(14) << static_cast<long long>(d.carrier_delay)
                  << std::setw(14) << static_cast<long long>(d.weather_delay)
                  << std::setw(14) << static_cast<long long>(d.nas_delay)
                  << std::setw(14) << static_cast<long long>(d.security_delay)
                  << std::setw(16) << static_cast<long long>(d.late_aircraft_delay)
                  << std::setw(14) << static_cast<long long>(d.totalDelay())
                  << std::setw(16) << d.dominantCause()
                  << "\n";
    }
}

void analyzeAirport(HashTable& ht, const std::string& airport) {
    std::cout << "\n===== ANALYSIS FOR AIRPORT: " << airport << " =====\n";
    std::vector<std::string> seasons = {"Winter", "Spring", "Summer", "Fall"};
    bool found = false;
    for (const auto& s : seasons) {
        std::string key = airport + "_" + s;
        DelayData* d = ht.get(key);
        if (d != nullptr) {
            found = true;
            d->print();
            std::cout << "---------------------------------------------\n";
        }
    }
    if (!found) std::cout << "  No data found for airport: " << airport << "\n";
}

int main(int argc, char* argv[]) {
    std::string filename = "Airline_Delay_Cause.csv";
    if (argc > 1) filename = argv[1];

    std::cout << "================================================\n";
    std::cout << "  CURE-E Flight Delay Analysis\n";
    std::cout << "  Sandesh Mallikarjun Karoli | 013114596\n";
    std::cout << "  Research Q: What is the primary cause of\n";
    std::cout << "  flight delays at major U.S. airports, and\n";
    std::cout << "  how does it vary across seasons?\n";
    std::cout << "================================================\n";
    std::cout << "\nLoading data from: " << filename << " ...\n";

    HashTable ht;
    int rowsLoaded = loadCSV(filename, ht);

    if (rowsLoaded == 0) {
        std::cerr << "No data loaded. Check that the CSV file is present.\n";
        return 1;
    }

    std::cout << "Rows loaded: " << rowsLoaded << "\n";
    std::cout << "Unique airport+season combinations: " << ht.size() << "\n";

    std::vector<DelayData> allEntries;
    {
        std::ifstream file(filename);
        std::string headerLine;
        std::getline(file, headerLine);
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::vector<std::string> fields = parseCSVLine(line);
            if (fields.size() < 21) continue;
            int month = safeInt(fields[1]);
            std::string airport = trim(fields[4]);
            if (airport.empty() || month < 1 || month > 12) continue;
            DelayData d(airport, monthToSeason(month));
            d.carrier_delay       = safeDouble(fields[16]);
            d.weather_delay       = safeDouble(fields[17]);
            d.nas_delay           = safeDouble(fields[18]);
            d.security_delay      = safeDouble(fields[19]);
            d.late_aircraft_delay = safeDouble(fields[20]);
            d.num_flights         = 1;
            allEntries.push_back(d);
        }
    }

    analyzeSeasons(allEntries);
    ht.printTopN(10);

    std::vector<std::string> majorAirports = {"ATL", "LAX", "ORD", "DFW", "DEN"};
    for (const auto& ap : majorAirports) analyzeAirport(ht, ap);

    std::cout << "\n================================================\n";
    std::cout << "  Analysis complete.\n";
    std::cout << "================================================\n";

    return 0;
}