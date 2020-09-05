#include "Header.h"

float pitchAngle = 0.0,yawAngle = 0.0,rollAngle = 0.0;
bool angleRequested = false;
bool zoomFlag = false;
uint8_t anglePkt[18];
uint8_t gimbalPktBuffer[VT_BUFFER_SIZE];
uint8_t yawCtrlBuffer[4];
uint8_t zoomCtrlBuffer[4];
uint8_t pitchCtrlBuffer[4];
bool flag=0;
uint8_t totalcount=0;
uint8_t pktCount=0;
int ctrlValue,zoomValue=370,zoomCount=0;
int zoomData=0;
int pitchValue=1500;
int yawValue=1500;
uint8_t count=0;
void retrieveAngle(void)
{ 
  uint8_t Cmd[5] = {0x3E,0x3D,0x00,0x3D,0x00};    
  USART_SendBuffer(USART1,Cmd,sizeof(Cmd));//////////////////////////
  angleRequested = true;
  waitMs(500);
  while((uart1RxCounter >= 59) && waitPeriod); 
  angleRequested = false;
  int curLoc = 0;
  totalcount = uart1RxCounter;  
  memcpy(gimbalPktBuffer,uart1RxBuffer,totalcount);
  uart1RxCounter = 0;
  while(curLoc <= totalcount)
   {
    if(gimbalPktBuffer[curLoc++] == 0x3E)
    {  
         if(gimbalPktBuffer[curLoc] == 0X3D)
         {
            if(gimbalPktBuffer[curLoc+2] == 0X73)
           {
                int32_t angle;
                angle = (int16_t)(0x00 | (gimbalPktBuffer[curLoc+28] << 24) | (gimbalPktBuffer[curLoc+27]<< 16)| (gimbalPktBuffer[curLoc+26] << 8) | (gimbalPktBuffer[curLoc+25]));
                pitchAngle = (90-(angle * 0.02197265625));
               // waitMs(500);

                angle =(int16_t) (0x00 | (gimbalPktBuffer[curLoc+46] << 24) | (gimbalPktBuffer[curLoc+45]<< 16)| (gimbalPktBuffer[curLoc+44] << 8) | (gimbalPktBuffer[curLoc+43]));
                yawAngle  = (angle * 0.02197265625);
                //waitMs(100);
           }
         }
     }
   }
}

void resetGimbal(void)
{
  uint8_t Cmd[20] = {0x3E,0x43,0x0F,0x52,0x05,0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F}; 
  USART_SendBuffer(USART1,Cmd,sizeof(Cmd));
}
void stopGimbal(void)
{
  uint8_t Cmd[20] = {0x3E,0x43,0x0F,0x52,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03}; 
  USART_SendBuffer(USART1,Cmd,sizeof(Cmd));
}
void setAngle(GIMBAL_MODE mode, float degree)
{
  int16_t angle;
  uint8_t Cmd[20] = {0x3E,0x43,0x0F,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F}; 
  
  angle = (int16_t)(degree / 0.02197265625);
  
  switch(mode)
  {
      case G_PITCH:
                    Cmd[5] = 0x05;
                    Cmd[14] = (uint8_t)((angle & 0xFF00) >> 8);
                    Cmd[13] = (uint8_t)((angle & 0x00FF));  
                    Cmd[19] = ((Cmd[5] + Cmd[13] + Cmd[14]) % 256);
                    break;
      case G_YAW:
                    Cmd[6] = 0x05;
                    Cmd[18] = (uint8_t)((angle & 0xFF00) >> 8);
                    Cmd[17] = (uint8_t)((angle & 0x00FF));  
                    Cmd[19] = ((Cmd[6] + Cmd[17] + Cmd[18]) % 256);
                    break;
      default:  
                    break;
    
  }
  USART_SendBuffer(USART1,Cmd,sizeof(Cmd));
}
void setSpeed(GIMBAL_MODE mode, uint16_t speed)
{
  uint8_t Cmd[20] = {0x3E,0x43,0x0F,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F}; 
  
  switch(mode)
  {
      case G_PITCH:
                    Cmd[5] = 0x01;
                    Cmd[12] = (uint8_t)((speed & 0xFF00) >> 8);
                    Cmd[11] = (uint8_t)((speed & 0x00FF)); 
                    Cmd[19] = ((Cmd[5] + Cmd[11] + Cmd[12]) % 256);
                    break;
      case G_YAW:
                    Cmd[6] = 0x01;
                    Cmd[10] = (uint8_t)((speed & 0xFF00) >> 8);
                    Cmd[9] = (uint8_t)((speed & 0x00FF)); 
                    Cmd[19] = ((Cmd[6] + Cmd[9] + Cmd[10]) % 256);
                    break;
      default:  
                    break;
    
  }
  USART_SendBuffer(USART1,Cmd,sizeof(Cmd));
}

void zoomIn(void)
{
  uint8_t Cmd[20] = {0xFF,0x01,0x00,0x20,0x00,0x00,0x21}; 
  USART_SendBuffer(USART1,Cmd,sizeof(Cmd));
}
void zoomOut(void)
{
  uint8_t Cmd[20] = {0x81,0x01,0x04,0x07,0x33,0xFF};
  USART_SendBuffer(USART1,Cmd,sizeof(Cmd));
}
void zoomStop(void)
{
  uint8_t Cmd[20] = {0xFF,0x01,0x00,0x00,0x00,0x00,0x01};
  USART_SendBuffer(USART1,Cmd,sizeof(Cmd));
}

void sendGimbalAngle(void)
{
  pitchAngle,yawAngle;
  sprintf(anglePkt,"%.2f,%.2f,N",pitchAngle,yawAngle);
  pktCount=strlen(anglePkt);
  USART_SendBuffer8(USART2,anglePkt,pktCount);
  myDelay(500);
  
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void gimbalCtrl()
{
  if(uart2RxBuffer[0]==0x58)
    {
      pitchValue=atoi(memmove(pitchCtrlBuffer, uart2RxBuffer+1, 4));
      timerChannelPwmOutConfig(TIM3,4,pitchValue); //channel = 3, HighPulse = 1ms,B0
      memset(pitchCtrlBuffer,0x00,4);
     }
    if(uart2RxBuffer[5]==0x59)
    {
      yawValue=atoi(memmove(yawCtrlBuffer, uart2RxBuffer+6, 4));
      timerChannelPwmOutConfig(TIM3,3,yawValue); //channel =4, HighPulse = 1ms,B1
      memset(yawCtrlBuffer,0x00,4);
    }

}
void zoomCtrl()
{
  
  if(zoomFlag==true)
  {
     if(uart2RxBuffer[10]==0x5A)
    {
     zoomData=atoi(memmove(zoomCtrlBuffer, uart2RxBuffer+11, 4));
     memset(zoomCtrlBuffer,0x00,4);
    }   
    memset(uart2RxBuffer,0x00,uart2RxCounter);   
    uart2RxCounter=0;
    zoomFlag=false;
    
    if(zoomData>1500)
    {
      zoomValue=map(zoomData,1501,2000,370,3700);
      zoomCount=map(zoomValue,370,3700,1,10);
      zoomIn();
      waitMs(zoomValue);
    }
    else if(zoomData<1500)
    {
      zoomValue=map(zoomData,1499,1000,370,3700);
      zoomCount=map(zoomData,370,3700,1,10);
      zoomOut();
      waitMs(zoomValue);    
    }
    else{
      zoomStop(); 
    }
   } 
}
