#ifndef CIRCBUFFER_H
#define CIRCBUFFER_H

#include "nodeStructure.h"
#include <stddef.h>

void CbInit(CircularBuffer *cb);
size_t CbWrite(CircularBuffer *cb, char *data, size_t len);
int CbRead(CircularBuffer *cb, char *buf, size_t len);
int CbAvail(CircularBuffer *cb);
void LiberateCircularBuffer(CircularBuffer *cb);

#endif // !CIRCBUFFER_H