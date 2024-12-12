#ifndef CAMERA_SETUP_H
#define CAMERA_SETUP_H

#include <esp_camera.h>
#include <Arduino.h>

#define CAMERA_MODEL_XIAO_ESP32S3

// Camera pin configuration (specific to Xiao ESP32S3)
#define PWDN_GPIO_NUM     -1 
#define RESET_GPIO_NUM    -1 
#define XCLK_GPIO_NUM     10 
#define SIOD_GPIO_NUM     40 
#define SIOC_GPIO_NUM     39
#define Y9_GPIO_NUM       48 
#define Y8_GPIO_NUM       11 
#define Y7_GPIO_NUM       12 
#define Y6_GPIO_NUM       14 
#define Y5_GPIO_NUM       16 
#define Y4_GPIO_NUM       18 
#define Y3_GPIO_NUM       17 
#define Y2_GPIO_NUM       15 
#define VSYNC_GPIO_NUM    38 
#define HREF_GPIO_NUM     47 
#define PCLK_GPIO_NUM     13
#define LED_GPIO_NUM      21

bool initializeCamera() {
    // Camera configuration
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000; // 20 MHz clock
    config.pixel_format = PIXFORMAT_JPEG; // JPEG format

    // Frame size, quality, and buffer count
    config.frame_size = FRAMESIZE_VGA; // VGA resolution (640x480)
    config.jpeg_quality = 12;         // JPEG quality (1-63, lower is better)
    config.fb_count = 2;              // Use 2 frame buffers for better performance

    // Initialize the camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera initialization failed with error 0x%x\n", err);
        return false;
    }

    Serial.println("Camera initialized successfully.");
    return true;
}

#endif // CAMERA_SETUP_H
