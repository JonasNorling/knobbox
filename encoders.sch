EESchema Schematic File Version 2  date Sun 15 Apr 2012 10:50:25 AM CEST
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
Sheet 5 5
Title ""
Date "15 apr 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 950  5350 0    60   ~ 0
PEL12D green/red max 30mA. Vf is 3.2/1.95V.\nDriven at 8mA, resistors should be 225/381 ohms.\nterminal 2 is green, terminal 3 is red.
$Comp
L C C16
U 1 1 4F893460
P 2050 6800
F 0 "C16" H 2100 6900 50  0000 L CNN
F 1 "10u" H 2100 6700 50  0000 L CNN
	1    2050 6800
	1    0    0    -1  
$EndComp
Text Label 2600 4200 0    60   ~ 0
LED42
Text Label 2600 4100 0    60   ~ 0
LED41
Text Label 2600 4000 0    60   ~ 0
LED32
Text Label 2600 3900 0    60   ~ 0
LED31
Text Label 2600 3800 0    60   ~ 0
LED22
Text Label 2600 3700 0    60   ~ 0
LED21
Text Label 2600 3600 0    60   ~ 0
LED12
Connection ~ 850  3800
Wire Wire Line
	850  3800 1150 3800
Connection ~ 1000 4400
Wire Wire Line
	1150 4400 1000 4400
Wire Wire Line
	2550 4000 3350 4000
Wire Wire Line
	2550 4200 3150 4200
Wire Wire Line
	2550 3700 2950 3700
Wire Wire Line
	2950 3700 2950 3550
Wire Wire Line
	2550 3500 2750 3500
Wire Wire Line
	2750 3500 2750 2100
Wire Wire Line
	2850 3600 2850 2200
Wire Wire Line
	2850 3600 2550 3600
Wire Wire Line
	2550 3800 3050 3800
Wire Wire Line
	2550 4100 3250 4100
Wire Wire Line
	2550 3900 3450 3900
Wire Wire Line
	2950 3550 4600 3550
Wire Wire Line
	10300 5650 10300 5800
Connection ~ 1850 7000
Wire Wire Line
	1850 7150 1850 7000
Connection ~ 1850 6600
Wire Wire Line
	1850 6500 1850 6600
Connection ~ 4550 850 
Wire Wire Line
	4550 850  4550 950 
Connection ~ 4350 850 
Wire Wire Line
	4350 850  4350 950 
Connection ~ 4150 850 
Wire Wire Line
	4150 850  4150 950 
Connection ~ 3950 850 
Wire Wire Line
	3950 850  3950 950 
Connection ~ 3650 850 
Wire Wire Line
	3650 950  3650 850 
Wire Wire Line
	4550 1450 4550 1750
Wire Wire Line
	4550 1750 6500 1750
Wire Wire Line
	6500 1750 6500 2050
Wire Wire Line
	5200 2000 4650 2000
Wire Wire Line
	4250 1450 4250 3250
Wire Wire Line
	4050 1450 4050 4600
Wire Wire Line
	3850 1450 3850 4800
Wire Wire Line
	4250 3250 6500 3250
Wire Wire Line
	6700 3700 6700 3150
Wire Wire Line
	5200 3450 4500 3450
Wire Wire Line
	6500 4700 3950 4700
Wire Wire Line
	6500 4700 6500 4950
Wire Wire Line
	5200 4900 4500 4900
Connection ~ 4850 6400
Wire Wire Line
	5200 6400 4550 6400
Wire Wire Line
	3650 1450 3650 6150
Wire Wire Line
	3650 6150 6500 6150
Wire Wire Line
	6500 6150 6500 6450
Wire Wire Line
	4600 6600 3150 6600
Wire Wire Line
	4600 5100 3350 5100
Wire Wire Line
	4600 3650 3050 3650
Wire Wire Line
	2850 2200 4600 2200
Wire Wire Line
	8950 4750 8950 4800
Wire Wire Line
	9650 4950 9650 5100
Wire Wire Line
	1000 4700 1000 4100
