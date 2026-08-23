# SimpleTelescopeMount — Serial Interface Reference

Reference documentation for the serial (UART) command interface of the
ESP32 alt-az mount controller firmware
(`esp32_firmware/SimpleTelescopeMount/`).

---

## 1. Physical / Link Parameters

| Property        | Value                                        |
| --------------- | -------------------------------------------- |
| Interface       | UART (ESP32 default `Serial`, i.e. UART0)    |
| Baud rate       | 115200 (set in `setup()` via `Serial.begin(115200)`) |
| Framing         | 8N1 (Arduino/ESP32 default)                  |
| Default pins    | GPIO1 (TX, device→host), GPIO3 (RX, host→device) — board dependent |
| Byte encoding   | ASCII, 7-bit text commands                   |
| Command terminator | `\n` (LF), `\r` (CR), or CRLF — either one terminates a command |
| Max command length | 63 characters (64-byte buffer, NUL not counted). A longer line triggers `ERROR: Buffer overflow` and the buffer is discarded; resend a shorter line. |

The link is fully half-duplex in practice: you send a line, the device
responds on the same serial port. There is no flow control.

### Boot banner

On power-on / reset the firmware prints:

```
ESP32 Serial Parser Starting...
Commands: N AXIS VALUE | S | T AZ ALT FROT
```

(The banner predates the `C` command — the full command set is documented
below. Motor objects also load their parameters from NVS before the banner.)

---

## 2. Firmware Architecture (where commands actually execute)

Understanding the two FreeRTOS tasks is important because it determines
*when* a command's effect becomes visible:

```
┌──────────────────────────── Core 0 (weak core) ────────────────────────────┐
│  serialParserTask()   [priority 1, stack 10000 bytes, pinned to core 0]   │
│    1. Reads one byte at a time from Serial (vTaskDelay(1) between polls)   │
│    2. Accumulates bytes into serialBuffer[64]                              │
│    3. On '\n' or '\r': NUL-terminate → parseCommand(serialBuffer)          │
│       ├─ 'N' → parseNudgeCommand()     → handleSerialInputN()             │
│       ├─ 'S' → handleSerialInputS()   (prints position reply)              │
│       ├─ 'T' → parseTrackingCommand() → handleSerialInputT()               │
│       ├─ 'P' → handleSerialInputP()   (prints parameter dump)              │
│       └─ 'C' → parseConfiguration()   → handleSerialC()                    │
└─────────────────────────────────────────────────────────────────────────────┘
                                        │  shared NudgeCommand + mutex
┌──────────────────────────── Core 1 (strong core) ─────────────────────────┐
│  loop()   [Arduino superloop, runs continuously]                          │
│    1. Non-blocking check of the nudge command slot (nudgeMutex)            │
│       → executes the blocking motor movement here                          │
│    2. altMotor.run(); azMotor.run(); fRotMotor.run(); focuser.run();       │
│    3. fRotMotorTentioner.runSpeed();  (constant-speed tensioner, 10 steps/s,
│                                        always running, not serial-controlled)
└─────────────────────────────────────────────────────────────────────────────┘
```

Consequences for the host:

* **`N` commands are asynchronous.** `handleSerialInputN()` only stores the
  command in a shared slot (`nudgeCmd`) under a mutex and returns immediately.
  `loop()` on Core 1 picks it up on its next iteration and performs the
  (blocking) movement there. There is **no ACK** for `N` — the reply (if any)
  is the next query you issue.
* **Only one `N` command is in flight at a time.** The slot is a single
  command; if you send more `N`s than the mount can execute, each new one
  overwrites the pending slot (a nudge still in progress is *not* interrupted
  — `loop()` is busy inside the blocking `nudge()` until it finishes, then
  processes the newest pending command).
* **Position/movement queries (`S`, `P`) are executed on Core 0** while the
  steppers are being serviced on Core 1. Replies are therefore always timely
  and never block on motor movement.
