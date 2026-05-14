# CURE-E Flight Delay Analysis

**Sandesh Mallikarjun Karoli**  
**Chico State / INGInious Username:** skaroli13  
**Course:** CSCI 211 Programming and Algorithms II, Spring 2026

---

## Research Question
What is the primary cause of flight delays at major U.S. airports, and how does it vary across different seasons?

## Stakeholders
- **Airlines** – identify delay causes to improve scheduling and operations
- **Airports** – manage traffic and reduce congestion-related delays
- **Passengers** – make informed decisions about travel timing and airline choices
- **Government agencies (FAA)** – monitor delay patterns and improve aviation infrastructure

---

## Files and Directories
| File | Description |
|------|-------------|
| `main.cpp` | Main program; loads CSV, runs analysis |
| `HashTable.h` | Header for custom hash table ADT |
| `HashTable.cpp` | Hash table implementation (separate chaining) |
| `DelayData.h` | Struct for delay data records |
| `Makefile` | Build instructions |

---

## How to Compile and Run

### Compile
```bash
make
```
Or manually:
```bash
g++ -std=c++11 -Wall -o flight-delay-analysis main.cpp HashTable.cpp
```

### Run
```bash
./flight-delay-analysis Airline_Delay_Cause.csv
```

---

## Data Structure
I implemented a **custom hash table** using **separate chaining**.
- **Key:** airport + season (e.g. `ATL_Summer`)
- **Value:** DelayData struct with cumulative delay minutes per cause
- **Hash function:** djb2
- **Table size:** 1021 (prime number)

---
## Key Findings
- **Late Aircraft** is the #1 delay cause in every season
- **Summer** has the highest total delays (243M minutes)
- **ORD Chicago in Summer** is the single worst airport+season combo
- **Weather** is actually the 2nd smallest delay category
- **Fall** is the least delayed season overall (132M minutes)
- **171,666** rows of real data analyzed across **1,553** airport+season combinations

---

## How to Interpret Output
1. **Season Breakdown** – total delay minutes per cause across all airports
2. **Top 10 Worst Combos** – ranked by total delay minutes
3. **Per-Airport Analysis** – breakdown for ATL, LAX, ORD, DFW, DEN