Wire Wire Line
	8950 4300 8400 4300
Wire Wire Line
	8400 4300 8400 4950
Wire Wire Line
	8400 4950 7300 4950
Wire Wire Line
	8950 4500 8600 4500
Wire Wire Line
	8600 4500 8600 6450
Wire Wire Line
	8600 6450 7300 6450
Wire Wire Line
	8900 5200 8900 4900
Wire Wire Line
	8900 4900 8950 4900
Wire Wire Line
	8950 4200 8550 4200
Wire Wire Line
	8550 4200 8550 3700
Wire Wire Line
	8550 3700 7300 3700
Wire Wire Line
	8950 4000 8750 4000
Wire Wire Line
	8750 4000 8750 2250
Wire Wire Line
	8750 2250 7300 2250
Wire Wire Line
	4850 6400 4850 6250
Wire Wire Line
	3350 5100 3350 4000
Wire Wire Line
	3150 6600 3150 4200
Wire Wire Line
	7450 6700 7450 6650
Wire Wire Line
	7750 7100 7450 7100
Wire Wire Line
	5200 6500 5100 6500
Connection ~ 6700 6650
Wire Wire Line
	6350 6650 6800 6650
Wire Wire Line
	5150 6850 5150 6700
Wire Wire Line
	5150 6700 5200 6700
Wire Wire Line
	6350 6450 6800 6450
Connection ~ 6500 6450
Wire Wire Line
	6350 6550 6600 6550
Wire Wire Line
	6600 6550 6600 6800
Wire Wire Line
	5100 6600 5200 6600
Wire Wire Line
	7600 7150 7600 7100
Connection ~ 7600 7100
Connection ~ 7450 6650
Wire Wire Line
	7750 6700 7750 6450
Connection ~ 7750 6450
Connection ~ 7750 4950
Wire Wire Line
	7750 5200 7750 4950
Connection ~ 7450 5150
Connection ~ 7600 5600
Wire Wire Line
	7600 5650 7600 5600
Wire Wire Line
	5100 5100 5200 5100
Wire Wire Line
	6600 5300 6600 5050
Wire Wire Line
	6600 5050 6350 5050
Connection ~ 6500 4950
Wire Wire Line
	6350 4950 6800 4950
Wire Wire Line
	5200 5200 5150 5200
Wire Wire Line
	5150 5200 5150 5350
Wire Wire Line
	6350 5150 6800 5150
Connection ~ 6700 5150
Wire Wire Line
	5200 5000 5100 5000
Wire Wire Line
	7750 5600 7450 5600
Wire Wire Line
	7450 5200 7450 5150
Wire Wire Line
	7450 3750 7450 3700
Wire Wire Line
	7750 4150 7450 4150
Wire Wire Line
	5200 3550 5100 3550
Connection ~ 6700 3700
Wire Wire Line
	6350 3700 6800 3700
Wire Wire Line
	5150 3900 5150 3750
Wire Wire Line
	5150 3750 5200 3750
Wire Wire Line
	6350 3500 6800 3500
Connection ~ 6500 3500
Wire Wire Line
	6350 3600 6600 3600
Wire Wire Line
	6600 3600 6600 3850
Wire Wire Line
	5100 3650 5200 3650
Wire Wire Line
	7600 4200 7600 4150
Connection ~ 7600 4150
Connection ~ 7450 3700
Wire Wire Line
	7750 3750 7750 3500
Connection ~ 7750 3500
Connection ~ 7750 2050
Wire Wire Line
	7750 2300 7750 2050
Connection ~ 7450 2250
Connection ~ 7600 2700
Wire Wire Line
	7600 2750 7600 2700
Wire Wire Line
	5100 2200 5200 2200
Wire Wire Line
	6600 2400 6600 2150
Wire Wire Line
	6600 2150 6350 2150
Connection ~ 6500 2050
Wire Wire Line
	6350 2050 6800 2050
Wire Wire Line
	5200 2300 5150 2300
Wire Wire Line
	5150 2300 5150 2450
Wire Wire Line
	6350 2250 6800 2250
