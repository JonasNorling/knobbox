EESchema Schematic File Version 2  date Sat 01 Jun 2013 11:43:44 AM CEST
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
Sheet 2 10
Title ""
Date "1 jun 2013"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 6700 3900 2    60   Input ~ 0
FLASH_SPI_MOSI
Text HLabel 6700 3800 2    60   Input ~ 0
FLASH_SPI_SCK
Text HLabel 4500 3700 0    60   Output ~ 0
FLASH_SPI_MISO
Text HLabel 4500 3600 0    60   Input ~ 0
FLASH_CS
$Comp
L C C1
U 1 1 50CC832F
P 5600 4850
F 0 "C1" H 5650 4950 50  0000 L CNN
F 1 "100n" H 5650 4750 50  0000 L CNN
	1    5600 4850
	1    0    0    -1  
$EndComp
$Comp
L SPI-FLASH U1
U 1 1 50CC832E
P 5650 3750
F 0 "U1" H 5450 4050 60  0000 C CNN
F 1 "AT25DF081A-SH" H 5650 3500 60  0000 C CNN
	1    5650 3750
	1    0    0    -1  
$EndComp
Text Label 4500 3600 0    60   ~ 0
CS_FLASH
Text Label 4500 3700 0    60   ~ 0
SPI_MISO
Text Label 6700 3800 2    60   ~ 0
SPI_SCK
Text Label 6700 3900 2    60   ~ 0
SPI_MOSI
$Comp
L GND #PWR15
U 1 1 50CC832D
P 4850 4050
F 0 "#PWR15" H 4850 4050 30  0001 C CNN
F 1 "GND" H 4850 3980 30  0001 C CNN
	1    4850 4050
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR19
U 1 1 50CC832C
P 6300 3400
F 0 "#PWR19" H 6300 3360 30  0001 C CNN
F 1 "+3.3V" H 6300 3510 30  0000 C CNN
	1    6300 3400
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR16
U 1 1 50CC832B
P 5000 3400
F 0 "#PWR16" H 5000 3360 30  0001 C CNN
F 1 "+3.3V" H 5000 3510 30  0000 C CNN
	1    5000 3400
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR20
U 1 1 50CC832A
P 6450 3400
F 0 "#PWR20" H 6450 3360 30  0001 C CNN
F 1 "+3.3V" H 6450 3510 30  0000 C CNN
	1    6450 3400
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR17
U 1 1 50CC8329
P 5600 4550
F 0 "#PWR17" H 5600 4510 30  0001 C CNN
F 1 "+3.3V" H 5600 4660 30  0000 C CNN
	1    5600 4550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR18
U 1 1 50CC8328
P 5600 5200
F 0 "#PWR18" H 5600 5200 30  0001 C CNN
F 1 "GND" H 5600 5130 30  0001 C CNN
	1    5600 5200
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 50CC8327
P 5850 4850
F 0 "C8" H 5900 4950 50  0000 L CNN
F 1 "DNP" H 5900 4750 50  0000 L CNN
	1    5850 4850
	1    0    0    -1  
$EndComp
Connection ~ 5600 5100
Wire Wire Line
	5600 5100 5850 5100
Wire Wire Line
	5850 5100 5850 5050
Wire Wire Line
	5600 5200 5600 5050
Wire Wire Line
	6300 3400 6300 3600
Wire Wire Line
	4850 4050 4850 3900
Wire Wire Line
	4850 3900 5050 3900
Wire Wire Line
	5050 3700 4500 3700
Wire Wire Line
	5050 3600 4500 3600
Wire Wire Line
	6300 3600 6200 3600
Wire Wire Line
	6200 3800 6700 3800
Wire Wire Line
	6200 3900 6700 3900
Wire Wire Line
	5050 3800 5000 3800
Wire Wire Line
	5000 3800 5000 3400
Wire Wire Line
	6200 3700 6450 3700
Wire Wire Line
	6450 3700 6450 3400
Wire Wire Line
	5600 4650 5600 4550
Wire Wire Line
	5850 4650 5850 4600
Wire Wire Line
	5850 4600 5600 4600
Connection ~ 5600 4600
$EndSCHEMATC
