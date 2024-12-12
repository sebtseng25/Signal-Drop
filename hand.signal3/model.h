#ifndef MODEL_H
#define MODEL_H
#include <Arduino.h>

// This header defines the TensorFlow Lite model data.
// The model is stored as a byte array, generated from a .tflite file
// using a conversion tool like xxd or a similar utility.

extern const unsigned char model_data[]; // Model binary data
extern const int model_data_len;         // Length of the model data

#endif // MODEL_H
