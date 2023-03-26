#include <stddef.h>
#include <string.h>

#include "CirBuffer.h"

/**
 * @brief Initializes a circular buffer.
 *
 * @param cb Pointer to the CircularBuffer structure.
 */
void CbInit(CircularBuffer *cb) {
  cb->ReadPos = 0;
  cb->WritePos = 0;
  cb->Count = 0;
}

/**
 * @brief Writes data to the circular buffer.
 *
 * @param cb Pointer to the CircularBuffer structure.
 * @param data Pointer to the data to be written.
 * @param len Length of data to be written.
 * @return The number of bytes written.
 */
size_t CbWrite(CircularBuffer *cb, char *data, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) {
    if (cb->Count == BUFFER_SIZE) {
      return i;
    }
    cb->data[cb->WritePos++] = data[i];
    cb->WritePos %= BUFFER_SIZE;
    cb->Count++;
  }
  return len;
}

/**
 * @brief Reads data from the circular buffer.
 *
 * @param cb Pointer to the CircularBuffer structure.
 * @param buf Pointer to the buffer to store the read data.
 * @param len Length of data to be read.
 * @return The number of bytes read.
 */
int CbRead(CircularBuffer *cb, char *buf, size_t len) {
  size_t i;
  int TempReadPos = cb->ReadPos;
  int TempCount = cb->Count;

  for (i = 0; i < len; i++) {
    if (cb->Count == 0) {
      cb->ReadPos = TempReadPos, cb->Count = TempCount;
      return 0;
    }
    buf[i] = cb->data[cb->ReadPos++];
    cb->ReadPos %= BUFFER_SIZE;
    cb->Count--;
    if (buf[i] == '\n') {
      // Enter \0 to differentiate message from trash
      buf[i + 1] = '\0';
      return 1;
    }
  }
  cb->ReadPos = TempReadPos, cb->Count = TempCount;
  return 0;
}

/**
 * @brief Gets the number of available bytes in the circular buffer.
 *
 * @param cb Pointer to the CircularBuffer structure.
 * @return The number of available bytes.
 */
int CbAvail(CircularBuffer *cb) { return BUFFER_SIZE - cb->Count; }

/**
 * @brief Resets the circular buffer.
 *
 * @param cb Pointer to the CircularBuffer structure.
 */
void LiberateCircularBuffer(CircularBuffer *cb) {
  cb->WritePos = 0;
  cb->ReadPos = 0;
  cb->Count = 0;
  memset(cb->data, 0, BUFFER_SIZE);
}
