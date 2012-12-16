EESchema Schematic File Version 2  date Sun 16 Dec 2012 05:53:49 PM CET
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
Date "16 dec 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 10900 3950 2    60   ~ 0
Add test points on SPI busses, shift regs, some unused pins.
$Comp
L TST TP11
U 1 1 50CDE40E
P 9000 3800
F 0 "TP11" H 9000 4100 40  0000 C CNN
F 1 "TST" H 9000 4050 30  0000 C CNN
	1    9000 3800
	1    0    0    -1  
$EndComp
$Comp
L TST TP0
U 1 1 50CDE3D7
P 10200 6900
F 0 "TP0" H 10200 7200 40  0000 C CNN
F 1 "TST" H 10200 7150 30  0000 C CNN
	1    10200 6900
	1    0    0    -1  
$EndComp
$Comp
L TST TP10
U 1 1 50CDE3C4
P 10850 6300
F 0 "TP10" H 10850 6600 40  0000 C CNN
F 1 "TST" H 10850 6550 30  0000 C CNN
	1    10850 6300
	1    0    0    -1  
$EndComp
$Comp
L TST TP9
U 1 1 50CDE3AD
P 2650 4100
F 0 "TP9" H 2650 4400 40  0000 C CNN
F 1 "TST" H 2650 4350 30  0000 C CNN
	1    2650 4100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3050 2500 3200 2500
Wire Wire Line
	9200 2100 9350 2100
Wire Wire Line
	10700 2600 10700 2500
Wire Wire Line
	9200 1900 9350 1900
Wire Wire Line
	9200 1800 9350 1800
Wire Wire Line
	9200 1700 9350 1700
Wire Wire Line
	6200 4000 6400 4000
Wire Wire Line
	6200 3900 6400 3900
Wire Wire Line
	2650 4400 3200 4400
Wire Wire Line
	2650 4300 3200 4300
Wire Wire Line
	2650 4200 3200 4200
Wire Wire Line
	2650 4100 3200 4100
Wire Wire Line
	2650 4000 3200 4000
Wire Wire Line
	2650 3900 3200 3900
Wire Wire Line
	3050 2400 3200 2400
Wire Wire Line
	3050 2200 3200 2200
Wire Wire Line
	3050 2000 3200 2000
Wire Notes Line
	6950 4400 7050 4350
Wire Notes Line
	7050 4350 6950 4300
Wire Wire Line
	6200 3300 6350 3300
Wire Wire Line
	2950 5350 2950 6000
Wire Wire Line
	2950 6000 3200 6000
Wire Wire Line
	7100 3250 7100 3500
Wire Wire Line
	3200 4600 2650 4600
Wire Wire Line
	2650 4700 3200 4700
Connection ~ 10300 6300
Wire Wire Line
	10300 6300 10300 6400
Wire Wire Line
	10300 6400 10200 6400
Wire Wire Line
	2150 7550 3650 7550
Wire Wire Line
	2150 7150 3650 7150
Connection ~ 10450 6900
Wire Wire Line
	10850 6900 8900 6900
Wire Wire Line
	10850 6900 10850 6750
Connection ~ 9650 6900
Wire Wire Line
	10450 6900 10450 6750
Connection ~ 10450 6300
Wire Wire Line
	9100 6300 8900 6300
Wire Wire Line
	2600 6000 2600 6150
Connection ~ 2400 7550
Wire Wire Line
	2950 5400 3200 5400
Wire Wire Line
	2650 5000 3200 5000
Wire Wire Line
	3200 5100 2650 5100
Connection ~ 3400 7550
Wire Wire Line
	3650 7550 3650 7650
Connection ~ 2900 7550
Connection ~ 3150 7150
Connection ~ 2650 7150
Connection ~ 7700 2650
Wire Wire Line
	7700 2850 7700 2650
Connection ~ 7700 3400
Wire Wire Line
	2650 4500 3200 4500
Connection ~ 6600 5800
Wire Wire Line
	6600 5800 6200 5800
Wire Wire Line
	6600 6200 6600 5600
Wire Wire Line
	6600 5600 6200 5600
Connection ~ 2950 5800
Wire Wire Line
	3200 5800 2950 5800
Wire Wire Line
	3200 4800 2650 4800
