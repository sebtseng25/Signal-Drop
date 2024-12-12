#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <stdint.h>
#include <esp_camera.h>
#include <Arduino.h>

// Ensure your model input matches these values
#define MODEL_INPUT_WIDTH 96
#define MODEL_INPUT_HEIGHT 96
#define MODEL_INPUT_CHANNELS 1
#define MODEL_INPUT_SIZE (MODEL_INPUT_WIDTH * MODEL_INPUT_HEIGHT * MODEL_INPUT_CHANNELS)

static uint8_t processedImage[MODEL_INPUT_SIZE];

uint8_t* preprocessImage(camera_fb_t* fb) {

    if (!fb || !fb->buf) {
        Serial.println("Invalid frame buffer!");
        return nullptr;
    }

    // Ensure input format is RGB or grayscale
    if (fb->format != PIXFORMAT_RGB565 && fb->format != PIXFORMAT_GRAYSCALE) {
        Serial.println("Unsupported image format! Use RGB565 or Grayscale.");
        return nullptr;
    }

    // Step 1: Convert to grayscale (if not already grayscale)
    size_t srcWidth = fb->width;
    size_t srcHeight = fb->height;
    uint8_t* grayscaleImage = new uint8_t[srcWidth * srcHeight];

    if (fb->format == PIXFORMAT_RGB565) {
        for (size_t i = 0, j = 0; i < fb->len; i += 2, j++) {
            uint16_t pixel = (fb->buf[i] << 8) | fb->buf[i + 1];
            uint8_t r = (pixel >> 11) & 0x1F;
            uint8_t g = (pixel >> 5) & 0x3F;
            uint8_t b = pixel & 0x1F;
            r = (r << 3) | (r >> 2); // Scale 5-bit to 8-bit
            g = (g << 2) | (g >> 4); // Scale 6-bit to 8-bit
            b = (b << 3) | (b >> 2); // Scale 5-bit to 8-bit
            grayscaleImage[j] = static_cast<uint8_t>(r * 0.2989 + g * 0.5870 + b * 0.1140); // Weighted average
        }
    } else if (fb->format == PIXFORMAT_GRAYSCALE) {
        memcpy(grayscaleImage, fb->buf, fb->len);
    }

    // Step 2: Resize to MODEL_INPUT_WIDTH x MODEL_INPUT_HEIGHT
    for (int y = 0; y < MODEL_INPUT_HEIGHT; y++) {
        for (int x = 0; x < MODEL_INPUT_WIDTH; x++) {
            int srcX = x * srcWidth / MODEL_INPUT_WIDTH;
            int srcY = y * srcHeight / MODEL_INPUT_HEIGHT;
            processedImage[y * MODEL_INPUT_WIDTH + x] = grayscaleImage[srcY * srcWidth + srcX];
        }
    }

    delete[] grayscaleImage; // Free allocated memory

    // Step 3: Normalize pixel values (to [0, 255] or [0, 1], based on model requirements)
    for (int i = 0; i < MODEL_INPUT_SIZE; i++) {
        processedImage[i] = processedImage[i]; // [0-255]. If [0, 1] is needed, convert to float array and divide by 255.0
    }

    return processedImage;
}

#endif // IMAGE_PROCESSING_H
