/*
 * STM32F4-HD44780
 *
 * An interrupt driven library for HD44780-based LCD displays
 *
 * http://eartoearoak.com/software/stm32f4-hd44780
 *
 * Copyright 2013 Al Brown
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>

#include "hd44780.h"
#include "stm32f4xx_conf.h"

#include "main.h"

#define UDG	0

int main(void) {
	// Drive GPIOA15 high (to enable level-shifter IC in my implementation)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_SetBits(GPIOA, GPIO_Pin_15);

	// Initialise and set up the display to 2 line mode
	hd44780_init(GPIOD, GPIOB, GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_4,
			GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7, HD44780_LINES_2,
			HD44780_FONT_5x8);

	// Use the character generator RAM to create a custom char (smiley face)
	char udg[] = { 0x00, 0x00, 0x0a, 0x00, 0x11, 0x0e, 0x00, 0x00 };
	hd44780_cgram(UDG, udg);

	// Set position to first line and write some text
	hd44780_position(0, 1);
	hd44780_print("Hello World! ");

	// Write the custom char
	hd44780_put(UDG);

	// Activate the display
	hd44780_display(true, false, false);

	// Write to the second line
	hd44780_position(1, 0);
	hd44780_print("Line 2");

	while (1)
		;

	return (0);
}

#ifdef USE_FULL_ASSERT

void assert_failed(u8* file, u32 line) {

	(void) file;
	(void) line;

	while (1)
		;
}

#endif