* **`T` (move-to) is asynchronous** like `N`: it only sets AccelStepper
  destination targets; the actual stepping happens in `loop()`. No ACK.
* **All movement is open-loop** (no encoders). There is no error/interrupt
  reporting on the serial link — the only negative responses are the
  parse/validation errors listed per-command.

### The four controlled axes

| Axis   | Object          | Type        | Motion unit | Driver pins (DIR, PUL) | Home pin | Notes |
| ------ | --------------- | ----------- | ----------- | ---------------------- | -------- | ----- |
| `ALT`  | `altMotor`      | `NEMAMotor` | degrees     | 23, 22                 | 34       | Altitude NEMA driver |
| `AZ`   | `azMotor`       | `NEMAMotor` | degrees     | 4, 2                   | 34       | Azimuth NEMA driver; `setInverted(false)` forced at boot |
| `FROT` | `fRotMotor`     | `NEMAMotor` | degrees     | 21, 19                 | 34       | Field-rotation wheel; 25:1-ish reduction (99.5) |
| `FOC`  | `focuser`       | `LinearNEMAMotor` | **millimetres** | 18, 5           | 34       | Linear focuser, 2.8 mm/rev, max travel 82 mm |
| `HFOC` | (focuser alias) | —           | —           | —                      | 34       | Special "home focuser" token, see `N` command |

All four axes share **GPIO 34** as their home-sense pin (a single
end-stop/limit switch wired in common).

A fifth motor, `fRotMotorTentioner` (28BYJ-48 on pins 13/14/12/27 via a
full 4-wire AccelStepper), runs continuously at a constant slow speed to
take up slack on the field-rotation axle. It is **not** controllable or
queryable over serial.

Motor parameters (speed, accel, microstep, reduction, motor steps, backlash,
inversion) are persisted in ESP32 NVS under the namespaces `ALT`, `AZ`,
`FROT`, `FOC` and re-loaded at every boot.

---

## 3. Command Summary

| Cmd | Syntax                        | Unit(s)     | Response on success | Response on failure                          |
| --- | ----------------------------- | ----------- | ------------------- | -------------------------------------------- |
| `N` | `N <AXIS> <VALUE>\n`          | deg / mm    | *(none — async)*    | `ERROR: Invalid N command format. Use: N AXIS VALUE` |
| `S` | `S\n`                         | —           | `S <az> <alt>`      | *(none)*                                     |
| `T` | `T <AZ> <ALT> <FROT>\n`       | degrees     | *(none — async)*    | `ERROR: Invalid T command format...` / range errors |
| `P` | `P\n`                         | —           | 4 lines `P <AXIS>,<csv>` | *(none)*                              |
| `C` | `C <AXIS>,<speed>,<accel>,<microstep>,<reduction>,<steps>,<posBL>,<negBL>,<homePin>,<inv>\n` | mixed | `C OK` | `C ERR <reason>` |

Command letters are **case-insensitive** (`n`, `s`, `t`, `p`, `c` all work).
Leading spaces before the command letter are ignored. Any other first
character yields:

```
ERROR: Unknown command '<char>'
```

---

## 4. Command Details & Call Traces

### 4.1 `N` — Nudge / relative move (and focuser controls)

**Syntax:** `N <AXIS> <VALUE>` terminated by newline.

**Call chain:**

```
serialParserTask (Core 0)
 └─ parseCommand()                // cmd[0] == 'N'/'n'
     └─ parseNudgeCommand(cmd+1)  // sscanf "%s %f" → axis string, float value
         │    • axis is upper-cased
         └─ handleSerialInputN(axis, value)
              • takes nudgeMutex (portMAX_DELAY)
              • stores nudgeCmd = { requested: true, value, axis }
              • releases mutex, returns immediately   ← no motion yet
loop() (Core 1, next iteration)
 └─ takes nudgeMutex (non-blocking), sees nudgeCmd.requested
     • clears the slot, releases mutex
     • dispatches by axis:
         "ALT"  → altMotor.nudge(value)
         "AZ"   → azMotor.nudge(value)
         "FROT" → fRotMotor.nudge(value)
         "FOC"  → focuser.setDestination(value)      ← absolute, not a nudge!
         "HFOC" → focuser.startHoming()               ← blocking home sequence
```

