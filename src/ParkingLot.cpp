// ============================================================
//  ParkingLot.cpp  –  ParkingLot implementation
//  Smart Car Parking System
// ============================================================
#include "../include/ParkingLot.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// ── Constructor ───────────────────────────────────────────────
ParkingLot::ParkingLot(const std::string& name) : m_name(name) {
    registerSensorListener();
}

// ── Register sensor event listener ───────────────────────────
void ParkingLot::registerSensorListener() {
    SensorSimulator::getInstance().addListener(
        [](const SensorReading& r) {
            // Optionally print sensor events in real time
            // (commented out to reduce noise; enable for demo)
            // std::cout << Color::MAGENTA << "[SENSOR] " << r.timestamp
            //           << "  " << r.slotID << "  " << r.detail << Color::RESET << "\n";
        }
    );
}

// ── Add a new level ───────────────────────────────────────────
void ParkingLot::addLevel(int compact, int regular, int large) {
    int levelNum = static_cast<int>(m_levels.size()) + 1;
    m_levels.push_back(
        std::make_unique<ParkingLevel>(levelNum, compact, regular, large));
}

// ── Find level with a free slot of given size ─────────────────
ParkingLevel* ParkingLot::findLevelWithSlot(SlotSize sz) {
    for (auto& lvl : m_levels)
        if (lvl->availableSlots(sz) > 0) return lvl.get();
    return nullptr;
}

// ── Park Vehicle ──────────────────────────────────────────────
std::shared_ptr<Ticket> ParkingLot::parkVehicle(const std::string& plate,
                                                  VehicleType type,
                                                  const std::string& ownerName)
{
    // Duplicate check
    if (m_activeTickets.count(plate)) {
        std::cout << Color::RED
                  << "  [ERROR] Vehicle " << plate << " is already parked!\n"
                  << Color::RESET;
        return nullptr;
    }

    SlotSize required = Utils::requiredSlot(type);

    // Find level with free slot
    ParkingLevel* level = findLevelWithSlot(required);
    if (!level) {
        std::cout << Color::RED
                  << "  [ERROR] Parking FULL for "
                  << Utils::slotSizeStr(required) << " slots!\n"
                  << Color::RESET;
        return nullptr;
    }

    // Get nearest slot
    ParkingSlot* slot = level->findAvailableSlot(required);
    if (!slot) {
        std::cout << Color::RED << "  [ERROR] No slot available.\n" << Color::RESET;
        return nullptr;
    }

    // Create vehicle and stamp entry time
    auto entryTime = Utils::now();
    auto vehicle   = std::make_shared<Vehicle>(plate, type, ownerName);
    vehicle->setEntryTime(entryTime);

    // Occupy slot
    slot->park(plate);

    // Issue ticket
    auto ticket = std::make_shared<Ticket>(++m_ticketCounter, vehicle,
                                            slot->getSlotID(), entryTime);
    m_activeTickets[plate] = ticket;

    // Fire sensor event
    SensorSimulator::getInstance().emitVehicleDetected(slot->getSlotID(), plate);

    std::cout << Color::GREEN
              << "  [OK] Vehicle " << plate << " parked at "
              << slot->getSlotID() << "\n" << Color::RESET;
    ticket->printTicket();
    return ticket;
}

// ── Exit Vehicle ──────────────────────────────────────────────
std::shared_ptr<Ticket> ParkingLot::exitVehicle(const std::string& plate,
                                                  PaymentMethod method)
{
    auto it = m_activeTickets.find(plate);
    if (it == m_activeTickets.end()) {
        std::cout << Color::RED
                  << "  [ERROR] Vehicle " << plate << " not found in active records.\n"
                  << Color::RESET;
        return nullptr;
    }

    auto ticket   = it->second;
    auto exitTime = Utils::now();

    // Compute duration and fee
    double minutes = Utils::minutesBetween(ticket->getEntryTime(), exitTime);
    double fee     = PricingEngine::getInstance().calculate(
                        ticket->getVehicle()->getType(), minutes,
                        ticket->getEntryTime());

    // Close ticket
    ticket->close(exitTime, fee);

    // Process payment
    Payment payment(ticket, method);
    payment.process();
    m_totalRevenue += fee;

    // Find and vacate slot
    std::string slotID = ticket->getSlotID();
    for (auto& lvl : m_levels) {
        ParkingSlot* s = lvl->findSlotByID(slotID);
        if (s) { s->vacate(); break; }
    }

    // Fire sensor event
    SensorSimulator::getInstance().emitSlotCleared(slotID);

    // Move to history
    m_history.push_back(ticket);
    m_activeTickets.erase(it);

    ticket->printReceipt();
    payment.printSummary();
    return ticket;
}

// ── Search ────────────────────────────────────────────────────
std::shared_ptr<Ticket> ParkingLot::findActiveTicket(const std::string& plate) const {
    auto it = m_activeTickets.find(plate);
    return (it != m_activeTickets.end()) ? it->second : nullptr;
}

ParkingSlot* ParkingLot::findSlot(const std::string& slotID) {
    for (auto& lvl : m_levels) {
        ParkingSlot* s = lvl->findSlotByID(slotID);
        if (s) return s;
    }
    return nullptr;
}

// ── Statistics ────────────────────────────────────────────────
int ParkingLot::totalSlots() const {
    int t = 0;
    for (const auto& lvl : m_levels) t += lvl->totalSlots();
    return t;
}

int ParkingLot::availableSlots() const {
    int t = 0;
    for (const auto& lvl : m_levels) t += lvl->availableSlots();
    return t;
}

int ParkingLot::occupiedSlots() const {
    return totalSlots() - availableSlots();
}

double ParkingLot::totalRevenue() const { return m_totalRevenue; }

