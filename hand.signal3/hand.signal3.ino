#include "camera_setup.h"
#include "image_processing.h"
#include "ml_model.h"
#include "arm_control.h"
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Initialize Camera
    Serial.println("Initializing Camera...");
    if (!initializeCamera()) {
        Serial.println("Camera initialization failed!");
        while (1);
    }

    // Initialize Model
    Serial.println("Initializing Model...");
    initializeModel();

    
    // Initialize Arm Control
    Serial.println("Initializing Robotic Arm...");
    setupArm();
    
}

void loop() {
    // Capture a frame
    Serial.println("Capturing frame...");
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Failed to capture frame.");
        return;
    }

    // Preprocess image
    Serial.println("Preprocessing image...");
    // Pass the frame buffer directly to preprocessImage
    uint8_t* processedImage = preprocessImage(fb);
    if (!processedImage) {
        Serial.println("Image preprocessing failed!");
        esp_camera_fb_return(fb);
        return;
    }

    // Perform inference
    Serial.println("Performing inference...");
    int result = predictHandSignal(processedImage);
    if (result == -1) {
        Serial.println("Inference failed!");
        esp_camera_fb_return(fb);
        return;
    }

    // Output result
    Serial.print("Detected Hand Signal: ");
    Serial.println(getHandSignalLabel(result));

    // Execute robotic arm command based on the detected signal
    Serial.println("Executing arm command...");
    executeArmCommand(result);

    // Return frame buffer
    esp_camera_fb_return(fb);

    // Safety check to stop motors if idle
    safetyCheck();

    delay(1000); // Delay between inferences
}
