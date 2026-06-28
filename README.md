# OSIM — Organizational Simulation and Internal Management System

A console-based C++ simulation of an organization's internal management system — modeling role-based access control across a multi-tier employee hierarchy, with secure internal messaging, task delegation, performance tracking, and built-in security features like OTP-based multi-factor authentication, audit logging, and anomaly detection.

## Features

- **Role-based access control (RBAC)** — a four-tier employee hierarchy (Junior → Manager → Director → Executive) where each role's clearance level governs what it can do: who it can promote, assign tasks to, or view inboxes for
- **Secure internal messaging** — per-user inboxes with encrypted private messages, and clearance-gated message types (e.g., system-wide ALERT broadcasts are restricted to Director level and above)
- **Multi-factor authentication** — OTP generation and verification as part of the login flow
- **Task management** — task assignment, recursive delegation, priority sorting, TTL-based expiry checks, completion tracking, and completion reports
- **Performance reviews** — automated scoring based on task completion rate and messaging activity
- **Notification system** — a dynamic, resizable notification log with global broadcast support
- **Anomaly detection** — parses activity logs to flag suspicious behavior, such as repeated failed logins or unusual task reassignment patterns
- **Audit logging** — tracks all system activity and supports filtering by date

## Tech Stack

- C++ (console application, no external dependencies)

## Architecture

See `24i2008_24i2130.drawio.pdf` for the full system/class diagram.

## Project Structure

```
OSIM/
├── User.cpp / User.h               # User hierarchy, authentication, PasswordManager
├── MFAHandler.cpp / .h             # OTP generation & verification
├── SecureMessenger.cpp / .h        # Encrypted messaging, inboxes, clearance checks
├── TaskManager.cpp / .h            # Task assignment, delegation, expiry
├── Task.cpp / .h                   # Individual task model
├── PolicyEngine.cpp / .h           # Centralized permission rules
├── PerformanceReview.cpp / .h      # Employee scoring
├── NotificationSystem.cpp / .h     # Notification dispatch
├── NotificationLog.cpp / .h        # Notification storage
├── NotificationDetector.cpp / .h   # Notification model
├── AnomalyDetector.cpp / .h        # Log analysis & suspicious activity flagging
├── Audit.cpp / .h                  # Activity logging
├── main.cpp                        # Menu-driven entry point
└── 24i2008_24i2130.drawio.pdf      # System diagram
```

## Getting Started

### Build
```bash
g++ -o osim main.cpp User.cpp TaskManager.cpp Task.cpp SecureMessenger.cpp NotificationSystem.cpp NotificationLog.cpp NotificationDetector.cpp PolicyEngine.cpp PerformanceReview.cpp MFAHandler.cpp AnomalyDetector.cpp Audit.cpp
```

### Run
```bash
./osim
```

### Note
The app reads/writes a few plain-text data files at runtime (`User.txt`, per-user inbox files, `tasks.txt`, etc.) to persist state between runs. These are intentionally excluded from version control since they fill up with real test data during use — on a fresh clone, the app will generate them as you register users and interact with the system.

## Authors

- Abdul Raffay (24i-2008)
- Muhammad Hamza Adil (24i-2130)

Built as a semester project at FAST University, Islamabad.
