#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Max OCR text length (including '\0')
#define CAMERA_MAX_TEXT_LEN 64

// Camera state enum
typedef enum
{
    CAMERA_STATE_IDLE = 0,
    CAMERA_STATE_REQUEST_SENT,
    CAMERA_STATE_RESULT_READY,
    CAMERA_STATE_ERROR
} CameraState_t;

// Error codes
typedef enum
{
    CAMERA_ERROR_NONE = 0,
    CAMERA_ERROR_PROTOCOL = 1,
    CAMERA_ERROR_REMOTE  = 2,
    CAMERA_ERROR_TIMEOUT = 3   // optional, not used yet
} CameraError_t;

// -----------------------------------------------------------------------------
// Callback types for serial I/O (pure C, no Arduino types here)
// -----------------------------------------------------------------------------

// Send a null-terminated string out to the camera/OCR link.
// Example implementation on Arduino: Serial1.print(str);
typedef void (*Camera_SendStringFn)(const char *str);

// Return how many bytes are available to read.
// Example: return Serial1.available();
typedef int  (*Camera_AvailableFn)(void);

// Read one byte from the serial link, return it as int (like Serial.read()).
// If no data, you normally don't call this (you check available first).
typedef int  (*Camera_ReadFn)(void);

// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------

// Initialize camera driver with serial callback functions.
// Must be called once at startup.
void Camera_Init(Camera_SendStringFn sendFn,
                 Camera_AvailableFn  availFn,
                 Camera_ReadFn       readFn);

// Start a new OCR scan (non-blocking).
// Returns true if command sent, false if busy or in error state.
bool Camera_RequestScan(void);

// Periodic task to be called regularly (e.g. each loop iteration).
// Processes incoming serial bytes and updates internal state.
void Camera_Task(void);

// Get current state.
CameraState_t Camera_GetState(void);

// True if OCR result is ready.
bool Camera_HasNewText(void);

// Copy last recognized text into dest (up to maxLen).
// Returns true if text copied, false if no text.
bool Camera_GetLastText(char *dest, uint8_t maxLen);

// Clear the "result ready" flag (go back to IDLE).
void Camera_ClearResult(void);

// Get last error code.
CameraError_t Camera_GetLastError(void);

// Compare last OCR text with an expected string (case-sensitive).
bool Camera_TextEquals(const char *expected);

#ifdef __cplusplus
}
#endif

#endif // CAMERA_MODULE_H
