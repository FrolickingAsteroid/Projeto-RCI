//===-- circbuffer.h -----------------------------------------------------===//
//
// This header file defines functions for managing a circular buffer. The functions
// include initializing the buffer, writing and reading data, checking the available
// space, and freeing the buffer's memory.
//
//===----------------------------------------------------------------------===//
#ifndef CIRCBUFFER_H
#define CIRCBUFFER_H

#include "nodeStructure.h"
#include <stddef.h>

// Initialize a circular buffer
void CbInit(CircularBuffer *cb);
// Write data to the circular buffer
size_t CbWrite(CircularBuffer *cb, char *data, size_t len);
// Read data from the circular buffer
int CbRead(CircularBuffer *cb, char *buf, size_t len);
// Check the available space in the circular buffer
int CbAvail(CircularBuffer *cb);
// Free the memory allocated for the circular buffer
void LiberateCircularBuffer(CircularBuffer *cb);

#endif // !CIRCBUFFER_H
