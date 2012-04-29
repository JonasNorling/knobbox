EESchema Schematic File Version 2  date Sun 29 Apr 2012 10:27:51 AM CEST
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
Date "29 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	6250 2900 6250 1150
Wire Wire Line
	6350 1800 6350 1250
Wire Wire Line
	8650 2950 8650 2750
Wire Wire Line
	8650 2750 8800 2750
Wire Wire Line
	5600 2500 5900 2500
Connection ~ 6450 1900
Wire Wire Line
	8800 1350 6450 1350
Wire Wire Line
	6250 1150 8800 1150
Wire Wire Line
	6700 2200 5600 2200
Wire Wire Line
	6700 1900 5600 1900
Wire Wire Line
	6700 1700 5600 1700
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
	3600 1800 3000 1800
Wire Wire Line
	3600 2100 3000 2100
Wire Wire Line
	3600 2400 3000 2400
Wire Wire Line
	8200 2000 8800 2000
Wire Wire Line
	8200 1800 8800 1800
Wire Wire Line
	6700 2300 5600 2300
Wire Wire Line
	5600 1800 6700 1800
Wire Wire Line
	5600 2100 6700 2100
Connection ~ 6250 1700
Wire Wire Line
	6350 1250 8800 1250
Connection ~ 6350 1800
Wire Wire Line
	8200 2250 8600 2250
Wire Wire Line
	8600 2250 8600 2550
Wire Wire Line
	8600 2550 5900 2550
Wire Wire Line
	5900 2550 5900 2500
Wire Wire Line
	6150 800  10350 800 
Wire Wire Line
	6150 800  6150 2000
Wire Wire Line
	6150 2000 6700 2000
Wire Wire Line
	10350 800  10350 2500
Wire Wire Line
	10350 2500 10200 2500
Wire Wire Line
	8200 2350 8350 2350
Wire Wire Line
	6450 1350 6450 1900
NoConn ~ 8350 2350
$Sheet
S 8800 1000 1400 2000
U 4F81A858
F0 "switchbus" 60
F1 "switchbus.sch" 60
F2 "SW4" I L 8800 2100 60 
F3 "SW3" I L 8800 2000 60 
F4 "SW2" I L 8800 1900 60 
F5 "SW1" I L 8800 1800 60 
F6 "BTNSHIFT_OUT0" O R 10200 2500 60 
F7 "BTN_CE" I L 8800 1350 60 
F8 "BTN_LOAD" I L 8800 1250 60 
F9 "SHIFT_CLK" I L 8800 1150 60 
F10 "BTNSHIFT_IN0" I L 8800 2750 60 
$EndSheet
Text Notes 2500 6600 1    60   ~ 0
Mounting holes
$Comp
L CONN_1 H6
U 1 1 4F90F604
P 2200 6400
F 0 "H6" H 2280 6400 40  0000 L CNN
F 1 "CONN_1" H 2200 6455 30  0001 C CNN
	1    2200 6400
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 H5
U 1 1 4F90F602
P 2200 6300
F 0 "H5" H 2280 6300 40  0000 L CNN
F 1 "CONN_1" H 2200 6355 30  0001 C CNN
	1    2200 6300
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 H4
U 1 1 4F90F601
P 2200 6200
F 0 "H4" H 2280 6200 40  0000 L CNN
F 1 "CONN_1" H 2200 6255 30  0001 C CNN
	1    2200 6200
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 H3
U 1 1 4F90F5FF
P 2200 6100
F 0 "H3" H 2280 6100 40  0000 L CNN
F 1 "CONN_1" H 2200 6155 30  0001 C CNN
	1    2200 6100
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 H2
U 1 1 4F90F5FD
P 2200 6000
F 0 "H2" H 2280 6000 40  0000 L CNN
F 1 "CONN_1" H 2200 6055 30  0001 C CNN
	1    2200 6000
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 H1
U 1 1 4F90F5D6
P 2200 5900
F 0 "H1" H 2280 5900 40  0000 L CNN
F 1 "CONN_1" H 2200 5955 30  0001 C CNN
	1    2200 5900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 4F8DA5D4
P 8650 2950
F 0 "#PWR01" H 8650 2950 30  0001 C CNN
F 1 "GND" H 8650 2880 30  0001 C CNN
	1    8650 2950
	1    0    0    -1  
$EndComp
Text Notes 6150 2850 3    60   ~ 0
Place at end of line
$Comp
L GND #PWR02
U 1 1 4F8DA3E0
P 6250 3400
F 0 "#PWR02" H 6250 3400 30  0001 C CNN
F 1 "GND" H 6250 3330 30  0001 C CNN
	1    6250 3400
	1    0    0    -1  
$EndComp
$Comp
L R R18
U 1 1 4F8DA3D4
P 6250 3150
F 0 "R18" V 6330 3150 50  0000 C CNN
F 1 "DNP" V 6250 3150 50  0000 C CNN
	1    6250 3150
	1    0    0    -1  
$EndComp
$Sheet
S 6700 1550 1500 900 
U 4F7E05A5
F0 "encoders" 60
F1 "encoders.sch" 60
F2 "SW4" O R 8200 2100 60 
F3 "SW3" O R 8200 2000 60 
F4 "SW2" O R 8200 1900 60 
F5 "SW1" O R 8200 1800 60 
F6 "SHIFT_CLK" I L 6700 1700 60 
F7 "BTN_LOAD" I L 6700 1800 60 
F8 "BTN_CE" I L 6700 1900 60 
F9 "BTNSHIFT_OUT" O R 8200 2250 60 
F10 "BTNSHIFT_IN" I L 6700 2000 60 
F11 "LEDSHIFT_OUT" O R 8200 2350 60 
F12 "LED_OE" I L 6700 2200 60 
F13 "LED_LOAD" I L 6700 2100 60 
F14 "LEDSHIFT_IN" I L 6700 2300 60 
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
F2 "MIDI_TX" O L 3600 1700 60 
F3 "MIDI_RX" I L 3600 1800 60 
F4 "USB_DP" B L 3600 2000 60 
F5 "USB_DM" B L 3600 2100 60 
F6 "PGM_RX" I L 3600 2300 60 
F7 "PGM_TX" O L 3600 2400 60 
F8 "LED_SHIFT" O R 5600 2300 60 
F9 "LED_OE" O R 5600 2200 60 
F10 "LED_LOAD" O R 5600 2100 60 
F11 "BTN_CE" O R 5600 1900 60 
F12 "BTN_LOAD" O R 5600 1800 60 
F13 "BTN_SHIFT" I R 5600 2500 60 
F14 "SHIFT_CLK" O R 5600 1700 60 
$EndSheet
Text Notes 1500 4050 0    60   ~ 0
STM32\n3V3 vreg\nMIDI I/O -- needs 5V\nmicro-USB\npins for USART1 programming\nencoders -- 2 LEDs, button, gray output\n   -- needs filter circuit 4R+2C?\n   -- blue and green LEDs have Vf up to 4V\n   -- so I need a level-changing buffer to drive the anode at 5V\n   -- use a latch instead of buffer, so the LEDs can be kept on all the time\nLCD COG or OLED display, on SPI\n2 buttons, at least\n   -- mode/power, ok, plypause, back\nunpopulated CV output circuitry?\nSPI EEPROM 8mbit\n"soft" power button -- keep idle power at uA levels\nSPI LCD display NHD-C12832A1Z-FSW-FBW-3V3 (Newhaven Display, 8.68€ @ Mouser)
$EndSCHEMATC