**Per-axis behaviour:**

| AXIS | Meaning of VALUE | Behaviour |
| ------ | ---------------- | --------- |
| `ALT` | degrees (signed) | `NEMAMotor::nudge()`: relative move of *value* degrees. **Blocking** — `loop()` spins `stepper.run()` until the move completes, so no other `N`/`T` motion progresses during it. Fractional steps are accumulated across calls (sub-microstep precision). |
| `AZ` | degrees (signed) | Same as `ALT`, on the azimuth axis. |
| `FROT` | degrees (signed) | Same, on the field-rotation wheel. |
| `FOC` | **absolute position in mm** (0 … 82) | *Not* a nudge — calls `LinearNEMAMotor::setDestination(mm)`, which clamps to `[0, maxTravelMm]` and moves **asynchronously** (no blocking; motion happens in `loop()`). |
| `HFOC` | (value ignored) | `LinearNEMAMotor::startHoming()`: **blocking** — moves focuser in the negative direction until the home pin (GPIO 34) reads HIGH, then stops, zeroes the position counter and resets the fractional-step accumulator. |

**Important `N` quirks (observed in `NEMAMotor::nudge()`):**

1. **The position readout does not change after an `N`.** `nudge()` saves
   the stepper position, moves by the offset, and after completion calls
   `stepper.setCurrentPosition(savedPosition)`. So the physical axis moves,
   but a subsequent `S` still reports the *pre-nudge* logical position.
   Treat `N` as a "bump the axis while keeping the coordinate system fixed"
   operation (e.g. nudging a target into the eyepiece without re-slewing).
2. Offsets that round to zero whole microsteps are silently ignored
   (`if (wholeSteps == 0) return;`). At the default geometry
   (200 steps × 256 microstep × 25 reduction) one microstep of ALT/AZ is
   360 / (200·256·25) ≈ 0.00028°, so values smaller than ~0.00015° are
   dropped unless accumulated fraction from earlier commands push them over
   the rounding boundary.
3. The backlash-compensation code in `nudge()`/`setDestination()` is
   **present but commented out** — the `positiveBacklash`/`negativeBacklash`
   parameters stored via `C` are persisted but currently *not applied*.

### 4.2 `S` — Report position

**Syntax:** `S`

**Call chain:**

```
serialParserTask (Core 0)
 └─ parseCommand()  // cmd[0] == 'S'/'s'
     └─ handleSerialInputS()
          • az  = azMotor.getCurrentPositionDeg()
          • alt = altMotor.getCurrentPositionDeg()
          • prints reply
```

**Response** (values at 5 decimals, degrees):

```
S <azimuth> <altitude>
<blank line>
```

Example:

```
S 172.35001 34.10000

```

Notes:

* Only **AZ and ALT** are reported — there is no serial query for FROT or
  FOC position (use `P` to confirm parameters only).
* Positions are the open-loop internal step counters, which are lost on
  power-cycle (no NVRAM position persistence). Zero the system by homing /
  re-referencing, or treat positions as relative to last power-on.
* Because of the `N` quirk in §4.1, `S` does not reflect nudge movements.

### 4.3 `T` — Move-to (absolute slew)

**Syntax:** `T <AZ> <ALT> <FROT>` (degrees, terminated by newline).

**Call chain:**

