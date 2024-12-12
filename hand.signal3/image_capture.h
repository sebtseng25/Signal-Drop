#ifndef IMAGE_CAPTURE_H
#define IMAGE_CAPTURE_H

#include <esp_camera.h>
#include "FS.h"
#include "SD_MMC.h"
#include <Arduino.h>

void captureAndSaveImage() {
    // Capture an image from the camera
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed!");
        return;
    }

    Serial.print("Captured image size: ");
    Serial.println(fb->len);

    // Uncomment this block if using SD card for storage
    /*
    // Initialize SD card if not already done
    if (!SD_MMC.begin()) {
        Serial.println("Failed to initialize SD card!");
        esp_camera_fb_return(fb);
        return;
    }

    // Open a file on the SD card
    File file = SD_MMC.open("/image.jpg", FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing!");
        esp_camera_fb_return(fb);
        return;
    }

    // Write image data to the file
    file.write(fb->buf, fb->len);
    file.close();

    Serial.println("Image saved to SD card: /image.jpg");
    */

    // Return the frame buffer to free memory
    esp_camera_fb_return(fb);
    return;
}

#endif // IMAGE_CAPTURE_H
