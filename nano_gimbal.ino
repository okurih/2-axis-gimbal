#include "I2Cdev.h"
#include "MPU6050.h"
#include <Wire.h>
#include <math.h>
#include <Servo.h>

MPU6050 accgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;

Servo servoPitch;
Servo servoRoll;

float pitch = 0;
float roll = 0;
unsigned long prevTime = 0;

// deadzone
const float deadzone = 0.5;

// min step
const float minStep = 0.5;

// smoothing 
const float servoResponse = 0.8;

//offset for mounting error
const float offset =6;

void setup()
{
    Wire.begin();
    Serial.begin(9600);

    accgyro.initialize();
    Serial.println(accgyro.testConnection() ? "MPU6050 connected" : "MPU6050 not connected");

    accgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    accgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);

    servoPitch.attach(4);
    servoRoll.attach(3);

    delay(500);

    // initialize angle from acc
    accgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    float ax_g = ax / 16384.0;
    float ay_g = ay / 16384.0;
    float az_g = az / 16384.0;

    pitch = atan2(ax_g, sqrt(ay_g*ay_g + az_g*az_g)) * 180 / PI;
    roll  = atan2(ay_g, sqrt(ax_g*ax_g + az_g*az_g)) * 180 / PI;

    prevTime = micros();

    servoPitch.write(90);
    servoRoll.write(90);
}

void loop()
{
    accgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    unsigned long now = micros();
    float dt = (now - prevTime) / 1000000.0;
    prevTime = now;

    // gyro deg/s
    float gx_deg = gx / 131.0;
    float gy_deg = gy / 131.0;

    // accel g
    float ax_g = ax / 16384.0;
    float ay_g = ay / 16384.0;
    float az_g = az / 16384.0;

    // accel angles
    float pitchAcc = atan2(ax_g, sqrt(ay_g*ay_g + az_g*az_g)) * 180 / PI;
    float rollAcc  = atan2(ay_g, sqrt(ax_g*ax_g + az_g*az_g)) * 180 / PI;

    // filter // right higher -> faster change
    pitch = 0.7 * pitch + 0.3 * pitchAcc;
    roll  = 0.7 * roll  + 0.3 * rollAcc;

    // angle deadzone
    static float pitchStable = 0;
    static float rollStable  = 0;

    if (abs(pitch - pitchStable) > deadzone)
        pitchStable = pitch;

    if (abs(roll - rollStable) > deadzone)
        rollStable = roll;

    // stabilizator
    float targetPitchServo = 90 - pitchStable;
    float targetRollServo  = 90 - rollStable;

    // servo limits
    targetPitchServo = constrain(targetPitchServo, 0, 180);
    targetRollServo  = constrain(targetRollServo, 0, 180);

    static float currentPitchServo = 90;
    static float currentRollServo  = 90;

    float pitchStep = (targetPitchServo - currentPitchServo) * servoResponse;
    float rollStep  = (targetRollServo  - currentRollServo)  * servoResponse;

    if (abs(pitchStep) < minStep)
        pitchStep = (pitchStep > 0) ? minStep : -minStep;

    if (abs(rollStep) < minStep)
        rollStep = (rollStep > 0) ? minStep : -minStep;

    if (abs(targetPitchServo - currentPitchServo) > deadzone)
        currentPitchServo += pitchStep;

    if (abs(targetRollServo - currentRollServo) > deadzone)
        currentRollServo += rollStep -offset;

    servoPitch.write(currentPitchServo);
    servoRoll.write(currentRollServo);

    // debug
    Serial.print("Pitch:");
    Serial.print(pitchStable);
    Serial.print(" Roll:");
    Serial.print(rollStable);
    Serial.print(" ServoP:");
    Serial.print(currentPitchServo);
    Serial.print(" ServoR:");
    Serial.println(currentRollServo);
}