```
serialParserTask (Core 0)
 └─ parseCommand()
     └─ parseTrackingCommand(cmd+1)
          • sscanf "%f %f %f" → az, alt, frot
          • range validation:
              az   ∈ [0, 360]   else "ERROR: AZ must be between 0 and 360"
              alt  ∈ [-90, 90]  else "ERROR: ALT must be between -90 and 90"
              frot ∈ [0, 360]   else "ERROR: FROT must be between 0 and 360"
          └─ handleSerialInputT(az, alt, frot)
               • guard: only proceeds if NONE of az/alt/frot motors is running
               • azMotor.setDestination(az)
               • altMotor.setDestination(alt)
               • fRotMotor.setDestination(frot)
```

**Behaviour / semantics:**

* All three rotation axes move **simultaneously**, with the trapezoidal
  acceleration profile from their stored speed/accel parameters.
* `NEMAMotor::setDestination()` accepts degrees in **[-360, +360]** and
  wraps anything outside that range by ±360° before converting to steps.
  (The parser itself rejects values outside 0–360, so in practice the wrap
  only protects direct library use.)
* **Silent rejection:** if any of the three motors is still moving (e.g.
  from a previous `T`, or while an `N` ALT/AZ/FROT is being executed), the
  command is **ignored with no message**. Poll `S` (or wait) before sending
  a new `T` if you need guaranteed acceptance.
* **No ACK and no completion notification.** The only way to know the slew
  finished is to poll `S` and detect the position converging to the target.
* FOC is not addressable by `T` — the focuser is only driven via `N FOC`
  (absolute mm) and `N HFOC` (home).
* Backlash compensation in `setDestination()` is commented out (see §4.1).

### 4.4 `P` — Dump motor parameters

**Syntax:** `P`

**Call chain:**

```
serialParserTask (Core 0)
 └─ parseCommand()
     └─ handleSerialInputP()
          • altMotor.getParametersCSV()
          • azMotor.getParametersCSV()
          • fRotMotor.getParametersCSV()
          • focuser.getParametersCSV()
          • prints four lines
```

**Response** (one line per axis, plus a trailing blank line):

```
P ALT,<speed>,<accel>,<microstep>,<reduction>,<motorSteps>,<posBacklash>,<negBacklash>,<homingPin>,<inverted>
P AZ,<...same fields...>
P FROT,<...same fields...>
P FOC,<...same fields...>

```

Field order (identical for all four axes, `getParametersCSV()`):

| # | Field | Type | Example (ALT defaults) |
| - | ------------- | ------- | ---------------------- |
| 1 | speed | float (steps/s) | 20000.000000 |
| 2 | accel | float (steps/s²) | 20000.000000 |
| 3 | microstep | int | 256 |
| 4 | reduction | float | 25.000000 |
| 5 | motorSteps | int (200 or 400) | 200 |
| 6 | posBacklash | float (deg) | 0.000000000 |
| 7 | negBacklash | float (deg) | 0.000000000 |
| 8 | homingPin | int | 34 |
| 9 | inverted | int (0/1) | 1 |

The focuser's `mmPerRev` / `maxTravelMm` are **not** included in its CSV
(they are commented out in `LinearNEMAMotor::getParametersCSV()`); they
remain at the compile-time values 2.8 mm/rev and 82 mm max travel.

`P` is the reliable way to confirm a `C` command took effect.

### 4.5 `C` — Configure a motor (persisted to NVS)

**Syntax:** `C <AXIS>,<speed>,<accel>,<microstep>,<reduction>,<motorSteps>,<posBacklash>,<negBacklash>,<homingPin>,<inverted>`

**Call chain:**

