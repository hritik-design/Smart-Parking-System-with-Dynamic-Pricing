#pragma once
// ============================================================
//  ParkingLevel.h  –  One floor of the parking structure
//  Smart Car Parking System
// ============================================================
#ifndef PARKINGLEVEL_H
#define PARKINGLEVEL_H

#include "ParkingSlot.h"
#include <vector>
#include <memory>
#include <optional>
#include <map>

// ─────────────────────────────────────────────────────────────
//  class ParkingLevel
//  Manages all slots on a single floor.
//  Uses a priority queue concept (sorted vector) to always
//  return the nearest (lowest-numbered) available slot.
// ─────────────────────────────────────────────────────────────
class ParkingLevel {
public:
    // ── Constructor ───────────────────────────────────────────
    ParkingLevel(int levelNumber,
                 int compactSlots,
                 int regularSlots,
                 int largeSlots);

    // ── Info ──────────────────────────────────────────────────
    int getLevelNumber() const;
    int totalSlots()     const;
    int availableSlots(SlotSize sz) const;
    int availableSlots()            const;   // total across sizes

    // ── Slot Access ───────────────────────────────────────────
    // Returns pointer to nearest available slot of given size, or nullptr
    ParkingSlot* findAvailableSlot(SlotSize sz);
    ParkingSlot* findSlotByID(const std::string& slotID);
    ParkingSlot* findSlotByLicense(const std::string& plate);

    const std::vector<ParkingSlot>& getSlots() const;
    std::vector<ParkingSlot>&       getSlots();

    // ── Display ───────────────────────────────────────────────
    void displayGrid()   const;
    void displayStatus() const;

private:
    int  m_levelNumber;
    std::vector<ParkingSlot> m_slots;   // all slots on this level

    void buildSlots(int compact, int regular, int large);
};

#endif // PARKINGLEVEL_H
