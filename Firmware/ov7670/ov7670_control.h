/**
*	==========================================================================
*	ov7670_control.h
*	(c) 2014-2015, Petr Machala
*
*	Description:
*	OV7670 camera configuration and control library.
*	Optimized for 32F429IDISCOVERY board.
*
*   License:
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*	==========================================================================
*/
 
#ifndef OV7670_CONTROL_H
#define OV7670_CONTROL_H
 
// Includes
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_dcmi.h"
#include "tm_stm32f4_ili9341.h"
//#include "stm32f4xx_hal_sram.h"
 
// SCCB write address
#define SCCB_REG_ADDR 			0x01
 
// DCMI data register
#define DCMI_DR_ADDRESS         (uint32_t) 0x50050028
 
// OV7670 camera settings
#define OV7670_REG_NUM 			122
#define OV7670_WRITE_ADDR   	0x42
 
// Image settings
#define IMG_HEIGHT   			320
#define IMG_WIDTH   			240
#define BYTES_PER_PX   			2
 
// TIM MCO (XCLK)
#define TIM_FREQ                24000000
#define TIM_PERIOD              (SystemCoreClock/TIM_FREQ)
#define TIM_DUTY_50             (TIM_PERIOD/2)
 
// Camera PWDN
#define PWDN_PIN				GPIO_Pin_4
#define PWDN_PORT				GPIOB
 
#define CAMERA_ON               GPIO_ResetBits(PWDN_PORT, PWDN_PIN)
#define CAMERA_OFF              GPIO_SetBits(PWDN_PORT, PWDN_PIN)
 
/*
* Initialize camera
*/
extern void Camera_Init(void);
 
/*
* OV7670 camera configuration
*/
extern uint8_t OV7670_Config(void);
 
#endif
