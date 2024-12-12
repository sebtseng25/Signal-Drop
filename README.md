# Signal-Drop
Hand signal detection to move an OWI robot arm

## Introduction
The premise of the project is to integrate a Xiao ESP32S3 with ASL letter hand symbol recognition and robotic arm control. The Xiao ESP32S3 is programmed to detect and classify 10 different ASL letters through a machine learning model deployed via the Arduino TensorFlow Lite library. Once the hand symbol is recognized, the corresponding motor movement will activate in the robot arm. The project demonstrates the possibility to interconnect human hand communication and machine interfacing. The combination of gesture recognition and robotic response serves as a foundation for more advanced applications in communication and automation. 

## Motivation: 
One may not realize it, but hand signals are an integral part of daily communication. Simple gestures such as waving your hand signify a friendly “Hello,” pointing a finger in a specific direction directs someone’s attention, and beckoning with a finger invites someone to come closer. These non-verbal cues are natural, intuitive, and universally understood, allowing us to convey messages quickly and efficiently without the need for spoken language. 

As interactions between people and machines or AI systems continue to grow, the need for intuitive and accessible methods of communication with these systems becomes more apparent. Current human-machine interfaces often require detailed, explicit instructions or technical knowledge to operate effectively. For example, instructing a self-driving car typically involves specifying exact coordinates, or operating a household robot might require navigating a series of complex commands or menus. This lack of natural interaction creates a barrier, making such technologies more unnatural to communicate with for the average everyday person, especially for those who are not familiar with modern technology.

If we want AI-driven machines to become seamlessly integrated into everyday life, they must be capable of understanding commands in the same way humans do. Hand signals offer a promising solution to bridge this communication gap. Although other communication methods such as voice commands are also effective and important, they can be hindered by language barriers, accents, or environmental noise, hand gestures are silent, universally recognizable, and can be used in a variety of settings. By enabling robotic systems to recognize and respond to hand gestures, we take a significant step toward making AI technology more user-friendly and accessible. This advancement not only improves the ease of operation but also expands the potential for AI to be applied to areas of focus such as the military, elderly assistance, and disability accommodation.

Thus, robotic understanding of human hand signals represents a crucial milestone in the broader effort to integrate AI seamlessly into everyday life. By developing intuitive interfaces that mimic human-to-human communication, we move closer to a future where interacting with machines is as effortless and natural as interacting with another person.

## Hardware Construct
The main hardware components used are the ESP32S3 Sensor and Xiao board, the L298N motor driver, and the OWI Robot Edge Arm

## Code Structure 
wifi_setup.h: 
The code implements a WiFi setup module for the robot arm sensor project, allowing the ESP32 board to connect to a specified network using predefined credentials. The function connectToWiFi attempts to establish a connection within a 10-second timeout period. Connection status is reported via the serial monitor, displaying the device's IP address upon successful connection or indicating failure if the connection cannot be established.

model.h: 
This file defines the interface for the TensorFlow Lite model used in the robot arm sensor project. It declares the model_data byte array, which contains the serialized TensorFlow Lite model, and model_data_len which specifies the length of this data. These declarations allow the model to be loaded and utilized for real-time gesture recognition on the ESP32 board.

model.cpp: 
The file contains the TensorFlow Lite model data necessary for the robot arm sensor project. The model, trained and exported as a tflite file using Google Colab, is converted into a C++ byte array (model_data). This byte array stores the binary representation of the model of our 10 different ASL letter symbols, making it suitable for deployment on the ESP32 microcontroller. The file also defines model_data_len, which specifies the length of the model data array. This structure allows the ESP32 to load the model at runtime and perform real-time gesture recognition to control the robot arm based on hand signals.

ml_model.h: 
This file initializes and manages the TensorFlow Lite model for the robot arm sensor project on the ESP32. The initializeModel function loads the TensorFlow Lite model, sets up the interpreter, and allocates memory for input and output tensors using a fixed tensor arena. The predictHandSignal function processes an input image, performs inference to recognize hand signals, and returns the index of the predicted class. The getHandSignalLabel function maps the predicted index to a corresponding hand signal label for user-friendly output. This setup allows real-time gesture recognition to control the robot arm based on detected hand signals.

image_processing.h: 
The image_processing.h file contains functions for preparing camera images for use with the TensorFlow Lite model in the robot arm sensor project. The preprocessImage function captures the raw image from the ESP32 camera, converts it to grayscale if necessary, and resizes it to a fixed size of 96x96 pixels to match the model's input dimensions. The function also normalizes the pixel values, ensuring compatibility with the model's requirements. This preprocessing ensures consistent input data, enabling accurate hand gesture recognition for controlling the robot arm.

image_capture.h: 
The image_capture.h file provides functionality to capture an image from the ESP32 camera module and save it to an SD card. The captureAndSaveImage function captures a snapshot using the camera and logs the image size to the serial monitor. It then initializes the SD card, opens a file for writing, and saves the image data as image.jpg on the SD card. After successfully saving the image, the frame buffer is released to free memory and maintain system efficiency. This ensures the captured image is stored persistently for later use or analysis.

camera_setup.h: 
The camera_setup.h file configures and initializes the camera module for the Xiao ESP32S3 board. The initializeCamera function defines the camera pin configurations, sets up the clock frequency, and specifies the image format as JPEG with a resolution of 640x480 (VGA). It also sets the JPEG quality to 12 and uses two frame buffers for better performance. The function initializes the camera using the provided configuration and returns true if successful, or logs an error and returns false if initialization fails. This setup ensures the camera is ready for capturing images in the robot arm sensor project.

arm_control.h: 
The arm_control.h file defines the motor control functions for the robot arm using the Xiao ESP32S3 Expansion Board. The setupArm function initializes motor control pins and ensures all motors are stopped at startup. The executeArmCommand function maps 10 hand signals to specific motor actions, such as gripping, moving up or down, or combined movements. A safety mechanism, implemented in the safetyCheck function, stops all motors if no command is received within 5 seconds to prevent unintended operation. This ensures precise and safe control of the robot arm based on recognized hand signals.

hand.signal3.ino: 
The main function in this code orchestrates the overall workflow of the robot arm sensor project. The setup function initializes the serial communication, camera, TensorFlow Lite model, and robotic arm control system. The loop function continuously captures an image using the ESP32 camera, preprocesses the image to match the model's input requirements, and performs inference to detect hand signals. Based on the inference result, the corresponding command is executed to control the robotic arm. A safety check is also performed to stop the motors if no commands are received within a specified timeout, ensuring safe operation.