Connection ~ 6700 2250
Wire Wire Line
	5200 2100 5100 2100
Wire Wire Line
	7750 2700 7450 2700
Wire Wire Line
	7450 2300 7450 2250
Wire Wire Line
	3050 3650 3050 3800
Wire Wire Line
	3250 4100 3250 6500
Wire Wire Line
	3450 3900 3450 5000
Wire Wire Line
	7300 2050 8850 2050
Wire Wire Line
	8850 2050 8850 3900
Wire Wire Line
	8850 3900 8950 3900
Wire Wire Line
	7300 3500 8650 3500
Wire Wire Line
	8650 3500 8650 4100
Wire Wire Line
	8650 4100 8950 4100
Wire Wire Line
	7300 6650 8700 6650
Wire Wire Line
	8700 6650 8700 4600
Wire Wire Line
	8700 4600 8950 4600
Wire Wire Line
	7300 5150 8500 5150
Wire Wire Line
	8500 5150 8500 4400
Wire Wire Line
	8500 4400 8950 4400
Wire Wire Line
	1000 4100 1150 4100
Wire Wire Line
	9650 3800 9650 3850
Wire Wire Line
	2750 2100 4600 2100
Wire Wire Line
	3450 5000 4600 5000
Wire Wire Line
	3250 6500 4600 6500
Wire Wire Line
	6700 6650 6700 6050
Wire Wire Line
	6700 6050 3750 6050
Wire Wire Line
	3550 1450 3550 6250
Wire Wire Line
	3750 6050 3750 1450
Wire Wire Line
	3550 6250 4850 6250
Wire Wire Line
	4850 4900 4850 4800
Connection ~ 4850 4900
Wire Wire Line
	4850 4800 3850 4800
Wire Wire Line
	6700 5150 6700 4600
Wire Wire Line
	6700 4600 4050 4600
Wire Wire Line
	6500 3250 6500 3500
Wire Wire Line
	4850 3450 4850 3350
Connection ~ 4850 3450
Wire Wire Line
	4850 3350 4150 3350
Wire Wire Line
	6700 3150 4350 3150
Wire Wire Line
	3950 4700 3950 1450
Wire Wire Line
	4150 3350 4150 1450
Wire Wire Line
	4350 3150 4350 1450
Wire Wire Line
	4850 2000 4850 1850
Connection ~ 4850 2000
Wire Wire Line
	4850 1850 4450 1850
Wire Wire Line
	4450 1850 4450 1450
Wire Wire Line
	6700 2250 6700 1650
Wire Wire Line
	6700 1650 4650 1650
Wire Wire Line
	4650 1650 4650 1450
Wire Wire Line
	3550 950  3550 850 
Wire Wire Line
	3550 850  4900 850 
Wire Wire Line
	3750 950  3750 850 
Connection ~ 3750 850 
Wire Wire Line
	3850 850  3850 950 
Connection ~ 3850 850 
Wire Wire Line
	4050 950  4050 850 
Connection ~ 4050 850 
Wire Wire Line
	4250 950  4250 850 
Connection ~ 4250 850 
Wire Wire Line
	4450 950  4450 850 
Connection ~ 4450 850 
Wire Wire Line
	4650 950  4650 850 
Connection ~ 4650 850 
Wire Wire Line
	1600 6600 2050 6600
Wire Wire Line
	2050 7000 1600 7000
Wire Wire Line
	10300 5100 10300 5250
Wire Wire Line
	1150 4300 850  4300
Wire Wire Line
	850  4300 850  3150
Text Label 2600 3500 0    60   ~ 0
LED11
$Comp
L 74HC595-PWR U3
U 1 1 4F834E72
P 1850 3950
F 0 "U3" H 2000 4550 70  0000 C CNN
F 1 "74HCT595" H 1850 3350 70  0000 C CNN
	1    1850 3950
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR053
U 1 1 4F834CB1
P 850 3150
F 0 "#PWR053" H 850 3240 20  0001 C CNN
F 1 "+5V" H 850 3240 30  0000 C CNN
	1    850  3150
	1    0    0    -1  