```
serialParserTask (Core 0)
 └─ parseCommand()
     └─ parseConfiguration(cmd+1)
          • strtok over ',' into 10 fields (axis, speed, accel, microstep,
            reduction, motorSteps, posBacklash, negBacklash, homingPin, inverted)
          └─ handleSerialC(axis, speed, accel, microstep, reduction,
                           motorSteps, posBacklash, negBacklash,
                           homingPin, inverted)
               • validation (see below) → "C ERR ..." + return on first failure
               • axis dispatch (strcmp against " ALT", " AZ", " FROT", " FOC"):
                   • ALT:  setSpeed, setAccel, setMicrostep, setReduction,
                           setMotorSteps, setPositiveBacklash, setNegativeBacklash,
                           setInverted
                   • AZ:   same eight setters as ALT
                   • FROT: setSpeed, setAccel, setMicrostep, setReduction,
                           setMotorSteps, setInverted
                           (backlash setters NOT called for FROT)
                   • FOC:  setSpeed, setAccel, setMicrostep, setReduction,
                           setMotorSteps, setPositiveBacklash, setNegativeBacklash
                           (setInverted NOT called for FOC)
               • prints "C OK"
```

Every setter both updates the live controller state (e.g.
`AccelStepper::setMaxSpeed`) **and** writes the value to NVS
(`Preferences`, namespace = axis name), so the configuration survives
reboots and is visible in `P`.

**Validation (checked in this order, first failure wins):**

| Check | Error printed |
| -------------------------------------------------- | ------------- |
| `speed <= 0` or `accel <= 0` | `C ERR Speed or Accel equal or less than 0` |
| `speed > 20000` or `accel > 20000` | `C ERR Speed or Accel too high, > 20000` |
| `microstep` ∉ {2, 4, 8, 16, 32, 64, 128, 256} | `C ERR Microsteps must be 2, 4, 8, 16, 32, 64, 128, 256` |
| `reduction <= 0` | `C ERR Reduction equal or less than 0` |
| `motorSteps` ∉ {200, 400} | `C ERR Motor Natural Steps must be 200 or 400` |
| `posBacklash < 0` or `negBacklash < 0` | `C ERR Backlash must be 0 or a positive number` |
| `inverted` ∉ {0, 1} | `C ERR Inverted must be between 0 and 1` |
| valid → apply | `C OK` |

**`C` quirks (read before scripting this command):**

1. **A single space is required between `C` and the axis name.** The parser
   passes `cmd+1` straight to `strtok`, so the first field keeps its leading
   space, and the axis dispatch compares against the literals `" ALT"`,
   `" AZ"`, `" FROT"`, `" FOC"`. `CALT,...` (no space) or `C  ALT,...`
   (two spaces) will pass validation but **match no axis** and still print
   `C OK` without changing anything.
2. **The `homingPin` field (field 9) is parsed and validated by shape only
   (any integer) but is never applied** — there is no `setHomingPin()`.
   The hardware home pin is fixed at compile time (GPIO 34). Echo the
   current value (34) in this field.
3. **Field 10 (`inverted`) is ignored for `FOC`**, and **fields 7–8
   (backlash) are ignored for `FROT`**.
4. Backlash values, although stored, are not used anywhere at present
   (compensation code is commented out).
5. Changing `microstep` / `reduction` / `motorSteps` via `C` **does not
   rescale the internal position counter** — the stored step count is
   reinterpreted with the new geometry. Do this before homing/referencing,
   and be aware `S` readouts will change meaning.

**Example:**

```
C ALT,5000,2000,256,25,200,0,0,34,1
→ C OK
P
→ P ALT,5000.000000,2000.000000,256,25.000000,200,0.000000000,0.000000000,34,1
   ...
```

---

## 5. Error & Diagnostic Messages (all of them)

