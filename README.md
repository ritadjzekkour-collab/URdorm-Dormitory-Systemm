# URdorm — University Residence Management System

A desktop application built with C++ and Qt for managing university dormitory operations, developed as part of the Object-Oriented Programming course project (ENSIA 2025-2026)

## Features
- Student card with photo, ID, room, and major information
- QR-based room entry display
- Room swap requests between students
- Room change requests to administration
- Room photo documentation for end-of-year validation
- Real-time water availability status
- Notifications system (hosting, water, announcements)
- Pavilion chief and administration contact information
- Multiple themes 
- Role-based views: Student, Administration, Pavilion Chief, Inspection Team

## Tech Stack
- **Language:** C++17
- **GUI Framework:** Qt 6 (Widgets)
- **Build System:** CMake

## Project Structure
- `main.cpp` — application entry point
- `DataManager` — manages student/notification data (in-memory, std::vector)
- `ThemeManager` — handles theme switching 
- `LoginDialog`, `MainWindow` — core navigation and login flow
- `StudentCardPage`, `ServicesPage`, `NotificationsPage`, `WaterStatusPage`, `InfoPage` — main feature pages

## How to Build
1. Open the project folder in Qt Creator
2. Configure with CMake using a Qt 6 MinGW kit
3. Build and run

## Author
Zekkour Ritadj  — ENSIA, 2025-2026
