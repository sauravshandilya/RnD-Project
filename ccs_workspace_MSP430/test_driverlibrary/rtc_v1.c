// blink both led - using driverlib

//#include <msp430.h>
#include <stdio.h>
#include <driverlib.h>
#include "rtc_a.h"

/*
 * main.c
 */
volatile Calendar newTime;

void main(void)
{
	Calendar currentTime;

	WDT_A_hold(WDT_A_BASE);			//stop watchdog timer

	//Setup Current Time for Calendar
	currentTime.Seconds = 		0;
	currentTime.Minutes = 		51;
	currentTime.Hours = 		1;
	currentTime.DayOfWeek = 	6;
	currentTime.DayOfMonth = 	28;
	currentTime.Month = 		3;
	currentTime.Year = 			2017;

	// Initialize Calendar Mode of RTC
	RTC_A_initCalendar(RTC_A_BASE, &currentTime, RTC_A_FORMAT_BCD);
//	RTC_A_calendarInit ( RTC_A_BASE, currentTime, RTC_A_FORMAT_BINARY);
	// Setup Calendar Alarm for 5:00pm on the 5th day of the week.
	// Note: Does not specify day of the week.
//	RTC_A_setCalendarAlarm ( RTC_A_BASE,0x00,0x17, RTC_A_ALARMCONDITION_OFF,0x05);

    //Setup Calendar Alarm
    //Note: Does not specify day of the month - choose either month or week
    RTC_A_configureCalendarAlarmParam param = {0};
    param.minutesAlarm = 		20;
    param.hoursAlarm =			13;
    param.dayOfWeekAlarm = 		6;
    param.dayOfMonthAlarm = 	RTC_A_ALARMCONDITION_OFF;

    RTC_A_configureCalendarAlarm(RTC_A_BASE, &param);

	// Specify an interrupt to assert every minute
	RTC_A_setCalendarEvent (RTC_A_BASE, RTC_A_CALENDAREVENT_MINUTECHANGE);

	// Clear interrupt bits before starting RTC
//	RTC_A_clearInterrupt ( RTC_A_BASE, RTC_A_CLOCK_READ_READY_INTERRUPT + RTC_A);
	// Enable interrupt for RTC Ready Status, that let’s us know RTC registers are ready to read.
	// Also, enable interrupts for the Calendar alarm & event.
//	RTC_A_enableInterrupt ( RTC_A_BASE, RTC_A_CLOCK_READ_READY_INTERRUPT + RTC_A);

	RTC_A_clearInterrupt(RTC_A_BASE, RTCRDYIFG + RTCTEVIFG + RTCAIFG);
	RTC_A_enableInterrupt(RTC_A_BASE, RTCRDYIE + RTCTEVIE + RTCAIE);

	// Start RTC Clock
	RTC_A_startClock ( RTC_A_BASE);

    // Read out the time and print
    newTime = RTC_A_getCalendarTime(RTC_A_BASE);
    printf("Current Time...\n");
    printf("Year         = %d\n", newTime.Year);
    printf("Month        = %d\n", newTime.Month);
    printf("Day of Month = %d\n", newTime.DayOfMonth);
    printf("Day of Week  = %d\n", newTime.DayOfWeek);
    printf("Hour         = %d\n", newTime.Hours);
    printf("Minutes =      %d\n", newTime.Minutes);
    printf("Second       = %d\n", newTime.Seconds);

	//Enter LPM3 mode with interrupts enabled
	__bis_SR_register(LPM3_bits + GIE);
	__no_operation();
}
