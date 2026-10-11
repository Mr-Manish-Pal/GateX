
# GateX
### ESP32-Based Smart Gate Automation System

**GateX is an embedded automation prototype that uses an ESP32 and an ultrasonic sensor to automate gate operation based on distance detection.** It combines sensor-based control, visual feedback, audible alerts, and user interaction in a compact embedded system.

The system is designed to detect an object or person near a gate and control the gate according to configured distance conditions. An OLED display provides animated expressions and gate status, while LEDs and a buzzer provide additional feedback.

GateX demonstrates the integration of microcontroller programming, sensor interfacing, display control, digital outputs, and user-controlled alert management in a practical automation project.

---

## Project Overview

GateX explores how embedded systems can improve the convenience and usability of a basic gate automation mechanism.

The ESP32 processes distance measurements from an ultrasonic sensor and uses the configured conditions to determine the required gate state. The display and indicators communicate the system status, while a dedicated button allows the buzzer to be disabled without stopping the main automation logic.

### Key Highlights

- Automatic gate control based on ultrasonic distance detection.
- ESP32-based embedded control architecture.
- OLED display with expressive visual feedback.
- Independent buzzer mute control.
- LED indicators for gate status.
- Integration of sensing, control, and user interaction.
- Modular foundation for future smart access and automation features.

---

## System Architecture

The system follows a sensor-controller-output architecture.

```text
       Ultrasonic Sensor
               |
               v
        Distance Reading
               |
               v
         ESP32 Controller
               |
       +-------+--------+
       |       |        |
       v       v        v
  Gate Control OLED    Status LEDs
               |
               v
          Buzzer Alert

       Mute Control Button
               |
               v
             ESP32
```

The ESP32 acts as the central controller. It processes the sensor readings, evaluates the configured distance conditions, and updates the relevant outputs.

**Note:** The gate actuator and its electrical interface depend on the final hardware implementation.

---

## Hardware Components

| Component | Purpose |
|---|---|
| ESP32 development board | Main controller |
| Ultrasonic distance sensor | Measures the distance of nearby objects |
| 0.96-inch OLED display | Displays expressions and gate status |
| Buzzer | Provides an audible alert |
| Push button | Enables or disables the buzzer |
| Red LED | Indicates the open-gate state |
| Green LED | Indicates the closed-gate state |
| Connecting wires | Hardware interconnections |
| Breadboard or suitable mounting | Prototype assembly |

The exact sensor model, actuator, GPIO assignments, and power requirements should be documented according to the final hardware configuration.

---

## Working Principle

### 1. Distance Detection

The ultrasonic sensor measures the distance between the sensor and a nearby object. The ESP32 reads the measurement and compares it with the configured distance threshold.

### 2. Gate Automation

When the measured distance satisfies the configured opening condition, the controller requests the gate-open state. When the distance satisfies the closing condition, the controller requests the gate-closed state.

The threshold and transition logic determine how the system responds to changes in distance.

### 3. OLED Status Display

The OLED provides visual feedback using simple expressions:

- **Happy expression:** Gate open.
- **Sad expression:** Gate closed.

This makes the system status easier to understand without requiring a separate monitoring interface.

### 4. Audible Alert

When the gate opens, the buzzer sounds for approximately five seconds.

The buzzer is intended to provide an alert during the opening event.

### 5. Independent Buzzer Control

A dedicated push button disables the buzzer when silent operation is preferred.

Disabling the buzzer should not disable distance detection, gate automation, OLED updates, or LED status indicators.

### 6. LED Indicators

The LEDs provide an additional visual indication of the gate state:

- **Red LED:** Gate open.
- **Green LED:** Gate closed.

These indicators complement the OLED display and make the system state visible from a distance.

---

## Control Logic

The intended control sequence is:

```text
Start System
     |
Initialize ESP32 and Peripherals
     |
Read Ultrasonic Sensor
     |
Evaluate Distance Condition
     |
Determine Gate State
     |
Update OLED and LEDs
     |
Check Buzzer Enable Setting
     |
Trigger Alert When Required
     |
Repeat Monitoring Cycle
```

The implementation should also account for invalid sensor readings, sensor timeouts, and rapid changes near the configured threshold.

A suitable hysteresis or state-transition strategy can help prevent unnecessary switching when the measured distance fluctuates around the threshold.

---

## Software and Development Tools

| Technology | Application |
|---|---|
| Embedded C/C++ | Microcontroller application logic |
| Arduino framework | ESP32 firmware development, if used |
| ESP32 Arduino core | Hardware and peripheral support |
| OLED display library | Rendering expressions and status |
| GPIO programming | LEDs, button, and buzzer control |
| Ultrasonic sensor interface | Distance measurement |
| Git and GitHub | Source-code version control |