$EndComp
Text Notes 950  4900 0    60   ~ 0
NXP 74HCT595 will source up to 8mA to each output\nand seems to be 5V/3V compatible.
Text HLabel 2550 4400 2    60   Output ~ 0
SHIFT_OUT
$Comp
L +3.3V #PWR054
U 1 1 4F829FD3
P 10300 5100
F 0 "#PWR054" H 10300 5060 30  0001 C CNN
F 1 "+3.3V" H 10300 5210 30  0000 C CNN
	1    10300 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR055
U 1 1 4F829FCF
P 10300 5800
F 0 "#PWR055" H 10300 5800 30  0001 C CNN
F 1 "GND" H 10300 5730 30  0001 C CNN
	1    10300 5800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR056
U 1 1 4F829EA4
P 1850 7150
F 0 "#PWR056" H 1850 7150 30  0001 C CNN
F 1 "GND" H 1850 7080 30  0001 C CNN
	1    1850 7150
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR057
U 1 1 4F829EA0
P 1850 6500
F 0 "#PWR057" H 1850 6590 20  0001 C CNN
F 1 "+5V" H 1850 6590 30  0000 C CNN
	1    1850 6500
	1    0    0    -1  
$EndComp
Text Notes 1000 6250 0    60   ~ 0
Dimension decoupling for shifting (MHz)\nplus LED power (64mA spikes)
$Comp
L R RPU12
U 1 1 4F828559
P 4650 1200
F 0 "RPU12" V 4730 1200 50  0000 C CNN
F 1 "20k" V 4650 1200 50  0000 C CNN
	1    4650 1200
	1    0    0    -1  
$EndComp
$Comp
L R RPU11
U 1 1 4F828556
P 4550 1200
F 0 "RPU11" V 4630 1200 50  0000 C CNN
F 1 "20k" V 4550 1200 50  0000 C CNN
	1    4550 1200
	1    0    0    -1  
$EndComp
$Comp
L R RPU10
U 1 1 4F828551
P 4450 1200
F 0 "RPU10" V 4530 1200 50  0000 C CNN
F 1 "20k" V 4450 1200 50  0000 C CNN
	1    4450 1200
	1    0    0    -1  
$EndComp
$Comp
L R RPU9
U 1 1 4F82854F
P 4350 1200
F 0 "RPU9" V 4430 1200 50  0000 C CNN
F 1 "20k" V 4350 1200 50  0000 C CNN
	1    4350 1200
	1    0    0    -1  
$EndComp
$Comp
L R RPU8
U 1 1 4F82854D
P 4250 1200
F 0 "RPU8" V 4330 1200 50  0000 C CNN
F 1 "20k" V 4250 1200 50  0000 C CNN
	1    4250 1200
	1    0    0    -1  
$EndComp
$Comp
L R RPU7
U 1 1 4F82854B
P 4150 1200
F 0 "RPU7" V 4230 1200 50  0000 C CNN
F 1 "20k" V 4150 1200 50  0000 C CNN
	1    4150 1200
	1    0    0    -1  
$EndComp
$Comp
L R RPU6
U 1 1 4F828544
P 4050 1200
F 0 "RPU6" V 4130 1200 50  0000 C CNN
F 1 "20k" V 4050 1200 50  0000 C CNN
	1    4050 1200
	1    0    0    -1  
$EndComp
$Comp
L R RPU5
U 1 1 4F828493
P 3950 1200
F 0 "RPU5" V 4030 1200 50  0000 C CNN
F 1 "20k" V 3950 1200 50  0000 C CNN
	1    3950 1200
	1    0    0    -1  
$EndComp
$Comp
L R RPU1
U 1 1 4F828486
P 3850 1200
AR Path="/4F7E05A5/4F82845B" Ref="RPU1"  Part="1" 
AR Path="/4F7E05A5/4F828486" Ref="RPU4"  Part="1" 
F 0 "RPU4" V 3930 1200 50  0000 C CNN
F 1 "20k" V 3850 1200 50  0000 C CNN
	1    3850 1200
	1    0    0    -1  