const std::string& ParkingLot::getName() const { return m_name; }

// ── Dashboard ─────────────────────────────────────────────────
void ParkingLot::displayDashboard() const {
    Utils::header(m_name + "  –  DASHBOARD");

    int total  = totalSlots();
    int avail  = availableSlots();
    int occ    = occupiedSlots();
    double pct = total ? (100.0 * occ / total) : 0.0;

    std::cout << "\n";
    std::cout << "  Levels      : " << m_levels.size()  << "\n";
    std::cout << "  Total Slots : " << total             << "\n";
    std::cout << "  Available   : " << Color::GREEN << avail << Color::RESET << "\n";
    std::cout << "  Occupied    : " << Color::RED   << occ   << Color::RESET << "\n";
    std::cout << "  Occupancy   : " << std::fixed << std::setprecision(1)
              << pct << "%\n";

    // Occupancy bar
    int barFull  = static_cast<int>(pct / 2.5);
    int barEmpty = 40 - barFull;
    std::cout << "  [";
    std::cout << Color::RED;
    for (int i = 0; i < barFull;  ++i) std::cout << "█";
    std::cout << Color::GREEN;
    for (int i = 0; i < barEmpty; ++i) std::cout << "░";
    std::cout << Color::RESET << "]\n\n";

    std::cout << "  Active Txns : " << m_activeTickets.size() << "\n";
    std::cout << "  Revenue     : Rs " << std::fixed << std::setprecision(2)
              << m_totalRevenue << "\n";
    std::cout << "\n";

    // Per-level summary
    for (const auto& lvl : m_levels)
        lvl->displayStatus();

    Utils::separator();
}

// ── All Levels Grid ───────────────────────────────────────────
void ParkingLot::displayGrid() const {
    Utils::header(m_name + "  –  SLOT MAP");
    for (const auto& lvl : m_levels)
        lvl->displayGrid();
}

// ── Active Tickets ────────────────────────────────────────────
void ParkingLot::displayActiveTickets() const {
    Utils::header("Active Parking Sessions");
    if (m_activeTickets.empty()) {
        std::cout << "  No active sessions.\n";
        Utils::separator();
        return;
    }

    auto now = Utils::now();
    std::cout << std::left
              << "  " << std::setw(12) << "Plate"
              << std::setw(10) << "Slot"
              << std::setw(10) << "Type"
              << std::setw(22) << "Entry"
              << std::setw(10) << "Duration"
              << "Est. Fee\n";
    Utils::separator("-");

    for (const auto& [plate, ticket] : m_activeTickets) {
        double mins = Utils::minutesBetween(ticket->getEntryTime(), now);
        double estFee = PricingEngine::getInstance().calculate(
                            ticket->getVehicle()->getType(), mins,
                            ticket->getEntryTime());
        std::cout << "  " << std::setw(12) << plate
                  << std::setw(10) << ticket->getSlotID()
                  << std::setw(10) << Utils::vehicleTypeStr(ticket->getVehicle()->getType())
                  << std::setw(22) << Utils::formatTime(ticket->getEntryTime())
                  << std::setw(10) << (std::to_string(static_cast<int>(mins)) + " min")
                  << "Rs " << std::fixed << std::setprecision(2) << estFee
                  << "\n";
    }
    Utils::separator();
}

// ── History ───────────────────────────────────────────────────
void ParkingLot::displayHistory() const {
    Utils::header("Parking History");
    if (m_history.empty()) {
        std::cout << "  No completed sessions yet.\n";
        Utils::separator();
        return;
    }

    std::cout << std::left
              << "  " << std::setw(14) << "Ticket"
              << std::setw(12) << "Plate"
              << std::setw(10) << "Slot"
              << std::setw(12) << "Duration"
              << "Amount\n";
    Utils::separator("-");

    for (const auto& t : m_history) {
        std::cout << "  " << std::setw(14) << t->getTicketID()
                  << std::setw(12) << t->getVehicle()->getLicensePlate()
                  << std::setw(10) << t->getSlotID()
                  << std::setw(12) << (std::to_string(static_cast<int>(t->getDurationMin())) + " min")
                  << "Rs " << std::fixed << std::setprecision(2) << t->getAmountDue()
                  << "\n";
    }
    Utils::separator();
}

// ── Revenue Report ────────────────────────────────────────────
void ParkingLot::displayRevenueReport() const {
    Utils::header("Revenue Report");

    double bikeRev = 0, carRev = 0, truckRev = 0;
    int bikeCount = 0, carCount = 0, truckCount = 0;

    for (const auto& t : m_history) {
        switch (t->getVehicle()->getType()) {
            case VehicleType::BIKE:  bikeRev  += t->getAmountDue(); bikeCount++;  break;
            case VehicleType::CAR:   carRev   += t->getAmountDue(); carCount++;   break;
            case VehicleType::TRUCK: truckRev += t->getAmountDue(); truckCount++; break;
        }
    }

    std::cout << "  Completed sessions : " << m_history.size()       << "\n";
    std::cout << "  Active sessions    : " << m_activeTickets.size() << "\n\n";
    std::cout << "  Bikes  (" << bikeCount  << " txns) : Rs " << std::fixed
              << std::setprecision(2) << bikeRev  << "\n";
    std::cout << "  Cars   (" << carCount   << " txns) : Rs " << carRev   << "\n";
    std::cout << "  Trucks (" << truckCount << " txns) : Rs " << truckRev << "\n";
    Utils::separator("-");
    std::cout << "  " << Color::BOLD << Color::GREEN
              << "TOTAL REVENUE       : Rs " << m_totalRevenue
              << Color::RESET << "\n";
    Utils::separator();
}
