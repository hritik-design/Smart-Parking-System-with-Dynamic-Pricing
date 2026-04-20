#pragma once
// ============================================================
//  ParkingSlot.h  –  Individual parking slot entity
//  Smart Car Parking System
// ============================================================
#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

#include "Utils.h"
#include <string>
#include <memory>

// Forward declaration
class Vehicle;

// ─────────────────────────────────────────────────────────────
//  class ParkingSlot
//  Represents one physical parking bay with sensor state.
// ─────────────────────────────────────────────────────────────
class ParkingSlot {
public:
    // ── Constructor ───────────────────────────────────────────
    ParkingSlot(int slotNumber, int level, SlotSize size);

    // ── Identity ──────────────────────────────────────────────
    std::string  getSlotID()     const;  // e.g. "L1-R07"
    int          getSlotNumber() const;
    int          getLevel()      const;
    SlotSize     getSize()       const;

    // ── Occupancy ─────────────────────────────────────────────
    bool         isOccupied()    const;
    bool         isSensorActive()const;  // smart sensor flag
    void         setSensorActive(bool active);

    // ── Vehicle Management ────────────────────────────────────
    bool         park(const std::string& licensePlate);
    bool         vacate();
    const std::string& getParkedLicense() const;

    // ── Display ───────────────────────────────────────────────
    void         display(bool compact = false) const;

    // ── Comparison for priority queue (min-heap by slot number)
    bool operator>(const ParkingSlot& other) const;
    bool operator<(const ParkingSlot& other) const;

private:
    int         m_slotNumber;
    int         m_level;
    SlotSize    m_size;
    bool        m_occupied     = false;
    bool        m_sensorActive = false;   // simulates IoT sensor
    std::string m_parkedLicense;

    // Prefix character for slot size
    char sizeLetter() const;
};

#endif // PARKINGSLOT_H