| Message | Source | Meaning / remedy |
| ------- | ------ | ---------------- |
| `ERROR: Buffer overflow` | `serialParserTask` | A line longer than 63 chars was received; buffer discarded. Send shorter lines. |
| `ERROR: Unknown command '<c>'` | `parseCommand` | First non-space character is not N/S/T/P/C (case-insensitive). |
| `ERROR: Invalid N command format. Use: N AXIS VALUE` | `parseNudgeCommand` | `sscanf` did not yield an axis token + float (e.g. `N ALT` or `N 5`). |
| `ERROR: Invalid T command format. Use: T AZ ALT FROT` | `parseTrackingCommand` | `sscanf` did not yield three floats. |
| `ERROR: AZ must be between 0 and 360` | `parseTrackingCommand` | AZ out of range. |
| `ERROR: ALT must be between -90 and 90` | `parseTrackingCommand` | ALT out of range. |
| `ERROR: FROT must be between 0 and 360` | `parseTrackingCommand` | FROT out of range. |
| `C ERR Speed or Accel equal or less than 0` | `handleSerialC` | see §4.5 |
| `C ERR Speed or Accel too high, > 20000` | `handleSerialC` | see §4.5 |
| `C ERR Microsteps must be 2, 4, 8, 16, 32, 64, 128, 256` | `handleSerialC` | see §4.5 |
| `C ERR Reduction equal or less than 0` | `handleSerialC` | see §4.5 |
| `C ERR Motor Natural Steps must be 200 or 400` | `handleSerialC` | see §4.5 |
| `C ERR Backlash must be 0 or a positive number` | `handleSerialC` | see §4.5 |
| `C ERR Inverted must be between 0 and 1` | `handleSerialC` | see §4.5 |
| `Failed to create nudge mutex!` (then hard lockup) | `setup()` | Fatal ESP32 memory/allocation failure at boot; power-cycle. |

Note there is **no "busy" response**: `T` while a motor is moving is
dropped silently (§4.3), and an unknown axis in `N` (e.g. `N XYZ 1.0`)
passes parsing and is then matched against no dispatch branch in `loop()` —
it is likewise silently ignored (the command slot is consumed with no
action).

---

## 6. Worked Session Example

```
host> S
dev>  S 0.00000 0.00000
dev>
host> T 180 45 0
        (no response; slewing in progress — at the default 20000 steps/s the
         ALT axis moves 20000/(200·256·25) ≈ 1.96°/s, so a 45° move takes
         roughly 23 s; poll S to watch progress)
host> S
dev>  S 97.20105 22.04999
dev>
host> S
dev>  S 180.00000 45.00000
dev>
host> N ALT 0.05
        (no response; nudges ALT +0.05° blocking-style; S readout unchanged
         after completion by design)
host> N FOC 12.5
        (no response; focuser slews asynchronously to 12.5 mm)
host> P
dev>  P ALT,20000.000000,20000.000000,256,25.000000,200,0.000000000,0.000000000,34,1
dev>  P AZ,20000.000000,20000.000000,256,25.000000,200,0.000000000,0.000000000,34,0
dev>  P FROT,20000.000000,20000.000000,32,99.500000,200,0.000000000,0.000000000,34,1
dev>  P FOC,1000.000000,500.000000,256,1.000000,200,0.010000000,0.010000000,34,0
dev>
host> C ALT,2000,1000,256,25,200,0,0,34,1
dev>  C OK
host> X
dev>  ERROR: Unknown command 'X'
```

---

## 7. Host-Integration Guidelines

1. **One command per line**, newline-terminated; wait for and consume the
   reply before sending the next line (replies for `S`/`P`/`C`/errors are
   synchronous; `N`/`T` produce none).
2. **Never assume `T` was accepted** — re-send `T` only after `S` shows
   convergence, or throttle sends to a cadence slower than your worst-case
   slew time.
3. **Throttle `N` ALT/AZ/FROT** — each is effectively synchronous
   (blocks the motion loop) and single-slot queued.
4. **Position is open-loop and non-persistent** across power cycles;
   re-home/re-reference after power events.
5. Keep lines under **63 characters** (the longest realistic line is a `C`
   command with large numbers — ~60 chars, so it fits but leaves little
   margin).
6. The focuser's shared home pin means `N HFOC` will also trigger if any
   other axis is homing against the same switch; only the focuser has a
   serial-accessible homing path anyway.
7. After power-on, wait for the boot banner (or ~1 s) before issuing
   commands; characters sent before the parser task starts are not
   buffered by the parser (the FreeRTOS UART driver buffers 256 bytes, so a
   very short early line will still arrive).
