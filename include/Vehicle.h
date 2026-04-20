#pragma once
// ============================================================
//  Vehicle.h  –  Vehicle entity
//  Smart Car Parking System
// ============================================================
#ifndef VEHICLE_H
#define VEHICLE_H

#include "Utils.h"
#include <string>
#include <chrono>

// ─────────────────────────────────────────────────────────────
//  class Vehicle
//  Represents any vehicle entering the parking facility.
// ─────────────────────────────────────────────────────────────
class Vehicle {
public:
    // ── Constructors ─────────────────────────────────────────
    Vehicle(const std::string& licensePlate,
            VehicleType        type,
            const std::string& ownerName = "Unknown");

    // ── Getters ───────────────────────────────────────────────
    const std::string& getLicensePlate() const;
    VehicleType        getType()         const;
    const std::string& getOwnerName()    const;
    SlotSize           getRequiredSlot() const;

    // Entry time is stamped when the vehicle actually parks
    void setEntryTime(const std::chrono::system_clock::time_point& tp);
    const std::chrono::system_clock::time_point& getEntryTime() const;

    // ── Display ───────────────────────────────────────────────
    void display() const;

    // ── Operators ─────────────────────────────────────────────
    bool operator==(const Vehicle& other) const;

private:
    std::string  m_licensePlate;
    VehicleType  m_type;
    std::string  m_ownerName;
    SlotSize     m_requiredSlot;
    std::chrono::system_clock::time_point m_entryTime;
    bool         m_entryTimeSet = false;
};

#endif // VEHICLE_H
