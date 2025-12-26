# ESP32-DualMPU-Shoulder-Rehab-Tracker
Real-time shoulder rehabilitation system using ESP32 and dual MPU6050 IMU sensors. Tracks arm flexion (forward bending) and abduction (sideways raising) relative to torso. Computes angles from accelerometer data, monitors maximum Range of Motion (ROM), calculates progress percentage against 180° target, and classifies recovery status: Severe (<30%), Moderate (<60%), Improving (<90%), Normal (≥90%).
Key Features:
Dual independent I2C buses (Arm: pins 20/21, Torso: 8/9)
Real-time angle calculation: Pitch/Roll via atan2 formulas
Medical-style serial output: Angle | Movement | Max_ROM | Progress% | Status
120ms update rate, battery-optimized
Session-based peak ROM tracking
Hardware: ESP32 + 2×MPU6050 (addresses 0x68/0x69)
Use Case: Home physiotherapy monitoring, research demos, telemedicine
