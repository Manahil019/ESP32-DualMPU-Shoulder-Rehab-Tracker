#include <Wire.h>
#include <math.h>

TwoWire I2C_Arm = Wire;
TwoWire I2C_Torso = TwoWire(1);

#define MPU_ARM   0x68
#define MPU_TORSO 0x69
#define MAX_ROM 180.0

float maxROM = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  I2C_Arm.begin(20, 21);
  I2C_Torso.begin(8, 9);

  initMPU(I2C_Arm, MPU_ARM);
  initMPU(I2C_Torso, MPU_TORSO);

  Serial.println("Angle Movement Max_ROM Progress% Status");
}

void loop() {
  int16_t axA, ayA, azA, gx, gy, gz;
  int16_t axT, ayT, azT;

  readMPU(I2C_Arm, MPU_ARM, axA, ayA, azA, gx, gy, gz);
  readMPU(I2C_Torso, MPU_TORSO, axT, ayT, azT, gx, gy, gz);

  // Compute angles
  float armPitch   = atan2(axA, sqrt(ayA*ayA + azA*azA)) * 180 / PI;
  float armRoll    = atan2(ayA, sqrt(axA*axA + azA*azA)) * 180 / PI;

  float torsoPitch = atan2(axT, sqrt(ayT*ayT + azT*azT)) * 180 / PI;
  float torsoRoll  = atan2(ayT, sqrt(axT*axT + azT*azT)) * 180 / PI;

  float flexion   = abs(armPitch - torsoPitch);
  float abduction = abs(armRoll  - torsoRoll);

  float shoulderAngle;
  String movement;

  if (flexion > abduction) {
    shoulderAngle = flexion;
    movement = "Flexion";
  } else {
    shoulderAngle = abduction;
    movement = "Abduction";
  }

  shoulderAngle = constrain(shoulderAngle, 0, MAX_ROM);

  if (shoulderAngle > maxROM)
    maxROM = shoulderAngle;

  float progress = (maxROM / MAX_ROM) * 100;

  String status;
  if (progress < 30) status = "Severe";
  else if (progress < 60) status = "Moderate";
  else if (progress < 90) status = "Improving";
  else status = "Normal";

  // ---- SERIAL OUTPUT (MEDICAL STYLE) ----
  Serial.print("Angle:");
  Serial.print(shoulderAngle);

  Serial.print("\tMovement:");
  Serial.print(movement);

  Serial.print("\tMax_ROM:");
  Serial.print(maxROM);

  Serial.print("\tProgress%:");
  Serial.print(progress);

  Serial.print("\tStatus:");
  Serial.print(status);

  Serial.println();
  delay(120);
}

// ------------------------

void initMPU(TwoWire &bus, byte addr) {
  bus.beginTransmission(addr);
  bus.write(0x6B);
  bus.write(0x00);
  bus.endTransmission();
}

void readMPU(TwoWire &bus, byte addr,
             int16_t &ax, int16_t &ay, int16_t &az,
             int16_t &gx, int16_t &gy, int16_t &gz) {
  bus.beginTransmission(addr);
  bus.write(0x3B);
  bus.endTransmission(false);
  bus.requestFrom(addr, 14, true);

  ax = bus.read()<<8 | bus.read();
  ay = bus.read()<<8 | bus.read();
  az = bus.read()<<8 | bus.read();
  bus.read(); bus.read();
  gx = bus.read()<<8 | bus.read();
  gy = bus.read()<<8 | bus.read();
  gz = bus.read()<<8 | bus.read();
}
