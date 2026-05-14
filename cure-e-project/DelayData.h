// DelayData.h
// Sandesh Mallikarjun Karoli
// 013114596
// Struct representing one aggregated delay record (keyed by airport + season)

#ifndef DELAYDATA_H
#define DELAYDATA_H

#include <string>
#include <iostream>

struct DelayData {
    std::string airport;
    std::string season;
    double carrier_delay;
    double weather_delay;
    double nas_delay;
    double security_delay;
    double late_aircraft_delay;
    int num_flights;

    DelayData()
        : airport(""), season(""), carrier_delay(0.0), weather_delay(0.0),
          nas_delay(0.0), security_delay(0.0), late_aircraft_delay(0.0),
          num_flights(0) {}

    DelayData(std::string ap, std::string s)
        : airport(ap), season(s), carrier_delay(0.0), weather_delay(0.0),
          nas_delay(0.0), security_delay(0.0), late_aircraft_delay(0.0),
          num_flights(0) {}

    double totalDelay() const {
        return carrier_delay + weather_delay + nas_delay +
               security_delay + late_aircraft_delay;
    }

    std::string dominantCause() const {
        double maxVal = carrier_delay;
        std::string cause = "Carrier";
        if (weather_delay > maxVal)       { maxVal = weather_delay;       cause = "Weather"; }
        if (nas_delay > maxVal)           { maxVal = nas_delay;           cause = "NAS"; }
        if (security_delay > maxVal)      { maxVal = security_delay;      cause = "Security"; }
        if (late_aircraft_delay > maxVal) { maxVal = late_aircraft_delay; cause = "Late Aircraft"; }
        return cause;
    }
// Prints a formatted summary of all delay fields for this airport+season combo
// Includes total delay and dominant cause for easy analysis
    void print() const {
        std::cout << "Airport: " << airport << " | Season: " << season << "\n";
        std::cout << "  Carrier Delay:       " << carrier_delay       << " min\n";
        std::cout << "  Weather Delay:       " << weather_delay       << " min\n";
        std::cout << "  NAS Delay:           " << nas_delay           << " min\n";
        std::cout << "  Security Delay:      " << security_delay      << " min\n";
        std::cout << "  Late Aircraft Delay: " << late_aircraft_delay << " min\n";
        std::cout << "  Total Delay:         " << totalDelay()        << " min\n";
        std::cout << "  Dominant Cause:      " << dominantCause()     << "\n";
        std::cout << "  Flights Counted:     " << num_flights         << "\n";
    }
};

#endif