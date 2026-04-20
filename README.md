🚗 Smart Parking System

An intelligent, multi-level parking management system developed using C++ and Object-Oriented Programming (OOP). This project simulates a real-world parking environment with features like dynamic pricing, real-time slot allocation, and sensor-based monitoring.

📌 Overview

The Smart Parking System is designed to automate and optimize parking operations by:

Efficiently allocating parking slots
Tracking vehicle entry and exit
Calculating parking fees dynamically
Managing parking data and reports
It provides a CLI-based interactive interface for smooth and structured user interaction.

⚙️ Features 

🚘 Intelligent slot allocation based on vehicle type
🧾 Ticket generation and session management
💰 Dynamic pricing (flat + peak hour pricing)
📊 Dashboard for real-time parking status
🔍 Vehicle search functionality
📜 Parking history and revenue reports
📡 Sensor simulation for real-time events
💳 Payment system (Cash, Card, UPI)

🧠 Technologies Used

Programming Language: C++
Concepts: Object-Oriented Programming (OOP)
Compiler: GCC / MinGW
IDE: VS Code / Code::Blocks / Dev C++

📂 Project Structure

SmartParking/
│── src/                 # Source files
│── include/             # Header files
│── main.cpp             # Entry point
│── SmartParkingApp.exe  # Executable (after compilation)

▶️ How to Run
🔹 Step 1: Navigate to project folder
cd SmartParking
🔹 Step 2: Compile
g++ -std=c++17 src/*.cpp main.cpp -I include -o SmartParkingApp
🔹 Step 3: Run
Windows (PowerShell):
.\SmartParkingApp.exe
Linux / Mac:
./SmartParkingApp

🧩 System Modules

ParkingLot → Main controller
ParkingLevel → Manages each floor
ParkingSlot → Individual slot handling
Vehicle → Stores vehicle details
Ticket → Tracks entry/exit and billing
PricingEngine → Fee calculation logic
Payment → Handles transactions
SensorSimulator → Simulates IoT events

🎯 Objectives

Automate parking management
Reduce manual effort and errors
Improve parking efficiency
Provide scalable and modular system design

🚀 Future Enhancements

GUI-based interface (desktop/web)
Real IoT sensor integration
Online booking system
Payment gateway integration
Advanced analytics and reporting

📌 Author

Hritik Raj
MCA (AI & ML)

⭐ Note

This project is developed as an academic project and demonstrates the practical implementation of OOP concepts and system design in a real-world scenario.
