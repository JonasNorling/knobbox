EESchema Schematic File Version 2  date Fri 06 Apr 2012 09:44:16 PM CEST
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:bourns-encoder
LIBS:stm32
LIBS:knobboard-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 3
Title ""
Date "6 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 2100 3400 2000 1900
U 4F7EF61F
F0 "stm32" 60
F1 "stm32.sch" 60
$EndSheet
$Sheet
S 4300 3400 1550 1350
U 4F7E05A5
F0 "encoders" 60
F1 "encoders.sch" 60
F2 "BUF_OE" I L 4300 3650 60 
F3 "BUF_CLK" I L 4300 3750 60 
F4 "SW4" O R 5850 3950 60 
F5 "SW3" O R 5850 3850 60 
F6 "SW2" O R 5850 3750 60 
F7 "SW1" O R 5850 3650 60 
F8 "SHIFT_LATCH" I L 4300 3950 60 
F9 "SHIFT_OE" I L 4300 4050 60 
F10 "SHIFT_CLK" I L 4300 4150 60 
F11 "SHIFT_OUT" O R 5850 4550 60 
F12 "SHIFT_IN" I L 4300 4550 60 
$EndSheet
Text Notes 6750 3800 0    60   ~ 0
STM32\n3V3 vreg\nMIDI I/O -- needs 5V\nmicro-USB\npins for USART1 programming\nencoders -- 2 LEDs, button, gray output\n   -- needs filter circuit 4R+2C?\n   -- blue and green LEDs have Vf up to 4V\n   -- so I need a level-changing buffer to drive the anode at 5V\n   -- use a latch instead of buffer, so the LEDs can be kept on all the time\nLCD COG or OLED display, on SPI\n2 buttons\nunpopulated CV output circuitry?\nSPI EEPROM 8mbit
$EndSCHEMATC
