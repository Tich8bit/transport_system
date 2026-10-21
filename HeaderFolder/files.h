#pragma once

#include <fstream>
#include <string>
#include "transport_system.h"
#include "collection.h"

class StorageManager {
public:
    static void saveState(const std::string& filename, const TransportSystem& system);
    static void loadState(const std::string& filename, TransportSystem& system);
    static void logAction(const std::string& logFilename, const std::string& action);
    static void generateReport(const std::string& reportFilename, const TransportSystem& system);

private:
    static void saveDrivers(std::ofstream& ofs, const TransportSystem& system);
    static void saveVehicles(std::ofstream& ofs, const TransportSystem& system);
    static void saveRoutes(std::ofstream& ofs, const TransportSystem& system);

    static void loadDriverLine(const std::string& line, TransportSystem& system);
    static void loadVehicleLine(const std::string& line, TransportSystem& system);
    static void loadRouteLine(const std::string& line, TransportSystem& system);

    static std::string replaceSpaces(const std::string& str);
    static std::string restoreSpaces(const std::string& str);

    static void reportDrivers(std::ofstream& ofs, const Collection<Driver>& drivers);
    static void reportVehicles(std::ofstream& ofs, const Collection<Vehicle>& vehicles);
    static void reportRoutes(std::ofstream& ofs, const Collection<Route>& routes);
};