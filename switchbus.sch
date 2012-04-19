EESchema Schematic File Version 2  date Thu 19 Apr 2012 10:08:52 PM CEST
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
Sheet 3 6
Title ""
Date "19 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 2550 4800 0    60   Input ~ 0
BTNSHIFT_IN0
Wire Wire Line
	2900 4800 2550 4800
Text HLabel 2650 5900 0    60   Input ~ 0
SHIFT_CLK
Text HLabel 2650 5750 0    60   Input ~ 0
BTN_LOAD
Text HLabel 2650 6000 0    60   Input ~ 0
BTN_CE
Wire Wire Line
	2900 5900 2650 5900
Wire Wire Line
	2650 5750 2900 5750
Wire Wire Line
	2650 6000 2900 6000
$Comp
L C C26
U 1 1 4F905CC6
P 5150 5300
F 0 "C26" H 5200 5400 50  0000 L CNN
F 1 "100n" H 5200 5200 50  0000 L CNN
	1    5150 5300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR024
U 1 1 4F905CC5
P 5150 4950
F 0 "#PWR024" H 5150 4910 30  0001 C CNN
F 1 "+3.3V" H 5150 5060 30  0000 C CNN
	1    5150 4950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR025
U 1 1 4F905CC4
P 5150 5700
F 0 "#PWR025" H 5150 5700 30  0001 C CNN
F 1 "GND" H 5150 5630 30  0001 C CNN
	1    5150 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 5500 5150 5700
Wire Wire Line
	5150 4950 5150 5100
$Comp
L 74LS165 U10
U 1 1 4F905CAB
P 3600 5400
F 0 "U10" H 3750 5350 60  0000 C CNN
F 1 "74HC165" H 3750 5150 60  0000 C CNN
	1    3600 5400
	1    0    0    -1  
$EndComp
NoConn ~ 4300 3000
Wire Wire Line
	2900 2800 2550 2800
Wire Wire Line
	5150 3100 5150 3250
Wire Wire Line
	5150 3650 5150 3850
Wire Wire Line
	2650 4000 2900 4000
Wire Wire Line
	2650 3750 2900 3750
Wire Wire Line
	3300 4050 3300 4250
Wire Wire Line
	3300 2500 3300 2750
Wire Wire Line
	2900 3900 2650 3900
$Comp
L 74LS165 U4
U 1 1 4F8DA66D
P 3600 3400
F 0 "U4" H 3750 3350 60  0000 C CNN
F 1 "74HC165" H 3750 3150 60  0000 C CNN
	1    3600 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR026
U 1 1 4F8DA672
P 3300 4250
F 0 "#PWR026" H 3300 4250 30  0001 C CNN
F 1 "GND" H 3300 4180 30  0001 C CNN
	1    3300 4250
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR027
U 1 1 4F8DA671
P 3300 2500
F 0 "#PWR027" H 3300 2460 30  0001 C CNN
F 1 "+3.3V" H 3300 2610 30  0000 C CNN
	1    3300 2500
	1    0    0    -1  
$EndComp
Text HLabel 4300 2900 2    60   Output ~ 0
BTNSHIFT_OUT0
Text HLabel 2650 4000 0    60   Input ~ 0
BTN_CE
Text HLabel 2650 3750 0    60   Input ~ 0
BTN_LOAD
Text HLabel 2650 3900 0    60   Input ~ 0
SHIFT_CLK
Text HLabel 2550 2800 0    60   Input ~ 0
BTNSHIFT_IN0
$Comp
L GND #PWR028
U 1 1 4F82A012
P 5150 3850
F 0 "#PWR028" H 5150 3850 30  0001 C CNN
F 1 "GND" H 5150 3780 30  0001 C CNN
	1    5150 3850
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR029
U 1 1 4F82A00E
P 5150 3100
F 0 "#PWR029" H 5150 3060 30  0001 C CNN
F 1 "+3.3V" H 5150 3210 30  0000 C CNN
	1    5150 3100
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 4F81BBEC
P 5150 3450
F 0 "C3" H 5200 3550 50  0000 L CNN
F 1 "100n" H 5200 3350 50  0000 L CNN
	1    5150 3450
	1    0    0    -1  
$EndComp
Text HLabel 2900 5600 0    60   Input ~ 0
SW16
Text HLabel 2900 5500 0    60   Input ~ 0
SW15
Text HLabel 2900 5400 0    60   Input ~ 0
SW14
Text HLabel 2900 5300 0    60   Input ~ 0
SW13
Text HLabel 2900 5200 0    60   Input ~ 0
SW12
Text HLabel 2900 5100 0    60   Input ~ 0
SW11
Text HLabel 2900 5000 0    60   Input ~ 0
SW10
Text HLabel 2900 4900 0    60   Input ~ 0
SW9
Text HLabel 2900 3600 0    60   Input ~ 0
SW8
Text HLabel 2900 3500 0    60   Input ~ 0
SW7
Text HLabel 2900 3400 0    60   Input ~ 0
SW6
Text HLabel 2900 3300 0    60   Input ~ 0
SW5
Text HLabel 2900 3200 0    60   Input ~ 0
SW4
Text HLabel 2900 3100 0    60   Input ~ 0
SW3
Text HLabel 2900 3000 0    60   Input ~ 0
SW2
Text HLabel 2900 2900 0    60   Input ~ 0
SW1
$EndSCHEMATC
