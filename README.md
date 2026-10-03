# 🚪 GateX

<p align="center">
  <img src="https://capsule-render.vercel.app/api?type=waving&color=0:00C6FF,50:0072FF,100:001F3F&height=220&section=header&text=GateX&fontSize=72&fontColor=ffffff&animation=fadeIn&fontAlignY=35&desc=Smart%20Ultrasonic%20Gate%20Position%20Monitoring%20System&descAlignY=58&descSize=18" width="100%"/>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Embedded%20Systems-Project-007ACC?style=for-the-badge"/>
  <img src="https://img.shields.io/badge/Ultrasonic-Sensing-FF6F00?style=for-the-badge"/>
  <img src="https://img.shields.io/badge/OLED-Interface-111111?style=for-the-badge"/>
  <img src="https://img.shields.io/badge/Real--Time-Control-2EA44F?style=for-the-badge"/>
  <img src="https://img.shields.io/badge/Status-Completed-success?style=for-the-badge"/>
</p>

<p align="center">
  <b>Sense the Distance • Determine the State • Communicate the Status</b>
</p>

---

## 🧠 Project Overview

**GateX** is a sensor-based embedded system designed to monitor the
position of a physical gate and determine whether it is **OPEN or CLOSED**
using **ultrasonic distance measurement**.

The GateX device is positioned behind the gate and uses an ultrasonic
sensor to measure the distance between the moving gate and a fixed
reference wall.

The measured distance is processed by the microcontroller and compared
against predefined thresholds. Based on the result, the system determines
the current gate state and provides immediate feedback through multiple
interfaces.

### GateX combines:

- 📡 Ultrasonic distance sensing
- 🧠 Embedded decision logic
- 🖥️ OLED graphical feedback
- 🔴🟢 LED status indication
- 🔊 Timed buzzer feedback
- 🔘 User-controlled silent mode
- ⚡ Real-time state monitoring

> **GateX converts a physical gate position into a real-time digital state
> and communicates that state through visual and audible interfaces.**

---

# 🎯 Problem Statement

Determining the position of a gate is commonly implemented using mechanical
limit switches or direct visual inspection.

However, a distance-based sensing approach can provide a simple and
flexible alternative.

The design question behind GateX was:

> **Can the physical position of a gate be determined using its distance
> from a fixed reference point and then communicated through an intuitive
> embedded interface?**

GateX addresses this problem using an ultrasonic sensor, threshold-based
state classification, and multiple feedback mechanisms.

---

# 💡 Core Engineering Concept

The complete system follows a simple embedded control pipeline:

```text
          PHYSICAL WORLD
                │
                ▼
      ┌──────────────────┐
      │ Ultrasonic Sensor│
      └────────┬─────────┘
               │
               ▼
       Distance Measurement
               │
               ▼
      ┌──────────────────┐
      │ Microcontroller  │
      │                  │
      │ Processing       │
      │ Threshold Logic  │
      │ State Detection  │
      └────────┬─────────┘
               │
               ▼
         Gate State
        ┌──────┴──────┐
        │             │
        ▼             ▼
      OPEN          CLOSED
        │             │
        ▼             ▼
      RED LED       GREEN LED
        │             │
        ▼             ▼
    Sad OLED       Happy OLED
        │             │
        ▼             ▼
   Buzzer 5 sec     No Buzzer

   ---

## 🧪 Testing & Validation

GateX was tested under different gate positions and operating conditions to verify reliable state detection.

### Test Cases

| Test Condition | Expected State | LED | OLED | Buzzer |
|---|---|---|---|---|
| Gate within calibrated open range | OPEN | 🔴 Red | 😢 Sad | 🔊 Beep |
| Gate beyond calibrated open range | CLOSED | 🟢 Green | 😊 Happy | 🔇 OFF |
| Gate remains OPEN | OPEN | 🔴 Red | 😢 Sad | 🔊 Timed |
| Silent Mode enabled | OPEN/CLOSED | Status LED active | OLED active | 🔇 Disabled |
| Gate returns to CLOSED | CLOSED | 🟢 Green | 😊 Happy | 🔇 OFF |

### Validation Goals

- Accurate gate-state detection
- Stable OPEN/CLOSED transitions
- Correct OLED state indication
- Correct LED status indication
- Controlled buzzer timing
- Silent Mode functionality
- Reliable operation during repeated gate movement

---

## ⚙️ Calibration

Ultrasonic-based detection depends on the physical installation position and distance between the sensor and the reference wall.

Therefore, GateX uses a **calibrated distance threshold** rather than assuming one fixed physical installation.

### Calibration Process

1. Install GateX behind the gate.
2. Point the ultrasonic sensor toward the reference wall.
3. Measure the distance when the gate is considered **OPEN**.
4. Measure the distance when the gate is considered **CLOSED**.
5. Select an appropriate threshold.
6. Test the system through multiple open/close cycles.
7. Adjust the threshold if required.

> Proper calibration improves reliability and reduces false state detection.

---

## 🧠 State-Based Control Logic

GateX follows a simple state-machine approach.

```text
                 ┌───────────────┐
                 │ Read Distance │
                 └───────┬───────┘
                         │
                         ▼
                ┌─────────────────┐
                │ Compare with    │
                │ calibrated      │
                │ threshold       │
                └────────┬────────┘
                         │
              ┌──────────┴──────────┐
              │                     │
              ▼                     ▼
        ┌───────────┐         ┌───────────┐
        │   OPEN    │         │  CLOSED   │
        └─────┬─────┘         └─────┬─────┘
              │                     │
              ▼                     ▼
        🔴 Red LED             🟢 Green LED
        😢 Sad OLED            😊 Happy OLED
        🔊 Buzzer              🔇 Buzzer OFF