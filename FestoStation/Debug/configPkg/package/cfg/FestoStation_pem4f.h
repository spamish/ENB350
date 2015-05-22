/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-A65
 */

#include <xdc/std.h>

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle LCD;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle FESTO;

#include <ti/sysbios/knl/Task.h>
extern const ti_sysbios_knl_Task_Handle ADC;

#include <ti/sysbios/knl/Event.h>
extern const ti_sysbios_knl_Event_Handle FestoEvents;

#include <ti/sysbios/knl/Event.h>
extern const ti_sysbios_knl_Event_Handle DisplayEvents;

#include <ti/sysbios/knl/Mailbox.h>
extern const ti_sysbios_knl_Mailbox_Handle DisplayMailbox;

#include <ti/sysbios/knl/Mailbox.h>
extern const ti_sysbios_knl_Mailbox_Handle ADCMailbox;

#include <ti/sysbios/knl/Clock.h>
extern const ti_sysbios_knl_Clock_Handle Clock_1_sec;

#define TI_DRIVERS_WIFI_INCLUDED 0

extern int xdc_runtime_Startup__EXECFXN__C;

extern int xdc_runtime_Startup__RESETFXN__C;

