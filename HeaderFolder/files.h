#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "transport_system.h"
#include "exceptions/exception.h"

class StorageManager {
public:
    static void saveState(const std::string& filename,
                          const TransportSystem& system);
    static void loadState(const std::string& filename,
                          TransportSystem& system);
    static void logAction(const std::string& logFilename,
                          const std::string& action);
    static void generateReport(const std::string& reportFilename,
                               const TransportSystem& system);

private:
    static constexpr const char* PREFIX_DRIVER  = "DRV";
    static constexpr const char* PREFIX_VEHICLE = "VEH";
    static constexpr const char* PREFIX_ROUTE   = "RTE";

    static constexpr const char* TYPE_BUS        = "Bus";
    static constexpr const char* TYPE_TROLLEYBUS = "Trolleybus";
    static constexpr const char* TYPE_TRAM       = "Tram";

    static std::vector<std::string> split(const std::string& str,
                                           char delimiter);
    static std::string trim(const std::string& str);
    static std::string replaceSpaces(const std::string& str);
    static std::string restoreSpaces(const std::string& str);
};