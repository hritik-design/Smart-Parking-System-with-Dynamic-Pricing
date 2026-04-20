// ============================================================
//  Vehicle.cpp  –  Vehicle entity implementation
//  Smart Car Parking System
// ============================================================
#include "../include/Vehicle.h"
#include <iostream>
#include <stdexcept>

// ── Constructor ───────────────────────────────────────────────
Vehicle::Vehicle(const std::string& licensePlate,
                 VehicleType        type,
                 const std::string& ownerName)
    : m_licensePlate(licensePlate),
      m_type(type),
      m_ownerName(ownerName),
      m_requiredSlot(Utils::requiredSlot(type))
{
    if (licensePlate.empty())
        throw std::invalid_argument("License plate cannot be empty.");
}

// ── Getters ───────────────────────────────────────────────────
const std::string& Vehicle::getLicensePlate() const { return m_licensePlate; }
VehicleType        Vehicle::getType()         const { return m_type;         }
const std::string& Vehicle::getOwnerName()    const { return m_ownerName;    }
SlotSize           Vehicle::getRequiredSlot() const { return m_requiredSlot; }

void Vehicle::setEntryTime(const std::chrono::system_clock::time_point& tp) {
    m_entryTime    = tp;
    m_entryTimeSet = true;
}

const std::chrono::system_clock::time_point& Vehicle::getEntryTime() const {
    return m_entryTime;
}

// ── Display ───────────────────────────────────────────────────
void Vehicle::display() const {
    std::cout << Color::BOLD << "  Vehicle Info\n" << Color::RESET;
    std::cout << "  Plate  : " << Color::YELLOW << m_licensePlate << Color::RESET << "\n";
    std::cout << "  Type   : " << Utils::vehicleTypeStr(m_type) << "\n";
    std::cout << "  Owner  : " << m_ownerName << "\n";
    std::cout << "  Slot   : " << Utils::slotSizeStr(m_requiredSlot) << "\n";
    if (m_entryTimeSet)
        std::cout << "  Entry  : " << Utils::formatTime(m_entryTime) << "\n";
}

// ── Equality ─────────────────────────────────────────────────
bool Vehicle::operator==(const Vehicle& other) const {
    return m_licensePlate == other.m_licensePlate;
}