Wire Wire Line
	3200 4900 2650 4900
Wire Wire Line
	2950 5900 3200 5900
Connection ~ 2950 5600
Wire Wire Line
	2950 5700 3200 5700
Connection ~ 2950 5700
Wire Wire Line
	6600 5900 6200 5900
Connection ~ 6600 5900
Wire Wire Line
	6200 5700 6600 5700
Connection ~ 6600 5700
Connection ~ 2950 5900
Connection ~ 7100 3400
Wire Wire Line
	7100 2850 7100 2650
Wire Wire Line
	7100 2650 7800 2650
Connection ~ 2900 7150
Wire Wire Line
	3650 7150 3650 7100
Connection ~ 3400 7150
Connection ~ 2650 7550
Connection ~ 3150 7550
Wire Wire Line
	7100 3500 6200 3500
Connection ~ 2950 5400
Connection ~ 2400 7150
Wire Wire Line
	2600 5600 3200 5600
Wire Wire Line
	10200 6300 10850 6300
Wire Wire Line
	10850 6300 10850 6350
Wire Wire Line
	10450 6100 10450 6350
Wire Wire Line
	8900 6900 8900 6700
Wire Wire Line
	9650 7000 9650 6850
Wire Wire Line
	8900 6300 8900 6200
Wire Wire Line
	6200 3600 7700 3600
Wire Wire Line
	7700 3600 7700 3250
Wire Wire Line
	6200 6000 6600 6000
Connection ~ 6600 6000
Wire Wire Line
	6200 3200 6650 3200
Wire Wire Line
	6400 4100 6200 4100
Wire Notes Line
	7200 3900 7300 3950
Wire Notes Line
	7300 3950 7200 4000
Wire Wire Line
	3200 2100 3050 2100
Wire Wire Line
	3200 2300 3050 2300
Wire Wire Line
	3200 2600 3050 2600
Wire Wire Line
	3200 2700 3050 2700
Wire Wire Line
	3200 2800 3050 2800
Wire Wire Line
	3200 2900 3050 2900
Wire Wire Line
	2650 5200 3200 5200
Wire Wire Line
	6200 4300 6400 4300
Wire Wire Line
	6200 4400 6400 4400
Wire Wire Line
	6200 4500 6400 4500
Wire Wire Line
	6200 4600 6400 4600
Wire Wire Line
	6200 4700 6400 4700
Wire Wire Line
	6200 4800 6400 4800
Wire Wire Line
	6200 4900 6400 4900
Wire Wire Line
	6200 5100 6400 5100
Wire Wire Line
	6200 5200 6400 5200
Wire Wire Line
	6200 5300 6400 5300
Wire Wire Line
	6200 5400 6400 5400
Wire Wire Line
	10150 1700 10700 1700
Wire Wire Line
	10150 1800 10700 1800
Wire Wire Line
	10150 1900 10700 1900
Wire Wire Line
	10150 2100 10700 2100
Wire Wire Line
	6200 4200 6700 4200
Wire Wire Line
	6200 5000 6400 5000
Text HLabel 6400 5000 2    60   Input ~ 0
MIDI_RX2
$Comp
L GND #PWR08
U 1 1 50CDD646
P 9200 2100
F 0 "#PWR08" H 9200 2100 30  0001 C CNN
F 1 "GND" H 9200 2030 30  0001 C CNN
	1    9200 2100
	0    1    1    0   
$EndComp
Text Label 10700 1900 2    60   ~ 0
SWO
Text Label 6700 4200 2    60   ~ 0
SWO
Text Label 10700 1800 2    60   ~ 0
SWCLK
Text Label 10700 2100 2    60   ~ 0
RST
Text Label 10700 1700 2    60   ~ 0
SWDIO
NoConn ~ 10150 2000
$Comp
L +3.3V #PWR09
U 1 1 50CDD48A
P 9200 1700
F 0 "#PWR09" H 9200 1660 30  0001 C CNN
F 1 "+3.3V" H 9200 1810 30  0000 C CNN
	1    9200 1700
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR010
U 1 1 50CDD483
P 9200 1900
F 0 "#PWR010" H 9200 1900 30  0001 C CNN
F 1 "GND" H 9200 1830 30  0001 C CNN
	1    9200 1900
	0    1    1    0   
