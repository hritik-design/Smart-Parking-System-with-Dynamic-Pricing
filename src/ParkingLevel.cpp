// ============================================================
//  ParkingLevel.cpp  –  ParkingLevel implementation
//  Smart Car Parking System
// ============================================================
#include "../include/ParkingLevel.h"
#include <iostream>
#include <algorithm>

// ── Constructor ───────────────────────────────────────────────
ParkingLevel::ParkingLevel(int levelNumber,
                           int compactSlots,
                           int regularSlots,
                           int largeSlots)
    : m_levelNumber(levelNumber)
{
    buildSlots(compactSlots, regularSlots, largeSlots);
}

// ── Build slots on this level ─────────────────────────────────
void ParkingLevel::buildSlots(int compact, int regular, int large) {
    int slotNum = 1;
    for (int i = 0; i < compact; ++i, ++slotNum)
        m_slots.emplace_back(slotNum, m_levelNumber, SlotSize::COMPACT);
    for (int i = 0; i < regular; ++i, ++slotNum)
        m_slots.emplace_back(slotNum, m_levelNumber, SlotSize::REGULAR);
    for (int i = 0; i < large; ++i, ++slotNum)
        m_slots.emplace_back(slotNum, m_levelNumber, SlotSize::LARGE);
}

// ── Info ──────────────────────────────────────────────────────
int ParkingLevel::getLevelNumber() const { return m_levelNumber; }
int ParkingLevel::totalSlots()     const { return static_cast<int>(m_slots.size()); }

int ParkingLevel::availableSlots(SlotSize sz) const {
    int count = 0;
    for (const auto& s : m_slots)
        if (s.getSize() == sz && !s.isOccupied()) ++count;
    return count;
}

int ParkingLevel::availableSlots() const {
    int count = 0;
    for (const auto& s : m_slots)
        if (!s.isOccupied()) ++count;
    return count;
}

// ── Find nearest available slot of given size ─────────────────
// Slots are stored in ascending slotNumber order so the first
// matching available slot is always the nearest.
ParkingSlot* ParkingLevel::findAvailableSlot(SlotSize sz) {
    for (auto& s : m_slots)
        if (s.getSize() == sz && !s.isOccupied())
            return &s;
    return nullptr;
}

// ── Find slot by its ID string ────────────────────────────────
ParkingSlot* ParkingLevel::findSlotByID(const std::string& slotID) {
    for (auto& s : m_slots)
        if (s.getSlotID() == slotID) return &s;
    return nullptr;
}

// ── Find slot by parked license plate ────────────────────────
ParkingSlot* ParkingLevel::findSlotByLicense(const std::string& plate) {
    for (auto& s : m_slots)
        if (s.isOccupied() && s.getParkedLicense() == plate) return &s;
    return nullptr;
}

// ── Accessors ─────────────────────────────────────────────────
const std::vector<ParkingSlot>& ParkingLevel::getSlots() const { return m_slots; }
std::vector<ParkingSlot>&       ParkingLevel::getSlots()       { return m_slots; }

// ── Display: coloured grid ────────────────────────────────────
void ParkingLevel::displayGrid() const {
    std::cout << "\n" << Color::BOLD << "  Level " << m_levelNumber
              << "  (" << availableSlots() << "/" << totalSlots() << " free)\n"
              << Color::RESET;
    Utils::separator(".");

    int col = 0;
    for (const auto& s : m_slots) {
        std::cout << "  ";
        s.display(true);
        if (++col % 5 == 0) std::cout << "\n";
    }
    if (col % 5 != 0) std::cout << "\n";

    // Legend
    std::cout << "\n  Legend: "
              << Color::BG_GRN << Color::WHITE << "[FREE]" << Color::RESET << "  "
              << Color::BG_RED << Color::WHITE << "[BUSY]" << Color::RESET << "\n";
    Utils::separator(".");
}

// ── Display: text status ──────────────────────────────────────
void ParkingLevel::displayStatus() const {
    std::cout << Color::BOLD << "\n  === Level " << m_levelNumber << " ===\n" << Color::RESET;
    std::cout << "  Compact  (Bike)  : "
              << availableSlots(SlotSize::COMPACT) << " free\n";
    std::cout << "  Regular  (Car)   : "
              << availableSlots(SlotSize::REGULAR) << " free\n";
    std::cout << "  Large    (Truck) : "
              << availableSlots(SlotSize::LARGE)   << " free\n";
    std::cout << "  Total            : "
              << availableSlots() << " / " << totalSlots() << " free\n";
}
