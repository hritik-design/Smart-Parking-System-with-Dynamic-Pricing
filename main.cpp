// ============================================================
//  main.cpp  –  Smart Car Parking System  |  CLI Entry Point
//  Smart Car Parking System
//
//  Compile:
//    g++ -std=c++17 -Wall -Wextra -o SmartParking \
//        src/Vehicle.cpp src/ParkingSlot.cpp src/ParkingLevel.cpp \
//        src/Ticket.cpp src/PricingEngine.cpp src/Payment.cpp \
//        src/SensorSimulator.cpp src/ParkingLot.cpp main.cpp \
//        -I include
//
//  Run:
//    ./SmartParking
// ============================================================

#include "include/ParkingLot.h"
#include "include/PricingEngine.h"
#include "include/SensorSimulator.h"
#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <algorithm>

// ── Helpers ───────────────────────────────────────────────────
static void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static std::string toUpper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

static VehicleType chooseVehicleType() {
    std::cout << "  Vehicle type (1=Bike  2=Car  3=Truck): ";
    int ch;
    std::cin >> ch;
    clearInput();
    switch (ch) {
        case 1: return VehicleType::BIKE;
        case 3: return VehicleType::TRUCK;
        default:return VehicleType::CAR;
    }
}

static PaymentMethod choosePaymentMethod() {
    std::cout << "  Payment  (1=Cash  2=Card  3=UPI): ";
    int ch;
    std::cin >> ch;
    clearInput();
    switch (ch) {
        case 2: return PaymentMethod::CARD;
        case 3: return PaymentMethod::UPI;
        default:return PaymentMethod::CASH;
    }
}

// ── Main Menu ─────────────────────────────────────────────────
static void printMenu() {
    Utils::separator("=");
    std::cout << Color::BOLD << Color::BLUE
              << "         SMART CAR PARKING SYSTEM\n" << Color::RESET;
    Utils::separator("=");
    std::cout << "  " << Color::CYAN << "1." << Color::RESET << "  Park a Vehicle\n";
    std::cout << "  " << Color::CYAN << "2." << Color::RESET << "  Exit / Remove Vehicle\n";
    std::cout << "  " << Color::CYAN << "3." << Color::RESET << "  Dashboard Overview\n";
    std::cout << "  " << Color::CYAN << "4." << Color::RESET << "  View Slot Map (Grid)\n";
    std::cout << "  " << Color::CYAN << "5." << Color::RESET << "  Active Sessions\n";
    std::cout << "  " << Color::CYAN << "6." << Color::RESET << "  Search Vehicle\n";
    std::cout << "  " << Color::CYAN << "7." << Color::RESET << "  Parking History\n";
    std::cout << "  " << Color::CYAN << "8." << Color::RESET << "  Revenue Report\n";
    std::cout << "  " << Color::CYAN << "9." << Color::RESET << "  Sensor Event Log\n";
    std::cout << "  " << Color::CYAN << "10." << Color::RESET << " Pricing Configuration\n";
    std::cout << "  " << Color::CYAN << "11." << Color::RESET << " Toggle Pricing Mode\n";
    std::cout << "  " << Color::CYAN << "12." << Color::RESET << " Demo: Simulate 5 Vehicles\n";
    std::cout << "  " << Color::CYAN << "0." << Color::RESET  << "  Exit Program\n";
    Utils::separator("-");
    std::cout << "  Choice: ";
}

// ── Demo Simulation ───────────────────────────────────────────
static void runDemo(ParkingLot& lot) {
    Utils::header("DEMO – Simulating 5 Vehicle Entries");
    struct Demo { std::string plate; VehicleType type; std::string name; };
    Demo demos[] = {
        {"MH12AB1234", VehicleType::CAR,   "Amit Shah"},
        {"DL01CD5678", VehicleType::BIKE,  "Priya Nair"},
        {"KA05EF9012", VehicleType::TRUCK, "Rajan Pillai"},
        {"UP32GH3456", VehicleType::CAR,   "Sunita Verma"},
        {"TN09IJ7890", VehicleType::BIKE,  "Karthik Raj"}
    };
    for (auto& d : demos) {
        lot.parkVehicle(d.plate, d.type, d.name);
        std::cout << "\n";
    }
    std::cout << Color::GREEN
              << "  [DEMO] 5 vehicles parked. Now exiting first 2...\n\n"
              << Color::RESET;
    lot.exitVehicle("MH12AB1234", PaymentMethod::UPI);
    lot.exitVehicle("DL01CD5678", PaymentMethod::CARD);
    std::cout << "\n" << Color::GREEN
              << "  [DEMO] Simulation complete.\n" << Color::RESET;
}

