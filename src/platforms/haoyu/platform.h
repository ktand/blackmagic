/*
 * This file is part of the Black Magic Debug project.
 *
 * Copyright (C) 2011  Black Sphere Technologies Ltd.
 * Written by Gareth McMullin <gareth@blacksphere.co.nz>
 * Patching for Haoyu board by Dave Marples <dave@marples.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* This file implements the platform specific functions for the Hapyu
 * implementation.
 */
#ifndef __PLATFORM_H
#define __PLATFORM_H

#include "gpio.h"
#include "timing.h"
#include "timing_stm32.h"
#include "version.h"

#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/f1/memorymap.h>
#include <libopencm3/usb/usbd.h>

#define BOARD_IDENT       "Black Magic Probe (Haoyu), (Firmware " FIRMWARE_VERSION ")"
#define BOARD_IDENT_DFU   "Black Magic for Haoyu, (Firmware " FIRMWARE_VERSION ")"
#define BOARD_IDENT_UPD   "Black Magic (DFU Upgrade) for Haoyu, (Firmware " FIRMWARE_VERSION ")"
#define DFU_IDENT         "Black Magic Firmware Upgrade (Haoyu) (Firmware " FIRMWARE_VERSION ")"
#define UPD_IFACE_STRING  "@Internal Flash   /0x08000000/8*001Kg"

/* Important pin mappings for STM32 Haoyu implementation:
 *
 * LED0 = 	PB12	(Yellow LED : Running)
 *
 * These are fixed by the implementation of the hardware
 * SRST_OUT = 	PA2
 * TDI = 	PB0
 * TMS = 	PA4 (input for SWDP)
 * TCK = 	PA3 
 * TDO = 	PA6 (input)
 * nSRST = 	PA2 (input)
 *
 * Serial port;
 * PB6
 * PB7
 *
 * USB cable pull-up: PA9
 */
#define PLATFORM_HAS_TRACESWO 1
#define TRACESWO_ASYNC 1

/* Hardware definitions... */
#define TRST_PORT GPIOB
#define TRST_PIN  GPIO1

#define TDI_PORT	GPIOB
#define TMS_PORT	GPIOA
#define TCK_PORT	GPIOA
#define TDO_PORT	GPIOA
#define TDI_PIN		GPIO0
#define TMS_PIN		GPIO4
#define TCK_PIN		GPIO3
#define TDO_PIN		GPIO6

#define SWDIO_PORT 	TMS_PORT
#define SWCLK_PORT 	TCK_PORT
#define SWDIO_PIN	TMS_PIN
#define SWCLK_PIN	TCK_PIN

#define SRST_PORT	GPIOA
#define SRST_PIN	GPIO2

#define USB_PU_PORT     GPIOA
#define USB_PU_PIN      GPIO9

#define LED_PORT	GPIOB
#define LED_IDLE_RUN    GPIO12

/* Use PC14 for a "dummy" uart led. So we can observere at least with scope*/
#define LED_PORT_UART	GPIOC
#define LED_UART	GPIO14

#define TMS_SET_MODE() \
	gpio_set_mode(TMS_PORT, GPIO_MODE_OUTPUT_50_MHZ, \
	              GPIO_CNF_OUTPUT_PUSHPULL, TMS_PIN);
#define SWDIO_MODE_FLOAT() \
	gpio_set_mode(SWDIO_PORT, GPIO_MODE_INPUT, \
	              GPIO_CNF_INPUT_FLOAT, SWDIO_PIN);
#define SWDIO_MODE_DRIVE() \
	gpio_set_mode(SWDIO_PORT, GPIO_MODE_OUTPUT_50_MHZ, \
	              GPIO_CNF_OUTPUT_PUSHPULL, SWDIO_PIN);

#define UART_PIN_SETUP() \
	gpio_set_mode(USBUSART_PORT, GPIO_MODE_OUTPUT_2_MHZ, \
	              GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, USBUSART_TX_PIN);

#define SWO_PIN_SETUP()						     \
        AFIO_MAPR |= AFIO_MAPR_USART1_REMAP;     \
        gpio_set_mode(SWOUSART_PORT, GPIO_MODE_INPUT, \
        GPIO_CNF_INPUT_FLOAT, SWOUSART_RX_PIN);

#define USB_DRIVER      stm32f103_usb_driver
#define USB_IRQ	        NVIC_USB_LP_CAN_RX0_IRQ
#define USB_ISR	        usb_lp_can_rx0_isr
/* Interrupt priorities.  Low numbers are high priority. */

#define IRQ_PRI_SWODMA          (1 << 4)
#define IRQ_PRI_USB		(2 << 4)
#define IRQ_PRI_USBUSART	(1 << 4)
#define IRQ_PRI_USBUSART_TIM	(3 << 4)
#define IRQ_PRI_USB_VBUS	(14 << 4)
#define IRQ_PRI_TIM3		(0 << 4)

/* Note that SWO needs to be on USART1 RX to get maximum speed */
#define SWOUSART                USART1
#define SWOUSARTDR              USART1_DR
#define SWOUSART_CR1            USART1_CR1
#define SWOUSART_IRQ            NVIC_USART1_IRQ
#define SWOUSART_CLK            RCC_USART1
#define SWOUSART_PORT           GPIOB
#define SWOUSART_RX_PIN         GPIO7
#define SWOUSART_ISR            usart1_isr

/* This DMA channel is set by the USART in use */
#define SWODMABUS               DMA1
#define SWDDMACHAN              DMA_CHANNEL5
#define SWODMAIRQ               NVIC_DMA1_CHANNEL5_IRQ

#define USBUSART USART1
#define USBUSART_CR1 USART1_CR1
#define USBUSART_IRQ NVIC_USART1_IRQ
#define USBUSART_CLK RCC_USART1
#define USBUSART_PORT GPIOB
#define USBUSART_TX_PIN GPIO6
#define USBUSART_ISR usart1_isr
#define USBUSART_TIM TIM4
#define USBUSART_TIM_CLK_EN() rcc_periph_clock_enable(RCC_TIM4)
#define USBUSART_TIM_IRQ NVIC_TIM4_IRQ
#define USBUSART_TIM_ISR tim4_isr


#define DEBUG(...)

#define SET_RUN_STATE(state)	{running_status = (state);}
#define SET_IDLE_STATE(state)	{gpio_set_val(LED_PORT, LED_IDLE_RUN, state);}
#define SET_ERROR_STATE(x)

/* Use newlib provided integer only stdio functions */
#define sscanf siscanf
#define sprintf siprintf
#define vasprintf vasiprintf
#define snprintf sniprintf

#endif

