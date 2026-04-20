#pragma once
// ============================================================
//  SensorSimulator.h  –  IoT sensor simulation layer
//  Smart Car Parking System
// ============================================================
#ifndef SENSORSIMULATOR_H
#define SENSORSIMULATOR_H

#include "Utils.h"
#include <string>
#include <functional>
#include <vector>

// ── Sensor Event Types ────────────────────────────────────────
enum class SensorEvent {
    VEHICLE_DETECTED,
    SLOT_CLEARED,
    ANOMALY_DETECTED
};

// ── Sensor Reading ────────────────────────────────────────────
struct SensorReading {
    std::string slotID;
    SensorEvent event;
    std::string timestamp;
    std::string detail;
};

// ─────────────────────────────────────────────────────────────
//  class SensorSimulator
//  Simulates IR / ultrasonic parking sensors.
//  Broadcasts events to registered listeners.
// ─────────────────────────────────────────────────────────────
class SensorSimulator {
public:
    using EventCallback = std::function<void(const SensorReading&)>;

    static SensorSimulator& getInstance();

    // ── Registration ──────────────────────────────────────────
    void addListener(EventCallback cb);

    // ── Event Emission ────────────────────────────────────────
    void emitVehicleDetected(const std::string& slotID, const std::string& plate);
    void emitSlotCleared    (const std::string& slotID);
    void emitAnomaly        (const std::string& slotID, const std::string& detail);

    // ── Diagnostics ───────────────────────────────────────────
    void printLog() const;
    void clearLog();

private:
    SensorSimulator() = default;
    SensorSimulator(const SensorSimulator&)            = delete;
    SensorSimulator& operator=(const SensorSimulator&) = delete;

    void broadcast(const SensorReading& reading);

    std::vector<EventCallback>  m_listeners;
    std::vector<SensorReading>  m_log;
};

#endif // SENSORSIMULATOR_H
