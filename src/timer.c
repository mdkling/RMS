// timer.c
#include "../localTypes.h"


//~ typedef struct TimerMemMap {
	//~ volatile u32 timeWriteHigh;
	//~ volatile u32 timeWriteLow;
	//~ volatile u32 timeReadHigh;
	//~ volatile u32 timeReadLow;
	//~ volatile u32 alarm[4];
	//~ volatile u32 armed;
	//~ volatile u32 timeReadHighRaw;
	//~ volatile u32 timeReadLowRaw;
	//~ volatile u32 debugPause;
	//~ volatile u32 pause;
	//~ volatile u32 intr;
	//~ volatile u32 inte;
	//~ volatile u32 intf;
	//~ volatile u32 ints;
//~ } TimerMemMap;

typedef struct TimerData {
	u32 *nextProcess;
} TimerData;

typedef struct TimersInfo {
	u32       usageBitMap;
	TimerData alarm[4];
} TimersInfo;

//~ static TimersInfo timerInfo;

/*e*/void
enableWatchDogTick(void)/*p;*/
{
	u32 *watchDogTickReg = (void*)0x4005802C;
	*watchDogTickReg = 12|(1<<9);
}

/*e*/void
timerInit(void)/*p;*/
{
	TimerMemMap *timer = (void*)TIMER_BASE;
	timer->intr = 0xFF;
	timer->inte = (1<<0)|(1<<1)|(1<<2)|(1<<3);
}

/*e*/
s32 timer_set(u32 selectedTimer, u32 micoseconds)/*p;*/
{
	if (micoseconds < 10) { micoseconds = 10; }
	u32 retVal = 0;
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentTime = timer->timeReadLowRaw;
	u32 targetTime = currentTime + micoseconds;
	timer->alarm[selectedTimer] = targetTime;
	return retVal;
}

static u32 periodCount;
static u32 alarmClockValue;

/*e*/u32
setAlarmClockValue(u32 increment)/*p;*/
{
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentTime = timer->timeReadLowRaw;
	alarmClockValue = currentTime + increment;
	return alarmClockValue;
}

/*e*/
void alarm1ISR_HK(void)/*p;*/
{
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentAlarm = timer->ints;
	timer->intr = currentAlarm;
	alarmClockValue += 2500;
	timer->alarm[0] = alarmClockValue;
}

/*e*/
void alarm1ISR(void)/*p;*/
{
	// this represents the top level task and fastest running task
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentAlarm = timer->ints;
	timer->intr = currentAlarm;
	//~ alarmClockValue += 2500;
	//~ timer->alarm[0] = alarmClockValue;
	u32 nextTime = timer->timeReadLowRaw + 2500;
	timer->alarm[0] = nextTime;
	//~ io_printin(endSysTimer());
	
	
	
	//~ u32 *intSet = (void*)PPB_INTERRUPT_SET_PEND;
	//~ u32 intsToSet = 0;
	//~ u32 currentTime = timer->timeReadLowRaw;
	//~ u32 targetTime = alarmClockValue;
	//~ if (currentTime != targetTime) { io_printsn("RMS broken!"); }
	// reset timer
	
	
	// frame start of 2.5ms
	// process inputs
	uart0processInputs();
	// process outputs
	uart0processOutputs();
	// END OF FRAME
	
	// 5ms tasking
	//~ if ( (periodCount & 0x001) == 0) { intsToSet |= (1<<26); }
	// 10ms tasking
	//~ if ( (periodCount & 0x003) == 0) { intsToSet |= (1<<27); }
	// 20ms tasking
	//~ if ( (periodCount & 0x007) == 0) { intsToSet |= (1<<28); }
	// 2560ms period 0x3FF (0x007 was last I could see with eye, 20ms on/off)
	if ( (periodCount & 0x1FF) == 0) { io_ledToggle(); }
	
	//~ startSysTimer();
	//~ io_printin(endSysTimer());
	
	//~ *intSet = intsToSet;
	// increment period count
	periodCount++;
}

