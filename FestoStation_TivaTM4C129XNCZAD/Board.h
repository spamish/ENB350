/*
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DK_TM4C129X.h"

#define Board_initEMAC              DK_TM4C129X_initEMAC
#define Board_initGeneral           DK_TM4C129X_initGeneral
#define Board_initGPIO              DK_TM4C129X_initGPIO
#define Board_initI2C               DK_TM4C129X_initI2C
#define Board_initPWM               DK_TM4C129X_initPWM
#define Board_initSDSPI             DK_TM4C129X_initSDSPI
#define Board_initSPI               DK_TM4C129X_initSPI
#define Board_initUART              DK_TM4C129X_initUART
#define Board_initUSB               DK_TM4C129X_initUSB
#define Board_initUSBMSCHFatFs      DK_TM4C129X_initUSBMSCHFatFs
#define Board_initWatchdog          DK_TM4C129X_initWatchdog
#define Board_initWiFi              DK_TM4C129X_initWiFi

#define Board_LED_ON                DK_TM4C129X_LED_ON
#define Board_LED_OFF               DK_TM4C129X_LED_OFF
#define Board_LED0                  DK_TM4C129X_LED_G
#define Board_LED1                  DK_TM4C129X_LED_B
#define Board_LED2                  DK_TM4C129X_LED_R
#define Board_BUTTON0               DK_TM4C129X_BUTTON_SELECT
#define Board_BUTTON1               DK_TM4C129X_BUTTON_UP
#define Board_BUTTON2               DK_TM4C129X_BUTTON_DOWN

/* FestoStation block start */
#define Board_ACTUATOR_RISER_DOWN	DK_TM4C129X_PORTL_1		//OUT 0 L1
#define Board_ACTUATOR_RISER_UP		DK_TM4C129X_PORTL_0		//OUT 1 L0
#define Board_ACTUATOR_EJECTOR		DK_TM4C129X_PORTL_2		//OUT 2 L2
#define Board_ACTUATOR_MEASURE_DOWN	DK_TM4C129X_PORTL_3		//OUT 3 L3
#define Board_DO_4					DK_TM4C129X_PORTL_4		//OUT 4 L4
#define Board_DO_5					DK_TM4C129X_PORTL_5		//OUT 5 L5
#define Board_DO_6					DK_TM4C129X_PORTP_5		//OUT 6 P5
#define Board_DO_7					DK_TM4C129X_PORTP_4		//OUT 7 P4

#define Board_SENSE_SAMPLE_METALLIC	DK_TM4C129X_PORTM_3		//IN  0 M3
#define Board_SENSE_SAMPLE_IN_PLACE	DK_TM4C129X_PORTM_2		//IN  1 M2
#define Board_SENSE_SAMPLE_COLOUR	DK_TM4C129X_PORTM_1		//IN  2 M1
#define Board_SENSE_RISER_DOWN		DK_TM4C129X_PORTM_0		//IN  3 M0
#define Board_SENSE_RISER_UP		DK_TM4C129X_PORTN_4		//IN  4 N4
#define Board_SENSE_EJECTOR_READY	DK_TM4C129X_PORTA_7		//IN  5 A7
#define Board_SENSE_MEASURE_DOWN	DK_TM4C129X_PORTC_6		//IN  6 C6
#define Board_DI_7					DK_TM4C129X_PORTC_5		//IN  7 C5
// #define Board_ANALOG_HEIGHT		DK_TM4129X_PORTX_X		//ADC 0 XX
/* FestoStation block end */

#define Board_I2C0                  DK_TM4C129X_I2C3
#define Board_I2C_TMP               DK_TM4C129X_I2C3
#define Board_I2C_NFC               DK_TM4C129X_I2C3
#define Board_I2C_TPL0401           DK_TM4C129X_I2C3

#define Board_PWM0                  DK_TM4C129X_PWM1
#define Board_PWM1                  DK_TM4C129X_PWM6

#define Board_SDSPI0                DK_TM4C129X_SDSPI0

#define Board_SPI0                  DK_TM4C129X_SPI3
#define Board_SPI1                  DK_TM4C129X_SPI2
#define Board_SPI_CC3100            DK_TM4C129X_SPI2

#define Board_USBMSCHFatFs0         DK_TM4C129X_USBMSCHFatFs0

#define Board_USBHOST               DK_TM4C129X_USBHOST
#define Board_USBDEVICE             DK_TM4C129X_USBDEVICE

#define Board_UART0                 DK_TM4C129X_UART0

#define Board_WATCHDOG0             DK_TM4C129X_WATCHDOG0

#define Board_WIFI                  DK_TM4C129X_WIFI

#define Board_gpioCallbacks0        DK_TM4C129X_gpioPortPCallbacks
#define Board_gpioCallbacks1        DK_TM4C129X_gpioPortNCallbacks

/* Board specific I2C addresses */
#define Board_TMP006_ADDR           (0x40)
#define Board_RF430CL330_ADDR       (0x28)
#define Board_TPL0401_ADDR          (0x40)

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */