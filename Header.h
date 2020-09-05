#ifndef HEADER
#define HEADER

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f10x.h"
#include "gimbal.h"
#include "math.h"



#define UART_BUFFER_SIZE 100
#define VT_BUFFER_SIZE 100

#pragma pack(1)
typedef struct __tgtGps
{
  uint8_t sync;
  double airlat;
  double airLong;
  double tarLat;
  double tarLong;
  float tgtDist;
  float slantDist;
  float airGpsAlt;
  float airBaroAlt;
  float airAlt;
  float airHead;
  float yawAngle;
  float pitchAngle;  
  float downVelocity;
  uint8_t numSat;
  uint8_t end;
}TGTGPS;

extern uint8_t uart1RxBuffer[],uart2RxBuffer[],uart3RxBuffer[],gimbalPktBuffer[],OSDPktBuffer[];
extern char uart2RxData;
extern uint8_t uart1RxCounter,uart2RxCounter,uart3RxCounter,count; 
extern uint8_t osdBuffer[];
extern uint16_t zoom,hot;
extern uint16_t pktCounter;
extern bool OsdHeaderRecevd;

extern bool waitPeriod,angleRequested,OsdCmdReady,flag2s,sbusCmdsReady,zoomFlag;
//extern TGTGPS tgtGps;zoomFlag

void Usart1Init(void);
void Usart2Init(void);

void USART_SendBuffer(USART_TypeDef* USARTx, uint8_t *buffer,uint16_t length);


void updateGimbal(void);

void sendGimbalAngle(void);

void gimbalCtrl(void);
void zoomCtrl(void);
void DelayInit(void);
void DelayUs(uint32_t us);
void DelayMs(uint32_t ms);
void waitMs(uint32_t ms);

void timerInit(TIM_TypeDef* TIMx,unsigned int clock,unsigned int pwmfreq);
int timerChannelPwmOutConfig(TIM_TypeDef* TIMx,uint16_t channel,int pwmPulse);
void timer(TIM_TypeDef* TIMx, FunctionalState NewState);
long map(long x, long in_min, long in_max, long out_min, long out_max);


#endif
