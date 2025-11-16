#include "camera_module.h"
#include <string.h>

// -----------------------------------------------------------------------------
// Static state
// -----------------------------------------------------------------------------

static CameraState_t s_state = CAMERA_STATE_IDLE;
static CameraError_t s_error = CAMERA_ERROR_NONE;

static char s_rxLineBuf[CAMERA_MAX_TEXT_LEN];
static uint8_t s_rxIndex = 0;

static char s_lastText[CAMERA_MAX_TEXT_LEN];

// Callback function pointers (provided by user code)
static Camera_SendStringFn s_sendFn = 0;
static Camera_AvailableFn  s_availFn = 0;
static Camera_ReadFn       s_readFn = 0;

// Internal helper
static void Camera_ProcessLine(const char *line);

// -----------------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------------

void Camera_Init(Camera_SendStringFn sendFn,
                 Camera_AvailableFn  availFn,
                 Camera_ReadFn       readFn)
{
    s_sendFn  = sendFn;
    s_availFn = availFn;
    s_readFn  = readFn;

    s_state   = CAMERA_STATE_IDLE;
    s_error   = CAMERA_ERROR_NONE;
    s_rxIndex = 0;
    s_lastText[0] = '\0';
}

bool Camera_RequestScan(void)
{
    if (!s_sendFn)
        return false; // not initialized

    if (s_state != CAMERA_STATE_IDLE && s_state != CAMERA_STATE_RESULT_READY)
    {
        // Busy or in error; don't start new scan
        return false;
    }

    // Clear previous result/error
    s_error = CAMERA_ERROR_NONE;
    s_lastText[0] = '\0';

    // Send command: "CAM:SCAN\n"
    s_sendFn("CAM:SCAN\n");

    s_state = CAMERA_STATE_REQUEST_SENT;
    return true;
}

void Camera_Task(void)
{
    if (!s_availFn || !s_readFn)
        return;

    // Non-blocking: only processes already-available bytes
    while (s_availFn() > 0)
    {
        int ich = s_readFn();
        if (ich < 0)
        {
            // No valid byte; stop for now
            break;
        }

        char ch = (char)ich;

        // Line termination
        if (ch == '\n' || ch == '\r')
        {
            if (s_rxIndex > 0)
            {
                // Terminate line and process
                s_rxLineBuf[s_rxIndex] = '\0';
                Camera_ProcessLine(s_rxLineBuf);
                s_rxIndex = 0;
            }
            // Else ignore empty line
        }
        else
        {
            if (s_rxIndex < (CAMERA_MAX_TEXT_LEN - 1))
            {
                s_rxLineBuf[s_rxIndex++] = ch;
            }
            else
            {
                // Overflow: protocol error
                s_rxLineBuf[CAMERA_MAX_TEXT_LEN - 1] = '\0';
                s_error = CAMERA_ERROR_PROTOCOL;
                s_state = CAMERA_STATE_ERROR;
                // Optionally: flush until next newline
            }
        }
    }
}

CameraState_t Camera_GetState(void)
{
    return s_state;
}

bool Camera_HasNewText(void)
{
    return (s_state == CAMERA_STATE_RESULT_READY);
}

bool Camera_GetLastText(char *dest, uint8_t maxLen)
{
    if (!dest || maxLen == 0)
        return false;

    if (s_lastText[0] == '\0')
        return false; // no text

    strncpy(dest, s_lastText, maxLen - 1);
    dest[maxLen - 1] = '\0';
    return true;
}

void Camera_ClearResult(void)
{
    if (s_state == CAMERA_STATE_RESULT_READY)
    {
        s_state = CAMERA_STATE_IDLE;
    }
}

CameraError_t Camera_GetLastError(void)
{
    return s_error;
}

bool Camera_TextEquals(const char *expected)
{
    if (!expected)
        return false;
    if (s_lastText[0] == '\0')
        return false;

    return (strcmp(s_lastText, expected) == 0);
}

// -----------------------------------------------------------------------------
// Internal helper
// -----------------------------------------------------------------------------

static void Camera_ProcessLine(const char *line)
{
    // We only care if waiting for a scan result, or idle.
    if (s_state != CAMERA_STATE_REQUEST_SENT && s_state != CAMERA_STATE_IDLE)
    {
        // Ignore unsolicited lines in other states
        return;
    }

    const char *txtPrefix = "TXT:";
    const char *errPrefix = "ERR:";

    size_t txtLen = strlen(txtPrefix);
    size_t errLen = strlen(errPrefix);

    // TXT:<recognized_text>
    if (strncmp(line, txtPrefix, txtLen) == 0)
    {
        const char *payload = line + txtLen;
        strncpy(s_lastText, payload, CAMERA_MAX_TEXT_LEN - 1);
        s_lastText[CAMERA_MAX_TEXT_LEN - 1] = '\0';

        s_state = CAMERA_STATE_RESULT_READY;
        s_error = CAMERA_ERROR_NONE;
    }
    // ERR:<code>
    else if (strncmp(line, errPrefix, errLen) == 0)
    {
        const char *payload = line + errLen;

        // Very simple: any ERR: -> CAMERA_ERROR_REMOTE
        (void)payload; // not used here
        s_error = CAMERA_ERROR_REMOTE;
        s_state = CAMERA_STATE_ERROR;
        s_lastText[0] = '\0';
    }
    else
    {
        // Unknown message format
        s_error = CAMERA_ERROR_PROTOCOL;
        s_state = CAMERA_STATE_ERROR;
        s_lastText[0] = '\0';
    }
}
