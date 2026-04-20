#pragma once
// ============================================================
//  Utils.h  –  Shared enums, constants & helper utilities
//  Smart Car Parking System
// ============================================================
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

// ── Vehicle Type ─────────────────────────────────────────────
enum class VehicleType {
    BIKE   = 0,
    CAR    = 1,
    TRUCK  = 2
};

// ── Slot Size ─────────────────────────────────────────────────
enum class SlotSize {
    COMPACT = 0,   // Bikes
    REGULAR = 1,   // Cars
    LARGE   = 2    // Trucks / SUVs
};

// ── Payment Status ────────────────────────────────────────────
enum class PaymentStatus {
    PENDING,
    COMPLETED,
    FAILED
};

// ── Pricing Mode ──────────────────────────────────────────────
enum class PricingMode {
    FLAT,        // fixed rate per hour
    DYNAMIC      // peak / off-peak rates
};

// ── ANSI Colour Codes (graceful fallback on Windows) ─────────
namespace Color {
    inline const std::string RESET   = "\033[0m";
    inline const std::string BOLD    = "\033[1m";
    inline const std::string RED     = "\033[31m";
    inline const std::string GREEN   = "\033[32m";
    inline const std::string YELLOW  = "\033[33m";
    inline const std::string BLUE    = "\033[34m";
    inline const std::string MAGENTA = "\033[35m";
    inline const std::string CYAN    = "\033[36m";
    inline const std::string WHITE   = "\033[37m";
    inline const std::string BG_RED  = "\033[41m";
    inline const std::string BG_GRN  = "\033[42m";
}

// ── Global Constants ──────────────────────────────────────────
namespace Constants {
    constexpr double BIKE_RATE_FLAT      = 20.0;   // ₹/hour
    constexpr double CAR_RATE_FLAT       = 40.0;
    constexpr double TRUCK_RATE_FLAT     = 80.0;

    constexpr double PEAK_MULTIPLIER     = 1.75;
    constexpr int    PEAK_START_HOUR     = 8;
    constexpr int    PEAK_END_HOUR       = 10;
    constexpr int    PEAK2_START_HOUR    = 17;
    constexpr int    PEAK2_END_HOUR      = 20;

    constexpr double MIN_CHARGE_MINUTES  = 30.0;   // at least 30-min billing
    constexpr int    TICKET_ID_START     = 1000;
}

// ── Utility Functions ─────────────────────────────────────────
namespace Utils {

    // Return current system time
    inline std::chrono::system_clock::time_point now() {
        return std::chrono::system_clock::now();
    }

    // Format a time_point to human-readable string
    inline std::string formatTime(const std::chrono::system_clock::time_point& tp) {
        std::time_t t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm_buf;
#ifdef _WIN32
        localtime_s(&tm_buf, &t);
#else
        localtime_r(&t, &tm_buf);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    // Duration in minutes between two time points
    inline double minutesBetween(const std::chrono::system_clock::time_point& start,
                                  const std::chrono::system_clock::time_point& end) {
        auto dur = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        return dur.count() / 60.0;
    }

    // Current hour (0–23)
    inline int currentHour() {
        auto now_t = std::chrono::system_clock::to_time_t(now());
        std::tm tm_buf;
#ifdef _WIN32
        localtime_s(&tm_buf, &now_t);
#else
        localtime_r(&now_t, &tm_buf);
#endif
        return tm_buf.tm_hour;
    }

    // Convert VehicleType to string
    inline std::string vehicleTypeStr(VehicleType vt) {
        switch (vt) {
            case VehicleType::BIKE:  return "Bike";
            case VehicleType::CAR:   return "Car";
            case VehicleType::TRUCK: return "Truck";
        }
        return "Unknown";
    }

    // Map VehicleType → required SlotSize
    inline SlotSize requiredSlot(VehicleType vt) {
        switch (vt) {
            case VehicleType::BIKE:  return SlotSize::COMPACT;
            case VehicleType::CAR:   return SlotSize::REGULAR;
            case VehicleType::TRUCK: return SlotSize::LARGE;
        }
        return SlotSize::REGULAR;
    }

    // Convert SlotSize to string
    inline std::string slotSizeStr(SlotSize ss) {
        switch (ss) {
            case SlotSize::COMPACT: return "Compact";
            case SlotSize::REGULAR: return "Regular";
            case SlotSize::LARGE:   return "Large";
        }
        return "Unknown";
    }

    // Print a coloured separator line
    // ch_str should be a single UTF-8 box-drawing char (or ASCII '-', '=', etc.)
    inline void separator(const std::string& ch_str = "-", int len = 60) {
        std::cout << Color::CYAN;
        for (int i = 0; i < len; ++i) std::cout << ch_str;
        std::cout << Color::RESET << "\n";
    }

    // Print coloured section header
    inline void header(const std::string& title) {
        separator("=");
        int pad = (60 - static_cast<int>(title.size())) / 2;
        std::cout << Color::BOLD << Color::BLUE;
        for (int i = 0; i < pad; ++i) std::cout << ' ';
        std::cout << title << Color::RESET << "\n";
        separator("=");
    }

    // Generate a unique ticket/receipt ID
    inline std::string generateID(const std::string& prefix, int counter) {
        std::ostringstream oss;
        oss << prefix << std::setw(5) << std::setfill('0') << counter;
        return oss.str();
    }
}

#endif // UTILS_H
