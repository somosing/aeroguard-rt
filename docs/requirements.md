# AeroGuard-RT Requirements

## Functional requirements

### FR-001 — Vehicle discovery

AeroGuard shall be able to discover one PX4 vehicle accessible through MAVSDK/MAVLink.

### FR-002 — Telemetry observation

AeroGuard shall receive selected vehicle telemetry without commanding flight during v0.1.0.

### FR-003 — Timestamping

AeroGuard shall associate received telemetry with local monotonic timestamps.

### FR-004 — Freshness detection

AeroGuard shall determine whether monitored telemetry has exceeded a configured age limit.

### FR-005 — Connection reporting

AeroGuard shall report vehicle connection and disconnection state.

### FR-006 — Logging

AeroGuard shall record important connection, telemetry-health, and shutdown events in a structured form.

## Safety requirements

### SR-001 — PX4 authority

PX4 shall retain responsibility for low-level stabilization, actuator control, and native flight failsafes.

### SR-002 — Independent PX4 safety

Loss, crash, restart, or disconnection of AeroGuard shall not intentionally disable PX4 native failsafes.

### SR-003 — No flight commands in v0.1.0

The v0.1.0 telemetry observer shall not arm, take off, change flight mode, or send Offboard setpoints.

### SR-004 — Stale telemetry

Safety decisions shall not treat telemetry older than its defined freshness limit as current data.

### SR-005 — Observable failures

Failed commands and invalid system states shall be observable rather than silently ignored.

### SR-006 — Simulation-first fault testing

Fault scenarios capable of producing unsafe vehicle behaviour shall initially be evaluated only in simulation.