$EndComp
$Comp
L R RPU1
U 1 1 4F828480
P 3750 1200
AR Path="/4F7E05A5/4F82845B" Ref="RPU1"  Part="1" 
AR Path="/4F7E05A5/4F828480" Ref="RPU3"  Part="1" 
F 0 "RPU3" V 3830 1200 50  0000 C CNN
F 1 "20k" V 3750 1200 50  0000 C CNN
	1    3750 1200
	1    0    0    -1  
$EndComp
$Comp
L R RPU1
U 1 1 4F828479
P 3650 1200
AR Path="/4F7E05A5/4F82845B" Ref="RPU1"  Part="1" 
AR Path="/4F7E05A5/4F828479" Ref="RPU2"  Part="1" 
F 0 "RPU2" V 3730 1200 50  0000 C CNN
F 1 "20k" V 3650 1200 50  0000 C CNN
	1    3650 1200
	1    0    0    -1  
$EndComp
$Comp
L R RPU1
U 1 1 4F82845B
P 3550 1200
F 0 "RPU1" V 3630 1200 50  0000 C CNN
F 1 "20k" V 3550 1200 50  0000 C CNN
	1    3550 1200
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 4F81BBD2
P 10300 5450
F 0 "C6" H 10350 5550 50  0000 L CNN
F 1 "100n" H 10350 5350 50  0000 L CNN
	1    10300 5450
	1    0    0    -1  
$EndComp
Text Notes 500  1100 0    60   ~ 0
Resistor pack BI Technologies 628A103\navailable in 220 mil chip (SO16N but slightly wider),\n0.56€ @ Mouser.\nType A is 8 parallel (0.07€ a pop),\ntype B is pullup/downs (0.037€ a pop).\nMouser 1206 resistors (667-ERJ-P08J103V) are 2€ for 100.\n0805 (667-ERJ-6GEYJ103V) are 1€ for 100.
$Comp
L +3.3V #PWR058
U 1 1 4F8131F4
P 8950 4750
F 0 "#PWR058" H 8950 4710 30  0001 C CNN
F 1 "+3.3V" H 8950 4860 30  0000 C CNN
	1    8950 4750
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR059
U 1 1 4F8131EC
P 9650 3800
F 0 "#PWR059" H 9650 3760 30  0001 C CNN
F 1 "+3.3V" H 9650 3910 30  0000 C CNN
	1    9650 3800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR060
U 1 1 4F8131C7
P 9650 5100
F 0 "#PWR060" H 9650 5100 30  0001 C CNN
F 1 "GND" H 9650 5030 30  0001 C CNN
	1    9650 5100
	1    0    0    -1  
$EndComp
Text Notes 9100 3650 0    60   ~ 0
74245: pull DIR (A->B) high.
$Comp
L 74LS245 U8
U 1 1 4F812DEE
P 9650 4400
F 0 "U8" H 9750 4975 60  0000 L BNN
F 1 "74HC245" H 9700 3825 60  0000 L TNN
	1    9650 4400
	1    0    0    -1  
$EndComp
Text Notes 950  5200 0    60   ~ 0
Needs to be 5V/3V compatible --> use HCT
$Comp
L GND #PWR061
U 1 1 4F803213
P 1000 4700
F 0 "#PWR061" H 1000 4700 30  0001 C CNN
F 1 "GND" H 1000 4630 30  0001 C CNN
	1    1000 4700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR062
U 1 1 4F803206
P 5150 5350
F 0 "#PWR062" H 5150 5350 30  0001 C CNN
F 1 "GND" H 5150 5280 30  0001 C CNN
	1    5150 5350
	1    0    0    -1  
