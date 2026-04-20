// ============================================================
//  PricingEngine.cpp  –  Pricing logic implementation
//  Smart Car Parking System
// ============================================================
#include "../include/PricingEngine.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>

// ── Singleton ─────────────────────────────────────────────────
PricingEngine& PricingEngine::getInstance() {
    static PricingEngine instance;
    return instance;
}
PricingEngine::PricingEngine() {}

// ── Mode ──────────────────────────────────────────────────────
void        PricingEngine::setMode(PricingMode mode) { m_mode = mode;   }
PricingMode PricingEngine::getMode()           const { return m_mode;   }

// ── Rate Configuration ────────────────────────────────────────
void PricingEngine::setFlatRate(VehicleType vt, double rate) {
    m_rates[static_cast<int>(vt)] = rate;
}
void   PricingEngine::setPeakMultiplier(double m) { m_peakMultiplier = m; }
double PricingEngine::getPeakMultiplier()   const { return m_peakMultiplier; }
double PricingEngine::getFlatRate(VehicleType vt) const {
    return m_rates[static_cast<int>(vt)];
}

// ── Peak Hour Check ───────────────────────────────────────────
bool PricingEngine::isPeakHour(int hour) const {
    return (hour >= Constants::PEAK_START_HOUR  && hour < Constants::PEAK_END_HOUR) ||
           (hour >= Constants::PEAK2_START_HOUR && hour < Constants::PEAK2_END_HOUR);
}

// ── Core Calculation ──────────────────────────────────────────
double PricingEngine::calculate(VehicleType vt,
                                double durationMinutes,
                                const std::chrono::system_clock::time_point& entryTime) const
{
    // Minimum charge floor
    double billableMinutes = std::max(durationMinutes, Constants::MIN_CHARGE_MINUTES);

    // Base rate (per hour)
    double baseRate = m_rates[static_cast<int>(vt)];

    // Determine peak multiplier for entry hour
    double multiplier = 1.0;
    if (m_mode == PricingMode::DYNAMIC) {
        std::time_t t = std::chrono::system_clock::to_time_t(entryTime);
        std::tm tm_buf;
#ifdef _WIN32
        localtime_s(&tm_buf, &t);
#else
        localtime_r(&t, &tm_buf);
#endif
        if (isPeakHour(tm_buf.tm_hour))
            multiplier = m_peakMultiplier;
    }

    // Fee = (minutes / 60) * rate * multiplier   (round up to nearest ₹0.50)
    double raw = (billableMinutes / 60.0) * baseRate * multiplier;
    return std::ceil(raw * 2.0) / 2.0;   // half-rupee ceiling
}

// ── Display Rates ─────────────────────────────────────────────
void PricingEngine::displayRates() const {
    Utils::separator();
    std::cout << Color::BOLD << "  Current Pricing Configuration\n" << Color::RESET;
    Utils::separator();
    std::cout << "  Mode      : " << (m_mode == PricingMode::DYNAMIC ? "DYNAMIC" : "FLAT") << "\n";
    std::cout << "  Bike      : Rs " << m_rates[0] << " / hr\n";
    std::cout << "  Car       : Rs " << m_rates[1] << " / hr\n";
    std::cout << "  Truck     : Rs " << m_rates[2] << " / hr\n";
    if (m_mode == PricingMode::DYNAMIC) {
        std::cout << "  Peak x    : " << m_peakMultiplier << "\n";
        std::cout << "  Peak hrs  : " << Constants::PEAK_START_HOUR << ":00-"
                  << Constants::PEAK_END_HOUR  << ":00  &  "
                  << Constants::PEAK2_START_HOUR << ":00-"
                  << Constants::PEAK2_END_HOUR   << ":00\n";
    }
    std::cout << "  Min bill  : " << Constants::MIN_CHARGE_MINUTES << " min\n";
    Utils::separator();
}