// ── Entry Point ───────────────────────────────────────────────
int main() {
    // ── Setup Parking Lot (3 levels) ──────────────────────────
    ParkingLot lot("Smart City Parking – Sector 21");

    // Level 1: Ground  – 5 Compact, 10 Regular, 3 Large
    lot.addLevel(5, 10, 3);
    // Level 2: First   – 5 Compact, 8  Regular, 2 Large
    lot.addLevel(5, 8, 2);
    // Level 3: Rooftop – 0 Compact, 6  Regular, 2 Large
    lot.addLevel(0, 6, 2);

    // ── Welcome Banner ────────────────────────────────────────
    std::cout << "\n";
    Utils::header("SMART CAR PARKING SYSTEM  v1.0");
    std::cout << "\n  " << Color::YELLOW
              << "Welcome! " << lot.getName() << Color::RESET << "\n";
    std::cout << "  Total Slots : " << lot.totalSlots()
              << " across 3 levels\n\n";

    // ── Main Loop ─────────────────────────────────────────────
    bool running = true;
    while (running) {
        printMenu();

        int choice = -1;
        if (!(std::cin >> choice)) {
            choice = -1;
        }
        clearInput();
        std::cout << "\n";

        switch (choice) {
            // ── 1. Park Vehicle ───────────────────────────────
            case 1: {
                std::string plate, owner;
                std::cout << "  License Plate : ";
                std::getline(std::cin, plate);
                plate = toUpper(plate);
                std::cout << "  Owner Name    : ";
                std::getline(std::cin, owner);
                VehicleType vt = chooseVehicleType();
                lot.parkVehicle(plate, vt, owner);
                break;
            }

            // ── 2. Exit Vehicle ───────────────────────────────
            case 2: {
                std::string plate;
                std::cout << "  License Plate : ";
                std::getline(std::cin, plate);
                plate = toUpper(plate);
                PaymentMethod pm = choosePaymentMethod();
                lot.exitVehicle(plate, pm);
                break;
            }

            // ── 3. Dashboard ──────────────────────────────────
            case 3:
                lot.displayDashboard();
                break;

            // ── 4. Slot Map ───────────────────────────────────
            case 4:
                lot.displayGrid();
                break;

            // ── 5. Active Sessions ────────────────────────────
            case 5:
                lot.displayActiveTickets();
                break;

            // ── 6. Search Vehicle ─────────────────────────────
            case 6: {
                std::string plate;
                std::cout << "  License Plate to search: ";
                std::getline(std::cin, plate);
                plate = toUpper(plate);
                auto ticket = lot.findActiveTicket(plate);
                if (ticket) {
                    Utils::header("Vehicle Found");
                    ticket->getVehicle()->display();
                    std::cout << "  Slot    : " << ticket->getSlotID() << "\n";
                    std::cout << "  Entry   : " << Utils::formatTime(ticket->getEntryTime()) << "\n";
                    double mins = Utils::minutesBetween(ticket->getEntryTime(), Utils::now());
                    double est  = PricingEngine::getInstance().calculate(
                                      ticket->getVehicle()->getType(), mins,
                                      ticket->getEntryTime());
                    std::cout << "  Parked  : " << static_cast<int>(mins) << " min\n";
                    std::cout << "  Est Fee : Rs " << std::fixed
                              << std::setprecision(2) << est << "\n";
                    Utils::separator();
                } else {
                    std::cout << Color::RED << "  Vehicle " << plate
                              << " is not currently parked.\n" << Color::RESET;
                }
                break;
            }

            // ── 7. History ────────────────────────────────────
            case 7:
                lot.displayHistory();
                break;

            // ── 8. Revenue Report ─────────────────────────────
            case 8:
                lot.displayRevenueReport();
                break;

            // ── 9. Sensor Log ─────────────────────────────────
            case 9:
                SensorSimulator::getInstance().printLog();
                break;

            // ── 10. Pricing Config ────────────────────────────
            case 10:
                PricingEngine::getInstance().displayRates();
                break;

            // ── 11. Toggle Pricing Mode ───────────────────────
            case 11: {
                auto& pe = PricingEngine::getInstance();
                if (pe.getMode() == PricingMode::DYNAMIC) {
                    pe.setMode(PricingMode::FLAT);
                    std::cout << Color::YELLOW
                              << "  Switched to FLAT pricing mode.\n" << Color::RESET;
                } else {
                    pe.setMode(PricingMode::DYNAMIC);
                    std::cout << Color::YELLOW
                              << "  Switched to DYNAMIC pricing mode.\n" << Color::RESET;
                }
                break;
            }

            // ── 12. Demo Simulation ───────────────────────────
            case 12:
                runDemo(lot);
                break;

            // ── 0. Exit ───────────────────────────────────────
            case 0:
                std::cout << Color::GREEN
                          << "  Thank you for using Smart Parking. Goodbye!\n"
                          << Color::RESET;
                running = false;
                break;

            default:
                std::cout << Color::RED
                          << "  Invalid option. Please try again.\n"
                          << Color::RESET;
        }
        std::cout << "\n";
    }

    return 0;
}
