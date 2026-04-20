// ============================================================
//  ParkingSlot.cpp  –  Parking slot implementation
//  Smart Car Parking System
// ============================================================
#include "../include/ParkingSlot.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// ── Constructor ───────────────────────────────────────────────
ParkingSlot::ParkingSlot(int slotNumber, int level, SlotSize size)
    : m_slotNumber(slotNumber), m_level(level), m_size(size) {}

// ── Identity ──────────────────────────────────────────────────
char ParkingSlot::sizeLetter() const {
    switch (m_size) {
        case SlotSize::COMPACT: return 'C';
        case SlotSize::REGULAR: return 'R';
        case SlotSize::LARGE:   return 'L';
    }
    return '?';
}

std::string ParkingSlot::getSlotID() const {
    std::ostringstream oss;
    oss << "L" << m_level << "-" << sizeLetter()
        << std::setw(2) << std::setfill('0') << m_slotNumber;
    return oss.str();
}

int      ParkingSlot::getSlotNumber() const { return m_slotNumber;    }
int      ParkingSlot::getLevel()      const { return m_level;         }
SlotSize ParkingSlot::getSize()       const { return m_size;          }

// ── Occupancy ─────────────────────────────────────────────────
bool ParkingSlot::isOccupied()     const { return m_occupied;     }
bool ParkingSlot::isSensorActive() const { return m_sensorActive; }

void ParkingSlot::setSensorActive(bool active) {
    m_sensorActive = active;
}

// ── Vehicle Management ────────────────────────────────────────
bool ParkingSlot::park(const std::string& licensePlate) {
    if (m_occupied) return false;
    m_occupied      = true;
    m_sensorActive  = true;
    m_parkedLicense = licensePlate;
    return true;
}

bool ParkingSlot::vacate() {
    if (!m_occupied) return false;
    m_occupied      = false;
    m_sensorActive  = false;
    m_parkedLicense.clear();
    return true;
}

const std::string& ParkingSlot::getParkedLicense() const {
    return m_parkedLicense;
}

// ── Display ───────────────────────────────────────────────────
void ParkingSlot::display(bool compact) const {
    if (compact) {
        // Visual grid cell
        if (m_occupied)
            std::cout << Color::BG_RED << Color::WHITE
                      << "[" << getSlotID() << "]" << Color::RESET;
        else
            std::cout << Color::BG_GRN << Color::WHITE
                      << "[" << getSlotID() << "]" << Color::RESET;
        return;
    }
    std::cout << "  Slot ID : " << Color::CYAN << getSlotID() << Color::RESET << "\n";
    std::cout << "  Level   : " << m_level << "\n";
    std::cout << "  Size    : " << Utils::slotSizeStr(m_size) << "\n";
    std::cout << "  Status  : ";
    if (m_occupied) {
        std::cout << Color::RED << "OCCUPIED" << Color::RESET
                  << "  [" << m_parkedLicense << "]\n";
    } else {
        std::cout << Color::GREEN << "AVAILABLE" << Color::RESET << "\n";
    }
    std::cout << "  Sensor  : " << (m_sensorActive ? "ACTIVE" : "IDLE") << "\n";
}

// ── Comparators ───────────────────────────────────────────────
bool ParkingSlot::operator>(const ParkingSlot& o) const {
    if (m_level != o.m_level) return m_level > o.m_level;
    return m_slotNumber > o.m_slotNumber;
}
bool ParkingSlot::operator<(const ParkingSlot& o) const {
    if (m_level != o.m_level) return m_level < o.m_level;
    return m_slotNumber < o.m_slotNumber;
}
