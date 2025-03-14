#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 64 // Define buffer size here

typedef struct {
  uint8_t buffer[BUFFER_SIZE];
  uint32_t head;
  uint32_t tail;
} CircularBuffer;

void CircularBuffer_Init(CircularBuffer *cb);
bool CircularBuffer_IsEmpty(CircularBuffer *cb);
bool CircularBuffer_IsFull(CircularBuffer *cb);
void CircularBuffer_Put(CircularBuffer *cb, uint8_t data);
bool CircularBuffer_Get(CircularBuffer *cb, uint8_t *data);

#endif // CIRCULAR_BUFFER_H
