#ifndef GIMBAL
#define GIMBAL

typedef enum  
{
  G_PITCH = 0,
  G_YAW,
  G_ROLL
}GIMBAL_MODE;

extern bool angleRequested;
extern float pitchAngle,yawAngle,rollAngle;

void setAngle(GIMBAL_MODE mode, float degree);
void setSpeed(GIMBAL_MODE mode, uint16_t speed);
void resetGimbal(void);
void stopGimbal(void);
void retrieveAngle(void);
#endif