$EndComp
$Comp
L GND #PWR011
U 1 1 50CDD47F
P 9200 1800
F 0 "#PWR011" H 9200 1800 30  0001 C CNN
F 1 "GND" H 9200 1730 30  0001 C CNN
	1    9200 1800
	0    1    1    0   
$EndComp
NoConn ~ 9350 2000
$Comp
L CONN_5X2 P2
U 1 1 50CDD464
P 9750 1900
F 0 "P2" H 9750 2200 60  0000 C CNN
F 1 "CONN_5X2" V 9750 1900 50  0000 C CNN
	1    9750 1900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 50CC9E8D
P 10700 2600
F 0 "#PWR012" H 10700 2600 30  0001 C CNN
F 1 "GND" H 10700 2530 30  0001 C CNN
	1    10700 2600
	1    0    0    -1  
$EndComp
$Comp
L C C33
U 1 1 50CC9E7F
P 10700 2300
F 0 "C33" H 10750 2400 50  0000 L CNN
F 1 "100n" H 10750 2200 50  0000 L CNN
	1    10700 2300
	1    0    0    -1  
$EndComp
Text Notes 7050 4400 0    60   ~ 0
TIM3_CH1/2
Text Notes 7300 4000 0    60   ~ 0
TIM1_CH2/3N
Text Notes 4950 5200 2    60   ~ 0
SPI2_NSS
Text Notes 3850 5300 0    60   ~ 0
SPI1_NSS
Text HLabel 2650 5200 0    60   Output ~ 0
FLASH_CS
Text HLabel 2650 4200 0    60   Output ~ 0
FLASH_SPI_SCK
Text HLabel 2650 4400 0    60   Output ~ 0
FLASH_SPI_MOSI
Text HLabel 2650 4300 0    60   Input ~ 0
FLASH_SPI_MISO
Text HLabel 6400 5400 2    60   Output ~ 0
LCD_SPI_MOSI
Text HLabel 6400 5200 2    60   Output ~ 0
LCD_SPI_SCK
Text HLabel 6400 5300 2    60   Output ~ 0
LCD_A0
Text HLabel 6400 5100 2    60   Output ~ 0
LCD_CS
Text HLabel 6400 3900 2    60   Output ~ 0
BACKLIGHT_PWM
Text HLabel 6400 4400 2    60   Input ~ 0
ENCODER2
Text HLabel 6400 4300 2    60   Input ~ 0
ENCODER1
Text HLabel 6400 4000 2    60   Output ~ 0
LED5
Text HLabel 3050 2900 0    60   Output ~ 0
LED4
Text HLabel 3050 2800 0    60   Output ~ 0
LED3
Text HLabel 3050 2700 0    60   Output ~ 0
LED2
Text HLabel 3050 2600 0    60   Output ~ 0
LED1
Text HLabel 3050 2400 0    60   Input ~ 0
BTN5
Text HLabel 3050 2300 0    60   Input ~ 0
BTN4
Text HLabel 3050 2200 0    60   Input ~ 0
BTN3
Text HLabel 3050 2100 0    60   Input ~ 0
BTN2
Text HLabel 3050 2000 0    60   Input ~ 0
BTN1
$Comp
L GND #PWR013
U 1 1 50CC7C9F
P 6400 4100
F 0 "#PWR013" H 6400 4100 30  0001 C CNN
F 1 "GND" H 6400 4030 30  0001 C CNN
	1    6400 4100
	0    -1   -1   0   
$EndComp
Text HLabel 6400 4900 2    60   Output ~ 0
MIDI_TX2
Text HLabel 2650 3900 0    60   Output ~ 0
MIDI_TX1
Text Label 6650 3200 2    60   ~ 0
RST
$Comp
L GND #PWR014
U 1 1 50CC762B
P 6350 3300
F 0 "#PWR014" H 6350 3300 30  0001 C CNN
F 1 "GND" H 6350 3230 30  0001 C CNN
	1    6350 3300
	0    -1   -1   0   
$EndComp
$Comp
L STM32F103RC U2
U 1 1 50CC75A2
P 4750 4550
F 0 "U2" H 5850 2900 60  0000 C CNN
F 1 "STM32F103RC" H 3800 2900 60  0000 C CNN
	1    4750 4550
	1    0    0    -1  
