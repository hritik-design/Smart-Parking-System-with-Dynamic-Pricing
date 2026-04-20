#pragma once
// ============================================================
//  PricingEngine.h  –  Dynamic / flat rate pricing calculator
//  Smart Car Parking System
// ============================================================
#ifndef PRICINGENGINE_H
#define PRICINGENGINE_H

#include "Utils.h"
#include <chrono>

// ─────────────────────────────────────────────────────────────
//  class PricingEngine
//  Computes parking fees using flat or dynamic (peak/off-peak)
//  pricing strategy.  Rates are configurable at runtime.
// ─────────────────────────────────────────────────────────────
class PricingEngine {
public:
    // ── Singleton ─────────────────────────────────────────────
    static PricingEngine& getInstance();

    // ── Mode ──────────────────────────────────────────────────
    void        setMode(PricingMode mode);
    PricingMode getMode() const;

    // ── Rate Configuration ────────────────────────────────────
    void setFlatRate(VehicleType vt, double ratePerHour);
    void setPeakMultiplier(double multiplier);

    double getFlatRate(VehicleType vt)   const;
    double getPeakMultiplier()           const;

    // ── Core Calculation ──────────────────────────────────────
    // entryTime used to determine if peak hours applied
    double calculate(VehicleType vt,
                     double durationMinutes,
                     const std::chrono::system_clock::time_point& entryTime) const;

    // ── Helpers ───────────────────────────────────────────────
    bool   isPeakHour(int hour) const;
    void   displayRates() const;

private:
    PricingEngine();                                    // private ctor (singleton)
    PricingEngine(const PricingEngine&)            = delete;
    PricingEngine& operator=(const PricingEngine&) = delete;

    PricingMode m_mode           = PricingMode::DYNAMIC;
    double      m_peakMultiplier = Constants::PEAK_MULTIPLIER;

    double m_rates[3] = {
        Constants::BIKE_RATE_FLAT,
        Constants::CAR_RATE_FLAT,
        Constants::TRUCK_RATE_FLAT
    };
};

#endif // PRICINGENGINE_H
