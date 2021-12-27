/**
 * \file
 *
 * \brief User board configuration template
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H


/************************************************************************/
/* I2C MODULE                                                           */
/************************************************************************/
#define I2C_MODULE							SERCOM1
#define I2C_PINMUX_PAD0						PINMUX_PA16C_SERCOM1_PAD0
#define I2C_PINMUX_PAD1						PINMUX_PA17C_SERCOM1_PAD1

/************************************************************************/
/* LIN MODULE                                                           */
/************************************************************************/
#define LIN_MODULE							SERCOM0
#define LIN_MUXSET							USART_RX_1_TX_0_XCK_1
#define LIN_PINMUX_PAD0						PINMUX_PA04D_SERCOM0_PAD0
#define LIN_PINMUX_PAD1						PINMUX_PA05D_SERCOM0_PAD1
#define LIN_PIN_TX							PIN_PA04
#define LIN_PIN_RX							PIN_PA05

/************************************************************************/
/* EXTERNAL INTERRUPT PIN SETTINGS                                      */
/************************************************************************/
#define IRQ_PIN_KEYPAD						PIN_PA18A_EIC_EXTINT2
#define IRQ_MUX_KEYPAD						PINMUX_PA18A_EIC_EXTINT2
#define IRQ_EIC_KEYPAD						2		// Ext. Int. Controller Line

/************************************************************************/
/* AUDIO CONTROL                                                        */
/************************************************************************/
#define AUDIO_WAVE_GENERATOR_PIN			PIN_PA08E_TCC0_WO0
#define AUDIO_WAVE_GENERATOR_PINMUX			PINMUX_PA08E_TCC0_WO0

/************************************************************************/
/* GPIO PINS                                                            */
/************************************************************************/
#define GPIO_PIN_LED						PIN_PA10
#define GPIO_PIN_SAFETY						PIN_PA22


#endif // CONF_BOARD_H
