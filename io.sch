EESchema Schematic File Version 2  date Mon 09 Apr 2012 11:08:00 PM CEST
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
Sheet 3 5
Title ""
Date "9 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_5 P5
U 1 1 4F8179DB
P 5900 6000
F 0 "P5" V 5850 6000 50  0000 C CNN
F 1 "CONN_5" V 5950 6000 50  0000 C CNN
	1    5900 6000
	1    0    0    -1  
$EndComp
Text HLabel 5200 6050 0    60   Input ~ 0
PGM_TX
Text HLabel 5200 5900 0    60   Output ~ 0
PGM_RX
Text HLabel 6550 1700 2    60   BiDi ~ 0
USB_DM
Text HLabel 6550 1550 2    60   BiDi ~ 0
USB_DP
Text HLabel 4600 4750 0    60   Input ~ 0
MIDI_TX
Text HLabel 8050 3100 0    60   Output ~ 0
MIDI_RX
Text Notes 5100 1400 0    60   ~ 0
micro-USB has five pins...
$Comp
L DIN_5 P4
U 1 1 4F8053D7
P 5700 4850
F 0 "P4" H 5700 4850 70  0000 C CNN
F 1 "DIN_5" H 5700 4700 70  0000 C CNN
	1    5700 4850
	1    0    0    -1  
$EndComp
$Comp
L DIN_5 P3
U 1 1 4F8053D3
P 4850 3250
F 0 "P3" H 4850 3250 70  0000 C CNN
F 1 "DIN_5" H 4850 3100 70  0000 C CNN
	1    4850 3250
	1    0    0    -1  
$EndComp
$Comp
L USB_2 J1
U 1 1 4F8053B5
P 5700 1700
F 0 "J1" H 5625 1950 60  0000 C CNN
F 1 "USB_2" H 5750 1400 60  0001 C CNN
F 4 "VCC" H 6025 1850 50  0001 C CNN "VCC"
F 5 "D+" H 6000 1750 50  0001 C CNN "Data+"
F 6 "D-" H 6000 1650 50  0001 C CNN "Data-"
F 7 "GND" H 6025 1550 50  0001 C CNN "Ground"
	1    5700 1700
	1    0    0    -1  
$EndComp
$Comp
L OPTO-TRANSISTOR4 U5
U 1 1 4F805397
P 6450 3050
F 0 "U5" H 6650 3300 60  0000 C CNN
F 1 "OPTO-TRANSISTOR4" H 6450 2800 60  0000 C CNN
	1    6450 3050
	1    0    0    -1  
$EndComp
$EndSCHEMATC
