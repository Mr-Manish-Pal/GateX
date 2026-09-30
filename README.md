# 🚪 GateX

<p align="center">
  <strong>Smart Ultrasonic Gate Position Monitoring & Automation System</strong>
</p>

<p align="center">
  A compact embedded system that determines gate status using ultrasonic distance measurement
  and provides real-time visual and audible feedback.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Embedded%20Systems-Project-blue?style=for-the-badge">
  <img src="https://img.shields.io/badge/Ultrasonic-Sensing-orange?style=for-the-badge">
  <img src="https://img.shields.io/badge/OLED-Display-black?style=for-the-badge">
  <img src="https://img.shields.io/badge/Status-Completed-success?style=for-the-badge">
</p>

---

## 📌 Overview

**GateX** is an embedded gate-position monitoring and automation system designed to
determine whether a gate is **OPEN or CLOSED** by measuring the distance between the
gate and a reference wall using an **ultrasonic sensor**.

Instead of relying on a conventional mechanical limit switch, GateX uses
**distance-based sensing** to identify the gate's position.

The system combines:

- Ultrasonic distance measurement
- Threshold-based decision logic
- OLED graphical feedback
- LED status indication
- Audible buzzer feedback
- User-controlled silent mode

into a single compact embedded system.

---

## 🎯 Problem Statement

A conventional gate-position system may require mechanical switches or direct
visual inspection to determine whether a gate is open or closed.

GateX explores a simple alternative:

> **Can the position of a gate be determined reliably using its distance from a
> fixed reference point?**

The answer is implemented through ultrasonic sensing and threshold-based
embedded decision logic.

---

## ⚙️ Working Principle

The GateX device is mounted behind the gate, facing the reference wall.

The ultrasonic sensor continuously measures the distance between the gate and
the wall.

The measured distance is compared against predefined thresholds.

```text
              GATE
                │
                │
          Ultrasonic Sensor
                │
                ▼
        Distance Measurement
                │
                ▼
        Threshold Comparison
                │
          ┌─────┴─────┐
          │           │
       1–2 ft       > 2 ft
          │           │
          ▼           ▼
        OPEN        CLOSED