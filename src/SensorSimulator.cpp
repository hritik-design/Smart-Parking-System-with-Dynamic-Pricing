// ============================================================
//  SensorSimulator.cpp  –  IoT sensor simulation implementation
//  Smart Car Parking System
// ============================================================
#include "../include/SensorSimulator.h"
#include <iostream>

SensorSimulator& SensorSimulator::getInstance() {
    static SensorSimulator instance;
    return instance;
}

void SensorSimulator::addListener(EventCallback cb) {
    m_listeners.push_back(std::move(cb));
}

void SensorSimulator::broadcast(const SensorReading& reading) {
    m_log.push_back(reading);
    for (auto& cb : m_listeners) cb(reading);
}

void SensorSimulator::emitVehicleDetected(const std::string& slotID,
                                           const std::string& plate) {
    SensorReading r;
    r.slotID    = slotID;
    r.event     = SensorEvent::VEHICLE_DETECTED;
    r.timestamp = Utils::formatTime(Utils::now());
    r.detail    = plate;
    broadcast(r);
}

void SensorSimulator::emitSlotCleared(const std::string& slotID) {
    SensorReading r;
    r.slotID    = slotID;
    r.event     = SensorEvent::SLOT_CLEARED;
    r.timestamp = Utils::formatTime(Utils::now());
    r.detail    = "Slot vacated";
    broadcast(r);
}

void SensorSimulator::emitAnomaly(const std::string& slotID, const std::string& detail) {
    SensorReading r;
    r.slotID    = slotID;
    r.event     = SensorEvent::ANOMALY_DETECTED;
    r.timestamp = Utils::formatTime(Utils::now());
    r.detail    = detail;
    broadcast(r);
}

void SensorSimulator::printLog() const {
    Utils::separator();
    std::cout << Color::BOLD << "  Sensor Event Log (" << m_log.size() << " events)\n" << Color::RESET;
    Utils::separator();
    if (m_log.empty()) {
        std::cout << "  No sensor events recorded.\n";
        Utils::separator();
        return;
    }
    for (const auto& r : m_log) {
        std::string evtStr;
        std::string evtCol;
        switch (r.event) {
            case SensorEvent::VEHICLE_DETECTED:
                evtStr = "DETECTED "; evtCol = Color::GREEN;  break;
            case SensorEvent::SLOT_CLEARED:
                evtStr = "CLEARED  "; evtCol = Color::BLUE;   break;
            case SensorEvent::ANOMALY_DETECTED:
                evtStr = "ANOMALY  "; evtCol = Color::RED;    break;
        }
        std::cout << "  " << r.timestamp << "  "
                  << evtCol << evtStr << Color::RESET
                  << "  " << r.slotID
                  << "  " << r.detail << "\n";
    }
    Utils::separator();
}

void SensorSimulator::clearLog() { m_log.clear(); }
