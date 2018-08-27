/*
 * platform.h
 *
 *  Created on: 27 Aug 2018
 *      Author: kevin
 */

#ifndef MAIN_TARGET_ARCH_STM32F1_PLATFORM_H_
#define MAIN_TARGET_ARCH_STM32F1_PLATFORM_H_

#define IRAM_ATTR

#define USE_RX_SUPPORT

#define USE_SCREEN


// User button
#define PIN_BUTTON_1 0

// Buzzer
#define PIN_BEEPER 12 // This seems appropriate for TTGO v1 boards, since 12 can't be pulled low during boot and it's not connected to anything

// SX127X connection
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's DIO0

#define PIN_SX127X_SCK 5
#define PIN_SX127X_MISO 19
#define PIN_SX127X_MOSI 27
#define PIN_SX127X_CS 18
#define PIN_SX127X_RST 14
#define PIN_SX127X_DIO0 26

// All v1 boards use PA_BOOST
#define SX127X_OUTPUT_TYPE SX127X_OUTPUT_PA_BOOST

// 0 is wired to button
// 2 needs to be left unconnected for flashing
// 4 is OLED SDA (boards with OLED)
// 5 is SX127X SCK
// 12 can't be pulled low during boot, otherwise we
// boot in VCC = 1.8V mode. See https://github.com/espressif/esptool/wiki/ESP32-Boot-Mode-Selection
// 14 is SX127X RST
// 15 is OLED SCL (boards with OLED)
// 16 is OLED RST (boards with OLED)
// 18 is SX127X CS
// 19 is SX127X MOSI
// 25 is LED (in some boards)
// 26 is SX127X IRQ
// 27 is SX127X MOSI
// 34..39 is input only
#define PIN_USABLE_BASE_MASK (PIN_N(1) | PIN_N(3) | PIN_N(4) | PIN_N(13) | PIN_N(15) | PIN_N(16) | PIN_N(17) | PIN_N(21) | PIN_N(22) | PIN_N(23) | PIN_N(32) | PIN_N(33))

#define PIN_DEFAULT_TX 13
#define PIN_DEFAULT_RX 21

#define PIN_UNUSED_TX 2
#define PIN_UNUSED_RX 35

#define USE_AIR_BAND_868
#define USE_AIR_BAND_915
#define CONFIG_AIR_BAND_DEFAULT CONFIG_AIR_BAND_868

// OLED screen connections
#define PIN_SCREEN_SDA 4
#define PIN_SCREEN_SCL 15
#define PIN_SCREEN_RST 16
#define SCREEN_I2C_ADDR 0x3c

#define PIN_LED_1 25
#define PIN_USABLE_MASK (PIN_USABLE_BASE_MASK & ~(PIN_N(PIN_SCREEN_SDA) | PIN_N(PIN_SCREEN_SCL) | PIN_N(PIN_SCREEN_RST)))
#define BOARD_NAME "R9 Mini"

#define PIN_MAX 63
#define PIN_FULL_MASK 0xFFFFFFFFFFFFFFFFll
#define PIN_N(n) ((uint64_t)1 << n)


#endif /* MAIN_TARGET_ARCH_STM32F1_PLATFORM_H_ */
