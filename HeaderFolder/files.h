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
    static void saveRoutes(std::ofstream& ofs, const TransportSystem& system);
    static void saveVehicles(std::ofstream& ofs, const TransportSystem& system);
    
    static void saveBuses(std::ofstream& ofs, const TransportSystem& system);
    static void saveTrolleybuses(std::ofstream& ofs, const TransportSystem& system);
    static void saveTrams(std::ofstream& ofs, const TransportSystem& system);

    static void loadDriver(std::ifstream& ifs, TransportSystem& system);

    static void loadVehicle(std::ifstream& ifs, TransportSystem& system);
    static void loadBus(std::ifstream& ifs, TransportSystem& system);
    static void loadTrolleybus(std::ifstream& ifs, TransportSystem& system);
    static void loadTram(std::ifstream& ifs, TransportSystem& system);

    static void loadRoute(std::ifstream& ifs, TransportSystem& system);

    static void reportDrivers(std::ofstream& ofs, const Collection<Driver>& drivers);
    static void reportVehicles(std::ofstream& ofs, const Collection<Vehicle>& vehicles);
    static void reportRoutes(std::ofstream& ofs, const Collection<Route>& routes);

    static constexpr const char* PREFIX_DRIVER  = "DRV";
    static constexpr const char* PREFIX_VEHICLE = "VEH";
    static constexpr const char* PREFIX_ROUTE   = "RTE";

    static constexpr const char* TYPE_BUS        = "Bus";
    static constexpr const char* TYPE_TROLLEYBUS = "Trolleybus";
    static constexpr const char* TYPE_TRAM       = "Tram";
};