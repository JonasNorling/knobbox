EESchema Schematic File Version 2  date Tue 10 Apr 2012 09:01:04 PM CEST
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
LIBS:knobboard-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 2 5
Title ""
Date "9 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4250 2800 4250 3000
Wire Wire Line
	6350 2050 6350 2200
Wire Wire Line
	5550 3100 5550 3150
Wire Wire Line
	5550 3150 5650 3150
Wire Wire Line
	7050 2950 7600 2950
Wire Wire Line
	7050 2850 7600 2850
Wire Wire Line
	7050 2750 7600 2750
Wire Wire Line
	7050 2650 7600 2650
Wire Wire Line
	7050 2550 7600 2550
Wire Wire Line
	7050 2450 7600 2450
Wire Wire Line
	7050 2350 7600 2350
Wire Wire Line
	7050 2250 7600 2250
Wire Wire Line
	5150 2250 5650 2250
Wire Wire Line
	5150 2350 5650 2350
Wire Wire Line
	5150 2450 5650 2450
Wire Wire Line
	5150 2550 5650 2550
Wire Wire Line
	5150 2650 5650 2650
Wire Wire Line
	5150 2750 5650 2750
Wire Wire Line
	5150 2850 5650 2850
Wire Wire Line
	5150 2950 5650 2950
Wire Wire Line
	5150 3250 5650 3250
Wire Wire Line
	6350 3300 6350 3450
Wire Wire Line
	4250 2250 4250 2400
$Comp
L GND #PWR01
U 1 1 4F82A012
P 4250 3000
F 0 "#PWR01" H 4250 3000 30  0001 C CNN
F 1 "GND" H 4250 2930 30  0001 C CNN
	1    4250 3000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR02
U 1 1 4F82A00E
P 4250 2250
F 0 "#PWR02" H 4250 2210 30  0001 C CNN
F 1 "+3.3V" H 4250 2360 30  0000 C CNN
	1    4250 2250
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 4F81BBEC
P 4250 2600
F 0 "C3" H 4300 2700 50  0000 L CNN
F 1 "C" H 4300 2500 50  0000 L CNN
	1    4250 2600
	1    0    0    -1  
$EndComp
Text HLabel 5150 3250 0    60   Input ~ 0
OE1
$Comp
L GND #PWR03
U 1 1 4F81A943
P 6350 3450
F 0 "#PWR03" H 6350 3450 30  0001 C CNN
F 1 "GND" H 6350 3380 30  0001 C CNN
	1    6350 3450
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR04
U 1 1 4F81A93C
P 5550 3100
F 0 "#PWR04" H 5550 3060 30  0001 C CNN
F 1 "+3.3V" H 5550 3210 30  0000 C CNN
	1    5550 3100
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR05
U 1 1 4F81A937
P 6350 2050
F 0 "#PWR05" H 6350 2010 30  0001 C CNN
F 1 "+3.3V" H 6350 2160 30  0000 C CNN
	1    6350 2050
	1    0    0    -1  
$EndComp
Text GLabel 7600 2950 2    60   BiDi ~ 0
BUS_7
Text GLabel 7600 2850 2    60   BiDi ~ 0
BUS_6
Text GLabel 7600 2750 2    60   BiDi ~ 0
BUS_5
Text GLabel 7600 2650 2    60   BiDi ~ 0
BUS_4
Text GLabel 7600 2550 2    60   BiDi ~ 0
BUS_3
Text GLabel 7600 2450 2    60   BiDi ~ 0
BUS_2
Text GLabel 7600 2350 2    60   BiDi ~ 0
BUS_1
Text GLabel 7600 2250 2    60   BiDi ~ 0
BUS_0
Text HLabel 5150 4350 0    60   Input ~ 0
SW16
Text HLabel 5150 4250 0    60   Input ~ 0
SW15
Text HLabel 5150 4150 0    60   Input ~ 0
SW14
Text HLabel 5150 4050 0    60   Input ~ 0
SW13
Text HLabel 5150 3950 0    60   Input ~ 0
SW12
Text HLabel 5150 3850 0    60   Input ~ 0
SW11
Text HLabel 5150 3750 0    60   Input ~ 0
SW10
Text HLabel 5150 3650 0    60   Input ~ 0
SW9
Text HLabel 5150 2950 0    60   Input ~ 0
SW8
Text HLabel 5150 2850 0    60   Input ~ 0
SW7
Text HLabel 5150 2750 0    60   Input ~ 0
SW6
Text HLabel 5150 2650 0    60   Input ~ 0
SW5
Text HLabel 5150 2550 0    60   Input ~ 0
SW4
Text HLabel 5150 2450 0    60   Input ~ 0
SW3
Text HLabel 5150 2350 0    60   Input ~ 0
SW2
Text HLabel 5150 2250 0    60   Input ~ 0
SW1
$Comp
L 74LS245 U4
U 1 1 4F81A8A5
P 6350 2750
F 0 "U4" H 6450 3325 60  0000 L BNN
F 1 "74HC245" H 6400 2175 60  0000 L TNN
	1    6350 2750
	1    0    0    -1  
$EndComp
$EndSCHEMATC
