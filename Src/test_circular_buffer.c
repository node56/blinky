#include "circular_buffer.h"
#include <assert.h>

void test_circular_buffer_init() {
  CircularBuffer cb;
  CircularBuffer_Init(&cb);
  assert(CircularBuffer_IsEmpty(&cb));
  assert(!CircularBuffer_IsFull(&cb));
}

void test_circular_buffer_put_get() {
  CircularBuffer cb;
  CircularBuffer_Init(&cb);
  uint8_t data = 10;
  CircularBuffer_Put(&cb, data);
  assert(!CircularBuffer_IsEmpty(&cb));
  uint8_t retrieved_data;
  assert(CircularBuffer_Get(&cb, &retrieved_data));
  assert(retrieved_data == data);
  assert(CircularBuffer_IsEmpty(&cb));
}

void test_circular_buffer_full() {
  CircularBuffer cb;
  CircularBuffer_Init(&cb);
  for (int i = 0; i < BUFFER_SIZE - 1; i++) {
    CircularBuffer_Put(&cb, i);
  }
  assert(!CircularBuffer_IsFull(&cb));
  CircularBuffer_Put(&cb, BUFFER_SIZE - 1);
  assert(CircularBuffer_IsFull(&cb));
}

void test_circular_buffer_overflow() {
  CircularBuffer cb;
  CircularBuffer_Init(&cb);
  for (int i = 0; i < BUFFER_SIZE + 5; i++) {
    CircularBuffer_Put(&cb, i);
  }
  assert(CircularBuffer_IsFull(&cb));
  uint8_t retrieved_data;
  for (int i = 0; i < BUFFER_SIZE; i++) {
    assert(CircularBuffer_Get(&cb, &retrieved_data));
    // We can't assert the exact values due to overflow, but we can check that we get data
  }
  assert(CircularBuffer_IsEmpty(&cb));
}

int main() {
  test_circular_buffer_init();
  test_circular_buffer_put_get();
  test_circular_buffer_full();
  test_circular_buffer_overflow();
  return 0;
}
