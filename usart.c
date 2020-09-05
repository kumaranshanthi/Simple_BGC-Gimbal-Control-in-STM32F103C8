#include "Header.h"

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;

uint8_t uart1RxBuffer[UART_BUFFER_SIZE];
uint8_t uart1RxCounter = 0x00; 
uint8_t uart2RxBuffer[15];
char uart2RxData;
uint8_t uart2RxCounter = 0x00; 

void USART_SendBuffer(USART_TypeDef* USARTx, uint8_t *buffer,uint16_t length);
 
void Usart1Init(void)
{
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = (USART_Mode_Tx | USART_Mode_Rx);

  /* Configure USART */
  USART_Init(USART1, &USART_InitStructure);
  
  /* Enable USART Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  /* Enable the USART */
  USART_Cmd(USART1, ENABLE);
}
void USART_SendBuffer(USART_TypeDef* USARTx, uint8_t *buffer,uint16_t length)
{
  int i = 0;
  while(i < length)
  {
    USART_SendData(USARTx, (uint16_t)buffer[i]);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    i++;
  }
}
 void USART_SendBuffer8(USART_TypeDef* USARTx, char *buffer,uint16_t length)
{
  int i = 0;
   char data = 0;
  while(i < length)
  {
    data = buffer[i];
    USART_SendData(USARTx, data);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);   
    i++;
  }
}

void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    if(angleRequested==1)
    {
      uart1RxBuffer[uart1RxCounter++] = USART_ReceiveData(USART1);
    }
    else
      uart1RxBuffer[uart1RxCounter++] = USART_ReceiveData(USART1);
     // USART_ReceiveData(USART1);
   
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
}

void Usart2Init(void)
{
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx ;

  /* Configure USART */
  USART_Init(USART2, &USART_InitStructure);
  
  /* Enable USART Receive and Transmit interrupts */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  /* Enable the USART */
  USART_Cmd(USART2, ENABLE);
}

void USART2_IRQHandler(void)
{
  static bool zoomData = false;
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
   // uart2RxBuffer[uart2RxCounter++] = USART_ReceiveData(USART2);
    /* Read one byte from the receive data register */
    
   if(zoomData)
    {
      uart2RxBuffer[uart2RxCounter++] = USART_ReceiveData(USART2);
    }
    else
    {
      uart2RxBuffer[0] = USART_ReceiveData(USART2);
      if(uart2RxBuffer[0] == 0x58)
      {
        zoomData = true;
        uart2RxCounter = 1;
      }
    }
   
    if(uart2RxCounter >= 15)
    {
      zoomFlag = true;
      zoomData = false;
    } 

    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }
}

void HardFault_Handler()
{
  NVIC_SystemReset();
}