$EndComp
Text Label 2700 4700 0    60   ~ 0
USART1_RX
Text HLabel 2650 4600 0    60   Output ~ 0
LED_SHIFT
Text HLabel 6400 4800 2    60   Output ~ 0
LED_OE
Text HLabel 6400 4700 2    60   Output ~ 0
LED_LOAD
Text HLabel 6400 4600 2    60   Output ~ 0
BTN_CE
Text HLabel 6400 4500 2    60   Output ~ 0
BTN_LOAD
Text HLabel 2650 4700 0    60   Input ~ 0
BTN_SHIFT
Text HLabel 2650 4500 0    60   Output ~ 0
SHIFT_CLK
$Comp
L C C10
U 1 1 4F892E97
P 3400 7350
F 0 "C10" H 3450 7450 50  0000 L CNN
F 1 "10u" H 3450 7250 50  0000 L CNN
	1    3400 7350
	1    0    0    -1  
$EndComp
Text Notes 7050 2400 0    60   ~ 0
ABLS-8.000MHZ-B2-T\nHC-49/US\nESR max 80ohm
$Comp
L +5V #PWR015
U 1 1 4F834C8B
P 8900 6200
F 0 "#PWR015" H 8900 6290 20  0001 C CNN
F 1 "+5V" H 8900 6290 30  0000 C CNN
	1    8900 6200
	1    0    0    -1  
$EndComp
$Comp
L LM1117 U6
U 1 1 4F8346B0
P 9650 6400
F 0 "U6" H 9850 6200 60  0000 C CNN
F 1 "LM1117" H 9650 6650 60  0000 C CNN
	1    9650 6400
	1    0    0    -1  
$EndComp
$Comp
L CP1 C17
U 1 1 4F8322A8
P 10850 6550
F 0 "C17" H 10900 6650 50  0000 L CNN
F 1 "100u" H 10900 6450 50  0000 L CNN
	1    10850 6550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 4F832167
P 9650 7000
F 0 "#PWR016" H 9650 7000 30  0001 C CNN
F 1 "GND" H 9650 6930 30  0001 C CNN
	1    9650 7000
	1    0    0    -1  
$EndComp
$Comp
L CP1 C4
U 1 1 4F83214B
P 10450 6550
F 0 "C4" H 10500 6650 50  0000 L CNN
F 1 "10u" H 10500 6450 50  0000 L CNN
	1    10450 6550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 4F8288D9
P 2600 6150
F 0 "#PWR017" H 2600 6150 30  0001 C CNN
F 1 "GND" H 2600 6080 30  0001 C CNN
	1    2600 6150
	1    0    0    -1  
$EndComp
$Comp
L C C7
U 1 1 4F8288AD
P 2600 5800
F 0 "C7" H 2650 5900 50  0000 L CNN
F 1 "1u" H 2650 5700 50  0000 L CNN
	1    2600 5800
	1    0    0    -1  
$EndComp
Text Notes 850  6650 0    60   ~ 0
AN2586 recommends 100n on all power pins,\nplus one 10uF, plus another 1u on VDDA-VSSA.\nWe're ignoring the 100n on VDDA, because the 1u is in the\nsame package (0805) with the same impedance.\nPlace small caps (at least 0805) for smaller impedance.
Text Notes 8450 5300 0    60   ~ 0
Decoupling demands? Big can on 5V for LEDs?\n100u 16V: UUR1C101MCL1GS (0.15€). φ8 × 6.2\n10u available in 1206 or 0805 @ 0.074€.\n100n available in 0805 @ 1.7€ for 100.\n\n771-NX1117C33Z115 is 0.2€, fixed 3.3V 1A, 1.2V dropout.\nSOT223, tab is VOUT. Wants 10u on in and out.
$Comp
L CP1 C5
U 1 1 4F81BBA4
P 8900 6500
F 0 "C5" H 8950 6600 50  0000 L CNN
F 1 "10u" H 8950 6400 50  0000 L CNN
	1    8900 6500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR018
U 1 1 4F805934
P 10450 6100
F 0 "#PWR018" H 10450 6060 30  0001 C CNN
F 1 "+3.3V" H 10450 6210 30  0000 C CNN
	1    10450 6100
	1    0    0    -1  