/*e*/
void alarm2ISR(void)/*p;*/
{
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentAlarm = timer->ints;
	timer->intr = currentAlarm;
	// reset timer
	//~ timer_set(1, 15*1000*1000);
	// frame start
	// turn on frame
	//~ u32 *intSet = (void*)PPB_INTERRUPT_SET_PEND;
	//~ *intSet = (1<<28);
}

/*e*/
void alarm3ISR(void)/*p;*/
{
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentAlarm = timer->ints;
	timer->intr = currentAlarm;
}

/*e*/
void alarm4ISR(void)/*p;*/
{
	TimerMemMap *timer = (void*)TIMER_BASE;
	u32 currentAlarm = timer->ints;
	timer->intr = currentAlarm;
}

/*
 * 1. frame int
 * 
*/

/*e*/
void SDI_1(void)/*p;*/
{
	io_printin(endSysTimer());
	u32 *intClear = (void*)PPB_INTERRUPT_CLEAR_PEND;
	*intClear = (1<<26);
}

/*e*/
void SDI_2(void)/*p;*/
{
	u32 *intClear = (void*)PPB_INTERRUPT_CLEAR_PEND;
	*intClear = (1<<27);
}

/*e*/
void SDI_3(void)/*p;*/
{
	u32 *intClear = (void*)PPB_INTERRUPT_CLEAR_PEND;
	*intClear = (1<<28);
}

//~ static u32 timerFlag;

/*e*/
void RMS_task0(u32 periodCount)/*p;*/ // period*1
{
	//~ if (timerFlag++ & 0x100) { io_printin(endSysTimer()); }
	
}

/*e*/
void RMS_task1(void)/*p;*/ // period*2
{
	
}

/*e*/
void RMS_task2(void)/*p;*/ // period*4
{
	// process outputs
	uart0processOutputs();
	// process inputs
	uart0processInputs();
}

/*e*/
void RMS_task3(void)/*p;*/  // period*8
{
	
}

//~ u32 RMS_task4_state;
/*e*/
void RMS_task4(void)/*p;*/  // period*16
{
	//~ if (RMS_task4_state++ != 0) { io_printsn("did not finish!");return; }
//~ do{
	//~ io_printsn("t4");
	//~ for (u32 i = 0; i < 4; i++)
	//~ {
		//~ bProgress();
	//~ }
	//~ RMS_task4_state--;
//~ } while (RMS_task4_state != 0);
}

/*e*/
void RMS_task5(void)/*p;*/  // period*32
{
	
}

/*e*/
void RMS_task6(void)/*p;*/  // period*64
{
	
}

/*e*/
void RMS_task7(void)/*p;*/ // period*128
{
	
}

/*e*/
void RMS_task8(void)/*p;*/ // period*256
{
	//~ io_printin(endSysTimer());
}

/*e*/
void RMS_task9(void)/*p;*/ // period*512
{
	
	
	//~ startSysTimer();
	//~ u32 *intSet = (void*)PPB_INTERRUPT_SET_PEND;
	//~ *intSet = (1<<26)|(1<<27)|(1<<28);
}

/*e*/
void RMS_task10(void)/*p;*/ // period*1024
{
	
}


/*e*/
void RMS_task11(void)/*p;*/ // period*2048
{
	
	io_ledToggle();
}


volatile u32 bCounter;

/*e*/
void timer_sleepMs(s32 ms)/*p;*/
{
	ms = ms * 2;
	while(ms > 0)
	{
		asm("wfi");
		ms -= 1;
	}
	//~ bCounter = 0;
	//~ while(bCounter < 256*1024)
	//~ {
		//~ bCounter++;
	//~ }
}

//~ /*e*/
//~ void Background(void)/*p;*/
//~ {
	//~ u32 *intClear = (void*)PPB_INTERRUPT_CLEAR_PEND;
	//~ *intClear = (1<<28);
	//~ io_prints("\nBackground Start!\n");
	//~ for (u32 i = 0; i < 20; i++)
	//~ {
		//~ bProgress();
	//~ }
	//~ io_prints("\nBackground End!\n");
//~ }
