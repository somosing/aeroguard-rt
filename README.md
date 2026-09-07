# AeroGuard-RT

**AeroGuard-RT** is a modern C++20 companion-computer system for supervising and executing autonomous PX4 missions under degraded operating conditions.

The initial platform consists of:

- PX4 SITL
- Gazebo simulation
- MAVSDK C++
- MAVLink over UDP
- Ubuntu development host
- Raspberry Pi 3 deployment in a later stage

PX4 remains responsible for low-level flight control and native failsafes.

AeroGuard-RT is responsible for high-level mission supervision, telemetry health monitoring, fault detection, recovery decisions, event logging, and runtime-assurance experiments.

## Current milestone

### v0.1.0 — Connected Telemetry Observer

The first release will demonstrate that a C++20 AeroGuard application can:

- discover a PX4 SITL vehicle,
- receive selected telemetry,
- timestamp telemetry using a monotonic clock,
- detect stale telemetry,
- report connection and health state,
- log important events,
- run unit tests without requiring PX4.

The v0.1.0 application will not arm, take off, change flight mode, or command Offboard setpoints.

## Status

Stage 0 — Specification and environment setup.
