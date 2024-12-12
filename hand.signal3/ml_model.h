#ifndef ML_MODEL_H
#define ML_MODEL_H

#include "model.h"
#include <TensorFlowLite_ESP32.h>
#include <Arduino.h>

// Undefine the conflicting DEFAULT macro
#undef DEFAULT

#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Define Tensor Arena size and tensor arena memory buffer
#define TENSOR_ARENA_SIZE 10240
static uint8_t tensor_arena[TENSOR_ARENA_SIZE];

// Declare pointers for model and interpreter
static const tflite::Model* model = nullptr;
static tflite::MicroInterpreter* interpreter = nullptr;

// Tensor pointers
static TfLiteTensor* input_tensor = nullptr;
static TfLiteTensor* output_tensor = nullptr;

// Labels for hand signals
static const char* handSignalLabels[] = {"I", "O", "U", "D", "X", "Y", "E", "F", "R", "L"};

void initializeModel() {
    // Load the TensorFlow Lite model
    model = tflite::GetModel(model_data);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        Serial.printf("Model version mismatch! Expected %d, but got %d\n", TFLITE_SCHEMA_VERSION, model->version());
        while (1); // Stop execution on error
    }

    // Create an error reporter
    static tflite::MicroErrorReporter micro_error_reporter;
    tflite::ErrorReporter* error_reporter = &micro_error_reporter;

    // Use MicroMutableOpResolver to register only the required operations
    static tflite::MicroMutableOpResolver<4> resolver;
    resolver.AddConv2D();          // Add Conv2D operation
    resolver.AddMean();            // Add Mean operation
    resolver.AddFullyConnected();  // Add FullyConnected operation
    resolver.AddSoftmax();         // Add Softmax operation

    // Create the interpreter
    static tflite::MicroInterpreter static_interpreter(model, resolver, tensor_arena, TENSOR_ARENA_SIZE, error_reporter);
    interpreter = &static_interpreter;

    // Allocate memory for tensors
    if (interpreter->AllocateTensors() != kTfLiteOk) {
        Serial.println("AllocateTensors failed! Check tensor arena size or model compatibility.");
        while (1); // Stop execution on error
    }

    // Set up input and output tensor pointers
    input_tensor = interpreter->input(0);
    output_tensor = interpreter->output(0);

    // Debug: Log tensor dimensions
    Serial.println("Model initialized successfully.");
    Serial.printf("Input tensor dims: [");
    for (int i = 0; i < input_tensor->dims->size; i++) {
        Serial.printf("%d", input_tensor->dims->data[i]);
        if (i < input_tensor->dims->size - 1) Serial.print(", ");
    }
    Serial.println("]");

    Serial.printf("Output tensor dims: [");
    for (int i = 0; i < output_tensor->dims->size; i++) {
        Serial.printf("%d", output_tensor->dims->data[i]);
        if (i < output_tensor->dims->size - 1) Serial.print(", ");
    }
    Serial.println("]");
}

int predictHandSignal(uint8_t* inputImage) {
    // Verify input size
    if (input_tensor->bytes != sizeof(uint8_t) * input_tensor->dims->data[1]) {
        Serial.println("Input size mismatch!");
        return -1;
    }

    // Prepare the input tensor
    memcpy(input_tensor->data.uint8, inputImage, input_tensor->bytes);

    // Perform inference
    if (interpreter->Invoke() != kTfLiteOk) {
        Serial.println("Model inference failed!");
        return -1;
    }

    // Find the highest confidence value in the output tensor
    int maxIndex = 0;
    float maxConfidence = -1.0f;
    for (int i = 0; i < output_tensor->dims->data[1]; i++) {
        float confidence = output_tensor->data.f[i];
        if (confidence > maxConfidence) {
            maxConfidence = confidence;
            maxIndex = i;
        }
    }

    return maxIndex;
}

const char* getHandSignalLabel(int index) {
    if (index < 0 || index >= sizeof(handSignalLabels) / sizeof(handSignalLabels[0])) {
        return "Unknown";
    }
    return handSignalLabels[index];
}

#endif // ML_MODEL_H
