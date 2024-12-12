#ifndef ARM_CONTROL_H
#define ARM_CONTROL_H

#include <Arduino.h>

// Define motor control pins for Xiao Expansion Board
#define IN1 3  // Motor 1 control pin
#define IN2 4  // Motor 2 control pin
#define IN3 5  // Motor 3 control pin
#define IN4 6  // Motor 4 control pin

// Timeout duration (in milliseconds) for safety
#define MOTOR_TIMEOUT 5000

// Last time a motor command was executed
unsigned long lastCommandTime = 0;

// Function declarations
void stopAllMotors();
void executeArmCommand(int signalIndex);
void setupArm();
void safetyCheck();

// Motor control setup
void setupArm() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    stopAllMotors();
    lastCommandTime = millis(); // Initialize command timer
}

void stopAllMotors() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.println("All motors stopped.");
}

// Safety check function to stop motors if idle too long
void safetyCheck() {
    if (millis() - lastCommandTime > MOTOR_TIMEOUT) {
        stopAllMotors();
        Serial.println("Motor safety timeout triggered. Motors stopped.");
    }
}

// Actions mapped to 10 hand signals
void executeArmCommand(int signalIndex) {
    stopAllMotors(); // Stop all motors before executing a new command

    switch (signalIndex) {
        case 0: // Signal 1: Grip in 
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            break;
        case 1: // Signal 2: Grip out
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            break;
        case 2: // Signal 3: Move up 
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            break;
        case 3: // Signal 4: Move down
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            break;
        case 4: // Signal 5: Grip in and move up 
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            break;
        case 5: // Signal 6: Grip out and move up 
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            break;
        case 6: // Signal 7: Grip in and move down 
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            break;
        case 7: // Signal 8: Grip out and move down 
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            break;
        case 8: // Signal 9: Grip in 
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            break;
        case 9: // Signal 10: Grip out 
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            break;
        default:
            Serial.printf("Unknown signal: %d. Stopping all motors.\n", signalIndex);
            stopAllMotors(); // Stop all motors for unknown signals
            break;
    }
}

#endif // ARM_CONTROL_H
