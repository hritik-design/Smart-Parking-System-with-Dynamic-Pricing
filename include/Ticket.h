#pragma once
// ============================================================
//  Ticket.h  –  Parking ticket entity
//  Smart Car Parking System
// ============================================================
#ifndef TICKET_H
#define TICKET_H

#include "Utils.h"
#include "Vehicle.h"
#include <string>
#include <chrono>
#include <memory>

// ─────────────────────────────────────────────────────────────
//  class Ticket
//  Issued at vehicle entry; stamped at exit for billing.
// ─────────────────────────────────────────────────────────────
class Ticket {
public:
    // ── Constructor ───────────────────────────────────────────
    Ticket(int ticketSeq,
           std::shared_ptr<Vehicle> vehicle,
           const std::string& slotID,
           const std::chrono::system_clock::time_point& entryTime);

    // ── Getters ───────────────────────────────────────────────
    const std::string& getTicketID()    const;
    const std::string& getSlotID()      const;
    std::shared_ptr<Vehicle> getVehicle() const;

    const std::chrono::system_clock::time_point& getEntryTime() const;
    const std::chrono::system_clock::time_point& getExitTime()  const;

    bool   isSettled()     const;
    double getDurationMin()const;   // minutes parked (0 if still parked)
    double getAmountDue()  const;

    // ── Setters ───────────────────────────────────────────────
    void close(const std::chrono::system_clock::time_point& exitTime, double amount);

    // ── Display ───────────────────────────────────────────────
    void printTicket()  const;
    void printReceipt() const;

private:
    std::string  m_ticketID;
    std::string  m_slotID;
    std::shared_ptr<Vehicle> m_vehicle;

    std::chrono::system_clock::time_point m_entryTime;
    std::chrono::system_clock::time_point m_exitTime;

    bool   m_settled   = false;
    double m_amountDue = 0.0;
};

#endif // TICKET_H
