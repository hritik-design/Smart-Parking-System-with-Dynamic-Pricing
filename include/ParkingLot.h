#pragma once
// ============================================================
//  ParkingLot.h  –  Central parking management system
//  Smart Car Parking System
// ============================================================
#ifndef PARKINGLOT_H
#define PARKINGLOT_H

#include "ParkingLevel.h"
#include "Vehicle.h"
#include "Ticket.h"
#include "Payment.h"
#include "PricingEngine.h"
#include "SensorSimulator.h"
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <optional>

// ─────────────────────────────────────────────────────────────
//  class ParkingLot
//  Top-level manager: orchestrates levels, vehicles, tickets,
//  payments and sensor events.
// ─────────────────────────────────────────────────────────────
class ParkingLot {
public:
    // ── Constructor ───────────────────────────────────────────
    explicit ParkingLot(const std::string& name);

    // ── Configuration ─────────────────────────────────────────
    // Add a level with given slot counts
    void addLevel(int compactSlots, int regularSlots, int largeSlots);

    // ── Core Operations ───────────────────────────────────────
    // Park a vehicle → issues ticket; returns ticket or nullptr on failure
    std::shared_ptr<Ticket> parkVehicle(const std::string& licensePlate,
                                        VehicleType type,
                                        const std::string& ownerName = "Guest");

    // Exit a vehicle → closes ticket, computes fee
    // Returns settled ticket or nullptr on failure
    std::shared_ptr<Ticket> exitVehicle(const std::string& licensePlate,
                                        PaymentMethod method = PaymentMethod::CASH);

    // ── Search ────────────────────────────────────────────────
    std::shared_ptr<Ticket> findActiveTicket(const std::string& licensePlate) const;
    ParkingSlot*            findSlot(const std::string& slotID);

    // ── Statistics ────────────────────────────────────────────
    int totalSlots()     const;
    int availableSlots() const;
    int occupiedSlots()  const;
    double totalRevenue() const;

    // ── Display ───────────────────────────────────────────────
    void displayDashboard()       const;
    void displayAllLevels()       const;
    void displayGrid()            const;
    void displayActiveTickets()   const;
    void displayHistory()         const;
    void displayRevenueReport()   const;

    const std::string& getName() const;

private:
    std::string m_name;
    int         m_ticketCounter = Constants::TICKET_ID_START;

    // Multi-level structure
    std::vector<std::unique_ptr<ParkingLevel>> m_levels;

    // Active: plate → ticket (currently parked)
    std::map<std::string, std::shared_ptr<Ticket>> m_activeTickets;

    // History: all closed tickets
    std::vector<std::shared_ptr<Ticket>> m_history;

    // Revenue accumulator
    double m_totalRevenue = 0.0;

    // ── Internal Helpers ──────────────────────────────────────
    ParkingLevel* findLevelWithSlot(SlotSize sz);
    void registerSensorListener();
};

#endif // PARKINGLOT_H
