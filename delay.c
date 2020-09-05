
#include "Header.h"

// For store tick counts in us
static volatile uint32_t usTick;
bool waitPeriod = false;
// SysTick_Handler function will be called every 1 us
void SysTick_Handler()
{
	if(usTick > 0)
	  usTick--;
        else
          usTick = 0;
}

void DelayInit()
{
	// Configure the SysTick timer to overflow every 1 us
	SysTick_Config(SystemCoreClock / 1000000);
}

void DelayUs(uint32_t us)
{
	// Reload us value
	usTick = us;
	// Wait until usTick reach zero
	while(usTick);
}

void DelayMs(uint32_t ms)
{
	// Wait until ms reach zero
	while (ms--)
	{
		// Delay 1ms
		DelayUs(1000);
	}
}

void waitMs(uint32_t ms)
{
        waitPeriod = true;
	// Wait until ms reach zero
	while (ms--)
	{
		// Delay 1ms
		DelayUs(1000);
	}
        waitPeriod = false;
}

/********************************* END OF FILE ********************************/
/******************************************************************************/