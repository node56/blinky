#include "main.h"

// UART receive buffers
#define UART_BUFFER_SIZE 64
uint8_t uart1_rx_buffer[UART_BUFFER_SIZE];
uint32_t uart1_rx_head = 0;
uint32_t uart1_rx_tail = 0;

uint8_t uart2_rx_buffer[UART_BUFFER_SIZE];
uint32_t uart2_rx_head = 0;
uint32_t uart2_rx_tail = 0;

// UART transmit buffers
uint8_t uart1_tx_buffer[UART_BUFFER_SIZE];
uint32_t uart1_tx_head = 0;
uint32_t uart1_tx_tail = 0;

uint8_t uart2_tx_buffer[UART_BUFFER_SIZE];
uint32_t uart2_tx_head = 0;
uint32_t uart2_tx_tail = 0;

// Function prototypes
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void state_machine(void);

// UART initialization functions
void MX_USART1_UART_Init(void) {
  LL_USART_InitTypeDef USART_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);

  /* Peripheral clock enable */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_USART1);

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9|LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, 0);
  NVIC_EnableIRQ(USART1_IRQn);
  LL_USART_EnableIT_RXNE(USART1);
}

void MX_USART2_UART_Init(void) {
  LL_USART_InitTypeDef USART_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration
  PA2   ------> USART2_TX
  PA3   ------> USART2_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART2);
  LL_USART_Enable(USART2);

    /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);
  LL_USART_EnableIT_RXNE(USART2);
}

// UART interrupt handlers
void USART1_IRQHandler(void) {
  if (LL_USART_IsActiveFlag_RXNE(USART1)) {
    uint8_t received_char = LL_USART_ReceiveData8(USART1);
    uart1_rx_buffer[uart1_rx_head] = received_char;
    uart1_rx_head = (uart1_rx_head + 1) % UART_BUFFER_SIZE;
    if (uart1_rx_head == uart1_rx_tail) {
      // Buffer overflow, drop the character
      uart1_rx_tail = (uart1_rx_tail + 1) % UART_BUFFER_SIZE;
    }
  }
}

void USART2_IRQHandler(void) {
  if (LL_USART_IsActiveFlag_RXNE(USART2)) {
    uint8_t received_char = LL_USART_ReceiveData8(USART2);
    uart2_rx_buffer[uart2_rx_head] = received_char;
    uart2_rx_head = (uart2_rx_head + 1) % UART_BUFFER_SIZE;
    if (uart2_rx_head == uart2_rx_tail) {
      // Buffer overflow, drop the character
      uart2_rx_tail = (uart2_rx_tail + 1) % UART_BUFFER_SIZE;
    }
  }
}

// State machine function
void state_machine(void) {
  // Check for received characters and echo them
  if (uart1_rx_head != uart1_rx_tail) {
    uint8_t char_to_send = uart1_rx_buffer[uart1_rx_tail];
    uart1_rx_tail = (uart1_rx_tail + 1) % UART_BUFFER_SIZE;
    // Add to transmit buffers
    uart1_tx_buffer[uart1_tx_head] = char_to_send;
    uart1_tx_head = (uart1_tx_head + 1) % UART_BUFFER_SIZE;
    uart2_tx_buffer[uart2_tx_head] = char_to_send;
    uart2_tx_head = (uart2_tx_head + 1) % UART_BUFFER_SIZE;
  }

  if (uart2_rx_head != uart2_rx_tail) {
    uint8_t char_to_send = uart2_rx_buffer[uart2_rx_tail];
    uart2_rx_tail = (uart2_rx_tail + 1) % UART_BUFFER_SIZE;
    // Add to transmit buffers
    uart1_tx_buffer[uart1_tx_head] = char_to_send;
    uart1_tx_head = (uart1_tx_head + 1) % UART_BUFFER_SIZE;
    uart2_tx_buffer[uart2_tx_head] = char_to_send;
    uart2_tx_head = (uart2_tx_head + 1) % UART_BUFFER_SIZE;
  }

  // Transmit from buffers if not busy
  if ((uart1_tx_head != uart1_tx_tail) && LL_USART_IsActiveFlag_TXE(USART1)) {
    uint8_t char_to_send = uart1_tx_buffer[uart1_tx_tail];
    uart1_tx_tail = (uart1_tx_tail + 1) % UART_BUFFER_SIZE;
    LL_USART_TransmitData8(USART1, char_to_send);
  }
  if ((uart2_tx_head != uart2_tx_tail) && LL_USART_IsActiveFlag_TXE(USART2)) {
    uint8_t char_to_send = uart2_tx_buffer[uart2_tx_tail];
    uart2_tx_tail = (uart2_tx_tail + 1) % UART_BUFFER_SIZE;
    LL_USART_TransmitData8(USART2, char_to_send);
  }


  // Add state machine logic here...
}
