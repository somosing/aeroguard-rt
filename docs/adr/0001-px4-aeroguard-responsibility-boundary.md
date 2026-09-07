# ADR-0001: PX4 and AeroGuard Responsibility Boundary

## Status

Accepted

## Context

AeroGuard-RT supervises a PX4-controlled autonomous vehicle from a companion computer.

The companion computer may fail, restart, lose communication, or miss deadlines. The vehicle must therefore not depend on AeroGuard for basic stabilization or direct motor control.

## Decision

PX4 remains responsible for:

- attitude control,
- angular-rate control,
- control allocation,
- actuator output,
- built-in flight modes,
- native PX4 failsafes.

AeroGuard-RT is responsible for:

- receiving telemetry,
- evaluating vehicle and telemetry health,
- high-level mission execution,
- requesting hold, return, landing, and other recovery actions,
- sending supported Offboard position or velocity setpoints when Offboard control is later introduced,
- detecting abnormal conditions,
- logging events and performance measurements.

AeroGuard-RT shall not be described as a replacement PX4 flight controller.

## Consequences

If AeroGuard crashes or disconnects, PX4 must continue running its own control loops and configured failsafes.

This separation also enables research comparisons between:

1. native PX4 failsafe behaviour, and
2. PX4 plus AeroGuard runtime supervision.
