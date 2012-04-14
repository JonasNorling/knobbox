EESchema Schematic File Version 2  date Sat 14 Apr 2012 07:12:50 PM CEST
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
LIBS:spi-flash
LIBS:nhd-lcd
LIBS:r-network
LIBS:lm1117
LIBS:74hc
LIBS:dogs102-6
LIBS:pdt-transistor
LIBS:tact
LIBS:knobboard-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 1 5
Title ""
Date "14 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	8800 2600 5600 2600
Wire Wire Line
	8200 1900 8800 1900
Wire Wire Line
	8200 2100 8800 2100
Wire Wire Line
	3000 2300 3600 2300
Wire Wire Line
	3000 2000 3600 2000
Wire Wire Line
	3000 1700 3600 1700
Wire Wire Line
	5600 1900 6700 1900
Wire Wire Line
	5600 1700 6700 1700
Wire Wire Line
	5600 1800 6700 1800
Wire Wire Line
	5600 2200 5700 2200
Wire Wire Line
	5700 2200 5700 2100
Wire Wire Line
	5700 2100 6700 2100
Wire Wire Line
	3600 1800 3000 1800
Wire Wire Line
	3600 2100 3000 2100
Wire Wire Line
	3600 2400 3000 2400
Wire Wire Line
	5600 2700 8600 2700
Wire Wire Line
	8200 2000 8800 2000
Wire Wire Line
	8200 1800 8800 1800
$Sheet
S 8800 1550 1400 3300
U 4F81A858
F0 "switchbus" 60
F1 "switchbus.sch" 60
F2 "OE1" I L 8800 2600 60 
F3 "SW16" I L 8800 4600 60 
F4 "SW15" I L 8800 4500 60 
F5 "SW14" I L 8800 4400 60 
F6 "SW13" I L 8800 4300 60 
F7 "SW12" I L 8800 4050 60 
F8 "SW11" I L 8800 3950 60 
F9 "SW10" I L 8800 3850 60 
F10 "SW9" I L 8800 3750 60 
F11 "SW8" I L 8800 3450 60 
F12 "SW7" I L 8800 3350 60 
F13 "SW6" I L 8800 3300 60 
F14 "SW5" I L 8800 3150 60 
F15 "SW4" I L 8800 2100 60 
F16 "SW3" I L 8800 2000 60 
F17 "SW2" I L 8800 1900 60 
F18 "SW1" I L 8800 1800 60 
$EndSheet
$Sheet
S 1500 1550 1500 1900
U 4F80537C
F0 "io" 60
F1 "io.sch" 60
F2 "MIDI_RX" O R 3000 1800 60 
F3 "MIDI_TX" I R 3000 1700 60 
F4 "PGM_TX" I R 3000 2400 60 
F5 "PGM_RX" O R 3000 2300 60 
F6 "USB_DM" B R 3000 2100 60 
F7 "USB_DP" B R 3000 2000 60 
$EndSheet
$Sheet
S 3600 1550 2000 1900
U 4F7EF61F
F0 "stm32" 60
F1 "stm32.sch" 60
F2 "LED_LATCH" O R 5600 1700 60 
F3 "LED_DATA" O R 5600 1900 60 
F4 "LED_CLK" O R 5600 1800 60 
F5 "BUF_OE6" O R 5600 2700 60 
F6 "BUF_OE5" O R 5600 2600 60 
F7 "BUF_OE4" O R 5600 2500 60 
F8 "BUF_OE3" O R 5600 2400 60 
F9 "BUF_OE2" O R 5600 2300 60 
F10 "BUF_OE1" O R 5600 2200 60 
F11 "MIDI_TX" O L 3600 1700 60 
F12 "MIDI_RX" I L 3600 1800 60 
F13 "USB_DP" B L 3600 2000 60 
F14 "USB_DM" B L 3600 2100 60 
F15 "PGM_RX" I L 3600 2300 60 
F16 "PGM_TX" O L 3600 2400 60 
$EndSheet
$Sheet
S 6700 1550 1500 900 
U 4F7E05A5
F0 "encoders" 60
F1 "encoders.sch" 60
F2 "BUF_OE" I L 6700 2100 60 
F3 "SW4" O R 8200 2100 60 
F4 "SW3" O R 8200 2000 60 
F5 "SW2" O R 8200 1900 60 
F6 "SW1" O R 8200 1800 60 
F7 "SHIFT_LATCH" I L 6700 1700 60 
F8 "SHIFT_CLK" I L 6700 1800 60 
F9 "SHIFT_OUT" O R 8200 2300 60 
F10 "SHIFT_IN" I L 6700 1900 60 
$EndSheet
Text Notes 4300 4450 0    60   ~ 0
STM32\n3V3 vreg\nMIDI I/O -- needs 5V\nmicro-USB\npins for USART1 programming\nencoders -- 2 LEDs, button, gray output\n   -- needs filter circuit 4R+2C?\n   -- blue and green LEDs have Vf up to 4V\n   -- so I need a level-changing buffer to drive the anode at 5V\n   -- use a latch instead of buffer, so the LEDs can be kept on all the time\nLCD COG or OLED display, on SPI\n2 buttons, at least\n   -- mode/power, ok, plypause, back\nunpopulated CV output circuitry?\nSPI EEPROM 8mbit\n"soft" power button -- keep idle power at uA levels\nSPI LCD display NHD-C12832A1Z-FSW-FBW-3V3 (Newhaven Display, 8.68€ @ Mouser)
$EndSCHEMATC