The final list of software dependencies should match the libraries and framework actually used in the firmware.

---

## Repository Structure

```text
GateX/
├── README.md
├── src/
│   └── GateX.ino
├── docs/
│   ├── wiring.md
│   └── working-principle.md
├── images/
│   ├── gatex-hardware.jpg
│   └── gatex-oled.jpg
└── LICENSE
```

This is a suggested structure. Keep only the files and folders that exist in the actual repository, and update the firmware filename if it differs.

---

## Installation and Setup

### Requirements

- ESP32 development board
- Ultrasonic sensor
- OLED display
- Buzzer, push button, and LEDs
- USB data cable
- Arduino IDE or the development environment used for the firmware

### Setup Procedure

1. Clone or download the GateX repository.
2. Open the firmware in the appropriate development environment.
3. Install the required ESP32 board support and libraries.
4. Connect the hardware according to the project's wiring documentation.
5. Verify the GPIO assignments and sensor configuration.
6. Select the correct ESP32 board and serial port.
7. Compile and upload the firmware.
8. Open the serial monitor, if enabled, to inspect sensor readings and system behavior.
9. Test distance detection, gate-state transitions, OLED feedback, LED indicators, and buzzer mute functionality.

**Hardware note:** ESP32 GPIOs operate at 3.3 V logic. Verify the sensor output voltage before connecting it to an ESP32 input. If an ultrasonic sensor uses a 5 V echo output, use a suitable voltage divider or level shifter. The actuator must use an appropriate driver and power supply rather than being powered directly from an ESP32 GPIO.

---

## Testing and Validation

The following checks should be completed and documented during hardware testing.

| Test | Expected result |
|---|---|
| Object enters the configured detection range | Gate-open condition is triggered |
| Object moves outside the configured closing range | Gate-closed condition is triggered |
| Gate enters the open state | OLED shows a happy expression |
| Gate enters the closed state | OLED shows a sad expression |
| Gate opens with buzzer enabled | Buzzer sounds for approximately five seconds |
| Buzzer is disabled using the button | Audible alert is suppressed |
| Buzzer is disabled | Automation and visual status continue |
| Gate state changes | Corresponding LED indicator updates |
| Sensor reading becomes invalid | System handles the reading without uncontrolled switching |

These are intended test cases, not claims that every test has already passed.

---

## Safety and Limitations

GateX is an educational embedded automation prototype. It should not be treated as a certified access-control system or a safety-rated industrial gate controller.

- Ultrasonic measurements can be affected by object shape, angle, surface, and environmental conditions.
- Distance thresholds should be configured and validated for the intended installation.
- A real gate mechanism requires appropriate motor control, limit switches, obstruction detection, and independent safety measures.
- Sensor failures and controller resets must not cause hazardous gate movement.
- Keep moving mechanisms isolated during development and testing.
- Do not connect motors or other high-current loads directly to ESP32 GPIO pins.

The prototype's actual safety and reliability depend on its hardware design, firmware implementation, and testing.

---

## Future Improvements

Potential extensions include:

- Configurable opening and closing thresholds.
- Improved filtering and hysteresis for stable distance detection.
- Gate-position feedback using limit switches or position sensors.
- Obstruction detection and safer actuator control.
- Wi-Fi-based monitoring through a local dashboard.
- Event logging and operating statistics.
- Configurable buzzer duration and alert settings.
- Enclosure design and a more reliable hardware mounting arrangement.
- Expanded diagnostics for sensor errors and controller status.

These features are proposed improvements and should not be interpreted as currently implemented.

---

## Project Objectives

GateX provides practical experience with:

- ESP32 firmware development.
- Ultrasonic sensor interfacing.
- Embedded control logic and state management.
- OLED graphics and visual feedback.
- GPIO-based input and output handling.
- Alert management and user interaction.
- Hardware integration and functional testing.

The project serves as a foundation for further development in embedded automation, smart access systems, and connected devices.

---

## Project Status

**Development stage:** Embedded automation prototype.

The repository should document the current hardware configuration, firmware behavior, and test results. Features should be marked as implemented only after they have been verified on the actual hardware.

---

## Author

**Manish Pal**

Aspiring Embedded Systems Engineer

- GitHub: [Mr-Manish-Pal](https://github.com/Mr-Manish-Pal)
- LinkedIn: [Manish Pal](https://www.linkedin.com/in/manish-pal-b34928438/)

---

## License

Add a license after choosing the terms under which others may use, modify, and distribute the project.
