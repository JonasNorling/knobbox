EESchema Schematic File Version 2  date Sun 09 Jun 2013 01:05:06 PM CEST
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
Sheet 3 10
Title ""
Date "9 jun 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5050 2850 4950 2850
Wire Wire Line
	4950 2850 4950 2550
Wire Wire Line
	4350 2750 5050 2750
Wire Wire Line
	6200 2650 7300 2650
Connection ~ 6500 2850
Wire Wire Line
	6500 2550 6500 2950
Wire Wire Line
	6500 2950 6200 2950
Wire Wire Line
	6500 2850 6200 2850
Connection ~ 5550 4350
Wire Wire Line
	5550 4350 5800 4350
Wire Wire Line
	5800 4350 5800 4400
Wire Wire Line
	5550 4400 5550 4300
Wire Wire Line
	5550 4950 5550 4800
Wire Wire Line
	5800 4800 5800 4850
Wire Wire Line
	5800 4850 5550 4850
Connection ~ 5550 4850
Wire Wire Line
	6200 2750 6400 2750
Wire Wire Line
	6400 2750 6400 3100
Wire Wire Line
	4350 2950 5050 2950
Wire Wire Line
	4350 2650 5050 2650
$Comp
L +3.3V #PWR014
U 1 1 51B358E6
P 4950 2550
F 0 "#PWR014" H 4950 2510 30  0001 C CNN
F 1 "+3.3V" H 4950 2660 30  0000 C CNN
	1    4950 2550
	1    0    0    -1  
$EndComp
Text Label 4400 2650 0    60   ~ 0
SPI_MOSI
Text HLabel 4350 2650 0    60   Input ~ 0
FLASH_SPI_MOSI
Text Label 4400 2750 0    60   ~ 0
SPI_SCK
Text HLabel 4350 2750 0    60   Input ~ 0
FLASH_SPI_SCK
Text Label 4400 2950 0    60   ~ 0
CS_FLASH
Text HLabel 4350 2950 0    60   Input ~ 0
FLASH_CS
Text Label 6850 2650 0    60   ~ 0
SPI_MISO
Text HLabel 7300 2650 2    60   Output ~ 0
FLASH_SPI_MISO
$Comp
L GND #PWR015
U 1 1 51B35829
P 6400 3100
F 0 "#PWR015" H 6400 3100 30  0001 C CNN
F 1 "GND" H 6400 3030 30  0001 C CNN
	1    6400 3100
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR016
U 1 1 51B35823
P 6500 2550
F 0 "#PWR016" H 6500 2510 30  0001 C CNN
F 1 "+3.3V" H 6500 2660 30  0000 C CNN
	1    6500 2550
	1    0    0    -1  
$EndComp
$Comp
L AT45DB U1
U 1 1 51B35808
P 5650 2800
F 0 "U1" H 5450 3100 60  0000 C CNN
F 1 "AT45DB" H 5650 2550 60  0000 C CNN
	1    5650 2800
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 50CC832F
P 5550 4600
F 0 "C1" H 5600 4700 50  0000 L CNN
F 1 "100n" H 5600 4500 50  0000 L CNN
	1    5550 4600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR017
U 1 1 50CC8329
P 5550 4300
F 0 "#PWR017" H 5550 4260 30  0001 C CNN
F 1 "+3.3V" H 5550 4410 30  0000 C CNN
	1    5550 4300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 50CC8328
P 5550 4950
F 0 "#PWR018" H 5550 4950 30  0001 C CNN
F 1 "GND" H 5550 4880 30  0001 C CNN
	1    5550 4950
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 50CC8327
P 5800 4600
F 0 "C8" H 5850 4700 50  0000 L CNN
F 1 "DNP" H 5850 4500 50  0000 L CNN
	1    5800 4600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
