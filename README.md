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

The v0.1.0 release demonstrates that a C++20 AeroGuard application can:

- discover a PX4 SITL vehicle,
- receive selected telemetry,
- timestamp telemetry using a monotonic clock,
- detect stale telemetry,
- report connection and health state,
- log important events,
- run unit tests without requiring PX4.

The v0.1.0 application does not arm, take off, change flight mode, or command Offboard setpoints.

## Status

Stage 1 — Live GNSS integrity monitoring.

The current implementation includes:

- live PX4 vehicle discovery over MAVSDK,
- MAVLink connection-state monitoring,
- event-driven GNSS telemetry ingestion,
- monotonic telemetry timestamps,
- GNSS states: `NO_DATA`, `NOMINAL`, `DEGRADED`, `LOST`, and `STALE`,
- thread-safe GNSS health evaluation,
- unit tests that run without PX4,
- deterministic PX4/Gazebo GNSS degradation experiments,
- structured GNSS freshness timing on `STALE` transitions,
- graceful SIGINT/SIGTERM shutdown with structured shutdown logging.

Verified SITL observations include GNSS degradation, stale-data detection and
recovery, and graceful application shutdown:

```text
[connection] CONNECTED
[gnss] NO_DATA -> NOMINAL
[gnss] NOMINAL -> LOST
[gnss] LOST -> NOMINAL
[gnss] NOMINAL -> STALE sample_age_ms=1599 threshold_ms=1500 threshold_overshoot_ms=99
[gnss] STALE -> NOMINAL
[app] SHUTDOWN signal=SIGINT
```

See [`docs/gnss_integrity_demo.md`](docs/gnss_integrity_demo.md) for the
reproducible experiment.
