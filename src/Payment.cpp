// ============================================================
//  Payment.cpp  –  Payment processing implementation
//  Smart Car Parking System
// ============================================================
#include "../include/Payment.h"
#include <iostream>
#include <iomanip>

int Payment::s_paymentCounter = 5000;

// ── Constructor ───────────────────────────────────────────────
Payment::Payment(std::shared_ptr<Ticket> ticket, PaymentMethod method)
    : m_ticket(std::move(ticket)),
      m_method(method),
      m_paymentID(Utils::generateID("PAY-", ++s_paymentCounter))
{}

// ── Process ───────────────────────────────────────────────────
bool Payment::process() {
    if (!m_ticket || !m_ticket->isSettled()) {
        m_status = PaymentStatus::FAILED;
        std::cout << Color::RED << "  [Payment] Ticket not valid or not settled.\n" << Color::RESET;
        return false;
    }

    m_amount = m_ticket->getAmountDue();

    // Simulate payment gateway (always succeeds in this simulation)
    std::cout << "\n" << Color::CYAN << "  Processing ";
    switch (m_method) {
        case PaymentMethod::CASH: std::cout << "Cash"; break;
        case PaymentMethod::CARD: std::cout << "Card"; break;
        case PaymentMethod::UPI:  std::cout << "UPI";  break;
    }
    std::cout << " payment of Rs " << std::fixed << std::setprecision(2)
              << m_amount << " ...\n" << Color::RESET;

    // Simulated processing delay (just dots)
    for (int i = 0; i < 3; ++i) {
        std::cout << "  ."; std::cout.flush();
    }
    std::cout << "\n";

    m_status = PaymentStatus::COMPLETED;
    return true;
}

// ── Getters ───────────────────────────────────────────────────
PaymentStatus      Payment::getStatus()     const { return m_status;    }
double             Payment::getAmountPaid() const { return m_amount;    }
const std::string& Payment::getPaymentID()  const { return m_paymentID; }
PaymentMethod      Payment::getMethod()     const { return m_method;    }

// ── Display ───────────────────────────────────────────────────
void Payment::printSummary() const {
    std::cout << "\n";
    Utils::separator();
    std::cout << Color::BOLD << "  Payment Summary\n" << Color::RESET;
    Utils::separator();
    std::cout << "  Payment ID : " << Color::CYAN << m_paymentID << Color::RESET << "\n";
    std::cout << "  Method     : ";
    switch (m_method) {
        case PaymentMethod::CASH: std::cout << "Cash\n"; break;
        case PaymentMethod::CARD: std::cout << "Card (Debit/Credit)\n"; break;
        case PaymentMethod::UPI:  std::cout << "UPI\n"; break;
    }
    std::cout << "  Amount     : Rs " << std::fixed << std::setprecision(2) << m_amount << "\n";
    std::cout << "  Status     : ";
    switch (m_status) {
        case PaymentStatus::COMPLETED:
            std::cout << Color::GREEN << "COMPLETED" << Color::RESET; break;
        case PaymentStatus::PENDING:
            std::cout << Color::YELLOW << "PENDING"   << Color::RESET; break;
        case PaymentStatus::FAILED:
            std::cout << Color::RED    << "FAILED"    << Color::RESET; break;
    }
    std::cout << "\n";
    Utils::separator();
}
