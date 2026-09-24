# GNSS Integrity Demo

This experiment verifies that AeroGuard-RT distinguishes vehicle connectivity
from GNSS navigation health.

## Setup

- PX4 v1.17.0 SITL
- Gazebo Harmonic
- X500 multicopter
- MAVSDK C++ v3.17.4
- AeroGuard-RT companion process

## Start PX4

```bash
cd "$HOME/Projects/PX4-Autopilot"
make px4_sitl gz_x500
```

## Start AeroGuard

In another terminal:

```bash
cd "$HOME/Projects/aeroguard-rt"
./build/aeroguard
```

A healthy simulated GNSS source should transition AeroGuard from:

```text
[gnss-state] NO_DATA -> NOMINAL
```

## Inject GNSS degradation

From the PX4 `pxh>` shell:

```text
param set SIM_GPS_USED 0
```

The MAVLink connection remains alive while GNSS loses its valid fix.

Expected AeroGuard transition:

```text
[connection] CONNECTED
[gnss-state] NOMINAL -> LOST
```

## Restore GNSS

```text
param set SIM_GPS_USED 10
```

Expected recovery:

```text
[gnss-state] LOST -> NOMINAL
```

## Stale-data detection

Stopping GNSS telemetry entirely causes the monitor to transition to `STALE`
after the configured freshness deadline.

This is intentionally separate from MAVLink connection state: a connected
vehicle does not imply that navigation telemetry is fresh or trustworthy.

## Limitation

`SIM_GPS_USED` is a deterministic SITL test input, not a complete model of
real GNSS jamming or spoofing.

Future integrity checks will use additional navigation signals and
cross-sensor consistency rather than satellite count alone.