$EndComp
Text GLabel 4900 850  2    60   Input ~ 0
VPULLUP
Text GLabel 10350 4600 2    60   3State ~ 0
BUS_7
Text GLabel 10350 4500 2    60   3State ~ 0
BUS_6
Text GLabel 10350 4400 2    60   3State ~ 0
BUS_5
Text GLabel 10350 4300 2    60   3State ~ 0
BUS_4
Text GLabel 10350 4200 2    60   3State ~ 0
BUS_3
Text GLabel 10350 4100 2    60   3State ~ 0
BUS_2
Text GLabel 10350 4000 2    60   3State ~ 0
BUS_1
Text GLabel 10350 3900 2    60   3State ~ 0
BUS_0
Text HLabel 8900 5200 3    60   Input ~ 0
BUF_OE
Text HLabel 4550 6400 0    60   Output ~ 0
SW4
Text HLabel 4500 4900 0    60   Output ~ 0
SW3
Text HLabel 4500 3450 0    60   Output ~ 0
SW2
Text HLabel 4650 2000 0    60   Output ~ 0
SW1
$Comp
L C C11
U 1 1 4F7F1642
P 1600 6800
F 0 "C11" H 1650 6900 50  0000 L CNN
F 1 "100n" H 1650 6700 50  0000 L CNN
	1    1600 6800
	1    0    0    -1  
$EndComp
Text HLabel 1150 4000 0    60   Input ~ 0
SHIFT_LATCH
Text HLabel 1150 3700 0    60   Input ~ 0
SHIFT_CLK
Text HLabel 1150 3500 0    60   Input ~ 0
SHIFT_IN
$Comp
L PEL12D ENC1
U 1 1 4F7E098E
P 5800 2150
F 0 "ENC1" H 5800 1800 60  0000 C CNN
F 1 "PEL12D" H 5800 2400 60  0000 C CNN
	1    5800 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR063
U 1 1 4F7E098D
P 5150 2450
F 0 "#PWR063" H 5150 2450 30  0001 C CNN
F 1 "GND" H 5150 2380 30  0001 C CNN
	1    5150 2450
	1    0    0    -1  
$EndComp
$Comp
L R RK12
U 1 1 4F7E098B
P 4850 2100
F 0 "RK12" V 4930 2100 50  0000 C CNN
F 1 "390" V 4850 2100 50  0000 C CNN
	1    4850 2100
	0    1    1    0   
$EndComp
$Comp
L R RK13
U 1 1 4F7E098A
P 4850 2200
F 0 "RK13" V 4930 2200 50  0000 C CNN
F 1 "240" V 4850 2200 50  0000 C CNN
	1    4850 2200
	0    1    1    0   
$EndComp
$Comp
L GND #PWR064
U 1 1 4F7E0987
P 6600 2400
F 0 "#PWR064" H 6600 2400 30  0001 C CNN
F 1 "GND" H 6600 2330 30  0001 C CNN
	1    6600 2400
	1    0    0    -1  
$EndComp
$Comp
L R RK16
U 1 1 4F7E0984
P 7050 2050
F 0 "RK16" V 7130 2050 50  0000 C CNN
F 1 "20k" V 7050 2050 50  0000 C CNN
	1    7050 2050
	0    1    1    0   
$EndComp
$Comp
L R RK17
U 1 1 4F7E0983
P 7050 2250
F 0 "RK17" V 7130 2250 50  0000 C CNN
F 1 "20k" V 7050 2250 50  0000 C CNN
	1    7050 2250
	0    1    1    0   
$EndComp
$Comp
L C CK11
U 1 1 4F7E0982
P 7450 2500
F 0 "CK11" H 7500 2600 50  0000 L CNN
F 1 "10n" H 7500 2400 50  0000 L CNN
	1    7450 2500
	1    0    0    -1  
$EndComp
$Comp
L C CK12
U 1 1 4F7E0981
P 7750 2500
F 0 "CK12" H 7800 2600 50  0000 L CNN
F 1 "10n" H 7800 2400 50  0000 L CNN
	1    7750 2500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR065
U 1 1 4F7E0980
P 7600 2750
F 0 "#PWR065" H 7600 2750 30  0001 C CNN
F 1 "GND" H 7600 2680 30  0001 C CNN
	1    7600 2750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR066
U 1 1 4F7E097D
P 7600 4200
F 0 "#PWR066" H 7600 4200 30  0001 C CNN
F 1 "GND" H 7600 4130 30  0001 C CNN
	1    7600 4200
	1    0    0    -1  