$EndComp
Text GLabel 3050 2500 0    60   Output ~ 0
VPULLUP
Text Label 2650 5000 0    60   ~ 0
SWDIO
Text Label 2650 5100 0    60   ~ 0
SWCLK
Text Notes 4000 1350 0    60   ~ 0
Boot modes:\nBOOT1:BOOT0 = x0 --> flash\nBOOT1:BOOT0 = 01 --> bootloader
$Comp
L +3.3V #PWR019
U 1 1 4F804819
P 3650 7100
F 0 "#PWR019" H 3650 7060 30  0001 C CNN
F 1 "+3.3V" H 3650 7210 30  0000 C CNN
	1    3650 7100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 4F804810
P 3650 7650
F 0 "#PWR020" H 3650 7650 30  0001 C CNN
F 1 "GND" H 3650 7580 30  0001 C CNN
	1    3650 7650
	1    0    0    -1  
$EndComp
$Comp
L C C13
U 1 1 4F8047E5
P 2400 7350
F 0 "C13" H 2450 7450 50  0000 L CNN
F 1 "100n" H 2450 7250 50  0000 L CNN
	1    2400 7350
	1    0    0    -1  
$EndComp
$Comp
L C C14
U 1 1 4F8047E3
P 2650 7350
F 0 "C14" H 2700 7450 50  0000 L CNN
F 1 "100n" H 2700 7250 50  0000 L CNN
	1    2650 7350
	1    0    0    -1  
$EndComp
$Comp
L C C15
U 1 1 4F8047E0
P 2900 7350
F 0 "C15" H 2950 7450 50  0000 L CNN
F 1 "100n" H 2950 7250 50  0000 L CNN
	1    2900 7350
	1    0    0    -1  
$EndComp
Text Label 6250 3600 0    60   ~ 0
OSCOUT
Text Label 6250 3500 0    60   ~ 0
OSCIN
$Comp
L GND #PWR021
U 1 1 4F804740
P 7800 2650
F 0 "#PWR021" H 7800 2650 30  0001 C CNN
F 1 "GND" H 7800 2580 30  0001 C CNN
	1    7800 2650
	0    -1   -1   0   
$EndComp
$Comp
L C C12
U 1 1 4F8046B9
P 7700 3050
F 0 "C12" H 7750 3150 50  0000 L CNN
F 1 "16pF" H 7750 2950 50  0000 L CNN
	1    7700 3050
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 4F8046B1
P 7100 3050
F 0 "C2" H 7150 3150 50  0000 L CNN
F 1 "16pF" H 7150 2950 50  0000 L CNN
	1    7100 3050
	1    0    0    -1  
$EndComp
$Comp
L CRYSTAL X1
U 1 1 4F8046A8
P 7400 3400
F 0 "X1" H 7400 3550 60  0000 C CNN
F 1 "8MHz" H 7400 3250 60  0000 C CNN
	1    7400 3400
	1    0    0    -1  
$EndComp
Text HLabel 2650 4000 0    60   Input ~ 0
MIDI_RX
Text HLabel 2650 4900 0    60   BiDi ~ 0
USB_DP
Text HLabel 2650 4800 0    60   BiDi ~ 0
USB_DM
$Comp
L +3.3V #PWR022
U 1 1 4F802EBD
P 2950 5350
F 0 "#PWR022" H 2950 5310 30  0001 C CNN
F 1 "+3.3V" H 2950 5460 30  0000 C CNN
	1    2950 5350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 4F802EA6
P 6600 6200
F 0 "#PWR023" H 6600 6200 30  0001 C CNN
F 1 "GND" H 6600 6130 30  0001 C CNN
	1    6600 6200
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 4F7F405D
P 3150 7350
F 0 "C9" H 3200 7450 50  0000 L CNN
F 1 "100n" H 3200 7250 50  0000 L CNN
	1    3150 7350
	1    0    0    -1  
$EndComp
Text Label 2700 4900 0    60   ~ 0
USB_DP
Text Label 2700 4800 0    60   ~ 0
USB_DM
Text Label 2700 4600 0    60   ~ 0
USART1_TX
Text Label 2700 4500 0    60   ~ 0
USART1_CK
$EndSCHEMATC
