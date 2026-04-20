#pragma once
// ============================================================
//  Payment.h  –  Payment processor
//  Smart Car Parking System
// ============================================================
#ifndef PAYMENT_H
#define PAYMENT_H

#include "Utils.h"
#include "Ticket.h"
#include <string>
#include <memory>

// ── Payment Methods ───────────────────────────────────────────
enum class PaymentMethod { CASH, CARD, UPI };

// ─────────────────────────────────────────────────────────────
//  class Payment
//  Handles fee settlement for a closed ticket.
// ─────────────────────────────────────────────────────────────
class Payment {
public:
    // ── Constructor ───────────────────────────────────────────
    Payment(std::shared_ptr<Ticket> ticket, PaymentMethod method);

    // ── Processing ────────────────────────────────────────────
    bool process();   // returns true on success

    // ── Getters ───────────────────────────────────────────────
    PaymentStatus  getStatus()    const;
    double         getAmountPaid()const;
    const std::string& getPaymentID() const;
    PaymentMethod  getMethod()    const;

    // ── Display ───────────────────────────────────────────────
    void printSummary() const;

private:
    static int s_paymentCounter;

    std::shared_ptr<Ticket> m_ticket;
    PaymentMethod           m_method;
    PaymentStatus           m_status    = PaymentStatus::PENDING;
    double                  m_amount    = 0.0;
    std::string             m_paymentID;
};

#endif // PAYMENT_H