$EndComp
$Comp
L C CK22
U 1 1 4F7E097C
P 7750 3950
F 0 "CK22" H 7800 4050 50  0000 L CNN
F 1 "10n" H 7800 3850 50  0000 L CNN
	1    7750 3950
	1    0    0    -1  
$EndComp
$Comp
L C CK21
U 1 1 4F7E097B
P 7450 3950
F 0 "CK21" H 7500 4050 50  0000 L CNN
F 1 "10n" H 7500 3850 50  0000 L CNN
	1    7450 3950
	1    0    0    -1  
$EndComp
$Comp
L R RK27
U 1 1 4F7E097A
P 7050 3700
F 0 "RK27" V 7130 3700 50  0000 C CNN
F 1 "20k" V 7050 3700 50  0000 C CNN
	1    7050 3700
	0    1    1    0   
$EndComp
$Comp
L R RK26
U 1 1 4F7E0979
P 7050 3500
F 0 "RK26" V 7130 3500 50  0000 C CNN
F 1 "20k" V 7050 3500 50  0000 C CNN
	1    7050 3500
	0    1    1    0   
$EndComp
$Comp
L GND #PWR067
U 1 1 4F7E0976
P 6600 3850
F 0 "#PWR067" H 6600 3850 30  0001 C CNN
F 1 "GND" H 6600 3780 30  0001 C CNN
	1    6600 3850
	1    0    0    -1  
$EndComp
$Comp
L R RK23
U 1 1 4F7E0973
P 4850 3650
F 0 "RK23" V 4930 3650 50  0000 C CNN
F 1 "240" V 4850 3650 50  0000 C CNN
	1    4850 3650
	0    1    1    0   
$EndComp
$Comp
L R RK22
U 1 1 4F7E0972
P 4850 3550
F 0 "RK22" V 4930 3550 50  0000 C CNN
F 1 "390" V 4850 3550 50  0000 C CNN
	1    4850 3550
	0    1    1    0   
$EndComp
$Comp
L GND #PWR068
U 1 1 4F7E0970
P 5150 3900
F 0 "#PWR068" H 5150 3900 30  0001 C CNN
F 1 "GND" H 5150 3830 30  0001 C CNN
	1    5150 3900
	1    0    0    -1  
$EndComp
$Comp
L PEL12D ENC2
U 1 1 4F7E096F
P 5800 3600
F 0 "ENC2" H 5800 3250 60  0000 C CNN
F 1 "PEL12D" H 5800 3850 60  0000 C CNN
	1    5800 3600
	1    0    0    -1  
$EndComp
$Comp
L PEL12D ENC3
U 1 1 4F7E096E
P 5800 5050
F 0 "ENC3" H 5800 4700 60  0000 C CNN
F 1 "PEL12D" H 5800 5300 60  0000 C CNN
	1    5800 5050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR069
U 1 1 4F7E096D
P 5150 5350
F 0 "#PWR069" H 5150 5350 30  0001 C CNN
F 1 "GND" H 5150 5280 30  0001 C CNN
	1    5150 5350
	1    0    0    -1  
$EndComp
$Comp
L R RK32
U 1 1 4F7E096B
P 4850 5000
F 0 "RK32" V 4930 5000 50  0000 C CNN
F 1 "390" V 4850 5000 50  0000 C CNN
	1    4850 5000
	0    1    1    0   
$EndComp
$Comp
L R RK33
U 1 1 4F7E096A
P 4850 5100
F 0 "RK33" V 4930 5100 50  0000 C CNN
F 1 "240" V 4850 5100 50  0000 C CNN
	1    4850 5100
	0    1    1    0   
$EndComp
$Comp
L GND #PWR070
U 1 1 4F7E0967
P 6600 5300
F 0 "#PWR070" H 6600 5300 30  0001 C CNN
F 1 "GND" H 6600 5230 30  0001 C CNN
	1    6600 5300
	1    0    0    -1  
