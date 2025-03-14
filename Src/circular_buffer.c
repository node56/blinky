#include "circular_buffer.h"

void CircularBuffer_Init(CircularBuffer *cb) {
  cb->head = 0;
  cb->tail = 0;
}

bool CircularBuffer_IsEmpty(CircularBuffer *cb) {
  return cb->head == cb->tail;
}

bool CircularBuffer_IsFull(CircularBuffer *cb) {
  return (cb->head + 1) % BUFFER_SIZE == cb->tail;
}

void CircularBuffer_Put(CircularBuffer *cb, uint8_t data) {
  __disable_irq();
  cb->buffer[cb->head] = data;
  cb->head = (cb->head + 1) % BUFFER_SIZE;
  if (cb->head == cb->tail) {
    // Buffer overflow, overwrite the oldest data
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
  }
  __enable_irq();
}

bool CircularBuffer_Get(CircularBuffer *cb, uint8_t *data) {
  __disable_irq();
  if (CircularBuffer_IsEmpty(cb)) {
    __enable_irq();
    return false; // Buffer is empty
  }
  *data = cb->buffer[cb->tail];
  cb->tail = (cb->tail + 1) % BUFFER_SIZE;
  __enable_irq();
  return true;
}
