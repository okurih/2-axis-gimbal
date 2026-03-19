# 2-axis gimbal
### | arduino nano | mpu-6050 | 2x mg995 | 5v supply |

Self stabilizing axis gimbal project using i2cdevlib for simple development focusing on the Pitch and Roll controll logic and smoothening algorithm.

---

## Hardware

| Component | Details |
|-|-|
| Microcontroller | Arduino Nano |
| IMU | MPU-6050 (i2c) |
| Servos | 2x mg995 |
| Power | 5V3A barrel jack + usb for nano |
| Body | custom 3d PLA print |

---
## Schematic & 3d models
|||
|-|-|
| ![schematic](./img/schematic.png) | ![schematic](./img/3dprints.png) |

---
## Demonstration



| roll and pitch calibration | axis |
|-|-|
| ![video](./img/gimbal.gif) | <img src="./img/pry.svg" style="background:white;"/> |
| |*Image: [Auawise](https://commons.wikimedia.org/wiki/File:Yaw_Axis_Corrected.svg), CC BY-SA 3.0, via Wikimedia Commons*|

---
## Code

> Currently has dependency on the i2cdevlib library for simplifying and cleaning up code. Might remove in the future.

| Dependencies |
|-|
|I2Cdev.cpp|
|I2Cdev.h|
|MPU6050.cpp|
|MPU6050.h|
|helper_3dmath.h|