$EndComp
$Comp
L R RK36
U 1 1 4F7E0964
P 7050 4950
F 0 "RK36" V 7130 4950 50  0000 C CNN
F 1 "20k" V 7050 4950 50  0000 C CNN
	1    7050 4950
	0    1    1    0   
$EndComp
$Comp
L R RK37
U 1 1 4F7E0963
P 7050 5150
F 0 "RK37" V 7130 5150 50  0000 C CNN
F 1 "20k" V 7050 5150 50  0000 C CNN
	1    7050 5150
	0    1    1    0   
$EndComp
$Comp
L C CK31
U 1 1 4F7E0962
P 7450 5400
F 0 "CK31" H 7500 5500 50  0000 L CNN
F 1 "10n" H 7500 5300 50  0000 L CNN
	1    7450 5400
	1    0    0    -1  
$EndComp
$Comp
L C CK32
U 1 1 4F7E0961
P 7750 5400
F 0 "CK32" H 7800 5500 50  0000 L CNN
F 1 "10n" H 7800 5300 50  0000 L CNN
	1    7750 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR071
U 1 1 4F7E0960
P 7600 5650
F 0 "#PWR071" H 7600 5650 30  0001 C CNN
F 1 "GND" H 7600 5580 30  0001 C CNN
	1    7600 5650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR072
U 1 1 4F7E095D
P 7600 7150
F 0 "#PWR072" H 7600 7150 30  0001 C CNN
F 1 "GND" H 7600 7080 30  0001 C CNN
	1    7600 7150
	1    0    0    -1  
$EndComp
$Comp
L C CK42
U 1 1 4F7E095C
P 7750 6900
F 0 "CK42" H 7800 7000 50  0000 L CNN
F 1 "10n" H 7800 6800 50  0000 L CNN
	1    7750 6900
	1    0    0    -1  
$EndComp
$Comp
L C CK41
U 1 1 4F7E095B
P 7450 6900
F 0 "CK41" H 7500 7000 50  0000 L CNN
F 1 "10n" H 7500 6800 50  0000 L CNN
	1    7450 6900
	1    0    0    -1  
$EndComp
$Comp
L R RK47
U 1 1 4F7E095A
P 7050 6650
F 0 "RK47" V 7130 6650 50  0000 C CNN
F 1 "20k" V 7050 6650 50  0000 C CNN
	1    7050 6650
	0    1    1    0   
$EndComp
$Comp
L R RK46
U 1 1 4F7E0959
P 7050 6450
F 0 "RK46" V 7130 6450 50  0000 C CNN
F 1 "20k" V 7050 6450 50  0000 C CNN
	1    7050 6450
	0    1    1    0   
$EndComp
$Comp
L GND #PWR073
U 1 1 4F7E0956
P 6600 6800
F 0 "#PWR073" H 6600 6800 30  0001 C CNN
F 1 "GND" H 6600 6730 30  0001 C CNN
	1    6600 6800
	1    0    0    -1  
$EndComp
$Comp
L R RK43
U 1 1 4F7E0953
P 4850 6600
F 0 "RK43" V 4930 6600 50  0000 C CNN
F 1 "240" V 4850 6600 50  0000 C CNN
	1    4850 6600
	0    1    1    0   
$EndComp
$Comp
L R RK42
U 1 1 4F7E0952
P 4850 6500
F 0 "RK42" V 4930 6500 50  0000 C CNN
F 1 "390" V 4850 6500 50  0000 C CNN
	1    4850 6500
	0    1    1    0   
$EndComp
$Comp
L GND #PWR074
U 1 1 4F7E0950
P 5150 6850
F 0 "#PWR074" H 5150 6850 30  0001 C CNN
F 1 "GND" H 5150 6780 30  0001 C CNN
	1    5150 6850
	1    0    0    -1  
$EndComp
$Comp
L PEL12D ENC4
U 1 1 4F7E094F
P 5800 6550
F 0 "ENC4" H 5800 6200 60  0000 C CNN
F 1 "PEL12D" H 5800 6800 60  0000 C CNN
	1    5800 6550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
