# ADR 0002: Runtime-assurance watchdog policy boundary

## Status

Accepted

## Context

AeroGuard now observes PX4 connection state and GNSS health, but observation alone does not define how degraded telemetry should affect higher-level mission supervision.

## Decision

AeroGuard will introduce a deterministic watchdog policy that converts health observations into high-level decisions.

The policy layer will:

- consume health state rather than raw MAVSDK telemetry,
- remain separate from telemetry acquisition,
- remain testable without PX4 or Gazebo,
- log decision transitions,
- initially produce decisions without sending flight commands.

PX4 continues to own low-level stabilization and native failsafes.

## Consequences

Telemetry monitoring, policy decisions, and future command execution remain separate responsibilities.

This makes fault-response behavior easier to test, review, and extend without coupling safety policy directly to MAVSDK callbacks.
