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
size_t CbRead(CircularBuffer *cb, char *buf, size_t len) {
  size_t i;
  for (i = 0; i < len; i++) {
    if (cb->Count == 0) {
      return i;
    }
    buf[i] = cb->data[cb->ReadPos++];
    cb->ReadPos %= BUFFER_SIZE;
    cb->Count--;
  }
  return len;
}

/**
 * @brief Gets the number of available bytes in the circular buffer.
 *
 * @param cb Pointer to the CircularBuffer structure.
 * @return The number of available bytes.
 */
size_t CbAvail(CircularBuffer *cb) { return cb->Count; }

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
