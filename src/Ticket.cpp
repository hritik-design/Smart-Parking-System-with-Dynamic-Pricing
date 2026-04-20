// ============================================================
//  Ticket.cpp  –  Ticket implementation
//  Smart Car Parking System
// ============================================================
#include "../include/Ticket.h"
#include <iostream>
#include <iomanip>

// ── Constructor ───────────────────────────────────────────────
Ticket::Ticket(int ticketSeq,
               std::shared_ptr<Vehicle> vehicle,
               const std::string& slotID,
               const std::chrono::system_clock::time_point& entryTime)
    : m_ticketID(Utils::generateID("TKT-", ticketSeq)),
      m_slotID(slotID),
      m_vehicle(std::move(vehicle)),
      m_entryTime(entryTime)
{}

// ── Getters ───────────────────────────────────────────────────
const std::string& Ticket::getTicketID()   const { return m_ticketID; }
const std::string& Ticket::getSlotID()     const { return m_slotID;   }
std::shared_ptr<Vehicle> Ticket::getVehicle() const { return m_vehicle; }

const std::chrono::system_clock::time_point& Ticket::getEntryTime() const {
    return m_entryTime;
}
const std::chrono::system_clock::time_point& Ticket::getExitTime() const {
    return m_exitTime;
}

bool Ticket::isSettled() const { return m_settled; }

double Ticket::getDurationMin() const {
    if (!m_settled) return 0.0;
    return Utils::minutesBetween(m_entryTime, m_exitTime);
}

double Ticket::getAmountDue() const { return m_amountDue; }

// ── Close ticket (called on exit) ────────────────────────────
void Ticket::close(const std::chrono::system_clock::time_point& exitTime, double amount) {
    m_exitTime  = exitTime;
    m_amountDue = amount;
    m_settled   = true;
}

// ── Print Entry Ticket ────────────────────────────────────────
void Ticket::printTicket() const {
    Utils::separator("-");
    std::cout << Color::BOLD << Color::YELLOW
              << "       SMART PARKING  –  ENTRY TICKET\n" << Color::RESET;
    Utils::separator("-");
    std::cout << "  Ticket ID : " << Color::CYAN << m_ticketID << Color::RESET << "\n";
    std::cout << "  Slot      : " << Color::GREEN << m_slotID << Color::RESET << "\n";
    std::cout << "  Plate     : " << m_vehicle->getLicensePlate() << "\n";
    std::cout << "  Owner     : " << m_vehicle->getOwnerName() << "\n";
    std::cout << "  Type      : " << Utils::vehicleTypeStr(m_vehicle->getType()) << "\n";
    std::cout << "  Entry     : " << Utils::formatTime(m_entryTime) << "\n";
    Utils::separator("-");
    std::cout << Color::YELLOW << "  Please keep this ticket for exit.\n" << Color::RESET;
    Utils::separator("-");
}

// ── Print Exit Receipt ────────────────────────────────────────
void Ticket::printReceipt() const {
    if (!m_settled) {
        std::cout << Color::RED << "  Ticket not yet settled.\n" << Color::RESET;
        return;
    }
    double hours   = getDurationMin() / 60.0;
    double minutes = getDurationMin();

    Utils::separator("=");
    std::cout << Color::BOLD << Color::GREEN
              << "       SMART PARKING  –  EXIT RECEIPT\n" << Color::RESET;
    Utils::separator("=");
    std::cout << "  Ticket ID : " << Color::CYAN << m_ticketID << Color::RESET << "\n";
    std::cout << "  Slot      : " << m_slotID << "\n";
    std::cout << "  Plate     : " << m_vehicle->getLicensePlate() << "\n";
    std::cout << "  Owner     : " << m_vehicle->getOwnerName() << "\n";
    std::cout << "  Type      : " << Utils::vehicleTypeStr(m_vehicle->getType()) << "\n";
    Utils::separator("-");
    std::cout << "  Entry     : " << Utils::formatTime(m_entryTime) << "\n";
    std::cout << "  Exit      : " << Utils::formatTime(m_exitTime)  << "\n";
    std::cout << "  Duration  : " << std::fixed << std::setprecision(1)
              << minutes << " min  (" << hours << " hr)\n";
    Utils::separator("-");
    std::cout << "  " << Color::BOLD << Color::GREEN
              << "Amount Due : Rs " << std::fixed << std::setprecision(2)
              << m_amountDue << Color::RESET << "\n";
    Utils::separator("=");
    std::cout << Color::YELLOW << "  Thank you! Drive safely.\n" << Color::RESET;
    Utils::separator("=");
}
