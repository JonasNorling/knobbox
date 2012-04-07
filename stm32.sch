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
Sheet 2 3
Title ""
Date "6 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 3950 3050 0    60   ~ 0
MIDI_RX
Text Label 3950 2950 0    60   ~ 0
MIDI_TX
Text Notes 7650 3900 0    60   ~ 0
Might need pullups\non these, depending\non what a dangling pin\non the chip does.
Text Notes 8450 3450 0    60   ~ 0
Boot pull-down.\nWhat will the latch\nthink of this?
Wire Wire Line
	8250 3250 8250 3150
Wire Wire Line
	7050 2350 7750 2350
Wire Wire Line
	3900 2450 4450 2450
Wire Wire Line
	7400 4200 7400 4250
Wire Wire Line
	7400 3600 7400 3650
Wire Wire Line
	7400 4250 7050 4250
Wire Wire Line
	7400 3650 7050 3650
Wire Wire Line
	3050 3750 4450 3750
Wire Wire Line
	3350 3650 3350 3500
Connection ~ 7450 4750
Wire Wire Line
	7050 4750 7450 4750
Connection ~ 7450 4950
Wire Wire Line
	7050 4950 7450 4950
Connection ~ 4200 4750
Wire Wire Line
	4450 4750 4200 4750
Connection ~ 4200 4650
Wire Wire Line
	4200 4450 4200 4950
Wire Wire Line
	4200 4950 4450 4950
Wire Wire Line
	7050 2650 7750 2650
Wire Wire Line
	4450 2950 3900 2950
Wire Wire Line
	4450 3350 3900 3350
Wire Wire Line
	4450 3950 3900 3950
Wire Wire Line
	4450 3850 3900 3850
Wire Wire Line
	4450 3250 3900 3250
Wire Wire Line
	4450 3450 3900 3450
Wire Wire Line
	4450 3050 3900 3050
Wire Wire Line
	7050 2750 7750 2750
Wire Wire Line
	4200 4650 4450 4650
Wire Wire Line
	4450 4850 4200 4850
Connection ~ 4200 4850
Wire Wire Line
	7050 4650 7450 4650
Wire Wire Line
	7450 4650 7450 5250
Wire Wire Line
	7450 4850 7050 4850
Connection ~ 7450 4850
Wire Wire Line
	4450 3550 3500 3550
Wire Wire Line
	3500 3550 3500 3500
Wire Wire Line
	3050 3650 4450 3650
Connection ~ 3350 3650
Wire Wire Line
	7050 2950 7400 2950
Wire Wire Line
	7050 3750 7400 3750
Wire Wire Line
	7400 2950 7400 3000
Wire Wire Line
	7400 3750 7400 3800
Wire Wire Line
	4450 2350 3900 2350
Wire Wire Line
	7050 2450 7750 2450
Wire Wire Line
	8250 3150 7050 3150
$Comp
L R R?
U 1 1 4F7F4598
P 8250 3500
F 0 "R?" V 8330 3500 50  0000 C CNN
F 1 "R" V 8250 3500 50  0000 C CNN
	1    8250 3500
	1    0    0    -1  
$EndComp
Text Label 7100 2450 0    60   ~ 0
BOOTJUMPER
Text Label 3950 2450 0    60   ~ 0
BTN_2
Text Label 3950 2350 0    60   ~ 0
BTN_1
Text Notes 7200 3900 0    60   ~ 0
encoder\noutput\nlatch\nenables
Text Notes 7200 3200 0    60   ~ 0
encoder\noutputs\nthrough\nbuffers/latches
Text HLabel 3750 2350 1    60   Input ~ 0
LED_LATCH
Text HLabel 3650 2350 1    60   Input ~ 0
LED_OE
$Comp
L CP1 C?
U 1 1 4F7F406E
P 5850 5650
F 0 "C?" H 5900 5750 50  0000 L CNN
F 1 "CP1" H 5900 5550 50  0000 L CNN
	1    5850 5650
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 4F7F405D
P 5350 5650
F 0 "C?" H 5400 5750 50  0000 L CNN
F 1 "C" H 5400 5550 50  0000 L CNN
	1    5350 5650
	1    0    0    -1  
$EndComp
Text Label 3950 3950 0    60   ~ 0
USB_DP
Text Label 3950 3850 0    60   ~ 0
USB_DM
Text HLabel 3350 3500 1    60   Output ~ 0
LED_DATA
Text Label 3950 3250 0    60   ~ 0
SPI_SCK
Text Label 3950 3350 0    60   ~ 0
SPI_MISO
Text Label 3950 3450 0    60   ~ 0
SPI_MOSI
Text HLabel 3500 3500 1    60   Output ~ 0
LED_CLK
Text Label 3950 3650 0    60   ~ 0
USART1_TX
Text Label 3950 3550 0    60   ~ 0
USART1_CK
$Comp
L STM32F102CX U2
U 1 1 4F7F00D7
P 5800 3600
F 0 "U2" H 6700 2050 60  0000 C CNN
F 1 "STM32F102CX" H 5050 2050 60  0000 C CNN
	1    5800 3600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
