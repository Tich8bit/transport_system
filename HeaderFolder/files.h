#pragma once

#include <string>
#include <string_view>
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

    static std::vector<std::string> split(std::string_view str,
                                           char delimiter);
    static std::string trim(std::string_view str);
    static std::string replaceSpaces(std::string_view str);
    static std::string restoreSpaces(std::string_view str);

    static void saveDrivers(std::ofstream& ofs, const TransportSystem& system);
    static void saveVehicles(std::ofstream& ofs, const TransportSystem& system);
    static void saveRoutes(std::ofstream& ofs, const TransportSystem& system);

    static void loadDrivers(std::ifstream& ifs, TransportSystem& system);
    static void loadVehicles(std::ifstream& ifs, TransportSystem& system);
    static void loadVehicleLine(const std::string& line,
                                 TransportSystem& system);
    static void loadRoutes(std::ifstream& ifs, TransportSystem& system);
    static void loadRouteLine(const std::string& line,
                               TransportSystem& system);

    static int findDriverIndex(
        const std::vector<std::shared_ptr<Driver>>& drivers,
        const std::shared_ptr<Driver>& target);

    static int findVehicleIndex(
        const std::vector<std::shared_ptr<Vehicle>>& vehicles,
        const std::shared_ptr<Vehicle>& target);

    static void reportDrivers(std::ofstream& ofs, 
                       const std::vector<std::shared_ptr<Driver>>& drivers);
                       
    static void reportVehicles(std::ofstream& ofs, 
                        const std::vector<std::shared_ptr<Vehicle>>& vehicles);
                        
    static void reportRoutes(std::ofstream& ofs, 
                      const std::vector<std::shared_ptr<Route>>& routes);
};